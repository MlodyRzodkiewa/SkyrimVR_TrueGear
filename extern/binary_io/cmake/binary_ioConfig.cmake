# Minimal binary_io config for CommonLibVR

set(binary_io_FOUND TRUE)

# Provide imported target if not already defined
if(NOT TARGET binary_io::binary_io)
    add_library(binary_io INTERFACE)
    add_library(binary_io::binary_io ALIAS binary_io)

    target_include_directories(binary_io INTERFACE
        "${CMAKE_CURRENT_LIST_DIR}/../include"
    )
endif()
