#include <stdint.h>

#ifdef _OPENMP
#include <omp.h>
#endif

static void ompTaskSpawn(int64_t trials, int64_t numTasks, int64_t runSerial) {
  int i, j;

  #ifdef _OPENMP
  if (runSerial) {
    omp_set_num_threads(1);
  } else {
    omp_set_num_threads(numTasks);
  }
  #endif

  for (i=0; i<trials; i++) {
    #ifdef _OPENMP
    #pragma omp parallel for
    #endif
    for (j=0; j<numTasks; j++) { }
  }
}
