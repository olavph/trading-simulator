#include "market_agent.hpp"

MarketAgent::MarketAgent(std::shared_ptr<IMatchingEngine> engine) :
    matching_engine(engine)
{}

void MarketAgent::insert(const NewOrder& order)
{
    matching_engine->insert(order);
}

void MarketAgent::amend(const Order& order)
{
    matching_engine->amend(order);
}

void MarketAgent::pull(order_id id)
{
    matching_engine->pull(id);
}
