#ifndef QUEEN_H
#define QUEEN_H

#include "Piece.h"
#include <io.h>
#include <fcntl.h>

using namespace std;

class Queen : public Piece {
private:
public:
	Queen(bool white){
		setWhite(white);
	}

	PieceType getType() const override {
		return PieceType::Queen;
	}

	wchar_t getSymbol() { return getWhite() ? L'\u265B' : L'\u2655'; }

};

#endif
