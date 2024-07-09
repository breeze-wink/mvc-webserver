project(test
    DESCRIPTION "给一些新增的小模块在并入项目前进行简单的使用测试"
)

set(test_output_path ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test)

add_subdirectory(web_parse_request)