#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdio.h>
#ifdef HAVE_SYSCTL
# ifdef HAVE_SYS_TYPES_H
#  include <sys/types.h>
# endif
# ifdef HAVE_SYS_SYSCTL_H
#  include <sys/sysctl.h>
# endif
#endif
#ifdef HAVE_MACH_THREAD_POLICY_H
# include <mach/mach_init.h>
# include <mach/thread_policy.h>
kern_return_t thread_policy_set(
    thread_t thread,
    thread_policy_flavor_t flavor,
    thread_policy_t policy_info,
    mach_msg_type_number_t count);
kern_return_t thread_policy_get(
    thread_t thread,
    thread_policy_flavor_t flavor,
    thread_policy_t policy_info,
    mach_msg_type_number_t * count,
    boolean_t * get_default);
#endif

#include "qthread_asserts.h"
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
    }
#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
    if (*nbworkers == 0) {
	*nbworkers = guess_num_workers_per_shep(*nbshepherds);
    }
#endif
}				       /*}}} */

qthread_shepherd_id_t guess_num_shepherds(
    void)
{				       /*{{{ */
    qthread_shepherd_id_t nshepherds = 1;
#if defined(HAVE_SYSCTL) && defined(CTL_HW) && defined(HW_NCPU)
    int name[2] = { CTL_HW, HW_NCPU };
    uint32_t oldv;
    size_t oldvlen = sizeof(oldv);
    if (sysctl(name, 2, &oldv, &oldvlen, NULL, 0) < 0) {
	/* sysctl is the official query mechanism on Macs, so if it failed,
	 * we want to know */
	perror("sysctl");
    } else {
	assert(oldvlen == sizeof(oldv));
	nshepherds = (qthread_shepherd_id_t) oldv;
    }
#endif
    if (nshepherds <= 0) {
	nshepherds = 1;
    }
    return nshepherds;
}				       /*}}} */

#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
void qt_affinity_set(
    qthread_worker_t * me)
{				       /*{{{ */
#ifndef SST
    mach_msg_type_number_t Count = THREAD_AFFINITY_POLICY_COUNT;
    thread_affinity_policy_data_t mask[THREAD_AFFINITY_POLICY_COUNT];

    memset(mask, 0,
	   sizeof(thread_affinity_policy_data_t) *
	   THREAD_AFFINITY_POLICY_COUNT);
    mask[0].affinity_tag = me->packed_worker_id + 1;
    Count = 1;
    if (thread_policy_set
	(mach_thread_self(), THREAD_AFFINITY_POLICY, (thread_policy_t) & mask,
	 Count) != KERN_SUCCESS) {
	fprintf(stderr, "ERROR! Cannot SET affinity for some reason\n");
    }
#endif
}				       /*}}} */
#else
void qt_affinity_set(
    qthread_shepherd_t * me)
{				       /*{{{ */
#ifndef SST
    mach_msg_type_number_t Count = THREAD_AFFINITY_POLICY_COUNT;
    thread_affinity_policy_data_t mask[THREAD_AFFINITY_POLICY_COUNT];

    /*
     * boolean_t GetDefault = 0;
     * if (thread_policy_get(mach_thread_self(),
     * THREAD_AFFINITY_POLICY,
     * (thread_policy_t)&mask,
     * &Count,
     * &GetDefault) != KERN_SUCCESS) {
     * printf("ERROR! Cannot get affinity for some reason\n");
     * }
     * printf("THREAD_AFFINITY_POLICY: krc=%#x default=%d\n",
     * krc, GetDefault);
     * printf("\tcount=%i\n", Count);
     * for (int i=0; i<Count; i++) {
     * printf("\t\taffinity_tag=%d (%#x)\n",
     * mask[i].affinity_tag, mask[i].affinity_tag);
     * } */
    memset(mask, 0,
	   sizeof(thread_affinity_policy_data_t) *
	   THREAD_AFFINITY_POLICY_COUNT);
    mask[0].affinity_tag = me->shepherd_id + 1;
    Count = 1;
    if (thread_policy_set
	(mach_thread_self(), THREAD_AFFINITY_POLICY, (thread_policy_t) & mask,
	 Count) != KERN_SUCCESS) {
	fprintf(stderr, "ERROR! Cannot SET affinity for some reason\n");
    }
#endif
}				       /*}}} */
#endif

#ifdef QTHREAD_MULTITHREADED_SHEPHERDS
unsigned int guess_num_workers_per_shep(
    qthread_shepherd_id_t nshepherds)
{				       /*{{{ */
    return 1;
}				       /*}}} */
#endif

int qt_affinity_gendists(
    qthread_shepherd_t * sheps,
    qthread_shepherd_id_t nshepherds)
{				       /*{{{ */
    /* there is no native way to detect distances, so unfortunately we must assume that they're all equidistant */
    return QTHREAD_SUCCESS;
}				       /*}}} */
