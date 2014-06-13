#define MODEL_SIZE_X 100
#define MODEL_SIZE_Y 100
#define CELL_SIZE 7
#define CELL_SEPARATION 1
#define WINDOW_SIZE_X 850
#define WINDOW_SIZE_Y 850
#define ITER_DELAY 0.0
#define TITLE "Simple tumor simulator 2d"
#define PREDICTIVE_BOUNCE_CHANCE 80
#define DIVIDE_CELLS 0
#define REPRODUCTION_INTERVAL 60

#if defined(__CYGWIN__) || defined(_WIN64) || defined(_WIN32)
  #include <windows.h>
  #define SLEEP_FUNC Sleep
	#define SLEEP_MULTIPLIER 1000
#else
	#include <unistd.h>
  #define SLEEP_FUNC usleep
#define SLEEP_MULTIPLIER 1000000
#endif