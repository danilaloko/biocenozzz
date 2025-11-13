cmake_minimum_required(VERSION 3.16)
project(biocenozzz)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 REQUIRED COMPONENTS 
    Core
)

set(CMAKE_AUTOMOC ON)

file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
    "src/*.c"
    "src/core/*.cpp"
    "src/core/*.c"
    "src/core/SpatialSubdivision/*.cpp"
    "src/core/SpatialSubdivision/*.c"
)

file(GLOB_RECURSE HEADERS 
    "src/*.h"
    "src/*.hpp"
    "src/core/*.h"
    "src/core/*.hpp"
    "src/core/SpatialSubdivision/*.h"
    "src/core/SpatialSubdivision/*.hpp"
)

list(FILTER SOURCES EXCLUDE REGEX ".*3rd-party.*")
list(FILTER HEADERS EXCLUDE REGEX ".*3rd-party.*")

add_subdirectory(src/3rd-party/plog)
add_subdirectory(src/3rd-party/httplib)
add_subdirectory(src/3rd-party/nlohmann_json)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(biocenozzz 
    Qt5::Core
    plog::plog
    httplib
    nlohmann_json
    #pthread
)

target_include_directories(biocenozzz PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)