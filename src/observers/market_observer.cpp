#include "market_observer.hpp"

// Public

MarketObserver::MarketObserver(std::shared_ptr<MarketNotifier> notifier)
    : market_notifier(notifier)
{
    market_notifier->subscribe(this);
}

MarketObserver::MarketObserver(MarketObserver &&)
{
    market_notifier->subscribe(this);
}

MarketObserver::~MarketObserver()
{
    market_notifier->unsubscribe(this);
}
