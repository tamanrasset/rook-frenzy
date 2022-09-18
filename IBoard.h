#pragma once

#include <chrono>

namespace AntiDrone_NS
{
    class IBoard
    {
    public:
        using Pos = int;
        virtual void start() = 0;
        virtual void waitForStart() = 0;
        virtual bool waitForMoveUntil(std::chrono::system_clock::time_point i_limit) = 0;
        virtual bool tryMove(int i_id, Pos i_nextPos) = 0;
        virtual int width() const = 0;
        virtual int height() const = 0;
        virtual ~IBoard() = default;
    };
}
