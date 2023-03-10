#pragma once

#include <stdexcept>

#include "i_matching_engine.hpp"

std::vector<std::string> getTokens(std::string input)
{
    size_t tokenStart = 0;
    size_t tokenEnd = input.find(',', tokenStart);
    std::vector<std::string> tokens;

    while (tokenEnd != std::string::npos)
    {
        tokens.push_back(input.substr(tokenStart, tokenEnd - tokenStart));
        tokenStart = tokenEnd + 1;
        tokenEnd = input.find(',', tokenStart);
    }
    tokens.push_back(input.substr(tokenStart, tokenEnd - tokenStart));

    return tokens;
}

void parseInput(IMatchingEngine& engine, std::string input)
{
    auto tokens = getTokens(input);

    // INSERT,<order_id>,<symbol>,<side>,<price>,<volume>
    if (tokens[0] == "INSERT")
    {
        if (tokens.size() != 6)
            throw std::runtime_error("Invalid number of arguments for INSERT");

        Side side;
        if (tokens[3] == "BUY")
            side = Side::Buy;
        else if  (tokens[3] == "SELL")
            side = Side::Sell;
        else
            throw std::runtime_error("Invalid input format");

        NewOrder order{
            tokens[2],
            side,
            std::stoul(tokens[1]),
            std::stod(tokens[4]),
            std::stoul(tokens[5])
            };

        engine.insert(order);
    }
    // AMEND,<order_id>,<price>,<volume>
    else if (tokens[0] == "AMEND")
    {
        if (tokens.size() != 4)
            throw std::runtime_error("Invalid number of arguments for AMEND");

        Order order{
            std::stoul(tokens[1]),
            std::stod(tokens[2]),
            std::stoul(tokens[3])
            };

        engine.amend(order);
    }
    // PULL,<order_id>
    else if (tokens[0] == "PULL")
    {
        if (tokens.size() != 2)
            throw std::runtime_error("Invalid number of arguments for PULL");

        engine.pull(std::stoul(tokens[1]));
    }
    else
    {
        throw std::runtime_error("Invalid operation");
    }
}
