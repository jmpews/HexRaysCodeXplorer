cmake_minimum_required(VERSION 3.1)
project(HexRaysCodeXplorer)

# macOS compile:
# 1. install gcc
# brew install gcc@7

# 2. close below comments
# SET(CMAKE_C_COMPILER "/usr/local/bin/gcc-7")
# SET(CMAKE_CXX_COMPILER "/usr/local/bin/g++-7")

# linux commpile: the same to above

include("ida-cmake/cmake/IDA.cmake")

set(sources "src/HexRaysCodeXplorer/Debug.cpp"
			"src/HexRaysCodeXplorer/CtreeExtractor.cpp"
			"src/HexRaysCodeXplorer/GCCObjectFormatParser.cpp"
			"src/HexRaysCodeXplorer/GCCTypeInfo.cpp"
			"src/HexRaysCodeXplorer/GCCVtableInfo.cpp"
			"src/HexRaysCodeXplorer/IObjectFormatParser.cpp" 
			"src/HexRaysCodeXplorer/MSVCObjectFormatParser.cpp" 
			"src/HexRaysCodeXplorer/ObjectExplorer.cpp" 
			"src/HexRaysCodeXplorer/CodeXplorer.cpp"
			"src/HexRaysCodeXplorer/CtreeGraphBuilder.cpp" 
			"src/HexRaysCodeXplorer/TypeReconstructor.cpp" 
			"src/HexRaysCodeXplorer/TypeExtractor.cpp"
			"src/HexRaysCodeXplorer/Utility.cpp"
			)

add_ida_plugin(${CMAKE_PROJECT_NAME} ${sources})