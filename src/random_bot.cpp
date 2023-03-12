#include "random_bot.hpp"

#include <array>
#include <random>

using namespace std::chrono_literals;

constexpr std::array ALLOWED_SYMBOLS{"AAPL", "GOOG", "META", "MSFT"};

// Public

RandomBot::RandomBot(std::shared_ptr<IMatchingEngine> engine)
    : MarketAgent(engine)
{
}

void RandomBot::run()
{
    if (thread.joinable())
        return;

    thread = std::jthread([this](std::stop_token stop_token)
                          {
        order_id id = 1;
        while (!stop_token.stop_requested())
        {
            constexpr auto ORDER_INTERVAL = 1s;
            constexpr price_t min_price = 1.0;
            constexpr price_t max_price = 10.0;
            constexpr size_t min_volume = 1;
            constexpr size_t max_volume = 10;

            std::random_device rd;  // Will be used to obtain a seed for the random number engine
            std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
            std::uniform_int_distribution<size_t> symbol_distrib(0, ALLOWED_SYMBOLS.size() - 1);
            std::uniform_int_distribution<int> side_distrib(0, 1);
            std::uniform_real_distribution<price_t> price_distrib(min_price, max_price);
            std::uniform_int_distribution<size_t> volume_distrib(min_volume, max_volume);

            auto symbol = ALLOWED_SYMBOLS[symbol_distrib(gen)];
            auto side = static_cast<Side>(side_distrib(gen));
            auto price = price_distrib(gen);
            auto volume = volume_distrib(gen);
            NewOrder order{symbol, side, id, price, volume};

            insert(order);

            std::this_thread::sleep_for(ORDER_INTERVAL);
            ++id;
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
