#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Implement connect 4 on a 5 (columns) x 4 (rows) board. */
enum {
    EMPTY = 0,
    RED ,
    BLUE,
};

typedef char board_t[4][5];
typedef char player_t;

player_t other_player(player_t player) {
    return (player == RED) ? BLUE : RED;
}

int has_won(board_t board, player_t player)
{
    // Checking horizontal win
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 2; col++) {
            if (board[row][col] == player &&
                board[row][col + 1] == player &&
                board[row][col + 2] == player &&
                board[row][col + 3] == player) {
                return 1;
            }
        }
    }

    // Checking vertical win
    for (int col = 0; col < 5; col++) {
        for (int row = 0; row < 1; row++) {
            if (board[row][col] == player &&
                board[row + 1][col] == player &&
                board[row + 2][col] == player &&
                board[row + 3][col] == player) {
                return 1;
            }
        }
    }

    // Checking diagonal (top-left to bottom-right)
    for (int row = 0; row < 2; row++) {
        for (int col = 0; col < 2; col++) {
            if (board[row][col] == player &&
                board[row + 1][col + 1] == player &&
                board[row + 2][col + 2] == player &&
                board[row + 3][col + 3] == player) {
                return 1;
            }
        }
    }

    // Checking diagonal (bottom-left to top-right)
    for (int row = 3; row >= 2; row--) {
        for (int col = 0; col < 2; col++) {
            if (board[row][col] == player &&
                board[row - 1][col + 1] == player &&
                board[row - 2][col + 2] == player &&
                board[row - 3][col + 3] == player) {
                return 1;
            }
        }
    }

    return 0;
}

int is_full(board_t board)
{
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 5; col++) {
            if (board[row][col] == EMPTY) {
                return 0;
            }
        }
    }
    return 1; //full
}

int column_NA(board_t board, int col) {
    return board[0][col] != EMPTY;
} 

typedef struct {
    int col;
    int score;
} move_t;

int empty_row(board_t board, int col, player_t player)
{
    for (int row = 3; row >= 0; row--) {
        if (board[row][col] == EMPTY) {
            board[row][col] = player;
            return row; 
        }
    }
    return -1; 
}

move_t best_move(board_t board, player_t player, int depth) {

    move_t Best_Move;
    move_t currentMove = {-1, -1};
    int no_move = 1;

    if (depth == 0 || is_full(board)) {
        currentMove.score = 0;
        return currentMove;
    }
     for (int col = 0; col < 5; ++col) {
        if (!column_NA(board, col)) {
            int row = empty_row(board, col, player);
            board[row][col] = player;
            if (has_won(board, player)) {
                board[row][col] = EMPTY;
                return (move_t){col, 1};
            }
            board[row][col] = EMPTY;
        }
    } 
    for (int col = 0; col < 5; ++col) {
        if (!column_NA(board, col)) {
            int row = empty_row(board, col, player);
            board[row][col] = player;
            if (is_full(board)) {
                board[row][col] = EMPTY;
                return (move_t){col, 0};
            }
            currentMove = best_move(board, other_player(player), depth - 1);
            board[row][col] = EMPTY;

            if (currentMove.score == -1) {
                return (move_t){col, 1};
            } else if (currentMove.score == 0) {
                Best_Move = (move_t){col, 0};
                no_move = 0;
            } else {
                if (no_move) {
                    Best_Move = (move_t){col, -1};
                    no_move = 0;
                }
            }
        }
    }

    return Best_Move;
}

void print_board(board_t board)
{
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 5; col++) {
            switch (board[row][col]) {
                case EMPTY:
                    printf(". ");
                    break;
                case RED:
                    printf("R ");
                    break;
                case BLUE:
                    printf("B ");
                    break;
            }
        }
        printf("\n");
    }
}

int main() {
    board_t board;
    player_t currentPlayer = RED; 
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 5; j++) {
            board[i][j] = EMPTY;
        }
    }
    while (1) {
        print_board(board);

        if (currentPlayer == RED) {
            int col;
            printf("%s's turn. Enter a column No. between 1-5: ", (currentPlayer == RED) ? "RED" : "BLUE");
            scanf("%d", &col);

            if (col < 1 || col > 5) {
                printf("Invalid column. Try Again\n");
                continue;
            }
            col--;

            if (column_NA(board, col)) {
                printf("Column is full. Please choose another column.\n");
                continue;
            }
          
            int emptyRow = empty_row(board, col, currentPlayer);
            board[emptyRow][col] = currentPlayer;
        } else {
            move_t computerMove = best_move(board, currentPlayer, 3); 
            int col = computerMove.col;

            if (column_NA(board, col)) {
                continue; 
            }
            int emptyRow = empty_row(board, col, currentPlayer);
            board[emptyRow][col] = currentPlayer;
            if (has_won(board, currentPlayer)) {
                print_board(board);
                printf("%s player wins!\n", (currentPlayer == RED) ? "RED" : "BLUE");
                break;
            }
        }

        if (has_won(board, currentPlayer)) {
            print_board(board);
            printf("%s player wins!\n", (currentPlayer == RED) ? "RED" : "BLUE");
            break;
        }

        if (is_full(board)) {
            printf("No further moves available, it's a draw!\n");
            break;
        }

        currentPlayer = other_player(currentPlayer);
    }

    return 0;
}
