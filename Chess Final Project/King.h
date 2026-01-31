#ifndef KING_H
#define KING_H

#include "Piece.h"
#include <io.h>
#include <fcntl.h>

using namespace std;

class King : public Piece {
private:

public:
	King(bool white){
		setWhite(white);
	}

	PieceType getType() const override {
		return PieceType::King;
	}

	wchar_t getSymbol() { return getWhite() ? L'\u265A' : L'\u2654'; }
};
#endif