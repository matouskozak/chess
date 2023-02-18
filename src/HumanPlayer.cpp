//
// Created by matous on 5/4/18.
//
#include "HumanPlayer.h"

/***************************Public***************************/
/**
 * Get move from user
 * @param board - chessboard ref (not needed here)
 * @return pointer to entered move
 */
ptrMove HumanPlayer::nextMove ( const Chessboard & board ) const
{
  ptrMove inputMove;
  // Player color
  if ( color == EColor::white )
    std::cout << "White on turn" << std::endl;
  else
    std::cout << "Black on turn" << std::endl;

  std::cout << "Press 'S' to save the game" << std::endl; // Option to save the game

  while ( !readMove(inputMove) ) {
    std::cin.clear();
    std::cin.ignore(INT_MAX, '\n');
    std::cout << "Invalid move format. Valid format ({a..h}{1..8}/[= Q | N | R | B])" << std::endl;
  }

  return inputMove;
}
/***************************Private***************************/
/**
	Read and validate move format from input
	@params: move - move to fill
	@return: true - read was succesful, false - read failed
*/
bool HumanPlayer::readMove ( ptrMove & move ) const
{
  Square startSq;
  Square endSq;
  char from [2];
  char to [4];
  char promotion [2];

  std::cout << "From ";
  std::cin >> from;

  // Save game selected
  if ( from[0] == 'S' ) {
    move = nullptr;
    return true;
  }

  // Validate position
  if ( 	(from[0] < 'a') || (from[0] > 'h') ||
        (from[1] < '1') || (from[1] > '8') )
    return false;

  startSq.file = EFile(from[0] - 'a');
  startSq.rank = from[1] - '0';

  std::cout << "To ";
  std::cin >> to;

  // Validate position
  if ( 	(to[0] < 'a') || (to[0] > 'h') ||
        (to[1] < '1') || (to[1] > '8') )
    return false;

  endSq.file = EFile(to[0] - 'a');
  endSq.rank = to[1] - '0';

  move = std::make_shared<Move>(startSq, endSq);

  promotion[0] = to[2];
  promotion[1] = to[3];

  // Promotion move
  if ( (promotion[0] == '=') &&  (  (promotion[1] == 'Q')
                                 || (promotion[1] == 'N')
                                 || (promotion[1] == 'R')
                                 || (promotion[1] == 'B')) ) {
    move -> promotePiece = EPiece(Chess_piece_string.find(promotion[1]));
  }

  return true;
}