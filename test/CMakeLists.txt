Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.1 # or a later release
)

FetchContent_MakeAvailable(Catch2)

include(CTest)
include(Catch)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)

add_executable(tests test_advt.cpp test_cexc.cpp test_coin.cpp test_dice.cpp test_expe.cpp test_hitp.cpp test_savt.cpp test_skil.cpp test_thac.cpp test_tund.cpp)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain adndtk)
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR/src})

catch_discover_tests(tests)

add_test(NAME tests COMMAND tests)

add_custom_command(TARGET tests POST_BUILD
    COMMAND ctest --config $<CONFIGURATION> #--exe $<TARGET_FILE:tests>
                  --verbosity high --parallel 16 --output-on-failure --
)