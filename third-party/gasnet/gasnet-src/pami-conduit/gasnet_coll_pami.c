/*   $Source: bitbucket.org:berkeleylab/gasnet.git/pami-conduit/gasnet_coll_pami.c $
 * Description: GASNet extended collectives implementation on PAMI
 * Copyright 2012, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#include <gasnet_coll_pami.h>
#include <limits.h> /* For INT_MAX */

/* ------------------------------------------------------------------------------------ */
/* Bootstrap collectives and dependencies */

static int
gasnetc_find_alg(const char *name, pami_metadata_t *md, size_t count)
{
  size_t len = strlen(name); /* string is used for PREFIX match */
  int alg;

  for (alg = 0; alg < count; ++alg) {
    if (0 == strncmp(name, md[alg].name, len)) break;
  }

  return alg;
}

/* Get the default algorithm for a given (geometery, collective) pair.
   This will be the first "always works" algorithm unless user provides an override.
*/
extern void
gasnetc_dflt_coll_alg(pami_geometry_t geom, pami_xfer_type_t op, pami_algorithm_t *alg_p) {
  static int print_once[PAMI_XFER_COUNT]; /* static var must initially be all zeros */
  pami_result_t rc;
  size_t counts[2];
  pami_algorithm_t *algorithms;
  pami_metadata_t *metadata;
  const char *envvar, *envval, *dfltval;
  int alg, fullcount;

  gasneti_assert(op >= 0);
  gasneti_assert(op < PAMI_XFER_COUNT);

  rc = PAMI_Geometry_algorithms_num(geom, op, counts);
  GASNETC_PAMI_CHECK(rc, "calling PAMI_Geometry_algorithms_num()");
  fullcount = counts[0] + counts[1];

  /* Space for algorithms and metadata */
  algorithms = alloca(fullcount * sizeof(pami_algorithm_t));
  metadata   = alloca(fullcount * sizeof(pami_metadata_t));

  rc = PAMI_Geometry_algorithms_query(geom, op,
                                      algorithms, metadata, counts[0],
                                      algorithms+counts[0], metadata+counts[0], counts[1]);
  GASNETC_PAMI_CHECK(rc, "calling PAMI_Geometry_algorithms_query()");

  /* Process environment and defaults: */
  switch(op) { /* please keep alphabetical */
  /* Used for blocking gasnet exchange: */
  case PAMI_XFER_ALLTOALL:
    envvar = "GASNET_PAMI_ALLTOALL_ALG";
  #if GASNETI_ARCH_BGQ
    dfltval = "I0:M2MComposite:MU:MU"; /* Best on BG/Q by a large margin */
  #else
    dfltval = "I0:Ring:"; /* Uniformly 2nd place (out of 3) on PERCS */
  #endif
    break;

  /* Used for blocking gasnet exchange w/ multiple images: */
  case PAMI_XFER_ALLTOALLV_INT:
    envvar = "GASNET_PAMI_ALLTOALLV_INT_ALG";
  #if GASNETI_ARCH_BGQ
    dfltval = "I0:M2MComposite:MU:MU"; /* Best on BG/Q by a large margin */
  #else
    dfltval = "I0:M2MComposite:"; /* Best on PERCS for all but smallest len (where it is close) */
  #endif
    break;

  /* Used for blocking gasnet gatherall and gasnetc_bootstrapExchange(): */
  case PAMI_XFER_ALLGATHER:
    envvar = "GASNET_PAMI_ALLGATHER_ALG";
  #if GASNETI_ARCH_BGQ && 0 /* TODO: split choice based on size */
    dfltval = "I0:RectangleDput:"; /* Uniformly best (or very near) for LARGE case only... */
    /* .. but only available for "rectangular" jobs. */
    alg = gasnetc_find_alg(dfltval, metadata, counts[0]);
    if (alg < counts[0]) break; /* Otherwise fall through */
  #endif
    dfltval = "I0:Binomial:"; /* Uniformly 2nd place (out of 3) on PERCS, uniformly "OK" on BG/Q */
    break;

  /* Used for blocking gasnet gatherall w/ multiple images: */
  case PAMI_XFER_ALLGATHERV_INT:
    envvar = "GASNET_PAMI_ALLGATHERV_INT_ALG";
  #if GASNETI_ARCH_BGQ && 0 /* TODO: split choice based on size */
    dfltval = "I0:RectangleDput:"; /* Uniformly best for small to moderate cases only ... */
    /* .. but only available for "rectangular" jobs. */
    alg = gasnetc_find_alg(dfltval, metadata, counts[0]);
    if (alg < counts[0]) break; /* Otherwise fall through */
  #endif
    dfltval = NULL; /* Only one other option available on systems I've tested -PHH */
    break;

  /* Used for exitcode reduction and "PAMIALLREDUCE" barrier: */
  case PAMI_XFER_ALLREDUCE:
    envvar = "GASNET_PAMI_ALLREDUCE_ALG";
  #if GASNETI_ARCH_BGQ
    dfltval = "I0:Binomial:-:ShortMU"; /* great when available */
    alg = gasnetc_find_alg(dfltval, metadata, fullcount);
    if (alg < fullcount) {
      /* make sure PAMI-allreduce barrier and exitcode reduction will "fit" */
      pami_metadata_t *md = &metadata[alg];
      if ((!md->check_correct.values.rangeminmax  || md->range_lo <= sizeof(int)) &&
          (!md->check_correct.values.rangeminmax  || md->range_hi >= 2*sizeof(uint64_t)) &&
          (!md->check_correct.values.sendminalign || md->send_min_align >= sizeof(char)) &&
          (!md->check_correct.values.recvminalign || md->recv_min_align >= sizeof(char))) {
        break; /* Otherwise fall through */
      }
    }
    dfltval = "I1:ShortAllreduce:"; /* excellent second choice on BG/Q */
    alg = gasnetc_find_alg(dfltval, metadata, fullcount);
    if (alg < fullcount) {
      /* make sure PAMI-allreduce barrier and exitcode reduction will "fit" */
      pami_metadata_t *md = &metadata[alg];
      if ((!md->check_correct.values.rangeminmax  || md->range_lo <= sizeof(int)) &&
          (!md->check_correct.values.rangeminmax  || md->range_hi >= 2*sizeof(uint64_t)) &&
          (!md->check_correct.values.sendminalign || md->send_min_align >= sizeof(char)) &&
          (!md->check_correct.values.recvminalign || md->recv_min_align >= sizeof(char))) {
        break; /* Otherwise fall through */
      }
    }
  #endif
    dfltval = "I0:Binomial:"; /* uniformly "good" on BG/Q and PERCS */
    break;

  /* Used for gasnetc_fast_barrier() and GASNET_BARRIERFLAG_UNNAMED */
  case PAMI_XFER_BARRIER:
    envvar = "GASNET_PAMI_BARRIER_ALG";
  #if GASNETI_ARCH_BGQ
    /* Note: this could be any of
     *     "I0:MultiSync2Device:SHMEM:GI"
     *     "I0:MultiSync:SHMEM:-",
     *     "I0:MultiSync:-:GI",
     * depending on job layout, and may not be available on team != ALL.
     */
    dfltval = "I0:MultiSync";
    alg = gasnetc_find_alg(dfltval, metadata, counts[0]);
    if (alg < counts[0]) break; /* Otherwise fall through */
  #endif
    dfltval = NULL; /* TODO: tune a better default than alg[0]? */
    break;

  /* Used for blocking gasnet broadcast: */
  case PAMI_XFER_BROADCAST:
    envvar = "GASNET_PAMI_BROADCAST_ALG";
  #if GASNETI_ARCH_BGQ
    dfltval = "I0:2-nary:"; /* uniformly "near-best" on BG/Q */
  #else
   #if 0 /* Seen to deadlock when using multiple procs/node */
    dfltval = "I0:4-nary:"; /* uniformly "near-best" on PERSC */
   #else
    dfltval = NULL; /* TODO: tune for better default or wait for bug fix */
   #endif
  #endif
    break;

  /* Used for blocking gasnet gather: */
  case PAMI_XFER_GATHER:
    envvar = "GASNET_PAMI_GATHER_ALG";
    dfltval = NULL; /* TODO: tune for better default */
    break;

  /* Used for blocking gasnet gather w/ multiple images: */
  case PAMI_XFER_GATHERV_INT:
    envvar = "GASNET_PAMI_GATHERV_INT_ALG";
    dfltval = NULL; /* TODO: tune for better default */
    break;

  /* Used for blocking gasnet scatter: */
  case PAMI_XFER_SCATTER:
    envvar = "GASNET_PAMI_SCATTER_ALG";
    dfltval = "I0:Binomial:"; /* uniformly "good" on BG/Q and PERSC */
    break;

  /* Used for blocking gasnet scatter w/ multiple images: */
  case PAMI_XFER_SCATTERV_INT:
    envvar = "GASNET_PAMI_SCATTERV_INT_ALG";
    dfltval = NULL; /* TODO: tune for better default */
    break;

  default:
    gasneti_fatalerror("Unknown 'op' value %d in %s", (int)op, __FUNCTION__);
    envvar = dfltval = NULL; /* for warning suppression only */
  }
  /* Override the defaults above for the single-task case: */
  if (gasneti_nodes == 1) {
    const char *onetask = "I0:OneTask";
    if (gasnetc_find_alg(onetask, metadata, counts[0]) < counts[0]) {
      dfltval = onetask;
    }
  }
  /* Now the user's environment value if any: */
  envval = gasneti_getenv_withdefault(envvar, dfltval);
  alg = 0; /* failsafe */
  if (NULL != envval) {
    while (envval[0] && isspace(envval[0])) ++envval; /* leading whitespace */
    if (!envval[0]) {
      /* empty - treat as zero */
    } else if (0 == strcmp("LIST", envval)) {
      if (!gasneti_mynode && !print_once[(int)op]) {
        int i;
        fprintf(stderr, "Listing available values for environment variable %s:\n", envvar);
        for (i=0; i<fullcount; ++i) {
          fprintf(stderr, " %c %3d %s\n", ((i<counts[0])?' ':'*'), i, metadata[i].name);
        }
        if (counts[1]) {
          fprintf(stderr,
                  "Note: Lines marked with '*' may not be valid for all inputs and/or job layouts.\n"
                  "      The user is responsible for ensuring only valid algorithms are requested.\n"
                 );
        }
        print_once[(int)op] = 1;
      }
    } else if (isdigit(envval[0])) {
      /* integer is used just as given */
      alg = atoi(envval);
      if (alg < 0 || alg >= fullcount) {
        if (!gasneti_mynode && !print_once[(int)op]) {
          fprintf(stderr, "WARNING: Ignoring value '%d' for environment variable %s,\n"
                          "         because it is outside the range of available algorithms.\n"
                          "         Set this variable to LIST for a list of all algorithms.\n",
                           alg, envvar);
          print_once[(int)op] = 1;
        }
        alg = 0;
      }
    } else {
      /* string is used for PREFIX match */
      alg = gasnetc_find_alg(envval, metadata, fullcount);
      if (alg == fullcount) {
        if (!gasneti_mynode && !print_once[(int)op] && (envval != dfltval)) {
          fprintf(stderr, "WARNING: Ignoring value '%s' for environment variable %s,\n"
                          "         because it does not match any available algorithm.\n"
                          "         Set this variable to LIST for a list of all algorithms.\n",
                           envval, envvar);
          print_once[(int)op] = 1;
        }
        alg = 0;
      }
    }
  }

  *alg_p = algorithms[alg];
}

static void native_collective(pami_xfer_t *op_p, int need_lock) {
  pami_result_t rc;
  volatile unsigned int counter = 0;

  op_p->cb_done = &gasnetc_cb_inc_uint;
  op_p->cookie = (void *)&counter;
  op_p->options.multicontext = PAMI_HINT_DISABLE;

  if (need_lock) GASNETC_PAMI_LOCK(gasnetc_context);
  rc = PAMI_Collective(gasnetc_context, op_p);
  GASNETC_PAMI_CHECK(rc, "initiating a native collective");
  if (need_lock) GASNETC_PAMI_UNLOCK(gasnetc_context);

  if (gasneti_attach_done) {
    gasneti_polluntil(counter);
  } else {
    rc = gasnetc_wait_uint(gasnetc_context, &counter, 1);
    GASNETC_PAMI_CHECK(rc, "polling a native collective");
  }
}

static pami_xfer_t fast_barrier_op;
static int fast_barrier_is_init = 0;

extern void
gasnetc_fast_barrier(void) {
  gasneti_assert(fast_barrier_is_init);
  native_collective(&fast_barrier_op, 1);
}

extern void
gasnetc_bootstrapBarrier(void) {
  if_pf (!fast_barrier_is_init) {
    memset(&fast_barrier_op, 0, sizeof(fast_barrier_op)); /* Shouldn't need for static, but let's be safe */
    gasnetc_dflt_coll_alg(gasnetc_world_geom, PAMI_XFER_BARRIER, &fast_barrier_op.algorithm);
    fast_barrier_is_init = 1;
  }
  native_collective(&fast_barrier_op, 0);
}


extern void
gasnetc_bootstrapExchange(void *src, size_t len, void *dst) {
  static pami_xfer_t op;
  static int is_init = 0;

  if_pf (!is_init) {
    memset(&op, 0, sizeof(op)); /* Shouldn't need for static, but let's be safe */
    gasnetc_dflt_coll_alg(gasnetc_world_geom, PAMI_XFER_ALLGATHER, &op.algorithm);
    is_init = 1;
  }

  op.cmd.xfer_allgather.sndbuf     = src;
  op.cmd.xfer_allgather.stype      = PAMI_TYPE_BYTE;
  op.cmd.xfer_allgather.stypecount = len;
  op.cmd.xfer_allgather.rcvbuf     = dst;
  op.cmd.xfer_allgather.rtype      = PAMI_TYPE_BYTE;
  op.cmd.xfer_allgather.rtypecount = len; /* times gasneti_nodes */

  native_collective(&op, 0);
}

/* ------------------------------------------------------------------------------------ */
/* Native collectives */

#if GASNET_PAMI_NATIVE_COLL

/* These live here, not in per-op files, to avoid unwanted link dependencies */
int gasnete_use_pami_allga = 0;
int gasnete_use_pami_allto = 0;
int gasnete_use_pami_bcast = 0;
int gasnete_use_pami_gathr = 0;
int gasnete_use_pami_scatt = 0;
pami_xfer_t gasnete_op_template_allga;
pami_xfer_t gasnete_op_template_allto;
pami_xfer_t gasnete_op_template_bcast;
pami_xfer_t gasnete_op_template_gathr;
pami_xfer_t gasnete_op_template_scatt;
#if GASNET_PAR
pami_xfer_t gasnete_op_template_allgavi;
pami_xfer_t gasnete_op_template_alltovi;
pami_xfer_t gasnete_op_template_gathrvi;
pami_xfer_t gasnete_op_template_scattvi;
#endif

static size_t scratch_size;

extern void
gasnete_coll_init_pami(void)
{
  if (gasneti_getenv_yesno_withdefault("GASNET_USE_PAMI_COLL", 1)) {
    scratch_size = gasneti_getenv_int_withdefault("GASNET_PAMI_COLL_SCRATCH", 1024*1024, 1);
    /* We use the int-type scatterv and gatherv on the assumption of reasonable sizes. */
    if (scratch_size > (size_t)INT_MAX) scratch_size = INT_MAX;

    if (gasneti_getenv_yesno_withdefault("GASNET_USE_PAMI_GATHERALL", 1)) {
      memset(&gasnete_op_template_allga, 0, sizeof(pami_xfer_t));
      gasnete_op_template_allga.cb_done = &gasnetc_cb_inc_uint; /* XXX: do we need release semantics? */
      gasnete_op_template_allga.options.multicontext = PAMI_HINT_DISABLE;
      gasnete_op_template_allga.cmd.xfer_allgather.stype = PAMI_TYPE_BYTE;
      gasnete_op_template_allga.cmd.xfer_allgather.rtype = PAMI_TYPE_BYTE;
    #if GASNET_PAR
      memset(&gasnete_op_template_allgavi, 0, sizeof(pami_xfer_t));
      gasnete_op_template_allgavi.cb_done = &gasnetc_cb_inc_uint; /* XXX: do we need release semantics? */
      gasnete_op_template_allgavi.options.multicontext = PAMI_HINT_DISABLE;
      gasnete_op_template_allgavi.cmd.xfer_allgatherv_int.stype = PAMI_TYPE_BYTE;
      gasnete_op_template_allgavi.cmd.xfer_allgatherv_int.rtype = PAMI_TYPE_BYTE;
    #endif
      gasnete_use_pami_allga = 1;
    }

    if (gasneti_getenv_yesno_withdefault("GASNET_USE_PAMI_EXCHANGE", 1)) {
      memset(&gasnete_op_template_allto, 0, sizeof(pami_xfer_t));
      gasnete_op_template_allto.cb_done = &gasnetc_cb_inc_uint; /* XXX: do we need release semantics? */
      gasnete_op_template_allto.options.multicontext = PAMI_HINT_DISABLE;
      gasnete_op_template_allto.cmd.xfer_alltoall.stype = PAMI_TYPE_BYTE;
      gasnete_op_template_allto.cmd.xfer_alltoall.rtype = PAMI_TYPE_BYTE;
    #if GASNET_PAR
      memset(&gasnete_op_template_alltovi, 0, sizeof(pami_xfer_t));
      gasnete_op_template_alltovi.cb_done = &gasnetc_cb_inc_uint; /* XXX: do we need release semantics? */
      gasnete_op_template_alltovi.options.multicontext = PAMI_HINT_DISABLE;
      gasnete_op_template_alltovi.cmd.xfer_alltoallv_int.stype = PAMI_TYPE_BYTE;
      gasnete_op_template_alltovi.cmd.xfer_alltoallv_int.rtype = PAMI_TYPE_BYTE;
    #endif
      gasnete_use_pami_allto = 1;
    }

    if (gasneti_getenv_yesno_withdefault("GASNET_USE_PAMI_BROADCAST", 1)) {
      memset(&gasnete_op_template_bcast, 0, sizeof(pami_xfer_t));
      gasnete_op_template_bcast.cb_done = &gasnetc_cb_inc_uint; /* XXX: do we need release semantics? */
      gasnete_op_template_bcast.options.multicontext = PAMI_HINT_DISABLE;
      gasnete_op_template_bcast.cmd.xfer_broadcast.type = PAMI_TYPE_BYTE;
      gasnete_use_pami_bcast = 1;
    }

    /* DISABLED by default: bug(s) in PE1207 and V1R1M1 have been reported to IBM */
    if (gasneti_getenv_yesno_withdefault("GASNET_USE_PAMI_GATHER", 0)) {
      memset(&gasnete_op_template_gathr, 0, sizeof(pami_xfer_t));
      gasnete_op_template_gathr.cb_done = &gasnetc_cb_inc_uint; /* XXX: do we need release semantics? */
      gasnete_op_template_gathr.options.multicontext = PAMI_HINT_DISABLE;
      gasnete_op_template_gathr.cmd.xfer_gather.stype = PAMI_TYPE_BYTE;
      gasnete_op_template_gathr.cmd.xfer_gather.rtype = PAMI_TYPE_BYTE;
    #if GASNET_PAR
      memset(&gasnete_op_template_gathrvi, 0, sizeof(pami_xfer_t));
      gasnete_op_template_gathrvi.cb_done = &gasnetc_cb_inc_uint; /* XXX: do we need release semantics? */
      gasnete_op_template_gathrvi.options.multicontext = PAMI_HINT_DISABLE;
      gasnete_op_template_gathrvi.cmd.xfer_gatherv_int.stype = PAMI_TYPE_BYTE;
      gasnete_op_template_gathrvi.cmd.xfer_gatherv_int.rtype = PAMI_TYPE_BYTE;
    #endif
      gasnete_use_pami_gathr = 1;
    }

    if (gasneti_getenv_yesno_withdefault("GASNET_USE_PAMI_SCATTER", 1)) {
      memset(&gasnete_op_template_scatt, 0, sizeof(pami_xfer_t));
      gasnete_op_template_scatt.cb_done = &gasnetc_cb_inc_uint; /* XXX: do we need release semantics? */
      gasnete_op_template_scatt.options.multicontext = PAMI_HINT_DISABLE;
      gasnete_op_template_scatt.cmd.xfer_scatter.stype = PAMI_TYPE_BYTE;
      gasnete_op_template_scatt.cmd.xfer_scatter.rtype = PAMI_TYPE_BYTE;
    #if GASNET_PAR
      memset(&gasnete_op_template_scattvi, 0, sizeof(pami_xfer_t));
      gasnete_op_template_scattvi.cb_done = &gasnetc_cb_inc_uint; /* XXX: do we need release semantics? */
      gasnete_op_template_scattvi.options.multicontext = PAMI_HINT_DISABLE;
      gasnete_op_template_scattvi.cmd.xfer_scatterv_int.stype = PAMI_TYPE_BYTE;
      gasnete_op_template_scattvi.cmd.xfer_scatterv_int.rtype = PAMI_TYPE_BYTE;
    #endif
      gasnete_use_pami_scatt = 1;
    }

    /* etc. */
  }
}

/* Called collectively by all team members near end of team construction */
extern void gasnete_coll_team_init_pami(gasnet_team_handle_t team) {
    if (team == GASNET_TEAM_ALL) {
        team->pami.geom = gasnetc_world_geom;
    } else {
        volatile unsigned int done = 0;
        pami_result_t rc;

        rc = PAMI_Geometry_create_tasklist(gasnetc_pami_client, 0,
                                           NULL, 0, /* No config */
                                           &team->pami.geom,
                                           PAMI_GEOMETRY_NULL, /* No parent */
                                           team->team_id,
                                           team->rel2act_map, team->total_ranks,
                                           gasnetc_context, &gasnetc_cb_inc_uint, (void*)&done);
        GASNETC_PAMI_CHECK(rc, "calling PAMI_Geometry_create_tasklist()");

        rc = gasnetc_wait_uint(gasnetc_context, &done, 1);
        GASNETC_PAMI_CHECK(rc, "advancing PAMI_Geometry_create_tasklist()");
    }

    if (gasnete_use_pami_allga) {
        gasnetc_dflt_coll_alg(team->pami.geom, PAMI_XFER_ALLGATHER, &team->pami.allga_alg);
      #if GASNET_PAR
        gasnetc_dflt_coll_alg(team->pami.geom, PAMI_XFER_ALLGATHERV_INT, &team->pami.allgavi_alg);
      #endif
    }
    if (gasnete_use_pami_allto) {
        gasnetc_dflt_coll_alg(team->pami.geom, PAMI_XFER_ALLTOALL, &team->pami.allto_alg);
      #if GASNET_PAR
        gasnetc_dflt_coll_alg(team->pami.geom, PAMI_XFER_ALLTOALLV_INT, &team->pami.alltovi_alg);
      #endif
    }
    if (gasnete_use_pami_bcast) {
        gasnetc_dflt_coll_alg(team->pami.geom, PAMI_XFER_BROADCAST, &team->pami.bcast_alg);
    }
    if (gasnete_use_pami_gathr) {
        gasnetc_dflt_coll_alg(team->pami.geom, PAMI_XFER_GATHER, &team->pami.gathr_alg);
      #if GASNET_PAR
        gasnetc_dflt_coll_alg(team->pami.geom, PAMI_XFER_GATHERV_INT, &team->pami.gathrvi_alg);
      #endif
    }
    if (gasnete_use_pami_scatt) {
        gasnetc_dflt_coll_alg(team->pami.geom, PAMI_XFER_SCATTER, &team->pami.scatt_alg);
      #if GASNET_PAR
        gasnetc_dflt_coll_alg(team->pami.geom, PAMI_XFER_SCATTERV_INT, &team->pami.scattvi_alg);
      #endif
    }

  #if GASNET_PAR
    team->pami.scratch_max_nbytes = scratch_size / team->max_images;
    team->pami.scratch_max_nbytes_allto = team->pami.scratch_max_nbytes / (2 * team->total_images);
    team->pami.scratch_space = gasneti_malloc(scratch_size);
    team->pami.counts = gasneti_malloc(2 * sizeof(int) * team->total_ranks);
    team->pami.displs = team->pami.counts + team->total_ranks;
    team->pami.prev_nbytes = 0;
    team->pami.tmp_addr = NULL;
    team->pami.barrier_phase = 0;
    gasneti_atomic_set(&team->pami.barrier_counter[0], team->my_images, 0);
    gasneti_atomic_set(&team->pami.barrier_counter[1], team->my_images, 0);
  #endif
}

#endif /* GASNET_PAMI_NATIVE_COLL */
