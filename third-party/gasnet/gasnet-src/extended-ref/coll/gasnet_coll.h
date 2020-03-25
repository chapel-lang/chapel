/*   $Source: bitbucket.org:berkeleylab/gasnet.git/extended-ref/coll/gasnet_coll.h $
 * Description: GASNet Extended API Collective declarations
 * Copyright 2004, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _GASNET_COLL_H
#define _GASNET_COLL_H

#if defined(_INCLUDED_GASNET_INTERNAL_H) && !defined(_IN_GASNET_INTERNAL_H)
  #error Internal GASNet code should not directly include gasnet_coll.h, just gasnet_internal.h
#endif

#include <gasnetex.h>

GASNETI_BEGIN_EXTERNC
GASNETI_BEGIN_NOWARN
#define GASNETI_COLL_FN_HEADER(FNNAME) extern
/*---------------------------------------------------------------------------------*/
/* Flag values: */

/* Sync flags - NO DEFAULT */
#define GASNET_COLL_IN_NOSYNC	(1<<0)
#define GASNET_COLL_IN_MYSYNC	(1<<1)
#define GASNET_COLL_IN_ALLSYNC	(1<<2)
#define GASNET_COLL_OUT_NOSYNC	(1<<3)
#define GASNET_COLL_OUT_MYSYNC	(1<<4)
#define GASNET_COLL_OUT_ALLSYNC	(1<<5)

#define GASNET_COLL_SYNC_FLAG_MASK (0x3F)

#define GASNET_COLL_SINGLE	%%%ERROR-GASNET_COLL_SINGLE-HAS-BEEN-REMOVED%%%
#define GASNET_COLL_LOCAL	(1<<7)

#define GASNET_COLL_FIXED_THREADS_PER_NODE (1<<9)

#define GASNET_COLL_DST_IN_SEGMENT	(1<<10)
#define GASNET_COLL_SRC_IN_SEGMENT	(1<<11)

#define GASNET_COLL_DISABLE_AUTOTUNE (1<<18)
#define GASNET_COLL_NO_IMAGES (1<<19)

/* (prefix-)reduction function flags */
#define GASNET_COLL_AMSAFE	(1<<0)
#define GASNET_COLL_NONCOMM	(1<<1)


#ifndef GASNETE_COLL_IMAGE_OVERRIDE
  /* gasnet_image_t must be large enough to index all threads that participate
   * in collectives.  A conduit may override this if a smaller type will suffice.
   * However, types larger than 32-bits won't pass as AM handler args.  So, for
   * a larger type, many default things will require overrides.
   */
  typedef uint32_t gasnet_image_t;
#endif

/*---------------------------------------------------------------------------------*/

#ifndef GASNETE_COLL_HANDLE_OVERRIDE
  /* Handle type for collective ops: */
  // TODO-EX: these legacy aliases should be removed as soon as we no
  // longer need to support clients using the GASNet-1 collective APIs.
  typedef gex_Event_t gasnet_coll_handle_t;
  #define GASNET_COLL_INVALID_HANDLE GEX_EVENT_INVALID
#endif

/* Functions, types, etc for computational collectives */

// Callback function for GEX Reduce/Scan:
typedef 
void (*gex_Coll_ReduceFn_t)(
            const void * _arg1,
            void *       _arg2_and_out,
            size_t       _count,
            const void * _cdata);

/* Handle type for collective teams: */
#ifndef GASNETE_COLL_TEAMS_OVERRIDE
struct gasnete_coll_team_t_;
typedef struct gasnete_coll_team_t_ *gasnete_coll_team_t;
typedef gasnete_coll_team_t gasnet_team_handle_t;
/*change this so even the TEAM_ALL has a default team allocated rather than NULL*/
#endif

#ifndef GASNET_TEAM_ALL
extern gasnet_team_handle_t gasnete_coll_team_all;
#define GASNET_TEAM_ALL gasnete_coll_team_all
#endif

/*---------------------------------------------------------------------------------*/


/* STUFF FOR COLLECTIVE AUTOTUNING*/
/*---------------------------------------------------------------------------------*
 * Prototypes for external interface to try different collective trees (only works for GASNet Team All)
 * Note that the preffered way for changing these values is in the environment rather than these functions themselves
 *---------------------------------------------------------------------------------*/

typedef enum {GASNET_COLL_BROADCAST_OP=0, 
  GASNET_COLL_SCATTER_OP, 
  GASNET_COLL_GATHER_OP, 
  GASNET_COLL_GATHER_ALL_OP,
  GASNET_COLL_EXCHANGE_OP,
  GASNET_COLL_NUM_COLL_OPTYPES
} gasnet_coll_optype_t;

typedef enum {GASNET_COLL_PIPE_SEG_SIZE, GASNET_COLL_DISSEM_RADIX, GASNET_COLL_TREE_TYPE,
              /*check to see if hte conduit has added any new tuning parameters to this list*/
#ifdef GASNETE_COLL_CONDUIT_TUNING_PARAMETERS
              GASNETE_COLL_CONDUIT_TUNING_PARAMETERS ,
#endif 
              GASNET_COLL_NUM_PARAM_TYPES} gasnet_coll_tuning_param_type_t ;



typedef void (*gasnet_coll_overlap_sample_work_t)(void *_arg);


void gasnete_coll_loadTuningState(char *_filename, gasnete_coll_team_t _team GASNETI_THREAD_FARG);
#define gasnet_coll_loadTuningState(FILENAME, TEAM) gasnete_coll_loadTuningState(FILENAME, TEAM GASNETI_THREAD_GET)


void gasnete_coll_dumpTuningState(char *_filename, gasnete_coll_team_t _team GASNETI_THREAD_FARG);
#define gasnet_coll_dumpTuningState(FILENAME, TEAM) gasnete_coll_dumpTuningState(FILENAME, TEAM GASNETI_THREAD_GET)

void gasnete_coll_dumpProfile(char *_filename, gasnete_coll_team_t _team GASNETI_THREAD_FARG);
#define gasnet_coll_dumpProfile(FILENAME, TEAM) gasnete_coll_dumpProfile(FILENAME, TEAM GASNETI_THREAD_GET)

extern int gasnet_coll_get_num_tree_classes(gasnet_team_handle_t _team, gasnet_coll_optype_t _optype);
extern void gasnet_coll_set_tree_kind(gasnet_team_handle_t _team, int _tree_type, int _fanout, gasnet_coll_optype_t _optype); 
extern void gasnet_coll_set_dissem_limit(gasnet_team_handle_t _team, size_t _dissemlimit, gasnet_coll_optype_t _optype); 

/*---------------------------------------------------------------------------------*/
/* Collectives tracing */
#if GASNETI_STATS_OR_TRACE
  /* In what follows, "????"")" protects us from evil trigraphs */
  #if 0
    /* XXX Not yet implemented */
    extern char * gasnete_coll_format_addrlist(const void *addrlist[], int flags);
  #else
    #define gasnete_coll_format_addrlist(list,flags) gasneti_extern_strdup("[LIST]")
  #endif 
  // TODO-EX: Remove this work-around for fact that collective's "team" is not a "tm"
  #define GASNETI_RADDRSTR_COLL(root,ptr) GASNETI_RADDRSTR((gex_TM_t)(uintptr_t)1,root,ptr)
  // Legacy Collective OPs
  #define GASNETI_TRACE_COLL_BROADCAST(name,team,dst,root,src,nbytes,flags) do {                           \
    GASNETI_TRACE_EVENT_VAL(W,name,nbytes);                                                                \
    if (GASNETI_TRACE_ENABLED(D)) {                                                                        \
      if (root == gasneti_mynode) {                                                                        \
        GASNETI_TRACE_PRINTF(D,(#name ": " GASNETI_LADDRFMT " <- " GASNETI_RADDRFMT                        \
				" (nbytes=%" PRIuSZ " team=%p flags=0x%x)\n",                              \
			        GASNETI_LADDRSTR(dst), GASNETI_RADDRSTR_COLL(root,src),                    \
      			        (size_t)nbytes, (void *)team, flags));                                     \
      } else {                                                                                             \
        GASNETI_TRACE_PRINTF(D,(#name ": " GASNETI_LADDRFMT " <- (%i,????"")"                              \
				" (nbytes=%" PRIuSZ " team=%p flags=0x%x)\n",                              \
			        GASNETI_LADDRSTR(dst), (int)(root),                                        \
      			        (size_t)nbytes, (void *)team, flags));                                     \
      }                                                                                                    \
    }                                                                                                      \
  } while (0)
  #define GASNETI_TRACE_COLL_SCATTER(name,team,dst,root,src,nbytes,flags) \
	GASNETI_TRACE_COLL_BROADCAST(name,team,dst,root,src,nbytes,flags)
  #define GASNETI_TRACE_COLL_GATHER(name,team,root,dst,src,nbytes,flags) do {                              \
    GASNETI_TRACE_EVENT_VAL(W,name,nbytes);                                                                \
    if (GASNETI_TRACE_ENABLED(D)) {                                                                        \
      if (root == gasneti_mynode) {                                                                        \
        GASNETI_TRACE_PRINTF(D,(#name ": " GASNETI_RADDRFMT " <- " GASNETI_LADDRFMT                        \
				" (nbytes=%" PRIuSZ " team=%p flags=0x%x)\n",                              \
			        GASNETI_RADDRSTR_COLL(root,dst), GASNETI_LADDRSTR(src),                    \
      			        (size_t)nbytes, (void *)team, flags));                                     \
      } else {                                                                                             \
        GASNETI_TRACE_PRINTF(D,(#name ": (%i,????"") <- " GASNETI_LADDRFMT                                 \
				" (nbytes=%" PRIuSZ " team=%p flags=0x%x)\n",                              \
			        (int)(root), GASNETI_LADDRSTR(src),                                        \
      			        (size_t)nbytes, (void *)team, flags));                                     \
      }                                                                                                    \
    }                                                                                                      \
  } while (0)
  #define GASNETI_TRACE_COLL_GATHER_ALL(name,team,dst,src,nbytes,flags) do {                               \
    GASNETI_TRACE_EVENT_VAL(W,name,nbytes);                                                                \
    if (GASNETI_TRACE_ENABLED(D)) {                                                                        \
      GASNETI_TRACE_PRINTF(D,(#name ": " GASNETI_LADDRFMT " <- " GASNETI_LADDRFMT                          \
			      " (nbytes=%" PRIuSZ " team=%p flags=0x%x)\n",                                \
			      GASNETI_LADDRSTR(dst), GASNETI_LADDRSTR(src),                                \
      			      (size_t)nbytes, (void *)team, flags));                                       \
    }                                                                                                      \
  } while (0)
  #define GASNETI_TRACE_COLL_EXCHANGE(name,team,dst,src,nbytes,flags) \
	GASNETI_TRACE_COLL_GATHER_ALL(name,team,dst,src,nbytes,flags)
  // GEX Collective Ops
  #define GASNETI_TRACE_TM_REDUCE(name,tm,root,dst,src,dt,dt_sz,dt_cnt,op,op_fnptr,op_cdata,flags) do { \
    GASNETI_TRACE_EVENT_VAL(W,name,dt_cnt);                                                             \
    if (GASNETI_TRACE_ENABLED(D)) {                                                                     \
      char *_tr_dtstr = (char *)gasneti_extern_malloc(gasneti_format_dt(NULL,(dt)));                    \
      gasneti_format_dt(_tr_dtstr,(dt));                                                                \
      char *_tr_opstr = (char *)gasneti_extern_malloc(gasneti_format_op(NULL,(op)));                    \
      gasneti_format_op(_tr_opstr,(op));                                                                \
      GASNETI_TRACE_PRINTF(D, (#name ": root = " GASNETI_TMRANKFMT ", dt = %" PRIuSZ "*%s, op = %s",    \
                               GASNETI_TMRANKSTR((tm),(root)),                                          \
                               (size_t)(dt_cnt), 7+_tr_dtstr, 7+_tr_opstr));                            \
      if ((root) == gex_TM_QueryRank(tm)) {                                                             \
        GASNETI_TRACE_PRINTF(D, (#name ": src = " GASNETI_LADDRFMT ", dst = " GASNETI_LADDRFMT,         \
                                 GASNETI_LADDRSTR(src), GASNETI_LADDRSTR(dst)));                        \
      } else {                                                                                          \
        GASNETI_TRACE_PRINTF(D, (#name ": src = " GASNETI_LADDRFMT, GASNETI_LADDRSTR(src)));            \
      }                                                                                                 \
      if (op == GEX_OP_USER || op == GEX_OP_USER_NC) {                                                  \
        const void * const * _tr_p_fnptr = (const void * const *)&(op_fnptr);                           \
        GASNETI_TRACE_PRINTF(D, (#name ": User-defined (fnptr, cdata) = (%p, %p)",                      \
                                 *_tr_p_fnptr, (op_cdata)));                                            \
      }                                                                                                 \
      gasneti_extern_free(_tr_dtstr);                                                                   \
      gasneti_extern_free(_tr_opstr);                                                                   \
    }                                                                                                   \
  } while (0)
  #define GASNETI_TRACE_TM_REDUCE_ALL(name,tm,dst,src,dt,dt_sz,dt_cnt,op,op_fnptr,op_cdata,flags) do {  \
    GASNETI_TRACE_EVENT_VAL(W,name,dt_cnt);                                                             \
    if (GASNETI_TRACE_ENABLED(D)) {                                                                     \
      char *_tr_dtstr = (char *)gasneti_extern_malloc(gasneti_format_dt(NULL,(dt)));                    \
      gasneti_format_dt(_tr_dtstr,(dt));                                                                \
      char *_tr_opstr = (char *)gasneti_extern_malloc(gasneti_format_op(NULL,(op)));                    \
      gasneti_format_op(_tr_opstr,(op));                                                                \
      GASNETI_TRACE_PRINTF(D, (#name ": dt = %" PRIuSZ "*%s, op = %s",                                  \
                               (size_t)(dt_cnt), 7+_tr_dtstr, 7+_tr_opstr));                            \
      GASNETI_TRACE_PRINTF(D, (#name ": src = " GASNETI_LADDRFMT ", dst = " GASNETI_LADDRFMT,           \
                               GASNETI_LADDRSTR(src), GASNETI_LADDRSTR(dst)));                          \
      if (op == GEX_OP_USER || op == GEX_OP_USER_NC) {                                                  \
        const void * const * _tr_p_fnptr = (const void * const *)&(op_fnptr);                           \
        GASNETI_TRACE_PRINTF(D, (#name ": User-defined (fnptr, cdata) = (%p, %p)",                      \
                                 *_tr_p_fnptr, (op_cdata)));                                            \
      }                                                                                                 \
      gasneti_extern_free(_tr_dtstr);                                                                   \
      gasneti_extern_free(_tr_opstr);                                                                   \
    }                                                                                                   \
  } while (0)
#else
  #define GASNETI_TRACE_COLL_BROADCAST(name,team,dst,root,src,nbytes,flags)
  #define GASNETI_TRACE_COLL_SCATTER(name,team,dst,root,src,nbytes,flags)
  #define GASNETI_TRACE_COLL_GATHER(name,team,root,dst,src,nbytes,flags)
  #define GASNETI_TRACE_COLL_GATHER_ALL(name,team,dst,src,nbytes,flags)
  #define GASNETI_TRACE_COLL_EXCHANGE(name,team,dst,src,nbytes,flags)
  #define GASNETI_TRACE_TM_REDUCE(name,tm,root,dst,src,dt,dt_sz,dt_cnt,op,op_fnptr,op_cdata,flags)
  #define GASNETI_TRACE_TM_REDUCE_ALL(name,tm,dst,src,dt,dt_sz,dt_cnt,op,op_fnptr,op_cdata,flags)
#endif

/*---------------------------------------------------------------------------------*/
//
// Legacy GASNet-1 APIs for try and wait
// These are simply redirects
//

#define gasnet_coll_try_sync(event)                     gex_Event_Test(event)
#define gasnet_coll_try_sync_some(pevent,numevents)     gex_Event_TestSome(pevent,numevents,0)
#define gasnet_coll_try_sync_all(pevent,numevents)      gex_Event_TestAll(pevent,numevents,0)

#define gasnet_coll_wait_sync(event)                    gex_Event_Wait(event)
#define gasnet_coll_wait_sync_some(pevent,numevents)    gex_Event_WaitSome(pevent,numevents,0)
#define gasnet_coll_wait_sync_all(pevent,numevents)     gex_Event_WaitAll(pevent,numevents,0)

/*---------------------------------------------------------------------------------*/
/* Team barrier functions */

extern void gasnete_coll_barrier_notify(gasnete_coll_team_t _team, int _id, int _flags GASNETI_THREAD_FARG);
extern int gasnete_coll_barrier_try(gasnete_coll_team_t _team, int _id, int _flags GASNETI_THREAD_FARG);
extern int gasnete_coll_barrier_wait(gasnete_coll_team_t _team, int _id, int _flags GASNETI_THREAD_FARG);
extern int gasnete_coll_barrier(gasnete_coll_team_t _team, int _id, int _flags GASNETI_THREAD_FARG);
extern int gasnete_coll_barrier_result(gasnete_coll_team_t _team, int *_id GASNETI_THREAD_FARG);


#define gasnet_coll_barrier_notify(team, id, flags) gasnete_coll_barrier_notify(team, id, flags GASNETI_THREAD_GET)
#define gasnet_coll_barrier_try(team, id, flags) gasnete_coll_barrier_try(team, id, flags GASNETI_THREAD_GET)
#define gasnet_coll_barrier_wait(team, id, flags) gasnete_coll_barrier_wait(team, id, flags GASNETI_THREAD_GET)
#define gasnet_coll_barrier(team, id, flags) gasnete_coll_barrier(team, id, flags GASNETI_THREAD_GET)
#define gasnet_coll_barrier_result(team, id) gasnete_coll_barrier_result(team, id GASNETI_THREAD_GET)

/*---------------------------------------------------------------------------------*/
/* no point trying to inline the collectives since they will involve non trivial communication costs*/
GASNETI_COLL_FN_HEADER(_gasnet_coll_broadcast_nb) 
gex_Event_t _gasnet_coll_broadcast_nb(gasnet_team_handle_t _team,
                          void *_dst,
                          gasnet_image_t _srcimage, void *_src,
                          size_t _nbytes, int _flags GASNETI_THREAD_FARG);

#define gasnet_coll_broadcast_nb(team,dst,srcimage,src,nbytes,flags) \
       _gasnet_coll_broadcast_nb(team,dst,srcimage,src,nbytes,flags GASNETI_THREAD_GET)

GASNETI_COLL_FN_HEADER(_gasnet_coll_broadcast) 
void _gasnet_coll_broadcast(gasnet_team_handle_t _team,
                                   void *_dst,
                                   gasnet_image_t _srcimage, void *_src,
                                   size_t _nbytes, int _flags GASNETI_THREAD_FARG);
#define gasnet_coll_broadcast(team,dst,srcimage,src,nbytes,flags) \
       _gasnet_coll_broadcast(team,dst,srcimage,src,nbytes,flags GASNETI_THREAD_GET)

/*---------------------------------------------------------------------------------*/
GASNETI_COLL_FN_HEADER(_gasnet_coll_scatter_nb) 
gex_Event_t _gasnet_coll_scatter_nb(gasnet_team_handle_t _team,
                        void *_dst,
                        gasnet_image_t _srcimage, void *_src,
                        size_t _nbytes, int _flags GASNETI_THREAD_FARG);

#define gasnet_coll_scatter_nb(team,dst,srcimage,src,nbytes,flags) \
       _gasnet_coll_scatter_nb(team,dst,srcimage,src,nbytes,flags GASNETI_THREAD_GET)

GASNETI_COLL_FN_HEADER(_gasnet_coll_scatter) 
void _gasnet_coll_scatter(gasnet_team_handle_t _team,
                          void *_dst, gasnet_image_t _srcimage, void *_src,
                                 size_t _nbytes, int _flags GASNETI_THREAD_FARG); 
#define gasnet_coll_scatter(team,dst,srcimage,src,nbytes,flags) \
       _gasnet_coll_scatter(team,dst,srcimage,src,nbytes,flags GASNETI_THREAD_GET)

/*---------------------------------------------------------------------------------*/
GASNETI_COLL_FN_HEADER(_gasnet_coll_gather_nb) 
gex_Event_t _gasnet_coll_gather_nb(gasnet_team_handle_t _team,
                       gasnet_image_t _dstimage, void *_dst,
                       void *_src,
                       size_t _nbytes, int _flags GASNETI_THREAD_FARG);
#define gasnet_coll_gather_nb(team,dstimage,dst,src,nbytes,flags) \
       _gasnet_coll_gather_nb(team,dstimage,dst,src,nbytes,flags GASNETI_THREAD_GET)

GASNETI_COLL_FN_HEADER(_gasnet_coll_gather) 
void _gasnet_coll_gather(gasnet_team_handle_t _team,
                                gasnet_image_t _dstimage, void *_dst,
                                void *_src,
                                size_t _nbytes, int _flags GASNETI_THREAD_FARG);
#define gasnet_coll_gather(team,dstimage,dst,src,nbytes,flags) \
       _gasnet_coll_gather(team,dstimage,dst,src,nbytes,flags GASNETI_THREAD_GET)

/*---------------------------------------------------------------------------------*/
GASNETI_COLL_FN_HEADER(_gasnet_coll_gather_all_nb) 
gex_Event_t _gasnet_coll_gather_all_nb(gasnet_team_handle_t _team,
                           void *_dst, void *_src,
                           size_t _nbytes, int _flags GASNETI_THREAD_FARG) ;
#define gasnet_coll_gather_all_nb(team,dst,src,nbytes,flags) \
       _gasnet_coll_gather_all_nb(team,dst,src,nbytes,flags GASNETI_THREAD_GET)

GASNETI_COLL_FN_HEADER(_gasnet_coll_gather_all) 
void _gasnet_coll_gather_all(gasnet_team_handle_t _team,
                                    void *_dst, void *_src,
                                    size_t _nbytes, int _flags GASNETI_THREAD_FARG);
#define gasnet_coll_gather_all(team,dst,src,nbytes,flags) \
       _gasnet_coll_gather_all(team,dst,src,nbytes,flags GASNETI_THREAD_GET)

/*---------------------------------------------------------------------------------*/
GASNETI_COLL_FN_HEADER(_gasnet_coll_exchange_nb) 
gex_Event_t _gasnet_coll_exchange_nb(gasnet_team_handle_t _team,
                         void *_dst, void *_src,
                         size_t _nbytes, int _flags GASNETI_THREAD_FARG) ;
#define gasnet_coll_exchange_nb(team,dst,src,nbytes,flags) \
       _gasnet_coll_exchange_nb(team,dst,src,nbytes,flags GASNETI_THREAD_GET)

GASNETI_COLL_FN_HEADER(_gasnet_coll_exchange) 
void _gasnet_coll_exchange(gasnet_team_handle_t _team,
                                  void *_dst, void *_src,
                                  size_t _nbytes, int _flags GASNETI_THREAD_FARG);
#define gasnet_coll_exchange(team,dst,src,nbytes,flags) \
       _gasnet_coll_exchange(team,dst,src,nbytes,flags GASNETI_THREAD_GET)

/*---------------------------------------------------------------------------------*/

#ifndef gasnete_tm_broadcast_nb
  extern gex_Event_t
  gasnete_tm_broadcast_nb(gex_TM_t _tm, gex_Rank_t _root,
                          void *_dst, const void *_src,
                          size_t _nbytes, gex_Flags_t _flags, uint32_t _sequence
                          GASNETI_THREAD_FARG) GASNETI_WARN_UNUSED_RESULT;
#endif
#define gex_Coll_BroadcastNB(tm,root,dst,src,nbytes,flags) \
        gasnete_tm_broadcast_nb(tm,root,dst,src,nbytes,flags,0 GASNETI_THREAD_GET)

/*---------------------------------------------------------------------------------*/

#ifndef gasnete_tm_reduce_nb
  extern gex_Event_t
  gasnete_tm_reduce_nb(gex_TM_t _tm, gex_Rank_t _root, void *_dst, const void *_src,
                       gex_DT_t _dt, size_t _dt_sz, size_t _dt_cnt,
                       gex_OP_t _op, gex_Coll_ReduceFn_t _user_op, void * _user_cdata,
                       gex_Flags_t _flags, uint32_t _sequence GASNETI_THREAD_FARG) GASNETI_WARN_UNUSED_RESULT;
#endif
#define gex_Coll_ReduceToOneNB(tm,root,dst,src,dt,dts,dtc,op,fn,cdata,flags) \
        gasnete_tm_reduce_nb(tm,root,dst,src,dt,dts,dtc,op,fn,cdata,flags,0 GASNETI_THREAD_GET)

/*---------------------------------------------------------------------------------*/

#ifndef gasnete_tm_reduce_all_nb
  extern gex_Event_t
  gasnete_tm_reduce_all_nb(
                        gex_TM_t _tm, void *_dst, const void *_src,
                        gex_DT_t _dt, size_t _dt_sz, size_t _dt_cnt,
                        gex_OP_t _op, gex_Coll_ReduceFn_t _user_op, void * _user_cdata,
                        gex_Flags_t _flags, uint32_t _sequence GASNETI_THREAD_FARG) GASNETI_WARN_UNUSED_RESULT;
#endif
#define gex_Coll_ReduceToAllNB(tm,dst,src,dt,dts,dtc,op,fn,cdata,flags) \
        gasnete_tm_reduce_all_nb(tm,dst,src,dt,dts,dtc,op,fn,cdata,flags,0 GASNETI_THREAD_GET)

/*---------------------------------------------------------------------------------*/

#undef GASNETI_COLL_FN_HEADER

GASNETI_END_NOWARN
GASNETI_END_EXTERNC

#endif
