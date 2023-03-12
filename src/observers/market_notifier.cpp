#include "market_notifier.hpp"

#include <algorithm>

// Public

void MarketNotifier::subscribe(IMarketObserver * observer)
{
    observers.push_back(observer);
}

void MarketNotifier::unsubscribe(IMarketObserver * observer)
{
    if (auto observer_it = std::find(observers.begin(), observers.end(), observer); observer_it != observers.end())
    {
        observers.erase(observer_it);
    }
}

void MarketNotifier::orderAccepted(const NewOrder &order)
{
    for (auto &observer : observers)
    {
        observer->orderAccepted(order);
    }
}

void MarketNotifier::tradeExecuted(const Trade &trade)
{
    for (auto &observer : observers)
    {
        observer->tradeExecuted(trade);
    }
}
