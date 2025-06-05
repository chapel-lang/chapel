#include <stdlib.h>

static qthread_shepherd_id_t shepcomp_src;

static int qthread_internal_shepcomp(void const *a, void const *b) {
  int a_dist = qthread_distance(shepcomp_src, *(qthread_shepherd_id_t *)a);
  int b_dist = qthread_distance(shepcomp_src, *(qthread_shepherd_id_t *)b);

  return a_dist - b_dist;
}
