/*   $Source: /var/local/cvs/gasnet/extended-ref/gasnet_extended_refbarrier.c,v $
 *     $Date: 2007/01/03 17:12:28 $
 * $Revision: 1.34 $
 * Description: Reference implemetation of GASNet Barrier, using Active Messages
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef GASNETI_GASNET_EXTENDED_REFBARRIER_C
  #error This file not meant to be compiled directly - included by gasnet_extended.c
#endif

#include <limits.h>

/*  TODO: add more reference barrier implementation options (bug 264) */

/* ------------------------------------------------------------------------------------ */
/* state shared between barrier implementations */

static enum { OUTSIDE_BARRIER, INSIDE_BARRIER } barrier_splitstate = OUTSIDE_BARRIER;
#if GASNETI_STATS_OR_TRACE
  gasneti_tick_t gasnete_barrier_notifytime; /* for statistical purposes */ 
#endif

/* ------------------------------------------------------------------------------------ */
/* the AM-based Dissemination implementation of barrier */

/*  an AM-based Dissemination barrier implementation:
     With N nodes, the barrier takes ceil(lg(N)) steps (lg = log-base-2).
     At step i (i=0..):
	node n first sends to node ((n + 2^i) mod N)
	then node n waits to receive (from node ((n + N - 2^i) mod N))
	once we receive for step i, we can move the step i+1 (or finish)
    The distributed nature makes this barrier more scalable than a centralized
     barrier, but also more sensitive to any lack of attentiveness to the
     network.
    We use a static allocation, limiting us to 2^GASNETE_AMBARRIER_MAXSTEP nodes.

    Algorithm is described in section 3.3 of
    John M. Mellor-Crummey and Michael L. Scott. "Algorithms for scalable synchronization
    on shared-memory multiprocessors." ACM ToCS, 9(1):21 65, 1991.
 */

#ifndef GASNETE_AMDBARRIER_MAXSTEP
  #define GASNETE_AMDBARRIER_MAXSTEP 32
#endif

static gasnet_hsl_t amdbarrier_lock = GASNET_HSL_INITIALIZER;
static int volatile amdbarrier_value; /*  local ambarrier value */
static int volatile amdbarrier_flags; /*  local ambarrier flags */
static int volatile amdbarrier_step = 0;  /*  local ambarrier step */
static int volatile amdbarrier_size = -1;  /*  ceil(lg(nodes)), or -1 if uninitialized */
static int volatile amdbarrier_phase = 0;  /*  2-phase operation to improve pipelining */
static int volatile amdbarrier_step_done[2][GASNETE_AMDBARRIER_MAXSTEP] = { { 0 } }; /*  non-zero when a step is complete */
static int volatile amdbarrier_mismatch[2] = { 0, 0 }; /*  non-zero if we detected a mismatch */
static int volatile amdbarrier_recv_value[2]; /*  consensus ambarrier value */
static int volatile amdbarrier_recv_value_present[2] = { 0, 0 }; /*  consensus ambarrier value is present */

static void gasnete_amdbarrier_init(void) {
  int i;
  int64_t j;

  gasneti_assert(amdbarrier_size < 0);

  /* determine barrier size (number of steps) */
  for (i=0, j=1; j < gasneti_nodes; ++i, j*=2) ;

  amdbarrier_size = i;
  gasneti_assert(amdbarrier_size <= GASNETE_AMDBARRIER_MAXSTEP);
}

static void gasnete_amdbarrier_notify_reqh(gasnet_token_t token, 
  gasnet_handlerarg_t phase, gasnet_handlerarg_t step, gasnet_handlerarg_t value, gasnet_handlerarg_t flags) {

  gasnet_hsl_lock(&amdbarrier_lock);
  { 
    /* Note we might not receive the steps in the numbered order.
     * We record the value received on the first one to actually arrive.
     * In subsequent steps we check for mismatch of received values.
     * The local value is compared in the kick function.
     */
    if (!(flags & (GASNET_BARRIERFLAG_ANONYMOUS|GASNET_BARRIERFLAG_MISMATCH)) && 
        !amdbarrier_recv_value_present[phase]) {  /* first named value we've seen */
      amdbarrier_recv_value[phase] = (int)value;
      gasneti_sync_writes();
      amdbarrier_recv_value_present[phase] = 1;
    } else if ((flags & GASNET_BARRIERFLAG_MISMATCH) || /* explicit mismatch */
               (!(flags & GASNET_BARRIERFLAG_ANONYMOUS) && /* 2nd+ named value and mismatch */
                 amdbarrier_recv_value[phase] != (int)value)) {
      amdbarrier_mismatch[phase] = 1;
    }
    
    gasneti_assert(amdbarrier_step_done[phase][step] == 0);
    
    gasneti_sync_writes();
    amdbarrier_step_done[phase][step] = 1;
  }
  gasnet_hsl_unlock(&amdbarrier_lock);
}

/* For a rmb() between unlocked reads of _recv_value_present and _recv_value
 * Equivalent to ``(gasneti_sync_reads(), ambarrier_recv_value[phase])'',
 * except w/o assuming gasneti_sync_reads() to be valid in expression context.
 */
GASNETI_INLINE(amdbarrier_recv_value_synced)
int amdbarrier_recv_value_synced(int phase) {
  gasneti_sync_reads();
  return amdbarrier_recv_value[phase];
}

void gasnete_amdbarrier_kick() {
  int phase = amdbarrier_phase;
  int step = amdbarrier_step;
  int numsteps = 0;
  gasnet_handlerarg_t flags, value;

  if (step == amdbarrier_size || !amdbarrier_step_done[phase][step]) 
    return; /* nothing to do */

  gasneti_assert(gasneti_nodes > 1);

  gasnet_hsl_lock(&amdbarrier_lock);
    phase = amdbarrier_phase;
    step = amdbarrier_step;
    /* count steps we can take while holding the lock - must release before send,
       so coalesce as many as possible in one acquisition */
    while (step+numsteps < amdbarrier_size && amdbarrier_step_done[phase][step+numsteps]) {
      numsteps++;
    }

    if (numsteps) { /* completed one or more steps */
      gasneti_sync_reads(); /* between unlocked reads of _step_done and _mismatch */
      if_pf (amdbarrier_mismatch[phase] ||
	     ((amdbarrier_flags == 0) && 
	      amdbarrier_recv_value_present[phase] &&
	      (amdbarrier_recv_value_synced(phase) != amdbarrier_value))) {
        amdbarrier_flags = GASNET_BARRIERFLAG_MISMATCH;
        amdbarrier_mismatch[phase] = 1;
      }
      if (step+numsteps == amdbarrier_size) { /* We got the last recv - barrier locally complete */
        GASNETI_PROGRESSFNS_DISABLE(gasneti_pf_barrier,BOOLEAN);
        gasneti_sync_writes(); /* flush state before the write to ambarrier_step below */
      } 
      if (step + 1 < amdbarrier_size) {
        /* we will send at least one message - so calculate args */
        if ((amdbarrier_flags & GASNET_BARRIERFLAG_ANONYMOUS) &&
	    amdbarrier_recv_value_present[phase]) {
	  /* If we are on an node with an anonymous barrier invocation we
	   * may have received a barrier name from another node.  If so we
	   * must forward it to allow for matching tests.
	   */
	  gasneti_sync_reads(); /* Between unlocked reads of _recv_value_present and _recv_value */
	  flags = 0;
	  value = amdbarrier_recv_value[phase];
        } else {
	  value = amdbarrier_value;
	  flags = amdbarrier_flags;
        }
      }
      /* notify all threads of the step increase - 
         this may allow other local threads to proceed on the barrier and even indicate
         barrier completion while we overlap outgoing notifications to other nodes
      */
      amdbarrier_step = step+numsteps;
    } 
  gasnet_hsl_unlock(&amdbarrier_lock);

  for ( ; numsteps; numsteps--) {
    gasnet_node_t peer;

    step++;
    if (step == amdbarrier_size) { /* no send upon reaching last step */
      gasneti_assert(numsteps == 1);
      break;
    }

    /* No need for a full mod because worst case is < 2*gasneti_nodes.
     * However, we must take care for overflow if we try to do the
     * arithmetic in gasnet_node_t.  An example is gasnet_node_t
     * of uint8_t and gasneti_nodes=250 nodes.  The largest value of
     * gasnet_mynode is 249 and the largest value of 2^step is 128.
     * We can't compute (249 + 128) mod 250 in 8-bit arithmetic.
     * If we are using GASNET_MAXNODES <= INT_MAX then we can
     * fit the arithmetic into unsigned integers (32-bit example is
     * 0x7ffffffe + 0x40000000 = 0xbffffffe).  Otherwise we are
     * confident that 64-bit integers are ALWAYS large enough.
     */
    {
      #if (GASNET_MAXNODES <= INT_MAX)
	unsigned int tmp;
      #else
	uint64_t tmp;
      #endif
      tmp = (1 << step) + gasneti_mynode;
      peer = (tmp >= gasneti_nodes) ? (tmp - gasneti_nodes)
                                    : tmp;
      gasneti_assert(peer < gasneti_nodes);
    }

    GASNETI_SAFE(
      gasnet_AMRequestShort4(peer, gasneti_handleridx(gasnete_amdbarrier_notify_reqh), 
                             phase, step, value, flags));
  }
}

static void gasnete_amdbarrier_notify(int id, int flags) {
  int phase;
  gasneti_sync_reads(); /* ensure we read correct barrier_splitstate */
  if_pf(barrier_splitstate == INSIDE_BARRIER) 
    gasneti_fatalerror("gasnet_barrier_notify() called twice in a row");


  /* If we are on an ILP64 platform, this cast will ensure we truncate the same
   * bits locally as we do when passing over the network.
   */
  amdbarrier_value = (gasnet_handlerarg_t)id;

  phase = !amdbarrier_phase; /*  enter new phase */
  if_pf (flags & GASNET_BARRIERFLAG_MISMATCH) {
    amdbarrier_mismatch[phase] = 1;
    flags = GASNET_BARRIERFLAG_MISMATCH;
  }
  amdbarrier_flags = flags;
  amdbarrier_step = 0;
  gasneti_sync_writes(); 
  amdbarrier_phase = phase;

  if (gasneti_nodes > 1) {
    /*  send notify msg to peer */
    gasnet_node_t peer = ((gasneti_mynode + 1) < gasneti_nodes) ? (gasneti_mynode + 1) : 0;
    GASNETI_SAFE(
      gasnet_AMRequestShort4(peer, gasneti_handleridx(gasnete_amdbarrier_notify_reqh), 
                             phase, 0, id, flags));
    GASNETI_PROGRESSFNS_ENABLE(gasneti_pf_barrier,BOOLEAN);
  } else {
    amdbarrier_recv_value[phase] = id;	/* to simplify checking in _wait */
  }

  /*  update state */
  barrier_splitstate = INSIDE_BARRIER;
  gasneti_sync_writes(); /* ensure all state changes committed before return */
}


extern int gasnete_amdbarrier_wait(int id, int flags) {
  int retval = GASNET_OK;
  int i;

  int phase;
  gasneti_sync_reads(); /* ensure we read correct barrier_splitstate */
  phase = amdbarrier_phase;
  if_pf(barrier_splitstate == OUTSIDE_BARRIER) 
    gasneti_fatalerror("gasnet_barrier_wait() called without a matching notify");

  if (amdbarrier_step == amdbarrier_size) { /* completed asynchronously before wait (via progressfns or try) */
    GASNETI_TRACE_EVENT_TIME(B,BARRIER_ASYNC_COMPLETION,GASNETI_TICKS_NOW_IFENABLED(B)-gasnete_barrier_notifytime);
  } else { /*  wait for response */
    GASNET_BLOCKUNTIL((gasnete_amdbarrier_kick(), amdbarrier_step == amdbarrier_size));
  }

  /* determine return value */
  if_pf((!(flags & GASNET_BARRIERFLAG_ANONYMOUS) && (gasnet_handlerarg_t)id != amdbarrier_value) || 
        flags != amdbarrier_flags ||
	amdbarrier_mismatch[phase]) {
        amdbarrier_mismatch[phase] = 0;
	retval = GASNET_ERR_BARRIER_MISMATCH;
  }

  /*  update state */
  barrier_splitstate = OUTSIDE_BARRIER;
  for (i=0; i < amdbarrier_size; ++i) {
    amdbarrier_step_done[phase][i] = 0;
  }
  amdbarrier_recv_value_present[phase] = 0;
  gasneti_sync_writes(); /* ensure all state changes committed before return */

  return retval;
}

extern int gasnete_amdbarrier_try(int id, int flags) {
  gasneti_sync_reads(); /* ensure we read correct barrier_splitstate */
  if_pf(barrier_splitstate == OUTSIDE_BARRIER) 
    gasneti_fatalerror("gasnet_barrier_try() called without a matching notify");

  GASNETI_SAFE(gasneti_AMPoll());
  gasnete_amdbarrier_kick();

  if (amdbarrier_step == amdbarrier_size) return gasnete_amdbarrier_wait(id, flags);
  else return GASNET_ERR_NOT_READY;
}

#define GASNETE_AMDBARRIER_HANDLERS()                                 \
  gasneti_handler_tableentry_no_bits(gasnete_amdbarrier_notify_reqh)

/* ------------------------------------------------------------------------------------ */
/* AM-based centralized implementation of barrier */

/*  a silly, centralized barrier implementation:
     everybody sends notifies to a single node, where we count them up
     central node eventually notices the barrier is complete (probably
     when it calls wait) and then it broadcasts the completion to all the nodes
    The main problem is the need for the master to call wait before the barrier can
     make progress - we really need a way for the "last thread" to notify all 
     the threads when completion is detected, but AM semantics don't provide a 
     simple way to do this.
    The centralized nature also makes it non-scalable - we really want to use 
     a tree-based barrier or pairwise exchange algorithm for scalability
     (but these impose even greater potential delays due to the lack of attentiveness to
     barrier progress)
 */

static int volatile amcbarrier_value; /*  local ambarrier value */
static int volatile amcbarrier_flags; /*  local ambarrier flags */
static int volatile amcbarrier_phase = 0;  /*  2-phase operation to improve pipelining */
static int volatile amcbarrier_response_done[2] = { 0, 0 }; /*  non-zero when ambarrier is complete */
static int volatile amcbarrier_response_mismatch[2] = { 0, 0 }; /*  non-zero if we detected a mismatch */

/*  global state on master */
#ifndef GASNETE_AMCBARRIER_MASTER
  #define GASNETE_AMCBARRIER_MASTER (gasneti_nodes-1)
#endif
static gasnet_hsl_t amcbarrier_lock = GASNET_HSL_INITIALIZER;
static int volatile amcbarrier_consensus_value[2]; /*  consensus ambarrier value */
static int volatile amcbarrier_consensus_value_present[2] = { 0, 0 }; /*  consensus ambarrier value found */
static int volatile amcbarrier_consensus_mismatch[2] = { 0, 0 }; /*  non-zero if we detected a mismatch */
static int volatile amcbarrier_count[2] = { 0, 0 }; /*  count of how many remotes have notified (on P0) */

void gasnete_amcbarrier_init(void) {
  /* Nothing to do */
}

static void gasnete_amcbarrier_notify_reqh(gasnet_token_t token, 
  gasnet_handlerarg_t phase, gasnet_handlerarg_t value, gasnet_handlerarg_t flags) {
  gasneti_assert(gasneti_mynode == GASNETE_AMCBARRIER_MASTER);

  gasnet_hsl_lock(&amcbarrier_lock);
  { int count = amcbarrier_count[phase];
    if (!(flags & (GASNET_BARRIERFLAG_ANONYMOUS|GASNET_BARRIERFLAG_MISMATCH)) && 
        !amcbarrier_consensus_value_present[phase]) {
      amcbarrier_consensus_value[phase] = (int)value;
      amcbarrier_consensus_value_present[phase] = 1;
    } else if ((flags & GASNET_BARRIERFLAG_MISMATCH) ||
               (!(flags & GASNET_BARRIERFLAG_ANONYMOUS) && 
                amcbarrier_consensus_value[phase] != (int)value)) {
      amcbarrier_consensus_mismatch[phase] = 1;
    }
    count++;
    if (count == gasneti_nodes) gasneti_sync_writes(); /* about to signal, ensure we flush state */
    amcbarrier_count[phase] = count;
  }
  gasnet_hsl_unlock(&amcbarrier_lock);
}

static void gasnete_amcbarrier_done_reqh(gasnet_token_t token, 
  gasnet_handlerarg_t phase,  gasnet_handlerarg_t mismatch) {
  gasneti_assert(phase == amcbarrier_phase);

  amcbarrier_response_mismatch[phase] = mismatch;
  gasneti_sync_writes();
  amcbarrier_response_done[phase] = 1;
}

/*  make some progress on the ambarrier */
void gasnete_amcbarrier_kick() {
  int phase = amcbarrier_phase;

  if (gasneti_mynode != GASNETE_AMCBARRIER_MASTER) return;

  /*  master does all the work */
  if (amcbarrier_count[phase] == gasneti_nodes) {
    int gotit = 0;
    gasnet_hsl_lock(&amcbarrier_lock);
      if (amcbarrier_count[phase] == gasneti_nodes) {
        amcbarrier_count[phase] = 0;
        gotit = 1;
      }
    gasnet_hsl_unlock(&amcbarrier_lock);

    if (gotit) { /*  ambarrier is complete */
      int i;
      int mismatch = amcbarrier_consensus_mismatch[phase];

      GASNETI_PROGRESSFNS_DISABLE(gasneti_pf_barrier,BOOLEAN);

      /*  inform the nodes */
      for (i=0; i < gasneti_nodes; i++) {
        GASNETI_SAFE(
          gasnet_AMRequestShort2(i, gasneti_handleridx(gasnete_amcbarrier_done_reqh), 
                               phase, mismatch));
      }

      /*  reset state */
      amcbarrier_consensus_mismatch[phase] = 0;
      amcbarrier_consensus_value_present[phase] = 0;
    }
  }
}

extern void gasnete_amcbarrier_notify(int id, int flags) {
  int phase;
  gasneti_sync_reads(); /* ensure we read correct barrier_splitstate */
  if_pf(barrier_splitstate == INSIDE_BARRIER) 
    gasneti_fatalerror("gasnet_barrier_notify() called twice in a row");

  /* If we are on an ILP64 platform, this cast will ensure we truncate the same
   * bits locally as we do when passing over the network.
   */
  amcbarrier_value = (gasnet_handlerarg_t)id;

  amcbarrier_flags = flags;
  phase = !amcbarrier_phase; /*  enter new phase */
  amcbarrier_phase = phase;

  if (gasneti_nodes > 1) {
    /*  send notify msg to master */
    GASNETI_SAFE(
      gasnet_AMRequestShort3(GASNETE_AMCBARRIER_MASTER, gasneti_handleridx(gasnete_amcbarrier_notify_reqh), 
                           phase, amcbarrier_value, flags));
    if (gasneti_mynode == GASNETE_AMCBARRIER_MASTER) GASNETI_PROGRESSFNS_ENABLE(gasneti_pf_barrier,BOOLEAN);
  } else {
    amcbarrier_response_mismatch[phase] = (flags & GASNET_BARRIERFLAG_MISMATCH);
    amcbarrier_response_done[phase] = 1;
  }

  /*  update state */
  barrier_splitstate = INSIDE_BARRIER;
  gasneti_sync_writes(); /* ensure all state changes committed before return */
}


extern int gasnete_amcbarrier_wait(int id, int flags) {
  int phase;
  gasneti_sync_reads(); /* ensure we read correct barrier_splitstate */
  phase = amcbarrier_phase;
  if_pf(barrier_splitstate == OUTSIDE_BARRIER) 
    gasneti_fatalerror("gasnet_barrier_wait() called without a matching notify");


  if (amcbarrier_response_done[phase]) { /* completed asynchronously before wait (via progressfns or try) */
    GASNETI_TRACE_EVENT_TIME(B,BARRIER_ASYNC_COMPLETION,GASNETI_TICKS_NOW_IFENABLED(B)-gasnete_barrier_notifytime);
  } else { /*  wait for response */
    GASNET_BLOCKUNTIL((gasnete_amcbarrier_kick(), amcbarrier_response_done[phase]));
  }

  /*  update state */
  barrier_splitstate = OUTSIDE_BARRIER;
  amcbarrier_response_done[phase] = 0;
  gasneti_sync_writes(); /* ensure all state changes committed before return */
  if_pf((!(flags & GASNET_BARRIERFLAG_ANONYMOUS) && (gasnet_handlerarg_t)id != amcbarrier_value) || 
        flags != amcbarrier_flags || 
        amcbarrier_response_mismatch[phase]) {
        amcbarrier_response_mismatch[phase] = 0;
        return GASNET_ERR_BARRIER_MISMATCH;
  }
  else return GASNET_OK;
}

extern int gasnete_amcbarrier_try(int id, int flags) {
  gasneti_sync_reads(); /* ensure we read correct barrier_splitstate */
  if_pf(barrier_splitstate == OUTSIDE_BARRIER) 
    gasneti_fatalerror("gasnet_barrier_try() called without a matching notify");

  GASNETI_SAFE(gasneti_AMPoll());
  gasnete_amcbarrier_kick();

  if (amcbarrier_response_done[amcbarrier_phase]) return gasnete_amcbarrier_wait(id, flags);
  else return GASNET_ERR_NOT_READY;
}

#define GASNETE_AMCBARRIER_HANDLERS()                                 \
  gasneti_handler_tableentry_no_bits(gasnete_amcbarrier_notify_reqh), \
  gasneti_handler_tableentry_no_bits(gasnete_amcbarrier_done_reqh)  

/* ------------------------------------------------------------------------------------ */
/* Initialization and barrier mechanism selection */

/* these three function pointers select the barrier implementation in use 
   they are set during gasnete_init and should never change subsequently
 */
void (*gasnete_barrier_notify)(int id, int flags) = NULL;
int (*gasnete_barrier_wait)(int id, int flags) = NULL;
int (*gasnete_barrier_try)(int id, int flags) = NULL;

/* optional pointer, for progress function if the barrier implementation wants it */
void (*gasnete_barrier_pf)() = NULL;

extern void gasnete_barrier_init() {
  #ifndef GASNETE_BARRIER_DEFAULT
  /* conduit plugin for default barrier mechanism */
  #define GASNETE_BARRIER_DEFAULT "AMDISSEM"
  #endif
  const char *selection = /* fetch user barrier selection */
    gasneti_getenv_withdefault("GASNET_BARRIER",GASNETE_BARRIER_DEFAULT);
  char tmp[255];
  char options[255];
  int i;
  for (i = 0; selection[i] && i < sizeof(tmp)-1; i++) {
    tmp[i] = toupper(selection[i]); /* normalize to uppercase */
  }
  tmp[i] = '\0';
  selection = tmp;
  options[0] = '\0';
  #define GASNETE_ISBARRIER(namestr) \
    ((options[0]?strcat(options, ", "),(void)0:(void)0),strcat(options, namestr), \
     !strcmp(selection, namestr))

  #ifndef GASNETE_BARRIER_INIT
  /* conduit plugin to select a barrier - 
     should use GASNETE_ISBARRIER("whatever") to check if enabled, and then set the
     barrier function pointers */
  #define GASNETE_BARRIER_INIT()
  #endif
  GASNETE_BARRIER_INIT();
  if (gasnete_barrier_notify) { /* conduit has identified a barrier mechanism */
    gasneti_assert(gasnete_barrier_wait && gasnete_barrier_try);
    return;
  }
  if (GASNETE_ISBARRIER("AMDISSEM")) {
    gasnete_barrier_notify = &gasnete_amdbarrier_notify;
    gasnete_barrier_wait =   &gasnete_amdbarrier_wait;
    gasnete_barrier_try =    &gasnete_amdbarrier_try;
    gasnete_barrier_pf =     &gasnete_amdbarrier_kick;
    gasnete_amdbarrier_init();
  } else if (GASNETE_ISBARRIER("AMCENTRAL")) {
    gasnete_barrier_notify = &gasnete_amcbarrier_notify;
    gasnete_barrier_wait =   &gasnete_amcbarrier_wait;
    gasnete_barrier_try =    &gasnete_amcbarrier_try;
    gasnete_barrier_pf =     &gasnete_amcbarrier_kick;
    gasnete_amcbarrier_init();
  } else {
    gasneti_fatalerror("GASNET_BARRIER=%s is not a recognized barrier mechanism. "
                       "Available mechanisms are: %s", selection, options);
  }
}
/* ------------------------------------------------------------------------------------ */
#define GASNETE_REFBARRIER_HANDLERS() \
        GASNETE_AMDBARRIER_HANDLERS(), \
        GASNETE_AMCBARRIER_HANDLERS()
