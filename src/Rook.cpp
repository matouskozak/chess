//
// Created by matous on 5/4/18.
//

#include "Rook.h"
/***************************Public***************************/
/**
 * Generate all pseudo legal rooks moves
 * @param empty - bitboard with empty squares
 * @param opponentPieces - bitboard with opponent pieces
 * @param flags - current chessboard flags
 * @return list with pointers to pseudo legal moves
 */
std::list<ptrMove> Rook::generateMoves ( const Bitboard & empty, const Bitboard & opponentPieces, const Flags & flags ) const
{
  Bitboard attacksBit;
  Bitboard pushesBit;
  Bitboard movesBit;
  std::list<ptrMove> moves;

  for ( size_t i = 0; i < (boardSize * boardSize); ++i ) {
    // Locating rooks
    if ( placement[i] ) {
      movesBit = generateMovement(i, ~empty & ~opponentPieces, opponentPieces);
      attacksBit = movesBit & opponentPieces;
      pushesBit = movesBit & ~opponentPieces;
      for ( size_t j = 0; j < (boardSize * boardSize); ++j ) {
        Move testMove(i, j);

        // Determinate which rook is moving
        // Oposite => set at false => castling not possible on this side
        testMove.setCastlingOOFlag (toSquare(i).file <= EFile::e, color);
        testMove.setCastlingOOOFlag(toSquare(i).file > EFile::e, color);
        // Correct castling sides
        if ( color == EColor::white )
          testMove.setBlackCastlingFlag(true);
        else
          testMove.setWhiteCastlingFlag(true);

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
 * Generate bitboard of all squares attacked by rooks
 * @param myPieces - bitboard with my pieces
 * @param opponentPieces - bitboard with opponent pieces
 * @param flags - current chessboard flags
 * @return bitboard attacked by rooks
 */
Bitboard Rook::attacksBitboard ( const Bitboard & myPieces, const Bitboard & opponentPieces, const Flags & flags ) const
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
 * Generate bitboard of all moves rook can make
 * @param position - position on chessboard
 * @param myPieces - bitboard with my pieces
 * @param opponentPieces - bitboard with opponent pieces
 * @return bitboard with all moves by rook
 */
Bitboard Rook::generateMovement (size_t position, const Bitboard & myPieces, const Bitboard & opponentPieces) const
{
  Bitboard moves;
  Bitboard occupied = myPieces | opponentPieces;

  moves |= allNorth(position, occupied);
  moves |= allSouth(position, occupied);
  moves |= allEast (position, occupied);
  moves |= allWest (position, occupied);
  moves &= ~myPieces;

  return moves;
}
/**
 * Print rook to os
 * @param os - ostream to print into
 */
void Rook::print ( std::ostream & os ) const
{
  if ( color == EColor::white )
    os << "R";
  else
    os << "r";
}