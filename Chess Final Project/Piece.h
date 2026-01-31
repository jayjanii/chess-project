#ifndef PIECE_H
#define PIECE_H

#include "Cursor.h"
#include <iostream>
#include <vector>
#include <io.h>
#include <fcntl.h>

using namespace std;

enum class PieceType { Pawn, Rook, Knight, Bishop, Queen, King };

class Piece {
private:
	bool isCaptured;
	string pieceName;
	bool isWhite;
	bool hasMoved;
	int x, y;

public:
	Piece(bool white = true) : isCaptured(false), isWhite(white) {}

	virtual wchar_t getSymbol() = 0;
	virtual PieceType getType() const = 0;

	bool getCaptured() const { return isCaptured; }
	void setCaptured(bool captured) { isCaptured = captured; }

	bool getWhite() const { return isWhite; }
	void setWhite(bool white) { isWhite = white; }

	bool getHasMoved() const { return hasMoved; }
	void setHasMoved(bool moved) { hasMoved = moved; }

	void setX(int x) { this->x = x; }
	void setY(int y) { this->y = y; }

	int getX() { return x; }
	int getY() { return y; }

};

#endif