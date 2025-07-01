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
  remove_directory("tLargeZarr.zarr3");
  
  int total_rows = 1000;
  int total_cols = 2000;
  int chunk_rows = 100;
  int chunk_cols = 200;
  
  // Create the spec - make sure data types match!
  ::nlohmann::json spec_json = {
    {"driver", "zarr3"},
    {"kvstore", {
      {"driver", "file"},
      {"path", "tLargeZarr.zarr3"}
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
  
  // Write data in chunks - use matching data type
  for (int row = 0; row < total_rows; row += chunk_rows) {
    for (int col = 0; col < total_cols; col += chunk_cols) {
      
      // Calculate actual chunk size (handle edge cases)
      int actual_chunk_rows = std::min(chunk_rows, total_rows - row);
      int actual_chunk_cols = std::min(chunk_cols, total_cols - col);
      
      // Create chunk array with matching data type
      auto chunk_array = tensorstore::AllocateArray<double>({actual_chunk_rows, actual_chunk_cols});
      
      // Fill with some test data
      for (int i = 0; i < actual_chunk_rows; ++i) {
        for (int j = 0; j < actual_chunk_cols; ++j) {
          chunk_array(i, j) = static_cast<double>((row + i) * total_cols + (col + j));
        }
      }
      
      // Use Write with explicit index specification - correct for v0.1.75
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
  
  // Optional: Verify the data was written by reading back a small portion
  try {
    auto read_future = tensorstore::Read(
      store | tensorstore::Dims(0, 1).TranslateSizedInterval({0, 0}, {10, 10})
    );
    auto read_result = read_future.result();
    
    if (read_result.ok()) {
      auto read_array = read_result.value();
      std::cout << "Verification: First 10x10 values:" << std::endl;
      for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
          std::cout << read_array(i, j) << " ";
        }
        std::cout << std::endl;
      }
    }
  } catch (const std::exception& e) {
    std::cout << "Could not verify data: " << e.what() << std::endl;
  }
  
  return 0;
}
