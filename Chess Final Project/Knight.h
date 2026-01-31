#ifndef KNIGHT_H
#define KNIGHT_H

#include "Piece.h"
#include <io.h>
#include <fcntl.h>

using namespace std;

class Knight : public Piece {
private:
public:
	Knight(bool white){
		setWhite(white);
	}

	PieceType getType() const override { 
		return PieceType::Knight; 
	}

	wchar_t getSymbol() { return getWhite() ? L'\u265E' : L'\u2658'; }
};

#endif