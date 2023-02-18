CXX=g++
LD=g++
CXXFLAGS=-std=c++14 -Wall -pedantic -Wno-long-long -O0 -ggdb
DOXYGEN=doxygen
PROGRAM=kozakmat


all: compile doc


compile: src/Bishop.o src/Chessboard.o src/ComputerPlayer.o src/Game.o src/HumanPlayer.o src/King.o src/Knight.o src/main.o src/Move.o src/Pawn.o src/Piece.o src/Player.o src/Queen.o src/Rook.o
	$(LD) -o $(PROGRAM) $^

run:
	./$(PROGRAM)

clean:
	rm -f $(PROGRAM) src/Bishop.o src/Chessboard.o src/ComputerPlayer.o src/Game.o src/HumanPlayer.o src/King.o src/Knight.o src/main.o src/Move.o src/Pawn.o src/Piece.o src/Player.o src/Queen.o src/Rook.o 
	rm -fr doc

doc:
	$(DOXYGEN) src/Doxyfile

%o: %cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

src/Bishop.o: src/Bishop.cpp src/Bishop.h src/Piece.h src/Move.h \
 src/const_chess_define.h
 
src/Chessboard.o: src/Chessboard.cpp src/Chessboard.h src/Move.h \
 src/const_chess_define.h src/Pawn.h src/Piece.h src/Knight.h \
 src/Bishop.h src/Rook.h src/Queen.h src/King.h
 
src/ComputerPlayer.o: src/ComputerPlayer.cpp src/ComputerPlayer.h \
 src/Player.h src/const_chess_define.h src/Chessboard.h src/Move.h \
 src/Pawn.h src/Piece.h src/Knight.h src/Bishop.h src/Rook.h src/Queen.h \
 src/King.h
 
src/Game.o: src/Game.cpp src/Game.h src/HumanPlayer.h src/Player.h \
 src/const_chess_define.h src/Chessboard.h src/Move.h src/Pawn.h \
 src/Piece.h src/Knight.h src/Bishop.h src/Rook.h src/Queen.h src/King.h \
 src/ComputerPlayer.h
 
src/HumanPlayer.o: src/HumanPlayer.cpp src/HumanPlayer.h src/Player.h \
 src/const_chess_define.h src/Chessboard.h src/Move.h src/Pawn.h \
 src/Piece.h src/Knight.h src/Bishop.h src/Rook.h src/Queen.h src/King.h
 
src/King.o: src/King.cpp src/King.h src/Piece.h src/Move.h \
 src/const_chess_define.h
 
src/Knight.o: src/Knight.cpp src/Knight.h src/Piece.h src/Move.h \
 src/const_chess_define.h
 
src/main.o: src/main.cpp src/Game.h src/HumanPlayer.h src/Player.h \
 src/const_chess_define.h src/Chessboard.h src/Move.h src/Pawn.h \
 src/Piece.h src/Knight.h src/Bishop.h src/Rook.h src/Queen.h src/King.h \
 src/ComputerPlayer.h
 
src/Move.o: src/Move.cpp src/Move.h src/const_chess_define.h

src/Pawn.o: src/Pawn.cpp src/Pawn.h src/Piece.h src/Move.h \
 src/const_chess_define.h
 
src/Piece.o: src/Piece.cpp src/Piece.h src/Move.h src/const_chess_define.h

src/Player.o: src/Player.cpp src/Player.h src/const_chess_define.h \
 src/Chessboard.h src/Move.h src/Pawn.h src/Piece.h src/Knight.h \
 src/Bishop.h src/Rook.h src/Queen.h src/King.h
 
src/Queen.o: src/Queen.cpp src/Queen.h src/Piece.h src/Move.h \
 src/const_chess_define.h
 
src/Rook.o: src/Rook.cpp src/Rook.h src/Piece.h src/Move.h \
 src/const_chess_define.h





