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
	printf("%i %i\n", x, y);
	draw_cir(cr, (x+1)*(size+1), (y+1)*(size+1), size);
	cairo_fill (cr);
}

void eraseCell(cairo_t *cr, int x, int y, int size) {
	GdkRGBA color = {0, 0, 0, 1.0};
  	gdk_cairo_set_source_rgba (cr, &color);
	draw_cir(cr, x*(size+1), y*(size+1), size+1);
	cairo_fill (cr);
}

void calculateCell(int **cells, int **buff, int x, int y) {
	printf("%i %i\n", x, y);
	switch(cells[x][y]) {
		case MOVING_UP:
			buff[(x-1) % MODEL_SIZE_Y][y] = cells[x][y];
		break;
		case MOVING_UP_R:
			buff[(x-1) % MODEL_SIZE_Y][(y+1)  % MODEL_SIZE_X] = cells[x][y];
		break;
		case MOVING_UP_L:
			buff[(x-1) % MODEL_SIZE_Y][(y-1)  % MODEL_SIZE_X] = cells[x][y];
		break;
		case MOVING_L:
			buff[x][(y-1) % MODEL_SIZE_X] = cells[x][y];
		break;
		case MOVING_R:
			buff[x][(y+1) % MODEL_SIZE_X] = cells[x][y];
		break;
		case MOVING_DOWN:
			buff[(x+1) % MODEL_SIZE_Y][y] = cells[x][y];
		break;
		case MOVING_DOWN_R:
			buff[(x+1) % MODEL_SIZE_Y][(y+1) % MODEL_SIZE_X] = cells[x][y];
		break;
		case MOVING_DOWN_L:
			buff[(x+1) % MODEL_SIZE_Y][(y-1) % MODEL_SIZE_X] = cells[x][y];
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
	buff[toX][toY] = cells[fromX][fromY];
}
