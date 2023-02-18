//
// Created by matous on 5/4/18.
//

#ifndef CHESS_ROOK_H
#define CHESS_ROOK_H

#include "Piece.h"

/**
 * Class representing rooks
 */
class Rook : public Piece
{
  public:
    Rook ( EColor color ) : Piece(color) {};
    virtual std::list<ptrMove> generateMoves ( const Bitboard & empty, const Bitboard & opponentPieces,
                                               const Flags & flags ) const;
    virtual Bitboard attacksBitboard ( const Bitboard & myPieces, const Bitboard & opponentPieces, const Flags & flags ) const;
    virtual inline int getPieceValue ( void ) const { return 5  * ((color == EColor::white) ? 1 : -1); };
    virtual inline EPiece toEPiece ( void ) const { return EPiece::rook; };
  protected:
    virtual Bitboard generateMovement ( size_t position, const Bitboard & myPieces, const Bitboard & opponentPieces ) const;
    virtual void print ( std::ostream & os ) const;

};


#endif //CHESS_ROOK_H
