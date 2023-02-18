//
// Created by matous on 5/1/18.
//

#include "Move.h"
/***************************Public***************************/
/**
 * Constructor for class Move
 * @param fromSq - from square
 * @param toSq - to square
 */
Move::Move ( const Square & fromSq, const Square & toSq ) : from(fromSq), to(toSq)
{
  whiteDoesCastling.OOO = false;
  whiteDoesCastling.OO = false;
  blackDoesCastling.OOO = false;
  blackDoesCastling.OO = false;
}
/**
 * Constructor for class Move
 * @param from - from abs. position
 * @param to - to abs. position
 */
Move::Move ( size_t from, size_t to ) : from(toSquare(from)), to(toSquare(to))
{
  whiteDoesCastling.OOO = false;
  whiteDoesCastling.OO = false;
  blackDoesCastling.OOO = false;
  blackDoesCastling.OO = false;
}

/**
 * Setting castling queen flag
 * @param value
 * @param color
 */
void Move::setCastlingOOOFlag(bool value, EColor color)
{
  if ( color == EColor::white )
    flags.whiteCastling.OOO = value;
  else
    flags.blackCastling.OOO = value;
}
/**
 * Setting castling king flag
 * @param value
 * @param color
 */
void Move::setCastlingOOFlag(bool value, EColor color)
{
  if ( color == EColor::white )
    flags.whiteCastling.OO = value;
  else
    flags.blackCastling.OO = value;
}
/**
 * Setting move castling queen flag
 * @param value
 * @param color
 */
void Move::setMoveCastlingOOO ( bool value, EColor color )
{
  if ( color == EColor::white )
    whiteDoesCastling.OOO = value;
  else
    blackDoesCastling.OOO = value;

}
/**
 * Setting move castling king flag
 * @param value
 * @param color
 */
void Move::setMoveCastlingOO ( bool value, EColor color )
{
  if ( color == EColor::white )
    whiteDoesCastling.OO = value;
  else
    blackDoesCastling.OO = value;
}
/**
 * Operator == for class Move
 * @param other - other move
 * @return true - moves start and end position are equal
 */
bool Move::operator == ( const Move & other ) const
{
  return ((other.from == from) && (other.to == to));
}
/***************************Friend***************************/
/**
 * Print move in standart format to ostream
 * @param os - ostream to use
 * @param move - move to print
 * @return ostream with printed move
 */
std::ostream & operator << ( std::ostream & os, const Move & move )
{
  os  << "Move: "
      << (char)(move.from.file + 'a') << move.from.rank
      << " -> "
      << (char)(move.to.file +   'a') << move.to.rank;
  return os;
}
