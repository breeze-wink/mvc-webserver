project(socketHandler
    DESCRIPTION "围绕Epoll，对各种功能进行封装, 最终以socketHandler类对网络IO进行操作"
)

aux_source_directory(. DIR_SRC)

add_library(${PROJECT_NAME} STATIC ${DIR_SRC})

target_include_directories(${PROJECT_NAME} PUBLIC 
                           ${ThreadPool_Dir} ${engine_Dir} ${xml_Dir}
                           ${task_Dir}
                           )

target_link_libraries(${PROJECT_NAME} PUBLIC task)



