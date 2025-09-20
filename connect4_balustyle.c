#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

#define ROWS 4
#define COLS 5
#define E 0
#define Y 1 
#define R 2

#define WIN  1
#define LOSS 2
#define DRAW 3
#define UNKNOWN 0

int AI_PIECE = Y;
int HUMAN_PIECE = R;

// --- Memoization Cache ---
#define CACHE_SIZE 1048576 // 1024 * 1024
typedef struct {
    uint64_t key;
    uint8_t outcome;
    int8_t best_move;
} CacheEntry;
CacheEntry cache[CACHE_SIZE] = {0};

void create_board(int board[ROWS][COLS]){ 
    for(int i=0; i<ROWS; i++){ 
        for(int j=0; j<COLS; j++){
            board[i][j]=E; 
        }
    }
}

void print_board(int board[ROWS][COLS]){ 
    for(int i=0; i<ROWS; i++){ 
        for(int j=0; j<COLS; j++){ 
            if(board[i][j]==Y) printf("Y "); 
            else if(board[i][j]==R) printf("R "); 
            else printf(". "); 
        } 
        printf("\n"); 
    } 
}

bool is_valid_location(int board[ROWS][COLS], int col){ 
    if (col < 0 || col >= COLS) return false; 
    return (board[0][col] == 0); 
}

bool check_win(int board[ROWS][COLS], int piece){ 
    for(int r=0; r<ROWS; r++){ 
        for(int c=0; c<=COLS-4; c++){ 
            if(board[r][c]==piece && board[r][c+1]==piece && board[r][c+2]==piece && board[r][c+3]==piece) return true; 
        }
    } 
    for(int c=0; c<COLS; c++){ 
        for(int r=0; r<=ROWS-4; r++){ 
            if(board[r][c]==piece && board[r+1][c]==piece && board[r+2][c]==piece && board[r+3][c]==piece) return true; 
        }
    } 
    for(int r=0; r<=ROWS-4; r++){ 
        for(int c=0; c<=COLS-4; c++){ 
            if(board[r][c]==piece && board[r+1][c+1]==piece && board[r+2][c+2]==piece && board[r+3][c+3]==piece) return true; 
        }
    } 
    for(int r=3; r<ROWS; r++){ 
        for(int c=0; c<=COLS-4; c++){ 
            if(board[r][c]==piece && board[r-1][c+1]==piece && board[r-2][c+2]==piece && board[r-3][c+3]==piece) return true; 
        }
    } 
    return false; 
}

bool is_draw(int board[ROWS][COLS]){ 
    for(int c=0; c<COLS; c++){
        if(board[0][c]==E) return false; 
    }
    return true; 
}

bool drop_piece(int board[ROWS][COLS], int col, int piece){ 
    if (!is_valid_location(board, col)) return false; 
    for(int r=ROWS-1; r>=0; r--){ 
        if(board[r][col]==E){ 
            board[r][col]=piece; 
            return true; 
        }
    } 
    return false; 
}

// Creates a unique number for each board state
uint64_t compute_key(int board[ROWS][COLS]) {
    uint64_t key = 0;
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            key = key * 3 + board[r][c];
        }
    }
    return key;
}

// The recursive solver function
uint8_t explore(int board[ROWS][COLS], int player) {
    int opponent = (player == Y) ? R : Y;
    if (check_win(board, opponent)) {
        return LOSS;
    }
    if (is_draw(board)) {
        return DRAW;
    }
    // Memoization Check: See if we've already solved this state
    uint64_t key = compute_key(board);
    int cache_index = key % CACHE_SIZE;
    if (cache[cache_index].key == key && cache[cache_index].outcome != UNKNOWN) {
        return cache[cache_index].outcome;
    }
    int draw_move = -1;
    // Explore all possible moves
    for (int col = 0; col < COLS; col++) {
        if (is_valid_location(board, col)) {
            int board_copy[ROWS][COLS];
            memcpy(board_copy, board, ROWS * COLS * sizeof(int));
            drop_piece(board_copy, col, player);
            // Recursively call for the other player
            uint8_t child_outcome = explore(board_copy, opponent);
            // If the opponent is guaranteed to lose from this move, we have found a winning move!
            if (child_outcome == LOSS) {
                cache[cache_index] = (CacheEntry){key, WIN, col};
                return WIN;
            }
            // If a move leads to a draw, remember it as a possibility
            if (child_outcome == DRAW) {
                draw_move = col;
            }
        }
    }
    // If we couldn't find a winning move, but found a draw, that's our best option
    if (draw_move != -1) {
        cache[cache_index] = (CacheEntry){key, DRAW, draw_move};
        return DRAW;
    }
    // If all moves lead to the opponent winning, we are forced to lose
    cache[cache_index] = (CacheEntry){key, LOSS, 0}; // Move doesn't matter if we lose
    return LOSS;
}

// The main interface
int find_best_move(int board[ROWS][COLS]) {
    // Make sure the current state is solved
    explore(board, AI_PIECE);
    // Retrieve the best move from the cache
    uint64_t key = compute_key(board);
    int cache_index = key % CACHE_SIZE;
    if (cache[cache_index].key == key) {
        return cache[cache_index].best_move;
    }
    // Fallback (should not be needed if explore works)
    for(int c=0; c<COLS; c++) if(is_valid_location(board, c)) return c;
    return 0;
}

// --- Main Game Loop ---
int main(){
    int board[ROWS][COLS];
    create_board(board);
    bool game_over = false;
    char my_color_char;
    scanf(" %c", &my_color_char);
    char ai_char, human_char;
    int current_turn_player; 
    if (my_color_char == 'Y') {
        AI_PIECE = Y; HUMAN_PIECE = R; ai_char = 'Y'; human_char = 'R';
        current_turn_player = 0; // 0 for Y's turn
    } else {
        AI_PIECE = R; HUMAN_PIECE = Y; ai_char = 'R'; human_char = 'Y';
        current_turn_player = 0; // Y (human) still goes first
    }
    while (!game_over){
        // AI's Turn
        if ((AI_PIECE == Y && current_turn_player == 0) || (AI_PIECE == R && current_turn_player == 1)) {
            int col = find_best_move(board);
            drop_piece(board, col, AI_PIECE);
            printf("%c %d\n", ai_char, col + 1);
            fflush(stdout);

            if (check_win(board, AI_PIECE)) {
                printf("%c won\n", ai_char); game_over = true;
            }
        } 
        // Human's Turn
        else {
            char color_char;
            int col;
            if(scanf(" %c %d", &color_char, &col) != 2) break; // End on bad input
            drop_piece(board, col - 1, HUMAN_PIECE);

            if (check_win(board, HUMAN_PIECE)) {
                printf("%c won\n", human_char); game_over = true;
            }
        }
        if (!game_over && is_draw(board)) {
            printf("draw\n"); game_over = true;
        }
        current_turn_player = 1 - current_turn_player; // Switch player (0 to 1, 1 to 0)
    }
    return 0;
}