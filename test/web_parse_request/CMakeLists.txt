project(http_request_test
    DESCRIPTION "测试对http请求的解析"
)

add_executable(${PROJECT_NAME} request_test.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC ${SRC_DIR}/web ${SRC_DIR}/jsonParser ${SRC_DIR}/utility)

target_link_libraries(${PROJECT_NAME} PUBLIC web utility jsonParser)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${test_output_path}
)