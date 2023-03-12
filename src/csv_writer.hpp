#pragma once

#include "market_observer.hpp"

#include <ostream>

// CSV_Writer prints all market data receive to an output stream in CSV format
class CSV_Writer : public MarketObserver
{
public:
    CSV_Writer(std::shared_ptr<MarketNotifier>, std::ostream &output);

    void orderAccepted(const NewOrder &) override;
    void tradeExecuted(const Trade &) override;

private:
    std::ostream &output;
};
