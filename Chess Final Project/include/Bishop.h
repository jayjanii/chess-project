#ifndef BISHOP_H
#define BISHOP_H

#include "Piece.h"
#include <vector>

using namespace std;

class Bishop : public Piece {
private:
public:
    Bishop(bool white) { setWhite(white); }

    PieceType getType() const override {
        return PieceType::Bishop;
    }

    wchar_t getSymbol() { return getWhite() ?  L'\u265D' : L'\u2657'; }
};

#endif