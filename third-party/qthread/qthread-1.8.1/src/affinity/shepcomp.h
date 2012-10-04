#include <stdlib.h>

#ifdef HAVE_QSORT_R
# if !defined(__linux__)
static int qthread_internal_shepcomp(void *src, const void *a, const void *b)
# else
static int qthread_internal_shepcomp(const void *a, const void *b, void *src)
# endif
{
    int a_dist =
	qthread_distance((qthread_shepherd_id_t) (intptr_t) src,
			 *(qthread_shepherd_id_t *) a);
    int b_dist =
	qthread_distance((qthread_shepherd_id_t) (intptr_t) src,
			 *(qthread_shepherd_id_t *) b);
    return a_dist - b_dist;
}
#else
static qthread_shepherd_id_t shepcomp_src;

#ifndef __SUN__
/* this cannot be static, because Sun's idiotic gccfss compiler sometimes (at
 * optimization levels > -O3) refuses to compile it if it is - note that this
 * doesn't seem to be something that can be detected with a configure script,
 * because it WORKS on small programs */
static
#endif
int qthread_internal_shepcomp(const void *a, const void *b)
{
    int a_dist = qthread_distance(shepcomp_src, *(qthread_shepherd_id_t *) a);
    int b_dist = qthread_distance(shepcomp_src, *(qthread_shepherd_id_t *) b);

    return a_dist - b_dist;
}
#endif

