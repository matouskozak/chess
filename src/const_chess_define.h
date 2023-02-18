

#ifndef CHESS_CONST_CHESS_DEFINE_H
#define CHESS_CONST_CHESS_DEFINE_H

#include <bitset>
#include <iostream>
#include <iomanip>
#include <climits>


const int boardSize = 8;
typedef std::bitset< boardSize * boardSize > Bitboard ;

enum EPiece { pawn, knight, bishop, rook, queen, king, NUM_PIECES = 6 };
const std::string Chess_piece_string = "PNBRQKpnbrqk";

enum EFile { a, b, c, d, e, f, g, h };

/**
 * Representation of chessboard square
 */
struct Square
{
  EFile file = a;
  int rank = 0; // Indicating that square was not set

  /**
   * Operator == for Squares
   * @param other - other square
   * @return true - squares are equal
   */
  bool operator == ( const Square & other ) const {
    return ( (other.file == file) && (other.rank == rank) );
  };
};

/**
 * Castling type
 * OOO - Queen-side/long castling
 * OO  - King-side/short castling
 */
struct Castling
{
  bool OOO = true;
  bool OO = true;
};

/**
 * Important flags
 */
struct Flags
{
  Square enPassantTargetSq; // Square that can be attackt by En Passant move
  Castling whiteCastling;
  Castling blackCastling;
};

enum EColor {white, black};

// Ranks and files on chessboard
static const Bitboard fileA = 0x0101010101010101ULL;
static const Bitboard fileB = fileA << 1;
static const Bitboard fileC = fileA << 2;
static const Bitboard fileD = fileA << 3;
static const Bitboard fileE = fileA << 4;
static const Bitboard fileF = fileA << 5;
static const Bitboard fileG = fileA << 6;
static const Bitboard fileH = fileA << 7;

static const Bitboard rank1 = 0xFF;
static const Bitboard rank2 = rank1 << (8 * 1);
static const Bitboard rank3 = rank1 << (8 * 2);
static const Bitboard rank4 = rank1 << (8 * 3);
static const Bitboard rank5 = rank1 << (8 * 4);
static const Bitboard rank6 = rank1 << (8 * 5);
static const Bitboard rank7 = rank1 << (8 * 6);
static const Bitboard rank8 = rank1 << (8 * 7);


inline static Square toSquare( size_t offset )
{
  Square sq;
  sq.rank = (offset / boardSize) + 1;
  sq.file = EFile (offset % boardSize);
  return sq;
}

/**
	Do: Calculate offset from standard chess position
	@params: file - file letter, rank - rank number
	@return: offset
*/
inline static size_t offset ( EFile file, int rank )
{
  return ((rank - 1) * boardSize) + file;
}
/**
	Do: Calculate offset from square on chessboard
	@params: sq - square on chessboard
	@return: offset
*/
inline static size_t offset ( const Square & sq )
{
  return ((sq.rank - 1) * boardSize) + sq.file;
}


#endif //CHESS_CONST_CHESS_DEFINE_H
