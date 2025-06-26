message("Configuring library header generation: ${SRC_DIR}/defs.h")
execute_process(
    COMMAND ${PYTHON_EXECUTABLE} ${PY_DIR}/generate_defs.py
                --database ${DB_FILE}
                --json ${PY_DIR}/tables_info.json
                --output ${SRC_DIR}/defs.h
                --namespace Adndtk:Defs
                --version ${PROJECT_VERSION}
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE CPP_CREATE_HEADER_DEFS_H
                COMMAND_ECHO STDOUT)
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
            RESULT_VARIABLE CPP_CREATE_HEADER_DICTIONARY_H
            COMMAND_ECHO STDOUT)
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
            RESULT_VARIABLE CPP_CREATE_HEADER_TABLES_H
            COMMAND_ECHO STDOUT)
if(NOT CPP_CREATE_HEADER_TABLES_H EQUAL "0")
    message(FATAL_ERROR "${PYTHON_EXECUTABLE} ${PY_DIR}/generate_tables.py --database ${DB_FILE} --json ${PY_DIR}/orm_mapping.json --output ${SRC_DIR}/tables.h --namespace Adndtk:Tables --version ${PROJECT_VERSION} failed with ${CPP_CREATE_HEADER_TABLES_H}")
endif()