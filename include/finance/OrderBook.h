#pragma once

#include "Common.h"

namespace qlexexchange
{
    template <typename T>
    class OrderBook
    {
        std::priority_queue<std::shared_ptr<T>> _orders;

    public:
        const std::shared_ptr<T> top() const { return _orders.top(); }
        void pop() { _orders.pop(); }
        void push(std::shared_ptr<T> order_) { _orders.push(order_); }
        void push(const T &order_) { _orders.push(std::make_shared<T>(order_)); }
        bool empty() const { return _orders.empty(); }
    };
} // qlexExchange