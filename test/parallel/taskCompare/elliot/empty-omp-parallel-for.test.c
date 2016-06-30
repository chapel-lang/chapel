#include <omp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static int numTrials = 100;
static int printTimings = 0;
static int numThreads = -1;

// Helper routine to parse Chapel config vars of the form `--var=value` where
// var is an int or a bool. For bools, a value of `true` returns 1, and
// everything else returns false
static int parseConfigInt(char* arg) {
  char* configVal = NULL;

  configVal = strchr(arg, '=');
  if (configVal == NULL) {
    printf("Error, config var '%s', must be of the form `--var=value`\n", arg);
  }
  configVal = configVal+sizeof(char);

  if (strcmp(configVal, "true") == 0) {
    return 1;
  }
  return (int) strtol(configVal, NULL, 10);
}

static void parseArgs(int argc, char** argv) {
  if (argc < 4) {
    printf("Usage is ./prog --numTrials=<trials> --printTimings=<timings> --numThreads=<threads>\n");
    exit(-1);
  }

  numTrials = parseConfigInt(argv[1]);
  printTimings = parseConfigInt(argv[2]);
  numThreads = parseConfigInt(argv[3]);
}

int main (int argc, char** argv) {
  double start, elapsed;
  int i, j;

  parseArgs(argc, argv);

  if (numThreads) {
    omp_set_num_threads(numThreads);
  }

  start = omp_get_wtime();
  for (i=0; i<numTrials; i++) {
    #pragma omp parallel for
    for (j=0; j<numThreads; j++) {
    }
  }
  elapsed = omp_get_wtime() - start;

  if (printTimings) {
    printf("Elapsed time: %f\n", elapsed);
  }

  return 0;
}
