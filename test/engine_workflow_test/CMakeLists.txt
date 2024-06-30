project(engine_workflow_test
    DESCRIPTION "对引入Context的workflow进行简单测试"
)


add_executable(${PROJECT_NAME} engine_workflow_test.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${SRC_DIR}/engine ${SRC_DIR}/utility ${SRC_DIR}/xml)

target_link_libraries(${PROJECT_NAME} PUBLIC engine)

target_compile_options(${PROJECT_NAME} PRIVATE -g)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${test_output_path}
)

add_test(
    NAME ${PROJECT_NAME}
    COMMAND ${PROJECT_NAME}
)