#ifndef ROOK_H
#define ROOK_H

#include "Piece.h"
#include <io.h>
#include <fcntl.h>

using namespace std;

class Rook : public Piece {
private:
public:
	Rook(bool white) {
		setWhite(white);
	}

	PieceType getType() const override {
		return PieceType::Rook;
	}

	wchar_t getSymbol() { return getWhite() ? L'\u265C' : L'\u2656'; }
};

#endif
