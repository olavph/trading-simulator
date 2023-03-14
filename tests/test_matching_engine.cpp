#include "engine/matching_engine.hpp"
#include "observers/market_notifier.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("MatchingEngine") {
    auto notifier = std::make_shared<MarketNotifier>();
    MatchingEngine engine{notifier};

    SECTION("insert zero price")
    {
        engine.insert({{"AAPL", Side::Buy}, {{0, 1}, 0, 5}});

        auto result = engine.getTradesAndPriceLevels();

        REQUIRE(result.size() == 0);
    }

    SECTION("insert duplicated order id")
    {
        engine.insert({{"AAPL", Side::Buy}, {{0, 1}, 12.2, 5}});
        engine.insert({{"AAPL", Side::Buy}, {{0, 1}, 10.0, 6}});

        auto result = engine.getTradesAndPriceLevels();

        REQUIRE(result.size() == 2);
        CHECK(result[0] == "===AAPL===");
        CHECK(result[1] == "12.2,5,,");
    }

    SECTION("insert duplicated order id after pulling")
    {
        engine.insert({{"AAPL", Side::Buy}, {{0, 1}, 12.2, 5}});
        engine.pull({0, 1});
        engine.insert({{"AAPL", Side::Buy}, {{0, 1}, 10.0, 6}});

        auto result = engine.getTradesAndPriceLevels();

        REQUIRE(result.size() == 2);
        CHECK(result[0] == "===AAPL===");
        CHECK(result[1] == "10,6,,");
    }

    SECTION("pull")
    {
        engine.insert({{"AAPL", Side::Buy}, {{0, 1}, 12.2, 5}});
        engine.pull({0, 1});

        auto result = engine.getTradesAndPriceLevels();

        REQUIRE(result.size() == 1);
        CHECK(result[0] == "===AAPL===");
    }

    SECTION("pull when empty")
    {
        engine.pull({0, 1});

        auto result = engine.getTradesAndPriceLevels();

        REQUIRE(result.size() == 0);
    }

    SECTION("pull invalid")
    {
        engine.insert({{"AAPL", Side::Buy}, {{0, 1}, 12.2, 5}});
        engine.pull({0, 2});

        auto result = engine.getTradesAndPriceLevels();

        REQUIRE(result.size() == 2);
        CHECK(result[0] == "===AAPL===");
        CHECK(result[1] == "12.2,5,,");
    }

    SECTION("amend simple")
    {
        engine.insert({{"GOOG", Side::Buy}, {{0, 1}, 45.95, 5}});
        engine.amend({{0, 1}, 46.0, 3});

        auto result = engine.getTradesAndPriceLevels();

        REQUIRE(result.size() == 2);
        CHECK(result[0] == "===GOOG===");
        CHECK(result[1] == "46,3,,");
    }

    SECTION("amend keep order")
    {
        engine.insert({{"GOOG", Side::Buy}, {{0, 1}, 45.95, 5}});
        engine.insert({{"GOOG", Side::Buy}, {{0, 2}, 45.95, 10}});
        engine.amend({{0, 1}, 45.95, 3});
        engine.insert({{"GOOG", Side::Sell}, {{0, 3}, 45, 2}});

        auto result = engine.getTradesAndPriceLevels();

        REQUIRE(result.size() == 3);
        CHECK(result[0] == "GOOG,45.95,2,0,3,0,1");
        CHECK(result[1] == "===GOOG===");
        CHECK(result[2] == "45.95,11,,");
    }

    SECTION("amend volume and reorder")
    {
        engine.insert({{"GOOG", Side::Buy}, {{0, 1}, 45.95, 5}});
        engine.insert({{"GOOG", Side::Buy}, {{0, 2}, 45.95, 10}});
        engine.amend({{0, 1}, 45.95, 6});
        engine.insert({{"GOOG", Side::Sell}, {{0, 3}, 45, 2}});

        auto result = engine.getTradesAndPriceLevels();

        REQUIRE(result.size() == 3);
        CHECK(result[0] == "GOOG,45.95,2,0,3,0,2");
        CHECK(result[1] == "===GOOG===");
        CHECK(result[2] == "45.95,14,,");
    }

    SECTION("amend price and reorder")
    {
        engine.insert({{"GOOG", Side::Buy}, {{0, 1}, 45.95, 5}});
        engine.insert({{"GOOG", Side::Buy}, {{0, 2}, 45.95, 10}});
        engine.amend({{0, 1}, 45.5, 5});
        engine.insert({{"GOOG", Side::Sell}, {{0, 3}, 45, 2}});

        auto result = engine.getTradesAndPriceLevels();

        REQUIRE(result.size() == 4);
        CHECK(result[0] == "GOOG,45.95,2,0,3,0,2");
        CHECK(result[1] == "===GOOG===");
        CHECK(result[2] == "45.95,8,,");
        CHECK(result[3] == "45.5,5,,");
    }

    SECTION("amend and match lower volume")
    {
        engine.insert({{"GOOG", Side::Buy}, {{0, 1}, 45.95, 5}});
        engine.insert({{"GOOG", Side::Sell}, {{0, 2}, 46, 5}});
        engine.amend({{0, 1}, 46, 2});

        auto result = engine.getTradesAndPriceLevels();

        REQUIRE(result.size() == 3);
        CHECK(result[0] == "GOOG,46,2,0,1,0,2");
        CHECK(result[1] == "===GOOG===");
        CHECK(result[2] == ",,46,3");
    }

    SECTION("amend and match higher volume")
    {
        engine.insert({{"GOOG", Side::Buy}, {{0, 1}, 45.95, 5}});
        engine.insert({{"GOOG", Side::Sell}, {{0, 2}, 46, 5}});
        engine.amend({{0, 1}, 46, 8});

        auto result = engine.getTradesAndPriceLevels();

        REQUIRE(result.size() == 3);
        CHECK(result[0] == "GOOG,46,5,0,1,0,2");
        CHECK(result[1] == "===GOOG===");
        CHECK(result[2] == "46,3,,");
    }
}
