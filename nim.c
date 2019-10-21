#include <stdio.h>
#include <stdlib.h>


typedef struct Coord {
    int x;
    int y;
} Coord;

typedef struct Cell {
    int nimber;
    Coord coordinates;
} Cell;


int read_int_in_bounds(int lower, int upper);
void read_game_params(int *rows, int *cols, int *pawns, int *first, int *level);
int nimber(int col, int row, int cols, int rows);
void init_board(Cell *board, int cols, int rows, int pawns);
int contains_pawn(Cell *board, int col, int row);


int main() {
    Cell *board = (Cell *)malloc(sizeof(Cell));

    init_board(board, 3, 3, 2);

    printf("contains pawn %d\n", contains_pawn(board, 1, 1));
    printf("contains pawn %d\n", contains_pawn(board, 1, 2));
    return 0;
}


/* Reads an int and returns it if in the bounds. */
int read_int_in_bounds(int lower, int upper){
    int n = 0;
    scanf("%d", &n);
    while (n < lower || n > upper) {
	printf("Out of bounds, enter another: ");
	scanf("%d", &n);
    }
    return n;
}

/* Reads all the game parameters. */
void read_game_params(int *rows, int *cols, int *pawns, int *first, int *level){
    printf("Game parameters\n ------------- \n");
    printf("Rows: (3-30) ");
    *rows = read_int_in_bounds(3, 30);
    printf("Columns: (3-30) ");
    *cols = read_int_in_bounds(3, 30);
    printf("Pawns: (3-30) ");
    *pawns = read_int_in_bounds(1, *rows);
    printf("Level of difficulty: (1-3) ");
    *level = read_int_in_bounds(1, 3);
    printf("Who starts? (1) for computer, (2) for player: ");
    *first = read_int_in_bounds(1, 2);
    printf(" GO ! \n");
}

/* Initialize an new board with the starting pawns. */
void init_board(Cell *board, int cols, int rows, int pawns){
    int i;
    for (i = 0; i < pawns; i++) {
	board[i].nimber = nimber(1, i, cols, rows);
	board[i].coordinates = (Coord){.x = 1, .y = i + 1};
    }
    board[++i] = (Cell){.nimber = -1, (Coord){.x = -1, .y = -1}};
}

/* Returns the number of pawns a cell contains. */
int contains_pawn(Cell *board, int col, int row){
    int contains = 0;
    int i = 0;
    while (board[i].nimber != -1) {
	if (board[i].coordinates.x == col && board[i].coordinates.y == row)
	    contains++;
	i++;
    }
    return contains;
}

/* Compute the nimber of a cell.
 * Depends on the total number of columns and of rows
 */
int nimber(int col, int row, int cols, int rows){
    return ((rows - row) % 3) ^ ((cols - col) % 3);
}
