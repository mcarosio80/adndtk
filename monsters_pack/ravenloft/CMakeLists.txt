set(SETTING ravenloft)

set(LOAD_MONSTER_STAGING_DATA_FILE ${DB_DIR}/load_monster_data_staging_${SETTING}.sqlite)
configure_file(${CFG_DIR}/load_monster_data_staging_${SETTING}.sqlite.in ${DB_DIR}/load_monster_data_staging_${SETTING}.sqlite)

message("Loading staging data into SQLite database")
execute_process(COMMAND ${SQLite3_EXECUTABLE}/sqlite3 ${DB_FILE}
                INPUT_FILE ${LOAD_MONSTER_STAGING_DATA_FILE}
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                RESULT_VARIABLE SQLITE_BUILD_DB)
if(NOT SQLITE_BUILD_DB EQUAL "0")
    message(FATAL_ERROR "${SQLite3_EXECUTABLE}/sqlite3 ${DB_FILE} < ${LOAD_MONSTER_STAGING_DATA_FILE} failed with ${SQLITE_BUILD_DB}")
endif()


function(fill_db_data DB_FILE_PATH SQL_DATA_FILE)
    message("Filling ${SQL_DATA_FILE} data file into SQLite database")
    execute_process(COMMAND ${SQLite3_EXECUTABLE}/sqlite3 ${DB_FILE_PATH} ".read ${SQL_DATA_FILE}"
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                    RESULT_VARIABLE SQLITE_FILL_DB)
    if(NOT SQLITE_FILL_DB EQUAL "0")
        message(FATAL_ERROR "${SQLite3_EXECUTABLE}/sqlite3 ${DB_FILE_PATH} '.read ${SQL_DATA_FILE}' failed with ${SQLITE_FILL_DB}")
    endif()
endfunction()

set(DB_MONS_FILE ${MONSTERS_PACK_DIR}/${SETTING}/monsters.sql)
# Filling Monsters data into SQLite database
fill_db_data(${DB_FILE} ${DB_MONS_FILE})

# set(SQL_FILE_SUFFIXES "ac" "activity_cycle" "alignment" "appearing" "climate_terrain" "diet" "movement" "organisation" "treasure")
# foreach(SQL_FILE_SUFFIX ${SQL_FILE_SUFFIXES})
#     fill_db_data(${DB_FILE} ${SQL_DIR}/monsters_${SQL_FILE_SUFFIX}.sql)
# endforeach()