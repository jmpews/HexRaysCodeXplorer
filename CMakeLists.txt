cmake_minimum_required(VERSION 3.1)
cmake_policy(SET CMP0054 NEW)

project(HexRaysCodeXplorer)

include(cmake/IDA.cmake)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


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