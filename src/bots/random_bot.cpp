#include "random_bot.hpp"

#include <array>
#include <random>

using namespace std::chrono_literals;

constexpr size_t num_symbols = 4;
constexpr std::array<std::string, num_symbols> allowed_symbols{"AAPL", "GOOG", "META", "MSFT"};
constexpr std::array<price_t, num_symbols> start_prices{110.0, 120.0, 130.0, 140.0};
constexpr auto order_interval = 100ms;
constexpr price_t price_step_delta = 1.0;
constexpr price_t market_price_delta = 5.0;
constexpr size_t min_volume = 1;
constexpr size_t max_volume = 10;

// Public

RandomBot::RandomBot(std::shared_ptr<MarketNotifier> notifier, std::shared_ptr<IMatchingEngine> engine, agent_id bot_id)
    : MarketObserver(notifier),
      MarketAgent(engine),
      bot_id(bot_id)
{
    for (size_t i = 0; i < num_symbols; ++i)
    {
        market_prices.emplace(allowed_symbols[i], start_prices[i]);
    }
}

void RandomBot::orderAccepted(const NewOrder &order)
{
}

void RandomBot::tradeExecuted(const Trade &trade)
{
    market_prices[trade.symbol].store(trade.price, std::memory_order::relaxed);
}

void RandomBot::run()
{
    if (thread.joinable())
        return;

    thread = std::jthread([this](std::stop_token stop_token)
                          {
        std::random_device rd;  // Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
        std::uniform_int_distribution<size_t> symbol_distrib(0, allowed_symbols.size() - 1);
        std::uniform_int_distribution<int> side_distrib(0, 1);
        std::uniform_int_distribution<size_t> volume_distrib(min_volume, max_volume);

        order_id id{bot_id, 0};
        auto last_prices = start_prices;

        while (!stop_token.stop_requested())
        {
            auto symbol_index = symbol_distrib(gen);
            auto last_market_price = market_prices[allowed_symbols[symbol_index]].load(std::memory_order::relaxed);
            std::uniform_real_distribution<price_t> price_distrib(
                std::max(last_market_price - market_price_delta, last_prices[symbol_index] - price_step_delta),
                std::min(last_market_price + market_price_delta, last_prices[symbol_index] + price_step_delta));

            auto symbol = allowed_symbols[symbol_index];
            auto side = static_cast<Side>(side_distrib(gen));
            auto price = price_distrib(gen);
            auto volume = volume_distrib(gen);
            NewOrder order{symbol, side, id, price, volume};

            insert(order);

            std::this_thread::sleep_for(order_interval);
            ++id.seq;
            last_prices[symbol_index] = price;
        } });
}

void RandomBot::wait()
{
    thread.join();
}

void RandomBot::stop()
{
    thread.request_stop();
}
