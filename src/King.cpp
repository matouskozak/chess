#include "King.h"
/***************************Public***************************/
/**
 * Generate all pseudo legal king moves
 * @param empty - bitboard with empty squares
 * @param opponentPieces - bitboard with opponent pieces
 * @param flags - current chessboard flags
 * @return list with pointers to pseudo legal moves
 */
std::list<ptrMove> King::generateMoves ( const Bitboard & empty, const Bitboard & opponentPieces, const Flags & flags ) const
{
  // Position does not matter (one king only)
  Bitboard movesBit = generateMovement(10, ~empty & ~opponentPieces, opponentPieces);
  Bitboard attacksBit = movesBit & opponentPieces;
  Bitboard pushesBit = movesBit & ~opponentPieces;
  size_t startPos;

  std::list <ptrMove> moves;

  // Locating king
  for ( size_t i = 0; i < (boardSize * boardSize); ++i ) {
    std::list<Move *> movesFrom;
    if ( placement[i] ) {
      startPos = i;
      break;  // Only 1 king in bitboard
    }
  }

  for ( size_t i = 0; i < (boardSize * boardSize); ++i ) {
    Move testMove (startPos, i);

    // Set castling ability to false, because king moves
    if ( color == EColor::white )
      testMove.setWhiteCastlingFlag(false);
    else
      testMove.setBlackCastlingFlag(false);


    if ( attacksBit[i] ) {
      testMove.capture = toSquare(i);
      moves.push_back(std::make_shared<Move>(testMove));
    } else if ( pushesBit[i] ) {
      moves.push_back(std::make_shared<Move>(testMove));
    }
  }

  // Castling moves
  Bitboard kingRank = (color == EColor::white ? rank1 : rank8);
  // King side
  if ( (flags.blackCastling.OO && (color == EColor::black))
       || (flags.whiteCastling.OO && (color == EColor::white)) ) {
     // Empty space between king and rook
    if ( (kingRank & (fileF | fileG) & ~empty) == 0 ) {
      Move castleMove(startPos, startPos + 2);
      castleMove.setMoveCastlingOO(true, color);
      // Disable castling
      castleMove.setCastlingOOOFlag(false, color);
      castleMove.setCastlingOOFlag (false, color);
      moves.push_back(std::make_shared<Move>(castleMove));
    }
  }
  // Queen side
  if ( (flags.blackCastling.OOO && (color == EColor::black))
       || (flags.whiteCastling.OOO && (color == EColor::white)) ) {
    // Empty space between king and rook
    if ( (kingRank & (fileB | fileC | fileD ) & ~empty) == 0 ) {
      Move castleMove(startPos, startPos - 2);
      castleMove.setMoveCastlingOOO(true, color);
      // Disable castling
      castleMove.setCastlingOOOFlag(false, color);
      castleMove.setCastlingOOFlag (false, color);
      moves.push_back(std::make_shared<Move>(castleMove));
    }
  }


  return moves;
}
/**
 * Generate bitboard of all squares attacked by king
 * @param myPieces - bitboard with my pieces
 * @param opponentPieces - bitboard with opponent pieces
 * @param flags - current chessboard flags
 * @return bitboard attacked by king
 */
Bitboard King::attacksBitboard(const Bitboard &myPieces, const Bitboard &opponentPieces, const Flags &flags) const
{
  Bitboard attacks;
  for ( size_t i = 0; i < (boardSize * boardSize); ++i ) {
    if ( placement[i] ) {
      attacks = generateMovement(i, myPieces, opponentPieces);
      break;
    }
  }
  attacks &= opponentPieces;
  return attacks;
}
/***************************Private***************************/
/**
 * Generate bitboard of all moves king can make
 * @param position - position on chessboard
 * @param myPieces - bitboard with my pieces
 * @param opponentPieces - bitboard with opponent pieces
 * @return bitboard with all moves by king
 */
Bitboard King::generateMovement ( size_t position, const Bitboard & myPieces, const Bitboard & opponentPieces ) const
{
  Bitboard moves;

  moves |= oneNorth(placement);
  moves |= oneNoEa(placement);
  moves |= oneEast(placement);
  moves |= oneSoEa(placement);
  moves |= oneSouth(placement);
  moves |= oneSoWe(placement);
  moves |= oneWest(placement);
  moves |= oneNoWe(placement);

  moves &= ~myPieces;

  return moves;
}
/**
 * Print king to os
 * @param os - ostream to print into
 */
void King::print ( std::ostream & os ) const
{
  if ( color == EColor::white )
    os << "K";
  else
    os << "k";
}