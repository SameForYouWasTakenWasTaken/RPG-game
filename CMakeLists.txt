cmake_minimum_required(VERSION 4.2.0)
project(main)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
#     set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
#     set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo")
# endif()

add_subdirectory(src)

include(Dependencies.cmake)

add_executable(main src/main.cpp)
target_link_libraries(main PUBLIC Game)