#pragma once

#include "Common.h"

#include "OrderBook.h"
#include "Order.h"

namespace qlexexchange
{
    typedef Order<OrderType::BID> BidOrder;
    typedef Order<OrderType::ASK> AskOrder;
    typedef std::unordered_map<std::shared_ptr<BidOrder>, int> QuantityFilledByBid;
    typedef std::unordered_map<std::shared_ptr<AskOrder>, int> QuantityAcquiredByAsk;

    struct AskBidVectorPair
    {
        std::vector<std::shared_ptr<AskOrder>> asks;
        std::vector<std::shared_ptr<BidOrder>> bids;
    };

    class LimitOrderBook
    {
        OrderBook<AskOrder> _askBook;
        OrderBook<BidOrder> _bidBook;

    public:
        void addAsks(const std::vector<std::shared_ptr<AskOrder>> &orders_);
        void addBids(const std::vector<std::shared_ptr<BidOrder>> &orders_);
        void update();

    private:
        bool isThereMatchingOrders() const;
        const AskBidVectorPair getMatchingOrders();
        const std::pair<QuantityAcquiredByAsk, QuantityFilledByBid> fillMatchingOrders(const AskBidVectorPair &matchingOrders_);
        const QuantityFilledByBid processBids(const AskBidVectorPair &matchingOrders_) const;
        const QuantityFilledByBid easyFillBids(const std::vector<std::shared_ptr<BidOrder>> &bids_) const;
        QuantityAcquiredByAsk removeQuantityFromAsks(const QuantityFilledByBid &matchedBids_, const std::vector<std::shared_ptr<AskOrder>> &askConcerned_) const;
        void affectResidualOrders(const QuantityAcquiredByAsk &asks_, const QuantityFilledByBid &bids_);
    };

} // qlexExchange