file(GLOB_RECURSE SRC_DIR
    "*.h"
    "*.cpp"
)

add_executable(${PROJECT_NAME}_test_app
    ${SRC_DIR}
)

target_link_libraries(${PROJECT_NAME}_test_app
    ${PROJECT_NAME}
)

target_include_directories(${PROJECT_NAME}_test_app PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)
