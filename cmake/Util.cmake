# Check files list exist
function(check_files_exist CHECK_FILES)
    foreach(file ${CHECK_FILES})
        if(NOT EXISTS "${file}")
            message(FATAL_ERROR "${file} NOT EXISTS!")
        endif()
    endforeach()
endfunction()
