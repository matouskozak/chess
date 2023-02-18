//
// Created by matous on 5/11/18.
//

#ifndef CHESS_COMPUTERPLAYER_H
#define CHESS_COMPUTERPLAYER_H

#include "Player.h"

/**
 * Class representing computer playing chess
 * Using Alpha-beta pruning for move decision
 */
class ComputerPlayer : public Player
{
  public:
    ComputerPlayer ( EColor color ) : Player(color) {};
    virtual ptrMove nextMove ( const Chessboard & board ) const;


  private:
    int minimax ( Chessboard & board, bool isMaxPlayer, int depth ) const;
    int alphaBeta ( Chessboard & board, bool isMaxPlayer, int alpha, int beta, int depth ) const;
};


#endif //CHESS_COMPUTERPLAYER_H
