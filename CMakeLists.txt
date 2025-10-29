cmake_minimum_required(VERSION 3.18.4)

set(ADNDTK_LIB adndtk)
project(${ADNDTK_LIB}
    LANGUAGES CXX
    VERSION 0.9.2508.1
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
set(DB_DIR ${CMAKE_SOURCE_DIR}/db)
set(SQL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sql)
set(PY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/py)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(CFG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/config)
set(DEPLOY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/deploy)
set(MONSTERS_PACK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/monsters_pack)
set(DEMO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/demo)
set(TEST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test)
set(CLI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cli)

# Setup SQLite parameters
set(DB_NAME adndtk.db)
set(DB_FILE ${DB_DIR}/${DB_NAME})
set(SQLITE_EXE ${SQLite3_EXECUTABLE}/sqlite3)

function(enter_directory DIR_PATH)
    message("Enterging subdirectory ${DIR_PATH}")
    add_subdirectory(${DIR_PATH})
endfunction()

enter_directory(${SQL_DIR})
enter_directory(${MONSTERS_PACK_DIR})
enter_directory(${PY_DIR})

message("Configuring target for defs.h")
add_custom_target(defs_h ALL DEPENDS ${SRC_DIR}/defs.h SOURCES ${DB_FILE})
message("Configuring target for tables.h")
add_custom_target(tables_h ALL DEPENDS ${SRC_DIR}/tables.h SOURCES ${DB_FILE})
message("Configuring target for database")
add_custom_target(adnd_db ALL DEPENDS ${DB_FILE} SOURCES ${DB_SCHEMA_FILE})

execute_process(COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:"%ad" --date=iso8601 OUTPUT_VARIABLE LAST_COMMIT_DATE
                COMMAND_ECHO STDOUT)
message("Last commit date: " ${LAST_COMMIT_DATE})
execute_process(COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:"%an" OUTPUT_VARIABLE LAST_COMMIT_AUTHOR_NAME
                COMMAND_ECHO STDOUT)
message("Commit author name: " ${LAST_COMMIT_AUTHOR_NAME})
execute_process(COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:"%H" OUTPUT_VARIABLE LAST_COMMIT_HASH
                COMMAND_ECHO STDOUT)
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

enter_directory(${SRC_DIR})
enter_directory(${DEMO_DIR})
enter_directory(${TEST_DIR})
enter_directory(${CLI_DIR})


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