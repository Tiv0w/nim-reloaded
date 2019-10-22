#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PAWNS 2 //hardcoding at the moment
#define COLS 3
#define ROWS 3


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
void display_board(Cell *board, int cols, int rows);
int nim_add(Cell *board);
void display_nimber(int cols, int rows);
void compute_neighbors(Coord *neighbors, int col, int row, int cols, int rows);
void print_neighbors(Coord *neighbors);
int random_up_to(int upper);


int main() {
    srand(time(NULL));

    Cell *board = (Cell *)malloc((PAWNS + 1) * sizeof(Cell));

    init_board(board, COLS, ROWS, PAWNS);

    printf("contains pawn %d\n", contains_pawn(board, 1, 1));
    printf("contains pawn %d\n", contains_pawn(board, 1, 2));

    display_nimber(COLS, ROWS);

    display_board(board, COLS, ROWS);

    printf("nim-add: %d\n", nim_add(board));

    Coord *neighbors = (Coord *)malloc(sizeof(Coord));
    compute_neighbors(neighbors, 2, 2, COLS, ROWS);
    print_neighbors(neighbors);

    printf("%d\n", random_up_to(8));
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
	board[i].nimber = nimber(1, (i + 1), cols, rows);
	board[i].coordinates = (Coord){.x = 1, .y = i + 1};
    }
    board[i] = (Cell){.nimber = -1, (Coord){.x = -1, .y = -1}};
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

/* Displays the current board */
void display_board(Cell *board, int cols, int rows){
    // display a nice separation between rows
    char *separation;
    separation = (char *)malloc((cols * 4 + 1) * sizeof(char));
    memset(separation, '-', (cols * 4 + 1));
    //

    printf("\n%s\n", separation);
    for (int i = 1; i <= rows; i++) {
	printf("|");
	for (int j = 1; j <= cols; j++) {
	    int pawns_number = contains_pawn(board, j, i);
	    if (pawns_number)
		printf(" %d |", pawns_number);
	    else
		printf("   |");
	}
	printf("\n%s\n", separation);
    }
}

/* Compute the nimber of a cell.
 * Depends on the total number of columns and of rows
 */
int nimber(int col, int row, int cols, int rows){
    return ((rows - row) % 3) ^ ((cols - col) % 3);
}

/* Compute the nim-addition of all the occupied cells */
int nim_add(Cell *board){
    int total = 0;
    int i = 0;
    while (board[i].nimber != -1) {
	total = total ^ board[i].nimber;
	i++;
    }
    return total;
}

/* Displays the current board with the nimbers
 * ONLY FOR DEBUGGING PURPOSES
 */
void display_nimber(int cols, int rows){
    // display a nice separation between rows
    char *separation;
    separation = (char *)malloc((cols * 4 + 1) * sizeof(char));
    memset(separation, '-', (cols * 4 + 1));
    //

    printf("\n%s\n", separation);
    for (int i = 1; i <= rows; i++) {
	printf("|");
	for (int j = 1; j <= cols; j++) {
	    printf(" %d |", nimber(j, i, cols, rows));
	}
	printf("\n%s\n", separation);
    }
}

/* Create the array of neighbors cells from the given cell */
void compute_neighbors(Coord *neighbors, int col, int row, int cols, int rows){
    int index = 0;
    int col_gap = cols - col;
    int row_gap = rows - row;

    if (col_gap > 0)
	neighbors[index++] = (Coord){.x = (col + 1), .y = row};
    if (col_gap > 1)
	neighbors[index++] = (Coord){.x = (col + 2), .y = row};
    if (row_gap > 0)
	neighbors[index++] = (Coord){.x = col, .y = (row + 1)};
    if (row_gap > 1)
	neighbors[index++] = (Coord){.x = col, .y = (row + 2)};

    neighbors[index] = (Coord){.x = -1, .y = -1};
}

/* Displays the neighbors
 * ONLY FOR DEBUGGING PURPOSES
 */
void print_neighbors(Coord *neighbors){
    int i = 0;
    while (neighbors[i].x != -1) {
	printf("neighbor[%d]: %d, %d\n", i, neighbors[i].x, neighbors[i].y);
	i++;
    }
}

/* Generate a random number between 1 and the upper bound (included) */
int random_up_to(int upper){
    return rand() % (upper - 1) + 1;
}
