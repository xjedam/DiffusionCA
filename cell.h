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

void initializeCells(int ***cells, int x, int y);
void drawCell(cairo_t *cr, int cell, int x, int y, int size);
void eraseCell(cairo_t *cr, int x, int y, int size);
void calculateCell(int **cells, int **buff, int x, int y);
void changeStateCell(int state, int x, int y, int **buff);
void createCell(int state, int x, int y, int **buff);
void moveCell(int fromX, int fromY, int toX, int toY, int **cells, int **buff);
void calculateHeadCollisionFar(int fromX, int fromY, int toX, int toY, int **cells, int **buff);
void calculateHeadCollisionNear(int fromX, int fromY, int toX, int toY, int **cells, int **buff);