file(GLOB_RECURSE GAME_SOURCES "*.cpp")

add_library(Game STATIC ${GAME_SOURCES})

target_include_directories(Game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Game PUBLIC Core)

# For workflow
install(DIRECTORY ${RESOURCES_DIR}
        DESTINATION bin/Resources)

target_compile_definitions(Game PRIVATE
    RESOURCES_DIRECTORY=\"Resources/\"
)
