#include <stdio.h>
#include <assert.h>

#include "test.h"

/*
 * The status of a line.
 */
enum {
    NO = 0, /* No line */
    RED = 1,
    BLUE = 2
};

/*
 * The board records the colors of the lines.
 * board[0] = color of 12
 * board[1] = color of 13
 * ...
 * board[14] = color of 56
 */
typedef char board_t[15];
typedef char player_t; /* A player should be RED or BLUE. */

player_t other(player_t player)
{
    return (player == RED) ? BLUE : RED;
}

void init_board(board_t board)
{
    for (int i = 0; i < 15; ++i) {
        board[i] = NO;
    }
}


int has_won(board_t board, player_t player)
{   int a[4] = {4, 7, 9, 10};
    int b[3] = {3, 5, 6};
    int c[2] = {2, 3};
    int d = 1;

    for (int i = 0; i < 4; i++) {
        for (int j = i + 1; j <= 4; j++) {
            if (board[i] == other(player) &&
                board[j] == other(player) &&
                board[j + a[i]] == other(player)) {
                return 1;
            }
        }
    }

    for (int i = 5; i < 8; i++) {
        for (int j = i + 1; j <= 8; j++) {
            if (board[i] == other(player) &&
                board[j] == other(player) &&
                board[j + b[i - 5]] == other(player)) {
                return 1;
            }
        }
    }

    for (int i = 9; i < 11; i++) {
        for (int j = i + 1; j <= 11; j++) {
            if (board[i] == other(player) &&
                board[j] == other(player) &&
                board[j + c[i - 9]] == other(player)) {
                return 1;
            }
        }
    }

    for (int i = 12; i < 13; i++) {
        for (int j = i + 1; j <= 13; j++) {
            if (board[i] == other(player) &&
                board[j] == other(player) &&
                board[j + d] == other(player)) {
                return 1;
            }
        }
    }

    return 0;
}

int is_full(board_t board)
{}

typedef struct {
    int line; /* 0 for 12, 1 for 13, ..., 14 for 56. */
    int score; /* -1 for loss, 0 for draw, 1 for win. */
} move_t;

move_t best_move(board_t board, player_t player)
{
    move_t response;
    move_t best;
    int no_best = 1;

    for (int i = 0; i < 15; ++i) {
        if (board[i] == NO) {
            board[i] = player;
            if (has_won(board, player)) {
                board[i] = NO;
                return (move_t){i, 1};
            }
            board[i] = NO;
        }
    }

    for (int i = 0; i < 15; ++i) {
        if (board[i] == NO) {
            board[i] = player;
            response = best_move(board, other(player));
            board[i] = NO;
            if (is_full(board)) {
                board[i] = NO;
                return (move_t) {i, 0};
            }
            if (response.score == -1) {
                return (move_t){i, 1};
            } else if (response.score == 0) {
                best = (move_t){i, 0};
                no_best = 0;
            } else {
                if (no_best) {
                    best = (move_t){i, -1};
                    no_best = 0;
                }
            }
        }
    }

    return best;
}

void print_board(board_t board)
{
    for (int i = 0; i < 15; ++i) {
        switch (board[i]) {
        case NO: printf("0  "); break;
        case RED: printf("R  "); break;
        case BLUE: printf("B  "); break;
        }
    }
}



int main()
{
    int move, i;
    board_t board;
    move_t response;
    int x;
    printf("Computer is playing as player 2.\n");
    printf("Do you want to play as RED (1) or BLUE (2): ");
    scanf("%d", &x);
    player_t current;

    if (x == 1) {
        current = RED;
    } else if (x == 2) {
        current = BLUE;
    } else {
        printf("\nInvalid choice");
        return 1;
    }

    init_board(board);

    while (1) {
        print_board(board);
        printf("\n\n");

        if (current == RED) {
            printf("0  1  2  3  4  5  6  7  8  9 10 11 12 13 14\n");
            printf("\nYour Move: ");
            scanf("%d", &move);
            i = move;
            if (i >= 0 && i < 15 && board[i] == NO) {
                board[i] = current;
            } 
            else {
                printf("Invalid move. Try again.\n");
                continue;
            }
        } else {
            response = best_move(board, current);
            board[response.line] = current;
        }

        if (has_won(board, current)) {
            print_board(board);
            printf("Player %c has won!\n", (current == RED) ? 'R' : 'B');
            break;
        }

        current = other(current);
    }
    /* Your game play logic. */
    /* The user should have the option to select red or blue player. */
    return 0;
}
