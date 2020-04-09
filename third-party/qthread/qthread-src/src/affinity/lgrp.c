#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef HAVE_PROCESSOR_BIND
# include <sys/types.h>
# include <sys/processor.h>
# include <sys/procset.h>
# ifdef HAVE_SYS_LGRP_USER_H
#  include <sys/lgrp_user.h>
# endif
#endif
#include <stdio.h>                     /* for perror() */
#include <stdlib.h>                    /* for malloc() */

#include "qt_visibility.h"
#include "qt_subsystems.h"
#include "qt_asserts.h"
//#include "qthread_innards.h"
#include "qt_affinity.h"
#include "qt_debug.h"

#include "shepcomp.h"
#include "shufflesheps.h"

static lgrp_cookie_t   lgrp_cookie;
static lgrp_id_t       mccoy_thread_home;
static lgrp_affinity_t mccoy_thread_home_affinity;

qthread_shepherd_id_t guess_num_shepherds(void);
qthread_worker_id_t guess_num_workers_per_shep(qthread_shepherd_id_t nshepherds);

static int lgrp_maxcpus(const lgrp_id_t lgrp,
                        int             cpu_max)
{                                      /*{{{ */
    int nchildren, ncpus =
        lgrp_cpus(lgrp_cookie, lgrp, NULL, 0, LGRP_CONTENT_DIRECT);

    if (ncpus == -1) {
        return cpu_max;
    } else if ((ncpus > 0) && (ncpus > cpu_max)) {
        cpu_max = ncpus;
    }
    nchildren = lgrp_children(lgrp_cookie, lgrp, NULL, 0);
    if (nchildren == -1) {
        return cpu_max;
    } else if (nchildren > 0) {
        int        i;
        int nchildren_save = nchildren;
        lgrp_id_t *children = MALLOC(nchildren * sizeof(lgrp_id_t));

        nchildren = lgrp_children(lgrp_cookie, lgrp, children, nchildren);
        if (nchildren == -1) {
            qthread_debug(ALWAYS_OUTPUT, "hardware giving inconsistent answers!\n");
            abort();
            return cpu_max;
        }
        for (i = 0; i < nchildren; i++) {
            cpu_max = lgrp_maxcpus(children[i], cpu_max);
        }
        FREE(children, nchildren * sizeof(lgrp_id_t));
    }
    return cpu_max;
}                                      /*}}} */

static int lgrp_walk(const lgrp_id_t lgrp,
                     processorid_t **cpus,
                     lgrp_id_t      *lgrp_ids,
                     int             cpu_grps)
{                                      /*{{{ */
    int nchildren, ncpus =
        lgrp_cpus(lgrp_cookie, lgrp, NULL, 0, LGRP_CONTENT_DIRECT);

    if (ncpus == -1) {
        return cpu_grps;
    } else if (ncpus > 0) {
        processorid_t *cpuids = MALLOC((ncpus + 1) * sizeof(processorid_t));

        ncpus =
            lgrp_cpus(lgrp_cookie, lgrp, cpuids, ncpus, LGRP_CONTENT_DIRECT);
        if (ncpus == -1) {
            qthread_debug(ALWAYS_OUTPUT, "hardware giving inconsistent answers!\n");
            abort();
            return cpu_grps;
        }
        cpuids[ncpus] = -1;
        if (cpus) {
            cpus[cpu_grps] = cpuids;
        }
        if (lgrp_ids) {
            lgrp_ids[cpu_grps] = lgrp;
        }
        cpu_grps++;
    }
    nchildren = lgrp_children(lgrp_cookie, lgrp, NULL, 0);
    if (nchildren == -1) {
        return cpu_grps;
    } else if (nchildren > 0) {
        int        i;
        lgrp_id_t *children = MALLOC(nchildren * sizeof(lgrp_id_t));

        nchildren = lgrp_children(lgrp_cookie, lgrp, children, nchildren);
        if (nchildren == -1) {
            qthread_debug(ALWAYS_OUTPUT, "hardware giving inconsistent answers!\n");
            abort();
            return cpu_grps;
        }
        for (i = 0; i < nchildren; i++) {
            cpu_grps = lgrp_walk(children[i], cpus, lgrp_ids, cpu_grps);
        }
        FREE(children, nchildren * sizeof(lgrp_id_t));
    }
    return cpu_grps;
}                                      /*}}} */

static void qt_affinity_internal_lgrp_teardown(void)
{
    lgrp_affinity_set(P_LWPID, P_MYID, mccoy_thread_home, mccoy_thread_home_affinity);
}

void INTERNAL qt_affinity_init(qthread_shepherd_id_t *nbshepherds,
                               qthread_worker_id_t   *nbworkers,
                               size_t                *hw_par)
{                                      /*{{{ */
    lgrp_cookie                = lgrp_init(LGRP_VIEW_OS);
    mccoy_thread_home          = lgrp_home(P_LWPID, P_MYID);
    mccoy_thread_home_affinity = lgrp_affinity_get(P_LWPID, P_MYID, mccoy_thread_home);
    qthread_internal_cleanup(qt_affinity_internal_lgrp_teardown);
    if (*nbshepherds == 0) {
        *nbshepherds = guess_num_shepherds();
    }
    if (*nbworkers == 0) {
        *nbworkers = guess_num_workers_per_shep(*nbshepherds);
    }
}                                      /*}}} */

qthread_shepherd_id_t INTERNAL guess_num_shepherds(void)
{                                      /*{{{ */
    qthread_shepherd_id_t guess = 1;

    guess = lgrp_walk(lgrp_root(lgrp_cookie), NULL, NULL, 0);
    if (guess <= 0) {
        guess = 1;
    }
    qthread_debug(AFFINITY_DETAILS, "guessing %i shepherds\n", (int)guess);
    return guess;
}                                      /*}}} */

void INTERNAL qt_affinity_set(qthread_worker_t *me, unsigned int Q_UNUSED(nw))
{                                      /*{{{ */
    /* if this seems wrong, first answer: why should workers have more than socket affinity? */
    qthread_debug(AFFINITY_DETAILS, "set shep %i worker %i to lgrp %i\n",
                  (int)me->shepherd->shepherd_id, (int)me->worker_id,
                  (int)me->shepherd->lgrp);
    if (lgrp_affinity_set
            (P_LWPID, P_MYID, me->shepherd->lgrp, LGRP_AFF_STRONG) != 0) {
        perror("lgrp_affinity_set");
    }
}                                      /*}}} */

qthread_worker_id_t INTERNAL guess_num_workers_per_shep(qthread_shepherd_id_t nshepherds)
{                                      /*{{{ */
    unsigned int guess     = 1;
    int          tot_nodes = lgrp_walk(lgrp_root(lgrp_cookie), NULL, NULL, 0);

    guess = lgrp_maxcpus(lgrp_root(lgrp_cookie), 0);

    qthread_debug(AFFINITY_DETAILS,
                  "guessing num workers for %i sheps (nodes:%i max:%i)\n",
                  (int)nshepherds, tot_nodes, (int)guess);

    if (nshepherds > tot_nodes) {
        guess /= (nshepherds / tot_nodes);
    }
    if (guess == 0) {
        guess = 1;
    }

    qthread_debug(AFFINITY_DETAILS, "guessing %i workers per shep\n", (int)guess);
    return guess;
}                                      /*}}} */

int INTERNAL qt_affinity_gendists(qthread_shepherd_t   *sheps,
                                  qthread_shepherd_id_t nshepherds)
{                                      /*{{{ */
    unsigned int    lgrp_offset;
    int             lgrp_count_grps;
    processorid_t **cpus     = NULL;
    lgrp_id_t      *lgrp_ids = NULL;

    switch (lgrp_cookie) {
        case EINVAL:
        case ENOMEM:
            qthread_debug(AFFINITY_DETAILS, "lgrp_cookie is invalid!\n");
            return QTHREAD_THIRD_PARTY_ERROR;
    }
    {
        size_t max_lgrps = lgrp_nlgrps(lgrp_cookie);

        if (max_lgrps <= 0) {
            qthread_debug(AFFINITY_DETAILS, "max_lgrps is <= zero! (%i)\n",
                          max_lgrps);
            return QTHREAD_THIRD_PARTY_ERROR;
        }
        cpus = qt_calloc(max_lgrps, sizeof(processorid_t *));
        assert(cpus);
        lgrp_ids = qt_calloc(max_lgrps, sizeof(lgrp_id_t));
        assert(lgrp_ids);
    }
    lgrp_count_grps = lgrp_walk(lgrp_root(lgrp_cookie), cpus, lgrp_ids, 0);
    if (lgrp_count_grps <= 0) {
        qthread_debug(AFFINITY_DETAILS, "lgrp_count_grps is <= zero ! (%i)\n",
                      lgrp_count_grps);
        return QTHREAD_THIRD_PARTY_ERROR;
    }
    for (qthread_shepherd_id_t i = 0; i < nshepherds; i++) {
        /* first, pick a lgrp/node */
        int          cpu;
        unsigned int first_loff;

        first_loff    = lgrp_offset = i % lgrp_count_grps;
        sheps[i].node = -1;
        sheps[i].lgrp = -1;
        /* now pick an available CPU */
        while (1) {
            cpu = 0;
            /* find an unused one */
            while (cpus[lgrp_offset][cpu] != (processorid_t)(-1)) cpu++;
            if (cpu == 0) {
                /* if no unused ones... try the next lgrp */
                lgrp_offset++;
                lgrp_offset *= (lgrp_offset < lgrp_count_grps);
                if (lgrp_offset == first_loff) {
                    break;
                }
            } else {
                /* found one! */
                cpu--;
                sheps[i].node          = cpus[lgrp_offset][cpu];
                sheps[i].lgrp          = lgrp_ids[lgrp_offset];
                cpus[lgrp_offset][cpu] = -1;
                break;
            }
        }
    }
    for (qthread_shepherd_id_t i = 0; i < nshepherds; i++) {
        const unsigned int node_i = sheps[i].lgrp;
        size_t             j;
        sheps[i].shep_dists = qt_calloc(nshepherds, sizeof(unsigned int));
        assert(sheps[i].shep_dists);
        for (j = 0; j < nshepherds; j++) {
            const unsigned int node_j = sheps[j].lgrp;

            if ((node_i != QTHREAD_NO_NODE) && (node_j != QTHREAD_NO_NODE)) {
                int ret = lgrp_latency_cookie(lgrp_cookie, node_i, node_j,
                                              LGRP_LAT_CPU_TO_MEM);

                if (ret < 0) {
                    assert(ret >= 0);
                    return QTHREAD_THIRD_PARTY_ERROR;
                } else {
                    sheps[i].shep_dists[j] = (unsigned int)ret;
                }
            } else {
                /* XXX too arbitrary */
                if (i == j) {
                    sheps[i].shep_dists[j] = 12;
                } else {
                    sheps[i].shep_dists[j] = 18;
                }
            }
        }
    }
    for (qthread_shepherd_id_t i = 0; i < nshepherds; i++) {
        size_t j, k = 0;

        sheps[i].sorted_sheplist = qt_calloc(nshepherds - 1,
                                             sizeof(qthread_shepherd_id_t));
        assert(sheps[i].sorted_sheplist);
        for (j = 0; j < nshepherds; j++) {
            if (j != i) {
                sheps[i].sorted_sheplist[k++] = j;
            }
        }
        if (nshepherds > 1) {
            sort_sheps(sheps[i].shep_dists, sheps[i].sorted_sheplist, nshepherds);
        }
    }
    if (cpus) {
        for (int i = 0; i < lgrp_count_grps; i++) {
            qt_free(cpus[i]);
        }
        qt_free(cpus);
    }
    if (lgrp_ids) {
        qt_free(lgrp_ids);
    }
    return QTHREAD_SUCCESS;
}                                      /*}}} */

/* vim:set expandtab: */
