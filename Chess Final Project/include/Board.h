#ifndef BOARD_H
#define BOARD_H

#include "Bishop.h"
#include "King.h"
#include "Knight.h"
#include "Pawn.h"
#include "Piece.h"
#include "Queen.h"
#include "Rook.h"
#include "windows.h"
#include <iostream>
#include <mutex>
#include <vector>
#include <io.h>
#include <fcntl.h>

using namespace std;

class Board {
private:
    int width, height;
    vector<vector<Piece*>> virtualMap;
    vector<Piece*> whiteCaptured;
    vector<Piece*> blackCaptured;
    mutex mtx;
    bool firstRun;
    Cursor cursor;
    bool whiteTurn;
    bool pieceMoved;
    Piece* capturedPiece;

    //  to store the selected piece coordinates
    int selectedPieceX;
    int prevSelectedX;
    int selectedPieceY;
    int prevSelectedY;
    bool selected;

    void ShowConsoleCursor(bool showFlag) {
        //-------do not change-------
        // hides the cursor, use only once at program start
        HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(out, &cursorInfo);
        cursorInfo.bVisible = showFlag; // set the cursor visibility
        SetConsoleCursorInfo(out, &cursorInfo);
    }

    BOOL gotoxy(const WORD x, const WORD y) {
        //-------do not change-------
        COORD xy;
        xy.X = x;
        xy.Y = y;
        return SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), xy);
    }

public:

    Board();
    ~Board();

    void c_update(int time, bool& game_running);
    void cursor_move_down();
    void cursor_move_up();
    void cursor_move_left();
    void cursor_move_right();

    void setup();
    void draw();

    bool getSelected();
    void movePiece();

    bool isValidMove(int fromX, int fromY, int toX, int toY);
    bool isCheck(bool white);
    bool isCheckmate(bool white);
    void findKing(bool white, int& kingX, int& kingY);
    bool isThreatened(int x, int y, bool white);

    void selectPiece();
    void unselectPiece();

    wstring pieceTypeToString(PieceType type);

    bool playAgainPrompt();
    void resetGame();
    void clearBoard();
};

#endif
