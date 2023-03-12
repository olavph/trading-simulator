#pragma once

#include "i_matching_engine.hpp"
#include "i_market_observer.hpp"
#include "order_book.hpp"

#include <map>
#include <memory>
#include <set>
#include <utility>

Side oppositeSide(Side);

class MatchingEngine : public IMatchingEngine
{
public:
    MatchingEngine(std::shared_ptr<IMarketObserver> market_notifier);
    void insert(const NewOrder &) override;
    void amend(const Order &) override;
    void pull(order_id) override;

    std::vector<Trade> getTrades() const override;
    PriceLevels getPriceLevels() const override;
    std::vector<std::string> getTradesAndPriceLevels() const override;

private:
    // Notifier of market data to other observers
    std::shared_ptr<IMarketObserver> market_notifier;
    // Map symbols to order books
    std::map<std::string, CentralLimitOrderBook> order_books;
    // Map order id to the book containing it, along with the price for fast search
    std::map<order_id, std::pair<BookInfo, price_t>> id_to_book_map;
    // Track completed trades
    std::vector<Trade> completed_trades;
};
