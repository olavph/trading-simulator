#include "console_observer.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("ConsoleObserver order accepted")
{
    auto notifier = std::make_shared<MarketNotifier>();
    std::stringstream sstream;
    ConsoleObserver observer{notifier, sstream};

    observer.orderAccepted({{"AAPL", Side::Buy}, {1, 2, 5}});

    REQUIRE(sstream.str() == "NEW ORDER: AAPL,BUY,1,2,5\n");
}

TEST_CASE("ConsoleObserver insert multiple orders accepted")
{
    auto notifier = std::make_shared<MarketNotifier>();
    std::stringstream sstream;
    ConsoleObserver observer{notifier, sstream};

    observer.orderAccepted({{"AAPL", Side::Buy}, {1, 2, 5}});
    observer.orderAccepted({{"AAPL", Side::Buy}, {2, 2, 5}});

    REQUIRE(sstream.str() == "NEW ORDER: AAPL,BUY,1,2,5\n"
                             "NEW ORDER: AAPL,BUY,2,2,5\n");
}

TEST_CASE("ConsoleObserver trade executed")
{
    auto notifier = std::make_shared<MarketNotifier>();
    std::stringstream sstream;
    ConsoleObserver observer{notifier, sstream};

    observer.tradeExecuted({"AAPL", 5, 2, 1, 2});

    REQUIRE(sstream.str() == "TRADE EXECUTED: AAPL,5,2,1,2\n");
}
