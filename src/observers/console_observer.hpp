#pragma once

#include "market_observer.hpp"

#include <ostream>

// ConsoleObserver prints all market data receive to an output stream (e.g. cout)
class ConsoleObserver : public MarketObserver
{
public:
    ConsoleObserver(std::shared_ptr<MarketNotifier>, std::ostream &output);

    void orderAccepted(const NewOrder &) override;
    void tradeExecuted(const Trade &) override;

private:
    std::ostream &output;
};
