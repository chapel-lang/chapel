#include <stdio.h>
#include <stdlib.h>

int main() {
  double A[128][128];
  int i, j;
  for (i = 0; i < 128; i++)
    for (j = 0; j < 128; j++)
      A[i][j] = 0.0;
  for (i = 0; i < 128; i++)
    for (j = 0; j < 128; j++)
      A[i][j] = i + j + 2;
  printf("%g\n", A[51][51]);
  return 0;
}
