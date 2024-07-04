project(reflect
    DESCRIPTION "实现了C++中没有、而其他语言又很常见的反射机制"
)

aux_source_directory(. DIR_SRC)

add_library(${PROJECT_NAME} ${DIR_SRC}) 

target_include_directories(${PROJECT_NAME} PUBLIC ${utility_Dir})

target_link_libraries(${PROJECT_NAME} PUBLIC utility)