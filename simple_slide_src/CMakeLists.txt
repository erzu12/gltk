file(GLOB_RECURSE SRC_DIR
    "*.h"
    "*.cpp"
)

set(EXECUTABLE_NAME simple_slide)

add_executable(${EXECUTABLE_NAME}
    ${SRC_DIR}
)

target_link_libraries(${EXECUTABLE_NAME}
    ${PROJECT_NAME}
)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)
