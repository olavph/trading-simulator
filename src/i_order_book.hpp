#pragma once

#include "i_matching_engine.hpp"

#include <string>
#include <vector>
#include <utility>

struct Trade
{
    std::string symbol;
    price_t price;
    size_t volume;
    order_id aggressive;
    order_id passive;
};

struct PriceLevel
{
    price_t price;
    size_t volume;
};

struct IOrderBook
{
    // Look for matches for the given order, removing the traded orders from the book.
    // Return the remainder of the order to be put in the opposite book, along with the
    // trades executed.
    virtual std::pair<Order, std::vector<Trade>> match(const Order&, const std::string& symbol) = 0;
    // Insert the order in the book.
    virtual void insert(Order) = 0;
    // Amend order, returning true if it needs to be re-evaluated for matches.
    // Only searches at the hinted price.
    virtual bool amend(Order, price_t price_hint) = 0;
    // Remove order. Only searches at the hinted price.
    virtual void pull(order_id, price_t price_hint) = 0;
    // Get volumes of orders at each price level.
    virtual std::vector<PriceLevel> getPriceLevels() const = 0;
};
