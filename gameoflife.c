#include <stdio.h>
#include <stdlib.h>
#include <time.h>
 #include <unistd.h>

#include "gameoflife.h"

int main(){
	Gameboard *gameboard = gameboard_init(WIDTH, HEIGHT);
	
	clear_screen();
	print_gameboard(gameboard);
	
	
	while(1){
		next_generation(gameboard);
		print_gameboard(gameboard);
		sleep(SPEED);
	}
	
	return 0;
}

Gameboard* gameboard_init(int width, int height){
	srand(time(NULL));
	Gameboard *gameboard = malloc(sizeof(Gameboard));
	gameboard->width = width;
	gameboard->height = height;
	gameboard->generation = 0;
	
	gameboard->cells = malloc(sizeof(Cell *) * height);
	
	for(int x = 0; x < height; x++){
		gameboard->cells[x] = malloc(sizeof(Cell) * width);
		
		for(int y = 0; y < width; y++){
			if(x == 0 || y == 0 || x == height - 1 || y == width - 1){
				gameboard->cells[x][y].state = 0;
			}else {
				gameboard->cells[x][y].state = rand() % 2;
			}
		}
	}
	
	return gameboard;
}

void clear_screen(){
	system("clear");
}

void print_gameboard(Gameboard *gameboard){
	
	for(int x = 0; x < gameboard->height; x++){
		for(int y = 0; y < gameboard->width; y++){
			gotoxy(x, y);
		
			if(check_if_cell_is_alive(gameboard->cells[x][y].state)){
				printf("*");
			
			}else {
				printf(" ");
				
			}
		}printf("\n");
	}
}

void gotoxy(int x, int y){
  	printf("%c[%d;%df", 0x1B, x, y);
}

int check_if_cell_is_alive(int cell_status){
	if(cell_status == 1){
		return 1;
	}
	
	return 0;
}

void update_cells_neighbors(Gameboard *gameboard){
	for(int x = 1; x < gameboard->height - 1; x++){
		for(int y = 1; y < gameboard->width - 1; y++){

			gameboard->cells[x][y].neighbors = 0;
					
			for(int i = -1; i <= 1; i++){
				for(int j = -1; j <= 1; j++){
					
					if(check_if_cell_is_alive(gameboard->cells[x + i][y + j].state)){
						gameboard->cells[x][y].neighbors++;
					}
						
				}
			}
			
			gameboard->cells[x][y].neighbors -= gameboard->cells[x][y].state;
		}
	}
}

void update_cells_state(Gameboard *gameboard){

	for(int x = 1; x < gameboard->height - 1; x++){
		for(int y = 1; y < gameboard->width - 1; y++){

			int new_cell_state = cell_state_next_generation(gameboard->cells[x][y].state, gameboard->cells[x][y].neighbors);
			gameboard->cells[x][y].state = new_cell_state;

		}
	}

}


int cell_state_next_generation(int cell_state, int quantity_neighbors){
	
	if(quantity_neighbors < 2 || quantity_neighbors > 3){
		return 0;
		
	}else if(quantity_neighbors == 3){
		return 1;
		
	}else {
		return cell_state;
		
	}
	
}

void next_generation(Gameboard *gameboard){	
	
	update_cells_neighbors(gameboard);
	update_cells_state(gameboard);

	
}

void delay(int milli_seconds){
    clock_t start_time = clock();
    
    while (clock() < start_time + milli_seconds);
}
