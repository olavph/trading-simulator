#pragma once

#include "i_matching_engine.hpp"

#include <string>
#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <catch2/trompeloeil.hpp>

struct MockMatchingEngine : public IMatchingEngine
{
    MAKE_MOCK1(insert, void (const NewOrder&));
    MAKE_MOCK1(amend, void (const Order&));
    MAKE_MOCK1(pull, void (order_id));

    MAKE_MOCK0(getTrades, std::vector<Trade> (), const);
    MAKE_MOCK0(getPriceLevels, PriceLevels (), const);
    MAKE_MOCK0(getTradesAndPriceLevels, std::vector<std::string> (), const);
};
