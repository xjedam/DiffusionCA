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
	cairo_fill (cr);
}

void eraseCell(cairo_t *cr, int x, int y, int size) {
	GdkRGBA color = {0, 0, 0, 1.0};
  	gdk_cairo_set_source_rgba (cr, &color);
	draw_cir(cr, x*(size+1), y*(size+1), size+1);
	cairo_fill (cr);
}

void moveCell(cairo_t *cr, int fromX, int fromY, int toX, int toY, int size) {
	drawCell(cr, 1, toX, toY, size);
	eraseCell(cr, fromX, fromY, size);
}
