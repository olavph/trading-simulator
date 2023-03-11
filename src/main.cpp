#include "console_agent.hpp"
#include "matching_engine.hpp"

#include <iostream>
#include <memory>

int main(int argc, char const *argv[])
{
    auto engine = std::make_shared<MatchingEngine>();
    ConsoleAgent agent(engine, std::cin);
    agent.run();
    agent.wait();
    return 0;
}
