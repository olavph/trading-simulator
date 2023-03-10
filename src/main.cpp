#include "main.hpp"

#include "input_parser.hpp"
#include "matching_engine.hpp"

std::vector<std::string> run(std::vector<std::string> const& input)
{
    MatchingEngine engine;
    for (auto line : input)
    {
        parseInput(engine, line);
    }
    return engine.getTradesAndPriceLevels();
}
