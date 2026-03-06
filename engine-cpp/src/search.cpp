#include "board.h"
#include <vector>
#include <iostream>

// Định nghĩa một số vô cực (lớn hơn mọi điểm số bàn cờ)
const int INF = 999999;

int ChessBoard::search(int depth, int alpha, int beta)
{
    // 1. Điều kiện dừng: Đạt đến độ sâu tối đa, trả về điểm số hiện tại
    if (depth == 0)
    {
        return evaluatePosition();
    }

    std::vector<Move> moves = generateAllLegalMoves();

    // 2. Kiểm tra Kết thúc ván cờ (Hết nước đi)
    if (moves.empty())
    {
        if (isInCheck(sideToMove))
        {
            // Bị Chiếu bí (Checkmate). Trả về điểm âm vô cực.
            // Cộng thêm 'depth' để AI thích thắng nhanh (depth cao) và trì hoãn trận thua (depth thấp).
            return -INF + (100 - depth);
        }
        // Hòa (Stalemate)
        return 0;
    }

    int bestScore = -INF;

    // 3. Đệ quy Negamax & Cắt tỉa Alpha-Beta
    for (Move move : moves)
    {
        char captured = getPieceAt(move.to);
        int oldCastling = castlingRights;

        // Thử nước đi
        makeMove(move.from, move.to);

        // Gọi đệ quy: Điểm của mình = Âm điểm của đối thủ ở bước tiếp theo
        // Đảo ngược dấu và vị trí của Alpha/Beta
        int score = -search(depth - 1, -beta, -alpha);

        // Hoàn tác
        unmakeMove(move.from, move.to, captured, oldCastling);

        // Cập nhật điểm tốt nhất
        if (score > bestScore)
        {
            bestScore = score;
        }

        // Cập nhật Alpha (Điểm tối thiểu mình chắc chắn đạt được)
        if (score > alpha)
        {
            alpha = score;
        }

        // ALPHA-BETA PRUNING: Cắt tỉa
        // Nếu điểm hiện tại đã tốt hơn hoặc bằng Beta (điểm tối đa đối thủ cho phép mình đạt được),
        // đối thủ sẽ không bao giờ đi nhánh này. Ta ngừng tìm kiếm nhánh này ngay lập tức.
        if (alpha >= beta)
        {
            break;
        }
    }

    return bestScore;
}

// Hàm khởi chạy tìm kiếm ở lớp ngoài cùng để lấy ra nước đi (thay vì chỉ lấy điểm)
Move ChessBoard::getBestMove(int depth)
{
    std::vector<Move> moves = generateAllLegalMoves();

    if (moves.empty())
        return {-1, -1}; // Không có nước đi

    Move bestMove = moves[0];
    int bestScore = -INF;
    int alpha = -INF;
    int beta = INF;

    for (Move move : moves)
    {
        char captured = getPieceAt(move.to);
        int oldCastling = castlingRights;

        makeMove(move.from, move.to);
        int score = -search(depth - 1, -beta, -alpha);
        unmakeMove(move.from, move.to, captured, oldCastling);

        if (score > bestScore)
        {
            bestScore = score;
            bestMove = move;
        }
        if (score > alpha)
        {
            alpha = score;
        }
    }

    return bestMove;
}
