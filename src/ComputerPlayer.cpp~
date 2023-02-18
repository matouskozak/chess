//
// Created by matous on 5/11/18.
//

#include "ComputerPlayer.h"
/***************************Public***************************/
/**
 * Calculates next best possible move
 * @param board - current board state
 * @return pointer to found move
 */
ptrMove ComputerPlayer::nextMove ( const Chessboard & board ) const
{
  Chessboard copyBoard = board;
  std::list<ptrMove> validMoves = copyBoard.getValidMoves();
  ptrMove bestMove;
  int bestValue = INT_MAX * ((color == EColor::white) ? -1 : 1);

  // Searching for best move
  for ( auto i = validMoves.cbegin(); i != validMoves.cend(); ++i ) {
    copyBoard.processMove(*i);

    int moveValue = alphaBeta(copyBoard, (copyBoard.getPlayer() == EColor::white) ? true : false, -INT_MAX, INT_MAX, 2);

    copyBoard.unProcessMove(*i);

    // New best move
    if ( ((color == EColor::white) && (moveValue > bestValue)) ||
         ((color == EColor::black) && (moveValue < bestValue))  ) {
      bestValue = moveValue;
      bestMove = *i;
    }
  }

  return bestMove;
}
/***************************Private***************************/
/**
 * Alpha-beta pruning algorithm for deciding best move
 * @param board - current chessboard state
 * @param isMaxPlayer - maximazing/minimazing player on turn
 * @param alpha - worst possible value for max. player
 * @param beta  - worst possible value for min. player
 * @param depth - current depth of game tree
 * @return - best value in game tree
 */
int ComputerPlayer::alphaBeta ( Chessboard & board, bool isMaxPlayer, int alpha, int beta, int depth ) const
{
  if ( depth == 0 ) // Max depth reached
    return board.evaluateBoard();

  std::list<ptrMove> validMoves = board.getValidMoves();
  if ( isMaxPlayer ) {
    int bestValue = -INT_MAX;
    for ( auto i = validMoves.cbegin(); i != validMoves.cend(); ++i ) {
      board.processMove(*i);
      bestValue = std::max(bestValue, alphaBeta(board, false, alpha, beta, depth - 1));
      board.unProcessMove(*i);

      alpha = std::max(alpha, bestValue); // Min value for maximazing player (worst case)

      // Beta cut-off
      if ( beta <= alpha )
        break;
    }
    return bestValue;
  } else {
    int bestValue = INT_MAX;
    for ( auto i = validMoves.cbegin(); i != validMoves.cend(); ++i ) {
      board.processMove(*i);
      bestValue = std::min(bestValue, alphaBeta(board, true, alpha, beta, depth - 1));
      board.unProcessMove(*i);

      beta = std::min(beta, bestValue); // Max value for minimazing player (worst case)

      // Alpha cut-off
      if ( beta <= alpha )
        break;
    }
    return bestValue;
  }
}
/**
 * NOT USED
 * Minimax algorithm for deciding best move
 * @param board - current chessboard state
 * @param isMaxPlayer - maximazing/minimazing player on turn
 * @param depth - current depth of game tree
 * @return
 */
int ComputerPlayer::minimax ( Chessboard & board, bool isMaxPlayer, int depth ) const
{
  if (depth == 0) // Max depth reached
    return board.evaluateBoard();

  std::list<ptrMove> validMoves = board.getValidMoves();
  if ( isMaxPlayer ) {
    int bestValue = -INT_MAX;
    for ( auto i = validMoves.begin(); i != validMoves.end(); ++i ) {
      board.processMove(*i);
      bestValue = std::max(bestValue, minimax(board, !isMaxPlayer, depth - 1));
      board.unProcessMove(*i);
    }
    return bestValue;
  } else {
    int bestValue = INT_MAX;
    for ( auto i = validMoves.begin(); i != validMoves.end(); ++i ) {
      board.processMove(*i);
      bestValue = std::min(bestValue, minimax(board, !isMaxPlayer, depth - 1));
      board.unProcessMove(*i);
    }
    return bestValue;
  }
}
