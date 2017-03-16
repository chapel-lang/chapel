#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef HAVE_SYSCTL
# ifdef HAVE_SYS_TYPES_H
#  include <sys/types.h>
# endif
# ifdef HAVE_SYS_SYSCTL_H
#  include <sys/sysctl.h>
# endif
#endif
#if defined(HAVE_SYSCONF) && defined(HAVE_UNISTD_H)
# include <unistd.h>
#endif

#include <plpa.h>

#include "qt_affinity.h"
#include "qt_asserts.h"
#include "shufflesheps.h"

qthread_shepherd_id_t guess_num_shepherds(void);
qthread_worker_id_t   guess_num_workers_per_shep(qthread_shepherd_id_t nshepherds);

void INTERNAL qt_affinity_init(qthread_shepherd_id_t *nbshepherds,
                               qthread_worker_id_t   *nbworkers,
                               size_t                *hw_par)
{                                      /*{{{ */
    if (*nbshepherds == 0) {
        *nbshepherds = guess_num_shepherds();
        if (*nbshepherds <= 0) {
            *nbshepherds = 1;
        }
    }
    if (*nbworkers == 0) {
        *nbworkers = guess_num_workers_per_shep(*nbshepherds);
        if (*nbworkers <= 0) {
            *nbworkers = 1;
        }
    }
}                                      /*}}} */

qthread_shepherd_id_t INTERNAL guess_num_shepherds(void)
{                                      /*{{{ */
    qthread_shepherd_id_t nshepherds = 1;

#if defined(HAVE_SYSCONF) && defined(_SC_NPROCESSORS_CONF)      /* Linux */
    long ret = sysconf(_SC_NPROCESSORS_CONF);
    nshepherds = (ret > 0) ? ret : 1;
#elif defined(HAVE_SYSCTL) && defined(CTL_HW) && defined(HW_NCPU)
    int      name[2] = { CTL_HW, HW_NCPU };
    uint32_t oldv;
    size_t   oldvlen = sizeof(oldv);
    if (sysctl(name, 2, &oldv, &oldvlen, NULL, 0) >= 0) {
        assert(oldvlen == sizeof(oldv));
        nshepherds = (int)oldv;
    }
#endif /* if defined(HAVE_SYSCONF) && defined(_SC_NPROCESSORS_CONF) */
    return nshepherds;
}                                      /*}}} */

void INTERNAL qt_affinity_set(qthread_worker_t *me, unsigned int Q_UNUSED(nw))
{                                      /*{{{ */
    plpa_cpu_set_t *cpuset =
        (plpa_cpu_set_t *)MALLOC(sizeof(plpa_cpu_set_t));

    PLPA_CPU_ZERO(cpuset);
    PLPA_CPU_SET(me->packed_worker_id, cpuset);
    if ((plpa_sched_setaffinity(0, sizeof(plpa_cpu_set_t), cpuset) < 0) &&
        (errno != EINVAL)) {
        perror("plpa setaffinity");
    }
    FREE(cpuset, sizeof(plpa_cpu_set_t));
}                                      /*}}} */

qthread_worker_id_t INTERNAL guess_num_workers_per_shep(qthread_shepherd_id_t nshepherds)
{                                      /*{{{ */
    return 1;
}                                      /*}}} */

int INTERNAL qt_affinity_gendists(qthread_shepherd_t   *sheps,
                                  qthread_shepherd_id_t nshepherds)
{   /*{{{*/
    for (size_t i = 0; i < nshepherds; ++i) {
        sheps[i].node = i * qlib->nworkerspershep;
        sheps[i].shep_dists      = qt_calloc(nshepherds, sizeof(unsigned int));
        sheps[i].sorted_sheplist = qt_calloc(nshepherds - 1,
                                             sizeof(qthread_shepherd_id_t));
        for (size_t j = 0, k = 0; j < nshepherds; ++j) {
            if (j != i) {
                sheps[i].shep_dists[j]        = 10;
                sheps[i].sorted_sheplist[k++] = j;
            }
        }
        // no need to sort; they're all equidistant
        shuffle_sheps(sheps[i].sorted_sheplist, nshepherds - 1);
    }
    /* there is no inherent way to detect distances, so unfortunately we must assume that they're all equidistant */
    return QTHREAD_SUCCESS;
} /*}}}*/

/* vim:set expandtab: */
