#include "Chessboard.h"
/***************************Public***************************/
/**
	Constructor for class Chessboard
	@params: fen - string in FEN format
*/
Chessboard::Chessboard ( const std::string & fen )
{
  // Alocated chess pieces
  for ( int i = EPiece::pawn; i < NUM_PIECES; ++i ) {
    switch ( i ) {
      case EPiece::pawn :
        whitePieces[i] = std::make_shared<Pawn> (EColor::white);
        blackPieces[i] = std::make_shared<Pawn>(EColor::black);
        break;
      case EPiece::knight :
        whitePieces[i] = std::make_shared<Knight>(EColor::white);
        blackPieces[i] = std::make_shared<Knight>(EColor::black);
        break;
      case EPiece::bishop :
        whitePieces[i] = std::make_shared<Bishop>(EColor::white);
        blackPieces[i] = std::make_shared<Bishop>(EColor::black);
        break;
      case EPiece::rook :
        whitePieces[i] = std::make_shared<Rook>(EColor::white);
        blackPieces[i] = std::make_shared<Rook>(EColor::black);
        break;
      case EPiece::queen :
        whitePieces[i] = std::make_shared<Queen>(EColor::white);
        blackPieces[i] = std::make_shared<Queen>(EColor::black);
        break;
      case EPiece::king :
        whitePieces[i] = std::make_shared<King>(EColor::white);
        blackPieces[i] = std::make_shared<King>(EColor::black);
        break;
    }
  }

  initializeBoard(fen);


  // Setup help bitboards
  reFillHelpBitboards();
  inCheck = kingInCheck(EColor(playerOnTurn));
  // Setup stacks
  flagsStack.push_back(flags);
  validMovesStack.push_back(searchValidMoves(playerOnTurn));
}
/**
	Destructor for class Chessboard
*/
Chessboard::~Chessboard ( void )
{

}
/**
 * Setup board according to given FEN, works only with correct FEN
 * @param fen - board position in FEN format
 */
void Chessboard::initializeBoard( const std::string & fen )
{
  std::istringstream iss (fen);
  Square pos;
  char input;
  size_t index;

  // Starting position
  pos.file = EFile::a;
  pos.rank = 8;

  iss >> std::noskipws;
  // Piece placement
  while ( (iss >> input) && !isspace(input) ) {
    if ( isdigit(input) ) {
      pos.file = EFile(pos.file + (input - '0'));
    } else if ( input == '/' ) { // New rank
      pos.rank = pos.rank - 1;
      pos.file = EFile::a;
    } else {
      if ( isupper(input) ) {   // White piece
        index = Chess_piece_string.find(input);
        whitePieces[index]->flipBit(offset(pos));
      } else {                  // Black piece
        index = Chess_piece_string.find(input) - 6;
        blackPieces[index]->flipBit(offset(pos));
      }
      pos.file = EFile(pos.file + 1); // Moving to next square (from left to right)
    }
  }

  // Player on turn
  iss >> input;
  playerOnTurn = ( input == 'w' ? EColor::white : EColor::black );
  iss >> input;

  // Default value
  flags.whiteCastling.OO = false;
  flags.whiteCastling.OOO = false;
  flags.blackCastling.OO = false;
  flags.blackCastling.OOO = false;

  // Castling
  while ( (iss >> input) && !isspace(input) ) {
    if ( input == '-' )
      continue;

    if ( isupper(input) ) {
      if ( input == 'K' )
        flags.whiteCastling.OO = true;
      else if ( input == 'Q' )
        flags.whiteCastling.OOO = true;
    } else {
      if ( input == 'k' )
        flags.blackCastling.OO = true;
      else if ( input == 'q' )
        flags.blackCastling.OOO = true;
    }
  }

  // En passant square
  if ( (iss >> input) && (input == '-') )
    flags.enPassantTargetSq.rank = 0;
  else {
    flags.enPassantTargetSq.file = EFile(input - 'a');
    iss >> input;
    flags.enPassantTargetSq.rank = input - '0';
  }
  iss >> input;

  // Half move clock
  iss >> halfMoveClock;
  iss >> input;

  // Full move clock
  iss >> fullMoveClock;
}
/**
 * Responsible for processing move (playing, editing flags, etc.)
 * @param move - move to play
 * @return
 *  1 - valid move
 *  3 - move not possible for this piece
 *  5 - king in check
 *  6 - checkmate
 *  7 - stalemate
 */
int Chessboard::processMove ( const ptrMove & move )
{
  ptrMove nextMove;

  // Looking for move in valid moves
  for ( auto i = getValidMoves().cbegin(); i != getValidMoves().cend(); ++i ) {
    if ( (**i) == (*move) ) {
      nextMove = std::make_shared<Move>(**i);
      nextMove -> promotePiece = move -> promotePiece;  // Getting promoted piece from user

      playMove(*nextMove);

      nextMove -> valid = true;
      break;
    }
  }

  // Move found
  if ( (nextMove) && (nextMove -> valid) ) {
    // Editing flags stack
    flagsStack.push_back(flags);
    if ( flagsStack.size() >= 10 ) {
      flagsStack.pop_front();
    }

    Flags nextMoveFlags = nextMove -> flags;

    // Set enPassant square
    flags.enPassantTargetSq = nextMoveFlags.enPassantTargetSq;

    // Edit castling flags if rook or king was moved
    if ( !nextMoveFlags.blackCastling.OOO )
      flags.blackCastling.OOO = false;
    if ( !nextMoveFlags.blackCastling.OO )
      flags.blackCastling.OO = false;
    if ( !nextMoveFlags.whiteCastling.OO )
      flags.whiteCastling.OO = false;
    if ( !nextMoveFlags.whiteCastling.OOO )
      flags.whiteCastling.OOO = false;

    // Capture was not made, pawn was not moved
    if ( (nextMove -> capture.rank == 0) && !(nextMove -> pawnMove) )
      halfMoveClock++;
    else
      halfMoveClock = 0;

    if ( playerOnTurn == EColor::black )
      fullMoveClock++;

    // Switching players
    playerOnTurn = EColor(!playerOnTurn);

    // Move giving check
    inCheck = kingInCheck(playerOnTurn);

    // Add valid moves to stack
    validMovesStack.push_back(searchValidMoves(playerOnTurn));
    // Clearing stack to prevent overflow
    if ( validMovesStack.size() >= 10 )
      validMovesStack.pop_front();

    if ( getValidMoves().empty() ) {
      if ( inCheck )
        return 6; // Checkmate
      else
        return 7; // Stalemate
    }

    // Fifty-move rule
    if ( halfMoveClock >= 50 )
      return 7;

    if ( inCheck )
      return 5;

    return 1;
  }
  return 3;
}
/**
 * Responsible for unprocessing move (unplaying, editing flags, etc.)
 * @param move - move to unplay
 * @return
 *  1 - valid move
 *  5 - king in check
 *  6 - checkmate
 *  7 - stalemate
 */
int Chessboard::unProcessMove ( const ptrMove & move )
{
  // Switching players
  playerOnTurn = EColor(!playerOnTurn);

  unPlayMove(*move);

  // Popping flags stack
  flags = flagsStack.back(); flagsStack.pop_back();

  // Edit counters
  // Capture was not made, pawn was not moved
  if ( (halfMoveClock != 0) && (move -> capture.rank == 0) && !(move -> pawnMove))
    halfMoveClock--;
  else
    halfMoveClock = 0;

  if ( playerOnTurn == EColor::black )
    fullMoveClock--;

  // Popping moves stack
  validMovesStack.pop_back();

  if ( getValidMoves().empty() ) {
    if ( inCheck )
      return 6; // Checkmate
    else
      return 7; // Stalemate
  }

  // Fifty-move rule
  if ( halfMoveClock >= 50 )
    return 7;

  if ( inCheck )
    return 5;

  return 1;
}
/**
 * Converts chessboard to FEN
 * @return chessboard in fen format
 */
const std::string Chessboard::getFen ( void ) const
{
  std::ostringstream fen;

  /*
   FEN FORMAT:
  <Piece Placement>
  <Side to move>
  <Castling ability>
  <En passant target square>
  <Halfmove clock>
  <Fullmove counter>
  */

  // Piece Placement
  for ( int i = boardSize; i > 0; --i ) {	// Rows
    int counter = 0;
    for ( int j = 0; j < boardSize; ++j ) {	// Columns
      ptrPiece figure = pieceAt(EFile(j), i);
      if ( figure ) {
        if ( counter > 0)
          fen << counter;
        counter = 0;
        fen << *figure;
      }
      else  // Empty squares
        counter++;
    }
    if ( counter > 0 )
      fen << counter;

    // Next row
    if ( i > 1)
      fen << "/";
  }
  fen << " ";

  // Player on turn
  fen << (playerOnTurn == EColor::white ? "w" : "b" );
  fen << " ";

  // Castling ability
  if ( !flags.whiteCastling.OO && !flags.whiteCastling.OOO && !flags.blackCastling.OO && !flags.blackCastling.OO )
    fen << "-";
  else {
    if ( flags.whiteCastling.OO )
      fen << "K";
    if ( flags.whiteCastling.OOO )
      fen << "Q";
    if ( flags.blackCastling.OO )
      fen << "k";
    if ( flags.blackCastling.OOO )
      fen << "q";
  }
  fen << " ";

  // En passant square
  if ( flags.enPassantTargetSq.rank == 0 )
    fen << "-";
  else {
    fen << char(flags.enPassantTargetSq.file + 'a');
    fen << flags.enPassantTargetSq.rank;
  }
  fen << " ";

  // Half move clock
  fen << halfMoveClock;
  fen << " ";

  // Full move clock
  fen << fullMoveClock;

  return fen.str();
}
/**
 * Evaluate board for alpha-beta algorithm.
 * Considers piece value, piece position and checks
 * @return value of board
 */
int Chessboard::evaluateBoard ( void ) const
{
  int value = 0;
  for ( int i = boardSize; i > 0; --i ) {	// Rows
    for ( int j = 0; j < boardSize; ++j ) {	// Columns
      ptrPiece figure = pieceAt(EFile(j), i);
      int posConst = 0.75;
      if ( (i >= 4) && (i <= 5) && (j >= 2) && (j <= 5) )
        posConst = 1.5;
      if ( figure )
        value += (figure -> getPieceValue()) * posConst;
    }
  }

  if ( inCheck ) {
    if ( getValidMoves().empty() )   // Checkmate
      return (playerOnTurn == EColor::white ? -1 : 1) * 1000000;
    else                                    // Check
      value += (10 * (playerOnTurn == EColor::white ? -1 : 1));
  }

  return value;
}
/***************************Private***************************/
/**
 * Finds all valid moves for given player
 * @param playerColor - player to find moves for
 * @return list of valid moves
 */
std::list<ptrMove> Chessboard::searchValidMoves ( EColor playerColor ) const
{
  std::list<ptrMove> allValidMoves;
  Chessboard copyBoard = *this;     // Making copy, to keep the method const
  for ( int j = (NUM_PIECES - 1); j >= 0; --j ) {
    // Pseudo legal moves
    std::list<ptrMove> pseudoLegalMoves = (
    playerColor == EColor::white ? whitePieces[j] : blackPieces[j]) -> generateMoves(
      emptySpace,
      (playerColor == EColor::white ? allBlack : allWhite),
      flags
    );

    // Validating pseudo legal moves
    for ( auto & move : pseudoLegalMoves ) {
      // Set captured piece
      if ( ((move -> capture).rank) != 0 )
        move -> capturedPiece = (copyBoard.pieceAt(move -> capture)) -> toEPiece();

      // Castling
      Castling castlingType;
      castlingType.OOO = move -> whiteDoesCastling.OOO || move -> blackDoesCastling.OOO;
      castlingType.OO = move -> whiteDoesCastling.OO || move -> blackDoesCastling.OO;

      // If castling move
      if ( castlingType.OO || castlingType.OOO ) {
        if ( !copyBoard.canCastle(playerOnTurn, castlingType) )
          continue;
      }

      copyBoard.playMove(*move);

      // Check if my king is in check => not valid move
      if ( copyBoard.kingInCheck(playerOnTurn) ) {
        copyBoard.unPlayMove(*move);
        continue;
      }

      copyBoard.unPlayMove(*move);  // Undo move

      allValidMoves.push_back(move);
    }
  }

  return allValidMoves;
}
/**
 * Play move, edit biatboards
 * @param move - move to play
 */
void Chessboard::playMove ( const Move & move )
{
  ptrPiece startPiece = pieceAt(move.from);
  ptrPiece endPiece = pieceAt(move.capture);
  std::size_t startPosOffset = offset(move.from);
  std::size_t endPosOffset = offset(move.to);

  // Editing piece bitboards
  startPiece -> flipBit(startPosOffset);
  startPiece -> flipBit(endPosOffset);

  // If capture was made
  if ( move.capture.rank != 0 )
    endPiece->flipBit(offset(move.capture));

  // Pawn promotion play
  if ( move.canPromote ) {
    (((startPiece -> getColor()) == EColor::white) ?
     whitePieces[move.promotePiece] : blackPieces[move.promotePiece]) -> flipBit(endPosOffset);
    startPiece -> flipBit(endPosOffset);
  }

  // Castling move
  if ( (move.blackDoesCastling.OO && ((startPiece -> getColor()) == EColor::black))
       || (move.whiteDoesCastling.OO && ((startPiece -> getColor()) == EColor::white)) ) {
    ptrPiece rook = (startPiece -> getColor()) == EColor::white ? whitePieces[EPiece::rook] : blackPieces[EPiece::rook];
    Bitboard placement = rook -> getPlacement();
    Bitboard rookPos = placement & fileH;     // Position of rook doing castling
    placement &= ~fileH;                      // Discarding fileH
    Bitboard newPlacement = (rookPos >> 2) | placement;
    rook -> setPlacement(newPlacement);

  } else if ( (move.blackDoesCastling.OOO && ((startPiece -> getColor()) == EColor::black))
              || (move.whiteDoesCastling.OOO && ((startPiece -> getColor()) == EColor::white)) ) {
    ptrPiece rook = (startPiece -> getColor()) == EColor::white ? whitePieces[EPiece::rook] : blackPieces[EPiece::rook];
    Bitboard placement = rook -> getPlacement();
    Bitboard rookPos = (placement & fileA);   // Position of rook doing castling
    placement &= ~fileA;                      // Discarding fileA
    Bitboard newPlacement = (rookPos << 3) | placement;
    rook -> setPlacement(newPlacement);
  }

  reFillHelpBitboards();
}
/**
 * Unplaying move, edit biatboards
 * @param move - move to unplay
 */
void Chessboard::unPlayMove ( const Move & move )
{
  ptrPiece startPiece = pieceAt(move.to);
  std::size_t startPosOffset = offset(move.from);
  std::size_t endPosOffset = offset(move.to);

  // Editing piece bitboards
  startPiece -> flipBit(startPosOffset);
  startPiece -> flipBit(endPosOffset);

  // If capture was made
  if ( move.capture.rank != 0 ) {
    ptrPiece capture = (playerOnTurn == EColor::white) ? blackPieces[move.capturedPiece] : whitePieces[move.capturedPiece];
    capture->flipBit(offset(move.capture));
  }

  // Pawn promotion play
  if ( move.canPromote ) {
    (((startPiece -> getColor()) == EColor::white) ?
     whitePieces[EPiece::pawn] : blackPieces[EPiece::pawn]) -> flipBit(startPosOffset);

    // Because startPiece is promoted piece not pawn
    startPiece -> flipBit(startPosOffset);
  }


  // Castling move
  if ( (move.blackDoesCastling.OO && ((startPiece -> getColor()) == EColor::black))
       || (move.whiteDoesCastling.OO && ((startPiece -> getColor()) == EColor::white)) ) {
    ptrPiece rook = (startPiece->getColor()) == EColor::white ? whitePieces[EPiece::rook] : blackPieces[EPiece::rook];
    Bitboard placement = rook->getPlacement();
    Bitboard rookPos = placement & fileF;
    placement &= ~fileF;
    Bitboard newPlacement = (rookPos << 2) | placement;
    rook->setPlacement(newPlacement);

  } else if ( (move.blackDoesCastling.OOO && ((startPiece -> getColor()) == EColor::black))
              || (move.whiteDoesCastling.OOO && ((startPiece -> getColor()) == EColor::white)) ) {
    ptrPiece rook = (startPiece -> getColor()) == EColor::white ? whitePieces[EPiece::rook] : blackPieces[EPiece::rook];
    Bitboard placement = rook -> getPlacement();
    Bitboard rookPos = placement & fileD;
    placement &= ~fileD;
    Bitboard newPlacement = (rookPos >> 3) | placement;
    rook -> setPlacement(newPlacement);
  }

  reFillHelpBitboards();
}
/**
	Get piece at position
	@params: col - column letter, row - row position
	@return: pointer to piece or nullptr if position empty
*/
ptrPiece Chessboard::pieceAt ( EFile col, int row ) const
{
  for ( int i = 0; i < NUM_PIECES; ++i ) {
    if ( !(whitePieces[i] -> empty(col, row)) )
      return whitePieces[i];
  }
  for ( int i = 0; i < NUM_PIECES; ++i ) {
    if ( !(blackPieces[i] -> empty(col, row)) )
      return blackPieces[i];
  }

  return nullptr;
}
/**
	Get piece at square
	@params: sq - square
	@return: pointer to piece or nullptr if square empty
*/
ptrPiece Chessboard::pieceAt ( const Square & sq ) const
{
  for ( int i = 0; i < NUM_PIECES; ++i ) {
    if ( !(whitePieces[i] -> empty(sq.file, sq.rank)) )
      return whitePieces[i];
  }
  for ( int i = 0; i < NUM_PIECES; ++i ) {
    if ( !(blackPieces[i] -> empty(sq.file, sq.rank)) )
      return blackPieces[i];
  }

  return nullptr;
}
/**
 * Re-fill all help bitboards
 */
void Chessboard::reFillHelpBitboards ( void )
{
  allWhite = 0;
  allBlack = 0;

  // Setup help bitboards
  for ( int i = 0; i < NUM_PIECES; ++i ) {
    allWhite |= whitePieces[i] -> getPlacement();
    allBlack |= blackPieces[i] -> getPlacement();
  }

  allPieces = allWhite | allBlack;
  emptySpace = ~allPieces;
}
/**
 * Creates bitboard with all attacks from given player
 * @param playerColor - attacking player
 * @param opponentPieces - opponent pieces
 * @return bitboard with attacked positions
 */
Bitboard Chessboard::playerAttacks ( EColor playerColor, const Bitboard & opponentPieces ) const
{
  Bitboard attacks;
  if ( playerColor == EColor::black ) {
    for ( size_t i = 0; i < NUM_PIECES; ++i )
      attacks |= blackPieces[i] -> attacksBitboard(allBlack, opponentPieces, flags);
  } else {
    for ( size_t i = 0; i < NUM_PIECES; ++i )
      attacks |= whitePieces[i] -> attacksBitboard(allWhite, opponentPieces, flags);
  }

  return attacks;
}
/**
 * Calculates if player's king is in check
 * @param playerColor - player to calculate
 * @return true - in check, false - not in check
 */
bool Chessboard::kingInCheck ( EColor playerColor ) const
{
  ptrPiece king = (playerColor == EColor::white ? whitePieces[EPiece::king] : blackPieces[EPiece::king]);
  Bitboard myPieces = (playerColor == EColor::white) ? allWhite : allBlack;
  Bitboard attacks = playerAttacks(EColor(!playerColor), myPieces); // Opponent attacks

  Bitboard intersection = (king -> getPlacement()) & attacks;

  if ( intersection == 0 )
    return false;

  return true;
}
/**
 * Calculates if castling squares are not in check
 * @param playerColor - player doing castling
 * @param castlingType - king / queen side
 * @return true - can castle, false - cannot castle
 */
bool Chessboard::canCastle ( EColor playerColor, Castling castlingType ) const
{
  Bitboard castlingSquares; // Squares over which king is moving during castling

  if ( castlingType.OO )  // King side
    castlingSquares = ((playerColor == EColor::white)? rank1 : rank8) & (fileE | fileF | fileH);
  else if ( castlingType.OOO) // Queen side
    castlingSquares = ((playerColor == EColor::white)? rank1 : rank8) & (fileC | fileD | fileE);

  // Opponent's attack on castlingSquares
  Bitboard opponentAttack = playerAttacks(EColor(!playerColor), castlingSquares | (playerColor == EColor::white ? allWhite : allBlack));

  // Opponent attacking castlingSquares
  if ( (opponentAttack & castlingSquares) != 0 )
    return false;

  return true;
}
/***************************Friend***************************/
/**
 * Print chessboard in classic format to ostream
 * @param  os - ostream to use
 * @param board - chessboard to print
 * @return ostream with printed chessboard
 */
std::ostream & operator << ( std::ostream & os, const Chessboard & board )
{
  os << "FEN: " << board.getFen() << std::endl;
  os << "  +------------------------+ " << std::endl;

  for ( int i = boardSize; i > 0; --i ) {	// Rows
    os << i << " |";
    for ( int j = 0; j < boardSize; ++j ) {	// Columns
       ptrPiece figure = board.pieceAt(EFile(j), i);
       os << std::setw(2) << std::setfill(' ');
       if ( figure )
         os <<  *figure << " ";
       else if ( ((i + j) % 2) == 0 )
         os << "   ";
       else
         os << " ■ ";
    }
    os << "|" << std::endl;
  }
  os << "  +------------------------+ " << std::endl;
  os << "    a  b  c  d  e  f  g  h " << std::endl;
  
	return os;
}