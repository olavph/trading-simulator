#pragma once

#include "agents/market_agent.hpp"
#include "observers/market_observer.hpp"

#include <atomic>
#include <unordered_map>
#include <thread>

// RandomBot sends orders to the exchange without concerns on profitability
class RandomBot : public MarketObserver, MarketAgent
{
public:
    RandomBot(std::shared_ptr<MarketNotifier>, std::shared_ptr<IMatchingEngine> engine, agent_id bot_id);

    // MarketObserver
    void orderAccepted(const NewOrder &) override;
    void tradeExecuted(const Trade &) override;

    void run();
    void wait();
    void stop();

private:
    std::jthread thread;
    agent_id bot_id;
    std::unordered_map<Symbol, std::atomic<price_t>> market_prices;
};
