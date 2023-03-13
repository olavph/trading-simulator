project(trading-simulator-tests)

Include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.1
)
FetchContent_MakeAvailable(Catch2)

add_executable(tests
    test_console_agent.cpp
    test_console_observer.cpp
    test_matching_engine.cpp
)
target_link_libraries(tests PRIVATE
    Catch2::Catch2WithMain
    trompeloeil::trompeloeil

    trading-simulator
)
target_include_directories(tests PRIVATE
    ../src
)
