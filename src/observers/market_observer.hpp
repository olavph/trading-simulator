#pragma once

#include "i_market_observer.hpp"
#include "market_notifier.hpp"

#include <memory>

class MarketObserver : public IMarketObserver
{
public:
    MarketObserver(std::shared_ptr<MarketNotifier>);
    virtual ~MarketObserver();
    MarketObserver(const MarketObserver &) = delete;
    MarketObserver(MarketObserver &&) = delete;
    MarketObserver &operator=(const MarketObserver &) = delete;
    MarketObserver &operator=(MarketObserver &&) = delete;

private:
    std::shared_ptr<MarketNotifier> market_notifier;
};
