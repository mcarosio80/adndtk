file (GLOB ADNDTK_LIB_HEADERS "*.h")
file (GLOB ADNDTK_LIB_SOURCES 
  ${ADNDTK_LIB_HEADERS}
  "*.cpp"
)

add_library(${ADNDTK_LIB} STATIC ${ADNDTK_LIB_SOURCES})

add_dependencies(${ADNDTK_LIB} rapidjson)

target_include_directories(${ADNDTK_LIB} PUBLIC ${SQLite3_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (${ADNDTK_LIB} ${SQLite3_LIBRARIES})


function(add_header SRC_FILE)
    get_filename_component(FILE_NAME ${SRC_FILE} NAME)
    file(READ ${CMAKE_SOURCE_DIR}/LICENSE.md HEADER)
    file(READ ${SRC_FILE} SOURCE)
    set(OUT_FILE ${DEPLOY_DIR}/${FILE_NAME})
    file(WRITE ${OUT_FILE} "/**")
    file(APPEND ${OUT_FILE} "\n${HEADER}")
    file(APPEND ${OUT_FILE} "*/\n\n")
    file(APPEND ${OUT_FILE} "${SOURCE}")
endfunction()

message("Adding headers")
foreach(FILE_PATH ${ADNDTK_LIB_SOURCES})
    add_header(${FILE_PATH})
endforeach()


# Install library
set_target_properties(${ADNDTK_LIB} PROPERTIES OUTPUT_NAME ${ADNDTK_LIB}${CMAKE_PROJECT_VERSION})
install(TARGETS ${ADNDTK_LIB}
    LIBRARY
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/${ADNDTK_LIB}/${CMAKE_PROJECT_VERSION}/bin"
)

# Overwrite deployable header files with license banners
file (GLOB ADNDTK_LIB_DEPLOYABLE_SOURCES
  "${DEPLOY_DIR}/*.h"
)

install(
    FILES
        ${ADNDTK_LIB_DEPLOYABLE_SOURCES}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${ADNDTK_LIB}
)