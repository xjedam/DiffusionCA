#include <stdlib.h>
#include <gtk/gtk.h>
#include <time.h>
#include <math.h>
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

statistics_t **initializeStatistics() {
  int i, j, k, 
    x_size = ceil(((double)MODEL_SIZE_X) / CELLCOUNT_FIELD_SIZE_X),
    y_size = ceil(((double)MODEL_SIZE_Y) / CELLCOUNT_FIELD_SIZE_Y);

  statistics_t **stats = (statistics_t **)malloc(x_size * sizeof(statistics_t *));
  for(i = 0; i < x_size; i++) {
    stats[i] = (statistics_t *)malloc(y_size * sizeof(statistics_t));
    for(j = 0; j < y_size; j++) {
      stats[i][j].probabilities = (probability_t *)malloc(AVERAGED_ITERATIONS * sizeof(probability_t));
      stats[i][j].lastValues = (int *)malloc(AVERAGED_ITERATIONS * sizeof(int));
      stats[i][j].startIndex = 0;
      stats[i][j].stopIndex = 0;
      for(k = 0; k < AVERAGED_ITERATIONS; k++) {
        stats[i][j].probabilities[k].value = EMPTY_VALUE;
        stats[i][j].lastValues[k] = EMPTY_VALUE;
        stats[i][j].probabilities[k].probability = 0.0;
      }
    }
  }

  // printf("1 x_size=%i, y_size=%i\n", x_size, y_size);
  // stats->startIndex = 0;
  // stats->stopIndex = 0;
  // stats->countTotal = 0;
  // stats->count = (int ***)malloc(x_size * sizeof(int **));
  // stats->probabilities = (probability_t ***)malloc(x_size * sizeof(probability_t **));
  // for(i = 0; i < x_size; i++) {
  //   stats->count[i] = (int **)malloc(y_size * sizeof(int *));
  //   stats->probabilities[i] = (probability_t **)malloc(y_size * sizeof(probability_t *));
  //   for(j = 0; j < y_size; j++) {
  //     stats->count[i][j] = (int *)malloc(AVERAGED_ITERATIONS * sizeof(int));
  //     stats->probabilities[i][j] = (probability_t *)malloc(AVERAGED_ITERATIONS * sizeof(probability_t)); 
  //     for(k = 0; k < AVERAGED_ITERATIONS; k++) {
  //       stats->count[i][j][k] = 0;
  //       stats->probabilities[i][j][k].value = EMPTY_VALUE;
  //       stats->probabilities[i][j][k].probability = 0.0;
  //       //printf("Alloc count[i]:%i\t[i][j]:%i\t[i][j][k]:%i\n", stats->count[i], stats->count[i][j], &stats->count[i][j][k]);
  //     }
  //   }
  //   //printf("SIZEOF double: %i, int: %i, probability_t: %i\n", sizeof(double), sizeof(int), sizeof(probability_t));
  // }

  return stats;
}

void calculateStatistics(FILE *out, int **cells, int printHeaders, statistics_t **stats) {
  int i, j, k, x1, y1, count, tmp, inputIndex;
  double average, sqAvg, var, stdDev;
  if(printHeaders) {
    fprintf(out, "#Index\tCount\n");
  }
  for(i = 0; i * CELLCOUNT_FIELD_SIZE_X < MODEL_SIZE_X; i++) {
    for(j = 0; j * CELLCOUNT_FIELD_SIZE_Y < MODEL_SIZE_Y; j++) {
      tmp = -2;
      x1 = i * CELLCOUNT_FIELD_SIZE_X;
      y1 = j * CELLCOUNT_FIELD_SIZE_Y;
      //printf("1 i=%i, j=%i, start=%i, stop=%i, [10]=%i\n", i, j, stats->startIndex, stats->stopIndex, stats->count[1][0]);
      // add new element, update list start and stop index
      count = countCells(x1, y1, x1 + CELLCOUNT_FIELD_SIZE_X - 1, y1 + CELLCOUNT_FIELD_SIZE_Y - 1, cells);
      //printf("1.5 ptr count[i]: %i\tcount[i][j]: %i\n", stats->count[i], stats->count[i][j]);
      
      //puts("1.6");
      stats[i][j].stopIndex = ((stats[i][j].stopIndex) + 1) % AVERAGED_ITERATIONS;
      if(stats[i][j].startIndex == stats[i][j].stopIndex) {
        tmp = stats[i][j].lastValues[stats[i][j].startIndex];
        stats[i][j].startIndex = ((stats[i][j].startIndex) + 1) % AVERAGED_ITERATIONS;
      }
      stats[i][j].lastValues[stats[i][j].stopIndex] = count;
      //printf("2, [10]=%i\n", stats->count[1][0]);
      // update probabilities
      inputIndex = -1;
      for(k = 0; k < AVERAGED_ITERATIONS; k++) {
        if(stats[i][j].probabilities[k].value == tmp) {
          stats[i][j].probabilities[k].probability -= (1.0/AVERAGED_ITERATIONS);
        } 
        if(stats[i][j].probabilities[k].probability <= DBL_ZERO_COMP_PRECISION && inputIndex == -1) {
          inputIndex = k;
        } else if(stats[i][j].probabilities[k].value == count) {
          inputIndex = k;
        }
      }
      stats[i][j].probabilities[inputIndex].value = count;
      //printf("2.9, [10]=%i\n", stats->count[1][0]);
      stats[i][j].probabilities[inputIndex].probability += (1.0/AVERAGED_ITERATIONS);
      //printf("3, [10]=%i\n", stats->count[1][0]);
      // count average
      average = 0.0;
      sqAvg = 0.0;
      var = 0.0;
      stdDev = 0.0;
      for(k = 0; k < AVERAGED_ITERATIONS; k++) {
        if(stats[i][j].probabilities[k].probability >= DBL_ZERO_COMP_PRECISION) {
          average += stats[i][j].probabilities[k].value * stats[i][j].probabilities[k].probability;
          sqAvg += stats[i][j].probabilities[k].value * stats[i][j].probabilities[k].value * stats[i][j].probabilities[k].probability;
        } 
      }
      var = sqAvg - average * average;
      stdDev = sqrt(var);
      
      fprintf(out, "[%i, %i]\tcount: %i\tavg: %lf\tVariance: %lf\tStd dev: %lf\n", i, j, count, average, var, stdDev);
    }
  }
}