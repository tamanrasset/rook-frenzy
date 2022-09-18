#pragma once

#include "IBoard.h"

#include <memory>

namespace AntiDrone_NS
{
    class Rook
    {
    public:
        Rook(int i_id, IBoard::Pos i_initialPos, std::shared_ptr<IBoard> i_board);
        void run();
    private:
        IBoard::Pos getNextPos(int i_step) const;
        bool tryMove(IBoard::Pos i_nextPos);
        int d_id = 0;
        IBoard::Pos d_pos = 0;
        std::shared_ptr<IBoard> d_board;
    };
}
