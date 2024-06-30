project(ThreadPool
    DESCRIPTION "使用C++11封装的一个线程池"
)

aux_source_directory(. DIR_SRC)
add_library(${PROJECT_NAME} STATIC ${DIR_SRC})
target_include_directories(${PROJECT_NAME} PUBLIC ${utility_Dir})