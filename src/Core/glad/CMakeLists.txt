add_library(glad STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/khrplatform.h
        ${CMAKE_CURRENT_SOURCE_DIR}/gl.c
        ${CMAKE_CURRENT_SOURCE_DIR}/gl.h
)
target_include_directories(glad PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})