project(utility)

aux_source_directory(. DIR_SRC)

add_library(${PROJECT_NAME} STATIC ${DIR_SRC})
set_target_properties(${PROJECT_NAME} PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)