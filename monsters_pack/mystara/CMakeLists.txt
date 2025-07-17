set(SETTING mystara)

set(LOAD_MONSTER_STAGING_DATA_FILE ${DB_DIR}/load_monster_data_staging_${SETTING}.sqlite)
configure_file(${CFG_DIR}/load_monster_data_staging_${SETTING}.sqlite.in ${LOAD_MONSTER_STAGING_DATA_FILE})


set(DB_MONS_FILE ${MONSTERS_PACK_DIR}/${SETTING}/monsters.sql)
# Filling Monsters data into database
fill_db_data(${DB_FILE} ${DB_MONS_FILE})

#set(SQL_FILE_SUFFIXES "ac" "activity_cycle" "alignment" "appearing" "climate_terrain" "diet" "movement" "organisation" "treasure")
set(SQL_FILE_SUFFIXES "ac" "alignment" "appearing" "diet" "movement" "treasure")
foreach(SQL_FILE_SUFFIX ${SQL_FILE_SUFFIXES})
    fill_db_data(${DB_FILE} ${SQL_DIR}/monsters_${SQL_FILE_SUFFIX}.sql)
endforeach()

message("Running staging script ${LOAD_MONSTER_STAGING_DATA_FILE} into database")
execute_process(COMMAND ${SQLITE_EXE} ${DB_FILE}
                INPUT_FILE ${LOAD_MONSTER_STAGING_DATA_FILE}
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                RESULT_VARIABLE SQLITE_BUILD_DB
                COMMAND_ECHO STDOUT)
if(NOT SQLITE_BUILD_DB EQUAL "0")
    message(FATAL_ERROR "${SQLITE_EXE} ${DB_FILE} < ${LOAD_MONSTER_STAGING_DATA_FILE} failed with ${SQLITE_BUILD_DB}")
endif()