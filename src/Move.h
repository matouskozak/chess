//
// Created by matous on 5/1/18.
//

#ifndef CHESS_MOVE_H
#define CHESS_MOVE_H


#include "const_chess_define.h"
#include <iostream>


/**
 * Move structure
 * Holding all important flags needed to process move correctly
 * No need to keep private, we cannot predict flags for each move at time of creations
 */
class Move
{
  public:
    Move ( const Move & other ) = default;
    Move ( const Square & fromSq, const Square & toSq );
    Move ( size_t from, size_t to );

    void setCastlingOOOFlag(bool value, EColor color);
    void setCastlingOOFlag(bool value, EColor color);
    void setMoveCastlingOOO ( bool value, EColor color );
    void setMoveCastlingOO ( bool value, EColor color );
    inline void setWhiteCastlingFlag(bool value) { flags.whiteCastling.OOO = value; flags.whiteCastling.OO = value; };
    inline void setBlackCastlingFlag(bool value) { flags.blackCastling.OOO = value; flags.blackCastling.OO = value; };

    bool operator == ( const Move & other ) const;

    Square from;
    Square to;

    Flags flags;    /**
     * enPassantTargetSq
     * whiteCastling - set at false, if castling is not possible
     * blackCastling
     */

    //! If move type is castling
    Castling whiteDoesCastling;
    Castling blackDoesCastling;

    bool pawnMove = false;
    bool valid = false;
    Square capture; //!< Square on which is captured piece (for enPassant)
    EPiece capturedPiece;

    //! Pawn promotion
    bool canPromote = false;
    EPiece promotePiece = EPiece::pawn;


  friend std::ostream & operator << ( std::ostream & os, const Move & move );
};
#endif //CHESS_MOVE_H
