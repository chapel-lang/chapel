#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "qthread/qthread.h"
#include "qt_visibility.h"
#include "qt_shepherd_innards.h"

#ifndef QTHREAD_SHEPHERD_TYPEDEF
# define QTHREAD_SHEPHERD_TYPEDEF
typedef struct qthread_shepherd_s qthread_shepherd_t;
#endif

#if defined(QTHREAD_HAVE_LIBNUMA)
# define QTHREAD_HAVE_MEM_AFFINITY
#endif

#if defined(QTHREAD_HAVE_HWLOC) && (HWLOC_API_VERSION > 0x00010000)
# define QTHREAD_HAVE_MEM_AFFINITY
#endif

#ifdef QTHREAD_HAVE_MEM_AFFINITY
# define MEM_AFFINITY_ONLY_ARG(x) x,
# define MEM_AFFINITY_ONLY(x)     x
#else
# define MEM_AFFINITY_ONLY_ARG(x)
# define MEM_AFFINITY_ONLY(x)
#endif

void INTERNAL qt_affinity_init(qthread_shepherd_id_t *nbshepherds,
                               qthread_worker_id_t   *nbworkers);
void INTERNAL qt_affinity_set(
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
                              qthread_worker_t * me
#else
                              qthread_shepherd_t * me
#endif
                              );
int qt_affinity_gendists(qthread_shepherd_t   *sheps,
                         qthread_shepherd_id_t nshepherds);

#ifdef QTHREAD_HAVE_MEM_AFFINITY
void INTERNAL *qt_affinity_alloc(size_t bytes);
void INTERNAL *qt_affinity_alloc_onnode(size_t bytes,
                                        int    node);
void INTERNAL qt_affinity_mem_tonode(void  *addr,
                                     size_t bytes,
                                     int    node);
void INTERNAL qt_affinity_free(void  *ptr,
                               size_t bytes);
#endif
/* vim:set expandtab: */
