#ifndef CHESS_FIGURE_H
#define CHESS_FIGURE_H


#include "Move.h"

#include <list>
#include <memory>



typedef std::shared_ptr<Move> ptrMove;

/**
 * Abstract class representing chess pieces
 */
class Piece
{
  public:
    Piece ( EColor color );
    virtual std::list<ptrMove> generateMoves ( const Bitboard & empty, const Bitboard & opponentPieces,
                                               const Flags & flags ) const = 0;
    virtual Bitboard attacksBitboard ( const Bitboard &myPieces, const Bitboard & opponentPieces, const Flags & flags ) const = 0;
    virtual int getPieceValue ( void ) const = 0;
    virtual EPiece toEPiece ( void ) const = 0;

    bool empty ( EFile file, int rank ) const;
    inline EColor getColor ( void ) const { return color; };
    inline const Bitboard & getPlacement ( void ) const { return placement; };
    inline void setPlacement ( const Bitboard & placement ) { this -> placement = placement; };
    inline void flipBit ( std::size_t position ) { placement.flip(position); };
  protected:
    Bitboard placement; //!< Piece position on bitboard
    EColor color;

    virtual Bitboard generateMovement ( size_t position, const Bitboard & myPieces, const Bitboard & opponentPieces ) const = 0;
    virtual void print ( std::ostream & os ) const = 0;

    Bitboard oneNorth (const Bitboard & bitboard) const;
    Bitboard oneSouth (const Bitboard & bitboard) const;
    Bitboard oneWest  (const Bitboard & bitboard) const;
    Bitboard oneEast  (const Bitboard & bitboard) const;
    Bitboard oneNoEa  (const Bitboard & bitboard) const;
    Bitboard oneNoWe  (const Bitboard & bitboard) const;
    Bitboard oneSoEa  (const Bitboard & bitboard) const;
    Bitboard oneSoWe  (const Bitboard & bitboard) const;
    Bitboard allNorth ( std::size_t position, const Bitboard & board ) const;
    Bitboard allSouth ( std::size_t position, const Bitboard & board ) const;
    Bitboard allWest  ( std::size_t position, const Bitboard & board ) const;
    Bitboard allEast  ( std::size_t position, const Bitboard & board ) const;
    Bitboard allNoWe  ( std::size_t position, const Bitboard & board ) const;
    Bitboard allNoEa  ( std::size_t position, const Bitboard & board ) const;
    Bitboard allSoWe  ( std::size_t position, const Bitboard & board ) const;
    Bitboard allSoEa  ( std::size_t position, const Bitboard & board ) const;

  friend std::ostream & operator << ( std::ostream & os, const Piece & x );
};

#endif //CHESS_FIGURE_H

