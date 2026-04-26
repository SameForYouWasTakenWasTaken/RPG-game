file(GLOB_RECURSE GAME_SOURCES "*.cpp")


add_library(Game STATIC ${GAME_SOURCES})

target_include_directories(Game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Game PUBLIC Core)


if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_custom_command(TARGET Game POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/Resources
        $<TARGET_FILE_DIR:Game>/Resources
    )
endif()

# For workflow
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Resources
        DESTINATION bin/Resources)

set(RESOURCE_DIR "$<TARGET_FILE_DIR:Game>/Resources")

# Give engine and app same directories
target_compile_definitions(Game PRIVATE RESOURCES_DIRECTORY="Resources/")
target_compile_definitions(Core PRIVATE RESOURCES_DIRECTORY="Resources/")
