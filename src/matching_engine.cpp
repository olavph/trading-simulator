#include "matching_engine.hpp"

#include <functional>
#include <sstream>

namespace
{
    // Format the price into a string with a precision of NUMBER_OF_DIGITS *after* the dot
    // and without trailing zeroes
    std::string format_price(price_t price)
    {
        static constexpr int NUMBER_OF_DIGITS = 4;
        size_t price_int = price;
        std::ostringstream line_stream;
        // The stream formatter counts the precision also for digits to the left of the dot,
        // so we need to split the integer and fraction part
        line_stream.precision(NUMBER_OF_DIGITS);
        line_stream << price_int;
        // Go back to match dot position
        line_stream.seekp(-1, line_stream.cur);
        line_stream << price - price_int;
        // Overwrite 0 before the dot
        line_stream.seekp(0);
        line_stream << price_int;
        return line_stream.str();
    }
}

// Side

Side oppositeSide(Side s)
{
    return static_cast<Side>(!static_cast<bool>(s));
}

// MatchingEngine

void MatchingEngine::insert(const NewOrder& new_order)
{
    if (id_to_book_map.contains(new_order.order_info.id))
        // Do not duplicate orders
        return;
    if (new_order.order_info.price == 0)
        // Invalid price
        return;

    auto& doubleSidedBook = order_books[new_order.book_info.symbol];
    auto& oppositeBook = doubleSidedBook.getOneSidedBook(oppositeSide(new_order.book_info.side));
    auto [matchRemainder, trades] = oppositeBook.match(new_order.order_info, new_order.book_info.symbol);
    if (matchRemainder.volume > 0)
    {
        auto& book = doubleSidedBook.getOneSidedBook(new_order.book_info.side);
        book.insert(std::move(matchRemainder));
        id_to_book_map.emplace(std::make_pair(
            new_order.order_info.id, std::make_pair(new_order.book_info, new_order.order_info.price)));
    }
    completed_trades.insert(completed_trades.end(), trades.begin(), trades.end());
}

void MatchingEngine::amend(const Order& order)
{
    if (id_to_book_map.contains(order.id))
    {
        auto [book_info, previous_price] = id_to_book_map.at(order.id);
        auto& book = order_books[book_info.symbol].getOneSidedBook(book_info.side);
        if (book.amend(order, previous_price))
        {
            id_to_book_map.erase(order.id);
            insert({book_info, order});
        }
    }
}

void MatchingEngine::pull(order_id id)
{
    if (id_to_book_map.contains(id))
    {
        auto [book_info, price] = id_to_book_map.at(id);
        auto& book = order_books[book_info.symbol].getOneSidedBook(book_info.side);
        book.pull(id, price);
        id_to_book_map.erase(id);
    }
}

std::vector<std::string> MatchingEngine::getTradesAndPriceLevels()
{
    std::vector<std::string> output;
    for (const auto& trade : completed_trades)
    {
        std::ostringstream line_stream;
        line_stream << trade.symbol << "," << format_price(trade.price) << ","<< trade.volume
                    << "," << trade.aggressive << "," << trade.passive;
        output.push_back(line_stream.str());
    }
    for (auto& [symbol, double_sided_book] : order_books)
    {
        std::ostringstream header_stream;
        header_stream << "===" << symbol << "===";
        output.push_back(header_stream.str());

        auto buys_levels = double_sided_book.buys.getPriceLevels();
        auto buys_it = buys_levels.begin();
        auto buys_end = buys_levels.end();
        auto sells_levels = double_sided_book.sells.getPriceLevels();
        auto sells_it = sells_levels.begin();
        auto sells_end = sells_levels.end();
        while (buys_it != buys_end || sells_it != sells_end)
        {
            std::ostringstream line_stream;

            if (buys_it != buys_end)
            {
                const PriceLevel& buy = *buys_it;
                line_stream << format_price(buy.price) << "," << buy.volume << ",";
                ++buys_it;
            }
            else
            {
                line_stream << ",,";
            }
            if (sells_it != sells_end)
            {
                const PriceLevel& sell = *sells_it;
                line_stream << format_price(sell.price) << "," << sell.volume;
                ++sells_it;
            }
            else
            {
                line_stream << ",";
            }
            output.push_back(line_stream.str());
        }
    }
    return output;
}
