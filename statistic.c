#include <stdlib.h>
#include <gtk/gtk.h>
#include <time.h>
#include <math.h>
#include "main.h"
#include "cell.h"
#include "statistic.h"

statistics_t globalStats;
int x_size = ceil(((double)MODEL_SIZE_X) / CELLCOUNT_FIELD_SIZE_X);
int y_size = ceil(((double)MODEL_SIZE_Y) / CELLCOUNT_FIELD_SIZE_Y);

// returns cell count in given area
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

// initialize statistics structures
statistics_t **initializeStatistics() {
  int i, j, k;

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

  globalStats.probabilities = (probability_t *)malloc(AVERAGED_ITERATIONS * sizeof(probability_t));
  globalStats.lastValues = (int *)malloc(AVERAGED_ITERATIONS * sizeof(int));
  globalStats.startIndex = 0;
  globalStats.stopIndex = 0;
  for(k = 0; k < AVERAGED_ITERATIONS; k++) {
    globalStats.probabilities[k].value = EMPTY_VALUE;
    globalStats.lastValues[k] = EMPTY_VALUE;
    globalStats.probabilities[k].probability = 0.0;
  }

  return stats;
}

// calculates and prints stats for current iteration
void calculateStatistics(FILE *out, int **cells, int printHeaders, statistics_t **stats) {
  int i, j, k, x1, y1, count, tmp, inputIndex;
  double average, sqAvg, var, stdDev, globDevAvg = .0, globDevSqAvg = .0, globStdDev;
  if(printHeaders) {
    fprintf(out, "#Index\tCount\tAverage\tVariance\tStandard deviation\n");
  }
  for(i = 0; i * CELLCOUNT_FIELD_SIZE_X < MODEL_SIZE_X; i++) {
    for(j = 0; j * CELLCOUNT_FIELD_SIZE_Y < MODEL_SIZE_Y; j++) {
      tmp = -2;
      x1 = i * CELLCOUNT_FIELD_SIZE_X;
      y1 = j * CELLCOUNT_FIELD_SIZE_Y;

      // add new element, update list start and stop index
      count = countCells(x1, y1, x1 + CELLCOUNT_FIELD_SIZE_X - 1, y1 + CELLCOUNT_FIELD_SIZE_Y - 1, cells);

      stats[i][j].stopIndex = ((stats[i][j].stopIndex) + 1) % AVERAGED_ITERATIONS;
      if(stats[i][j].startIndex == stats[i][j].stopIndex) {
        tmp = stats[i][j].lastValues[stats[i][j].startIndex];
        stats[i][j].startIndex = ((stats[i][j].startIndex) + 1) % AVERAGED_ITERATIONS;
      }
      stats[i][j].lastValues[stats[i][j].stopIndex] = count;

      // update probabilities
      inputIndex = -1;
      for(k = 0; k < AVERAGED_ITERATIONS; k++) {
        // remove probabilities from oldest count
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
      stats[i][j].probabilities[inputIndex].probability += (1.0/AVERAGED_ITERATIONS);

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
      globDevAvg += stdDev;
      globDevSqAvg += stdDev * stdDev;
      fprintf(out, "[%i, %i]\t%i\t%lf\t%lf\t%lf\n", i, j, count, average, var, stdDev);
    }
  }
  globDevAvg /= (i * j);
  globDevSqAvg /= (i * j);
  globStdDev = sqrt(globDevSqAvg - globDevAvg * globDevAvg);
  
  for(i = 0; i < x_size; i++) {
    for(j = 0; j < y_size; j++) {
    }
  }
  fprintf(out, "\n#Global standard deviation average: %lf, standard deviation of deviations: %lf\n", globDevAvg, globStdDev);
}