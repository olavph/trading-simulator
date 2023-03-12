#pragma once

#include "i_market_observer.hpp"

#include <memory>
#include <vector>

class MarketNotifier : public IMarketObserver
{
public:
    void subscribe(IMarketObserver *);
    void unsubscribe(IMarketObserver *);

    void orderAccepted(const NewOrder &) override;
    void tradeExecuted(const Trade &) override;

private:
    std::vector<IMarketObserver *> observers;
};
