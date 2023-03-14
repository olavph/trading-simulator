#pragma once

#include <map>
#include <string>
#include <vector>

using agent_id = size_t;
using seq_num = size_t;
using price_t = double;
using Symbol = std::string;

enum class Side : bool
{
    Buy,
    Sell
};

struct order_id
{
    agent_id agent;
    seq_num seq;

    auto operator<=>(const order_id&) const = default;
};
std::ostream &operator<<(std::ostream &os, const order_id &id);

struct Order
{
    order_id id;
    price_t price;
    size_t volume;

    auto operator<=>(const Order&) const = default;
};

struct BookInfo
{
    Symbol symbol;
    Side side;

    auto operator<=>(const BookInfo&) const = default;
};

struct NewOrder
{
    BookInfo book_info;
    Order order_info;

    auto operator<=>(const NewOrder&) const = default;
};

struct Trade
{
    Symbol symbol;
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
    std::map<Symbol, PriceLevelsForSymbol> symbols;
};
