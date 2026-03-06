#include "board.h"
#include <iostream>
#include <vector>

// --- Các hàm bổ trợ Bitboard ---
U64 ChessBoard::getWhitePieces()
{
    return whitePawns | whiteKnights | whiteBishops | whiteRooks | whiteQueens | whiteKing;
}

U64 ChessBoard::getBlackPieces()
{
    return blackPawns | blackKnights | blackBishops | blackRooks | blackQueens | blackKing;
}

U64 ChessBoard::getAllPieces()
{
    return getWhitePieces() | getBlackPieces();
}

// --- Logic sinh nước đi cho từng loại quân ---
U64 ChessBoard::getKnightMoves(U64 knights, U64 friendlyPieces)
{
    U64 moves = 0ULL;
    moves |= (knights << 17) & notAFile;
    moves |= (knights << 15) & notHFile;
    moves |= (knights << 10) & notABFile;
    moves |= (knights << 6) & notHGFile;
    moves |= (knights >> 17) & notHFile;
    moves |= (knights >> 15) & notAFile;
    moves |= (knights >> 10) & notHGFile;
    moves |= (knights >> 6) & notABFile;
    return moves & ~friendlyPieces;
}

U64 ChessBoard::getSlidingMoves(int square, U64 allPieces, U64 friendlyPieces, const int directions[], int numDirs)
{
    U64 moves = 0ULL;
    int startRow = square / 8, startCol = square % 8;

    for (int i = 0; i < numDirs; i++)
    {
        int dRow = directions[i * 2], dCol = directions[i * 2 + 1];
        for (int step = 1; step < 8; step++)
        {
            int r = startRow + dRow * step, c = startCol + dCol * step;
            if (r < 0 || r > 7 || c < 0 || c > 7)
                break;

            int targetSquare = r * 8 + c;
            U64 targetBit = 1ULL << targetSquare;
            if (targetBit & friendlyPieces)
                break;
            moves |= targetBit;
            if (targetBit & allPieces)
                break;
        }
    }
    return moves;
}

U64 ChessBoard::getRookMoves(int square, U64 allPieces, U64 friendlyPieces)
{
    int dirs[] = {0, 1, 0, -1, 1, 0, -1, 0};
    return getSlidingMoves(square, allPieces, friendlyPieces, dirs, 4);
}

U64 ChessBoard::getBishopMoves(int square, U64 allPieces, U64 friendlyPieces)
{
    int dirs[] = {1, 1, 1, -1, -1, 1, -1, -1};
    return getSlidingMoves(square, allPieces, friendlyPieces, dirs, 4);
}

U64 ChessBoard::getQueenMoves(int square, U64 allPieces, U64 friendlyPieces)
{
    return getRookMoves(square, allPieces, friendlyPieces) | getBishopMoves(square, allPieces, friendlyPieces);
}

U64 ChessBoard::getKingMoves(int square, U64 friendlyPieces)
{
    U64 moves = 0ULL;
    U64 kingBit = (1ULL << square);
    moves |= (kingBit << 8) | (kingBit >> 8);
    moves |= (kingBit << 1) & notAFile;
    moves |= (kingBit >> 1) & notHFile;
    moves |= (kingBit << 9) & notAFile;
    moves |= (kingBit << 7) & notHFile;
    moves |= (kingBit >> 7) & notAFile;
    moves |= (kingBit >> 9) & notHFile;
    return moves & ~friendlyPieces;
}

// --- Logic An toàn và Hợp lệ ---
bool ChessBoard::isSquareAttacked(int sq, int attackerSide)
{
    U64 all = getAllPieces();
    // Mã đối phương tấn công?
    if (getKnightMoves(1ULL << sq, 0) & (attackerSide == WHITE ? whiteKnights : blackKnights))
        return true;
    // Vua đối phương đứng cạnh?
    if (getKingMoves(sq, 0) & (attackerSide == WHITE ? whiteKing : blackKing))
        return true;
    // Xe/Hậu tấn công?
    if (getRookMoves(sq, all, 0) & (attackerSide == WHITE ? (whiteRooks | whiteQueens) : (blackRooks | blackQueens)))
        return true;
    // Tượng/Hậu tấn công?
    if (getBishopMoves(sq, all, 0) & (attackerSide == WHITE ? (whiteBishops | whiteQueens) : (blackBishops | blackQueens)))
        return true;
    // Tốt đối phương tấn công?
    U64 bit = (1ULL << sq);
    if (attackerSide == WHITE)
    {
        if (((bit >> 7) & whitePawns & notAFile) || ((bit >> 9) & whitePawns & notHFile))
            return true;
    }
    else
    {
        if (((bit << 7) & blackPawns & notHFile) || ((bit << 9) & blackPawns & notAFile))
            return true;
    }
    return false;
}

bool ChessBoard::isInCheck(int side)
{
    U64 king = (side == WHITE) ? whiteKing : blackKing;
    if (!king)
        return false;
    return isSquareAttacked(__builtin_ctzll(king), 1 - side);
}

bool ChessBoard::isLegalMove(int from, int to)
{
    char capturedPiece = getPieceAt(to);
    int originalSide = sideToMove;
    int oldCastling = castlingRights; // Lưu quyền nhập thành trước khi đi

    makeMove(from, to);
    bool legal = !isInCheck(originalSide);
    unmakeMove(from, to, capturedPiece, oldCastling); // Trả lại trạng thái cũ
    sideToMove = originalSide;

    return legal;
}

// --- Hàm quan trọng nhất: Sinh tất cả nước đi hợp lệ ---

std::vector<Move> ChessBoard::generateAllLegalMoves()
{
    std::vector<Move> legalMoves;
    U64 allPieces = getAllPieces();
    U64 friendly = (sideToMove == WHITE) ? getWhitePieces() : getBlackPieces();
    U64 opponent = (sideToMove == WHITE) ? getBlackPieces() : getWhitePieces();

    // 1. Quét quân Mã (Knight)
    U64 knights = (sideToMove == WHITE) ? whiteKnights : blackKnights;
    while (knights)
    {
        int from = __builtin_ctzll(knights);
        U64 moves = getKnightMoves(1ULL << from, friendly);
        while (moves)
        {
            int to = __builtin_ctzll(moves);
            if (isLegalMove(from, to))
                legalMoves.push_back({from, to});
            moves &= (moves - 1);
        }
        knights &= (knights - 1);
    }

    // 2. Quét Xe, Tượng, Hậu (Sliding Pieces)
    U64 sliders = (sideToMove == WHITE) ? (whiteRooks | whiteBishops | whiteQueens) : (blackRooks | blackBishops | blackQueens);
    while (sliders)
    {
        int from = __builtin_ctzll(sliders);
        U64 moves = 0;
        U64 bit = 1ULL << from;
        if (bit & (whiteRooks | blackRooks | whiteQueens | blackQueens))
            moves |= getRookMoves(from, allPieces, friendly);
        if (bit & (whiteBishops | blackBishops | whiteQueens | blackQueens))
            moves |= getBishopMoves(from, allPieces, friendly);

        while (moves)
        {
            int to = __builtin_ctzll(moves);
            if (isLegalMove(from, to))
                legalMoves.push_back({from, to});
            moves &= (moves - 1);
        }
        sliders &= (sliders - 1);
    }

    // 3. Quét quân Vua (King) & Nhập thành
    U64 king = (sideToMove == WHITE) ? whiteKing : blackKing;
    if (king)
    {
        int from = __builtin_ctzll(king);
        U64 moves = getKingMoves(from, friendly);
        while (moves)
        {
            int to = __builtin_ctzll(moves);
            if (isLegalMove(from, to))
                legalMoves.push_back({from, to});
            moves &= (moves - 1);
        }

        // Logic Nhập thành
        if (sideToMove == WHITE)
        {
            if ((castlingRights & WHITE_OO) && !(allPieces & (1ULL << 5 | 1ULL << 6)) && !isSquareAttacked(4, BLACK) && !isSquareAttacked(5, BLACK) && !isSquareAttacked(6, BLACK))
                legalMoves.push_back({4, 6});
            if ((castlingRights & WHITE_OOO) && !(allPieces & (1ULL << 1 | 1ULL << 2 | 1ULL << 3)) && !isSquareAttacked(4, BLACK) && !isSquareAttacked(3, BLACK) && !isSquareAttacked(2, BLACK))
                legalMoves.push_back({4, 2});
        }
        else
        {
            if ((castlingRights & BLACK_OO) && !(allPieces & (1ULL << 61 | 1ULL << 62)) && !isSquareAttacked(60, WHITE) && !isSquareAttacked(61, WHITE) && !isSquareAttacked(62, WHITE))
                legalMoves.push_back({60, 62});
            if ((castlingRights & BLACK_OOO) && !(allPieces & (1ULL << 57 | 1ULL << 58 | 1ULL << 59)) && !isSquareAttacked(60, WHITE) && !isSquareAttacked(59, WHITE) && !isSquareAttacked(58, WHITE))
                legalMoves.push_back({60, 58});
        }
    }

    // 4. Quét quân Tốt (Pawns) - Đơn giản hóa
    U64 pawns = (sideToMove == WHITE) ? whitePawns : blackPawns;
    while (pawns)
    {
        int from = __builtin_ctzll(pawns);
        // Tốt trắng tiến lên (đánh chỉ mục theo hàng tăng dần)
        if (sideToMove == WHITE)
        {
            if (!(allPieces & (1ULL << (from + 8))))
            {
                if (isLegalMove(from, from + 8))
                    legalMoves.push_back({from, from + 8});
                if ((from / 8 == 1) && !(allPieces & (1ULL << (from + 16))))
                    if (isLegalMove(from, from + 16))
                        legalMoves.push_back({from, from + 16});
            }
            // Ăn chéo
            if ((1ULL << (from + 7)) & opponent & notHFile)
                if (isLegalMove(from, from + 7))
                    legalMoves.push_back({from, from + 7});
            if ((1ULL << (from + 9)) & opponent & notAFile)
                if (isLegalMove(from, from + 9))
                    legalMoves.push_back({from, from + 9});
        }
        else
        {
            // Tốt đen tương tự nhưng trừ 8
            if (!(allPieces & (1ULL << (from - 8))))
            {
                if (isLegalMove(from, from - 8))
                    legalMoves.push_back({from, from - 8});
                if ((from / 8 == 6) && !(allPieces & (1ULL << (from - 16))))
                    if (isLegalMove(from, from - 16))
                        legalMoves.push_back({from, from - 16});
            }
            if ((1ULL << (from - 7)) & opponent & notAFile)
                if (isLegalMove(from, from - 7))
                    legalMoves.push_back({from, from - 7});
            if ((1ULL << (from - 9)) & opponent & notHFile)
                if (isLegalMove(from, from - 9))
                    legalMoves.push_back({from, from - 9});
        }
        pawns &= (pawns - 1);
    }

    return legalMoves;
}
