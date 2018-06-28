cmake_minimum_required(VERSION 3.1)
cmake_policy(SET CMP0054 NEW)
# set(CMAKE_MACOSX_RPATH 0)
# set(CMAKE_BUILD_WITH_INSTALL_RPATH 0)
# set(CMAKE_BUILD_WITH_INSTALL_NAME_DIR 0)


project(HexRaysCodeXplorer)

SET(CMAKE_C_COMPILER "/usr/local/Cellar/gcc/8.1.0/bin/gcc-8")
SET(CMAKE_CXX_COMPILER "/usr/local/Cellar/gcc/8.1.0/bin/g++-8")

include(cmake/IDA.cmake)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")

# already fix Common.h wrong usage
# add_definitions(-DUSE_DANGEROUS_FUNCTIONS=1)
# add_definitions(-DUSE_STANDARD_FILE_FUNCTIONS=1)


set(HexRaysCodeXplorer.path ./src/HexRaysCodeXplorer)
set(GLOBAL.SOURCE_FILE "")

set(sources
    ${HexRaysCodeXplorer.path}/Debug.cpp
    "${HexRaysCodeXplorer.path}/CtreeExtractor.cpp"
    "${HexRaysCodeXplorer.path}/GCCObjectFormatParser.cpp"
    "${HexRaysCodeXplorer.path}/GCCTypeInfo.cpp"
    "${HexRaysCodeXplorer.path}/GCCVtableInfo.cpp"
    "${HexRaysCodeXplorer.path}/IObjectFormatParser.cpp"
    "${HexRaysCodeXplorer.path}/MSVCObjectFormatParser.cpp"
    "${HexRaysCodeXplorer.path}/ObjectExplorer.cpp"
    "${HexRaysCodeXplorer.path}/CodeXplorer.cpp"
    "${HexRaysCodeXplorer.path}/CtreeGraphBuilder.cpp"
    "${HexRaysCodeXplorer.path}/TypeReconstructor.cpp"
    "${HexRaysCodeXplorer.path}/TypeExtractor.cpp"
    "${HexRaysCodeXplorer.path}/Utility.cpp"
    )

add_ida_plugin(HexRaysCodeXplorer ${sources})
