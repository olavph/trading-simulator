#pragma once

#include "i_market_observer.hpp"

#include <condition_variable>
#include <queue>
#include <thread>
#include <memory>
#include <mutex>
#include <vector>

class MarketNotifier : public IMarketObserver
{
public:
    MarketNotifier();

    void subscribe(IMarketObserver *);
    void unsubscribe(IMarketObserver *);

    void orderAccepted(const NewOrder &) override;
    void tradeExecuted(const Trade &) override;

private:
    void run();

    std::vector<IMarketObserver *> observers;
    std::jthread thread;
    std::mutex mtx;
    std::condition_variable cv;
    std::queue<NewOrder> order_queue;
    std::queue<Trade> trade_queue;
};
