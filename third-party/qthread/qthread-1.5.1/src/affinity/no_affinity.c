#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "qthread_innards.h"
#include "qt_affinity.h"

qthread_shepherd_id_t guess_num_shepherds(
    void);
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
qthread_worker_id_t guess_num_workers_per_shep(
    qthread_shepherd_id_t nshepherds);
#endif

void qt_affinity_init(
    qthread_shepherd_id_t * nbshepherds
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    ,
    qthread_worker_id_t * nbworkers
#endif
    )
{				       /*{{{ */
    if (*nbshepherds == 0) {
	*nbshepherds = guess_num_shepherds();
	if (*nbshepherds <= 0) {
	    *nbshepherds = 1;
	}
    }
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    if (*nbworkers == 0) {
	*nbworkers = guess_num_workers_per_shep(*nbshepherds);
	if (*nbworkers <= 0) {
	    *nbworkers = 1;
	}
    }
#endif
}				       /*}}} */

qthread_shepherd_id_t guess_num_shepherds(
    void)
{				       /*{{{ */
#if defined(HAVE_SYSCONF) && defined(_SC_NPROCESSORS_CONF)	/* Linux */
    long ret = sysconf(_SC_NPROCESSORS_CONF);
    qthread_debug(ALL_DETAILS, "based on sysconf(), guessing %i shepherds\n",
		  (int)ret);
    return (ret > 0) ? ret : 1;
#elif defined(HAVE_SYSCTL) && defined(CTL_HW) && defined(HW_NCPU)
    int name[2] = { CTL_HW, HW_NCPU };
    uint32_t oldv;
    size_t oldvlen = sizeof(oldv);
    if (sysctl(name, 2, &oldv, &oldvlen, NULL, 0) >= 0) {
	assert(oldvlen == sizeof(oldv));
	qthread_debug(ALL_DETAILS,
		      "based on sysctl(), guessing %i shepherds\n",
		      (int)oldv);
	return oldv;
    }
#endif
    qthread_debug(ALL_DETAILS,
		  "no useful interfaces present; assuming a single shepherd\n");
    return 1;
}				       /*}}} */

#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
void qt_affinity_set(
    qthread_worker_t * me)
{
}
#else
void qt_affinity_set(
    qthread_shepherd_t * me)
{
}
#endif

#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
unsigned int guess_num_workers_per_shep(
    qthread_shepherd_id_t nshepherds)
{				       /*{{{ */
    size_t num_procs = 1;
    size_t guess = 1;
    qthread_debug(ALL_DETAILS, "guessing workers for %i shepherds\n",
		  (int)nshepherds);
#if defined(HAVE_SYSCONF) && defined(_SC_NPROCESSORS_CONF)	/* Linux */
    long ret = sysconf(_SC_NPROCESSORS_CONF);
    qthread_debug(ALL_DETAILS, "sysconf() says %i processors\n", (int)ret);
    num_procs = (ret > 0) ? (size_t) ret : 1;
#elif defined(HAVE_SYSCTL) && defined(CTL_HW) && defined(HW_NCPU)
    int name[2] = { CTL_HW, HW_NCPU };
    uint32_t oldv;
    size_t oldvlen = sizeof(oldv);
    if (sysctl(name, 2, &oldv, &oldvlen, NULL, 0) >= 0) {
	assert(oldvlen == sizeof(oldv));
	qthread_debug(ALL_DETAILS, "sysctl() says %i CPUs\n", (int)oldv);
	num_procs = (size_t) oldv;
    }
#endif
    guess = num_procs / nshepherds;
    if (guess == 0) {
	guess = 1;
    }
    qthread_debug(ALL_DETAILS, "guessing %i workers per shepherd\n",
		  (int)guess);
    return (qthread_shepherd_id_t) guess;
}				       /*}}} */
#endif

int qt_affinity_gendists(
    qthread_shepherd_t * sheps,
    qthread_shepherd_id_t nshepherds)
{				       /*{{{ */
    return QTHREAD_SUCCESS;
}				       /*}}} */
