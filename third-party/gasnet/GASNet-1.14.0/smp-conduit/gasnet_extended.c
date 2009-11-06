/*   $Source: /var/local/cvs/gasnet/smp-conduit/gasnet_extended.c,v $
 *     $Date: 2009/09/18 23:33:46 $
 * $Revision: 1.2 $
 * Description: GASNet Extended API for smp-conduit
 * Copyright 2009, E. O. Lawrence Berekely National Laboratory
 * Terms of use are as specified in license.txt
 */

#include <gasnet_core_internal.h>

#ifndef _IN_GASNET_EXTENDED_C
#define _IN_GASNET_EXTENDED_C
#else
#error "#include loop detected"
#endif

/* ------------------------------------------------------------------------------------ */
#if GASNET_PSHM
/*
  Conduit-specifc Barrier "interface":
  ===================================
*/

static void gasnete_pshmbarrier_init(void);
static void gasnete_pshmbarrier_notify(gasnete_coll_team_t team, int id, int flags);
static int gasnete_pshmbarrier_wait(gasnete_coll_team_t team, int id, int flags);
static int gasnete_pshmbarrier_try(gasnete_coll_team_t team, int id, int flags);
static void dummy_fn(void) {}

#define GASNETE_BARRIER_DEFAULT "PSHM"
#define GASNETE_BARRIER_READENV() do { \
        if(GASNETE_ISBARRIER("PSHM")) gasnete_coll_default_barrier_type = GASNETE_COLL_BARRIER_PSHM; \
    } while (0)

#define GASNETE_BARRIER_INIT(TEAM, BARRIER_TYPE) do {       \
    if ((BARRIER_TYPE) == GASNETE_COLL_BARRIER_PSHM &&      \
        (TEAM) == GASNET_TEAM_ALL) {                        \
      (TEAM)->barrier_notify = &gasnete_pshmbarrier_notify; \
      (TEAM)->barrier_wait =   &gasnete_pshmbarrier_wait;   \
      (TEAM)->barrier_try =    &gasnete_pshmbarrier_try;    \
      gasnete_barrier_pf = &dummy_fn;                       \
      gasnete_pshmbarrier_init();                           \
    }                                                       \
  } while (0)

#endif /* GASNET_PSHM */
/* ------------------------------------------------------------------------------------ */

/* pull in the reference extended w/o any changes */
#include "extended-ref/gasnet_extended.c"

/* ------------------------------------------------------------------------------------ */
#if GASNET_PSHM
/*
  Conduit-specifc Barrier "implementation":
  ========================================
*/

/* The "phase" variable - alternates between 0 and 1
 * and prevents deadlock if 2 barriers are consecutievly
 * called */
static int gasnete_pshmbarrier_phase;

/* The "goal" variable - alternates between gasneti_nodes
 * and zero.  Indicates target counter value and avoids
 * the need to "reset" the barrier */
static int gasnete_pshmbarrier_goal;

static void gasnete_pshmbarrier_init(void) {
    gasnete_pshmbarrier_phase = 0;
    gasnete_pshmbarrier_goal = gasneti_nodes;

    /* Counter used to detect that all nodes have reached the barrier */
    gasneti_atomic_set(&gasneti_pshm_barrier->counter[0], 0, GASNETI_ATOMIC_REL);
    gasneti_atomic_set(&gasneti_pshm_barrier->counter[1], 0, 0);
}

static void gasnete_pshmbarrier_notify(gasnete_coll_team_t team, int id, int flags) {
  int phase;
  gasneti_sync_reads();
  phase = gasnete_pshmbarrier_phase;

  if_pf (team->barrier_info->barrier_splitstate == INSIDE_BARRIER) {
    gasneti_fatalerror("gasnet_barrier_notify() called twice in a row");
  } 

  /* Record the passed flag and value */
  gasneti_pshm_barrier->node[gasneti_mynode].flags[phase] = flags;
  gasneti_pshm_barrier->node[gasneti_mynode].value[phase] = id;
  
  /* Notify others that I have reached the barrier */
  if (gasnete_pshmbarrier_goal) { /* Counting UP */
    gasneti_atomic_increment(&gasneti_pshm_barrier->counter[phase], GASNETI_ATOMIC_REL);
  } else {
    (void)gasneti_atomic_decrement_and_test(&gasneti_pshm_barrier->counter[phase], GASNETI_ATOMIC_REL);
  }
  
  /* No sync_writes() needed due to REL, above */
  team->barrier_info->barrier_splitstate = INSIDE_BARRIER; 
}

static int finish_barrier(gasnete_coll_team_t team, int id, int flags, int phase) {
  int ret = GASNET_OK; /* assume success */
  int orig_flags = gasneti_pshm_barrier->node[gasneti_mynode].flags[phase];
  int orig_value = gasneti_pshm_barrier->node[gasneti_mynode].value[phase];
  
  /* Check all the conditions for mismatch */
  if_pf((flags != orig_flags) ||
	(!(flags & GASNET_BARRIERFLAG_ANONYMOUS) && (id != orig_value))) {
    ret = GASNET_ERR_BARRIER_MISMATCH; 
  } else {
    int have_value = 0;
    int value, i;

    for (i = 0; i < gasneti_nodes; ++i) {
      int flag = gasneti_pshm_barrier->node[i].flags[phase];
      if_pt (flag & GASNET_BARRIERFLAG_ANONYMOUS) {
        continue;
      } else if_pf (flag & GASNET_BARRIERFLAG_MISMATCH) {
        ret = GASNET_ERR_BARRIER_MISMATCH; 
        break;
      } else if (!have_value) {
        have_value = 1;
        value = gasneti_pshm_barrier->node[i].value[phase];
      } else if (value != gasneti_pshm_barrier->node[i].value[phase]) {
        ret = GASNET_ERR_BARRIER_MISMATCH; 
        break;
      }
    }
  }

  /* Switch the barrier variables.*/
  gasnete_pshmbarrier_phase = 1 ^ phase;
  if (phase) gasnete_pshmbarrier_goal = gasneti_nodes - gasnete_pshmbarrier_goal;
  team->barrier_info->barrier_splitstate = OUTSIDE_BARRIER;
  gasneti_sync_writes();

  return ret;
}

static int gasnete_pshmbarrier_wait(gasnete_coll_team_t team, int id, int flags) {
  gasneti_atomic_t *counter;
  int phase;

  gasneti_sync_reads();
  phase = gasnete_pshmbarrier_phase;

  if_pf (team->barrier_info->barrier_splitstate == OUTSIDE_BARRIER) {
    gasneti_fatalerror("gasnet_barrier_wait() called without a matching notify");
  }

  counter = &gasneti_pshm_barrier->counter[phase];
  if (gasnete_pshmbarrier_goal == gasneti_atomic_read(counter, 0)) {
    /* completed asynchronously before wait */ 
    GASNETI_TRACE_EVENT_TIME(B,BARRIER_ASYNC_COMPLETION,GASNETI_TICKS_NOW_IFENABLED(B)-gasnete_barrier_notifytime);
    gasneti_local_rmb();
  } else {
    /* Poll until all nodes have reached the barrier (polluntil includes the final RMB) */
    gasneti_polluntil(gasnete_pshmbarrier_goal == gasneti_atomic_read(counter, 0));
  }

  return finish_barrier(team, id, flags, phase);
}

static int gasnete_pshmbarrier_try(gasnete_coll_team_t team, int id, int flags) { 
  gasneti_atomic_t *counter;
  int phase;

  gasneti_sync_reads();
  phase = gasnete_pshmbarrier_phase;

  if_pf (team->barrier_info->barrier_splitstate == OUTSIDE_BARRIER) {
    gasneti_fatalerror("gasnet_barrier_try() called without a matching notify");
  }

  counter = &gasneti_pshm_barrier->counter[phase];
  return (gasnete_pshmbarrier_goal == gasneti_atomic_read(counter, GASNETI_ATOMIC_ACQ))
         ? finish_barrier(team, id, flags, phase) : GASNET_ERR_NOT_READY;
}
#endif /* GASNET_PSHM */
/* ------------------------------------------------------------------------------------ */
