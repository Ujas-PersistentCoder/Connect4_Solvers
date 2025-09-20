#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>

#define ROWS 4
#define COLS 5
#define E 0
#define Y 1 
#define R 2

int AI_PIECE = Y;
int HUMAN_PIECE = R;

// --- Forward declaration with new alpha-beta parameters ---
int minimax(int board[ROWS][COLS], int depth, int alpha, int beta, bool is_maximizing_player);


void create_board(int board[ROWS][COLS]){
    for (int i = 0; i < ROWS; i++){ 
        for (int j = 0; j < COLS; j++){ 
            board[i][j] = E; 
        } 
    } 

}

void print_board(int board[ROWS][COLS]){ 
    for (int i = 0; i < ROWS; i++){ 
        for (int j = 0; j < COLS; j++){ 
            if (board[i][j] == Y) { 
                printf("Y "); 
            } else if (board[i][j] == R) { 
                printf("R "); 
            } else { 
                printf(". "); 
            } 
        } 
        printf("\n"); 
    } 
}

bool is_valid_location(int board[ROWS][COLS], int col){ 
    if (col < 0 || col >= COLS) return false; 
    return (board[0][col] == 0); 
}

bool drop_piece(int board[ROWS][COLS], int col, int piece){ 
    if (!is_valid_location(board, col)) return false; 
    for (int r = ROWS - 1; r >= 0; r--) { 
        if (board[r][col] == E) { 
            board[r][col] = piece; 
            return true; 
        } 
    } 
    return false; 
}

bool check_win(int board[ROWS][COLS], int piece){ 
    for (int r = 0; r < ROWS; r++) { 
        for (int c = 0; c <= COLS - 4; c++) { 
            if (board[r][c] == piece && board[r][c+1] == piece && board[r][c+2] == piece && board[r][c+3] == piece) return true; 
        } 
    } 
    for (int c = 0; c < COLS; c++) { 
        for (int r = 0; r <= ROWS - 4; r++) { 
            if (board[r][c] == piece && board[r+1][c] == piece && board[r+2][c] == piece && board[r+3][c] == piece) return true; 
        } 
    } 
    for (int r = 0; r <= ROWS - 4; r++) { 
        for (int c = 0; c <= COLS - 4; c++) { 
            if (board[r][c] == piece && board[r+1][c+1] == piece && board[r+2][c+2] == piece && board[r+3][c+3] == piece) return true; 
        } 
    } 
    for (int r = 3; r < ROWS; r++) { 
        for (int c = 0; c <= COLS - 4; c++) { 
            if (board[r][c] == piece && board[r-1][c+1] == piece && board[r-2][c+2] == piece && board[r-3][c+3] == piece) return true; 
        } 
    } 
    return false; 
}

bool is_draw(int board[ROWS][COLS]) { 
    for (int c = 0; c < COLS; c++) { 
        if (board[0][c] == E) return false; 
    } 
    return true; }

int max(int a, int b) { 
    return (a > b) ? a : b; 
}

int min(int a, int b) { 
    return (a < b) ? a : b; 
}

int evaluate_window(int window[4], int piece) { 
    int score = 0; 
    int opp_piece = (piece == Y) ? R : Y; 
    int piece_count = 0; 
    int opp_count = 0; 
    int empty_count = 0; 
    for (int i = 0; i < 4; i++) { 
        if (window[i] == piece) piece_count++; 
        else if (window[i] == opp_piece) opp_count++; 
        else empty_count++; 
    } 
    if (piece_count == 4) score += 100; 
    else if (piece_count == 3 && empty_count == 1) score += 5; 
    else if (piece_count == 2 && empty_count == 2) score += 2; 
    if (opp_count == 3 && empty_count == 1) score -= 4; 
    return score; 
}

int score_position(int board[ROWS][COLS], int piece) { 
    int score = 0; 
    for (int r = 0; r < ROWS; r++) { 
        if (board[r][COLS / 2] == piece) score += 3; 
    } 
    for (int r = 0; r < ROWS; r++) { 
        for (int c = 0; c <= COLS - 4; c++) { 
            int window[4] = {board[r][c], board[r][c+1], board[r][c+2], board[r][c+3]}; 
            score += evaluate_window(window, piece); 
        } 
    } 
    for (int c = 0; c < COLS; c++) { 
        for (int r = 0; r <= ROWS - 4; r++) { 
            int window[4] = {board[r][c], board[r+1][c], board[r+2][c], board[r+3][c]}; 
            score += evaluate_window(window, piece); 
        } 
    } 
    for (int r = 0; r <= ROWS - 4; r++) { 
        for (int c = 0; c <= COLS - 4; c++) { 
            int window_pos[4] = {board[r][c], board[r+1][c+1], board[r+2][c+2], board[r+3][c+3]}; 
            score += evaluate_window(window_pos, piece); 
            int window_neg[4] = {board[r+3][c], board[r+2][c+1], board[r+1][c+2], board[r][c+3]}; 
            score += evaluate_window(window_neg, piece); 
        } 
    } 
    return score; }

int find_best_move(int board[ROWS][COLS], int turn_count){
    int best_score = INT_MIN;
    int best_col = -1;

    // The center column is often the best starting move
    if (is_valid_location(board, COLS / 2)) {
        best_col = COLS / 2;
    }

    for (int col = 0; col < COLS; col++){
        if (is_valid_location(board, col)) {
            int board_copy[ROWS][COLS];
            memcpy(board_copy, board, ROWS * COLS * sizeof(int));
            
            drop_piece(board_copy, col, AI_PIECE);
            
            // **CHANGE 1: Calculate full depth and use alpha-beta**
            int depth = (ROWS * COLS) - turn_count; // Search all remaining moves
            int score = minimax(board_copy, depth, INT_MIN, INT_MAX, false);
            
            if (score > best_score){
                best_score = score;
                best_col = col;
            }
        }
    }
    return best_col;
}

// **CHANGE 2: Minimax now includes alpha and beta for pruning**
int minimax(int board[ROWS][COLS], int depth, int alpha, int beta, bool is_maximizing_player){
    // Base Case check
    bool is_terminal = check_win(board, AI_PIECE) || check_win(board, HUMAN_PIECE) || is_draw(board);
    if (depth == 0 || is_terminal) {
        if (is_terminal) {
            if (check_win(board, AI_PIECE)) return 10000000;
            if (check_win(board, HUMAN_PIECE)) return -10000000;
            if (is_draw(board)) return 0;
        }
        return score_position(board, AI_PIECE); // Heuristic if depth runs out
    }
    
    if (is_maximizing_player){ // AI's turn (Maximizer)
        int best_score = INT_MIN;
        for (int c = 0; c < COLS; c++) {
            if (is_valid_location(board, c)) {
                int board_copy[ROWS][COLS];
                memcpy(board_copy, board, ROWS * COLS * sizeof(int));
                drop_piece(board_copy, c, AI_PIECE);
                int score = minimax(board_copy, depth - 1, alpha, beta, false);
                best_score = max(best_score, score);
                alpha = max(alpha, best_score); // Update alpha
                if (alpha >= beta) break; // Pruning
            }
        }
        return best_score;
    } 
    else { // Human's turn (Minimizer)
        int best_score = INT_MAX;
        for (int c = 0; c < COLS; c++) {
            if (is_valid_location(board, c)) {
                int board_copy[ROWS][COLS];
                memcpy(board_copy, board, ROWS * COLS * sizeof(int));
                drop_piece(board_copy, c, HUMAN_PIECE);
                int score = minimax(board_copy, depth - 1, alpha, beta, true);
                best_score = min(best_score, score);
                beta = min(beta, best_score); // Update beta
                if (alpha >= beta) break; // Pruning
            }
        }
        return best_score;
    }
}

// --- Main function to handle game flow ---

int main(){
    int board[ROWS][COLS];
    create_board(board);
    bool game_over = false;
    
    char my_color_char;
    scanf(" %c", &my_color_char);
    
    char ai_char, human_char;
    int turn_number = 0;
    int current_turn_player; // 0 for player Y, 1 for player R

    if (my_color_char == 'Y') {
        AI_PIECE = Y; HUMAN_PIECE = R; ai_char = 'Y'; human_char = 'R';
        current_turn_player = 0;
    } else {
        AI_PIECE = R; HUMAN_PIECE = Y; ai_char = 'R'; human_char = 'Y';
        current_turn_player = 0; // Y (human) still goes first
    }

    while (!game_over){
        // AI's Turn
        if ((AI_PIECE == Y && current_turn_player == 0) || (AI_PIECE == R && current_turn_player == 1)) {
            int col = find_best_move(board, turn_number + 1);
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
            scanf(" %c %d", &color_char, &col);
            drop_piece(board, col - 1, HUMAN_PIECE);

            if (check_win(board, HUMAN_PIECE)) {
                printf("%c won\n", human_char); game_over = true;
            }
        }

        if (!game_over && is_draw(board)) {
            printf("draw\n"); game_over = true;
        }

        turn_number++;
        current_turn_player = 1 - current_turn_player; // Switch player
    }
    return 0;
}