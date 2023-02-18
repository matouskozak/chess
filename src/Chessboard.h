//
// Created by matous on 4/30/18.
//

#ifndef CHESS_CHESSBOARD_H
#define CHESS_CHESSBOARD_H

#include "Move.h"
#include "Pawn.h"
#include "Knight.h"
#include "Bishop.h"
#include "Rook.h"
#include "Queen.h"
#include "King.h"

#include "iostream"
#include <sstream>
#include "memory"
#include <list>
#include <deque>
#include <climits>

typedef std::shared_ptr<Piece> ptrPiece;

/**
 * Chessboard class representing chessbord, pieces represented as bits in bitboards
 * Processing moves, controlling rules, etc.
 */
class Chessboard
{
  public:
    Chessboard ( const std::string & fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" );
    ~Chessboard ( void );
    void initializeBoard ( const std::string & fen );
    int processMove ( const ptrMove & move );
    int unProcessMove ( const ptrMove & move );
    inline bool playerInCheck ( void ) const { return inCheck; };
    const std::string getFen(void) const;
    inline EColor getPlayer ( void ) const { return playerOnTurn; };
    inline const std::list<ptrMove> & getValidMoves ( void ) const { return *(--validMovesStack.cend()); };
    int evaluateBoard ( void ) const;
  private:
    EColor playerOnTurn;
    Flags flags;              //!< Current flags
    bool inCheck;             //!< Player on turn is in check
    size_t halfMoveClock;     //!< For 50 move draw rule, reset to 0 after capture or pawn move
    size_t fullMoveClock;     //!< Starts at 1, incremented after Black's move

    ptrPiece whitePieces [NUM_PIECES];
    ptrPiece blackPieces [NUM_PIECES];

    Bitboard allWhite, allBlack, emptySpace, allPieces;

    std::list<Flags> flagsStack;                        //!< Previous flags (for undoing moves)
    std::deque<std::list<ptrMove>> validMovesStack;     //!< Previous valid moves (speeds up AI)

    std::list<ptrMove> searchValidMoves ( EColor playerColor ) const;
    void playMove ( const Move & move );
    void unPlayMove ( const Move & move );
    ptrPiece pieceAt ( EFile col, int row ) const;
    ptrPiece pieceAt ( const Square & sq ) const;
    void reFillHelpBitboards(void);
    Bitboard playerAttacks ( EColor playerColor, const Bitboard & opponentPieces ) const;
    bool kingInCheck ( EColor playerColor ) const;
    bool canCastle ( EColor playerColor, Castling castlingType ) const;

  friend std::ostream & operator << ( std::ostream & os, const Chessboard & board );
};

#endif //CHESS_CHESSBOARD_H
