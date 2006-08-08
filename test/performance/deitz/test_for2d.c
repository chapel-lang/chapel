#include <stdio.h>
#include <stdlib.h>

#define N 128
#define ITERS 100

#define A(i,j) ARR[(i-1)*N+j-1]

int main() {
  int *ARR;
  ARR = (int*)malloc(N*N*sizeof(int));

  int i, j, k;
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      A(i,j) = 0;
  for (k = 0; k < ITERS; k++)
    for (i = 0; i < N; i++)
      for (j = 0; j < N; j++)
        A(i,j) = i + j + 2 + k;
  printf("%d\n", A(52,52));
  return 0;
}
