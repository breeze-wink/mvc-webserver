project(engine_context_test
    DESCRIPTION "给engine模块中新增的上下文类Context进行简单的测试"
)

add_executable(${PROJECT_NAME} engine_context_test.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC ${SRC_DIR}/engine)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${test_output_path}
)

add_test(
    NAME ${PROJECT_NAME}
    COMMAND ${PROJECT_NAME}
)