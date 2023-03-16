Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.1 # or a later release
)

FetchContent_MakeAvailable(Catch2)

add_executable(tests test.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain adndtk)
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR/src})

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)
catch_discover_tests(tests)

add_custom_command(TARGET tests POST_BUILD
  COMMAND ctest -C $<CONFIGURATION> --verbosity high #--output-on-failure --
)