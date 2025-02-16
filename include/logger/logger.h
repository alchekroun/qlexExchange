#pragma once

#include <sstream>
#include <iostream>

namespace qlexexchange
{

    class logger
    {
    public:
        logger(const std::string &className_) : _className(className_) {}

        template <typename First, typename... Args>
        inline void msg(const First &v1, const Args &...v)
        {
            std::stringstream helper;
            buildStr(helper, v1, v...);
            std::cout << "[" << _className << "]" << helper.str() << std::endl;
        }

    private:
        void buildStr(std::stringstream &ss)
        {
        }

        template <typename First, typename... Args>
        void buildStr(std::stringstream &ss, const First &v1, const Args &...v)
        {
            ss << v1;
            buildStr(ss, v...);
        }

        std::string _className;
    };

} // qlexexchange