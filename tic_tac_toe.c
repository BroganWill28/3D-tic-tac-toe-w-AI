#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define SIZE 3
#define EMPTY ' '
#define PLAYER_X 'X'
#define PLAYER_O 'O'

char board[SIZE][SIZE][SIZE];

// Function to initialize the 3D board
void initBoard() {
    for (int z = 0; z < SIZE; z++) {
        for (int x = 0; x < SIZE; x++) {
            for (int y = 0; y < SIZE; y++) {
                board[z][x][y] = EMPTY;
            }
        }
    }
}

// Function to print the 3D board
void printBoard() {
    for (int z = 0; z < SIZE; z++) {
        printf("Layer %d:\n", z + 1);
        for (int x = 0; x < SIZE; x++) {
            for (int y = 0; y < SIZE; y++) {
                printf("%c ", board[z][x][y]);
            }
            printf("\n");
        }
        printf("\n");
    }
}

// Function to check if a player has won
int isWinner(char player) {
    // Check all 2D layers (rows, columns, diagonals)
    for (int z = 0; z < SIZE; z++) {
        for (int x = 0; x < SIZE; x++) {
            if (board[z][x][0] == player && board[z][x][1] == player && board[z][x][2] == player) return 1;  // Row in layer
            if (board[z][0][x] == player && board[z][1][x] == player && board[z][2][x] == player) return 1;  // Column in layer
        }
        if (board[z][0][0] == player && board[z][1][1] == player && board[z][2][2] == player) return 1;  // Main diagonal in layer
        if (board[z][0][2] == player && board[z][1][1] == player && board[z][2][0] == player) return 1;  // Other diagonal in layer
    }

    // Check vertical wins across layers
    for (int x = 0; x < SIZE; x++) {
        for (int y = 0; y < SIZE; y++) {
            if (board[0][x][y] == player && board[1][x][y] == player && board[2][x][y] == player) return 1;  // Vertical column in 3D
        }
    }

    // Check main 3D diagonals
    if (board[0][0][0] == player && board[1][1][1] == player && board[2][2][2] == player) return 1;  // (0,0,0) -> (2,2,2)
    if (board[0][0][2] == player && board[1][1][1] == player && board[2][2][0] == player) return 1;  // (0,0,2) -> (2,2,0)
    if (board[0][2][0] == player && board[1][1][1] == player && board[2][0][2] == player) return 1;  // (0,2,0) -> (2,0,2)
    if (board[0][2][2] == player && board[1][1][1] == player && board[2][0][0] == player) return 1;  // (0,2,2) -> (2,0,0)

    // Check layer-crossing 3D diagonals (rows)
    for (int x = 0; x < SIZE; x++) {
        if (board[0][x][0] == player && board[1][x][1] == player && board[2][x][2] == player) return 1;
        if (board[0][x][2] == player && board[1][x][1] == player && board[2][x][0] == player) return 1;
    }

    // Check layer-crossing 3D diagonals (columns)
    for (int y = 0; y < SIZE; y++) {
        if (board[0][0][y] == player && board[1][1][y] == player && board[2][2][y] == player) return 1;
        if (board[0][2][y] == player && board[1][1][y] == player && board[2][0][y] == player) return 1;
    }

    return 0;
}


// Function to check if the board is full
int isFull() {
    for (int z = 0; z < SIZE; z++) {
        for (int x = 0; x < SIZE; x++) {
            for (int y = 0; y < SIZE; y++) {
                if (board[z][x][y] == EMPTY) {
                    return 0;  // Board is not full
                }
            }
        }
    }
    return 1;  // Board is full
}

// Minimax function to calculate the best move for the AI
int minimax(int depth, int isMaximizing) {
    //chech if the depth limit is reached
    if (depth >=6){
        return 0;
    }
    // Check for terminal conditions (winner or full board)
    if (isWinner(PLAYER_X)) {
        return 10 - depth;
    }
    if (isWinner(PLAYER_O)) {
        return depth - 10;
    }
    if (isFull()) {
        return 0;
    }

    if (isMaximizing) {
        int maxEval = INT_MIN;
        for (int z = 0; z < SIZE; z++) {
            for (int x = 0; x < SIZE; x++) {
                for (int y = 0; y < SIZE; y++) {
                    if (board[z][x][y] == EMPTY) {
                        board[z][x][y] = PLAYER_X;
                        int eval = minimax(depth + 1, 0);
                        board[z][x][y] = EMPTY;
                        maxEval = (eval > maxEval) ? eval : maxEval;
                    }
                }
            }
        }
        return maxEval;
    } else {
        int minEval = INT_MAX;
        for (int z = 0; z < SIZE; z++) {
            for (int x = 0; x < SIZE; x++) {
                for (int y = 0; y < SIZE; y++) {
                    if (board[z][x][y] == EMPTY) {
                        board[z][x][y] = PLAYER_O;
                        int eval = minimax(depth + 1, 1);
                        board[z][x][y] = EMPTY;
                        minEval = (eval < minEval) ? eval : minEval;
                    }
                }
            }
        }
        return minEval;
    }
}

// Function to find the best move for the AI
void bestMove() {
    int bestVal = INT_MIN;
    int move[3] = {-1, -1, -1};  // Stores the best move (z, x, y)

    for (int z = 0; z < SIZE; z++) {
        for (int x = 0; x < SIZE; x++) {
            for (int y = 0; y < SIZE; y++) {
                if (board[z][x][y] == EMPTY) {
                    board[z][x][y] = PLAYER_X;  // Try move
                    int moveVal = minimax(0, 0);
                    board[z][x][y] = EMPTY;  // Undo move

                    if (moveVal > bestVal) {
                        bestVal = moveVal;
                        move[0] = z;
                        move[1] = x;
                        move[2] = y;
                    }
                }
            }
        }
    }

    // Ensure the AI moves exactly where it says
    if (move[0] != -1 && move[1] != -1 && move[2] != -1) {
        printf("AI plays at position (%d, %d, %d)\n", move[0], move[1], move[2]);
        board[move[0]][move[1]][move[2]] = PLAYER_X;  // **Now actually make the move**
    } else {
        printf("AI could not find a valid move.\n");  // Fallback case
    }
}



// Function to play the game
void playGame() {
    initBoard();
    printBoard();

    while (1) {
        // Player's move
        int x, y, z;
        printf("Enter your move (x y z): ");
        if (scanf("%d %d %d", &x, &y, &z) != 3) {
            printf("Invalid input. Please enter three integers.\n");
            while(getchar() != '\n'); // Clear the input buffer
            continue;
        }
        if (x < 0 || x >= SIZE || y < 0 || y >= SIZE || z < 0 || z >= SIZE || board[z][x][y] != EMPTY) {
            printf("Invalid move. Try again.\n");
            continue;
        }
        board[z][x][y] = PLAYER_O;

        printBoard();

        if (isWinner(PLAYER_O)) {
            printf("You win!\n");
            break;
        }

        if (isFull()) {
            printf("It's a draw!\n");
            break;
        }

        // AI's move
        bestMove();
        printBoard();

        if (isWinner(PLAYER_X)) {
            printf("AI wins!\n");
            break;
        }

        if (isFull()) {
            printf("It's a draw!\n");
            break;
        }
    }
}

// Main function to run the game
int main() {
    char playAgain;
    do {
        playGame();
        printf("Do you want to play again? (y/n): ");
        while(getchar() != '\n'); // Clear the input buffer
        scanf("%c", &playAgain);
    } while (playAgain == 'y' || playAgain == 'Y');

    return 0;
}
