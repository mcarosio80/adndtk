file (GLOB DEMO_SOURCES "*.cpp")

SET(DEMO_TARGET "adndtk-demo")

add_executable(${DEMO_TARGET} ${DEMO_SOURCES})

target_include_directories(${DEMO_TARGET}
        PUBLIC ${CMAKE_SOURCE_DIR}/src
)

target_link_directories(${DEMO_TARGET}
        PUBLIC ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(${DEMO_TARGET} adndtk ${SQLite3_LIBRARIES})