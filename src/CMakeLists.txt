file (GLOB ADNDTK_LIB_SOURCES 
  "*.h"
  "*.cpp"
)

add_library(adndtk STATIC ${ADNDTK_LIB_SOURCES})

add_dependencies(adndtk rapidjson)

target_include_directories(adndtk PUBLIC ${SQLite3_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (adndtk ${SQLite3_LIBRARIES})