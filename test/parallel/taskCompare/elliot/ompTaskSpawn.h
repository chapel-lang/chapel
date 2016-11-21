#include <stdint.h>

#ifdef _OPENMP
#include <omp.h>
#endif

static void ompTaskSpawn(int64_t trials, int64_t numTasks) {
  int i, j;

  #ifdef _OPENMP
  omp_set_num_threads(numTasks);
  #endif

  for (i=0; i<trials; i++) {
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (j=0; j<numTasks; j++) { }
  }
}
