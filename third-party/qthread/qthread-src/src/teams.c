#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* The API */
#include "qthread/qthread.h"

/* System Headers */
#include <stdio.h>

/* Internal Headers */
#include "qt_teams.h"
#include "qt_subsystems.h"
#include "qt_qthread_struct.h"
#include "qt_qthread_mgmt.h"
#include "qt_visibility.h"
#include "qt_mpool.h"
#include "qthread_innards.h" /* for qlib */
#include "qt_initialized.h" // for qthread_library_initialized
#include "qt_expect.h"
#include "qt_asserts.h"
#include "qt_subsystems.h"
#include "qt_debug.h"

/* Memory management macros */
#if defined(UNPOOLED)
# define ALLOC_TEAM() (qt_team_t *)MALLOC(sizeof(qt_team_t))
# define FREE_TEAM(t) FREE(t, sizeof(qt_team_t))
#else
static qt_mpool generic_team_pool = NULL;
# define ALLOC_TEAM() (qt_team_t *)qt_mpool_alloc(generic_team_pool)
# define FREE_TEAM(t) qt_mpool_free(generic_team_pool, (t))
#endif

static void qt_internal_teams_shutdown(void);
static void qt_internal_teams_destroy(void);
#ifdef TEAM_PROFILE
static void qt_team_profile(void);
#endif

void INTERNAL qt_internal_teams_init(void)
{   /*{{{*/
#ifdef TEAM_PROFILE
    qlib->team_create  = 0;
    qlib->team_destroy = 0;

    qlib->team_subteam_create  = 0;
    qlib->team_subteam_destroy = 0;

    qlib->team_sinc_create  = 0;
    qlib->team_sinc_destroy = 0;

    qlib->team_subteams_sinc_create  = 0;
    qlib->team_subteams_sinc_destroy = 0;

    qlib->team_leader_start = 0;
    qlib->team_leader_stop  = 0;

    qlib->team_watcher_start = 0;
    qlib->team_watcher_stop  = 0;
#endif /* ifdef TEAM_PROFILE */
    qlib->max_team_id = 2;    /* team 1 is reserved for the default team */
    qlib->team_count  = 0;    /* count of existing teams */
    QTHREAD_FASTLOCK_INIT(qlib->max_team_id_lock);
    QTHREAD_FASTLOCK_INIT(qlib->team_count_lock);
#ifndef UNPOOLED
    generic_team_pool = qt_mpool_create(sizeof(qt_team_t));
#endif
    qthread_internal_cleanup(qt_internal_teams_shutdown);
    qthread_internal_cleanup_late(qt_internal_teams_destroy);
} /*}}}*/

void INTERNAL qt_internal_teams_reclaim(void)
{
    // Wait for all team structures to be reclaimed.
    while (qlib->team_count) {
#ifdef QTHREAD_DEBUG
        unsigned int ct = qlib->team_count;
#endif
        qthread_yield();
#ifdef QTHREAD_DEBUG
        if (ct != qlib->team_count) {
            printf("waiting for %u teams...\n", (unsigned int)qlib->team_count);
        }
#endif
    }
}

static void qt_internal_teams_shutdown(void)
{   /*{{{*/
#ifdef TEAM_PROFILE
    qt_team_profile();
#endif
} /*}}}*/

static void qt_internal_teams_destroy(void)
{   /*{{{*/
    qthread_debug(CORE_CALLS, "begin\n");
    QTHREAD_FASTLOCK_DESTROY(qlib->max_team_id_lock);
#ifndef UNPOOLED
    qt_mpool_destroy(generic_team_pool);
    generic_team_pool = NULL;
#endif
    qthread_debug(CORE_CALLS, "end\n");
} /*}}}*/

/* Returns the team id. If there is no team structure associated with the task,
 * it is considered to be in the default team with id 1. */
unsigned int API_FUNC qt_team_id(void)
{   /*{{{*/
    assert(qthread_library_initialized);
    qthread_t *t = qthread_internal_self();

    qthread_debug(THREAD_CALLS, "tid(%u), team_id(%u)\n", t ? t->thread_id : QTHREAD_NON_TASK_ID,
                  t ? (t->team ? t->team->team_id : QTHREAD_DEFAULT_TEAM_ID) : QTHREAD_NON_TEAM_ID);
    return t ? (t->team ? t->team->team_id : QTHREAD_DEFAULT_TEAM_ID) : QTHREAD_NON_TEAM_ID;
} /*}}}*/

/* Returns the parent team id. If there is no team structure associated with
 * the task, it is considered to be in the default team with id, and to have
 * no parent (id 0). */
unsigned int API_FUNC qt_team_parent_id(void)
{   /*{{{*/
    if (NULL != qlib) {
        qthread_t *self = qthread_internal_self();

        if ((NULL != self) && (NULL != self->team)) {
            return self->team->parent_id;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
} /*}}}*/

#ifdef TEAM_PROFILE
static void qt_team_profile(void)
{   /*{{{*/
    fprintf(stderr, "\n======== Teams Profile ========\n\n");
    fprintf(stderr, "Teams:\n");
    fprintf(stderr, "%8lu create\n",
            (unsigned long)qlib->team_create);
    fprintf(stderr, "%8lu destroy\n",
            (unsigned long)qlib->team_destroy);
    fprintf(stderr, "%8lu teams were not destroyed\n",
            (unsigned long)(qlib->team_create - qlib->team_destroy));

    fprintf(stderr, "\nSubteams:\n");
    fprintf(stderr, "%8lu subteam create\n",
            (unsigned long)qlib->team_subteam_create);
    fprintf(stderr, "%8lu subteam destroy\n",
            (unsigned long)qlib->team_subteam_destroy);
    fprintf(stderr, "%8lu subteams were not destroyed\n",
            (unsigned long)(qlib->team_subteam_create -
                            qlib->team_subteam_destroy));

    fprintf(stderr, "\nSincs:\n");
    fprintf(stderr, "%8lu sinc_create\n",
            (unsigned long)qlib->team_sinc_create);
    fprintf(stderr, "%8lu sinc_destroy\n",
            (unsigned long)qlib->team_sinc_destroy);
    fprintf(stderr, "%8lu sincs were not destroyed\n",
            (unsigned long)(qlib->team_sinc_create -
                            qlib->team_sinc_destroy));

    fprintf(stderr, "\nSubteams Sincs:\n");
    fprintf(stderr, "%8lu subteams_sinc_create\n",
            (unsigned long)qlib->team_subteams_sinc_create);
    fprintf(stderr, "%8lu subteams_sinc_destroy\n",
            (unsigned long)qlib->team_subteams_sinc_destroy);
    fprintf(stderr, "%8lu subteams sncs were not destroyed\n",
            (unsigned long)(qlib->team_subteams_sinc_create -
                            qlib->team_subteams_sinc_destroy));

    fprintf(stderr, "\nLeaders:\n");
    fprintf(stderr, "%8lu leader_start\n",
            (unsigned long)qlib->team_leader_start);
    fprintf(stderr, "%8lu leader_stop\n",
            (unsigned long)qlib->team_leader_stop);
    fprintf(stderr, "%8lu leaders did not stop\n",
            (unsigned long)(qlib->team_leader_start -
                            qlib->team_leader_stop));

    fprintf(stderr, "\nWatchers:\n");
    fprintf(stderr, "%8lu watcher_start\n",
            (unsigned long)qlib->team_watcher_start);
    fprintf(stderr, "%8lu watcher_stop\n",
            (unsigned long)qlib->team_watcher_stop);
    fprintf(stderr, "%8lu watchers did not stop\n",
            (unsigned long)(qlib->team_watcher_start -
                            qlib->team_watcher_stop));
    fprintf(stderr, "\n");

    fprintf(stderr, "Total current team count %8lu\n",
            (unsigned long)(qlib->team_count));
} /*}}}*/

#endif /* ifdef TEAM_PROFILE */

// This is called in `qthread_wrapper()` immediately after each team task
// returns.
void INTERNAL qt_internal_teamfinish(qt_team_t   *team,
                                     uint_fast8_t flags)
{   /*{{{*/
    assert(team != NULL);

    if (QTHREAD_TEAM_LEADER & flags) {
        // 1. This task is a sub/team leader

        if (QTHREAD_NON_TEAM_ID == team->parent_id) {
            // 1.1. This task is a team leader
            assert(team->sinc);
            assert(team->subteams_sinc);
            assert(NULL == team->parent_subteams_sinc);
            assert(NULL == team->parent_eureka);

            // Wait for all participants on team sinc after submitting to
            // team sinc for the leader
            qt_sinc_submit(team->sinc, NULL);
            qt_sinc_wait(team->sinc, NULL);

            // Wait for all participants on team subteams sinc
            qt_sinc_submit(team->subteams_sinc, NULL);
            qt_sinc_wait(team->subteams_sinc, NULL);

            // Clean up
            qt_sinc_destroy(team->sinc);
            team->sinc = NULL;
            qt_sinc_destroy(team->subteams_sinc);
            team->subteams_sinc = NULL;
            qthread_debug(FEB_DETAILS, "tid %u killing team %u, filling my own eureka (%p)\n", qthread_id(), team->team_id, &team->eureka);
            qthread_fill(&team->eureka);

            FREE_TEAM(team);

            qthread_internal_incr(&(qlib->team_count), &qlib->team_count_lock, -1);

#ifdef TEAM_PROFILE
            qthread_incr(&qlib->team_destroy, 1);
#endif
        } else {
            // 1.2. This task is a subteam leader
            assert(team->sinc);
            assert(team->subteams_sinc);

            if (QTHREAD_DEFAULT_TEAM_ID == team->parent_id) {
                // 1.2.1. This is a subteam of the default team
                assert(NULL == team->parent_eureka);
                assert(NULL == team->parent_subteams_sinc);

                // Wait for all participants on team sinc after subtmitting
                // to team sinc - there is no watcher
                qt_sinc_submit(team->sinc, NULL);
                qt_sinc_wait(team->sinc, NULL);

                // Wait for all participants on team subteams sinc
                qt_sinc_submit(team->subteams_sinc, NULL);
                qt_sinc_wait(team->subteams_sinc, NULL);

                // Clean up
                qt_sinc_destroy(team->sinc);
                team->sinc = NULL;
                qt_sinc_destroy(team->subteams_sinc);
                team->subteams_sinc = NULL;
            } else {
                // 1.2.2. This is a subteam of a non-default team
                assert(team->parent_eureka);
                assert(team->parent_subteams_sinc);

                // Wait for all participants on team sinc after subtmitting
                // to team sinc for leader and watcher; reset team sinc
                // for watcher
                qt_sinc_submit(team->sinc, NULL);
                qt_sinc_submit(team->sinc, NULL);
                qt_sinc_wait(team->sinc, NULL);

                // Wait for all participants on team subteams sinc
                qt_sinc_submit(team->subteams_sinc, NULL);
                qt_sinc_wait(team->subteams_sinc, NULL);

                if (team->watcher_started == 0) {
                    // Signal watcher to exit and wait for it
                    qt_sinc_reset(team->sinc, 1);
                    qthread_debug(FEB_DETAILS, "tid %u killing team %u signalling team %u's watcher (%p)\n", qthread_id(), team->team_id, team->parent_id, team->parent_eureka);
                    qthread_writeEF_const(team->parent_eureka,
                            TEAM_SIGNAL_EXIT(team->team_id));
                    qt_sinc_wait(team->sinc, NULL);
                }

                // Submit to parent team subteams sinc
                qt_sinc_submit(team->parent_subteams_sinc, NULL);

                // Clean up
                qt_sinc_destroy(team->sinc);
                team->sinc = NULL;
                qt_sinc_destroy(team->subteams_sinc);
                team->subteams_sinc = NULL;

                team->parent_eureka        = NULL;
                team->parent_subteams_sinc = NULL;
            }

            qthread_debug(FEB_DETAILS, "tid %u killing team %u, filling my own eureka (%p)\n", qthread_id(), team->team_id, &team->eureka);
            qthread_fill(&team->eureka);

            FREE_TEAM(team);

            qthread_internal_incr(&(qlib->team_count), &qlib->team_count_lock, -1);

#ifdef TEAM_PROFILE
            qthread_incr(&qlib->team_subteam_destroy, 1);
#endif
        }

#ifdef TEAM_PROFILE
        qthread_incr(&qlib->team_sinc_destroy, 1);
        qthread_incr(&qlib->team_subteams_sinc_destroy, 1);
        qthread_incr(&qlib->team_leader_stop, 1);
#endif
    } else {
        // 2. This task is not a sub/team leader: a watcher or participant
        assert(team);

        // Submit to the team sinc
        qt_sinc_submit(team->sinc, NULL);
    }
} /*}}}*/

static aligned_t qt_team_watcher(void *args_)
{   /*{{{*/
    aligned_t    code   = 0;
    unsigned int myteam = qt_team_id();

    qt_team_t *team = (qt_team_t *)args_;

    assert(team);
    assert(myteam == team->team_id);

    aligned_t *parent_eureka = team->parent_eureka;
    assert(parent_eureka);

    qthread_debug(FEB_DETAILS, "watcher (tid %u) of team %u filling watcher_started (%p)\n", qthread_id(), myteam, &team->watcher_started);
    qthread_fill(&team->watcher_started);
#ifdef TEAM_PROFILE
    qthread_incr(&qlib->team_watcher_start, 1);
#endif

    do {
        qthread_debug(FEB_DETAILS, "team %u's watcher (tid %u) waiting for a eureka or a team exit (%p)\n", myteam, qthread_id(), parent_eureka);
        qthread_readFF(&code, parent_eureka);

        if (TEAM_SIGNAL_ISEXIT(code)) {
            if (myteam == TEAM_SIGNAL_SENDERID(code)) {
                // Reset the FEB and exit
                qthread_debug(FEB_DETAILS, "team %u's watcher (tid %u) preparing to exit, emptying parent's eureka (%p)\n", myteam, qthread_id(), parent_eureka);
                qthread_empty(parent_eureka);
                break;
            } else {
                // Yield control of the resource while waiting for FEB to be reset
                qthread_yield();
            }
        } else if (TEAM_SIGNAL_ISEUREKA(code)) {
            if (myteam == TEAM_SIGNAL_SENDERID(code)) {
                // I must survive this eureka: I am the waiter, the Beholder!
                assert("Error: why was the eureka propogated UPWARD?!?" && 0);
            } else {
                // I must propogate this eureka: I am the end of all things, and like a Shoggoth, I will sweep my team evilly free of litter
                qthread_debug(FEB_DETAILS, "team %u's watcher (tid %u) preparing to destroy its team\n", myteam, qthread_id());
#ifdef QTHREAD_USE_EUREKAS
                qt_team_eureka();
#endif /* QTHREAD_USE_EUREKAS */
                team->watcher_started = 1; // signal that the watcher doesn't need to be killed
                qthread_empty(parent_eureka);
                break;
            }
        } else {
            assert("Error: watcher received code 0 or 1" && 0); // not sure what this means
        }
    } while (1);

#ifdef TEAM_PROFILE
    qthread_incr(&qlib->team_watcher_stop, 1);
#endif

    return 0;
} /*}}}*/

qt_team_t INTERNAL *qt_internal_team_new(void *restrict      ret,
                                         unsigned int        feature_flag,
                                         qt_team_t *restrict curr_team,
                                         unsigned int        parent_id)
{   /*{{{*/
    DEBUG_ONLY(qthread_t * me = qthread_internal_self());

    // Allocate new team structure
    qt_team_t *new_team = ALLOC_TEAM();
    assert(new_team);

    // Initialize new team values
    new_team->team_id         = qthread_internal_incr(&(qlib->max_team_id), &qlib->max_team_id_lock, 1);
    new_team->eureka          = QTHREAD_NON_TASK_ID;
    new_team->eureka_lock     = 0;
    new_team->watcher_started = 0;
    new_team->sinc            = qt_sinc_create(0, NULL, NULL, 1);
    assert(new_team->sinc);
    new_team->subteams_sinc = qt_sinc_create(0, NULL, NULL, 1);
    assert(new_team->subteams_sinc);
    new_team->parent_id            = parent_id;
    new_team->parent_eureka        = NULL;
    new_team->parent_subteams_sinc = NULL;
    new_team->return_loc           = ret;
    new_team->flags                = feature_flag & QTHREAD_RET_MASK;

    if (QTHREAD_UNLIKELY(new_team->team_id == QTHREAD_NULL_TEAM_ID)) {
        new_team->team_id = qthread_internal_incr(&(qlib->max_team_id), &qlib->max_team_id_lock, 3);
    }

    // Empty new team FEBs
    qthread_debug(FEB_DETAILS, "tid %i emptying NEW team %u's eureka (%p)\n", me ? ((int)me->thread_id) : -1, new_team->team_id, &new_team->eureka);
    qthread_empty(&new_team->eureka);

    if (curr_team) {
        new_team->parent_id     = curr_team->team_id;
        new_team->parent_eureka = &curr_team->eureka;
        assert(new_team->parent_eureka);
        new_team->parent_subteams_sinc = curr_team->subteams_sinc;
        assert(new_team->parent_subteams_sinc);

        // Notify the parent of the new subteam
        qt_sinc_expect(new_team->parent_subteams_sinc, 1);
    }

#ifdef TEAM_PROFILE
    qthread_incr(&qlib->team_sinc_create, 1);
    qthread_incr(&qlib->team_subteams_sinc_create, 1);
    qthread_incr(&qlib->team_create, 1);
#endif
    return new_team;
} /*}}}*/

void INTERNAL qt_internal_subteam_leader(qthread_t *t)
{   /*{{{*/
    qthread_debug(FEB_DETAILS, "tid %u emptying team %u's watcher_started (%p)\n", t->thread_id, t->team->team_id, &t->team->watcher_started);
    qthread_empty(&t->team->watcher_started);
    qthread_fork(qt_team_watcher, t->team, NULL);
    qthread_readFF(NULL, &t->team->watcher_started);
} /*}}}*/

/* vim:set expandtab: */
