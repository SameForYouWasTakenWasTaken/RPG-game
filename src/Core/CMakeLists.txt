file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")   

add_library(Core STATIC ${SOURCES})

target_include_directories(Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Core PUBLIC Shared)
target_link_libraries(Core PUBLIC SFML::Graphics)
target_link_libraries(Core PUBLIC glm::glm)
target_link_libraries(Core PUBLIC Tracy::TracyClient)