#include "Game.h"
#include <iostream>

/*! \mainpage Chess simulator made by Matous Kozak
 *  Chess simulator for casual players\n
 *
 * <pre>
 *  %Game modes:
  *	         %Player vs %Player
  *             %Player vs Computer
  *             Computer vs Computer
 *  </pre>
 *  Made as semester work for subject PA2. \n
 */

// TODO Game states as enum

/**
 * Main game cycle
 * @return 0
 */
int main ( void )
{
  int option;
  bool exit = false;
  std::string input;
  std::cout << "Chess program developed by MK\n";

  Game newGame;
  while ( !exit ) {
    std::cout << "(0) - New game: Player vs Player\n";
    std::cout << "(1) - New game: Player vs AI\n";
    std::cout << "(2) - New game: AI vs AI\n";
    std::cout << "(3) - Load game\n";
    std::cout << "(4) - Exit\n";
    std::cout << "\n Choose option 0-4 \n";
    while ( !(std::cin >> option) ) {
      std::cin.clear();
      std::cin.ignore(INT_MAX, '\n');
      std::cout << "\nNot a valid option\n";
    }
    switch ( option ) {
      case 0 :
        newGame.startGame(true, true);   // PvP
        break;
      case 1 :
        newGame.startGame(true, false);  // PvE
        break;
      case 2 :
        newGame.startGame(false, false); // EvE
        break;
      case 3 :
        std::cout << "Game name: " << std::endl;
        while ( !(std::cin >> input) || !newGame.loadGame(input) ) {
          std::cin.clear();
          std::cin.ignore(INT_MAX, '\n');
          std::cout << "Game cannot be loaded." << std::endl;
          input.clear();
        }
        break;
      case 4 :
        exit = true;
        break;
      default:
        std::cout << "\n" << option << " - Not a valid option\n";
    }
  }

  return 0;

}
