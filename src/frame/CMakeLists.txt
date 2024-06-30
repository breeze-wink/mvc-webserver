project(frame
    DESCRIPTION "将server的启动进行一个封装"
)

aux_source_directory(. DIR_SRC)

add_library(${PROJECT_NAME} STATIC ${DIR_SRC})
target_include_directories(${PROJECT_NAME} PUBLIC ${utility_Dir} ${socketHandler_Dir} ${ThreadPool_Dir})
target_link_libraries(${PROJECT_NAME} PUBLIC utility socketHandler ThreadPool)