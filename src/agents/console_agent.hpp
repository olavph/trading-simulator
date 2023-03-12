#pragma once

#include "market_agent.hpp"

#include <istream>
#include <thread>

// ConsoleAgent parses input from a stream (e.g. cin) and sends orders
// to the MatchingEngine
class ConsoleAgent : public MarketAgent
{
public:
    ConsoleAgent(std::shared_ptr<IMatchingEngine> engine, std::istream & input);

    void run();
    void wait();
    void stop();
private:
    std::vector<std::string> getTokens(std::string input);
    void parseInput(std::string input);

    std::istream & input;
    std::jthread thread;
};
