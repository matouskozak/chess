//
// Created by matous on 5/4/18.
//

#ifndef CHESS_PLAYER_H
#define CHESS_PLAYER_H

#include "const_chess_define.h"
#include "Chessboard.h"
#include "Move.h"

/**
 * Abstract class representing players
 */
class Player
{
  public:
    Player ( EColor color ) : color(color) {};
    virtual ~Player ( void ) = default;
    inline EColor getColor ( void ) { return color; };
    virtual ptrMove nextMove ( const Chessboard & board ) const = 0;
  protected:
    EColor color;
};


#endif //CHESS_PLAYER_H
