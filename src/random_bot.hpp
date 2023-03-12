#pragma once

#include "market_agent.hpp"

#include <thread>

// RandomBot sends orders to the exchange without concerns on profitability
class RandomBot : public MarketAgent
{
public:
    RandomBot(std::shared_ptr<IMatchingEngine> engine);

    void run();
    void wait();
    void stop();

private:
    std::jthread thread;
};
