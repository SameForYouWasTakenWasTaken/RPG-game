file(GLOB_RECURSE GAME_SOURCES "*.cpp")


add_library(Game STATIC ${GAME_SOURCES})

target_include_directories(Game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Game PUBLIC Core)


add_custom_command(TARGET Game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/Resources
    $<TARGET_FILE_DIR:Game>/Resources
)


# For workflow
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Resources
        DESTINATION Resources)

# Always relative to executable directory (best practice)
target_compile_definitions(Game PRIVATE
    RESOURCES_DIRECTORY="Resources/"
)

target_compile_definitions(Core PRIVATE
    RESOURCES_DIRECTORY="Resources/"
)
