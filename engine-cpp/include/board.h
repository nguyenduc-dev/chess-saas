#ifndef BOARD_H
#define BOARD_H

#include <cstdint>
#include <vector>
#include <iostream>

typedef uint64_t U64;

inline constexpr U64 notAFile = 0xfefefefefefefefeULL;
inline constexpr U64 notABFile = 0xfcfcfcfcfcfcfcfcULL;
inline constexpr U64 notHFile = 0x7f7f7f7f7f7f7f7fULL;
inline constexpr U64 notHGFile = 0x3f3f3f3f3f3f3f3fULL;

enum Color
{
    WHITE = 0,
    BLACK = 1
};

enum CastlingRight
{
    WHITE_OO = 1,
    WHITE_OOO = 2,
    BLACK_OO = 4,
    BLACK_OOO = 8
};

struct Move
{
    int from;
    int to;
};

class ChessBoard
{
public:
    U64 whitePawns, whiteKnights, whiteBishops, whiteRooks, whiteQueens, whiteKing;
    U64 blackPawns, blackKnights, blackBishops, blackRooks, blackQueens, blackKing;
    int sideToMove;
    int castlingRights;

    ChessBoard();
    void setupInitialPosition();

    // Các hàm bổ trợ Bitboard
    U64 getWhitePieces();
    U64 getBlackPieces();
    U64 getAllPieces();

    // Sinh nước đi (Move Generation)
    U64 getKnightMoves(U64 knights, U64 friendlyPieces);
    U64 getSlidingMoves(int square, U64 allPieces, U64 friendlyPieces, const int directions[], int numDirs);
    U64 getRookMoves(int square, U64 allPieces, U64 friendlyPieces);
    U64 getBishopMoves(int square, U64 allPieces, U64 friendlyPieces);
    U64 getQueenMoves(int square, U64 allPieces, U64 friendlyPieces);
    U64 getKingMoves(int square, U64 friendlyPieces);

    // Logic nước đi và An toàn
    void makeMove(int from, int to);
    void unmakeMove(int from, int to, char capturedPiece, int oldCastlingRights);
    bool isSquareAttacked(int sq, int attackerSide);
    bool isInCheck(int side);
    bool isLegalMove(int from, int to);
    char getPieceAt(int sq);

    // Hàm hỗ trợ nhập thành
    void updateCastlingRights(int from, int to);

    // In ấn (Utilities)
    void printBoard();
    void printBitboard(U64 bitboard);

    // Hàm mới cho AI
    std::vector<Move> generateAllLegalMoves();

    // Hàm đánh giá thế cờ
    int evaluatePosition();

    // 2 hàm này cho AI
    int search(int depth, int alpha, int beta);
    Move getBestMove(int depth);
};

#endif
