/*   $Source: bitbucket.org:berkeleylab/gasnet.git/gasnet_team.c $
 * Description: GASNet implementation of teams
 * Copyright 2018, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#include <gasnet_coll_internal.h>

// TODO-EX:
//
// Currently we implement a "team" (the abstract distributed collection, of
// which a "tm" is a representative) in terms of the legacy GASNet-1 collective
// team.  We should eventually using something smarter, including scalable
// storage.

// Given (tm,rank) return the jobrank
gex_Rank_t gasneti_tm_fwd_lookup(gasneti_TM_t tm, gex_Rank_t rank) {
  gasnete_coll_team_t team = tm->_coll_team;
  gasneti_assert(team != GASNET_TEAM_ALL); // TM0 should not reach here
  return team->rel2act_map[rank];
}

// Given (tm,jobrank) return the rank or GEX_RANK_INVALID
// TODO-EX: THIS IS A HORRIBLE O(size(tm)) SCAN!
gex_Rank_t gasneti_tm_rev_lookup(gasneti_TM_t tm, gex_Rank_t jobrank) {
  gasnete_coll_team_t team = tm->_coll_team;
  gasneti_assert(team != GASNET_TEAM_ALL); // TM0 should not reach here
  gex_Rank_t size = tm->_size;
  gasneti_assert_uint(size ,==, team->total_ranks);
  for (gex_Rank_t rank = 0; rank < size; ++rank) {
    if (team->rel2act_map[rank] == jobrank) return rank;
  }
  return GEX_RANK_INVALID;
}

static size_t split_scratch_size(gex_TM_t *new_tm_p, gasneti_TM_t i_parent,
                                 int color, int key, gex_Flags_t flags)
{
  if (!new_tm_p) {
    return 0;
  }

  static size_t minimum, recommended;
  static int is_init = 0;
  if_pf (!is_init) {
     static gasneti_mutex_t lock = GASNETI_MUTEX_INITIALIZER;
     gasneti_mutex_lock(&lock);
     if (!is_init) {
       minimum = gasneti_getenv_int_withdefault("GASNET_COLL_MIN_SCRATCH_SIZE",
                                                GASNETE_COLL_MIN_SCRATCH_SIZE_DEFAULT,1);
       recommended = gasneti_getenv_int_withdefault("GASNET_COLL_SCRATCH_SIZE",
                                                    GASNETE_COLL_SCRATCH_SIZE_DEFAULT,1);
       gasneti_sync_writes();
       is_init = 1;
     }
     gasneti_mutex_unlock(&lock);
  } else {
     gasneti_sync_reads();
  }

  if (flags & GEX_FLAG_TM_SCRATCH_SIZE_MIN) {
    // The current true minimum is one byte for every member in the new team.
    // However, performing a collective to size the teams seems unnecessary.
    // So, we take the size of the parent, and round up to cache line size.
    // TODO-EX: is this really the value we want to advertise?
    return MAX(minimum, GASNETI_ALIGNUP(i_parent->_size, GASNETI_CACHE_LINE_BYTES));
  }

  if (flags & GEX_FLAG_TM_SCRATCH_SIZE_RECOMMENDED) {
    return MAX(minimum, recommended);
  }

  gasneti_fatalerror("Invalid team scratch size query");
  return 0;
}

size_t gasneti_TM_Split(gex_TM_t *new_tm_p, gex_TM_t e_parent, int color, int key,
                        void *addr, size_t len, gex_Flags_t flags
                        GASNETI_THREAD_FARG)
{
  gasneti_TM_t i_parent = gasneti_import_tm(e_parent);
  gasneti_EP_t ep = i_parent->_ep;

#if GASNET_DEBUG
  if ((flags & GEX_FLAG_TM_SCRATCH_SIZE_MIN) &&
      (flags & GEX_FLAG_TM_SCRATCH_SIZE_RECOMMENDED)) {
    gasneti_fatalerror("Call to gex_TM_Split() with mutually-exclusive "
                       "GEX_FLAG_TM_SCRATCH_SIZE_MIN and "
                       "GEX_FLAG_TM_SCRATCH_SIZE_RECOMMENDED both set in flags argument");
  }
#endif
  if (flags & (GEX_FLAG_TM_SCRATCH_SIZE_MIN | GEX_FLAG_TM_SCRATCH_SIZE_RECOMMENDED)) {
    return split_scratch_size(new_tm_p, i_parent, color, key, flags);
  }

  if (!new_tm_p) {
    color = -1; // tell gasnete_coll_team_split() not to create a team for this caller
  } else {
    gasneti_assert_int(color ,>=, 0);
#if !GASNET_SEGMENT_EVERYTHING
    gasneti_assert(ep->_segment);
    gasneti_assert_ptr(addr     ,>=, ep->_segment->_addr);
    gasneti_assert_ptr((uint8_t*)addr+len ,<=, ep->_segment->_ub);
#endif
    gasneti_assert_uint(len ,>=, split_scratch_size(new_tm_p, i_parent, color, key,
                                             flags | GEX_FLAG_TM_SCRATCH_SIZE_MIN));
  }

  gasnet_seginfo_t scratch;
  scratch.addr = addr;
  scratch.size = len;
  gasnete_coll_team_t team =
      gasnete_coll_team_split(i_parent->_coll_team, color, key,
                              &scratch GASNETI_THREAD_PASS);

  if (team == NULL) {
    gasneti_assert(!new_tm_p);
    GASNETI_TRACE_PRINTF(W,("Split: parent="GASNETI_TMSELFFMT" [No team created]",
                            GASNETI_TMSELFSTR(e_parent)));
    return 0;
  }

  // TODO-EX: use of a conduit-specific hook is needed here
  gasneti_TM_t i_tm = gasneti_alloc_tm(ep, team->myrank, team->total_ranks, flags, 0);
  i_tm->_coll_team = team;
  gex_TM_t e_tm = gasneti_export_tm(i_tm);
  team->e_tm = e_tm;
  *new_tm_p = e_tm;

  GASNETI_TRACE_PRINTF(W,("Split: parent="GASNETI_TMSELFFMT" color=%d key=%d result="GASNETI_TMSELFFMT,
                          GASNETI_TMSELFSTR(e_parent), color, key, GASNETI_TMSELFSTR(e_tm)));

  return 1; // return is documented as undefined
}

/* ------------------------------------------------------------------------------------ */
/* TM trace formatting - legal even without STATS/TRACE */

// Format a gex_TM_t as a GUID
extern const char *gasneti_formattm(gex_TM_t e_tm) {
  if ((uintptr_t)e_tm == 1)     return "N/A";  // GASNet-1 collectives team
  if (e_tm == NULL)             return "JOB";  // JobRank, as with token
  gasnete_coll_team_t team = gasneti_import_tm(e_tm)->_coll_team;
  if (team == NULL)             return "TM0";  // Team0 before end of Client_Init
  return gasneti_dynsprintf("TM%x", (unsigned int)team->team_id);
}
