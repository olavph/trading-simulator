#include "console_agent.hpp"
#include "console_observer.hpp"
#include "market_notifier.hpp"
#include "matching_engine.hpp"

#include <iostream>
#include <memory>

int main(int argc, char const *argv[])
{
    auto notifier = std::make_shared<MarketNotifier>();
    ConsoleObserver observer{notifier, std::cout};
    auto engine = std::make_shared<MatchingEngine>(notifier);
    ConsoleAgent agent{engine, std::cin};
    agent.run();
    agent.wait();
    return 0;
}
