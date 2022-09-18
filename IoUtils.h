#pragma once

#include "Board.h"

#include <string_view>

namespace AntiDrone_NS
{
    class ConsoleLogger
    {
    public:
        static ConsoleLogger& get();
        ConsoleLogger(const ConsoleLogger&) = delete;
        ConsoleLogger(ConsoleLogger&&) = delete;
        ConsoleLogger& operator=(const ConsoleLogger&) = delete;
        ConsoleLogger& operator=(ConsoleLogger &&) = delete;
        void log(std::string_view i_source, std::string_view i_message);
        void setEnabled(bool i_enabled);
    private:
        ConsoleLogger() {}
        std::mutex d_mutex;
        bool d_enabled = false;
    };


    void printBoard(const Board& i_state);
}
