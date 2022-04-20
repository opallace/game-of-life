#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <GL/glut.h>

#include "gameoflife.h"

GLint FPS = 25;
GLint window_width  = 666;
GLint window_height = 666;
GLfloat left   = 0.0;
GLfloat right  = 1.0; //largura relativa
GLfloat bottom = 0.0; //altura relativa
GLfloat top    = 1.0;
GLint game_width  = 400; //quantidade de peças na horizontal
GLint game_height = 400; //quantidade de peças na vertical

Gameboard *gameboard;

void MyInit(){
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

void RenderScene(){
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat xSize = (right - left) / game_width;
	GLfloat ySize = (top - bottom) / game_height;

	glBegin(GL_QUADS);
		
	
		GLint x = 0;
		GLint y = 0;
		for (GLint x = 0; x < game_width; ++x) {
			for (GLint y = 0; y < game_height; ++y) {
				if(check_if_cell_is_alive(gameboard->cells[x][y].state)){
					glColor3f(0.0f, 0.0f, 0.0f);
				
				}else {
					glColor3f(1.0f, 1.0f, 1.0f);
					
				}

				glVertex2f(    x*xSize,    y*ySize);
				glVertex2f((x+1)*xSize,    y*ySize);
				glVertex2f((x+1)*xSize,(y+1)*ySize);
				glVertex2f(    x*xSize,(y+1)*ySize);

			}
		}
		
	glEnd();

	glutSwapBuffers();
	
}


void Reshape(int w, int h) {
	window_width  = w;
	window_height = h;

	glViewport(0, 0, window_width, window_height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(left, right, bottom, top);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glutPostRedisplay();
}

void Update(int value) {

	next_generation(gameboard);

	glutPostRedisplay();
	glutTimerFunc(1000 / FPS, Update, 0);
}

int main(int argc, char **argv){

	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(window_width, window_height);	
	glutCreateWindow("Game of life by Conway");
	
	glutReshapeFunc(Reshape);
	glutDisplayFunc(RenderScene);
	
	gameboard = gameboard_init(game_width, game_height);

	MyInit();
	Update(0);
	glutMainLoop();

	
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
