#include "board.h"
#include <iostream>
#include <string>
#include <chrono>

std::string squareToAlgebraic(int sq)
{
    char file = 'a' + (sq % 8);
    char rank = '1' + (sq / 8);
    return std::string(1, file) + std::string(1, rank);
}

int main()
{
    ChessBoard board;
    board.setupInitialPosition();

    std::cout << "--- AI DANG SUY NGHI (Depth = 4) ---" << std::endl;

    // Đo thời gian suy nghĩ
    auto start = std::chrono::high_resolution_clock::now();

    // Gọi AI tìm nước đi tốt nhất
    Move bestMove = board.getBestMove(4);

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    if (bestMove.from != -1)
    {
        std::cout << "Nuoc di tot nhat: "
                  << squareToAlgebraic(bestMove.from)
                  << squareToAlgebraic(bestMove.to) << std::endl;
        std::cout << "Thoi gian tinh toan: " << elapsed.count() << " giay." << std::endl;

        // Thử thực hiện nước đi đó
        board.makeMove(bestMove.from, bestMove.to);
        // board.printBoard(); // Mở comment nếu bạn muốn in bàn cờ
    }
    else
    {
        std::cout << "Khong tim thay nuoc di!" << std::endl;
    }

    return 0;
}
