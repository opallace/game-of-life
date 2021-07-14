#define WIDTH 800
#define HEIGHT 89
#define SPEED 1

typedef struct{
	int state;
	int neighbors;
} Cell;

typedef struct{
	int width;
	int height;
	int generation;
	Cell **cells;
} Gameboard;

Gameboard* gameboard_init(int width, int height);
void clear_screen();
void print_gameboard(Gameboard *gameboard);
void gotoxy(int x, int y);
int neighbors(Gameboard *gameboard, int x_cell, int y_cell);
int check_if_cell_is_alive(int cell_status);
void update_cells_neighbors(Gameboard *gameboard);
void update_cells_state(Gameboard *gameboard);
int cell_state_next_generation(int cell_state, int quantity_neighbors);
void next_generation(Gameboard *gameboard);
void delay(int milli_seconds);