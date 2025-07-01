# ms2zarr

## Introduction
This is not a complete casacore ms to zarr converter at this point, but rather a proof of concept.
It builds the table system portion of the casacore library, abseil and tensorstore (v0.1.75). The
version of abseil is selected to match the version used with tensorstore. This is built with a
conda environment created with:

```
conda create --name zarr-c++ts 'python>3.11.13' cpp-half 'nasm=2.16.03=h4bc722e_0' nlohmann_json grpcio grpcio-tools bazel numpy pkg-config
```
The versions used are:

*  **casacore -** [8e2648a90745a6cd82fd64dbe8662a968b81b54e](https://github.com/casacore/casacore/commit/8e2648a90745a6cd82fd64dbe8662a968b81b54e)
*  **tensorstore -** [v0.1.75](https://github.com/google/tensorstore/releases/tag/v0.1.75)
*  **abseil -** [5f3435aba00bcd7f12062d2e8e1839b4eaf1a575](https://github.com/abseil/abseil-cpp/commit/5f3435aba00bcd7f12062d2e8e1839b4eaf1a575)

## Build targets

Build the libraries for test:

```
bash$ make
```

Run casacore table tests:
```
bash$ make test-ctds
```

Build test for Zarr 3 availability:
```
bash$ make tInfoZarr
```

Build test for Zarr 3 chunked reading and writing:
```
bash$ make tLargeZarr
```

Build test for reading UVW data from MS and writing to Zarr 3 group:
```
bash$ make tCtds2Zarr
```