//
// Created by matous on 4/30/18.
//

#ifndef CHESS_PAWN_H
#define CHESS_PAWN_H

#include "Piece.h"

/**
 * Class representing pawns
 */
class Pawn : public Piece
{
  public:
    Pawn ( EColor color ) : Piece(color) {};
    virtual std::list<ptrMove> generateMoves ( const Bitboard & empty, const Bitboard & opponentPieces,
                                               const Flags & flags ) const;
    virtual Bitboard attacksBitboard ( const Bitboard & myPieces, const Bitboard & opponentPieces, const Flags & flags ) const;
    virtual inline int getPieceValue ( void ) const { return 1 * ((color == EColor::white) ? 1 : -1); };
    virtual inline EPiece toEPiece ( void ) const { return EPiece::pawn; };

  protected:
    virtual Bitboard generateMovement (size_t position, const Bitboard & myPieces, const Bitboard & opponentPieces ) const;
    virtual void print ( std::ostream & os ) const;

    std::list<ptrMove> generatePushes  ( const Bitboard & empty, const Flags & flags ) const;
    std::list<ptrMove> generateAttacks ( const Bitboard & opponent, const Flags & flags ) const;
    std::list<ptrMove> generatePushes  ( size_t position, const Bitboard & empty, const Flags & flags ) const;
    std::list<ptrMove> generateAttacks ( size_t position, const Bitboard & opponent, const Flags & flags ) const;
};

#endif //CHESS_PAWN_H
