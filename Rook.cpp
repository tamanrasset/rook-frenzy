#include "Rook.h"
#include "IoUtils.h"

#include <random>
#include <stdexcept>
#include <thread>

namespace
{
    constexpr auto MoveCountLimit = 50;
    constexpr auto MaxMoveTime = std::chrono::milliseconds(5000);
    constexpr auto MinDelayMs = 200;
    constexpr auto MaxDelayMs = 300;
}

namespace AntiDrone_NS
{
    Rook::Rook(int i_id, IBoard::Pos i_initialPos, std::shared_ptr<IBoard> i_board)
        : d_id(i_id)
        , d_pos(i_initialPos)
        , d_board(std::move(i_board))
    {
        if (!d_board)
        {
            throw std::runtime_error("Board dispatcher cannot be nullptr");
        }
    }

    void Rook::run()
    {
        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<> stepDistr(1, d_board->width() + d_board->height() - 2);
        std::uniform_int_distribution<> delayDistr(MinDelayMs, MaxDelayMs);

        d_board->waitForStart();
        int moveCount = 0;
        while (moveCount < MoveCountLimit)
        {
            auto nextPos = getNextPos(stepDistr(generator));
            if (tryMove(nextPos))
            {
                ConsoleLogger::get().log(
                    "Rook" + std::to_string(d_id),
                    "(step " + std::to_string(moveCount + 1) + ") moved from " + std::to_string(d_pos) + " to "  + std::to_string(nextPos));
                d_pos = nextPos;
                if(++moveCount % 2 && !moveCount != MoveCountLimit)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(delayDistr(generator)));
                }
            }
        }
    }

    bool Rook::tryMove(IBoard::Pos i_nextPos)
    {
        if (!d_board->tryMove(d_id, i_nextPos))
        {
            ConsoleLogger::get().log(
                        "Rook" + std::to_string(d_id),
                        "Position " + std::to_string(i_nextPos) + " is blocked");
            auto limit = std::chrono::system_clock::now() + MaxMoveTime;
            while(!d_board->tryMove(d_id, i_nextPos))
            {
                if (!d_board->waitForMoveUntil(limit))
                {
                    ConsoleLogger::get().log(
                        "Rook" + std::to_string(d_id),
                        "Failed to move to " + std::to_string(i_nextPos));
                    return false;
                }
            }
        }

        return true;
    }

    IBoard::Pos Rook::getNextPos(int i_step) const
    {
        const auto w = d_board->width();
        const auto h = d_board->height();
        return (i_step < w)
            ? h * (d_pos / h) + ((d_pos % h) + i_step) % w
            : (d_pos + w * (i_step - w + 1)) % (h * w);
    }
}
