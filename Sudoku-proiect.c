#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 9

void printBoard(int board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        if (i % 3 == 0 && i != 0) {
            printf("---------------------\n");
        }
        for (int j = 0; j < SIZE; j++) {
            if (j % 3 == 0 && j != 0) {
                printf("| ");
            }
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
}

int isValidInRow(int board[SIZE][SIZE], int row, int num) {
    for (int col = 0; col < SIZE; col++) {
        if (board[row][col] == num) {
            return 0;
        }
    }
    return 1;
}

int isValidInCol(int board[SIZE][SIZE], int col, int num) {
    for (int row = 0; row < SIZE; row++) {
        if (board[row][col] == num) {
            return 0;
        }
    }
    return 1;
}

int isValidInBox(int board[SIZE][SIZE], int startRow, int startCol, int num) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i + startRow][j + startCol] == num) {
                return 0;
            }
        }
    }
    return 1;
}

int isValidMove(int board[SIZE][SIZE], int row, int col, int num) {
    return isValidInRow(board, row, num) &&
           isValidInCol(board, col, num) &&
           isValidInBox(board, row - row % 3, col - col % 3, num);
}

int solveSudoku(int board[SIZE][SIZE], int row, int col) {
    if (row == SIZE) return 1;
    if (col == SIZE) return solveSudoku(board, row + 1, 0);
    if (board[row][col] != 0) return solveSudoku(board, row, col + 1);
    
    for (int num = 1; num <= 9; num++) {
        if (isValidMove(board, row, col, num)) {
            board[row][col] = num;
            if (solveSudoku(board, row, col + 1)) return 1;
            board[row][col] = 0;
        }
    }
    return 0;
}

void generateSudoku(int board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = 0;
        }
    }
    solveSudoku(board, 0, 0);
}

void removeNumbers(int board[SIZE][SIZE], int difficulty) {
    int count = difficulty;
    while (count > 0) {
        int row = rand() % SIZE;
        int col = rand() % SIZE;
        if (board[row][col] != 0) {
            board[row][col] = 0;
            count--;
        }
    }
}

void userMove(int board[SIZE][SIZE]) {
    int row, col, num;
    while (1) {
        printf("Introduceti rand, coloana si numar (1-9) sau 0 0 0 pentru a iesi: ");
        scanf("%d %d %d", &row, &col, &num);
        
        if (row == 0 && col == 0 && num == 0) break;
        
        if (row < 1 || row > 9 || col < 1 || col > 9 || num < 1 || num > 9) {
            printf("Valori invalide! Incercati din nou.\n");
            continue;
        }
        
        row--; col--;
        
        if (board[row][col] == 0 && isValidMove(board, row, col, num)) {
            board[row][col] = num;
            printBoard(board);
        } else {
            printf("Mutare invalida! Incercati din nou.\n");
        }
    }
}

int main() {
    srand(time(NULL));
    
    int board[SIZE][SIZE];
    generateSudoku(board);
    removeNumbers(board, 40); 
    
    printf("Tabla initiala:\n");
    printBoard(board);
    
    userMove(board);
    
    return 0;
}
