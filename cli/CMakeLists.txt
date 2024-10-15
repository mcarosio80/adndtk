file (GLOB CLI_TEST_SOURCES 
  "*.h"
  "*.cpp"
)

find_package(Boost 1.74 COMPONENTS program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

set(CLI_TARGET adndtk-cli)
add_executable(${CLI_TARGET} ${CLI_TEST_SOURCES})

target_include_directories(${CLI_TARGET}
        PUBLIC ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_directories(${CLI_TARGET}
        PUBLIC ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(${CLI_TARGET} adndtk ${Boost_LIBRARIES})