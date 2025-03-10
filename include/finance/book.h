#pragma once

#include "Common.h"

namespace qlexexchange
{
    template <typename T>
    class Book
    {
        std::priority_queue<std::shared_ptr<T>> _orders;

    public:
        T top() const { return _orders.top(); }
        void pop() { _orders.pop(); }
        void push(std::shared_ptr<T> order_) { _orders.push(order_); }
        bool empty() { return _orders.empty(); }
    };
} // qlexExchange