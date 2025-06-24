cmake_minimum_required(VERSION 3.18.4)

set(ADNDTK_LIB adndtk)
project(${ADNDTK_LIB}
    LANGUAGES CXX
    VERSION 0.9.2501.1
    DESCRIPTION "AdndTK is an open C++ role playing games library. It aims to mimic the ruleset of Advanced Dungeons & Dragons 2nd Edition"
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

string(TIMESTAMP CURRENT_YEAR "%Y")

#Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++20")

option(RUN_UNIT_TESTS "Run unit tests after build" OFF)
option(RUN_CLANG_TIDY "Run clang-tidy linering tool" OFF)

include(GNUInstallDirs)
include(ExternalProject)

#Find packages
find_package(PythonInterp 3 REQUIRED)

find_package(SQLite3 REQUIRED)
if(SQLite3_FOUND)
    find_path(SQLite3_EXECUTABLE sqlite3)
    message("SQLite3 " ${SQLite3_VERSION} " executable found in " ${SQLite3_EXECUTABLE})
    message("SQLite3_INCLUDE_DIRS" " " ${SQLite3_INCLUDE_DIRS})
    message("SQLite3_LIBRARIES: " ${SQLite3_LIBRARIES})
endif()

find_package(Git REQUIRED)
if(Git_FOUND)
    message("Git found: ${GIT_EXECUTABLE}")
endif()

#Include RapidJSON module
include("${CMAKE_SOURCE_DIR}/vendor/rapidjson.cmake")
include_directories(${RAPIDJSON_INCLUDE_DIR})

# Setup database subdirectories
set(DB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/db)
set(SQL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sql)
set(PY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/py)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(CFG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/config)
set(DEPLOY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deploy)

set(LOAD_MONSTER_STAGING_DATA_FILE ${DB_DIR}/load_monster_data_staging.sqlite)

# Setup SQLite files
set(DB_NAME adndtk.db)
set(DB_FILE ${DB_DIR}/${DB_NAME})
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

message("Configuring database generation: ${DB_FILE}")

execute_process(COMMAND mkdir -p ${DB_DIR})


configure_file(${CFG_DIR}/load_monster_data_staging.sqlite.in ${DB_DIR}/load_monster_data_staging.sqlite)


# Creation of SQLite database
message("Creation of SQLite database")
execute_process(COMMAND ${SQLite3_EXECUTABLE}/sqlite3 ${DB_FILE}
                INPUT_FILE ${DB_SCHEMA_FILE}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE SQLITE_BUILD_DB)
if(NOT SQLITE_BUILD_DB EQUAL "0")
    message(FATAL_ERROR "${SQLite3_EXECUTABLE}/sqlite3 ${DB_FILE} < ${DB_SCHEMA_FILE} failed with ${SQLITE_BUILD_DB}")
endif()


message("Loading staging data into SQLite database")
execute_process(COMMAND ${SQLite3_EXECUTABLE}/sqlite3 ${DB_FILE}
                INPUT_FILE ${LOAD_MONSTER_STAGING_DATA_FILE}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE SQLITE_BUILD_DB)
if(NOT SQLITE_BUILD_DB EQUAL "0")
    message(FATAL_ERROR "${SQLite3_EXECUTABLE}/sqlite3 ${DB_FILE} < ${LOAD_MONSTER_STAGING_DATA_FILE} failed with ${SQLITE_BUILD_DB}")
endif()


function(fill_db_data DB_FILE_PATH SQL_DATA_FILE)
    message("Filling ${SQL_DATA_FILE} data file into SQLite database")
    execute_process(COMMAND ${SQLite3_EXECUTABLE}/sqlite3 ${DB_FILE_PATH} ".read ${SQL_DATA_FILE}"
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    RESULT_VARIABLE SQLITE_FILL_DB)
    if(NOT SQLITE_FILL_DB EQUAL "0")
        message(FATAL_ERROR "${SQLite3_EXECUTABLE}/sqlite3 ${DB_FILE_PATH} '.read ${SQL_DATA_FILE}' failed with ${SQLITE_FILL_DB}")
    endif()
endfunction()


# Filling Base data into SQLite database
fill_db_data(${DB_FILE} ${DB_BASE_FILE})

# Filling Miscellaneous data into SQLite database
fill_db_data(${DB_FILE} ${DB_DATA_FILE})

# Filling Equipment data into SQLite database
fill_db_data(${DB_FILE} ${DB_EQUI_FILE})

# Filling Saving Roll data into SQLite database
fill_db_data(${DB_FILE} ${DB_SAVE_FILE})

# Filling Undead data into SQLite database
fill_db_data(${DB_FILE} ${DB_UNDE_FILE})

# Filling Deities data into SQLite database
fill_db_data(${DB_FILE} ${DB_DEIT_FILE})

# Filling Treasure data into SQLite database
fill_db_data(${DB_FILE} ${DB_TREA_FILE})

# Filling Spells data into SQLite database
fill_db_data(${DB_FILE} ${DB_SPEL_FILE})

# Filling Monsters data into SQLite database
fill_db_data(${DB_FILE} ${DB_MONS_FILE})

set(SQL_FILE_SUFFIXES "ac" "activity_cycle" "alignment" "appearing" "climate_terrain" "diet" "movement" "organisation" "treasure")
foreach(SQL_FILE_SUFFIX ${SQL_FILE_SUFFIXES})
    fill_db_data(${DB_FILE} ${SQL_DIR}/monsters_${SQL_FILE_SUFFIX}.sql)
endforeach()


# Filling Geography data into SQLite database
fill_db_data(${DB_FILE} ${DB_GEOG_FILE})

# Filling Roll of the Years data into SQLite database
fill_db_data(${DB_FILE} ${DB_ROLL_FILE})

# Creating index on SQLite database
message("Creating index on SQLite database")
execute_process(COMMAND ${SQLite3_EXECUTABLE}/sqlite3 ${DB_FILE} ".read ${DB_IDX_FILE}"
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE SQLITE_CREATE_INDEX)
if(NOT SQLITE_CREATE_INDEX EQUAL "0")
    message(FATAL_ERROR "${SQLite3_EXECUTABLE}/sqlite3 ${DB_FILE} '.read ${DB_IDX_FILE}' failed with ${SQLITE_CREATE_INDEX}")
endif()

message("Configuring library header generation: ${SRC_DIR}/defs.h")
execute_process(
    COMMAND ${PYTHON_EXECUTABLE} ${PY_DIR}/generate_defs.py
           --database ${DB_FILE}
           --json ${PY_DIR}/tables_info.json
           --output ${SRC_DIR}/defs.h
           --namespace Adndtk:Defs
           --version ${PROJECT_VERSION}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    RESULT_VARIABLE CPP_CREATE_HEADER_DEFS_H)
if(NOT CPP_CREATE_HEADER_DEFS_H EQUAL "0")
    message(FATAL_ERROR "${PYTHON_EXECUTABLE} ${PY_DIR}/generate_defs.py --database ${DB_FILE} --json ${PY_DIR}/tables_info.json --output ${SRC_DIR}/defs.h --namespace Adndtk:Defs --version ${PROJECT_VERSION} failed with ${CPP_CREATE_HEADER_DEFS_H}")
endif()

message("Configuring library header generation: ${SRC_DIR}/dictionary.h")
execute_process(
    COMMAND ${PYTHON_EXECUTABLE} ${PY_DIR}/generate_dictionary.py
           --database ${DB_FILE}
           --json ${PY_DIR}/dictionary_config.json
           --output ${SRC_DIR}/dictionary.h
           --namespace Adndtk:Dictionary
           --version ${PROJECT_VERSION}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    RESULT_VARIABLE CPP_CREATE_HEADER_DICTIONARY_H)
if(NOT CPP_CREATE_HEADER_DICTIONARY_H EQUAL "0")
    message(FATAL_ERROR "${PYTHON_EXECUTABLE} ${PY_DIR}/generate_dictionary.py --database ${DB_FILE} --json ${PY_DIR}/dictionary_config.json --output ${SRC_DIR}/dictionary.h --namespace Adndtk:Dictionary --version ${PROJECT_VERSION} failed with ${CPP_CREATE_HEADER_DICTIONARY_H}")
endif()

message("Configuring library header generation: ${SRC_DIR}/tables.h")
execute_process(
    COMMAND ${PYTHON_EXECUTABLE} ${PY_DIR}/generate_tables.py
           --database ${DB_FILE}
           --json ${PY_DIR}/orm_mapping.json
           --output ${SRC_DIR}/tables.h
           --namespace Adndtk:Tables
           --version ${PROJECT_VERSION}
           WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
           RESULT_VARIABLE CPP_CREATE_HEADER_TABLES_H)
if(NOT CPP_CREATE_HEADER_TABLES_H EQUAL "0")
    message(FATAL_ERROR "${PYTHON_EXECUTABLE} ${PY_DIR}/generate_tables.py --database ${DB_FILE} --json ${PY_DIR}/orm_mapping.json --output ${SRC_DIR}/tables.h --namespace Adndtk:Tables --version ${PROJECT_VERSION} failed with ${CPP_CREATE_HEADER_TABLES_H}")
endif()

message("Configuring target for defs.h")
add_custom_target(defs_h ALL DEPENDS ${SRC_DIR}/defs.h SOURCES ${DB_FILE})
message("Configuring target for tables.h")
add_custom_target(tables_h ALL DEPENDS ${SRC_DIR}/tables.h SOURCES ${DB_FILE})
message("Configuring target for SQLite database")
add_custom_target(adnd_db ALL DEPENDS ${DB_FILE} SOURCES ${DB_SCHEMA_FILE})

#execute_process(COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:"%ad" --date=iso8601 --reverse OUTPUT_VARIABLE FIRST_COMMIT_DATE)
execute_process(COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:"%ad" --date=iso8601 OUTPUT_VARIABLE LAST_COMMIT_DATE)
message("Last commit date: " ${LAST_COMMIT_DATE})
execute_process(COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:"%an" OUTPUT_VARIABLE LAST_COMMIT_AUTHOR_NAME)
message("Commit author name: " ${LAST_COMMIT_AUTHOR_NAME})
execute_process(COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:"%H" OUTPUT_VARIABLE LAST_COMMIT_HASH)
message("Last commit hash: " ${LAST_COMMIT_HASH})

#Setting database source
set(ADNDTK_DATABASE_SOURCE "")
if(NOT DATABASE_SOURCE)
    message("DATABASE_SOURCE not set, using default value")
    set(ADNDTK_DATABASE_SOURCE DatabaseSource::memory)
else()
    string(TOLOWER "${DATABASE_SOURCE}" DATABASE_SOURCE_LOWERCASE)

    if(DATABASE_SOURCE_LOWERCASE STREQUAL "memory")
        set(ADNDTK_DATABASE_SOURCE DatabaseSource::memory)
    elseif(DATABASE_SOURCE_LOWERCASE STREQUAL "file")
        set(ADNDTK_DATABASE_SOURCE DatabaseSource::file)
    else()
        message("DATABASE_SOURCE: unknown value (${DATABASE_SOURCE_LOWERCASE}), using default value")
        set(ADNDTK_DATABASE_SOURCE DatabaseSource::memory)
    endif()
    message("ADNDTK_DATABASE_SOURCE = " ${ADNDTK_DATABASE_SOURCE})
endif()



#Setting database path
set(ADNDTK_DATABASE_PATH "")
if(NOT DATABASE_PATH)
    message("DATABASE_PATH not set, using default value")
    set(ADNDTK_DATABASE_PATH ${DB_FILE})
elseif(ADNDTK_DATABASE_SOURCE STREQUAL DatabaseSource::file)
    set(ADNDTK_DATABASE_PATH ${DB_FILE})
    message("ADNDTK_DATABASE_PATH = " ${ADNDTK_DATABASE_PATH})
else()
    set(ADNDTK_DATABASE_PATH "file:${DB_FILE}?mode=memory&cache=shared")
    message("ADNDTK_DATABASE_PATH = " ${ADNDTK_DATABASE_PATH})
endif()



#Configuring error handling method
set(ADNDTK_ERROR_HANDLER "")
if(NOT ERROR_HANDLER)
    message("ERROR_HANDLER not set, using default value")
    set(ADNDTK_ERROR_HANDLER "exception")
else()
    string(TOLOWER "${ERROR_HANDLER}" ERROR_HANDLER_LOWERCASE)

    if(ERROR_HANDLER_LOWERCASE STREQUAL standard_output)
        set(ADNDTK_ERROR_HANDLER ${ERROR_HANDLER_LOWERCASE})
    elseif(ERROR_HANDLER_LOWERCASE STREQUAL last_message)
        set(ADNDTK_ERROR_HANDLER ${ERROR_HANDLER_LOWERCASE})
    elseif(ERROR_HANDLER_LOWERCASE STREQUAL exception)
        set(ADNDTK_ERROR_HANDLER ${ERROR_HANDLER_LOWERCASE})
    else()
        message("ERROR_HANDLER: unknown value (${ERROR_HANDLER_LOWERCASE}), using default value")
        set(ADNDTK_ERROR_HANDLER "exception")
    endif()
    
    message("ADNDTK_ERROR_HANDLER = " ${ADNDTK_ERROR_HANDLER})
endif()



#Configuring Cyclopedia query output type
set(ADNDTK_KB_QUERY_OUTPUT "")
if(NOT KB_QUERY_OUTPUT)
    message("KB_QUERY_OUTPUT not set, using default value")
    set(ADNDTK_KB_QUERY_OUTPUT "json")
elseif(KB_QUERY_OUTPUT STREQUAL "json")
    set(ADNDTK_KB_QUERY_OUTPUT "json")
    message("ADNDTK_KB_QUERY_OUTPUT = " ${ADNDTK_KB_QUERY_OUTPUT})
elseif(KB_QUERY_OUTPUT STREQUAL "plain")
    set(ADNDTK_KB_QUERY_OUTPUT "plain")
    message("ADNDTK_KB_QUERY_OUTPUT = " ${ADNDTK_KB_QUERY_OUTPUT})
else()
    set(ADNDTK_DATABASE_PATH "json")
    message("ADNDTK_KB_QUERY_OUTPUT = " ${ADNDTK_KB_QUERY_OUTPUT})
endif()


configure_file(${CFG_DIR}/config.h.in ${SRC_DIR}/config.h)
configure_file(${CFG_DIR}/LICENSE.md.in ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md)

set(ADNDTK_DATABASE_PATH ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/${ADNDTK_LIB}/${CMAKE_PROJECT_VERSION}/data/${DB_NAME})
configure_file(${CFG_DIR}/config.h.in ${DEPLOY_DIR}/config.h)

message("Adding subdirectory src")
add_subdirectory(src)
message("Adding subdirectory demo")
add_subdirectory(demo)
message("Adding subdirectory test")
add_subdirectory(test)
message("Adding subdirectory cli")
add_subdirectory(cli)


# Clang-tidy
if(RUN_CLANG_TIDY)
    message("Clang-tidy was required: checking module availability")
    find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
    if(CLANG_TIDY_EXE)
        message("Clang-tidy was found: module enabled")
        set(CLANG_TIDY_COMMAND
            ${CLANG_TIDY_EXE}" "
            --checks=*" "
            --header-filter=." "
            --warnings-as-errors=*" "
            --use-color
        )
        set_target_properties(${ADNDTK_LIB} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
    else()
        message("Clang-tidy not found: module disabled")
    endif()
else()
    message("Clang-tidy was not required: lintering disabled")
endif()


# Install Sqlite DB
install(
    FILES
        ${DB_FILE}
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/${ADNDTK_LIB}/${CMAKE_PROJECT_VERSION}/data
)