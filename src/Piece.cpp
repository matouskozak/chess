#include "Piece.h"
/***************************Public***************************/
/**
 * Constructor for class Piece
 * @param color - piece color
 */
Piece::Piece ( EColor color ) : color(color)
{

}
/**
 * Finds if position is placement is empty
 * @param file
 * @param rank
 * @return piece is occupiing give position
 */
bool Piece::empty ( EFile file, int rank ) const
{
  if ( placement[offset(file, rank)] )
    return false;

  return true;
}
/***************************Private***************************/
/**
 * Sliding move northwards until obstacle reached
 * @param position - start pos
 * @param board - obstacles
 * @return bitboard with moves to the north from position
 */
Bitboard Piece::allNorth ( std::size_t position, const Bitboard & board ) const
{
  Bitboard piecePos;

  piecePos.flip(position);

  // Piece already on rank 8
  if ( ((piecePos & rank8) != 0) )
    return Bitboard();

  piecePos = oneNorth(piecePos);
  while ( ((piecePos & board) == 0) && ((piecePos & rank8) == 0) ) {
    piecePos |= oneNorth(piecePos);
  }

  return piecePos;
}
/**
 * Sliding move southwards until obstacle reached
 * @param position - start pos
 * @param board - obstacles
 * @return bitboard with moves to the south from position
 */
Bitboard Piece::allSouth ( std::size_t position, const Bitboard & board ) const
{
  Bitboard piecePos;

  piecePos.flip(position);

  // Piece already on rank 1
  if ( ((piecePos & rank1) != 0) )
    return Bitboard();

  piecePos = oneSouth(piecePos);
  while ( ((piecePos & board) == 0) && ((piecePos & rank1) == 0) ) {
    piecePos |= oneSouth(piecePos);
  }

  return piecePos;
}
/**
 * Sliding move westwards until obstacle reached
 * @param position - start pos
 * @param board - obstacles
 * @return bitboard with moves to the west from position
 */
Bitboard Piece::allWest ( std::size_t position, const Bitboard & board ) const
{
  Bitboard piecePos;

  piecePos.flip(position);

  // Piece already on file A
  if ( ((piecePos & fileA) != 0) )
    return Bitboard();

  piecePos = oneWest(piecePos);
  while ( ((piecePos & board) == 0) && ((piecePos & fileA) == 0) ) {
    piecePos |= oneWest(piecePos);
  }

  return piecePos;
}
/**
 * Sliding move eastwards until obstacle reached
 * @param position - start pos
 * @param board - obstacles
 * @return bitboard with moves to the east from position
 */
Bitboard Piece::allEast ( std::size_t position, const Bitboard & board ) const
{
  Bitboard piecePos;

  piecePos.flip(position);

  // Piece already on file H
  if ( ((piecePos & fileH) != 0) )
    return Bitboard();

  piecePos = oneEast(piecePos);
  while ( ((piecePos & board) == 0) && ((piecePos & fileH) == 0) ) {
    piecePos |= oneEast(piecePos);
  }
  return piecePos;
}
/**
 * Sliding move northwest until obstacle reached
 * @param position - start pos
 * @param board - obstacles
 * @return bitboard with moves to the northwest from position
 */
Bitboard Piece::allNoWe ( std::size_t position, const Bitboard & board ) const
{
  Bitboard piecePos;

  piecePos.flip(position);

  // Piece already on fileA or rank8
  if ( ((piecePos & fileA) != 0) || ((piecePos & rank8) != 0) )
    return Bitboard();

  piecePos = oneNoWe(piecePos);
  while ( ((piecePos & board) == 0) && ((piecePos & fileA) == 0) && ((piecePos & rank8) == 0) ) {
    piecePos |= oneNoWe(piecePos);
  }

  return piecePos;
}
/**
 * Sliding move northeast until obstacle reached
 * @param position - start pos
 * @param board - obstacles
 * @return bitboard with moves to the northeast from position
 */
Bitboard Piece::allNoEa ( std::size_t position, const Bitboard & board ) const
{
  Bitboard piecePos;

  piecePos.flip(position);

  // Piece already on fileH or rank8
  if ( ((piecePos & fileH) != 0) || ((piecePos & rank8) != 0) )
    return Bitboard();

  piecePos = oneNoEa(piecePos);
  while ( ((piecePos & board) == 0) && ((piecePos & fileH) == 0) && ((piecePos & rank8) == 0) ) {
    piecePos |= oneNoEa(piecePos);
  }

  return piecePos;
}
/**
 * Sliding move southwest until obstacle reached
 * @param position - start pos
 * @param board - obstacles
 * @return bitboard with moves to the southwest from position
 */
Bitboard Piece::allSoWe ( std::size_t position, const Bitboard & board ) const
{
  Bitboard piecePos;

  piecePos.flip(position);

  // Piece already on fileA or rank1
  if ( ((piecePos & fileA) != 0) || ((piecePos & rank1) != 0) )
    return Bitboard();

  piecePos = oneSoWe(piecePos);
  while ( ((piecePos & board) == 0) && ((piecePos & fileA) == 0) && ((piecePos & rank1) == 0) ) {
    piecePos |= oneSoWe(piecePos);
  }

  return piecePos;
}
/**
 * Sliding move southeast until obstacle reached
 * @param position - start pos
 * @param board - obstacles
 * @return bitboard with moves to the southeast from position
 */
Bitboard Piece::allSoEa ( std::size_t position, const Bitboard & board ) const
{
  Bitboard piecePos;

  piecePos.flip(position);

  // Piece already on fileh or rank1
  if ( ((piecePos & fileH) != 0) || ((piecePos & rank1) != 0) )
    return Bitboard();

  piecePos = oneSoEa(piecePos);
  while ( ((piecePos & board) == 0) && ((piecePos & fileH) == 0) && ((piecePos & rank1) == 0) ) {
    piecePos |= oneSoEa(piecePos);
  }

  return piecePos;
}
/**
 * Shifts entire bitboard to the north
 * @param placement bitboard to shift
 * @return bitboard shifted to the north
 */
Bitboard Piece::oneNorth(const Bitboard & placement) const
{
  return placement << 8;
}
/**
 * Shifts entire bitboard to the south
 * @param placement bitboard to shift
 * @return bitboard shifted to the south
 */
Bitboard Piece::oneSouth(const Bitboard & placement) const
{
  return placement >> 8;
}
/**
 * Shifts entire bitboard to the west
 * @param placement bitboard to shift
 * @return bitboard shifted to the west
 */
Bitboard Piece::oneWest(const Bitboard & placement) const
{
  return (placement >> 1) & ~fileH;
}
/**
 * Shifts entire bitboard to the east
 * @param placement bitboard to shift
 * @return bitboard shifted to the east
 */
Bitboard Piece::oneEast(const Bitboard & placement) const
{
  return (placement << 1) & ~fileA;
}
/**
 * Shifts entire bitboard to the northeast
 * @param placement bitboard to shift
 * @return bitboard shifted to the northeast
 */
Bitboard Piece::oneNoEa(const Bitboard & placement) const
{
  return (placement << 9) & ~fileA;
}
/**
 * Shifts entire bitboard to the northwest
 * @param placement bitboard to shift
 * @return bitboard shifted to the northwest
 */
Bitboard Piece::oneNoWe(const Bitboard & placement) const
{
  return (placement << 7) & ~fileH;
}
/**
 * Shifts entire bitboard to the southeast
 * @param placement bitboard to shift
 * @return bitboard shifted to the southeast
 */
Bitboard Piece::oneSoEa(const Bitboard & placement) const
{
  return (placement >> 7) & ~fileA;
}
/**
 * Shifts entire bitboard to the southwest
 * @param placement bitboard to shift
 * @return bitboard shifted to the southwest
 */
Bitboard Piece::oneSoWe(const Bitboard & placement) const
{
  return (placement >> 9) & ~fileH;
}
/***************************Friend***************************/
/**
 * Print piece to ostream
 * @param os - ostream to print into
 * @param x - piece to print
 * @return ostream with printed piece
 */
std::ostream & operator << ( std::ostream & os, const Piece & x ) {
  x.print(os);
  return os;
}
