#include "csv_writer.hpp"

#include "i_order_types.hpp"

#include <chrono>
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

CSV_Writer::CSV_Writer(std::shared_ptr<MarketNotifier> notifier, std::ostream &output) :
    MarketObserver(notifier),
    output(output)
{
    output << "Time,Symbol,Price,Volume,AgressiveAgentId,AgressiveSeqNum,PassiveAgentId,PassiveSeqNum" << std::endl;
}

void CSV_Writer::orderAccepted(const NewOrder & order)
{
}

void CSV_Writer::tradeExecuted(const Trade & trade)
{
    auto time = std::chrono::system_clock::now();
    output << time.time_since_epoch() << ","
           << trade.symbol << "," << format_price(trade.price) << ","<< trade.volume
           << "," << trade.aggressive << "," << trade.passive << std::endl;
}
