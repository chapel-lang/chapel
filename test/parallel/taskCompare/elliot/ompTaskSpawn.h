#include <omp.h>
#include <stdint.h>

static void ompTaskSpawn(int64_t trials, int64_t numTasks) {
  int i, j;
  omp_set_num_threads(numTasks);

  for (i=0; i<trials; i++) {
    #pragma omp parallel for
    for (j=0; j<numTasks; j++) { }
  }
}
