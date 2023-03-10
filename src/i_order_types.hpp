#pragma once

#include <map>
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

struct PriceLevelsForSymbol
{
    std::vector<PriceLevel> buys;
    std::vector<PriceLevel> sells;
};

struct PriceLevels
{
    std::map<std::string, PriceLevelsForSymbol> symbols;
};