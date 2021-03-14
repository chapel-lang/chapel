#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

typedef unsigned long long randType;
typedef randType elemType;
typedef randType indexType;

#ifndef printParams
#define printParams 1
#endif
#ifndef printArrays
#define printArrays 0
#endif
#ifndef printStats
#define printStats 1
#endif

#define numArrays 1

indexType n;
indexType N_U;
indexType m;
indexType indexMask;

#define errorTolerance 1.0e-2


randType m2[64];
int randWidth = 64;

// copied from Chapel runtime
double _now_time(void) {
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

double getCurrentTime(void) {
  return _now_time() / 1.0e+6;
}

void getNextRandom(randType* x) {
  randType POLY = 0x7;
  randType hiRandBit = 0x1LL << (randWidth-1);
  *x = (*x << 1) ^ ((*x & hiRandBit) ? POLY : 0);
}


void computeM2Vals(int numVals) {
  randType nextVal = 0x1;
  int i;

  for (i=0; i<64; i++) {
    m2[i] = nextVal;
    getNextRandom(&nextVal);
    getNextRandom(&nextVal);
  }
}

int lg(int n) {
  int retval = -1;
  while (n != 0) {
    n >> 1;
    retval += 1;
  }
  return retval;
}

randType getNthRandom(indexType n) {
  randType period = 0x7fffffffffffffffLL/7;

  n %= period;
  if (n == 0) { return 0x1; }
  
  randType ran = 0x2;
  int i = lg(n)-1;
  while (i != -1) {
    randType val = 0;
    int j;
    for (j=0; j<64; j++) {
      if ((ran >> j) & 1) { val ^= m2[j]; }
    }
    ran = val;
    if ((n >> i) & 1) { getNextRandom(&ran); }
    i--;
  }
  return ran;
}

randType getFirstRandom() {
  static int firstCall = 1;

  if (firstCall) {
    firstCall = 0;
    computeM2Vals(64);
  }
  return getNthRandom(0);
}



void printProblemSize(indexType problemSize, indexType lgProbSize) {
  const indexType bytesPerArray = problemSize * sizeof(elemType);
  const double totalMemInGB = (double)bytesPerArray / (1024.0*1024.0*1024.0);

  printf("Problem size = %llu (2 ** %llu)\n", problemSize, lgProbSize);
  printf("Bytes per array = %llu\n", bytesPerArray);
  printf("Total memory required (GB) = %lg\n", totalMemInGB);
}


void printConfiguration() {
  if (printParams) {
    printProblemSize(m,n);
    printf("Number of updates = %llu\n\n", N_U);
  }
}


void printTable(elemType T[]) {
  indexType i;
  for (i=0; i<m-1; i++) {
    printf("%llu ", T[i]);
  }
  printf("%llu", T[m-1]);
  printf("\n");
}


int verifyResults(elemType T[]) {
  indexType i;

  if (printArrays) {
    printf("After updates, T is: ");
    printTable(T);
    printf("\n");
  }
  
  randType r = getFirstRandom();
  for (i=0; i<N_U; i++) {
    getNextRandom(&r);
    T[r & indexMask] ^= r;
  }

  if (printArrays) {
    printf("After verification, T is: ");
    printTable(T);
    printf("\n");
  }
  
  indexType numErrors = 0;
  for (i=0; i<m; i++) {
    if (T[i] != i) {
      numErrors++;
    }
  }
  if (printStats) { printf("Number of errors is: %d\n\n", numErrors); }

  return (numErrors <= (errorTolerance * N_U));
}


void printResults(int successful, double execTime) {
  printf("Validation: %s\n", successful ? "SUCCESS" : "FAILURE");
  if (printStats) {
    printf("Execution time = %lg\n", execTime);
    printf("Performance (GUPS) =  %lg\n", (double)N_U / execTime * 1.0e-9);
  }
}




int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <n>\n", argv[0]);
    fprintf(stderr, "  n is log2() of the problem size\n");
    exit(0);
  }

  n = atoi(argv[1]);
  N_U = pow(2, n+2);
  m = 0x1 << n;
  indexMask = m-1;

  printConfiguration();
  elemType * const __restrict T = (elemType*)malloc(m*sizeof(elemType));
  
  indexType i;
  for (i=0; i<m; i++) T[i] = 0; // warm up memory (and zero out)

  double startTime = getCurrentTime();

  for (i=0; i<m; i++) {
    T[i] = i;
  }

  randType r = getFirstRandom();
  for (i=0; i<N_U; i++) {
    getNextRandom(&r);
    T[r & indexMask] ^= r;
  }

  double execTime = getCurrentTime() - startTime;

  int validAnswer = verifyResults(T);
  printResults(validAnswer, execTime);
}
