#pragma once

#include "i_order_types.hpp"

// An IMarketObserver receives execution data from the MatchingEngine.
struct IMarketObserver
{
    virtual void orderAccepted(const NewOrder &) = 0;
    virtual void tradeExecuted(const Trade &) = 0;
};
