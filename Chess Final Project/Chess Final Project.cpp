#include <iostream>
#include "windows.h"
#include "Board.h"
#include <iostream>
#include <mutex>
#include <random>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include <io.h>
#include <fcntl.h>

using namespace std;

int main() {

    _setmode(_fileno(stdout), _O_U16TEXT);

    bool game_running = true;
    int time = 70;
    Board myBoard;
    system("cls");
    do {
        myBoard.resetGame();
        thread thread_obj(&Board::c_update, &myBoard, time, ref(game_running)); // thread for cursor update
        game_running = true;
        do {
            myBoard.draw();
            system("pause>nul");

            if (GetAsyncKeyState(VK_DOWN)) {
                myBoard.cursor_move_down();
            }
            else if (GetAsyncKeyState(VK_UP)) {
                myBoard.cursor_move_up();

            }
            else if (GetAsyncKeyState(VK_RIGHT)) {
                myBoard.cursor_move_right();

            }
            else if (GetAsyncKeyState(VK_LEFT)) {
                myBoard.cursor_move_left();
            }
            else if (GetAsyncKeyState(VK_SPACE)) {
                if (!myBoard.getSelected()) {
                    myBoard.selectPiece();
                }
                else {
                    myBoard.movePiece();
                }
            }
            else if (GetAsyncKeyState(VK_TAB)) {
                myBoard.unselectPiece();
            }
            else if (GetAsyncKeyState(VK_ESCAPE)) {
                game_running = false;
            }
        } while (game_running);
        thread_obj.detach();
        system("cls");
    } while (myBoard.playAgainPrompt());
    system("cls");

    return 0;
}
