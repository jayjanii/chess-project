#ifndef PAWN_H
#define PAWN_H

#include "Piece.h"
#include <vector>

using namespace std;

class Pawn : public Piece {

public:
	Pawn(bool white) {
		setWhite(white);
	}

	PieceType getType() const override {
		return PieceType::Pawn;
	}

	wchar_t getSymbol() { return getWhite() ? L'\u265F' : L'\u2659'; }

};

#endif