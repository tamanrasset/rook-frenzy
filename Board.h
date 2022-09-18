#pragma once

#include "IBoard.h"

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <vector>

namespace AntiDrone_NS
{
    class Board: public IBoard
    {
    public:
        using State = std::vector<IBoard::Pos>;
        Board(int i_width, int i_height, State i_initialState);
        void start() override;
        void waitForStart() override;
        bool waitForMoveUntil(std::chrono::system_clock::time_point i_limit) override;
        bool tryMove(int i_id, Pos i_nextPos) override;
        int width() const override { return d_width; }
        int height() const override { return d_height; }
        const State& getState() const;
    private:
        bool areOnSameRow(Pos i_lhs, Pos i_rhs) const;
        bool areOnSameColumn(Pos i_lhs, Pos i_rhs) const;
        bool isValidMove(Pos i_current, Pos i_next) const;
        bool isBlocked(Pos i_current, Pos i_next) const;
        State d_state;
        int d_width;
        int d_height;
        mutable std::mutex d_stateMutex;
        std::mutex d_cvMutex;
        std::condition_variable d_cv;
        std::atomic_bool d_started = false;
    };
}
