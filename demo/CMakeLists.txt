add_executable(demo_adndtk demo.cpp)

target_include_directories(demo_adndtk
        PUBLIC ${CMAKE_SOURCE_DIR}/src
        #PUBLIC ${SQLite3_INCLUDE_DIRS}
)

target_link_directories(demo_adndtk
        PUBLIC ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(demo_adndtk adndtk ${SQLite3_LIBRARIES})