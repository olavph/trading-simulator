add_library(trading-simulator
    agents/console_agent.cpp
    agents/market_agent.cpp
    bots/random_bot.cpp
    engine/matching_engine.cpp
    engine/order_book.cpp
    engine/order_types.cpp
    observers/console_observer.cpp
    observers/csv_writer.cpp
    observers/market_notifier.cpp
    observers/market_observer.cpp
)
target_include_directories(trading-simulator
    PUBLIC include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(trading-simulator-exec
    main.cpp
)
target_link_libraries(trading-simulator-exec PRIVATE
    trading-simulator
)
