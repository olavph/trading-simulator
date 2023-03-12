#include "console_observer.hpp"

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

// Public

ConsoleObserver::ConsoleObserver(std::shared_ptr<MarketNotifier> notifier, std::ostream &output) :
    MarketObserver(notifier),
    output(output)
{}

void ConsoleObserver::orderAccepted(const NewOrder & order)
{
    output << "NEW ORDER: "
           << order.book_info.symbol << "," << (order.book_info.side == Side::Buy ? "BUY" : "SELL")
           << "," << order.order_info.id << "," << format_price(order.order_info.price) << ","<< order.order_info.volume << std::endl;
}

void ConsoleObserver::tradeExecuted(const Trade & trade)
{
    output << "TRADE EXECUTED: "
           << trade.symbol << "," << format_price(trade.price) << ","<< trade.volume
           << "," << trade.aggressive << "," << trade.passive << std::endl;
}
