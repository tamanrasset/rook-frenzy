#include "Board.h"
#include "Rook.h"
#include "IoUtils.h"

#include <algorithm>
#include <array>
#include <future>
#include <iostream>
#include <random>

namespace
{
    constexpr auto RookCount = 5;
    constexpr auto BoardWidth = 8;
    constexpr auto BoardHeight = 8;

#ifdef DENABLE_LOGGING
    constexpr auto EnableLoggingToConsole = false;
#else
    constexpr auto EnableLoggingToConsole = true;
#endif
}


using namespace AntiDrone_NS;

Board::State generateInitialState()
{
    std::array<Board::Pos, BoardWidth * BoardHeight> initialPositions;
    std::iota(initialPositions.begin(), initialPositions.end(), 0);
    std::shuffle(initialPositions.begin(), initialPositions.end(), std::default_random_engine {});

    Board::State state(RookCount);
    std::generate(state.begin(), state.end(), [&initialPositions, n = 0] () mutable { return initialPositions[n++]; });
    return state;
}


int main()
{
    ConsoleLogger::get().setEnabled(EnableLoggingToConsole);

    auto board = std::make_shared<Board>(BoardWidth, BoardHeight, generateInitialState());
    std::cout << "Initial position:\n";
    printBoard(*board);
    std::vector<Rook> rooks;
    rooks.reserve(RookCount);
    for (auto i = 0; i < RookCount; ++i)
    {
        rooks.emplace_back(i, board->getState()[i], board);
    }

    std::vector<std::future<void>> tasks;
    tasks.reserve(RookCount);

    for (auto& rook : rooks)
    {
        tasks.emplace_back(std::async(std::launch::async, [&rook](){ rook.run(); }));
    }

    std::cout << "Simulation started. Please wait until done..." << std::endl;
    board->start();
    for (auto& task : tasks)
    {
        try
        {
            task.get();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }

    std::cout << "\nFinal position:\n";
    printBoard(*board);
    return 0;
}
