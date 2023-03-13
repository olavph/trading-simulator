#include "matching_engine.hpp"

// OrderBook

template <class OrderCompare>
std::pair<Order, std::vector<Trade>> OrderBook<OrderCompare>::match(
    const Order& order, const Symbol& symbol)
{
    Order remainder{ order };
    std::vector<Trade> trades;
    OrderCompare compare;
    auto order_it = orders.begin();
    while (order_it != orders.end() && remainder.volume > 0)
    {
        const Order& matching_order = *order_it;
        if (!compare(remainder, matching_order))
        {
            size_t volume = std::min(matching_order.volume, remainder.volume);
            remainder.volume -= volume;

            trades.emplace_back(symbol, matching_order.price, volume, remainder.id, matching_order.id);

            auto insert_position = order_it;
            ++insert_position;
            orders.erase(order_it);
            if (matching_order.volume > volume)
            {
                // Reinsert at the same position with updated volume
                Order new_matched_order{ matching_order };
                new_matched_order.volume -= volume;
                orders.insert(insert_position, new_matched_order);
            }
            order_it = insert_position;
        }
        else
        {
            // No match anymore
            order_it = orders.end();
        }
    }
    return std::make_pair(std::move(remainder), std::move(trades));
}

template <class OrderCompare>
void OrderBook<OrderCompare>::insert(Order order)
{
    orders.insert(order);
}

template <class OrderCompare>
bool OrderBook<OrderCompare>::amend(Order amended_order, price_t price_hint)
{
    auto order_it = find(amended_order.id, price_hint);
    if (order_it == orders.end())
        return false;

    if (price_hint == amended_order.price)
    {
        if (amended_order.volume <= (*order_it).volume)
        {
            // Keep priority
            auto insert_position = order_it;
            ++insert_position;
            orders.erase(order_it);
            orders.insert(insert_position, amended_order);
        }
        else
        {
            // Re-prioritize
            orders.erase(order_it);
            orders.insert(amended_order);
        }
        return false;
    }
    else
    {
        // Remove and cause a re-evaluation
        orders.erase(order_it);
        return true;
    }
}

template <class OrderCompare>
void OrderBook<OrderCompare>::pull(order_id id, price_t price_hint)
{
    auto order_it = find(id, price_hint);
    if (order_it == orders.end())
        return;

    orders.erase(order_it);
}

template <class OrderCompare>
std::vector<PriceLevel> OrderBook<OrderCompare>::getPriceLevels() const
{
    std::vector<PriceLevel> levels;
    price_t last_price{ 0 };
    for (const auto& order : orders)
    {
        if (last_price != 0 && order.price == last_price)
        {
            levels.back().volume += order.volume;
        }
        else
        {
            levels.push_back({order.price, order.volume});
            last_price = order.price;
        }
    }
    return levels;
}

// OrderBook - private

template <class OrderCompare>
std::multiset<Order, OrderCompare>::iterator OrderBook<OrderCompare>::find(
    order_id id, price_t price_hint) const
{
    auto order_it = orders.find({0, price_hint, 0});
    while (order_it != orders.end() && (*order_it).price == price_hint)
    {
        if ((*order_it).id == id)
        {
            return order_it;
        }
        ++order_it;
    }
    return orders.end();
}

// CentralLimitOrderBook

IOrderBook& CentralLimitOrderBook::getOneSidedBook(Side side)
{
    return (side == Side::Buy) ? static_cast<IOrderBook&>(buys) : static_cast<IOrderBook&>(sells);
}
