// Compile: gcc -std=c99 dgemm.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef _USE_RDTSC_
#include "rdtsc.h"
typedef unsigned long long _timertype;
static double clock_ticks = (double) 278941100;
#define getTime() rdtsc()
#define getSeconds(t) ((double) (t))/clock_ticks
#else
#include <sys/time.h>
#include <time.h>
typedef double _timertype;
double _now_time(void) {
  struct tm * now;
  struct timeval t;
  gettimeofday(&t, NULL);
  now = localtime(&t.tv_sec);
  return (double)(now->tm_hour)*3600.0e+6 +
    (double)(now->tm_min)*60.0e+6 +
    (double)(now->tm_sec)*1.0e+6 +
    (double)(t.tv_usec);
}
#define getTime() _now_time()
#define getSeconds(t) ((t)/1.0e+6)
#endif

#define N 64
#define M 64
#define O 64
static float A2D[N][M];
static float B2D[M][O];
static float C2D[N][O];

static float A1D[N*M];
static float B1D[M*O];
static float C1D[N*O];

void
dgemm2D_static() {
  int i, j, k;
  _timertype t0 = getTime();
  for (i=0; i<N; i++) {
    for (j=0; j<O; j++) {
      for (k=0; k<M; k++) {
        C2D[i][j] -= A2D[i][k] * B2D[k][j];
      }
    }
  }
  _timertype t1 = getTime();
  printf("dgemm2D static (%d,%d,%d): %f\n", N, M, O, getSeconds(t1-t0));
}

void
dgemm1D_static() {
  int i, j, k;
  _timertype t0 = getTime();
  for (i=0; i<N; i++) {
    for (j=0; j<O; j++) {
      for (k=0; k<M; k++) {
        C1D[i*O+j] -= A1D[i*M+k] * B1D[k*O+j];
      }
    }
  }
  _timertype t1 = getTime();
  printf("dgemm1D static (%d,%d,%d): %f\n", N, M, O, getSeconds(t1-t0));
}

void
dgemm2Df(int p, int q, int r, float * A, float * B, float * C) {
  int i, j, k;
  _timertype t0 = getTime();
  for (i=0; i<p; i++) {
    for (j=0; j<r; j++) {
      for (k=0; k<q; k++) {
        C[i*r+j] -= A[i*q+k] * B[k*r+j];
      }
    }
  }
  _timertype t1 = getTime();
  printf("dgemm2Df (%d,%d,%d): %f\n", p, q, r, getSeconds(t1-t0));
}

void
dgemm2Df_restrict(int p, int q, int r,
                 float * restrict A, float * restrict B, float * restrict C) {
  int i, j, k;
  _timertype t0 = getTime();
  for (i=0; i<p; i++) {
    for (j=0; j<r; j++) {
      for (k=0; k<q; k++) {
        C[i*r+j] -= A[i*q+k] * B[k*r+j];
      }
    }
  }
  _timertype t1 = getTime();
  printf("dgemm2Df restrict(%d,%d,%d): %f\n", p, q, r, getSeconds(t1-t0));
}

int
main(int argc, char *argv[]) {
  int i, j, k;
  int n = argc > 1 ? strtol(argv[1], NULL, 10) : N;
  int m = argc > 2 ? strtol(argv[2], NULL, 10) : M;
  int o = argc > 3 ? strtol(argv[3], NULL, 10) : O;
  float *Ad = (float *) malloc (n*m*sizeof(float));
  float *Bd = (float *) malloc (m*o*sizeof(float));
  float *Cd = (float *) malloc (n*o*sizeof(float));

  for (i=0; i<N; i++) {
    for (j=0; j<M; j++) {
      float a = (float) rand()/12345.6789;
      A2D[i][j] = A1D[i*M+j] = a*2.0 - 1.0;
    }
    for (k=0; k<O; k++) {
      C2D[i][k] = C1D[i*O+k] = 1.0;
    }
  }
  for (j=0; j< M; j++) {
    for (k=0; k<O; k++) {
      B2D[j][k] = B1D[j*O+k] = 1.0;
    }
  }

  for (i=0; i<n; i++) {
    for (j=0; j<m; j++) {
      float a = (float) rand()/12345.6789;
      Ad[i*m+j] = a*2.0 - 1.0;
    }
    for (k=0; k<o; k++) {
      Cd[i*o+k] = 1.0;
    }
  }
  for (j=0; j< m; j++) {
    for (k=0; k<o; k++) {
      Bd[j*o+k] = 1.0;
    }
  }

  dgemm2Df(n,m,o,Ad,Bd,Cd);
  dgemm2Df_restrict(n,m,o,Ad,Bd,Cd);
  dgemm1D_static();
  dgemm2D_static();
}

