project(xml-parser)

aux_source_directory(. DIR_SRC)

add_library(${PROJECT_NAME} STATIC ${DIR_SRC})
target_include_directories(${PROJECT_NAME} PUBLIC ${utility_Dir})
