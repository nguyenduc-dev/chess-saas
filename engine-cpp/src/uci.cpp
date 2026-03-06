#include "board.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

// Hàm hỗ trợ 1: Chuyển đổi chuỗi "e2e4" từ Web gửi xuống thành struct Move
Move parseMove(ChessBoard &board, std::string moveStr)
{
    std::vector<Move> moves = board.generateAllLegalMoves();

    int fromCol = moveStr[0] - 'a';
    int fromRow = moveStr[1] - '1';
    int toCol = moveStr[2] - 'a';
    int toRow = moveStr[3] - '1';

    int from = fromRow * 8 + fromCol;
    int to = toRow * 8 + toCol;

    // Đối chiếu với danh sách nước đi hợp lệ để đảm bảo an toàn
    for (Move m : moves)
    {
        if (m.from == from && m.to == to)
        {
            return m;
        }
    }
    return {-1, -1}; // Trả về lỗi nếu nước đi không hợp lệ
}

// Hàm hỗ trợ 2: Chuyển struct Move của AI thành chuỗi "e2e4" để gửi lên Web
std::string moveToString(Move m)
{
    if (m.from == -1)
        return "0000";
    char fCol = 'a' + (m.from % 8);
    char fRow = '1' + (m.from / 8);
    char tCol = 'a' + (m.to % 8);
    char tRow = '1' + (m.to / 8);
    return std::string(1, fCol) + std::string(1, fRow) + std::string(1, tCol) + std::string(1, tRow);
}

// VÒNG LẶP LẮNG NGHE LỆNH UCI
void uciLoop()
{
    ChessBoard board;
    board.setupInitialPosition();

    std::string line, command;

    // Liên tục đọc từng dòng lệnh từ giao diện bên ngoài gửi vào
    while (std::getline(std::cin, line))
    {
        std::istringstream iss(line);
        iss >> command;

        if (command == "uci")
        {
            // Giới thiệu bản thân
            std::cout << "id name MySaaS_ChessEngine\n";
            std::cout << "id author You\n";
            std::cout << "uciok\n"; // Báo hiệu đã sẵn sàng
        }
        else if (command == "isready")
        {
            std::cout << "readyok\n";
        }
        else if (command == "ucinewgame")
        {
            board.setupInitialPosition();
        }
        else if (command == "position")
        {
            // Lệnh position thường có dạng: position startpos moves e2e4 e7e5...
            std::string token;
            iss >> token;
            if (token == "startpos")
            {
                board.setupInitialPosition();
            }

            // Đọc và áp dụng các nước đi đã xảy ra trên bàn cờ
            iss >> token;
            if (token == "moves")
            {
                std::string moveStr;
                while (iss >> moveStr)
                {
                    Move m = parseMove(board, moveStr);
                    if (m.from != -1)
                    {
                        board.makeMove(m.from, m.to);
                    }
                }
            }
        }
        else if (command == "go")
        {
            // Nhận lệnh bắt đầu suy nghĩ. Ở đây ta fix cứng Depth = 4.
            // (Bản hoàn chỉnh sẽ đọc thêm tham số thời gian để tự tính Depth)
            Move bestMove = board.getBestMove(4);

            // Trả kết quả về cho Web/GUI
            std::cout << "bestmove " << moveToString(bestMove) << "\n";
        }
        else if (command == "quit")
        {
            break; // Tắt Engine
        }
    }
}
