project(task
    DESCRIPTION "实现各种业务需要的任务"
)

aux_source_directory(. DIR_SRC)
add_library(${PROJECT_NAME} STATIC ${DIR_SRC})

target_include_directories(${PROJECT_NAME} PUBLIC ${socketHandler_Dir} 
                           ${engine_Dir} ${utility_Dir} ${ThreadPool_Dir}
                           ${xml_Dir})
                           
target_link_libraries(${PROJECT_NAME} PUBLIC engine ThreadPool)