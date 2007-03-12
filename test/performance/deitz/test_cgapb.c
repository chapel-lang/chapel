#include <stdio.h>
#include <stdlib.h>

#define myint int
#define N 1024
#define ITERS 200

#define A(i,j) ARR[i*N+j]
#define B(i,j) BRR[i*N+j]
#define C(i,j) CRR[i*N+j]

int main() {
  myint *ARR, *BRR, *CRR;
  ARR = (myint*)malloc(N*N*sizeof(myint));
  BRR = (myint*)malloc(N*N*sizeof(myint));
  CRR = (myint*)malloc(N*N*sizeof(myint));

  myint i, j, k;
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      A(i,j) = i+j;
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      B(i,j) = i*j;
  for (k = 0; k < ITERS; k++)
    for (i = 0; i < N; i++)
      for (j = 0; j < N; j++)
        C(i,j) = A(i,j) + B(i,j);
  printf("%d\n", C(51,51));
  return 0;
}
