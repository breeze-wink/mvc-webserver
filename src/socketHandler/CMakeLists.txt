project(socketHandler
    DESCRIPTION "封装了socket，ServerSocket,ClientSocket,Epoll相关功能，最后封装SocketHandler作为外部调用的接口"
)

aux_source_directory(. DIR_SRC)

add_library(${PROJECT_NAME} STATIC ${DIR_SRC})

target_include_directories(${PROJECT_NAME} PUBLIC 
                           ${ThreadPool_Dir} 
                           ${task_Dir}
                           )

target_link_libraries(${PROJECT_NAME} PUBLIC task)



