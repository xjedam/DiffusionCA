#include <stdlib.h>
#include <gtk/gtk.h>
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
	//draw_sq(cr, x, y, size);
	//printf("%i %i\n", x, y);
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
	//printf("%i %i\n", x, y);
	switch(cells[x][y]) {
		case MOVING_UP:
			moveCell(x, y, x, (y-1+MODEL_SIZE_Y) % MODEL_SIZE_Y, cells, buff);
			//buff[x][(y-1+MODEL_SIZE_Y) % MODEL_SIZE_Y] = cells[x][y];
		break;
		case MOVING_UP_R:
			moveCell(x, y, (x+1) % MODEL_SIZE_X, (y-1+MODEL_SIZE_Y) % MODEL_SIZE_Y, cells, buff);
			//buff[(x+1) % MODEL_SIZE_X][(y-1+MODEL_SIZE_Y) % MODEL_SIZE_Y] = cells[x][y];
		break;
		case MOVING_UP_L:
			moveCell(x, y, (x-1+MODEL_SIZE_X) % MODEL_SIZE_X, (y-1+MODEL_SIZE_Y) % MODEL_SIZE_Y, cells, buff);
			//buff[(x-1+MODEL_SIZE_X) % MODEL_SIZE_X][(y-1+MODEL_SIZE_Y) % MODEL_SIZE_Y] = cells[x][y];
		break;
		case MOVING_L:
			moveCell(x, y, (x-1+MODEL_SIZE_X) % MODEL_SIZE_X, y, cells, buff);
			//buff[(x-1+MODEL_SIZE_X) % MODEL_SIZE_X][y] = cells[x][y];
		break;
		case MOVING_R:
			moveCell(x, y, (x+1) % MODEL_SIZE_X, y, cells, buff);
			//buff[(x+1) % MODEL_SIZE_X][y] = cells[x][y];
		break;
		case MOVING_DOWN:
			moveCell(x, y, x, (y+1) % MODEL_SIZE_Y, cells, buff);
			//buff[x][(y+1) % MODEL_SIZE_Y] = cells[x][y];
		break;
		case MOVING_DOWN_R:
			moveCell(x, y, (x+1) % MODEL_SIZE_X, (y+1) % MODEL_SIZE_Y, cells, buff);
			//buff[(x+1) % MODEL_SIZE_X][(y+1) % MODEL_SIZE_Y] = cells[x][y];
		break;
		case MOVING_DOWN_L:
			moveCell(x, y, (x-1+MODEL_SIZE_X) % MODEL_SIZE_X, (y+1) % MODEL_SIZE_Y, cells, buff);
			//buff[(x-1+MODEL_SIZE_X) % MODEL_SIZE_X][(y+1) % MODEL_SIZE_Y] = cells[x][y];
		break;
	}
	cells[x][y] = 0;
}

void changeStateCell(int state, int x, int y, int **buff) {
	buff[x][y] = state;
}

void createCell(int state, int x, int y, int **buff) {
	buff[x][y] = state;
}

void moveCell(int fromX, int fromY, int toX, int toY, int **cells, int **buff) {
	int tmp;
	if(buff[toX][toY] == 0 && buff[fromX][fromY] == 0) {
		buff[toX][toY] = cells[fromX][fromY];
		//zderzenie przeciwne kierunki
	} else /*if((buff[fromX][fromY]+cells[toX][toY] - buff[toX][toY]+cells[fromX][fromY] + 8) % 8 == 4)*/ {	
		printf("Zderzenie [%i, %i] wektor %i z [%i, %i] wektor %i\n", fromX, fromY, buff[toX][toY]+cells[fromX][fromY],
		 toX, toY, buff[fromX][fromY]+cells[toX][toY]);
		//do ponownego przeliczenia z odwroconym kierunkiem
		tmp = buff[toX][toY];
		//cells[toX][toY] = cells[fromX][fromY];									
		buff[toX][toY] = cells[fromX][fromY];
		//calculateCell(cells, buff, toX, toY);

		//odwracamy kierunek i przeliczamy jeszcze raz
		cells[fromX][fromY] = tmp;
		calculateCell(cells, buff, fromX, fromY);
		//printf("Wektor 1: %i\n", cells[fromX][fromY]);
		//printf("Wektor 2: %i\n", cells[toX][toY]);
	} /*else {
		printf("Nieobsluzony przypadek zderzenia [%i, %i] wektor %i z [%i, %i] wektor %i\n", fromX, fromY, cells[fromX][fromY],
		 toX, toY, buff[toX][toY]);
		buff[toX][toY] = cells[fromX][fromY];
	}*/
}
