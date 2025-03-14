#pragma once

#include "Common.h"

namespace qlexexchange
{

    enum OrderType
    {
        ASK,
        BID
    };

    template <OrderType T>
    class Order
    {
        std::chrono::system_clock::time_point _timestamp = std::chrono::system_clock::now();
        uint32_t _idClient;
        float _price;
        int _quantity;

    public:
        Order(const uint32_t &idClient_, float const &price_, float const &quantity_) : _idClient(idClient_), _price(price_), _quantity(quantity_) {};

        bool operator==(const Order &other_)
        {
            if (this->_timestamp != other_._timestamp || this->_quantity != other_._quantity || this->_price != other_._price)
            {
                if (std::abs(this->_price - other_._price) >= 0.001)
                    return false;
            }
            return true;
        }

        bool operator!=(const Order &other_) { return !(*this == other_); }

        int getQuantity() const { return _quantity; }
        uint32_t getIdClient() const { return _idClient; }
        float getPrice() const { return _price; }
        std::chrono::system_clock::time_point getTime() const { return _timestamp; }

        friend bool operator<(const Order<ASK> &lhs, const Order<ASK> &rhs);
        friend bool operator>(const Order<ASK> &lhs, const Order<ASK> &rhs);
        friend bool operator<(const Order<BID> &lhs, const Order<BID> &rhs);
        friend bool operator>(const Order<BID> &lhs, const Order<BID> &rhs);
    };

    inline bool operator<(const Order<ASK> &lhs, const Order<ASK> &rhs) { return lhs._price < rhs._price; }
    inline bool operator>(const Order<ASK> &lhs, const Order<ASK> &rhs) { return lhs._price > rhs._price; }
    inline bool operator<(const Order<BID> &lhs, const Order<BID> &rhs) { return lhs._price > rhs._price; }
    inline bool operator>(const Order<BID> &lhs, const Order<BID> &rhs) { return lhs._price < rhs._price; }
} // qlexExchange