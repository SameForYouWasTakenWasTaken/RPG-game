cmake_minimum_required(VERSION 3.30)
project(main)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
#     set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
#     set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo")
# endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(RESOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/Game/Resources")

add_subdirectory(src)

include(Dependencies.cmake)


add_executable(main src/main.cpp)
target_link_libraries(main PUBLIC Game)

add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${RESOURCES_DIR}
    $<TARGET_FILE_DIR:main>/Resources
)

install(TARGETS main)

# Automatically copy all required DLLs on Windows
if (WIN32)
    install(FILES $<TARGET_RUNTIME_DLLS:main>
            DESTINATION .
    )
endif()