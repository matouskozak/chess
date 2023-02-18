//
// Created by matous on 5/4/18.
//

#include "Knight.h"
/***************************Public***************************/
/**
 * Generate all pseudo legal knight moves
 * @param empty - bitboard with empty squares
 * @param opponentPieces - bitboard with opponent pieces
 * @param flags - current chessboard flags
 * @return list with pointers to pseudo legal moves
 */
std::list<ptrMove> Knight::generateMoves ( const Bitboard & empty, const Bitboard & opponentPieces,
                                           const Flags & flags ) const
{
  Bitboard attacksBit;
  Bitboard pushesBit;
  Bitboard movesBit;
  std::list<ptrMove> moves;

  // Locating Knights
  for ( size_t i = 0; i < (boardSize * boardSize); ++i ) {
    if ( placement[i] ) {
      movesBit = generateMovement(i, ~empty & ~opponentPieces, opponentPieces);
      attacksBit = movesBit & opponentPieces;
      pushesBit = movesBit & ~attacksBit;

      for ( size_t j = 0; j < (boardSize * boardSize); ++j ) {
        Move testMove(toSquare(i), toSquare(j));
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
 * Generate bitboard of all squares attacked by knights
 * @param myPieces - bitboard with my pieces
 * @param opponentPieces - bitboard with opponent pieces
 * @param flags - current chessboard flags
 * @return bitboard attacked by knights
 */
Bitboard Knight::attacksBitboard ( const Bitboard &myPieces, const Bitboard & opponentPieces, const Flags & flags ) const
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
 * Generate bitboard of all moves knight can make
 * @param position - position on chessboard
 * @param myPieces - bitboard with my pieces
 * @param opponentPieces - bitboard with opponent pieces
 * @return bitboard with all moves by knight
 */
Bitboard Knight::generateMovement(size_t position, const Bitboard &myPieces, const Bitboard &opponentPieces) const
{
  Bitboard moves;
  Bitboard piecePos;

  // North jump
  piecePos[position] = 1;
  piecePos = oneNorth(oneNorth(piecePos));
  moves |= oneWest(piecePos);
  moves |= oneEast(piecePos);
  // South jump
  piecePos.reset();
  piecePos[position] = 1;
  piecePos = oneSouth(oneSouth(piecePos));
  moves |= oneWest(piecePos);
  moves |= oneEast(piecePos);
  // West jump
  piecePos.reset();
  piecePos[position] = 1;
  piecePos = oneWest(oneWest(piecePos));
  moves |= oneSouth(piecePos);
  moves |= oneNorth(piecePos);
  // East jump
  piecePos.reset();
  piecePos[position] = 1;
  piecePos = oneEast(oneEast(piecePos));
  moves |= oneSouth(piecePos);
  moves |= oneNorth(piecePos);

  moves &= ~myPieces;

  return moves;
}
/**
 * Print knight to os
 * @param os - ostream to print into
 */
void Knight::print ( std::ostream & os ) const
{
  if ( color == EColor::white )
    os << "N";
  else
    os << "n";
}