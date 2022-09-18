#include "Board.h"

#include <algorithm>

namespace AntiDrone_NS
{
    Board::Board(int i_width, int i_height, State i_initialState)
        : d_width(i_width)
        , d_height(i_height)
        , d_state(std::move(i_initialState))
    {
    }

    void Board::start()
    {
        d_started = true;
        d_cv.notify_all();
    }

    void Board::waitForStart()
    {
        std::unique_lock<std::mutex> lock(d_cvMutex);
        d_cv.wait(lock, [this]{ return d_started.load(); });
    }

    bool Board::waitForMoveUntil(std::chrono::system_clock::time_point i_limit)
    {
        std::unique_lock<std::mutex> lock(d_cvMutex);
        return d_cv.wait_until(lock, i_limit) != std::cv_status::timeout;
    }

    bool Board::tryMove(int i_id, Pos i_nextPos)
    {
        {
            std::lock_guard<std::mutex> lock(d_stateMutex);

            if (i_id < 0 || i_id >= static_cast<int>(d_state.size()))
            {
                throw std::logic_error("Invalid rook ID");
            }

            auto currentPos = d_state[i_id];
            if(!isValidMove(currentPos, i_nextPos))
            {
                throw std::logic_error("Invalid move");
            }

            if (isBlocked(currentPos, i_nextPos))
            {
                return false;
            }

            d_state[i_id] = i_nextPos;
        }

        d_cv.notify_all();
        return true;
    }

    const Board::State& Board::getState() const
    {
        std::lock_guard<std::mutex> lock(d_stateMutex);
        return d_state;
    }

    bool Board::areOnSameRow(Pos i_lhs, Pos i_rhs) const
    {
        return i_lhs / d_height == i_rhs / d_height;
    }

    bool Board::areOnSameColumn(Pos i_lhs, Pos i_rhs) const
    {
        return (i_lhs - i_rhs) % d_width == 0;
    }

    bool Board::isValidMove(Pos i_current, Pos i_next) const
    {
        return i_current != i_next && (areOnSameRow(i_current, i_next) || areOnSameColumn(i_current, i_next));
    }

    bool Board::isBlocked(Pos i_current, Pos i_next) const
    {
        auto [min, max] = std::minmax(i_current, i_next);
        for (auto pos : d_state)
        {
            if (pos != i_current && pos >= min && pos <= max &&
                ((areOnSameRow(pos, i_next) && areOnSameRow(pos, i_current)) ||
                (areOnSameColumn(pos, i_next) && areOnSameColumn(pos, i_current))))
            {
                return true;
            }
        }

        return false;
    }
}