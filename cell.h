#define STATIONARY      1
#define MOVING_UP       2
#define MOVING_UP_R     3
#define MOVING_R        4
#define MOVING_DOWN_R   5
#define MOVING_DOWN     6
#define MOVING_DOWN_L   7
#define MOVING_L        8
#define MOVING_UP_L     9

void initializeCells(int ***cells, int x, int y);
void drawCell(cairo_t *cr, int cell, int x, int y, int size);
void eraseCell(cairo_t *cr, int x, int y, int size);
void calculateCell(int **cells, int **buff, int x, int y);
void changeStateCell(int state, int x, int y, int **buff);
void createCell(int state, int x, int y, int **buff);
void moveCell(int fromX, int fromY, int toX, int toY, int **cells, int **buff);