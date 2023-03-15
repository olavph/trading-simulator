#include "market_notifier.hpp"

#include <algorithm>
#include <iostream>

// Public

MarketNotifier::MarketNotifier()
{
    run();
}

void MarketNotifier::subscribe(IMarketObserver *observer)
{
    observers.push_back(observer);
}

void MarketNotifier::unsubscribe(IMarketObserver *observer)
{
    if (auto observer_it = std::find(observers.begin(), observers.end(), observer); observer_it != observers.end())
    {
        observers.erase(observer_it);
    }
}

void MarketNotifier::orderAccepted(const NewOrder &order)
{
    order_queue.push(order);
    cv.notify_one();
}

void MarketNotifier::tradeExecuted(const Trade &trade)
{
    trade_queue.push(trade);
    cv.notify_one();
}

// Private

void MarketNotifier::run()
{
    thread = std::jthread([this](std::stop_token stop_token)
    {
        while (!stop_token.stop_requested())
        {
            std::unique_lock lock(mtx);
            cv.wait(lock);
            while (!order_queue.empty())
            {
                for (auto &observer : observers)
                {
                    observer->orderAccepted(order_queue.front());
                }
                order_queue.pop();
            }
            while (!trade_queue.empty())
            {
                for (auto &observer : observers)
                {
                    observer->tradeExecuted(trade_queue.front());
                }
                trade_queue.pop();
            }
        }
    });
}
