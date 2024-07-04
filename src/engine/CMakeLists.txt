project(engine
    DESCRIPTION "对业务插件进行管理、运行的引擎"
)

aux_source_directory(. DIR_SRC)
add_library(${PROJECT_NAME} STATIC ${DIR_SRC})

target_include_directories(${PROJECT_NAME} PUBLIC ${xml_Dir} ${utility_Dir})
target_link_libraries(${PROJECT_NAME} PUBLIC utility xml-parser)
