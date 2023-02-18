//
// Created by matous on 4/30/18.
//

#ifndef CHESS_GAME_H
#define CHESS_GAME_H

#include "Chessboard.h"
#include "HumanPlayer.h"
#include "ComputerPlayer.h"

#include <iostream>
#include <climits>
#include <fstream>

/**
 * Game class responsible for game managment
 */
class Game
{
  public:
    Game ( void );
    ~Game ( void );
    void saveGame ( const std::string & gameName ) const;
    bool loadGame ( const std::string & gameName );
    void startGame ( bool humanPlayer1, bool humanPlayer2 );
  private:
    Chessboard chessboard;
    Player * player1; //!< White player
    Player * player2; //!< Black Player

    //! For restarting games
    bool humanPlayer1;  //!< Is white player human?
    bool humanPlayer2;  //!< Is black player human?

    void playGame ( void );
};

#endif //CHESS_GAME_H