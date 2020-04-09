#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

typedef double elemType;

static const int numVectors = 3;

#ifndef printParams
#define printParams 1
#endif
#ifndef printArrays
#define printArrays 0
#endif
#ifndef printStats
#define printStats 1
#endif

static int m = 0;
static int numTrials = 10;
static double alpha = 3.0;
static double epsilon = 0.0;

// copied from Chapel runtime
static double _now_time(void) {
  struct tm * now;
  struct timezone tz;
  struct timeval t;
  gettimeofday(&t, &tz);
  now = localtime(&t.tv_sec);
  return (double)(now->tm_hour)*3600.0e+6 +
    (double)(now->tm_min)*60.0e+6 +
    (double)(now->tm_sec)*1.0e+6 +
    (double)(t.tv_usec);
}

static double getCurrentTime(void) {
  return _now_time() / 1.0e+6;
}


static void printProblemSize(int problemSize) {
  const int bytesPerArray = problemSize * sizeof(elemType);
  const double totalMemInGB = ((double)bytesPerArray * numVectors) / (1024.0*1024.0*1024.0);

  printf("Problem size = %d\n", problemSize);
  printf("Bytes per array = %d\n", bytesPerArray);
  printf("Total memory required (GB) = %lg\n", totalMemInGB);
}


static void printConfiguration() {
  if (printParams) {
    printProblemSize(m);
    printf("Number of trials = %d\n", numTrials);
  }
}


static void printArray(elemType X[]) {
  int i;
  for (i=0; i<m-1; i++) {
    printf("%lf ", X[i]);
  }
  printf("%lf", X[m-1]);
  printf("\n");
}


static void initVectors(elemType B[], elemType C[]) {
  int i;
  for (i=0; i<m; i++) {
    B[i] = 1.2;
  }
  for (i=0; i<m; i++) {
    C[i] = 3.4;
  }

  if (printArrays) {
    printf("B is: ");
    printArray(B);
    printf("\n");

    printf("C is: ");
    printArray(C);
    printf("\n");
  }
}


static double max(double x, double y) {
  if (x < y) {
    return y;
  } else {
    return x;
  }
}

static double min(double x, double y) {
  if (x > y) {
    return y;
  } else {
    return x;
  }
}


static int verifyResults(elemType A[], elemType B[], elemType C[]) {
  int i;

  if (printArrays) {
    printf("A is: ");
    printArray(A);
    printf("\n");
  }

  double infNorm = 0.0;
  for (i=0; i<m; i++) {
    infNorm = max(infNorm, abs(A[i] - (B[i] + alpha * C[i])));
  }
  
  return (infNorm <= epsilon);
}


static void printResults(int successful, double execTime[]) {
  printf("Validation: %s\n", successful ? "SUCCESS" : "FAILURE");
  if (printStats) {
    double totalTime = 0.0, avgTime = 0.0, minTime = DBL_MAX;
    int i;
    for (i=0; i<numTrials; i++) {
      totalTime += execTime[i];
      minTime = min(minTime, execTime[i]);
    }
    avgTime = totalTime / numTrials;
    printf("Execution time:\n");
    printf("  tot = %lf\n", totalTime);
    printf("  avg = %lf\n", avgTime);
    printf("  min = %lf\n", minTime);

    double GBPerSec = numVectors * sizeof(elemType) * (m/minTime) * 1.0e-9;
    printf("Performance (GB/s) = %lf\n", GBPerSec);
  }
}




int main(int argc, char* argv[]) {
  if (argc < 2) {
    fprintf(stderr, "Usage: %s <m> [<numTrials> <alpha> <epsilon>]\n", argv[0]);
    fprintf(stderr, "  m is the problem size\n");
    fprintf(stderr, "  numTrials is the number of trials to run\n");
    fprintf(stderr, "  alpha is the scalar multiplier\n");
    fprintf(stderr, "  epsilon is the tolerance for verification\n");
    exit(0);
  }

  m = atoi(argv[1]);
  if (argc >= 3) {
    numTrials = atoi(argv[2]);
    if (argc >= 4) {
      alpha = atof(argv[3]);
      if (argc >= 5) {
        epsilon = atof(argv[4]);
      }
    }
  }
  

  printConfiguration();
  elemType* const __restrict A = (elemType*)malloc(m*sizeof(elemType));
  elemType* const __restrict B = (elemType*)malloc(m*sizeof(elemType));
  elemType* const __restrict C = (elemType*)malloc(m*sizeof(elemType));
  
  initVectors(B, C);

  double execTime[numTrials];

  int trial;
  for (trial=0; trial<numTrials; trial++) {
    double startTime = getCurrentTime();

    int j;
    for (j=0; j<m; j++) {
      A[j] = B[j] + alpha * C[j];
    }
    
    execTime[trial] = getCurrentTime() - startTime;
  }

  int validAnswer = verifyResults(A, B, C);
  printResults(validAnswer, execTime);

  return 0;
}
