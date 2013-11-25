void initializeCells(int ***cells, int x, int y);
void drawCell(cairo_t *cr, int cell, int x, int y, int size);
void eraseCell(cairo_t *cr, int x, int y, int size);
void moveCell(cairo_t *cr, int fromX, int fromY, int toX, int toY, int size);