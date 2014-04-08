#define CELLCOUNT_FIELD_SIZE_X 	(MODEL_SIZE_X/5) 
#define CELLCOUNT_FIELD_SIZE_Y 	(MODEL_SIZE_Y/5) 
#define AVERAGED_ITERATIONS		20
#define EMPTY_VALUE				-1
#define DBL_ZERO_COMP_PRECISION 0.00001

typedef struct STAT {
	int value;
	double probability;
} probability_t;

typedef struct SITE_PROB {
	probability_t *probabilities;
	int *lastValues;
	int startIndex;
	int stopIndex;
} statistics_t;

int countCells(int x1, int y1, int x2, int y2, int **cells);
statistics_t **initializeStatistics();
void calculateStatistics(FILE *out, int **cells, int printHeaders, statistics_t **stats);