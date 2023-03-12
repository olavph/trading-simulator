#include "runner.hpp"

#include "input_parser.hpp"
#include "market_notifier.hpp"
#include "matching_engine.hpp"

std::vector<std::string> run(std::vector<std::string> const &input)
{
    auto notifier = std::make_shared<MarketNotifier>();
    MatchingEngine engine{notifier};
    for (auto line : input)
    {
        parseInput(engine, line);
    }
    return engine.getTradesAndPriceLevels();
}
