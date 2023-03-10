#pragma once

#include <string>
#include <vector>

using order_id = size_t;
using price_t = double;

enum class Side : bool
{
    Buy,
    Sell
};

struct Order
{
    order_id id;
    price_t price;
    size_t volume;
};

struct BookInfo
{
    std::string symbol;
    Side side;
};

struct NewOrder
{
    BookInfo book_info;
    Order order_info;
};

struct IMatchingEngine
{
    virtual void insert(const NewOrder&) = 0;
    virtual void amend(const Order&) = 0;
    virtual void pull(order_id) = 0;

    virtual std::vector<std::string> getTradesAndPriceLevels() = 0;
};
