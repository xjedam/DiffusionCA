#include <stdlib.h>
#include <gtk/gtk.h>
#include <time.h>
#include "main.h"
#include "cell.h"
#include "statistic.h"

int countCells(int x1, int y1, int x2, int y2, int **cells) {
  int i, j, count = 0;
  for(i = x1; i <= x2; i++) {
    for(j = y1; j <= y2; j++) {
      if(cells[i][j] != 0) {
        count++;
      }
    }
  }
  return count;
}

void printStatistics(FILE *out, int **cells, int printHeaders) {
  int i, j, x1, y1;
  if(printHeaders) {
    fprintf(out, "#Index\tCount\n");
  }
  for(i = 0; i * CELLCOUNT_FIELD_SIZE_X < MODEL_SIZE_X; i++) {
    for(j = 0; j * CELLCOUNT_FIELD_SIZE_Y < MODEL_SIZE_Y; j++) {
      x1 = i * CELLCOUNT_FIELD_SIZE_X;
      y1 = j * CELLCOUNT_FIELD_SIZE_Y;
      fprintf(out, "[%i, %i]\t%i\n", i, j, countCells(x1, y1, x1 + CELLCOUNT_FIELD_SIZE_X - 1, y1 + CELLCOUNT_FIELD_SIZE_Y - 1, cells));
    }
  }
}