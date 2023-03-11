#pragma once

#include "i_matching_engine.hpp"

#include <memory>

// A MarketAgent sends orders to the MatchingEngine.
// Sub-classes define their behavior.
class MarketAgent
{
public:
    MarketAgent(std::shared_ptr<IMatchingEngine>);

    void insert(const NewOrder&);
    void amend(const Order&);
    void pull(order_id);

private:
    std::shared_ptr<IMatchingEngine> matching_engine;
};
