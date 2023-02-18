#include "Pawn.h"

/***************************Public***************************/
/**
 * Generate all pseudo legal pawn moves
 * @param empty - bitboard with empty squares
 * @param opponentPieces - bitboard with opponent pieces
 * @param flags - current chessboard flags
 * @return list with pointers to pseudo legal moves
 */
std::list<ptrMove> Pawn::generateMoves ( const Bitboard & empty, const Bitboard & opponentPieces, const Flags & flags ) const
{
  std::list<ptrMove> moves;
  std::list<ptrMove> attacks = generateAttacks(opponentPieces, flags);
  std::list<ptrMove> pushes = generatePushes(empty, flags);

  for ( auto i = attacks.cbegin(); i != attacks.cend(); ++i )
    moves.push_back(*i);
  for ( auto i = pushes.cbegin(); i != pushes.cend(); ++i )
    moves.push_back(*i);

  return moves;
}
/**
 * Generate bitboard of all squares attacked by pawns
 * @param myPieces - bitboard with my pieces
 * @param opponentPieces - bitboard with opponent pieces
 * @param flags - current chessboard flags
 * @return bitboard attacked by pawns
 */
Bitboard Pawn::attacksBitboard ( const Bitboard & myPieces, const Bitboard & opponentPieces, const Flags & flags ) const
{
  Bitboard westAttacks;
  Bitboard eastAttacks;
  Bitboard copyOp = opponentPieces;

  // En Passant target sq is set
  if ( flags.enPassantTargetSq.rank != 0 )
    copyOp[offset(flags.enPassantTargetSq)] = 1;

  if ( color == EColor::white ) {
    westAttacks = oneNoWe(placement) & copyOp;
    eastAttacks = oneNoEa(placement) & copyOp;
  } else {
    westAttacks = oneSoWe(placement) & copyOp;
    eastAttacks = oneSoEa(placement) & copyOp;
  }

  return westAttacks | eastAttacks;
}

/***************************Private***************************/
/**
 * NOT USABLE, Pawn moves to specific
 * Generate bitboard of all moves pawn can make
 * @param position - position on chessboard
 * @param myPieces - bitboard with my pieces
 * @param opponentPieces - bitboard with opponent pieces
 * @return bitboard with all moves by pawn
 */
Bitboard Pawn::generateMovement ( size_t position, const Bitboard & myPieces, const Bitboard & opponentPieces ) const
{
  Bitboard moves;

  return moves;
}
/**
 * Print pawn to os
 * @param os - ostream to print into
 */
void Pawn::print ( std::ostream & os ) const
{
  if ( color == EColor::white )
    os << "P";
  else
    os << "p";
}
/**
 * Generate all pushes by pawns
 * @param empty - bitboard with empty squares
 * @param flags - current chessboard flags
 * @return list with pointers to push moves
 */
std::list<ptrMove> Pawn::generatePushes ( const Bitboard & empty, const Flags & flags ) const
{
  std::list <ptrMove> moves;

  // Locating pawns
  for ( size_t i = 0; i < (boardSize * boardSize); ++i ) {
    if ( placement[i] ) {
      std::list <ptrMove> movesFrom = generatePushes(i, empty, flags);

      for ( auto j = movesFrom.cbegin(); j != movesFrom.cend(); ++j )
        moves.push_back(*j);
    }
  }

  return moves;
}
/**
 * Generate all pushes pawn at position
 * @param position - position of pawn
 * @param empty - bitboard with empty squares
 * @param flags - current chessboard flags
 * @return list with pointers to push moves
 */
std::list<ptrMove> Pawn::generatePushes ( size_t position, const Bitboard & empty, const Flags & flags ) const
{
  Bitboard piecePosition;
  Bitboard singlePush;
  Bitboard doublePush;
  std::list<ptrMove> moves;


  piecePosition[position] = 1;

  if ( color == EColor::white ) {   // White player
    singlePush = oneNorth(piecePosition) & empty;
    const Bitboard row4 = Bitboard(0x00000000FF000000);
    doublePush = oneNorth(singlePush) & empty & row4;

    Bitboard promotion = ( color == EColor::white ? rank8 : rank1) & singlePush;

    for ( size_t i = 0; i < (boardSize * boardSize); ++i )  {
      Move testMove (position, i);
      testMove.pawnMove = true;

      if ( promotion.test(i) )  {
        testMove.canPromote = true;
        testMove.promotePiece = EPiece::queen;
      }

      if ( singlePush.test(i) )
        moves.push_back(std::make_shared<Move>(testMove));
      else if ( doublePush.test(i) ) {
        testMove.flags.enPassantTargetSq = toSquare(i - 8);
        moves.push_back(std::make_shared<Move>(testMove));
      }
    }
  } else {                        // Black Player
    singlePush = oneSouth(piecePosition) & empty;
    const Bitboard row5 = Bitboard(0x000000FF00000000);
    doublePush = oneSouth(singlePush) & empty & row5;
    Bitboard promotion = ((color == EColor::white) ? rank8 : rank1) & (singlePush | doublePush);

    for ( size_t i = 0; i < (boardSize * boardSize); ++i )  {
      Move testMove (position, i);
      testMove.pawnMove = true;

      if ( promotion.test(i) ) {
        //Move promotionMove (testMove);
        testMove.canPromote = true;
        testMove.promotePiece = EPiece::queen;
      }
      if ( singlePush.test(i) )
        moves.push_back(std::make_shared<Move>(testMove));
      else if ( doublePush.test(i) ) {
        testMove.flags.enPassantTargetSq = toSquare(i + 8);
        moves.push_back(std::make_shared<Move>(testMove));
      }
    }
  }

  return moves;
}
/**
 * Generate all attacks by pawns
 * @param opponent - bitboard with opponent pieces
 * @param flags - current chessboard flags
 * @return list with pointers to attack moves
 */
std::list<ptrMove> Pawn::generateAttacks ( const Bitboard & opponent, const Flags & flags ) const
{
  std::list <ptrMove> attacks;
  Bitboard copyOp = opponent;
  // En Passant target sq is set
  if ( flags.enPassantTargetSq.rank != 0 )
    copyOp[offset(flags.enPassantTargetSq)] = 1;

  for ( size_t i = 0; i < (boardSize * boardSize); ++i ) {
    if ( placement[i] ) {
      std::list <ptrMove> attacksFrom = generateAttacks(i, copyOp, flags);

      for ( auto j = attacksFrom.cbegin(); j != attacksFrom.cend(); ++j )
        attacks.push_back(*j);
    }
  }

  return attacks;
}
/**
 * Generate all attacks by pawn
 * @param opponent - bitboard with opponent pieces
 * @param flags - current chessboard flags
 * @return list with pointers to attack moves
 */
std::list<ptrMove> Pawn::generateAttacks ( size_t position, const Bitboard & opponent, const Flags & flags ) const
{
  std::list<ptrMove> attacks;
  Bitboard piecePosition;
  Bitboard westAttacks;
  Bitboard eastAttacks;

  piecePosition[position] = 1;

  if ( color == EColor::white ) {
    westAttacks = oneNoWe(piecePosition) & opponent;
    eastAttacks = oneNoEa(piecePosition) & opponent;
  } else {
    westAttacks = oneSoWe(piecePosition) & opponent;
    eastAttacks = oneSoEa(piecePosition) & opponent;
  }
  Bitboard promotion = ((color == EColor::white) ? rank8 : rank1) & (westAttacks | eastAttacks);

  for ( size_t i = 0; i < (boardSize * boardSize); ++i )  {
    Move testMove (position, i);
    if ( toSquare(i) == flags.enPassantTargetSq ) {
      // Set Square where is located capture pawn in enPassant move
      if ( color == EColor::white )
        testMove.capture = toSquare(i - 8);
      else
        testMove.capture = toSquare(i + 8);
    }
    else
      testMove.capture = toSquare(i);

    if ( promotion.test(i) ) {
      testMove.canPromote = true;
      testMove.promotePiece = EPiece::queen;
    }

    if ( westAttacks.test(i) || eastAttacks.test(i) )
      attacks.push_back(std::make_shared<Move>(testMove));
  }
  return attacks;
}


