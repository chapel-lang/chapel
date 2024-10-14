#include <stdlib.h>

#ifdef HAVE_QSORT_R
#if !defined(__linux__)
static int qthread_internal_shepcomp(void *src, void const *a, void const *b)
#else
static int qthread_internal_shepcomp(void const *a, void const *b, void *src)
#endif
{
  int a_dist = qthread_distance((qthread_shepherd_id_t)(intptr_t)src,
                                *(qthread_shepherd_id_t *)a);
  int b_dist = qthread_distance((qthread_shepherd_id_t)(intptr_t)src,
                                *(qthread_shepherd_id_t *)b);
  return a_dist - b_dist;
}
#else
static qthread_shepherd_id_t shepcomp_src;

static int qthread_internal_shepcomp(void const *a, void const *b) {
  int a_dist = qthread_distance(shepcomp_src, *(qthread_shepherd_id_t *)a);
  int b_dist = qthread_distance(shepcomp_src, *(qthread_shepherd_id_t *)b);

  return a_dist - b_dist;
}
#endif

