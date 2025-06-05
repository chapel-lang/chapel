/* The API */
#include "qthread/qthread.h"

/* System Headers */
#include <stdio.h>

/* Internal Headers */
#include "qt_asserts.h"
#include "qt_expect.h"
#include "qt_initialized.h" // for qthread_library_initialized
#include "qt_mpool.h"
#include "qt_qthread_mgmt.h"
#include "qt_qthread_struct.h"
#include "qt_subsystems.h"
#include "qt_teams.h"
#include "qt_visibility.h"
#include "qthread_innards.h" /* for qlib */

/* Memory management macros */
static qt_mpool generic_team_pool = NULL;
#define ALLOC_TEAM() (qt_team_t *)qt_mpool_alloc(generic_team_pool)
#define FREE_TEAM(t) qt_mpool_free(generic_team_pool, (t))

static void qt_internal_teams_shutdown(void);
static void qt_internal_teams_destroy(void);

void INTERNAL qt_internal_teams_init(void) {
  qlib->max_team_id = 2; /* team 1 is reserved for the default team */
  qlib->team_count = 0;  /* count of existing teams */
  QTHREAD_FASTLOCK_INIT(qlib->max_team_id_lock);
  QTHREAD_FASTLOCK_INIT(qlib->team_count_lock);
  generic_team_pool = qt_mpool_create(sizeof(qt_team_t));
  qthread_internal_cleanup(qt_internal_teams_shutdown);
  qthread_internal_cleanup_late(qt_internal_teams_destroy);
}

void INTERNAL qt_internal_teams_reclaim(void) {
  // Wait for all team structures to be reclaimed.
  while (qlib->team_count) { qthread_yield(); }
}

static void qt_internal_teams_shutdown(void) {}

static void qt_internal_teams_destroy(void) {
  QTHREAD_FASTLOCK_DESTROY(qlib->max_team_id_lock);
  qt_mpool_destroy(generic_team_pool);
  generic_team_pool = NULL;
}

/* Returns the team id. If there is no team structure associated with the task,
 * it is considered to be in the default team with id 1. */
unsigned int API_FUNC qt_team_id(void) {
  assert(qthread_library_initialized);
  qthread_t *t = qthread_internal_self();

  return t ? (t->team ? t->team->team_id : QTHREAD_DEFAULT_TEAM_ID)
           : QTHREAD_NON_TEAM_ID;
}

/* Returns the parent team id. If there is no team structure associated with
 * the task, it is considered to be in the default team with id, and to have
 * no parent (id 0). */
unsigned int API_FUNC qt_team_parent_id(void) {
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
}

// This is called in `qthread_wrapper()` immediately after each team task
// returns.
void INTERNAL qt_internal_teamfinish(qt_team_t *team, uint_fast8_t flags) {
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
      qthread_fill(&team->eureka);

      FREE_TEAM(team);

      qthread_internal_incr(
        &(qlib->team_count), &qlib->team_count_lock, (aligned_t)-1);
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

        team->parent_eureka = NULL;
        team->parent_subteams_sinc = NULL;
      }

      qthread_fill(&team->eureka);

      FREE_TEAM(team);

      qthread_internal_incr(
        &(qlib->team_count), &qlib->team_count_lock, (aligned_t)-1);
    }

  } else {
    // 2. This task is not a sub/team leader: a watcher or participant
    assert(team);

    // Submit to the team sinc
    qt_sinc_submit(team->sinc, NULL);
  }
}

static aligned_t qt_team_watcher(void *args_) {
  aligned_t code = 0;
  unsigned int myteam = qt_team_id();

  qt_team_t *team = (qt_team_t *)args_;

  assert(team);
  assert(myteam == team->team_id);

  aligned_t *parent_eureka = team->parent_eureka;
  assert(parent_eureka);

  qthread_fill(&team->watcher_started);

  do {
    qthread_readFF(&code, parent_eureka);

    if (TEAM_SIGNAL_ISEXIT(code)) {
      if (myteam == TEAM_SIGNAL_SENDERID(code)) {
        // Reset the FEB and exit
        qthread_empty(parent_eureka);
        break;
      } else {
        // Yield control of the resource while waiting for FEB to be reset
        qthread_yield();
      }
    } else {
      assert("Error: watcher received code 0 or 1" &&
             0); // not sure what this means
    }
  } while (1);

  return 0;
}

qt_team_t INTERNAL *qt_internal_team_new(void *restrict ret,
                                         unsigned int feature_flag,
                                         qt_team_t *restrict curr_team,
                                         unsigned int parent_id) {
  // Allocate new team structure
  qt_team_t *new_team = ALLOC_TEAM();
  assert(new_team);

  // Initialize new team values
  new_team->team_id =
    qthread_internal_incr(&(qlib->max_team_id), &qlib->max_team_id_lock, 1);
  new_team->eureka = QTHREAD_NON_TASK_ID;
  new_team->eureka_lock = 0;
  new_team->watcher_started = 0;
  new_team->sinc = qt_sinc_create(0, NULL, NULL, 1);
  assert(new_team->sinc);
  new_team->subteams_sinc = qt_sinc_create(0, NULL, NULL, 1);
  assert(new_team->subteams_sinc);
  new_team->parent_id = parent_id;
  new_team->parent_eureka = NULL;
  new_team->parent_subteams_sinc = NULL;
  new_team->return_loc = ret;
  new_team->flags = feature_flag & QTHREAD_RET_MASK;

  if (QTHREAD_UNLIKELY(new_team->team_id == QTHREAD_NULL_TEAM_ID)) {
    new_team->team_id =
      qthread_internal_incr(&(qlib->max_team_id), &qlib->max_team_id_lock, 3);
  }

  // Empty new team FEBs
  qthread_empty(&new_team->eureka);

  if (curr_team) {
    new_team->parent_id = curr_team->team_id;
    new_team->parent_eureka = &curr_team->eureka;
    assert(new_team->parent_eureka);
    new_team->parent_subteams_sinc = curr_team->subteams_sinc;
    assert(new_team->parent_subteams_sinc);

    // Notify the parent of the new subteam
    qt_sinc_expect(new_team->parent_subteams_sinc, 1);
  }

  return new_team;
}

void INTERNAL qt_internal_subteam_leader(qthread_t *t) {
  qthread_empty(&t->team->watcher_started);
  qthread_fork(qt_team_watcher, t->team, NULL);
  qthread_readFF(NULL, &t->team->watcher_started);
}

/* vim:set expandtab: */
