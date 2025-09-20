# Connect 4 AI Solvers in C

This repository contains C implementations of an unbeatable AI for the game of Connect 4. The project was developed to explore and implement classic game theory algorithms, demonstrating how a perfect player can be created for a solved game.

The repository includes two distinct approaches to creating an optimal AI player, both of which will always win when playing as the first player ('Y') on the default board.

## Features

* **Two Unbeatable AI Engines:**
    1.  A classic **Minimax** algorithm with Alpha-Beta Pruning.
    2.  A fast **Recursive Solver** using Memoization (Transposition Tables).
* **Interactive Command-Line Interface:** Play directly against the AI in your terminal.
* **Optimal Play:** The AI plays perfectly, guaranteeing a win or draw from any position.
* **Customizable Board Size:** Easily change the game board dimensions by modifying global constants.
* **Well-Commented Code:** The source code is commented to explain the logic behind the algorithms.

## The AI Approaches Explained

### 1. Minimax with Alpha-Beta Pruning (`minimax_solver.c`)

This AI behaves like a **master strategist**. It simulates the game many moves into the future to find the best possible move.

* **Core Logic:** It uses the Minimax algorithm to explore a tree of possible future moves, assuming the opponent will always play their best possible move in response.
* **Optimization:** The search is made incredibly efficient by **Alpha-Beta Pruning**, which allows the AI to ignore branches of the game tree that it knows are worse than an option it has already found.

### 2. Recursive Solver with Memoization (`memoization_solver.c`)

This AI behaves like a **mathematician with a perfect memory**. It doesn't guess how good a position is; it solves it for a guaranteed outcome.

* **Core Logic:** It uses a recursive Depth-First Search (`explore`) to determine if a board state is a guaranteed **WIN**, **LOSS**, or **DRAW**.
* **Memoization (Caching):** This is the AI's "photographic memory." Every time it solves a board position, it stores the result in a large cache (a hash table). If it ever sees that same board position again, it recalls the solution instantly instead of re-calculating it.

## How to Compile and Run

### Prerequisites

You must have a C compiler like **GCC** installed on your system. You can check if it's installed by running:

```sh
gcc --version
```
**###Compilation**
Clone the repository and navigate into the directory.

Compile the desired AI program using one of the following commands:

To compile the Minimax version:

```sh
gcc -o minimax_player minimax_solver.c
To compile the Memoization Solver version:
```
```sh
gcc -o memo_player memoization_solver.c
```

**###Running the Game**
Run the compiled program from your terminal:
```sh
./minimax_player
```
or
```sh
./memo_player
```
The program will wait for the first line of input. To make the computer play first (as 'Y'), type Y and press Enter.

The computer will print its move (e.g., Y 3).

Enter your move in the format R n, where n is the column number (e.g., R 2).

Continue playing until a winner is declared or the game is a draw.

Customization and Debugging
Changing the Board Size
You can easily change the game dimensions in either C file by modifying the global constants at the top of the code:

```c
// --- Global Board Constants (Can be changed) ---
#define ROWS 4
#define COLS 5
// ------------------------------------------------
```
Note: Changing the board size may impact the AI's ability to play perfectly, as the game may no longer be "solved" within a practical search depth. The check_win function is hardcoded for a win condition of 4 in a row. But you can play with it, change the depth and see what happens.

Debugging with print_board
Each C file contains a print_board() function. You can call this function at any point (e.g., within the main loop or inside the AI functions) to print the current state of the board to the console for debugging purposes, or for ease of playing.

```c
// Example usage:
drop_piece(board, col, AI_PIECE);
// print_board(board); // <-- Uncomment or add this line to see the board state
```

Any suggestions or pull requests are welcome.
