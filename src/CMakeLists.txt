add_library(adndtk STATIC adndtk.cpp dice.cpp skills.cpp)

target_include_directories(adndtk PUBLIC ${SQLite3_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (adndtk ${SQLite3_LIBRARIES})