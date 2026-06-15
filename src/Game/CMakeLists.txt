file(GLOB_RECURSE GAME_SOURCES "*.cpp")

add_library(Game STATIC ${GAME_SOURCES})

target_include_directories(Game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Game PUBLIC Core nlohmann_json::nlohmann_json tinyxml2::tinyxml2)

# For workflow
install(DIRECTORY ${RESOURCES_DIR}
        DESTINATION bin/)

target_compile_definitions(Game PRIVATE
    RESOURCES_DIRECTORY=\"Resources/\"
)
