#include "Board.h"

Board::Board() : width(8), height(8), firstRun(true), whiteTurn(true), selectedPieceX(100), selectedPieceY(100), pieceMoved(false), capturedPiece(nullptr) {
    virtualMap.resize(height, vector<Piece*>(width, nullptr));
    ShowConsoleCursor(false);
    setup();
}

Board::~Board() {
    clearBoard();
}

void Board::draw() {
    const wchar_t WHITE = L'\u2588'; // Unicode full block for white tile
    const wchar_t BLACK = ' ';       // Space character for black tile
    const int tileWidth = 6;
    const int tileHeight = 3;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    mtx.lock();

    if (firstRun) {

        // Draw the column labels
        for (int x = 0; x < width; ++x) {
            gotoxy(x * tileWidth + tileWidth / 2, 0);
            wcout << wchar_t('A' + x);
            gotoxy(x * tileWidth + tileWidth / 2, height * tileHeight + 1);
            wcout << wchar_t('A' + x);
        }

        // Draw the row labels
        for (int y = 0; y < height; ++y) {
            gotoxy(0, y * tileHeight + tileHeight / 2);
            wcout << wchar_t('8' - y);
            gotoxy(width * tileWidth + 1, y * tileHeight + tileHeight / 2);
            wcout << wchar_t('8' - y);
        }


        // Draw the board background for the first run
        for (int y = 0; y < height; ++y) {
            for (int i = 0; i < tileHeight; ++i) {
                for (int x = 0; x < width; ++x) {
                    for (int j = 0; j < tileWidth; ++j) {
                        gotoxy(x * tileWidth + j, y * tileHeight + i);
                        if ((x + y) % 2 == 0) {
                            wcout << WHITE;
                        }
                        else {
                            wcout << BLACK;
                        }
                    }
                }
            }
        }
    }

    // Clear the previous cursor position
    int prevCursorX = cursor.get_prev_x();
    int prevCursorY = cursor.get_prev_y();
    if (prevCursorX >= 0 && prevCursorY >= 0 && prevCursorX < width && prevCursorY < height) {
        for (int i = 0; i < tileHeight; ++i) {
            for (int j = 0; j < tileWidth; ++j) {
                gotoxy(prevCursorX * tileWidth + j, prevCursorY * tileHeight + i);
                if ((prevCursorX + prevCursorY) % 2 == 0) {
                    wcout << WHITE;
                }
                else {
                    wcout << BLACK;
                }
            }
        }
    }

    // Clear the previous position of the moved piece
    if (prevSelectedX >= 0 && prevSelectedY >= 0 && prevSelectedX < width && prevSelectedY < height) {
        for (int i = 0; i < tileHeight; ++i) {
            for (int j = 0; j < tileWidth; ++j) {
                gotoxy(prevSelectedX * tileWidth + j, prevSelectedY * tileHeight + i);
                if ((prevSelectedX + prevSelectedY) % 2 == 0) {
                    wcout << WHITE;
                }
                else {
                    wcout << BLACK;
                }
            }
        }
    }

    // Draw the pieces on the board
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Print piece symbol if a piece is present at (x, y)
            if (virtualMap.at(y).at(x) != nullptr) {
                gotoxy(x * tileWidth + (tileWidth / 2) - 1, y * tileHeight + (tileHeight / 2)); // Print in the center of the tile
                wcout << virtualMap.at(y).at(x)->getSymbol();
                gotoxy(x * tileWidth + (tileWidth / 2), y * tileHeight + (tileHeight / 2));
                wcout << BLACK; // Ensure the space to the right is always black
            }
        }
    }

    // Draw the cursor
    int cursorX = cursor.get_x_location();
    int cursorY = cursor.get_y_location();
    gotoxy(cursorX * tileWidth + (tileWidth / 2), cursorY * tileHeight + (tileHeight / 2));
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    wcout << cursor.show_cursor(); // Display cursor symbol

    // Highlight selected piece
    if (selected) {
        gotoxy(selectedPieceX * tileWidth + (tileWidth / 2), selectedPieceY * tileHeight + (tileHeight / 2));
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
        wcout << L'\u2713'; // Example: print "checkmark" character to indicate selection
    }

    // Print game status
    gotoxy(width * tileWidth + 4, 0);
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN);
    wcout << L"STATUS";
    gotoxy(width * tileWidth + 4, 1);

    // Show whose turn it is
    if (whiteTurn) {
        wcout << L"Turn: White   ";
    }
    else {
        wcout << L"Turn: Black   ";
    }
    gotoxy(width * tileWidth + 4, 2);

    if (selected) {
        Piece* selectedPiece = virtualMap.at(selectedPieceY).at(selectedPieceX);
        if (selectedPiece->getWhite()) {
            wcout << L"Piece selected: White " << pieceTypeToString(selectedPiece->getType()) << L"        ";
        }
        else {
            wcout << L"Piece selected: Black " << pieceTypeToString(selectedPiece->getType()) << L"        ";
        }
    }
    else if (capturedPiece != nullptr) {
        if (capturedPiece->getWhite()) {
            wcout << L"Captured: White " << pieceTypeToString(capturedPiece->getType()) << L"       ";
        }
        else {
            wcout << L"Captured: Black " << pieceTypeToString(capturedPiece->getType()) << L"        ";
        }
    }
    else {
        wcout << L"No Piece selected           ";
    }
    int kingXWhite, kingYWhite, kingXBlack, kingYBlack;
    findKing(true, kingXWhite, kingYWhite);
    findKing(false, kingXBlack, kingYBlack);
    bool whiteInCheck = isThreatened(kingXWhite, kingYWhite, true);
    bool blackInCheck = isThreatened(kingXBlack, kingYBlack, false);
    bool whiteInMate = isCheckmate(true);
    bool blackInMate = isCheckmate(false);

    gotoxy(width * tileWidth + 4, 3);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);

    if (whiteInMate) {
        wcout << L"White Checkmated. Black Wins!        ";
    }
    else if (blackInMate) {
        wcout << L"Black Checkmated. White Wins!         ";
    }
    else if (whiteInCheck) {
        wcout << L"White King in Check!          ";
    }
    else if (blackInCheck) {
        wcout << L"Black King in Check!          ";
    }
    else {
        wcout << L"                      ";
    }

    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

    // Draw captured pieces
    int captureXOffset = width * tileWidth + 4; // Offset from the board
    int captureYOffset = 5;

    // Draw white captured pieces
    gotoxy(width * tileWidth + 4, 5);
    wcout << L"White:       ";
    captureYOffset += 1;
    for (Piece* piece : whiteCaptured) {
        gotoxy(captureXOffset, captureYOffset);
        wcout << piece->getSymbol() << L"      ";
        captureXOffset += 2;
    }

    captureXOffset = width * tileWidth + 4;
    captureYOffset += 3;

    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);

    // Draw black captured pieces
    gotoxy(width * tileWidth + 4, 8);
    wcout << L"Black:       ";
    for (Piece* piece : blackCaptured) {
        gotoxy(captureXOffset, captureYOffset);
        wcout << piece->getSymbol() << L"      ";
        captureXOffset += 2;
    }

    // Reset console text attributes
    SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

    firstRun = false;
    mtx.unlock();
}

void Board::c_update(int time, bool& game_running) {
    while (game_running) {
        draw();
        this_thread::sleep_for(std::chrono::milliseconds(time));
    }
}

void Board::cursor_move_down() {
    int current_x = cursor.get_x_location();
    int current_y = cursor.get_y_location();
    int desired_y = current_y + 1;

    if (desired_y < height) {
        cursor.set_c_location(current_x, desired_y);
    }
}

void Board::cursor_move_up() {
    int current_x = cursor.get_x_location();
    int current_y = cursor.get_y_location();
    int desired_y = current_y - 1;

    if (desired_y >= 0) {
        cursor.set_c_location(current_x, desired_y);
    }
}

void Board::cursor_move_left() {
    int current_x = cursor.get_x_location();
    int current_y = cursor.get_y_location();
    int desired_x = current_x - 1;

    if (desired_x >= 0) {
        cursor.set_c_location(desired_x, current_y);
    }
}

void Board::cursor_move_right() {
    int current_x = cursor.get_x_location();
    int current_y = cursor.get_y_location();
    int desired_x = current_x + 1;

    if (desired_x < width) {
        cursor.set_c_location(desired_x, current_y);
    }
}

void Board::setup() {
    // Place black pieces
    virtualMap.at(0).at(0) = new Rook(false);
    virtualMap.at(0).at(1) = new Knight(false);
    virtualMap.at(0).at(2) = new Bishop(false);
    virtualMap.at(0).at(3) = new Queen(false);
    virtualMap.at(0).at(4) = new King(false);
    virtualMap.at(0).at(5) = new Bishop(false);
    virtualMap.at(0).at(6) = new Knight(false);
    virtualMap.at(0).at(7) = new Rook(false);

    for (int i = 0; i < 8; i++) {
        virtualMap.at(1).at(i) = new Pawn(false);
    }

    // Place white pieces
    virtualMap.at(7).at(0) = new Rook(true);
    virtualMap.at(7).at(1) = new Knight(true);
    virtualMap.at(7).at(2) = new Bishop(true);
    virtualMap.at(7).at(3) = new Queen(true);
    virtualMap.at(7).at(4) = new King(true);
    virtualMap.at(7).at(5) = new Bishop(true);
    virtualMap.at(7).at(6) = new Knight(true);
    virtualMap.at(7).at(7) = new Rook(true);

    for (int i = 0; i < 8; i++) {
        virtualMap.at(6).at(i) = new Pawn(true);
    }
}

bool Board::getSelected() {
    return selected;
}

void Board::movePiece() {
    int toX = cursor.get_x_location();
    int toY = cursor.get_y_location();

    int fromY = selectedPieceY;
    int fromX = selectedPieceX;

    Piece* fromPiece = virtualMap.at(fromY).at(fromX);

    if (isValidMove(fromX, fromY, toX, toY)) {
        // Store previous position of the moved piece
        prevSelectedX = fromX;
        prevSelectedY = fromY;

        // Check if a piece is captured
        capturedPiece = virtualMap.at(toY).at(toX);
        if (capturedPiece != nullptr) {
            if (capturedPiece->getWhite()) {
                whiteCaptured.push_back(capturedPiece);
            }
            else {
                blackCaptured.push_back(capturedPiece);
            }
        }

        virtualMap.at(toY).at(toX) = virtualMap.at(fromY).at(fromX);
        virtualMap.at(fromY).at(fromX) = nullptr;

        if (selectedPieceX == fromX && selectedPieceY == fromY) {
            selectedPieceX = toX;
            selectedPieceY = toY;
        }

        fromPiece->setHasMoved(true);
        whiteTurn = !whiteTurn;
        pieceMoved = true;
    }
    else {
        capturedPiece = nullptr; // No piece captured
    }

    unselectPiece();
}

void Board::selectPiece() {
    int x = cursor.get_x_location();
    int y = cursor.get_y_location();

    Piece* piece = virtualMap.at(y).at(x);
    if (piece != nullptr && piece->getWhite() == whiteTurn) {
        selectedPieceX = x;
        selectedPieceY = y;
        selected = true;
    }
}

void Board::unselectPiece() {
    selectedPieceX = -1;
    selectedPieceY = -1;
    selected = false;
}

bool Board::isThreatened(int x, int y, bool white) {
    bool threatened = false;
    for (int fromY = 0; fromY < height; ++fromY) {
        for (int fromX = 0; fromX < width; ++fromX) {
            Piece* piece = virtualMap.at(fromY).at(fromX);
            if (piece != nullptr && piece->getWhite() != white) {
                // Check if the opponent's piece can move to the specified position
                switch (piece->getType()) {
                case PieceType::Pawn:
                    if (white) { // Black pawn threats
                        if ((x == fromX - 1 || x == fromX + 1) && y == fromY + 1) {
                            threatened = true;
                        }
                    }
                    else { // White pawn threats
                        if ((x == fromX - 1 || x == fromX + 1) && y == fromY - 1) {
                            threatened = true;
                        }
                    }
                    break;
                case PieceType::Rook:
                    if ((fromX == x || fromY == y) && isValidMove(fromX, fromY, x, y)) {
                        threatened = true;
                    }
                    break;
                case PieceType::Knight:
                    if (isValidMove(fromX, fromY, x, y)) {
                        threatened = true;
                    }
                    break;
                case PieceType::Bishop:
                    if (abs(fromX - x) == abs(fromY - y) && isValidMove(fromX, fromY, x, y)) {
                        threatened = true;
                    }
                    break;
                case PieceType::Queen:
                    if ((fromX == x || fromY == y || abs(fromX - x) == abs(fromY - y)) && isValidMove(fromX, fromY, x, y)) {
                        threatened = true;
                    }
                    break;
                case PieceType::King:
                    if (abs(fromX - x) <= 1 && abs(fromY - y) <= 1) {
                        threatened = true;
                    }
                    break;
                default:
                    break;
                }
            }
        }
    }
    return threatened;
}

bool Board::isValidMove(int fromX, int fromY, int toX, int toY) {
    bool isValid = true;

    // Check for out of bounds
    if (toX < 0 || toX >= width || toY < 0 || toY >= height) {
        isValid = false;
    }

    Piece* fromPiece = virtualMap.at(fromY).at(fromX);
    if (!fromPiece) {
        isValid = false;
    }

    // Check if moving to a square occupied by a piece of the same color
    if (virtualMap.at(toY).at(toX) != nullptr &&
        virtualMap.at(toY).at(toX)->getWhite() == fromPiece->getWhite()) {
        isValid = false;
    }

    if (isValid) {
        switch (fromPiece->getType()) {
        case PieceType::Pawn: {
            int direction = fromPiece->getWhite() ? -1 : 1;
            int startRow = fromPiece->getWhite() ? 6 : 1;

            // Normal move
            if (toY == fromY + direction && toX == fromX && virtualMap.at(toY).at(toX) == nullptr) {
                isValid = true;
            }
            // Double move from starting position
            else if (fromY == startRow && toY == fromY + 2 * direction && toX == fromX &&
                virtualMap.at(fromY + direction).at(fromX) == nullptr && virtualMap.at(toY).at(toX) == nullptr) {
                isValid = true;
            }
            // Capture
            else if (toY == fromY + direction && abs(toX - fromX) == 1 && virtualMap.at(toY).at(toX) != nullptr) {
                isValid = true;
            }
            else {
                isValid = false;
            }
            break;
        }
        case PieceType::Rook: {
            if (fromX == toX || fromY == toY) {
                if (fromX == toX) { // Moving vertically
                    int step = (toY - fromY) > 0 ? 1 : -1;
                    for (int y = fromY + step; y != toY; y += step) {
                        if (virtualMap.at(y).at(fromX) != nullptr) {
                            isValid = false;
                        }
                    }
                }
                else { // Moving horizontally
                    int step = (toX - fromX) > 0 ? 1 : -1;
                    for (int x = fromX + step; x != toX; x += step) {
                        if (virtualMap.at(fromY).at(x) != nullptr) {
                            isValid = false;
                        }
                    }
                }
            }
            else {
                isValid = false;
            }
            break;
        }
        case PieceType::Knight: {
            int dx = abs(toX - fromX);
            int dy = abs(toY - fromY);
            if ((dx == 2 && dy == 1) || (dx == 1 && dy == 2)) {
                isValid = true;
            }
            else {
                isValid = false;
            }
            break;
        }
        case PieceType::Bishop: {
            if (abs(fromX - toX) == abs(fromY - toY)) {
                int stepX = (toX - fromX) > 0 ? 1 : -1;
                int stepY = (toY - fromY) > 0 ? 1 : -1;
                for (int x = fromX + stepX, y = fromY + stepY; x != toX && y != toY; x += stepX, y += stepY) {
                    if (virtualMap.at(y).at(x) != nullptr) {
                        isValid = false;
                    }
                }
            }
            else {
                isValid = false;
            }
            break;
        }
        case PieceType::Queen: {
            if ((fromX == toX || fromY == toY) || (abs(fromX - toX) == abs(fromY - toY))) {
                if (fromX == toX) { // Moving vertically
                    int step = (toY - fromY) > 0 ? 1 : -1;
                    for (int y = fromY + step; y != toY; y += step) {
                        if (virtualMap.at(y).at(fromX) != nullptr) {
                            isValid = false;
                        }
                    }
                }
                else if (fromY == toY) { // Moving horizontally
                    int step = (toX - fromX) > 0 ? 1 : -1;
                    for (int x = fromX + step; x != toX; x += step) {
                        if (virtualMap.at(fromY).at(x) != nullptr) {
                            isValid = false;
                        }
                    }
                }
                else { // Moving like a bishop
                    int stepX = (toX - fromX) > 0 ? 1 : -1;
                    int stepY = (toY - fromY) > 0 ? 1 : -1;
                    for (int x = fromX + stepX, y = fromY + stepY; x != toX && y != toY; x += stepX, y += stepY) {
                        if (virtualMap.at(y).at(x) != nullptr) {
                            isValid = false;
                        }
                    }
                }
            }
            else {
                isValid = false;
            }
            break;
        }
        case PieceType::King: {
            if (abs(fromX - toX) <= 1 && abs(fromY - toY) <= 1) {
                isValid = true;
            }
            else {
                isValid = false;
            }
            break;
        }
        default:
            isValid = false;
            break;
        }
    }

    if (isValid) {
        // Temporarily move the piece to check if it puts the king in check
        Piece* temp = virtualMap.at(toY).at(toX);
        virtualMap.at(toY).at(toX) = fromPiece;
        virtualMap.at(fromY).at(fromX) = nullptr;

        bool kingInCheck = isCheck(fromPiece->getWhite());

        // Restore the board to its original state
        virtualMap.at(fromY).at(fromX) = fromPiece;
        virtualMap.at(toY).at(toX) = temp;

        if (kingInCheck) {
            isValid = false;
        }
    }

    return isValid;
}

wstring Board::pieceTypeToString(PieceType type) {
    wstring wstr = L"";
    switch (type) {
    case PieceType::Pawn: {
        wstr = L"Pawn";
        break;
    }
    case PieceType::Rook: {
        wstr = L"Rook";
        break;
    }
    case PieceType::Knight: {
        wstr = L"Knight";
        break;
    }
    case PieceType::Bishop: {
        wstr = L"Bishop";
        break;
    }
    case PieceType::Queen: {
        wstr = L"Queen";
        break;
    }
    case PieceType::King: {
        wstr = L"King";
        break;
    }
    default: {
        wstr = L"Unknown";
        break;
    }
           return wstr;
    }
}

void Board::findKing(bool white, int& kingX, int& kingY) {
    bool found = false;
    for (int y = 0; y < height && !found; ++y) {
        for (int x = 0; x < width && !found; ++x) {
            Piece* piece = virtualMap.at(y).at(x);
            if (piece != nullptr && piece->getType() == PieceType::King && piece->getWhite() == white) {
                kingX = x;
                kingY = y;
                found = true;
            }
        }
    }
    if (!found) {
        kingX = -1;
        kingY = -1; // Should never happen
    }
}

bool Board::isCheck(bool white) {
    int kingX, kingY;
    findKing(white, kingX, kingY);
    return isThreatened(kingX, kingY, white);
}

bool Board::isCheckmate(bool white) {
    bool result = true;

    if (!isCheck(white)) {
        result = false;
    }
    else {
        bool canEscape = false;

        // Check if there are any valid moves to get out of check
        for (int fromY = 0; fromY < height && !canEscape; ++fromY) {
            for (int fromX = 0; fromX < width && !canEscape; ++fromX) {
                Piece* piece = virtualMap.at(fromY).at(fromX);
                if (piece != nullptr && piece->getWhite() == white) {
                    for (int toY = 0; toY < height && !canEscape; ++toY) {
                        for (int toX = 0; toX < width && !canEscape; ++toX) {
                            if (isValidMove(fromX, fromY, toX, toY)) {
                                // Temporarily move the piece
                                Piece* tempPiece = virtualMap.at(toY).at(toX);
                                virtualMap.at(toY).at(toX) = piece;
                                virtualMap.at(fromY).at(fromX) = nullptr;

                                if (!isCheck(white)) {
                                    canEscape = true;
                                }

                                // Undo the move
                                virtualMap.at(fromY).at(fromX) = piece;
                                virtualMap.at(toY).at(toX) = tempPiece;
                            }
                        }
                    }
                }
            }
        }

        result = !canEscape;
    }

    return result;
}

bool Board::playAgainPrompt() {
    string response;
    gotoxy(0, 0);
    wcout << "Do you want to play again? (y/n): ";
    getline(cin, response);
    return response == "y" || response == "Y";
}

void Board::clearBoard() {
    for (int i = 0; i < virtualMap.size(); ++i) {
        for (int j = 0; j < virtualMap.at(i).size(); ++j) {
            delete virtualMap.at(i).at(j);
            virtualMap.at(i).at(j) = nullptr;
        }
    }
    whiteCaptured.clear();
    blackCaptured.clear();
}

void Board::resetGame() {
    clearBoard();
    firstRun = true;
    whiteTurn = true;
    selectedPieceX = 100;
    selectedPieceY = 100;
    pieceMoved = false;
    capturedPiece = nullptr;
    setup();
}