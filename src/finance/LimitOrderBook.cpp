#include "finance/LimitOrderBook.h"

namespace qlexexchange
{

    void LimitOrderBook::addAsks(const std::vector<std::shared_ptr<AskOrder>> &orders_)
    {
        for (auto const &o : orders_)
        {
            _askBook.push(o);
        }
    }

    void LimitOrderBook::addBids(const std::vector<std::shared_ptr<BidOrder>> &orders_)
    {
        for (auto const &o : orders_)
        {
            _bidBook.push(o);
        }
    }

    bool LimitOrderBook::isThereMatchingOrders() const
    {
        return _askBook.top()->getPrice() == _bidBook.top()->getPrice();
    }

    const AskBidVectorPair LimitOrderBook::getMatchingOrders()
    {
        // .first ASK - .second BID
        AskBidVectorPair matchingOrders;

        // For now the lower ask price is laways the price reference.
        float const priceRef = _bidBook.top()->getPrice();

        while (!_bidBook.empty())
        {
            if (abs(_bidBook.top()->getPrice() - priceRef) < 0.0001)
            {
                matchingOrders.bids.emplace_back(_bidBook.top());
                _bidBook.pop();
            }
            else
            {
                break;
            }
        }

        while (!_askBook.empty())
        {
            if (abs(_askBook.top()->getPrice() - priceRef) < 0.0001)
            {
                matchingOrders.asks.emplace_back(_askBook.top());
                _askBook.pop();
            }
            else
            {
                break;
            }
        }

        return matchingOrders;
    }

    const QuantityFilledByBid LimitOrderBook::easyFillBids(const std::vector<std::shared_ptr<BidOrder>> &bids_) const
    {
        QuantityFilledByBid quantityByBids;

        std::for_each(begin(bids_), end(bids_),
                      [&quantityByBids](const std::shared_ptr<BidOrder> &b)
                      {
                          quantityByBids[b] = b->getQuantity();
                      });

        return quantityByBids;
    }

    const QuantityFilledByBid LimitOrderBook::processBids(const AskBidVectorPair &matchingOrders_) const
    {

        QuantityFilledByBid bidFilled;

        const auto totalAskQuantityAvailable = std::accumulate(begin(matchingOrders_.asks), end(matchingOrders_.asks), 0,
                                                               [](const int x, const std::shared_ptr<AskOrder> &a)
                                                               { return x + a->getQuantity(); });
        const auto totalBidsQuantityAvailable = std::accumulate(begin(matchingOrders_.bids), end(matchingOrders_.bids), 0,
                                                                [](const int x, const std::shared_ptr<BidOrder> &b)
                                                                { return x + b->getQuantity(); });

        // If everybody can't have what they ask, we launch the special allocation
        if (totalAskQuantityAvailable < totalBidsQuantityAvailable)
        {
        }
        else
        {
            bidFilled = easyFillBids(matchingOrders_.bids);
        }

        return bidFilled;
    }

    QuantityAcquiredByAsk LimitOrderBook::removeQuantityFromAsks(const QuantityFilledByBid &matchedBids_, const std::vector<std::shared_ptr<AskOrder>> &askConcerned_) const
    {
        QuantityAcquiredByAsk output(askConcerned_.size());
        int currentAskIndex = 0, quantityPlaced = 0, quantityPlacedCurrentAsk = 0;

        for (const auto &[ptr, qte] : matchedBids_)
        {
            while (quantityPlaced < qte)
            {
                int const realQte = qte - quantityPlaced;
                int const realAskQte = askConcerned_[currentAskIndex]->getQuantity() - quantityPlacedCurrentAsk;
                int const diff = realAskQte - realQte;
                if (diff > 0)
                {
                    output[askConcerned_[currentAskIndex]] += realQte;
                    quantityPlaced += realQte;
                    quantityPlacedCurrentAsk += realQte;
                }
                else if (diff == 0)
                {
                    output[askConcerned_[currentAskIndex++]] += realQte;
                    quantityPlaced += realQte;
                    quantityPlacedCurrentAsk = 0;
                }
                else
                {
                    output[askConcerned_[currentAskIndex++]] += realAskQte;
                    quantityPlaced += realQte;
                    quantityPlacedCurrentAsk = 0;
                }
            }
            quantityPlaced = 0;
        }

        return output;
    }

    void LimitOrderBook::affectResidualOrders(const QuantityAcquiredByAsk &asks_, const QuantityFilledByBid &bids_)
    {
        std::vector<std::shared_ptr<AskOrder>> asksToReinsert;
        for (const auto &[ptr, qte] : asks_)
        {
            // TODO : review copy constructor + do not reset the timestamp it should keep the original
            if (ptr->getQuantity() > qte)
                asksToReinsert.emplace_back(std::make_shared<AskOrder>(ptr->getIdClient(), ptr->getPrice(), ptr->getQuantity() - qte));
        }
        addAsks(asksToReinsert);

        std::vector<std::shared_ptr<BidOrder>> bidsToReinsert;
        for (const auto &[ptr, qte] : bids_)
        {
            // TODO : review copy constructor + do not reset the timestamp it should keep the original
            if (ptr->getQuantity() > qte)
                bidsToReinsert.emplace_back(std::make_shared<BidOrder>(ptr->getIdClient(), ptr->getPrice(), ptr->getQuantity() - qte));
        }
        addBids(bidsToReinsert);
    }

    const std::pair<QuantityAcquiredByAsk, QuantityFilledByBid> LimitOrderBook::fillMatchingOrders(const AskBidVectorPair &matchingOrders_)
    {
        std::pair<QuantityAcquiredByAsk, QuantityFilledByBid> output;
        auto const bidFilled = processBids(matchingOrders_);
        output.second = bidFilled;
        auto const askFilled = removeQuantityFromAsks(bidFilled, matchingOrders_.asks);
        output.first = askFilled;
        return output;
    }

    void LimitOrderBook::update()
    {
        if (isThereMatchingOrders())
        {
            auto const matchingOrders = getMatchingOrders();
            auto const matchedOrders = fillMatchingOrders(matchingOrders);
        }
    }

} // qlexExchange