#include "tensorstore/tensorstore.h"
#include "tensorstore/context.h"
#include <iostream>

int main() {
    auto context = tensorstore::Context::Default();
    
    // Test Zarr v2
    auto zarr2_spec = tensorstore::Spec::FromJson({
        {"driver", "zarr"},
        {"kvstore", {{"driver", "file"}, {"path", "/tmp/test_zarr2.zarr"}}}
    });
    
    // Test Zarr v3  
    auto zarr3_spec = tensorstore::Spec::FromJson({
        {"driver", "zarr3"},
        {"kvstore", {{"driver", "file"}, {"path", "/tmp/test_zarr3.zarr"}}}
    });
    
    std::cout << "Zarr v2 available: " << (zarr2_spec.ok() ? "YES" : "NO") << std::endl;
    std::cout << "Zarr v3 available: " << (zarr3_spec.ok() ? "YES" : "NO") << std::endl;
    
    return 0;
}
