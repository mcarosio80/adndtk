

# Setup SQLite files
set(DB_SCHEMA_FILE ${SQL_DIR}/schema.sql)
set(DB_BASE_FILE ${SQL_DIR}/base.sql)
set(DB_DATA_FILE ${SQL_DIR}/data.sql)
set(DB_EQUI_FILE ${SQL_DIR}/equipment.sql)
set(DB_SAVE_FILE ${SQL_DIR}/saving.sql)
set(DB_UNDE_FILE ${SQL_DIR}/undead.sql)
set(DB_DEIT_FILE ${SQL_DIR}/deities.sql)
set(DB_TREA_FILE ${SQL_DIR}/treasure.sql)
set(DB_SPEL_FILE ${SQL_DIR}/spells.sql)
set(DB_MONS_FILE ${SQL_DIR}/monsters.sql)
set(DB_GEOG_FILE ${SQL_DIR}/geography.sql)
set(DB_ROLL_FILE ${SQL_DIR}/roll_of_the_years.sql)
set(DB_IDX_FILE ${SQL_DIR}/indexes.sql)

#Prepare SQLite DB
file(REMOVE ${DB_FILE})

set(LOAD_MONSTER_STAGING_DATA_FILE ${DB_DIR}/load_monster_data_staging.sqlite)
configure_file(${CFG_DIR}/load_monster_data_staging.sqlite.in ${LOAD_MONSTER_STAGING_DATA_FILE})

message("Configuring database generation: ${DB_FILE}")

execute_process(COMMAND mkdir -p ${DB_DIR}
                COMMAND_ECHO STDOUT)

# Creation of database
message("Creation of database")
execute_process(COMMAND ${SQLITE_EXE} ${DB_FILE}
                INPUT_FILE ${DB_SCHEMA_FILE}
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                RESULT_VARIABLE SQLITE_BUILD_DB
                COMMAND_ECHO STDOUT)
if(NOT SQLITE_BUILD_DB EQUAL "0")
    message(FATAL_ERROR "${SQLITE_EXE} ${DB_FILE} < ${DB_SCHEMA_FILE} failed with ${SQLITE_BUILD_DB}")
endif()


function(fill_db_data DB_FILE_PATH SQL_DATA_FILE)
    message("Filling ${SQL_DATA_FILE} data file into database")
    execute_process(COMMAND ${SQLITE_EXE} ${DB_FILE_PATH} ".read ${SQL_DATA_FILE}"
                    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                    RESULT_VARIABLE SQLITE_FILL_DB
                    COMMAND_ECHO STDOUT)
    if(NOT SQLITE_FILL_DB EQUAL "0")
        message(FATAL_ERROR "${SQLITE_EXE} ${DB_FILE_PATH} '.read ${SQL_DATA_FILE}' failed with ${SQLITE_FILL_DB}")
    endif()
endfunction()


# Filling Base data into database
fill_db_data(${DB_FILE} ${DB_BASE_FILE})

# Filling Miscellaneous data into database
fill_db_data(${DB_FILE} ${DB_DATA_FILE})

# Filling Equipment data into database
fill_db_data(${DB_FILE} ${DB_EQUI_FILE})

# Filling Saving Roll data into database
fill_db_data(${DB_FILE} ${DB_SAVE_FILE})

# Filling Undead data into database
fill_db_data(${DB_FILE} ${DB_UNDE_FILE})

# Filling Deities data into database
fill_db_data(${DB_FILE} ${DB_DEIT_FILE})

# Filling Treasure data into database
fill_db_data(${DB_FILE} ${DB_TREA_FILE})

# Filling Spells data into database
fill_db_data(${DB_FILE} ${DB_SPEL_FILE})

# Filling Geography data into database
fill_db_data(${DB_FILE} ${DB_GEOG_FILE})

# Filling Roll of the Years data into database
fill_db_data(${DB_FILE} ${DB_ROLL_FILE})


# Filling Monsters data into database
fill_db_data(${DB_FILE} ${DB_MONS_FILE})

set(SQL_FILE_SUFFIXES "ac" "activity_cycle" "alignment" "appearing" "climate_terrain" "diet" "movement" "organisation" "treasure" "variant")
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

# Creating index on database
message("Creating index on database")
execute_process(COMMAND ${SQLITE_EXE} ${DB_FILE} ".read ${DB_IDX_FILE}"
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                RESULT_VARIABLE SQLITE_CREATE_INDEX
                COMMAND_ECHO STDOUT)
if(NOT SQLITE_CREATE_INDEX EQUAL "0")
    message(FATAL_ERROR "${SQLITE_EXE} ${DB_FILE} '.read ${DB_IDX_FILE}' failed with ${SQLITE_CREATE_INDEX}")
endif()