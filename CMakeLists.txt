cmake_minimum_required(VERSION 3.0.0)
set (CMAKE_CXX_STANDARD 20)
project(trading-simulator)

include(CTest)
enable_testing()

add_library(trading-simulator
    src/console_agent.cpp
    src/console_observer.cpp
    src/csv_writer.cpp
    src/market_agent.cpp
    src/market_notifier.cpp
    src/market_observer.cpp
    src/matching_engine.cpp
    src/order_book.cpp
    src/random_bot.cpp
    src/runner.cpp
)

add_executable(trading-simulator-exec
    src/main.cpp
)
target_link_libraries(trading-simulator-exec PRIVATE
    trading-simulator
)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

add_subdirectory(tests)
add_subdirectory(third_party/trompeloeil)
