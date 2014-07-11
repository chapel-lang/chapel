#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

#include "chpl-bitops.h"
#include "chpltimers.h"

#define NUM_ITER 100000000

double diff_time(_timevalue a, _timevalue b) {
  return ((double)a.tv_sec*1.0E6+a.tv_usec)-(b.tv_sec*1.0E6+b.tv_usec);
}

int main(int argc, char** argv) {
  int start;
  uint64_t accumulator = 0;
  _timevalue start_time;
  _timevalue end_time;

  // we need to make the start dynamic so everything isn't expanded into
  // constants
  if(argc < 2) {
    return 1;
  }
  start = atoi(argv[1]);

  printf("%s", "popcount: ");
  start_time = chpl_now_timevalue();
  for(uint64_t i=start; i<(uint32_t)start+NUM_ITER; i++) {
    accumulator += chpl_bitops_popcount_64(i);
  }
  end_time = chpl_now_timevalue();
#ifdef PRINT_TIMING
  printf("%f\n", diff_time(end_time, start_time)/1000000.0);
#endif
  printf("%" PRIu64 "\n", accumulator);

  accumulator = 0;
  printf("%s", "clz: ");
  start_time = chpl_now_timevalue();
  for(uint64_t i=start; i<(uint32_t)start+NUM_ITER; i++) {
    accumulator += chpl_bitops_clz_64(i);
  }
  end_time = chpl_now_timevalue();
#ifdef PRINT_TIMING
  printf("%f\n", diff_time(end_time, start_time)/1000000.0);
#endif
  printf("%" PRIu64 "\n", accumulator);

  accumulator = 0;
  printf("%s", "ctz: ");
  start_time = chpl_now_timevalue();
  for(uint64_t i=start; i<(uint32_t)start+NUM_ITER; i++) {
    accumulator += chpl_bitops_ctz_64(i);
  }
  end_time = chpl_now_timevalue();
#ifdef PRINT_TIMING
  printf("%f\n", diff_time(end_time, start_time)/1000000.0);
#endif
  printf("%" PRIu64 "\n", accumulator);

  return 0;
}
