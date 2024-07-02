project(jsonParser
    DESCRIPTION "实现了一个json文件的解析器"
)

aux_source_directory(. DIR_SRC)

add_library(${PROJECT_NAME} STATIC ${DIR_SRC})

