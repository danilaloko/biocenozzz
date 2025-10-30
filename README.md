# Biocenozzz
## Building

### Dependencies
* CMake 3.16 or higher
* Qt5 (Core)
* C++17 compatible compiler

### Build via cli
```
git submodule update --init --recursive
mkdir build
cd build
cmake .. -DCMAKE_PREFIX_PATH=<PATH_TO_QT>\msvc2019_64\lib\cmake\Qt5
cmake --build . --config Release
```
