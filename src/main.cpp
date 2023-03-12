#include "agents/console_agent.hpp"
#include "agents/random_bot.hpp"
#include "engine/matching_engine.hpp"
#include "observers/console_observer.hpp"
#include "observers/csv_writer.hpp"
#include "observers/market_notifier.hpp"

#include <fstream>
#include <iostream>
#include <memory>

const std::string csv_file_name{"plotting/market_data.csv"};

int main(int argc, char const *argv[])
{
    auto notifier = std::make_shared<MarketNotifier>();
    auto engine = std::make_shared<MatchingEngine>(notifier);

    ConsoleObserver observer{notifier, std::cout};
    std::ofstream csv_file_stream{csv_file_name, std::ios_base::trunc | std::ios_base::out};
    CSV_Writer csv_writer{notifier, csv_file_stream};

    ConsoleAgent agent{engine, std::cin};
    RandomBot bot{engine};

    agent.run();
    bot.run();

    agent.wait();
    return 0;
}
