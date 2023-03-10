#include "../src/main.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("insert zero price") {
    auto input = std::vector<std::string>();

    input.emplace_back("INSERT,1,AAPL,BUY,0,5");

    auto result = run(input);

    REQUIRE(result.size() == 0);
}

TEST_CASE("insert duplicated order id") {
    auto input = std::vector<std::string>();

    input.emplace_back("INSERT,1,AAPL,BUY,12.2,5");
    input.emplace_back("INSERT,1,AAPL,BUY,10,6");

    auto result = run(input);

    REQUIRE(result.size() == 2);
    CHECK(result[0] == "===AAPL===");
    CHECK(result[1] == "12.2,5,,");
}

TEST_CASE("insert duplicated order id after pulling") {
    auto input = std::vector<std::string>();

    input.emplace_back("INSERT,1,AAPL,BUY,12.2,5");
    input.emplace_back("PULL,1");
    input.emplace_back("INSERT,1,AAPL,BUY,10,6");

    auto result = run(input);

    REQUIRE(result.size() == 2);
    CHECK(result[0] == "===AAPL===");
    CHECK(result[1] == "10,6,,");
}

TEST_CASE("pull") {
    auto input = std::vector<std::string>();

    input.emplace_back("INSERT,1,AAPL,BUY,12.2,5");
    input.emplace_back("PULL,1");

    auto result = run(input);

    REQUIRE(result.size() == 1);
    CHECK(result[0] == "===AAPL===");
}

TEST_CASE("pull when empty") {
    auto input = std::vector<std::string>();

    input.emplace_back("PULL,1");

    auto result = run(input);

    REQUIRE(result.size() == 0);
}

TEST_CASE("pull invalid") {
    auto input = std::vector<std::string>();

    input.emplace_back("INSERT,1,AAPL,BUY,12.2,5");
    input.emplace_back("PULL,2");

    auto result = run(input);

    REQUIRE(result.size() == 2);
    CHECK(result[0] == "===AAPL===");
    CHECK(result[1] == "12.2,5,,");
}

TEST_CASE("amend simple") {
    auto input = std::vector<std::string>();

    input.emplace_back("INSERT,1,GOOG,BUY,45.95,5");
    input.emplace_back("AMEND,1,46,3");

    auto result = run(input);

    REQUIRE(result.size() == 2);
    CHECK(result[0] == "===GOOG===");
    CHECK(result[1] == "46,3,,");
}

TEST_CASE("amend keep order") {
    auto input = std::vector<std::string>();

    input.emplace_back("INSERT,1,GOOG,BUY,45.95,5");
    input.emplace_back("INSERT,2,GOOG,BUY,45.95,10");
    input.emplace_back("AMEND,1,45.95,3");
    input.emplace_back("INSERT,3,GOOG,SELL,45,2");

    auto result = run(input);

    REQUIRE(result.size() == 3);
    CHECK(result[0] == "GOOG,45.95,2,3,1");
    CHECK(result[1] == "===GOOG===");
    CHECK(result[2] == "45.95,11,,");
}

TEST_CASE("amend volume and reorder") {
    auto input = std::vector<std::string>();

    input.emplace_back("INSERT,1,GOOG,BUY,45.95,5");
    input.emplace_back("INSERT,2,GOOG,BUY,45.95,10");
    input.emplace_back("AMEND,1,45.95,6");
    input.emplace_back("INSERT,3,GOOG,SELL,45,2");

    auto result = run(input);

    REQUIRE(result.size() == 3);
    CHECK(result[0] == "GOOG,45.95,2,3,2");
    CHECK(result[1] == "===GOOG===");
    CHECK(result[2] == "45.95,14,,");
}

TEST_CASE("amend price and reorder") {
    auto input = std::vector<std::string>();

    input.emplace_back("INSERT,1,GOOG,BUY,45.95,5");
    input.emplace_back("INSERT,2,GOOG,BUY,45.95,10");
    input.emplace_back("AMEND,1,45.5,5");
    input.emplace_back("INSERT,3,GOOG,SELL,45,2");

    auto result = run(input);

    REQUIRE(result.size() == 4);
    CHECK(result[0] == "GOOG,45.95,2,3,2");
    CHECK(result[1] == "===GOOG===");
    CHECK(result[2] == "45.95,8,,");
    CHECK(result[3] == "45.5,5,,");
}

TEST_CASE("amend and match lower volume") {
    auto input = std::vector<std::string>();

    input.emplace_back("INSERT,1,GOOG,BUY,45.95,5");
    input.emplace_back("INSERT,2,GOOG,SELL,46,5");
    input.emplace_back("AMEND,1,46,2");

    auto result = run(input);

    REQUIRE(result.size() == 3);
    CHECK(result[0] == "GOOG,46,2,1,2");
    CHECK(result[1] == "===GOOG===");
    CHECK(result[2] == ",,46,3");
}

TEST_CASE("amend and match higher volume") {
    auto input = std::vector<std::string>();

    input.emplace_back("INSERT,1,GOOG,BUY,45.95,5");
    input.emplace_back("INSERT,2,GOOG,SELL,46,5");
    input.emplace_back("AMEND,1,46,8");

    auto result = run(input);

    REQUIRE(result.size() == 3);
    CHECK(result[0] == "GOOG,46,5,1,2");
    CHECK(result[1] == "===GOOG===");
    CHECK(result[2] == "46,3,,");
}
