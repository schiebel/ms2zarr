#include <tensorstore/tensorstore.h>
#include "tensorstore/index_space/dim_expression.h"
#include <tensorstore/json_serialization_options.h>
#include <tensorstore/driver/driver_spec.h>
#include <tensorstore/open.h>
#include <tensorstore/array.h>
#include <tensorstore/spec.h>
#include <tensorstore/box.h>
#include <tensorstore/index_space/index_domain.h>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <cstdlib>

#include <casacore/tables/Tables/Table.h>
#include <casacore/tables/Tables/ArrayColumn.h>
#include <casacore/casa/Arrays/Array.h>
#include <casacore/casa/Exceptions.h>
#include <casacore/casa/Arrays/IPosition.h>

template <typename T>
size_t elsize(const casacore::Array<T>& arr) { return sizeof(T); }

using namespace casacore;
namespace stdfs = std::filesystem;
using std::string;
using std::min;

size_t guess_chunk_rows( size_t rows, size_t row_bytes, size_t max_chunk_bytes ) {

    if ( row_bytes == 0 || rows == 0 ) return 0;
    if ( row_bytes >= max_chunk_bytes ) return 1;

    size_t all_bytes = 0;
    if ( ! __builtin_mul_overflow(rows, row_bytes, &all_bytes) &&
         all_bytes < max_chunk_bytes ) {
        size_t ret = rows / 2;
        while ( (ret+1) * row_bytes * 2 < all_bytes ) ret += 1;
        return ret;
    }

    auto compute_chunks = [&]( size_t current_rows, auto&& self ) -> size_t { // Note the auto&& self
        if ( current_rows == 0 ) return 0;

        size_t max_rows_for_chunk = max_chunk_bytes / row_bytes;

        if ( current_rows <= max_rows_for_chunk ) {
            while ( (current_rows+1) * row_bytes < max_chunk_bytes ) current_rows += 1;
            return current_rows;
        } else {
            return self( current_rows / 2, self ); // Recursive call using self
        }
    };

    return compute_chunks( rows, compute_chunks );
}

int remove_directory(const char *path) {
    DIR *d = opendir(path);
    size_t path_len;
    int r = -1;

    if (d) {
        struct dirent *p;
        path_len = strlen(path);
        r = 0;
        while (!r && (p = readdir(d))) {
            int r2 = -1;
            char *buf;
            size_t len;

            /* Skip the names "." and ".." as we don't want to recurse on them. */
            if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
                continue;

            len = path_len + strlen(p->d_name) + 2;
            buf = (char*) malloc(len);

            if (buf) {
                struct stat statbuf;
                snprintf(buf, len, "%s/%s", path, p->d_name);
                if (!stat(buf, &statbuf)) {
                    if (S_ISDIR(statbuf.st_mode))
                        r2 = remove_directory(buf);
                    else
                        r2 = remove(buf);
                }
                free(buf);
            }
            r = r2;
        }
        closedir(d);
    }

    if (!r)
        r = rmdir(path);

    return r;
}

int main() {
    // Clean up existing directory to avoid conflicts
    remove_directory("tCtds2Zarr.zarr3");

    try{
        // Open the MeasurementSet
        char *datadir = getenv("testsrcdir");
        
        Table msTable(( stdfs::path(datadir ? datadir : "data") /
                        stdfs::path("ea25_cal_small_before_fixed.split.ms")
                      ).string(), Table::Old);

        if (!msTable.tableDesc().isColumn("UVW")) {
            std::cerr << "Error: UVW column not found in table." << std::endl;
            return 1;
        }

        // Access the UVW column as an ArrayColumn of Array<Double>
        ArrayColumn<Double> uvwCol(msTable, "UVW");

        constexpr size_t maxchunk = 1024*1024*20;
        size_t row_bytes = elsize(uvwCol((rownr_t)0)) * uvwCol((rownr_t)0).size();

        size_t total_rows = uvwCol.nrow( );
        size_t total_cols = uvwCol((rownr_t)0).size();

        std::cout << "                    rows: " << total_rows << std::endl;
        std::cout << "            row elements: " << total_cols << std::endl;
        std::cout << "        row element size: " << elsize(uvwCol((rownr_t)0)) << std::endl;
        std::cout << "               row bytes: " << row_bytes << std::endl;

        size_t entire_size = total_rows * row_bytes;
        std::cout << "        entire size (mb): " << ((double) entire_size / (1024*1024))  << std::endl;
        std::cout << "              chunks (b): " << ((double) entire_size / (1024*1024))  << std::endl;

        size_t chunk_size = maxchunk;
        size_t chunks = (double)total_rows * (double)row_bytes / (double)maxchunk;
        if ( chunks < 2 ) {
            chunks = 2;
            chunk_size = entire_size / 2;
        }

        std::cout << "              chunks (b): " << chunks  << std::endl;
        std::cout << "          chunk size (b): " << chunk_size  << std::endl;

        size_t rows_per_chunk = guess_chunk_rows( total_rows, row_bytes, maxchunk );
        std::cout << "          rows per chunk: " << rows_per_chunk << " [bytes: " <<
            (rows_per_chunk*row_bytes) << "] [slop: " <<
            (maxchunk - rows_per_chunk*row_bytes) << "]" << std::endl;

        // Process the UVW data
        std::cout << "First ten UVW rows:" << std::endl;
        std::cout << "--------------------------------------" << std::endl;
        for (size_t i = 0; i < min((size_t) uvwCol.nrow(), (size_t) 10); ++i) {

            // Access the Array<Double> and its elements using IPosition
            std::cout << "UVW[" << i << "]: " << uvwCol((rownr_t)i) << std::endl;
        }
        std::cout << "--------------------------------------" << std::endl;

        if ( total_rows <= 0 || total_cols <= 0 ) {
            std::cerr << "Invalid number of rows (" << total_rows << ") or columns (" << total_cols << ")" << std::endl;
            return 1;
        }

        size_t chunk_rows = rows_per_chunk;
        size_t chunk_cols = total_cols;
  
        // Create the spec - make sure data types match!
        ::nlohmann::json spec_json = {
            {"driver", "zarr3"},
            {"kvstore", {
                {"driver", "file"},
                {"path", "tCtds2Zarr.zarr3"}
            }},
            {"path", "UVW"},
            {"schema", {
                {"dtype", "float64"},  // Match your store type
                    {"domain", {
                        {"shape", {total_rows, total_cols}}
                    }}
            }},
            {"metadata", {
                {"chunk_grid", {
                    {"name", "regular"},
                    {"configuration", {
                        {"chunk_shape", {chunk_rows, chunk_cols}}
                    }}
                }}
            }},
            {"open", true},
            {"create", true}
        };
  
        // Convert JSON to Spec
        auto spec_result = tensorstore::Spec::FromJson(spec_json);
        if (!spec_result.ok()) {
            std::cerr << "Error creating spec: " << spec_result.status() << std::endl;
            return 1;
        }
  
        // Open with matching data type
        auto result = tensorstore::Open<double, 2, tensorstore::ReadWriteMode::dynamic>(
                          spec_result.value()
                      ).result();
  
        if (!result.ok()) {
            std::cerr << "Error opening TensorStore: " << result.status() << std::endl;
            return 1;
        }
  
        auto store = result.value();
        std::cout << "Successfully opened TensorStore!" << std::endl;
        std::cout << "Domain: " << store.domain() << std::endl;
        std::cout << "Dtype: " << store.dtype() << std::endl;
  
        for (int row = 0; row < total_rows; row += chunk_rows) {
            for (int col = 0; col < total_cols; col += chunk_cols) {
      
                // Calculate actual chunk size (handle edge cases)...
                int actual_chunk_rows = std::min(chunk_rows, total_rows - row);
                int actual_chunk_cols = std::min(chunk_cols, total_cols - col);
      
                // Create chunk array with matching data type...
                auto chunk_array = tensorstore::AllocateArray<double>({actual_chunk_rows, actual_chunk_cols});
      
                // Fill chunk with data from the column...
                for (int i = 0; i < actual_chunk_rows; ++i) {
                    auto current_row = uvwCol((rownr_t)i);
                    for (int j = 0; j < actual_chunk_cols; ++j) {
                        chunk_array(i, j) = static_cast<double>(current_row(IPosition(1,j)));
                    }
                }

                // Tested with tensorstore v0.1.75:
                // https://github.com/google/tensorstore/releases/tag/v0.1.75
                try {
                    auto write_future = tensorstore::Write(
                        chunk_array,
                        store | tensorstore::Dims(0, 1).TranslateSizedInterval(
                            {row, col}, {actual_chunk_rows, actual_chunk_cols}
                        )
                    );

                    auto write_result = write_future.result();
                    if (!write_result.ok()) {
                        std::cerr << "Error writing chunk at (" << row << ", " << col << "): "
                                  << write_result.status() << std::endl;
                        continue;
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Exception writing chunk at (" << row << ", " << col << "): "
                              << e.what() << std::endl;
                    continue;
                }
      
                std::cout << "Written chunk (" << row << ", " << col << ") -> (" 
                          << row + actual_chunk_rows - 1 << ", " << col + actual_chunk_cols - 1 << ")" << std::endl;
            }
    }
  

  
    std::cout << "Successfully wrote all chunks to TensorStore!" << std::endl;
  
    // Try to verify the first 10 rows...
    std::cout << "--------------------------------------" << std::endl;
    try {
        auto read_future = tensorstore::Read(
            store | tensorstore::Dims(0, 1).TranslateSizedInterval({0, 0}, {10, 3})
        );
        auto read_result = read_future.result();
    
        if (read_result.ok()) {
            auto read_array = read_result.value();
            std::cout << "Verification: First 10x3 values:" << std::endl;
            for (int i = 0; i < 10; ++i) {
                for (int j = 0; j < 3; ++j) {
                    std::cout << read_array(i, j) << " ";
                }
                std::cout << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cout << "Could not verify data: " << e.what() << std::endl;
    }

    } catch (const AipsError &e) {
        std::cerr << "Casacore AipsError: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
