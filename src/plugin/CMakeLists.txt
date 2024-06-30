project(plugin
    DESCRIPTION "实现业务插件"
)

add_library(echo_plugin SHARED echo_plugin.cpp ${utility_Dir}/Logger.cpp ${utility_Dir}/system.cpp)
target_include_directories(echo_plugin PUBLIC ${engine_Dir} ${utility_Dir})
# target_link_libraries(echo_plugin PUBLIC utility)

set_target_properties(echo_plugin PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${plugin_output_Dir}
)
