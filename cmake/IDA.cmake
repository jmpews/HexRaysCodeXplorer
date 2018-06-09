cmake_minimum_required(VERSION 3.1)
cmake_policy(SET CMP0054 NEW)

include(cmake/Macros.cmake)
include(cmake/Util.cmake)

set(IDA_BINARY_64         OFF   CACHE BOOL "Build a 64 bit binary (IDA>= 7.0)"             )
set(IDA_EA_64             OFF   CACHE BOOL "Build for 64 bit IDA (ida64, sizeof(ea_t) == 8)")
set(IDA_SDK_DIR           ""    CACHE PATH "Path to IDA SDK"                                )
set(IDA_INSTALL_DIR       ""    CACHE PATH "Install path of IDA"                            )
set(IDA_VERSION           690   CACHE INT  "IDA Version to build for (e.g. 6.9 is 690)."    )

message(STATUS "IDA Plugin BUILD INFO:")
message(STATUS "\tSystem: ${CMAKE_SYSTEM_NAME}")
message(STATUS "\tProcessor: ${CMAKE_SYSTEM_PROCESSOR}")
message(STATUS "\tVersion: ${IDA_VERSION}")
message(STATUS "\tIDA_BINARY_64: ${IDA_BINARY_64}")
message(STATUS "\tIDA_SDK_DIR: ${IDA_SDK_DIR}")
message(STATUS "\tIDA_INSTALL_DIR: ${IDA_INSTALL_DIR}")

set(GLOBAL.CHECK_PATH "")
# darwin sdk path, install path check
if (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    list(APPEND GLOBAL.CHECK_PATH
        ${IDA_INSTALL_DIR}/plugins/hexrays_sdk/include
        ${IDA_SDK_DIR}/include
    )
    set(GLOBAL.INCLUDE_PATH ${GLOBAL.INCLUDE_PATH} ${GLOBAL.CHECK_PATH})
    set(IDA_IDA_LIBRARY_PATH ${IDA_INSTALL_DIR})
    if(IDA_BINARY_64)
        set(IDA_PRO_LIBRARY_PATH ${IDA_SDK_DIR}/lib/x64_mac_gcc_64)
    else()
        set(IDA_PRO_LIBRARY_PATH ${IDA_SDK_DIR}/lib/x64_mac_gcc_32)
    endif()
endif()
check_files_exist(${GLOBAL.CHECK_PATH})

# check ida library
if(IDA_BINARY_64)
    find_library(IDA_IDA_LIBRARY "ida64" NAMES "ida64" PATHS ${IDA_INSTALL_DIR} REQUIRED)
else()
    find_library(IDA_IDA_LIBRARY "ida" NAMES "ida" PATHS ${IDA_INSTALL_DIR} REQUIRED)
endif()
if(NOT IDA_IDA_LIBRARY)
    message(FATAL_ERROR "[!] NOT FOUND [ida] LIBRARY ${IDA_INSTALL_DIR}")
endif()

# check pro static library
find_library(IDA_PRO_LIBRARY NAMES "pro.a" "pro.lib" PATHS ${IDA_PRO_LIBRARY_PATH} REQUIRED)
if(NOT IDA_PRO_LIBRARY)
    find_file(IDA_PRO_LIBRARY_FILE NAMES "pro.a" "pro.lib" PATHS ${IDA_PRO_LIBRARY_PATH})
    if(NOT IDA_PRO_LIBRARY_FILE)
        message(FATAL_ERROR "[!] NOT FOUND [pro] LIBRARY FROM ${IDA_PRO_LIBRARY_PATH}")
    else()
        add_library(IDA_PRO_LIBRARY STATIC IMPORTED)
        SET_TARGET_PROPERTIES(IDA_PRO_LIBRARY PROPERTIES IMPORTED_LOCATION ${IDA_PRO_LIBRARY_FILE})
    endif()
endif()



# check ida version
if (IDA_VERSION LESS 690)
    message(FATAL_ERROR "IDA versions below 6.9 are no longer supported.")
endif()

# check processor arch
if (NOT CMAKE_SYSTEM_PROCESSOR MATCHES "^x86_64")
    message(FATAL_ERROR "ONLY SUPPORT x86_64!")
endif()

if(UNIX)
    if (NOT IDA_BINARY_64)
        set(CMAKE_C_FLAGS   "-m32" CACHE STRING "C compiler flags"   FORCE)
        set(CMAKE_CXX_FLAGS "-m32" CACHE STRING "C++ compiler flags" FORCE)
    endif()
endif()

# include directory
include_directories(${GLOBAL.INCLUDE_PATH})

function (add_ida_plugin plugin_name)
    message(STATUS "\tPlugin Name: ${plugin_name}")
    set(sources ${ARGV})
    if (sources)
        list(REMOVE_AT sources 0)
    endif ()
    add_library(${plugin_name} SHARED ${sources})

    # Compiler specific properties.
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        target_compile_definitions(${plugin_name} PUBLIC "__VC__")
        target_compile_options(${plugin_name} PUBLIC "/wd4996" "/MP")
    endif ()

    # General definitions required throughout all kind of IDA modules.
    target_compile_definitions(${plugin_name} PUBLIC
        "NO_OBSOLETE_FUNCS"
        "__IDP__")

    target_include_directories(${plugin_name} PUBLIC "${IDA_SDK}/include")

    if (IDA_BINARY_64)
        target_compile_definitions(${plugin_name} PUBLIC "__X64__")
        set(plugin_extension "64${LIB_EXT}")
    else()
        set(plugin_extension "${LIB_EXT}")
    endif()
    
    if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
        target_compile_definitions(${plugin_name} PUBLIC "__NT__")
    elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
        target_compile_definitions(${plugin_name} PUBLIC "__MAC__")
    elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
        target_compile_definitions(${plugin_name} PUBLIC "__LINUX__")
    else()
        message(FATAL_ERROR "[!] IDA DO NOT KNOWN YOUR SYSTEM")
    endif()
    

    set_target_properties(${plugin_name} PROPERTIES
        PREFIX ""
        SUFFIX ${plugin_extension}
        OUTPUT_NAME ${plugin_name})
    
    target_link_libraries(${plugin_name} ${IDA_IDA_LIBRARY} ${IDA_PRO_LIBRARY})
endfunction()