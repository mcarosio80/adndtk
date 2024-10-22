file (GLOB ADNDTK_LIB_SOURCES 
  "*.h"
  "*.cpp"
)

set(ADNDTK_LIB adndtk)

add_library(${ADNDTK_LIB} STATIC ${ADNDTK_LIB_SOURCES})

add_dependencies(${ADNDTK_LIB} rapidjson)

target_include_directories(${ADNDTK_LIB} PUBLIC ${SQLite3_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries (${ADNDTK_LIB} ${SQLite3_LIBRARIES})