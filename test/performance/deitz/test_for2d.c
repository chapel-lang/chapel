#include <stdio.h>
#include <stdlib.h>

#define myint int
#define N 1024
#define ITERS 100

#define A(i,j) ARR[i*N+j]

int main() {
  myint *ARR;
  ARR = (myint*)malloc(N*N*sizeof(myint));

  myint i, j, k;
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      A(i,j) = 0;
  for (k = 0; k < ITERS; k++)
    for (i = 0; i < N; i++)
      for (j = 0; j < N; j++)
        A(i,j) = i + j + 2 + k + 1;
  printf("%d\n", A(51,51));
  return 0;
}
