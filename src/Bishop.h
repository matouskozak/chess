//
// Created by matous on 5/4/18.
//

#ifndef CHESS_BISHOP_H
#define CHESS_BISHOP_H

#include "Piece.h"

/**
 * Class representing bishops
 */
class Bishop : public Piece
{
  public:
    Bishop ( EColor color ) : Piece(color) {};
    virtual std::list<ptrMove> generateMoves ( const Bitboard & empty, const Bitboard & opponentPieces,
                                               const Flags & flags ) const;
    virtual Bitboard attacksBitboard ( const Bitboard & myPieces, const Bitboard & opponentPieces, const Flags & flags ) const;
    virtual inline int getPieceValue ( void ) const { return 3 * ((color == EColor::white) ? 1 : -1); };
    virtual inline EPiece toEPiece ( void ) const { return EPiece::bishop; };
  protected:
    virtual Bitboard generateMovement ( size_t position, const Bitboard & myPieces, const Bitboard & opponentPieces ) const;
    virtual void print ( std::ostream & os ) const;
};


#endif //CHESS_BISHOP_H
