add_library(adndtk STATIC adndtk.cpp)

target_include_directories(adndtk PUBLIC ${SQLite3_INCLUDE_DIRS})

target_link_libraries (adndtk ${SQLite3_LIBRARIES})