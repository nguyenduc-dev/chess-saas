#include "board.h"
#include <iostream>

void ChessBoard::printBoard()
{
    std::cout << "\n  a b c d e f g h" << std::endl;
    std::cout << "  ---------------" << std::endl;

    for (int rank = 7; rank >= 0; rank--)
    {
        std::cout << rank + 1 << "|"; // In số thứ tự hàng từ 8 xuống 1

        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            U64 bit = 1ULL << square;
            char piece = '.'; // Mặc định là ô trống

            // Kiểm tra quân Trắng (Viết hoa)
            if (whitePawns & bit)
                piece = 'P';
            else if (whiteKnights & bit)
                piece = 'N';
            else if (whiteBishops & bit)
                piece = 'B';
            else if (whiteRooks & bit)
                piece = 'R';
            else if (whiteQueens & bit)
                piece = 'Q';
            else if (whiteKing & bit)
                piece = 'K';

            // Kiểm tra quân Đen (Viết thường)
            else if (blackPawns & bit)
                piece = 'p';
            else if (blackKnights & bit)
                piece = 'n';
            else if (blackBishops & bit)
                piece = 'b';
            else if (blackRooks & bit)
                piece = 'r';
            else if (blackQueens & bit)
                piece = 'q';
            else if (blackKing & bit)
                piece = 'k';

            std::cout << piece << " ";
        }
        std::cout << "|" << rank + 1 << std::endl;
    }
    std::cout << "  ---------------" << std::endl;
    std::cout << "  a b c d e f g h\n"
              << std::endl;
}

void ChessBoard::printBitboard(U64 bitboard)
{
    for (int rank = 7; rank >= 0; rank--)
    {
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            if ((bitboard >> square) & 1ULL)
                std::cout << "X ";
            else
                std::cout << ". ";
        }
        std::cout << std::endl;
    }
}
