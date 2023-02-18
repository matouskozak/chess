//
// Created by matous on 5/4/18.
//

#ifndef CHESS_HUMANPLAYER_H
#define CHESS_HUMANPLAYER_H

#include "Player.h"

/**
 * Class representing human player
 * Reads move from console
 */
class HumanPlayer : public Player
{
  public:
    HumanPlayer ( EColor color ) : Player(color) {};
    virtual ptrMove nextMove ( const Chessboard & board ) const;
  private:
    bool readMove ( ptrMove & move ) const;
};


#endif //CHESS_HUMANPLAYER_H
