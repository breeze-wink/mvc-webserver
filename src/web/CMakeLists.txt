project(web
    DESCRIPTION "实现web相关的解析组件" 
)

aux_source_directory(. DIR_SRC)

add_library(${PROJECT_NAME} STATIC ${DIR_SRC})

target_include_directories(${PROJECT_NAME} PUBLIC ${jsonParser_Dir} ${utility_Dir})
target_link_libraries(${PROJECT_NAME} PUBLIC jsonParser utility)