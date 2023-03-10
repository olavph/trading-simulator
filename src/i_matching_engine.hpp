#pragma once

#include "i_order_types.hpp"

#include <string>
#include <vector>

struct IMatchingEngine
{
    virtual void insert(const NewOrder&) = 0;
    virtual void amend(const Order&) = 0;
    virtual void pull(order_id) = 0;

    // Get trades executed.
    virtual std::vector<Trade> getTrades() const = 0;
    // Get volumes of orders at each price level.
    virtual PriceLevels getPriceLevels() const = 0;
    // Get string summary of trades and price levels.
    virtual std::vector<std::string> getTradesAndPriceLevels() const = 0;
};
