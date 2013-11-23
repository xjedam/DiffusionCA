#include <stdlib.h>

void initializeCells(int ***cells, int x, int y) {
  int i;
  *cells = (int **)malloc(x * sizeof(int *));
  for(i=0; i<x; i++) {
    (*cells)[i] = (int *)calloc(y, sizeof(int));
  }
}