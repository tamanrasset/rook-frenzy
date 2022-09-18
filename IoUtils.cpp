#include "IoUtils.h"

#include <iomanip>
#include <iostream>
#include <unordered_map>

namespace
{
    std::unordered_map<AntiDrone_NS::IBoard::Pos, int> getRookPositions(const AntiDrone_NS::Board::State& i_state)
    {
        std::unordered_map<AntiDrone_NS::IBoard::Pos, int> res;
        for (auto i  = 0; i < static_cast<int>(i_state.size()); ++i)
        {
            res[i_state[i]] = i;
        }

        return res;
    }
}

namespace AntiDrone_NS
{
    ConsoleLogger& ConsoleLogger::get()
    {
        static ConsoleLogger logger;
        return logger;
    }

    void ConsoleLogger::log(std::string_view i_source, std::string_view i_message)
    {
        if(!d_enabled)
        {
            return;
        }

        auto msSinceEpoch = std::chrono::system_clock::now().time_since_epoch() / std::chrono::milliseconds(1);
        std::lock_guard<std::mutex> lock(d_mutex);
        std::cout << msSinceEpoch << " " << i_source << ": " << i_message << std::endl;
    }

    void ConsoleLogger::setEnabled(bool i_enabled)
    {
        d_enabled = i_enabled;
    }


    void printBoard(const Board& i_board)
    {
        auto rookPositions = getRookPositions(i_board.getState());
        for(auto h = 0; h < i_board.height(); ++h)
        {
            std::cout << '|';
            for (auto w = 0; w < i_board.width(); ++w)
            {
                if(rookPositions.count(h * i_board.height() + w))
                {
                    std::cout << 'R' << rookPositions.at(h * i_board.width() + w);
                }
                else 
                {
                    std::cout << "__";
                }

                std::cout << '|';
            }

            std::cout << '\n';
        }

        std::cout << std::endl;
    }
}
