#include <stdlib.h>
#include <gtk/gtk.h>
#include "interface.h"

void initializeCells(int ***cells, int x, int y) {
  int i;
  *cells = (int **)malloc(x * sizeof(int *));
  for(i=0; i<x; i++) {
    (*cells)[i] = (int *)calloc(y, sizeof(int));
  }
}

void drawCell(cairo_t *cr, int cell, int x, int y, int size) {
	//draw_sq(cr, x, y, size);
	draw_cir(cr, x*(size+1), y*(size+1), size);
}