#include "board.h"

// Bảng điểm cho Tốt (Khuyến khích tiến lên và kiểm soát trung tâm)
const int pawnEvalWhite[64] = {
    0, 0, 0, 0, 0, 0, 0, 0,
    50, 50, 50, 50, 50, 50, 50, 50,
    10, 10, 20, 30, 30, 20, 10, 10,
    5, 5, 10, 25, 25, 10, 5, 5,
    0, 0, 0, 20, 20, 0, 0, 0,
    5, -5, -10, 0, 0, -10, -5, 5,
    5, 10, 10, -20, -20, 10, 10, 5,
    0, 0, 0, 0, 0, 0, 0, 0};

// Bảng điểm cho Mã (Phạt nặng nếu ra rìa, thưởng nếu ở trung tâm)
const int knightEval[64] = {
    -50, -40, -30, -30, -30, -30, -40, -50,
    -40, -20, 0, 0, 0, 0, -20, -40,
    -30, 0, 10, 15, 15, 10, 0, -30,
    -30, 5, 15, 20, 20, 15, 5, -30,
    -30, 0, 15, 20, 20, 15, 0, -30,
    -30, 5, 10, 15, 15, 10, 5, -30,
    -40, -20, 0, 5, 5, 0, -20, -40,
    -50, -40, -30, -30, -30, -30, -40, -50};

// Hàm hỗ trợ: Lấy chỉ số ô bị lật ngược cho quân Đen
int mirrorScore(int sq)
{
    return sq ^ 56;
}

int ChessBoard::evaluatePosition()
{
    int whiteScore = 0;
    int blackScore = 0;

    const int pawnValue = 100;
    const int knightValue = 320;
    const int bishopValue = 330;
    const int rookValue = 500;
    const int queenValue = 900;

    // Tính điểm vật chất (Material)
    whiteScore += __builtin_popcountll(whitePawns) * pawnValue;
    whiteScore += __builtin_popcountll(whiteKnights) * knightValue;
    whiteScore += __builtin_popcountll(whiteBishops) * bishopValue;
    whiteScore += __builtin_popcountll(whiteRooks) * rookValue;
    whiteScore += __builtin_popcountll(whiteQueens) * queenValue;

    blackScore += __builtin_popcountll(blackPawns) * pawnValue;
    blackScore += __builtin_popcountll(blackKnights) * knightValue;
    blackScore += __builtin_popcountll(blackBishops) * bishopValue;
    blackScore += __builtin_popcountll(blackRooks) * rookValue;
    blackScore += __builtin_popcountll(blackQueens) * queenValue;

    // Tính điểm vị trí (Positional Bonus)
    // 1. Quét quân Mã Trắng
    U64 tempKnights = whiteKnights;
    while (tempKnights)
    {
        int sq = __builtin_ctzll(tempKnights);
        whiteScore += knightEval[sq];
        tempKnights &= (tempKnights - 1);
    }
    // 2. Quét quân Mã Đen (dùng hàm mirror để lật ngược bảng điểm)
    tempKnights = blackKnights;
    while (tempKnights)
    {
        int sq = __builtin_ctzll(tempKnights);
        blackScore += knightEval[mirrorScore(sq)];
        tempKnights &= (tempKnights - 1);
    }

    // 3. Quét Tốt Trắng
    U64 tempPawns = whitePawns;
    while (tempPawns)
    {
        int sq = __builtin_ctzll(tempPawns);
        whiteScore += pawnEvalWhite[sq];
        tempPawns &= (tempPawns - 1);
    }
    // 4. Quét Tốt Đen
    tempPawns = blackPawns;
    while (tempPawns)
    {
        int sq = __builtin_ctzll(tempPawns);
        blackScore += pawnEvalWhite[mirrorScore(sq)];
        tempPawns &= (tempPawns - 1);
    }

    int evaluation = whiteScore - blackScore;
    return (sideToMove == WHITE) ? evaluation : -evaluation;
}
