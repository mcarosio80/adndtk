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

foreach(T IN ITEMS advt cexc coin dice expe hitp savt skil thac tund)
    message("Adding unit-test: " ${T})

    add_executable(${T} test_${T}.cpp)
    target_link_libraries(${T} PRIVATE Catch2::Catch2WithMain adndtk)
    target_include_directories(${T} PRIVATE ${CMAKE_SOURCE_DIR/src})

    catch_discover_tests(${T})

    add_test(NAME test_${T} COMMAND ${T})

    add_custom_command(TARGET ${T} POST_BUILD
        COMMAND ctest -C $<CONFIGURATION> --verbosity high --parallel 16 --output-on-failure --
    )
endforeach()
