
file (GLOB ADNDTK_LIB_HEADERS "*.h")
file (GLOB ADNDTK_LIB_SOURCES 
  ${ADNDTK_LIB_HEADERS}
  "*.cpp"
)

add_library(${ADNDTK_LIB} STATIC ${ADNDTK_LIB_SOURCES})

add_dependencies(${ADNDTK_LIB} rapidjson)

target_include_directories(${ADNDTK_LIB} PUBLIC ${SQLite3_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (${ADNDTK_LIB} ${SQLite3_LIBRARIES})


# Install library
set_target_properties(${ADNDTK_LIB} PROPERTIES OUTPUT_NAME ${ADNDTK_LIB}${CMAKE_PROJECT_VERSION})
install(TARGETS ${ADNDTK_LIB}
    LIBRARY
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/${ADNDTK_LIB}/${CMAKE_PROJECT_VERSION}/bin"
)

# # Install headers
target_sources(${ADNDTK_LIB} PUBLIC ${ADNDTK_LIB_HEADERS})
get_target_property(ADNDTK_PUBLIC_HEADERS ${ADNDTK_LIB} INTERFACE_SOURCES)
set_target_properties(${ADNDTK_LIB} PROPERTIES PUBLIC_HEADER "${ADNDTK_PUBLIC_HEADERS}")
install(TARGETS ${ADNDTK_LIB}
    PUBLIC_HEADER
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${ADNDTK_LIB}"
)


# Overwrite deployable 'config.h' header file
install(
    FILES
        ${DEPLOY_DIR}/config.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${ADNDTK_LIB}
)