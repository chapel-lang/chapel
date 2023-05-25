// Adapted from #20861

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "exported-2.h"

extern void foo(double const *sendBuf, double *recvBuf, int const *count);

void bar(int procID, int size) {
  double *send = malloc(size * sizeof(double));
  double *recv = malloc(size * sizeof(double));
  assert(send != NULL);
  assert(recv != NULL);

  for (int i = 0; i < size; ++i) {
    send[i] = (double)procID;
  }

  foo(send, recv, &size);

  for (int i = 0; i < size; ++i) {
    fprintf(stderr, "%.3f, ", recv[i]);
  }
  fprintf(stderr, "\n");

  free(send);
  free(recv);
}
