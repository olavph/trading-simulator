#include "agents/console_agent.hpp"

#include "mock_matching_engine.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("ConsoleAgent no orders")
{
    auto mock_engine = std::make_shared<MockMatchingEngine>();
    std::stringstream sstream;
    ConsoleAgent console(mock_engine, sstream, 0);

    console.run();
    console.wait();
}

TEST_CASE("ConsoleAgent insert order")
{
    auto mock_engine = std::make_shared<MockMatchingEngine>();
    std::stringstream sstream("INSERT,1,AAPL,BUY,2,5");
    ConsoleAgent console(mock_engine, sstream, 0);

    NewOrder expected_order{{"AAPL", Side::Buy}, {{0, 1}, 2, 5}};
    REQUIRE_CALL(*mock_engine, insert(expected_order));

    console.run();
    console.wait();
}

TEST_CASE("ConsoleAgent pull order")
{
    auto mock_engine = std::make_shared<MockMatchingEngine>();
    std::stringstream sstream("PULL,1");
    ConsoleAgent console(mock_engine, sstream, 0);

    order_id expected_order_id{0, 1};
    REQUIRE_CALL(*mock_engine, pull(expected_order_id));

    console.run();
    console.wait();
}

TEST_CASE("ConsoleAgent amend order")
{
    auto mock_engine = std::make_shared<MockMatchingEngine>();
    std::stringstream sstream("AMEND,1,2,5");
    ConsoleAgent console(mock_engine, sstream, 0);

    Order expected_order{{0, 1}, 2, 5};
    REQUIRE_CALL(*mock_engine, amend(expected_order));

    console.run();
    console.wait();
}
