#define DIRECTION(cell)           (cell & 0xF) 
#define STATIONARY                1
#define MOVING_UP                 2
#define MOVING_UP_R               3
#define MOVING_R                  4
#define MOVING_DOWN_R             5
#define MOVING_DOWN               6
#define MOVING_DOWN_L             7
#define MOVING_L                  8
#define MOVING_UP_L               9
#define OPPOSITE_DIRECTION(d)     (((d + 2) % 8) + 2)
#define CHANGE_DIRECTION(c, d)    ((c & 0xFFFFFFF0) | d) 

#define TYPE(cell)                ((cell & 0xF0) >> 4)
#define TYPE1                     1
#define TYPE2                     2
#define TYPE3                     3
#define TYPE4                     4
#define TYPE5                     5
#define CHANGE_TYPE(c, t)         ((c & 0xFFFFFF0F) | (t << 4))     

#define DIVISIONS_LEFT(cell)      ((cell & 0x3F00) >> 8)
#define SET_DIVISIONS_LEFT(c, t)  ((c & 0xFFFFC0FF) | (t << 8)) 
#define SUB_DIVISIONS_LEFT(c)     ((((c & 0x3F00) >> 8) - 1) << 8 | (c & 0xFFFFC0FF)) 

#define DIVISION_TIME(cell)       ((cell & 0xFFC000) >> 14)
#define SET_DIVISION_TIME(c, t)   ((c & 0xFF003FFF) | (t << 14)) 
#define SUB_DIVISION_TIME(c)      ((((c & 0xFFC000) >> 14) - 1) << 14 | (c & 0xFF003FFF))

#define NEW_CELL_DIVISIONS        1

void initializeCells(int ***cells, int x, int y);
void drawCell(cairo_t *cr, int cell, int x, int y, int size);
void eraseCell(cairo_t *cr, int x, int y, int size);
void calculateCell(int **cells, int **buff, int x, int y);
void changeStateCell(int state, int x, int y, int **buff);
void createCell(int state, int x, int y, int **buff);
void moveCell(int fromX, int fromY, int toX, int toY, int **cells, int **buff);
void calculateHeadCollisionFar(int fromX, int fromY, int toX, int toY, int **cells, int **buff);
void calculateHeadCollisionNear(int fromX, int fromY, int toX, int toY, int **cells, int **buff);
void calculateCollisionFar(int fromX, int fromY, int toX, int toY, int **cells, int **buff);
void reproduceCell(int x, int y, int **cells, int **buff);