#include "random_bot.hpp"

#include <array>
#include <random>

using namespace std::chrono_literals;

constexpr std::array allowed_symbols{"AAPL", "GOOG", "META", "MSFT"};
constexpr auto order_interval = 100ms;
constexpr price_t min_price = 1.0;
constexpr price_t max_price = 100.0;
constexpr price_t start_price = 50.0;
constexpr price_t price_step_delta = 1.0;
constexpr size_t min_volume = 1;
constexpr size_t max_volume = 10;

// Public

RandomBot::RandomBot(std::shared_ptr<IMatchingEngine> engine, agent_id bot_id)
    : MarketAgent(engine),
      bot_id(bot_id)
{
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
        std::array<price_t, allowed_symbols.size()> last_prices;
        last_prices.fill(start_price);

        while (!stop_token.stop_requested())
        {
            auto symbol_index = symbol_distrib(gen);
            std::uniform_real_distribution<price_t> price_distrib(
                std::max(min_price, last_prices[symbol_index] - price_step_delta),
                std::min(max_price, last_prices[symbol_index] + price_step_delta));

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
