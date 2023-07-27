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

add_executable(tests test_advt.cpp test_cexc.cpp test_coin.cpp test_dice.cpp
                    test_expe.cpp test_hitp.cpp test_savt.cpp test_skil.cpp
                    test_thac.cpp test_tund.cpp test_monb.cpp test_tabi.cpp
                    test_inve.cpp test_skst.cpp test_splb.cpp test_hols.cpp)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain adndtk)
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR/src})

catch_discover_tests(tests)

add_test(NAME tests COMMAND tests)

if(RUN_UNIT_TESTS)
    message("Executing unit tests after build (disable with -DRUN_UNIT_TESTS=OFF)")
    
    add_custom_command(TARGET tests POST_BUILD
    COMMAND ctest --config $<CONFIGURATION> #--exe $<TARGET_FILE:tests>
                  --verbosity high --parallel 16 --output-on-failure --
)
else()
    message("Skip unit-tests (enable with -DRUN_UNIT_TESTS=ON)")
endif(RUN_UNIT_TESTS)

unset(RUN_UNIT_TESTS CACHE)