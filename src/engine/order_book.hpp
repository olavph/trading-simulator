#pragma once

#include "i_order_book.hpp"

#include <set>

template <class OrderCompare>
class OrderBook : public IOrderBook
{
public:
    std::pair<Order, std::vector<Trade>> match(const Order&, const std::string& symbol) override;
    void insert(Order) override;
    bool amend(Order, price_t price_hint) override;
    void pull(order_id, price_t price_hint) override;
    std::vector<PriceLevel> getPriceLevels() const override;

private:
    std::multiset<Order, OrderCompare>::iterator find(order_id, price_t price_hint) const;

    std::multiset<Order, OrderCompare> orders;
};

struct OrderCompareLesser
{
    bool operator()(const Order& a, const Order& b) const
    {
        return a.price < b.price;
    }
};

struct OrderCompareGreater
{
    bool operator()(const Order& a, const Order& b) const
    {
        return a.price > b.price;
    }
};

// Instantiate necessary class templates, defined in source file
template class OrderBook<OrderCompareGreater>;
template class OrderBook<OrderCompareLesser>;

struct CentralLimitOrderBook
{
    OrderBook<OrderCompareGreater> buys;
    OrderBook<OrderCompareLesser> sells;

    IOrderBook& getOneSidedBook(Side);
};
