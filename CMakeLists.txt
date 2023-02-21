cmake_minimum_required(VERSION 3.18.4)

project(adndtk
    LANGUAGES CXX
    VERSION 0.1.0.0
    DESCRIPTION "AdndTK is an open C++ role playing games library. It aims to mimic the ruleset of Advanced Dungeons & Dragons 2nd Edition"
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_path(SQLITE3_EXECUTABLE sqlite3)
message("SQLite3 executable found in ${SQLITE3_EXECUTABLE}")

# Setup database subdirectories
set(DB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/db)
set(SQL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sql)

# Setup SQLite files
set(DB_FILE ${DB_DIR}/adndtk.db)
set(DB_SCHEMA_FILE ${SQL_DIR}/schema.sql)
set(DB_DATA_FILE ${SQL_DIR}/data.sql)
set(DB_IDX_FILE ${SQL_DIR}/indexes.sql)

#Prepare SQLite DB
file(REMOVE ${DB_FILE})

#add_custom_target(adnd_db ALL DEPENDS ${DB_FILE} SOURCES ${DB_SCHEMA_FILE})

message("Generating database ${DB_FILE}")
add_custom_command(
    OUTPUT ${DB_FILE}
    DEPENDS ${DB_SCHEMA_FILE} ${DB_DATA_FILE} ${DB_IDX_FILE}
    COMMAND ${SQLITE3_EXECUTABLE}/sqlite3 ${DB_FILE} < ${DB_SCHEMA_FILE}
    COMMAND ${SQLITE3_EXECUTABLE}/sqlite3 ${DB_FILE} ".read ${DB_DATA_FILE}"
    COMMAND ${SQLITE3_EXECUTABLE}/sqlite3 ${DB_FILE} ".read ${DB_IDX_FILE}"
)