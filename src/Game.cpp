#include "Game.h"
/***************************Public***************************/
/**
	Constructor for class Game
*/
Game::Game ( void )
{

}
/**
	Destructor for class Game
*/
Game::~Game ( void )
{

}
/**
 * Saves game to text file in directory examples
 * Format: 1st line: FEN, 2nd line: [1/0] [1/0]
 * @param gameName - string with name of game
 */
void Game::saveGame ( const std::string & gameName ) const
{
	std::ofstream ofs ("./examples/" + gameName, std::ofstream::out);

	ofs << chessboard.getFen() << std::endl;
	ofs << humanPlayer1 << " " << humanPlayer2;

	ofs.close();

	std::cout << "Game saved" << std::endl;
}
/**
 * Loads game from text file from directory examples
 * Format: 1st line: FEN, 2nd line: [1/0] [1/0]
 * @param gameName - string with name of game
 */
bool Game::loadGame ( const std::string & gameName )
{
	// Starting default game
	if ( gameName == "default" ) {
		chessboard = Chessboard();
		startGame(true, true);
	}

	std::ifstream ifs ("./examples/" + gameName, std::ifstream::in);

  // File cannot be openned
	if ( !ifs.is_open() )
		return false;

	// 1st line fen
	char fen [256];
  ifs.getline(fen, 256);
	chessboard = Chessboard(fen); // Accepts only correct fen

	// 2nd line player setup
	int humanPlayer1;	// 1 - human, 0 - AI
	int humanPlayer2;

	if ( !(ifs >> humanPlayer1) || ((humanPlayer1 != 0) && (humanPlayer1 != 1)) )
    return false;

	if ( !(ifs >> humanPlayer2) || ((humanPlayer2 != 0) && (humanPlayer2 != 1)) )
		return false;

  ifs.close();

	this -> humanPlayer1 = humanPlayer1;
	this -> humanPlayer2 = humanPlayer2;

	if ( humanPlayer1 )
		player1 = new HumanPlayer(EColor::white);
	else
		player1  = new ComputerPlayer(EColor::white);
	if ( humanPlayer2 )
		player2  = new HumanPlayer(EColor::black);
	else
		player2  = new ComputerPlayer(EColor::black);

	std::cout << "Game loaded" << std::endl;

	playGame();

	return true;
}
/**
 * Sets up players for game
 * @param humanPlayer1 - true if white is human
 * @param humanPlayer2 - true if black is human
 */
void Game::startGame ( bool humanPlayer1, bool humanPlayer2 )
{
  srand(time(NULL));
  // 0 => 1st player is white, 1 => 2nd player is white
  int whitePlayer = rand() % 2;	// Random starting player

  // Setting up players
  if ( humanPlayer1 )
    (whitePlayer == 0 ? player1 : player2) = new HumanPlayer(EColor(0 + whitePlayer));
  else
    (whitePlayer == 0 ? player1 : player2)  = new ComputerPlayer(EColor(0 + whitePlayer));
  if ( humanPlayer2 )
    (whitePlayer == 0 ? player2 : player1)  = new HumanPlayer(EColor(1 - whitePlayer));
  else
    (whitePlayer == 0 ? player2 : player1)  = new ComputerPlayer(EColor(1 - whitePlayer));

  this -> humanPlayer1 = (whitePlayer == 0 ? humanPlayer1 : humanPlayer2);
  this -> humanPlayer2 = (whitePlayer == 0 ? humanPlayer2 : humanPlayer1);;

  chessboard = Chessboard();	// Inicialize chessboard

  playGame();
}
/***************************Private***************************/
/**
	Game cycle
	@params:
	@return:
*/
void Game::playGame ( void )
{
	ptrMove nextMove;
	int retValue;
	std::string gameName;
	Player * playerOnTurn;
	bool gameOver = false;

  system("clear");
  std::cout << chessboard << std::endl;
  if ( chessboard.playerInCheck() )
		std::cout << "Check" << std::endl;

	while ( !gameOver ) {
		playerOnTurn = (chessboard.getPlayer() == EColor::white) ? player1 : player2;

		nextMove = playerOnTurn->nextMove(chessboard);	// Getting next move from player

		// Save game
		if ( !nextMove ) {
			std::cout << "Name of game" << std::endl;
			std::cin >> gameName;

			saveGame(gameName);
			break;
		}

		while ( (retValue = chessboard.processMove(nextMove)) == 3 ) {
      system("clear");
			std::cout << chessboard << std::endl;
			std::cout << "Please choose valid move" << std::endl;
      nextMove = playerOnTurn->nextMove(chessboard);
		}

    system("clear");

		// Board look after played move
    std::cout << *nextMove << std::endl;
		std::cout << chessboard << std::endl;

		// Ret value after processing
		switch ( retValue ) {
			case 5 :
				std::cout << "Check" << std::endl;
				break;
			case 6 :
				std::cout << "Checkmate" << std::endl;
				gameOver = true;
				break;
			case 7 :
				std::cout << "Stalemate" << std::endl;
				gameOver = true;
		}
	}

	// End of game
	delete player1;
	delete player2;
}