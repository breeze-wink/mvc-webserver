set(socketHandler_Dir ${CMAKE_CURRENT_SOURCE_DIR}/socketHandler)
set(utility_Dir ${CMAKE_CURRENT_SOURCE_DIR}/utility)
set(ThreadPool_Dir ${CMAKE_CURRENT_SOURCE_DIR}/thread)
set(frame_Dir ${CMAKE_CURRENT_SOURCE_DIR}/frame)
set(engine_Dir ${CMAKE_CURRENT_SOURCE_DIR}/engine)
set(plugin_Dir ${CMAKE_CURRENT_SOURCE_DIR}/plugin)
set(xml_Dir ${CMAKE_CURRENT_SOURCE_DIR}/xml)
set(task_Dir ${CMAKE_CURRENT_SOURCE_DIR}/Task)


add_subdirectory(${utility_Dir})
add_subdirectory(${ThreadPool_Dir})
add_subdirectory(${frame_Dir})
add_subdirectory(${plugin_Dir})
add_subdirectory(${xml_Dir})
add_subdirectory(${engine_Dir})
add_subdirectory(${task_Dir})
add_subdirectory(${socketHandler_Dir})

add_executable(Server server.cpp)
add_executable(Client client.cpp)

target_include_directories(Server PUBLIC ${frame_Dir} ${utility_Dir})
target_include_directories(Client PUBLIC ${socketHandler_Dir} ${utility_Dir})

target_link_libraries(Server PUBLIC frame)
target_link_libraries(Client PUBLIC utility socketHandler)