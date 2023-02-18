//
// Created by matous on 5/4/18.
//

#ifndef CHESS_QUEEN_H
#define CHESS_QUEEN_H

#include "Piece.h"

/**
 * Class representing queens
 */
class Queen : public Piece
{
  public:
    Queen ( EColor color ) : Piece(color) {};
    virtual std::list<ptrMove> generateMoves ( const Bitboard & empty, const Bitboard & opponentPieces,
                                               const Flags & flags ) const;
    virtual Bitboard attacksBitboard ( const Bitboard & myPieces, const Bitboard & opponentPieces, const Flags & flags ) const;
    virtual inline int getPieceValue ( void ) const { return 9 * ((color == EColor::white) ? 1 : -1); };
    virtual inline EPiece toEPiece ( void ) const { return EPiece::queen; };
  protected:
    virtual Bitboard generateMovement ( size_t position, const Bitboard & myPieces, const Bitboard & opponentPieces ) const;
    virtual void print ( std::ostream & os ) const;
};


#endif //CHESS_QUEEN_H
