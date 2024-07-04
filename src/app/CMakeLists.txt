project(app)

aux_source_directory(. DIR_SRC)

add_library(${PROJECT_NAME} SHARED ${DIR_SRC})

target_include_directories(${PROJECT_NAME} PUBLIC ${web_Dir})
target_link_libraries(${PROJECT_NAME} PUBLIC web)

