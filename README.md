# Chess Final Project

A console-based Chess application built in C++ for Windows, featuring a multi-threaded rendering engine and full chess logic.

## Overview

This project implements a complete Chess game playable within the Windows command prompt. It utilizes Unicode characters for rendering the board and pieces, providing a clean and intuitive interface.

## Techniques & Features

### 1. Multi-Threading
The application employs **Multi-threading** to separate user input handling from the display logic:
- **Rendering Thread:** A background thread (`Board::c_update`) is responsible for periodically refreshing the board display. This ensures that UI elements like the cursor remain responsive.
- **Main Thread:** Handles keyboard inputs and game state transitions.
- **Thread Synchronization:** A `std::mutex` is used to prevent race conditions when the rendering thread and the main thread both attempt to access or modify the console state simultaneously.

### 2. Object-Oriented Programming (OOP)
The codebase is structured around core OOP principles:
- **Inheritance:** A base `Piece` class defines common attributes (color, position, symbols), while derived classes (`Pawn`, `Rook`, `Knight`, `Bishop`, `Queen`, `King`) implement specific movement rules via polymorphism.
- **Encapsulation:** The `Board` class encapsulates the game grid, move validation, and win-condition checks.

### 3. Windows Console API
The game leverages the **Windows.h** library for low-level console control:
- **Coordinate Manipulation:** Uses `SetConsoleCursorPosition` (via `gotoxy`) for precision drawing of the board tiles and pieces.
- **Input Detection:** Uses `GetAsyncKeyState` for non-blocking real-time keyboard input handling.
- **Cursor Management:** Hides the native console cursor to provide a custom game-style cursor experience.

### 4. Advanced Game Logic
- **Check/Checkmate Detection:** Real-time scanning of the board to determine king threats.
- **Move Validation:** Complex pathfinding and collision detection for each piece type.
- **Unicode Support:** Utilizes `_setmode` with `_O_U16TEXT` to display high-quality chess symbols.

## Controls

- **Arrow Keys:** Navigate the cursor.
- **Space:** Select piece / Confirm move.
- **Tab:** Unselect piece.
- **Esc:** Exit game.

## Build Requirements

- **Platform:** Windows
- **Compiler:** MSVC (Visual Studio 2019/2022 recommended)
- **Language Standard:** C++11 or higher