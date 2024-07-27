file(GLOB_RECURSE SRC_DIR
    "*.h"
    "*.cpp"
)

add_library(${PROJECT_NAME}
    ${SRC_DIR}
)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries("${PROJECT_NAME}"
    glad
    glfw
)

target_include_directories("${PROJECT_NAME}"
    PUBLIC ${CMAKE_SOURCE_DIR}/extern/glad/include
)
