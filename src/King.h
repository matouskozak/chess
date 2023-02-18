//
// Created by matous on 4/30/18.
//

#ifndef CHESS_KING_H
#define CHESS_KING_H

#include "Piece.h"

/**
 * Class representing king
 */
class King : public Piece
{
  public:
    King ( EColor color ) : Piece(color) {};
    virtual std::list<ptrMove> generateMoves ( const Bitboard & empty, const Bitboard & opponentPieces,
                                               const Flags & flags ) const;
    virtual Bitboard attacksBitboard ( const Bitboard & myPieces, const Bitboard & opponentPieces, const Flags & flags ) const;
    virtual inline int getPieceValue ( void ) const { return 0 * ((color == EColor::white) ? 1 : -1); };
    virtual inline EPiece toEPiece ( void ) const { return EPiece::king; };
  protected:
    virtual Bitboard generateMovement ( size_t position, const Bitboard & myPieces, const Bitboard & opponentPieces) const;
    virtual void print ( std::ostream & os ) const;
};



#endif //CHESS_KING_H
