#define CELLCOUNT_FIELD_SIZE_X MODEL_SIZE_X/5 
#define CELLCOUNT_FIELD_SIZE_Y MODEL_SIZE_Y/5 

int countCells(int x1, int y1, int x2, int y2, int **cells);
void printStatistics(FILE *out, int **cells, int printHeaders);