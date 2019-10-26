#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PAWNS 1 //hardcoding at the moment
#define COLS 3
#define ROWS 3
#define FIRST 2
#define LEVEL 3


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
int update_board(Cell *board, int pawn_index, Coord new_position, int cols, int rows);
int player_move(Cell *board, int cols, int rows);
int random_move(Cell *board, int cols, int rows);
int winning_move(Cell *board, int cols, int rows);
int bot_move(Cell *board, int cols, int rows, int difficulty);


int main() {
    int state = 1;
    int cols, rows, pawns, first, level;
    srand(time(NULL));

    read_game_params(&cols, &rows, &pawns, &first, &level);

    Cell *board = (Cell *)malloc((pawns + 1) * sizeof(Cell));
    init_board(board, cols, rows, pawns);

    /* display_nimber(cols, rows); */

    display_board(board, cols, rows);
    while (state != 0) {
    	if (first == 1) {
    	    state = bot_move(board, cols, rows, level);
    	    first = 2;
    	} else {
    	    state = player_move(board, cols, rows);
    	    first = 1;
    	}
    	display_board(board, cols, rows);
    }
    if (first == 1)
	printf("\n\nYou just won!\n-------------\n\n");
    else
	printf("\n\nThe machine was better than you!\n--------------------------------\n\n");

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
    printf("Pawns: (1-%d) ", *rows);
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

/* Generate a random number between 0 and the upper bound (excluded) */
int random_up_to(int upper){
    if (upper < 1)
	return 0;
    return rand() % upper;
}

/* Updates the board depending on the current move.
 * Takes the index of the moving pawn, the new position as Coord,
 * the total columbs and rows
 */
int update_board(Cell *board, int pawn_index, Coord new_position, int cols, int rows){
    board[pawn_index].coordinates = new_position;
    board[pawn_index].nimber = nimber(new_position.x, new_position.y, cols, rows);

    if (contains_pawn(board, cols, rows)) {
	int i = pawn_index;
	while (board[i].nimber != -1) {
	    board[i] = board[i+1];
	    i++;
	}
	board[i] = (Cell){.nimber = -1, (Coord){.x = -1, .y = -1}};
    }

    if (board[0].nimber == -1) {
	printf("End\n");
	return 0;
    }
    return 1;
}

/* Let the player select the move */
int player_move(Cell *board, int cols, int rows){
    int i, selected_pawn, selected_neighbor;
    Coord pawn_coord, neighbor_coord;
    Coord *neighbors = (Coord *)malloc(5 * sizeof(Coord));

    printf("\n\nYour turn!\nSelect a pawn: ");
    for (i = 0; board[i].nimber != -1; i++)
	printf("%d:(%d, %d) ", i, board[i].coordinates.x, board[i].coordinates.y);
    do {
	printf("\n---> ");
	scanf("%d", &selected_pawn);
	if (selected_pawn < 0 || selected_pawn >= i)
	    printf("Invalid selection!\n");
    } while (selected_pawn < 0 || selected_pawn >= i);

    pawn_coord = board[selected_pawn].coordinates;
    compute_neighbors(neighbors, pawn_coord.x, pawn_coord.y, cols, rows);

    printf("\nSelect a destination: ");
    for (i = 0; neighbors[i].x != -1; i++)
	printf("%d:(%d, %d)  ", i, neighbors[i].x, neighbors[i].y);
    do {
	printf("\n---> ");
	scanf("%d", &selected_neighbor);
	if (selected_neighbor < 0 || selected_neighbor >= i)
	    printf("Invalid selection!\n");
    } while (selected_neighbor < 0 || selected_neighbor >= i);

    neighbor_coord = neighbors[selected_neighbor];
    return update_board(board, selected_pawn, neighbor_coord, cols, rows);
}

/* Makes a random move */
int random_move(Cell *board, int cols, int rows){
    int i, selected_pawn, selected_neighbor;
    Coord pawn_coord, neighbor_coord;
    Coord *neighbors = (Coord *)malloc(5 * sizeof(Coord));

    for (i = 0; board[i].nimber != -1; i++);
    selected_pawn = random_up_to(i);
    pawn_coord = board[selected_pawn].coordinates;
    compute_neighbors(neighbors, pawn_coord.x, pawn_coord.y, cols, rows);

    for (i = 0; neighbors[i].x != -1; i++);
    selected_neighbor = random_up_to(i);
    neighbor_coord = neighbors[selected_neighbor];
    return update_board(board, selected_pawn, neighbor_coord, cols, rows);
}

/* Makes a winning move, and if none is available, a random move */
int winning_move(Cell *board, int cols, int rows){
    int i, j;
    Coord pawn_coord, neighbor_coord;
    Coord *neighbors = (Coord *)malloc(5 * sizeof(Coord));
    int nim_total = nim_add(board);

    for (i = 0; board[i].nimber != -1; i++) {
	pawn_coord = board[i].coordinates;
	compute_neighbors(neighbors, pawn_coord.x, pawn_coord.y, cols, rows);

	for (j = 0; neighbors[j].x != -1; j++) {
	    neighbor_coord = neighbors[j];
	    int new_nimber = nimber(neighbor_coord.x, neighbor_coord.y, cols, rows);
	    if (new_nimber == (board[i].nimber ^ nim_total))
		return update_board(board, i, neighbor_coord, cols, rows);
	}
    }
    return random_move(board, cols, rows);
}

/* Makes a bot_move, depending on the difficulty */
int bot_move(Cell *board, int cols, int rows, int difficulty){
    switch (difficulty) {
    case 1:
	if (random_up_to(100) < 10)
	    return winning_move(board, cols, rows);
	else
	    return random_move(board, cols, rows);
	break;
    case 2:
	if (random_up_to(100) < 50)
	    return winning_move(board, cols, rows);
	else
	    return random_move(board, cols, rows);
	break;
    case 3:
	if (random_up_to(100) < 90)
	    return winning_move(board, cols, rows);
	else
	    return random_move(board, cols, rows);
	break;
    default:
	printf("Problem, the level is not valid.");
	return 2;
    }
}
