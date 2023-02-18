//
// Created by matous on 5/4/18.
//

#include "Bishop.h"
/***************************Public***************************/
/**
 * Generate all pseudo legal bishop moves
 * @param empty - bitboard with empty squares
 * @param opponentPieces - bitboard with opponent pieces
 * @param flags - current chessboard flags
 * @return list with pointers to pseudo legal moves
 */
std::list<ptrMove> Bishop::generateMoves ( const Bitboard & empty, const Bitboard & opponentPieces,
                                           const Flags & flags ) const
{
  Bitboard attacksBit;
  Bitboard pushesBit;
  Bitboard movesBit;
  std::list<ptrMove> moves;

  // Locating Bishops
  for ( size_t i = 0; i < (boardSize * boardSize); ++i ) {
    if ( placement[i] ) {
      movesBit = generateMovement(i, ~empty & ~opponentPieces, opponentPieces);
      attacksBit = movesBit & opponentPieces;
      pushesBit = movesBit & ~opponentPieces;

      for ( size_t j = 0; j < (boardSize * boardSize); ++j ) {
        Move testMove(i, j);
        if ( attacksBit[j] ) {
          testMove.capture = toSquare(j);
          moves.push_back(std::make_shared<Move>(testMove));
        } else if ( pushesBit[j] ) {
          moves.push_back(std::make_shared<Move>(testMove));
        }
      }
    }
  }
  return moves;
}
/**
 * Generate bitboard of all squares attacked by bishops
 * @param myPieces - bitboard with my pieces
 * @param opponentPieces - bitboard with opponent pieces
 * @param flags - current chessboard flags
 * @return bitboard attacked by bishops
 */
Bitboard Bishop::attacksBitboard ( const Bitboard & myPieces, const Bitboard & opponentPieces, const Flags & flags ) const
{
  Bitboard attacks;

  for ( size_t i = 0; i < (boardSize * boardSize); ++i ) {
    if ( placement[i] ) {
      attacks |= generateMovement(i, myPieces, opponentPieces);
    }
  }
  attacks &= opponentPieces;

  return attacks;
}
/***************************Private***************************/
/**
 * Generate bitboard of all moves bishop can make
 * @param position - position on chessboard
 * @param myPieces - bitboard with my pieces
 * @param opponentPieces - bitboard with opponent pieces
 * @return bitboard with all moves by bishop
 */
Bitboard Bishop::generateMovement ( size_t position, const Bitboard & myPieces, const Bitboard & opponentPieces ) const
{
  Bitboard moves;
  Bitboard occupied = myPieces | opponentPieces;
  moves |= allNoEa(position, occupied);
  moves |= allNoWe(position, occupied);
  moves |= allSoEa(position, occupied);
  moves |= allSoWe(position, occupied);

  moves &= ~myPieces;

  return moves;
}
/**
 * Print bishop to os
 * @param os - ostream to print into
 */
void Bishop::print ( std::ostream & os ) const
{
  if ( color == EColor::white )
    os << "B";
  else
    os << "b";
}
