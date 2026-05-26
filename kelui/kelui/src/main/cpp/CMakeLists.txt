cmake_minimum_required(VERSION 3.22.1)

project("kelapi")

# Optimization flags for stripping and size
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s -Oz")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s -Oz")

# Define project root relative to this file
set(KEL_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../../../..)

# Include directories
include_directories(
    ${KEL_ROOT}/api
    ${KEL_ROOT}/externalstuff
)

add_library( # Sets the name of the library.
             kelapi

             # Sets the library as a shared library.
             SHARED

             # Native lib entry point
             native-lib.cpp

             # API implementation
             ${KEL_ROOT}/api/kelapi.cpp

             # External stuff
             ${KEL_ROOT}/externalstuff/patchinline.cpp
             ${KEL_ROOT}/externalstuff/open.cpp

             # UI Backend
             ${KEL_ROOT}/kel/ui/VulkanBackend.cpp
)

find_library( # Sets the name of the path variable.
              vulkan-lib
              vulkan )

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

target_link_libraries( # Specifies the target library.
                       kelapi

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib}
                       ${vulkan-lib}
                       android
)
