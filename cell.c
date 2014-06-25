#include <stdlib.h>
#include <gtk/gtk.h>
#include <time.h>
#include "interface.h"
#include "main.h"
#include "cell.h"

void initializeCells(int ***cells, int x, int y) {
  int i;
  *cells = (int **)malloc(x * sizeof(int *));
  for(i=0; i<x; i++) {
    (*cells)[i] = (int *)calloc(y, sizeof(int));
  }
}

void drawCell(cairo_t *cr, int x, int y, int state, int size) {
	GdkRGBA color = {0.1, 0.1, 0.1, 1.0};
	switch(TYPE(state)) {
		case TYPE1:
			color.red = 0.6;
		break;
		case TYPE2:
			color.green = 0.6;
		break;
		case TYPE3:
			color.blue = 0.6;
		break;
		case TYPE4:
			color.red = 0.6;
			color.green = 0.6;
		break;
		case TYPE5:
			color.green = 0.6;
			color.blue = 0.6;
		break;
	}
  gdk_cairo_set_source_rgba (cr, &color);
	draw_cir(cr, (x+1) * (size + CELL_SEPARATION), (y+1) * (size + CELL_SEPARATION), size);
	cairo_fill (cr);
}

void eraseCell(cairo_t *cr, int x, int y, int size) {
	GdkRGBA color = {0, 0, 0, 1.0};
  gdk_cairo_set_source_rgba (cr, &color);
	draw_cir(cr, x*(size + CELL_SEPARATION), y*(size + CELL_SEPARATION), size+1);
	cairo_fill (cr);
}

void calculateCell(int **cells, int **buff, int x, int y) {
  if(DIVIDE_CELLS) {
    if(DIVISION_TIME(cells[x][y]) == 1) {
      if(DIVISIONS_LEFT(cells[x][y]) > 0) {
        reproduceCell(x, y, cells, buff);
      } else {
        cells[x][y] = 0;
        return;
      }
    } else {
      cells[x][y] = SUB_DIVISION_TIME(cells[x][y]);
    }
  }

	int target_y, target_x;
	switch(DIRECTION(cells[x][y])) {
		case MOVING_UP:
			if(y > 0) {
				target_y = y - 1;
			} else {
				cells[x][y] = CHANGE_DIRECTION(cells[x][y], MOVING_DOWN);
				target_y = y + 1;
			}
			moveCell(x, y, x, target_y, cells, buff);
		break;
		case MOVING_UP_R:
			if(y > 0 && x + 1 < MODEL_SIZE_X) {
				target_y = y - 1; target_x = x + 1;
			} else if(y <= 0 && x+1 >= MODEL_SIZE_X) {
				cells[x][y] = CHANGE_DIRECTION(cells[x][y], MOVING_DOWN_L);
				target_y = y + 1;  target_x = x - 1;
			} else if(y <= 0){
				cells[x][y] = CHANGE_DIRECTION(cells[x][y], MOVING_DOWN_R);
				target_y = y + 1;  target_x = x + 1;
			} else {
				cells[x][y] = CHANGE_DIRECTION(cells[x][y], MOVING_UP_L);
				target_y = y - 1;  target_x = x - 1;
			}
			moveCell(x, y, target_x, target_y, cells, buff);
		break;
		case MOVING_UP_L:
			if(y > 0 && x > 0) {
				target_y = y - 1; target_x = x - 1;
			} else if(y <= 0 && x <= 0) {
				cells[x][y] = CHANGE_DIRECTION(cells[x][y], MOVING_DOWN_R);
				target_y = y + 1;  target_x = x + 1;
			} else if(y <= 0){
				cells[x][y] = CHANGE_DIRECTION(cells[x][y], MOVING_DOWN_L);
				target_y = y + 1;  target_x = x - 1;
			} else {
				cells[x][y] = CHANGE_DIRECTION(cells[x][y], MOVING_UP_R);
				target_y = y - 1;  target_x = x + 1;
			}
			moveCell(x, y, target_x, target_y, cells, buff);
		break;
		case MOVING_L:
			if(x > 0) {
				target_x = x - 1;
			} else {
				cells[x][y] = CHANGE_DIRECTION(cells[x][y], MOVING_R);
				target_x = x + 1;
			}
			moveCell(x, y, target_x % MODEL_SIZE_X, y, cells, buff);
		break;
		case MOVING_R:
			if(x+1 < MODEL_SIZE_X) {
				target_x = x + 1;
			} else {
				cells[x][y] = CHANGE_DIRECTION(cells[x][y], MOVING_L);
				target_x = x - 1;
			}
			moveCell(x, y, target_x, y, cells, buff);
		break;
		case MOVING_DOWN:
			if(y+1 < MODEL_SIZE_Y) {
				target_y = y + 1;
			} else {
				cells[x][y] = CHANGE_DIRECTION(cells[x][y], MOVING_UP);
				target_y = y - 1;
			}
			moveCell(x, y, x, target_y, cells, buff);
		break;
		case MOVING_DOWN_R:
			if(y+1 < MODEL_SIZE_Y && x+1 < MODEL_SIZE_X) {
				target_y = y + 1; target_x = x + 1;
			} else if(y+1 >= MODEL_SIZE_Y && x+1 >= MODEL_SIZE_X) {
				cells[x][y] = CHANGE_DIRECTION(cells[x][y], MOVING_UP_L);
				target_y = y - 1;  target_x = x - 1;
			} else if(y+1 >= MODEL_SIZE_Y){
				cells[x][y] = CHANGE_DIRECTION(cells[x][y], MOVING_UP_R);
				target_y = y - 1;  target_x = x + 1;
			} else {
				cells[x][y] = CHANGE_DIRECTION(cells[x][y], MOVING_DOWN_L);
				target_y = y + 1;  target_x = x - 1;
			}
			moveCell(x, y, target_x, target_y, cells, buff);
		break;
		case MOVING_DOWN_L:
			if(y+1 < MODEL_SIZE_Y && x > 0) {
				target_y = y + 1; target_x = x - 1;
			}	else if(y+1 >= MODEL_SIZE_Y && x <= 0) {
				cells[x][y] = CHANGE_DIRECTION(cells[x][y], MOVING_UP_R);
				target_y = y - 1;  target_x = x + 1;
			} else if(y+1 >= MODEL_SIZE_Y){
				cells[x][y] = CHANGE_DIRECTION(cells[x][y], MOVING_UP_L);
				target_y = y - 1;  target_x = x - 1;
			} else {
				cells[x][y] = CHANGE_DIRECTION(cells[x][y], MOVING_DOWN_R);
				target_y = y + 1;  target_x = x + 1;
			}
			moveCell(x, y, target_x, target_y, cells, buff);
		break;
		case STATIONARY:
			moveCell(x, y, x, y, cells, buff);
			//buff[(x-1+MODEL_SIZE_X) % MODEL_SIZE_X][(y+1) % MODEL_SIZE_Y] = cells[x][y];
		break;
	}
	//cells[x][y] = 0;
}

void changeStateCell(int state, int x, int y, int **buff) {
	buff[x][y] = state;
}

void createCell(int state, int x, int y, int **buff) {
  if(x > -1  && y > -1 && x < MODEL_SIZE_X && y < MODEL_SIZE_Y) {
    printf("Created cell at [%i, %i]: %i\n", x, y, DIRECTION(state));
    buff[x][y] = state;
  }
}

void moveCell(int fromX, int fromY, int toX, int toY, int **cells, int **buff) {
	int tmp;
	//sprawdzamy czy komorka rekurencyjnie nie zostala przemieszczona przez inna kolizje zagniezdzona
	if(cells[fromX][fromY] != 0) {
		//brak zderzenia
		if(buff[toX][toY] == 0 && cells[toX][toY] == 0) {
			buff[toX][toY] = cells[fromX][fromY];
			cells[fromX][fromY] = 0;
		//wystapilo zderzenie
		} else {
			//brak ruchu - komorka stacjonarna
			if(DIRECTION(cells[fromX][fromY]) == STATIONARY) {
				//printf("stationary \n");
				buff[toX][toY] = cells[fromX][fromY];
				cells[fromX][fromY] = 0;
				//zderzenie czolowe do tej samej komorki x->o<-x
			} else if(buff[toX][toY] != 0 && DIRECTION(buff[toX][toY]) != STATIONARY && 
					abs(DIRECTION(buff[toX][toY]) - DIRECTION(cells[fromX][fromY])) == 4){
				calculateHeadCollisionFar(fromX, fromY, toX, toY, cells, buff);
				//zderzenie czolowe do roznych komorek x-><-x
			} else if((cells[toX][toY] != 0 && DIRECTION(cells[toX][toY]) != STATIONARY &&
					abs(DIRECTION(cells[toX][toY]) - DIRECTION(cells[fromX][fromY])) == 4)) {
				printf("zderzenie czolowe do roznych komorek [%i, %i] wektor %i z [%i, %i] wektor %i lub %i\n", fromX, fromY, DIRECTION(cells[fromX][fromY]),
				 toX, toY, DIRECTION(cells[toX][toY]), DIRECTION(buff[toX][toY]));
				calculateHeadCollisionNear(fromX, fromY, toX, toY, cells, buff);
				//zderzenie pod katem prostym na komorke wchodzaca	x->o
				//																										 ^x
			} else if(DIRECTION(buff[toX][toY]) != 0 && DIRECTION(buff[toX][toY]) != STATIONARY && 
					((DIRECTION(buff[toX][toY]) % 8) + 2 == DIRECTION(cells[fromX][fromY]) || (DIRECTION(cells[fromX][fromY]) % 8) + 2 == DIRECTION(buff[toX][toY]))) {
				calculateCollisionFar(fromX, fromY, toX, toY, cells, buff);
			//wepchniecie w tym samym kierunku z innego zdarzenia - nie ruszaj czastki po zderzeniu
			} else if(DIRECTION(cells[fromX][fromY]) == DIRECTION(buff[toX][toY])) {
				if(buff[fromX][fromY] == 0) {
					buff[fromX][fromY] = cells[fromX][fromY];
					cells[fromX][fromY] = 0;
				} else {
					int rnd = (rand()%8)+2;
					cells[fromX][fromY] = CHANGE_DIRECTION(cells[fromX][fromY], rnd);
					calculateCell(cells, buff, fromX, fromY);
					if(DIRECTION(cells[fromX][fromY]) == rnd) {
						cells[fromX][fromY] = 0;
					}
				}
				//pozostale - jednoczesne wejscie na komorke
			} else if(buff[toX][toY] != 0){
				calculateCollisionFar(fromX, fromY, toX, toY, cells, buff);
			//pozostale - zderzenie z wychodzaca komorka
			} else if(DIRECTION(cells[fromX][fromY]) != DIRECTION(cells[toX][toY])){
				tmp = DIRECTION(cells[toX][toY]);
				cells[toX][toY] = CHANGE_DIRECTION(cells[toX][toY], DIRECTION(cells[fromX][fromY]));
				cells[fromX][fromY] = CHANGE_DIRECTION(cells[fromX][fromY], OPPOSITE_DIRECTION(tmp));
				calculateCell(cells, buff, toX, toY);
				if(cells[toX][toY] == 0 && cells[fromX][fromY] != 0 && DIRECTION(cells[fromX][fromY]) == OPPOSITE_DIRECTION(tmp)) {
					cells[toX][toY] = cells[fromX][fromY];
					cells[fromX][fromY] = 0;
					calculateCell(cells, buff, toX, toY);
					cells[toX][toY] = 0;
				}		
			} else {
				calculateCell(cells, buff, toX, toY);
				cells[toX][toY] = 0;
				buff[toX][toY] = cells[fromX][fromY];
				cells[fromX][fromY] = 0;
			}
		}
	}
}

void reproduceCell(int x, int y, int **cells, int **buff) {
	int newX, newY, startX, startY, i, j;
	cells[x][y] = SUB_DIVISIONS_LEFT(SET_DIVISION_TIME(cells[x][y], REPRODUCTION_INTERVAL));
	int cell = cells[x][y];
	if(!ONLY_STATIONARY) {
		cell = CHANGE_DIRECTION(cells[x][y], rand()%8+2);
	}

	startX = rand()%3;
	startY = rand()%3;
	for(i=0; i<3; i++) {
		for(j=0; j<3; j++) {
			newX = (startX + i) % 3;
			newY = (startY + j) % 3;
			if(x + newX > 0 && y + newY > 0 &&
			 		x + newX < MODEL_SIZE_X - 1 &&
			  	y + newY < MODEL_SIZE_Y - 1 &&
			  	cells[x + newX - 1][y + newY - 1] == 0) {
				createCell(cell, x + newX - 1, y + newY - 1, cells);
				return;
			}
		}
	}
}

void calculateHeadCollisionFar(int fromX, int fromY, int toX, int toY, int **cells, int **buff) {
	int tmp, dirChange, rnd = rand() % 75;
	if(rnd < 15) {
		buff[fromX][fromY] = CHANGE_DIRECTION(cells[fromX][fromY], OPPOSITE_DIRECTION(DIRECTION(cells[fromX][fromY])));
		cells[fromX][fromY] = 0;
		cells[toX][toY] = CHANGE_DIRECTION(buff[toX][toY], OPPOSITE_DIRECTION(DIRECTION(buff[toX][toY])));
		buff[toX][toY] = 0;
		calculateCell(cells, buff, toX, toY);
		cells[toX][toY] = 0;
	} else if(rnd < 45) {
		dirChange = 7 - (rnd % 2) * 2;
		buff[fromX][fromY] = CHANGE_DIRECTION(cells[fromX][fromY], ((OPPOSITE_DIRECTION(DIRECTION(cells[fromX][fromY])) + dirChange) % 8) + 2);
		cells[fromX][fromY] = 0;
		cells[toX][toY] = CHANGE_DIRECTION(buff[toX][toY], ((OPPOSITE_DIRECTION(DIRECTION(buff[toX][toY])) + dirChange) % 8) + 2);
		buff[toX][toY] = 0;
		tmp = cells[fromX][fromY];
		calculateCell(cells, buff, toX, toY);
		cells[toX][toY] = 0;
		if(cells[toX][toY] == 0 && cells[fromX][fromY] != 0 && cells[fromX][fromY] == tmp) {
			cells[toX][toY] = cells[fromX][fromY];
			cells[fromX][fromY] = 0;
			calculateCell(cells, buff, toX, toY);
		}
	} else {
		dirChange = 8 - (rnd % 2) * 4;
		buff[fromX][fromY] = CHANGE_DIRECTION(cells[fromX][fromY], ((OPPOSITE_DIRECTION(DIRECTION(cells[fromX][fromY])) + dirChange) % 8) + 2);
		cells[fromX][fromY] = 0;
		cells[toX][toY] = CHANGE_DIRECTION(buff[toX][toY], ((OPPOSITE_DIRECTION(DIRECTION(buff[toX][toY])) + dirChange) % 8) + 2);
		buff[toX][toY] = 0;
		tmp = cells[fromX][fromY];
		calculateCell(cells, buff, toX, toY);
		cells[toX][toY] = 0;
		if(cells[toX][toY] == 0 && cells[fromX][fromY] != 0 && cells[fromX][fromY] == tmp) {
			cells[toX][toY] = cells[fromX][fromY];
			cells[fromX][fromY] = 0;
			calculateCell(cells, buff, toX, toY);
		}
	}
}

void calculateCollisionFar(int fromX, int fromY, int toX, int toY, int **cells, int **buff) {
	int tmp, dir, rnd = rand() % 90;
  tmp = DIRECTION(buff[toX][toY]);
  if (rnd < 30 && !OPPOSITE_DIRECTION(DIRECTION(cells[fromX][fromY]) == ((tmp - 1) % 8) + 2) && abs(DIRECTION(cells[fromX][fromY]) - tmp) != 1) {
    cells[toX][toY] = CHANGE_DIRECTION(buff[toX][toY], ((DIRECTION(cells[fromX][fromY]) - 1) % 8) + 2);
    cells[fromX][fromY] = CHANGE_DIRECTION(cells[fromX][fromY], ((tmp - 1) % 8) + 2);
	} else if(rnd < 60 && !OPPOSITE_DIRECTION(DIRECTION(cells[fromX][fromY])== ((tmp + 5) % 8) + 2)  && abs(DIRECTION(cells[fromX][fromY]) - tmp) != 1) {
    cells[toX][toY] = CHANGE_DIRECTION(buff[toX][toY], ((DIRECTION(cells[fromX][fromY]) + 5) % 8) + 2);
    cells[fromX][fromY] = CHANGE_DIRECTION(cells[fromX][fromY], ((tmp + 5) % 8) + 2);
  } else {
    cells[toX][toY] = CHANGE_DIRECTION(buff[toX][toY], DIRECTION(cells[fromX][fromY]));
    cells[fromX][fromY] = CHANGE_DIRECTION(cells[fromX][fromY], tmp);
  }
  buff[toX][toY] = 0;
  calculateCell(cells, buff, toX, toY);
  //jezeli inne zderzenie nie przemiescilo juz dana komorke
  if(cells[toX][toY] == 0 && cells[fromX][fromY] != 0 && DIRECTION(cells[fromX][fromY]) == tmp) {
    cells[toX][toY] = cells[fromX][fromY];
    cells[fromX][fromY] = 0;
    calculateCell(cells, buff, toX, toY);
    // cells[toX][toY] = 0;
  }
}

void calculateHeadCollisionNear(int fromX, int fromY, int toX, int toY, int **cells, int **buff) {
	int rnd = rand() % 100;
	if(rnd < PREDICTIVE_BOUNCE_CHANCE) {
		cells[fromX][fromY] = CHANGE_DIRECTION(cells[fromX][fromY], OPPOSITE_DIRECTION(DIRECTION(cells[fromX][fromY])));
		cells[toX][toY] = CHANGE_DIRECTION(cells[toX][toY], OPPOSITE_DIRECTION(DIRECTION(cells[toX][toY])));
		calculateCell(cells, buff, fromX, fromY);
		cells[fromX][fromY] = 0;
		calculateCell(cells, buff, toX, toY);
		cells[toX][toY] = 0;
	} else {
		cells[fromX][fromY] = CHANGE_DIRECTION(cells[fromX][fromY], ((DIRECTION(cells[fromX][fromY]) + ((rnd % 2) * 4)) % 8) + 2);
		cells[toX][toY] = CHANGE_DIRECTION(cells[toX][toY], ((DIRECTION(cells[toX][toY]) + ((rnd % 2) * 4)) % 8) + 2);
		calculateCell(cells, buff, fromX, fromY);
		cells[fromX][fromY] = 0;
		calculateCell(cells, buff, toX, toY);
		cells[toX][toY] = 0;
	}
}
