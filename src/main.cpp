#include "agents/console_agent.hpp"
#include "agents/random_bot.hpp"
#include "engine/matching_engine.hpp"
#include "observers/console_observer.hpp"
#include "observers/csv_writer.hpp"
#include "observers/market_notifier.hpp"

#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

const std::string csv_file_name{"plotting/market_data.csv"};
constexpr size_t NUM_BOTS = 5;

int main(int argc, char const *argv[])
{
    auto notifier = std::make_shared<MarketNotifier>();
    auto engine = std::make_shared<MatchingEngine>(notifier);

    ConsoleObserver observer{notifier, std::cout};
    std::ofstream csv_file_stream{csv_file_name, std::ios_base::trunc | std::ios_base::out};
    CSV_Writer csv_writer{notifier, csv_file_stream};

    ConsoleAgent agent{engine, std::cin, 0};
    std::vector<RandomBot> bots;
    bots.reserve(NUM_BOTS);
    for (int i = 1; i <= NUM_BOTS; ++i)
        bots.emplace_back(engine, i);

    agent.run();
    for (auto &bot : bots)
        bot.run();

    agent.wait();
    return 0;
}
