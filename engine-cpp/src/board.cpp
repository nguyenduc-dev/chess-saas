#include "board.h"
#include <iostream>

ChessBoard::ChessBoard()
{
    // Khởi tạo tất cả bằng 0 (bàn cờ trống)
    whitePawns = whiteKnights = whiteBishops = whiteRooks = whiteQueens = whiteKing = 0ULL;
    blackPawns = blackKnights = blackBishops = blackRooks = blackQueens = blackKing = 0ULL;

    sideToMove = WHITE;
    castlingRights = 15; // 15 = 0b1111 (Cả 2 bên đều có đủ 2 quyền Nhập thành)
}

void ChessBoard::setupInitialPosition()
{
    // Quân Trắng (Hàng 1 và 2)
    whitePawns = 0x000000000000FF00ULL;   // a2-h2
    whiteKnights = 0x0000000000000042ULL; // b1, g1
    whiteBishops = 0x0000000000000024ULL; // c1, f1
    whiteRooks = 0x0000000000000081ULL;   // a1, h1
    whiteQueens = 0x0000000000000008ULL;  // d1
    whiteKing = 0x0000000000000010ULL;    // e1

    // Quân Đen (Hàng 7 và 8)
    blackPawns = 0x00FF000000000000ULL;   // a7-h7
    blackKnights = 0x4200000000000000ULL; // b8, g8
    blackBishops = 0x2400000000000000ULL; // c8, f8
    blackRooks = 0x8100000000000000ULL;   // a8, h8
    blackQueens = 0x0800000000000000ULL;  // d8
    blackKing = 0x1000000000000000ULL;    // e8

    sideToMove = WHITE;
    castlingRights = 15;
}

void ChessBoard::makeMove(int from, int to)
{
    char piece = getPieceAt(from);

    U64 fromBit = (1ULL << from);
    U64 toBit = (1ULL << to);
    U64 moveMask = fromBit | toBit;

    // 1. Di chuyển quân cờ (Cả Trắng và Đen)
    if (whitePawns & fromBit)
        whitePawns ^= moveMask;
    else if (whiteKnights & fromBit)
        whiteKnights ^= moveMask;
    else if (whiteBishops & fromBit)
        whiteBishops ^= moveMask;
    else if (whiteRooks & fromBit)
        whiteRooks ^= moveMask;
    else if (whiteQueens & fromBit)
        whiteQueens ^= moveMask;
    else if (whiteKing & fromBit)
        whiteKing ^= moveMask;
    else if (blackPawns & fromBit)
        blackPawns ^= moveMask;
    else if (blackKnights & fromBit)
        blackKnights ^= moveMask;
    else if (blackBishops & fromBit)
        blackBishops ^= moveMask;
    else if (blackRooks & fromBit)
        blackRooks ^= moveMask;
    else if (blackQueens & fromBit)
        blackQueens ^= moveMask;
    else if (blackKing & fromBit)
        blackKing ^= moveMask;

    // 2. Xử lý ăn quân (Xóa quân đối phương ở ô 'to')
    U64 captureMask = ~toBit;
    if (sideToMove == WHITE)
    {
        blackPawns &= captureMask;
        blackKnights &= captureMask;
        blackBishops &= captureMask;
        blackRooks &= captureMask;
        blackQueens &= captureMask;
        blackKing &= captureMask;
    }
    else
    {
        whitePawns &= captureMask;
        whiteKnights &= captureMask;
        whiteBishops &= captureMask;
        whiteRooks &= captureMask;
        whiteQueens &= captureMask;
        whiteKing &= captureMask;
    }

    // 3. Xử lý di chuyển quân Xe khi Nhập thành
    if (piece == 'K' || piece == 'k')
    {
        if (to - from == 2)
        { // Nhập thành cánh Vua (O-O)
            int rookFrom = (sideToMove == WHITE) ? 7 : 63;
            int rookTo = (sideToMove == WHITE) ? 5 : 61;
            // Di chuyển Xe bằng XOR
            if (sideToMove == WHITE)
                whiteRooks ^= ((1ULL << rookFrom) | (1ULL << rookTo));
            else
                blackRooks ^= ((1ULL << rookFrom) | (1ULL << rookTo));
        }
        else if (from - to == 2)
        { // Nhập thành cánh Hậu (O-O-O)
            int rookFrom = (sideToMove == WHITE) ? 0 : 56;
            int rookTo = (sideToMove == WHITE) ? 3 : 59;
            if (sideToMove == WHITE)
                whiteRooks ^= ((1ULL << rookFrom) | (1ULL << rookTo));
            else
                blackRooks ^= ((1ULL << rookFrom) | (1ULL << rookTo));
        }
    }

    updateCastlingRights(from, to);

    // 3. Đổi lượt đi
    sideToMove = 1 - sideToMove;
}

void ChessBoard::unmakeMove(int from, int to, char capturedPiece, int oldCastlingRights)
{
    U64 fromBit = (1ULL << from);
    U64 toBit = (1ULL << to);
    U64 moveMask = fromBit | toBit;
    char piece = getPieceAt(to);

    // 1. Tìm quân cờ đang ở ô 'to' và đưa nó về 'from'
    if (whitePawns & toBit)
        whitePawns ^= moveMask;
    else if (whiteKnights & toBit)
        whiteKnights ^= moveMask;
    else if (whiteBishops & toBit)
        whiteBishops ^= moveMask;
    else if (whiteRooks & toBit)
        whiteRooks ^= moveMask;
    else if (whiteQueens & toBit)
        whiteQueens ^= moveMask;
    else if (whiteKing & toBit)
        whiteKing ^= moveMask;
    else if (blackPawns & toBit)
        blackPawns ^= moveMask;
    else if (blackKnights & toBit)
        blackKnights ^= moveMask;
    else if (blackBishops & toBit)
        blackBishops ^= moveMask;
    else if (blackRooks & toBit)
        blackRooks ^= moveMask;
    else if (blackQueens & toBit)
        blackQueens ^= moveMask;
    else if (blackKing & toBit)
        blackKing ^= moveMask;

    // 2. Khôi phục quân bị ăn tại ô 'to'
    if (capturedPiece != ' ')
    {
        U64 toBit64 = (1ULL << to);
        switch (capturedPiece)
        {
        case 'P':
            whitePawns |= toBit64;
            break;
        case 'N':
            whiteKnights |= toBit64;
            break;
        case 'B':
            whiteBishops |= toBit64;
            break;
        case 'R':
            whiteRooks |= toBit64;
            break;
        case 'Q':
            whiteQueens |= toBit64;
            break;
        case 'K':
            whiteKing |= toBit64;
            break;
        case 'p':
            blackPawns |= toBit64;
            break;
        case 'n':
            blackKnights |= toBit64;
            break;
        case 'b':
            blackBishops |= toBit64;
            break;
        case 'r':
            blackRooks |= toBit64;
            break;
        case 'q':
            blackQueens |= toBit64;
            break;
        case 'k':
            blackKing |= toBit64;
            break;
        }
    }
    // 3. Xử lý di chuyển ngược cho Xe nếu là Nhập thành
    if (piece == 'K' || piece == 'k')
    {
        if (to - from == 2)
        { // Vừa nhập thành cánh Vua
            int rookFrom = (piece == 'K') ? 7 : 63;
            int rookTo = (piece == 'K') ? 5 : 61;
            if (piece == 'K')
                whiteRooks ^= ((1ULL << rookFrom) | (1ULL << rookTo));
            else
                blackRooks ^= ((1ULL << rookFrom) | (1ULL << rookTo));
        }
        else if (from - to == 2)
        { // Vừa nhập thành cánh Hậu
            int rookFrom = (piece == 'K') ? 0 : 56;
            int rookTo = (piece == 'K') ? 3 : 59;
            if (piece == 'K')
                whiteRooks ^= ((1ULL << rookFrom) | (1ULL << rookTo));
            else
                blackRooks ^= ((1ULL << rookFrom) | (1ULL << rookTo));
        }
    }

    castlingRights = oldCastlingRights;
    sideToMove = 1 - sideToMove;
}

// Trong board.cpp
void ChessBoard::updateCastlingRights(int from, int to)
{
    // Mảng mặt nạ cho 64 ô. Mặc định là 15 (0b1111 - giữ nguyên quyền)
    // Chỉ những ô quan trọng mới có giá trị khác 15
    static const int castlingMask[64] = {
        7, 15, 15, 15, 3, 15, 15, 11, // Hàng 1: a1(7), e1(3), h1(11)
        15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15,
        15, 15, 15, 15, 15, 15, 15, 15,
        13, 15, 15, 15, 12, 15, 15, 14 // Hàng 8: a8(13), e8(12), h8(14)
    };

    // Cập nhật quyền dựa trên ô đi và ô đến
    // Ví dụ: Nếu Vua trắng đi từ e1 (index 4), castlingRights &= 3 (giữ lại quyền của Đen)
    castlingRights &= castlingMask[from];
    castlingRights &= castlingMask[to];
}

char ChessBoard::getPieceAt(int sq)
{
    U64 bit = (1ULL << sq);
    if (whitePawns & bit)
        return 'P';
    if (whiteKnights & bit)
        return 'N';
    if (whiteBishops & bit)
        return 'B';
    if (whiteRooks & bit)
        return 'R';
    if (whiteQueens & bit)
        return 'Q';
    if (whiteKing & bit)
        return 'K';

    if (blackPawns & bit)
        return 'p';
    if (blackKnights & bit)
        return 'n';
    if (blackBishops & bit)
        return 'b';
    if (blackRooks & bit)
        return 'r';
    if (blackQueens & bit)
        return 'q';
    if (blackKing & bit)
        return 'k';
    return ' ';
}
