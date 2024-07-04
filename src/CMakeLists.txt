set(socketHandler_Dir ${CMAKE_CURRENT_SOURCE_DIR}/socketHandler)
set(utility_Dir ${CMAKE_CURRENT_SOURCE_DIR}/utility)
set(ThreadPool_Dir ${CMAKE_CURRENT_SOURCE_DIR}/thread)
set(frame_Dir ${CMAKE_CURRENT_SOURCE_DIR}/frame)
set(engine_Dir ${CMAKE_CURRENT_SOURCE_DIR}/engine)
set(plugin_Dir ${CMAKE_CURRENT_SOURCE_DIR}/plugin)
set(xml_Dir ${CMAKE_CURRENT_SOURCE_DIR}/xml)
set(task_Dir ${CMAKE_CURRENT_SOURCE_DIR}/Task)
set(jsonParser_Dir ${CMAKE_CURRENT_SOURCE_DIR}/jsonParser)
set(web_Dir ${CMAKE_CURRENT_SOURCE_DIR}/web)
set(reflect_Dir ${CMAKE_CURRENT_SOURCE_DIR}/reflect)
set(app_Dir ${CMAKE_CURRENT_SOURCE_DIR}/app)


add_subdirectory(${utility_Dir})
add_subdirectory(${ThreadPool_Dir})
add_subdirectory(${frame_Dir})
add_subdirectory(${plugin_Dir})
add_subdirectory(${xml_Dir})
add_subdirectory(${engine_Dir})
add_subdirectory(${task_Dir})
add_subdirectory(${jsonParser_Dir})
add_subdirectory(${reflect_Dir})
add_subdirectory(${web_Dir})
add_subdirectory(${socketHandler_Dir})
add_subdirectory(${app_Dir})


add_executable(Server main.cpp)

target_include_directories(Server PUBLIC ${web_Dir} ${utility_Dir})

target_link_libraries(Server PUBLIC frame app)

set_target_properties(Server PROPERTIES
    BUILD_RPATH ${PROJECT_SOURCE_DIR}/lib
)