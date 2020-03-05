/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testratomic.c $
 * Description: GASNet remote atomics correctness tests
 * Copyright (c) 2017, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#include <gasnetex.h>
#include <gasnet_coll.h>

static size_t scratch_sz1;
static size_t scratch_sz2;
#ifndef TEST_SEGSZ
#define TEST_SEGSZ_EXPR (PAGESZ + scratch_sz1 + scratch_sz2) // space for test + 2 teams
#endif

#include <test.h>


// NOTE regarding "atomic access phases":
//
// Currently, each of the subtests in this file uses the same 16 bytes (or
// less) at the start of each rank's segment.  The rules for use of multiple
// ADs with the same target location requires a "transition" between atomic
// access phases.  While the mechanism(s) for that are not fully specified
// yet, we do have the grantee that AD destruction ends an atomic access
// phase.  For that reason, all current subtests are grantee with respect to
// phases only because they each quiesce and destroy the AD before the next
// subtest begins.

static gex_Client_t  myclient;
static gex_EP_t      myep;
static gex_TM_t      myteam;
static gex_Segment_t mysegment;

#include <gasnet_ratomic.h>

#include <stdint.h>
#include <float.h>

static gex_Rank_t myrank;
static gex_Rank_t numranks;
static gex_Rank_t peer;
static void *peerseg;

static int iters = 0;

/* Hidden state for error reporting and recovery */
static const char* subtest = "N/A";
#define SUBTEST(_name) subtest = _name
static int prev_fail = 0;
static int failures = 0;

static int multinbrhd;
static int mynbrhd;          // jobrank of first member of the neighborhood
static gex_Rank_t neighbor;  // next neighbor w/ wrap-around, possibly self.
static gex_Rank_t nbrhdsize; // size of the neighborhood
static gex_Rank_t nbrhdrank; // rank in the neighborhood
static gex_RankInfo_t *nbrhdinfo;

// Macros to simplify iteration over types
#define I32_type  int32_t
#define I32_isint 1
#define U32_type  uint32_t
#define U32_isint 1
#define I64_type  int64_t
#define I64_isint 1
#define U64_type  uint64_t
#define U64_isint 1
#define FLT_type  float
#define FLT_isint 0
#define DBL_type  double
#define DBL_isint 0
#define FORALL_DT(MACRO) \
  MACRO(I32) MACRO(U32)  \
  MACRO(I64) MACRO(U64)  \
  MACRO(FLT) MACRO(DBL)

/* Blocking atomic via either NB or NBI (chosen at random) */
/* With or without IMMEDIATE (also at random) */
/* With or without RANK_IS_JOBRANK (also at random) */
/* Note that some arguments and variables are hard-coded */
#define _TEST_ROP(_tcode, _result_p, _opcode, _op1, _op2) do { \
    gex_Flags_t flags = TEST_RAND_ONEIN(2) ? GEX_FLAG_IMMEDIATE : 0;                       \
    gex_Rank_t tgt = peer;                                                                 \
    if (TEST_RAND_ONEIN(4)) {                                                              \
      flags |= GEX_FLAG_RANK_IS_JOBRANK;                                                   \
      tgt = gex_TM_TranslateRankToJobrank(gex_AD_QueryTM(ad), tgt);                        \
    }                                                                                      \
    if (TEST_RAND_ONEIN(2)) {                                                              \
      gex_Event_t ev;                                                                      \
      while (GEX_EVENT_NO_OP ==                                                            \
             (ev = gex_AD_OpNB_##_tcode(ad,_result_p,tgt,peerseg,_opcode,_op1,_op2,flags))) {\
        assert_always(flags & GEX_FLAG_IMMEDIATE);                                         \
        flags &= ~GEX_FLAG_IMMEDIATE;                                                      \
      }                                                                                    \
      gex_Event_Wait(ev);                                                                  \
    } else {                                                                               \
      while (gex_AD_OpNBI_##_tcode(ad,_result_p,tgt,peerseg,_opcode,_op1,_op2,flags)) {    \
        assert_always(flags & GEX_FLAG_IMMEDIATE);                                         \
        flags &= ~GEX_FLAG_IMMEDIATE;                                                      \
      }                                                                                    \
      gex_NBI_Wait(GEX_EC_ALL,0);                                                          \
    }                                                                                      \
  } while (0)

// Update mirror value
// Also store remotely if (and only if) previous check failed
#define _TEST_ROP_MIRROR(_tcode, _newval) do { \
    _tcode##_mirror = _newval;                 \
    if_pf (prev_fail) {                        \
      gex_Event_Wait(gex_AD_OpNB_##_tcode(ad,NULL,peer,peerseg,GEX_OP_SET,_tcode##_mirror,0,0)); \
    }                                          \
    prev_fail = 0;                             \
  } while (0)

/* Test an atomic op (fetching and non-fetching variants)
 * NC suffix = no change
 */
#define TEST_ROP_NC(_tcode, _opcode, _op1, _op2)  do { \
        if (! (_opcode & ops)) break;                 \
        _TEST_ROP(_tcode, NULL, _opcode, _op1, _op2); \
  } while (0)
#define TEST_ROP_FETCH_NC(_tcode, _opcode, _op1, _op2) do { \
    if (! (_opcode & ops)) break;                        \
    _TEST_ROP(_tcode, &_tcode##_fetch, _opcode, _op1, _op2); \
    prev_fail = (_tcode##_fetch != _tcode##_mirror);     \
    if_pf (prev_fail) {                                  \
      ++failures;                                        \
      static int once = 0;                               \
      if (!once) {                                       \
        ERR("Valued fetched by \"%s\" did not match expected value "  \
            "(got %d, want %d)\n", subtest,              \
            (int)_tcode##_fetch, (int)_tcode##_mirror);  \
        once = 1;                                        \
      }                                                  \
    }                                                    \
  } while (0)

/* As above, but also update the expected value and store
 * it remotely if the most recent fetching op failed validation
 */
#define TEST_ROP(_tcode, _opcode, _op1, _op2, _newval)  do { \
    TEST_ROP_NC(_tcode, _opcode, _op1, _op2); \
    if (_opcode & ops) _TEST_ROP_MIRROR(_tcode, _newval); \
  } while (0)
#define TEST_ROP_FETCH(_tcode, _opcode, _op1, _op2, _newval)  do { \
    TEST_ROP_FETCH_NC(_tcode, _opcode, _op1, _op2); \
    if (_opcode & ops) _TEST_ROP_MIRROR(_tcode, _newval); \
  } while (0)



/* Randomized testing of atomic ops */
#define TEST_RAND_DECL(_dtcode) \
        TEST_RAND_DECL1(_dtcode, _dtcode##_type, _dtcode##_isint, \
                        _dtcode##_mirror, _dtcode##_unused, _dtcode##_x, _dtcode##_y)
/* extra pass to expand concatenations: */
#define TEST_RAND_DECL1(_tcode, _type, _isint, mirror, unused, x, y) \
        TEST_RAND_DECL2(_tcode, _type, _isint, mirror, unused, x, y)
#define TEST_RAND_DECL2(_tcode, _type, _isint, mirror, unused, x, y) \
static _type _tcode##_fetch, mirror, unused, x, y; \
GASNETT_NEVER_INLINE(test_rand1_##_tcode, \
void test_rand1_##_tcode(gex_AD_t ad, gex_OP_t ops, int64_t lo, int64_t hi)) {\
    SUBTEST("SET(x)");                                        \
      TEST_ROP(_tcode, GEX_OP_SET, x, unused, x);             \
    SUBTEST("GET(x)");                                        \
      TEST_ROP_FETCH_NC(_tcode, GEX_OP_GET, unused, unused);  \
    SUBTEST("SET(0)");                                        \
      TEST_ROP(_tcode, GEX_OP_SET, 0, unused, 0);             \
    SUBTEST("GET(0)");                                        \
      TEST_ROP_FETCH_NC(_tcode, GEX_OP_GET, unused, unused);  \
} \
GASNETT_NEVER_INLINE(test_rand2_##_tcode, \
void test_rand2_##_tcode(gex_AD_t ad, gex_OP_t ops, int64_t lo, int64_t hi)) {\
    SUBTEST("FINC()");                                        \
      TEST_ROP_FETCH(_tcode, GEX_OP_FINC, unused, unused, mirror + 1); \
    SUBTEST("INC()");                                         \
      TEST_ROP(_tcode, GEX_OP_INC, unused, unused, mirror + 1); \
    SUBTEST("FADD(x+1)");                                     \
      TEST_ROP_FETCH(_tcode, GEX_OP_FADD, (x+1), unused, mirror + x + 1); \
    SUBTEST("ADD(x+1)");                                      \
      TEST_ROP(_tcode, GEX_OP_ADD, (x+1), unused, mirror + x + 1); \
} \
GASNETT_NEVER_INLINE(test_rand3_##_tcode, \
void test_rand3_##_tcode(gex_AD_t ad, gex_OP_t ops, int64_t lo, int64_t hi)) {\
    SUBTEST("FDEC()");                                        \
      TEST_ROP_FETCH(_tcode, GEX_OP_FDEC, unused, unused, mirror - 1); \
    SUBTEST("DEC()");                                         \
      TEST_ROP(_tcode, GEX_OP_DEC, unused, unused, mirror - 1); \
    SUBTEST("FSUB(x)");                                       \
      TEST_ROP_FETCH(_tcode, GEX_OP_FSUB, x, unused, mirror - x); \
    SUBTEST("SUB(x)");                                        \
      TEST_ROP(_tcode, GEX_OP_SUB, x, unused, mirror - x);    \
} \
GASNETT_NEVER_INLINE(test_rand4_##_tcode, \
void test_rand4_##_tcode(gex_AD_t ad, gex_OP_t ops, int64_t lo, int64_t hi)) {\
    SUBTEST("FMULT(x)");                                      \
      TEST_ROP_FETCH(_tcode, GEX_OP_FMULT, x, unused, mirror * x); \
    SUBTEST("MULT(x)");                                       \
      TEST_ROP(_tcode, GEX_OP_MULT, x, unused, mirror * x);   \
} \
GASNETT_NEVER_INLINE(test_rand5_##_tcode, \
void test_rand5_##_tcode(gex_AD_t ad, gex_OP_t ops, int64_t lo, int64_t hi)) {\
    SUBTEST("SWAP(x)");                                       \
      TEST_ROP_FETCH(_tcode, GEX_OP_SWAP, x, unused, x);      \
    SUBTEST("FCAS(mirror,mirror) - PASS");                    \
      TEST_ROP_FETCH_NC(_tcode, GEX_OP_FCAS, mirror, mirror); \
    SUBTEST("FCAS(mirror+1,0) - FAIL");                       \
      TEST_ROP_FETCH_NC(_tcode, GEX_OP_FCAS, mirror+1, 0);    \
} \
GASNETT_NEVER_INLINE(test_rand6_##_tcode, \
void test_rand6_##_tcode(gex_AD_t ad, gex_OP_t ops, int64_t lo, int64_t hi)) {\
    SUBTEST("FCAS(mirror,random) - PASS");                    \
      y = (_type)TEST_RAND(lo,hi);                            \
      TEST_ROP_FETCH(_tcode, GEX_OP_FCAS, mirror, y, y);      \
    SUBTEST("FCAS(random,random) - FAIL");                    \
      do { y = (_type)TEST_RAND(lo,hi); } while (y == mirror);\
      TEST_ROP_FETCH_NC(_tcode, GEX_OP_FCAS, y, y);           \
    SUBTEST("GET(fcas)");                                     \
      TEST_ROP_FETCH_NC(_tcode, GEX_OP_GET, unused, unused);  \
} \
GASNETT_NEVER_INLINE(test_rand7_##_tcode, \
void test_rand7_##_tcode(gex_AD_t ad, gex_OP_t ops, int64_t lo, int64_t hi)) {\
    SUBTEST("CAS(mirror,mirror) - PASS");                     \
      TEST_ROP_NC(_tcode, GEX_OP_CAS, mirror, mirror);        \
    SUBTEST("GET(cas1)");                                     \
      TEST_ROP_FETCH_NC(_tcode, GEX_OP_GET, unused, unused);  \
    SUBTEST("CAS(mirror+1,0) - FAIL");                        \
      TEST_ROP_NC(_tcode, GEX_OP_CAS, mirror+1, 0);           \
    SUBTEST("GET(cas2)");                                     \
      TEST_ROP_FETCH_NC(_tcode, GEX_OP_GET, unused, unused);  \
} \
GASNETT_NEVER_INLINE(test_rand8_##_tcode, \
void test_rand8_##_tcode(gex_AD_t ad, gex_OP_t ops, int64_t lo, int64_t hi)) {\
    SUBTEST("CAS(mirror,random) - PASS");                     \
      y = (_type)TEST_RAND(lo,hi);                            \
      TEST_ROP(_tcode, GEX_OP_CAS, mirror, y, y);             \
    SUBTEST("GET(cas3)");                                     \
      TEST_ROP_FETCH_NC(_tcode, GEX_OP_GET, unused, unused);  \
    SUBTEST("CAS(random,random) - FAIL");                     \
      do { y = (_type)TEST_RAND(lo,hi); } while (y == mirror);\
      TEST_ROP_NC(_tcode, GEX_OP_CAS, y, y);                  \
    SUBTEST("GET(cas4)");                                     \
      TEST_ROP_FETCH_NC(_tcode, GEX_OP_GET, unused, unused);  \
} \
GASNETT_NEVER_INLINE(test_rand9_##_tcode, \
void test_rand9_##_tcode(gex_AD_t ad, gex_OP_t ops, int64_t lo, int64_t hi)) {\
    SUBTEST("MIN(random)");                                   \
      y = (_type)TEST_RAND(lo,hi);                            \
      y = TEST_RAND_ONEIN(2) ? y : ((_type)-1) * y;           \
      TEST_ROP(_tcode, GEX_OP_MIN, y, unused, MIN(mirror,y)); \
    SUBTEST("FMIN(random)");                                  \
      y = (_type)TEST_RAND(lo,hi);                            \
      y = TEST_RAND_ONEIN(2) ? y : ((_type)-1) * y;           \
      TEST_ROP_FETCH(_tcode, GEX_OP_FMIN, y, unused, MIN(mirror,y)); \
    SUBTEST("MAX(random)");                                   \
      y = (_type)TEST_RAND(lo,hi);                            \
      y = TEST_RAND_ONEIN(2) ? y : ((_type)-1) * y;           \
      TEST_ROP(_tcode, GEX_OP_MAX, y, unused, MAX(mirror,y)); \
    SUBTEST("FMAX(random)");                                  \
      y = (_type)TEST_RAND(lo,hi);                            \
      y = TEST_RAND_ONEIN(2) ? y : ((_type)-1) * y;           \
      TEST_ROP_FETCH(_tcode, GEX_OP_FMAX, y, unused, MAX(mirror,y)); \
} \
GASNETT_NEVER_INLINE(test_rand_##_tcode, \
void test_rand_##_tcode(gex_AD_t ad, int64_t lo, int64_t hi, const char *msg)) {\
  mirror = 0;                                                 \
  gex_OP_t ops = gex_AD_QueryOps(ad);                         \
  MSG0("    Randomized ops test with operation set 0x%x%s",   \
       (unsigned int)ops, msg);                               \
  for (int i = 0; i < iters; ++i) {                           \
    unused = (_type)TEST_RAND(lo,hi); /* garbage */           \
    /* first few iterations are NON-random (0, 1, 2) */       \
    x = (i <= 2) ? (_type)i : (_type)TEST_RAND(lo,hi);        \
    test_rand1_##_tcode(ad, ops, lo, hi);                     \
    test_rand2_##_tcode(ad, ops, lo, hi);                     \
    test_rand3_##_tcode(ad, ops, lo, hi);                     \
    test_rand4_##_tcode(ad, ops, lo, hi);                     \
    test_rand5_##_tcode(ad, ops, lo, hi);                     \
    test_rand6_##_tcode(ad, ops, lo, hi);                     \
    test_rand7_##_tcode(ad, ops, lo, hi);                     \
    test_rand8_##_tcode(ad, ops, lo, hi);                     \
    test_rand9_##_tcode(ad, ops, lo, hi);                     \
    TEST_RAND_BITS##_isint(_tcode,_type,mirror,unused,x,y);   \
    SUBTEST("GET(final)");                                    \
      TEST_ROP_FETCH_NC(_tcode, GEX_OP_GET, unused, unused);  \
  }                                                           \
  if (failures) {                                             \
    MSG("  Total: %d failures for type " #_type, failures);   \
    failures = 0;                                             \
  }                                                           \
}
#define TEST_RAND_BITS0(_tcode,_type,mirror,unused,x,y) /*empty*/
#define TEST_RAND_BITS1(_tcode,_type,mirror,unused,x,y) \
    SUBTEST("AND(random)");                                   \
      y = (_type)TEST_RAND(lo,hi);                            \
      TEST_ROP(_tcode, GEX_OP_AND, y, unused, mirror & y);    \
    SUBTEST("FAND(random)");                                  \
      y = (_type)TEST_RAND(lo,hi);                            \
      TEST_ROP_FETCH(_tcode, GEX_OP_FAND, y, unused, mirror & y); \
    SUBTEST("OR(random)");                                    \
      y = (_type)TEST_RAND(lo,hi);                            \
      TEST_ROP(_tcode, GEX_OP_OR, y, unused, mirror | y);     \
    SUBTEST("FOR(random)");                                   \
      y = (_type)TEST_RAND(lo,hi);                            \
      TEST_ROP_FETCH(_tcode, GEX_OP_FOR, y, unused, mirror | y);  \
    SUBTEST("XOR(random)");                                   \
      y = (_type)TEST_RAND(lo,hi);                            \
      TEST_ROP(_tcode, GEX_OP_XOR, y, unused, mirror ^ y);    \
    SUBTEST("FXOR(random)");                                  \
      y = (_type)TEST_RAND(lo,hi);                            \
      TEST_ROP_FETCH(_tcode, GEX_OP_FXOR, y, unused, mirror ^ y); \
//
FORALL_DT(TEST_RAND_DECL)

/* Deterministic testing of AD_MY_* flags */
#define TEST_FLAGS_DECL(_tcode) \
GASNETT_NEVER_INLINE(test_flags_##_tcode, \
void test_flags_##_tcode(gex_AD_t ad)) {                                     \
  gex_TM_t testtm = gex_AD_QueryTM(ad);                                      \
  gex_Event_t ev;                                                            \
  _tcode##_type result, operand;                                             \
  _tcode##_type unused = 911; /* garbage */                                  \
  MSG0("    Flags test");                                                    \
                                                                             \
  /* MY_RANK and MY_NBRHD applied to self */                                 \
  operand = myrank + 1;                                                      \
  ev = gex_AD_OpNB_##_tcode(ad,NULL,myrank,TEST_MYSEG(),GEX_OP_SET,          \
                            operand,unused,GEX_FLAG_AD_MY_RANK);             \
  gex_Event_Wait(ev);                                                        \
  gex_AD_OpNBI_##_tcode(ad,&result,myrank,TEST_MYSEG(),GEX_OP_FADD,          \
                        operand,unused,GEX_FLAG_AD_MY_RANK);                 \
  gex_NBI_Wait(GEX_EC_RMW,0);                                                \
  assert_always(result == operand);                                          \
  ev = gex_AD_OpNB_##_tcode(ad,&result,myrank,TEST_MYSEG(),GEX_OP_FADD,      \
                            operand,unused,GEX_FLAG_AD_MY_NBRHD);            \
  gex_Event_Wait(ev);                                                        \
  assert_always(result == 2*operand);                                        \
  BARRIER();                                                                 \
                                                                             \
  /* MY_NBRHD applied to not-self-unless-no-other-valid-choice */            \
  void * nbr_addr = TEST_SEG_TM(testtm, neighbor);                           \
  assert(nbr_addr);                                                          \
  operand = neighbor + 1;                                                    \
  gex_AD_OpNBI_##_tcode(ad,&result,neighbor,nbr_addr,GEX_OP_FADD,            \
                        operand,unused,GEX_FLAG_AD_MY_NBRHD);                \
  gex_NBI_Wait(GEX_EC_RMW,0);                                                \
  assert_always(result == 3*operand);                                        \
}
FORALL_DT(TEST_FLAGS_DECL)

/* (F)ADD/(F)INC race test */
#define TEST_CNTR_DECL(_tcode) \
GASNETT_NEVER_INLINE(test_cntr_##_tcode, \
void test_cntr_##_tcode(gex_AD_t ad, int max_goal)) {                        \
  gex_TM_t testtm = gex_AD_QueryTM(ad);                                      \
  MSG0("    Central-counter concurrent updates test (FADD/ADD/FINC/INC)");   \
  _tcode##_type unused = 911; /* garbage */                                  \
  int goal = MIN(iters, max_goal);                                           \
  int my_share = (goal / numranks) + ((myrank < (goal % numranks)) ? 1 : 0); \
  if (!myrank) {                                                             \
    gex_Event_Wait(gex_AD_OpNB_##_tcode(ad,NULL,0,TEST_MYSEG(),GEX_OP_SET,   \
                                        0,unused,GEX_FLAG_AD_MY_RANK));      \
  }                                                                          \
  BARRIER();                                                                 \
  _tcode##_type result;                                                      \
  { /* Next line intentionally shadows two globals */                        \
    gex_Rank_t peer = 0;  void * peerseg = TEST_SEG_TM(testtm, 0);           \
    _tcode##_type prev_result = 0;                                           \
    int remain = my_share;                                                   \
    while (remain) {                                                         \
      if (TEST_RAND_ONEIN(20)) { /* Mix in an occasional GET */              \
        _TEST_ROP(_tcode, &result, GEX_OP_GET, unused, unused);              \
      } else if (TEST_RAND_ONEIN(2)) {                                       \
        /* (F)ADD with a small (possibly 0) random increment */              \
        const int max_incr = 4;                                              \
        int incr = (remain < max_incr) ? 1 : TEST_RAND(0, max_incr);         \
        remain -= incr;                                                      \
        if (TEST_RAND_ONEIN(2)) {                                            \
          _TEST_ROP(_tcode, &result, GEX_OP_FADD, incr, unused);             \
        } else {                                                             \
          _TEST_ROP(_tcode, NULL, GEX_OP_ADD, incr, unused);                 \
          continue; /* skip checks of monotonicity and range */              \
        }                                                                    \
      } else {                                                               \
        /* (F)INC */                                                         \
        remain -= 1;                                                         \
        if (TEST_RAND_ONEIN(2)) {                                            \
          _TEST_ROP(_tcode, &result, GEX_OP_FINC, unused, unused);           \
        } else {                                                             \
          _TEST_ROP(_tcode, NULL, GEX_OP_INC, unused, unused);               \
          continue; /* skip checks of monotonicity and range */              \
        }                                                                    \
      }                                                                      \
      assert_always(!prev_result || result >= prev_result);                  \
      assert_always(result < goal);                                          \
      prev_result = result;                                                  \
    }                                                                        \
  }                                                                          \
  BARRIER();                                                                 \
  if (!myrank) {                                                             \
    gex_Event_Wait(gex_AD_OpNB_##_tcode(ad,&result,0,TEST_MYSEG(),GEX_OP_GET,\
                                        unused,unused,GEX_FLAG_AD_MY_RANK)); \
    assert_always(result == goal);                                           \
  }                                                                          \
}
FORALL_DT(TEST_CNTR_DECL)

/* FCAS race test */
#define TEST_FCAS_DECL(_tcode) \
GASNETT_NEVER_INLINE(test_fcas_##_tcode, \
void test_fcas_##_tcode(gex_AD_t ad, int max_goal)) {                        \
  gex_TM_t testtm = gex_AD_QueryTM(ad);                                      \
  MSG0("    Central-counter concurrent updates test (FCAS)");                \
  _tcode##_type unused = 911; /* garbage */                                  \
  int goal = MIN(iters, max_goal);                                           \
  int my_share = (goal / numranks) + ((myrank < (goal % numranks)) ? 1 : 0); \
  if (!myrank) {                                                             \
    gex_Event_Wait(gex_AD_OpNB_##_tcode(ad,NULL,0,TEST_MYSEG(),GEX_OP_SET,   \
                                        0,unused,GEX_FLAG_AD_MY_RANK));      \
  }                                                                          \
  BARRIER();                                                                 \
  _tcode##_type result;                                                      \
  { /* Next line intentionally shadows two globals */                        \
    gex_Rank_t peer = 0;  void * peerseg = TEST_SEG_TM(testtm, 0);           \
    _tcode##_type oldval = 0;                                                \
    int remain = my_share;                                                   \
    while (remain) {                                                         \
      int swapped;                                                           \
      if (TEST_RAND_ONEIN(20)) { /* Mix in an occasional GET */              \
        _TEST_ROP(_tcode, &result, GEX_OP_GET, unused, unused);              \
        swapped = 0;                                                         \
      } else {                                                               \
        _TEST_ROP(_tcode, &result, GEX_OP_FCAS, oldval, oldval+1);           \
        swapped = (oldval == result) ? 1 : 0;                                \
      }                                                                      \
      assert_always(!oldval || result >= oldval);                            \
      assert_always(result < goal);                                          \
      oldval = result + swapped;                                             \
      remain -= swapped;                                                     \
    }                                                                        \
  }                                                                          \
  BARRIER();                                                                 \
  if (!myrank) {                                                             \
    gex_Event_Wait(gex_AD_OpNB_##_tcode(ad,&result,0,TEST_MYSEG(),GEX_OP_GET,\
                                        unused,unused,GEX_FLAG_AD_MY_RANK)); \
    assert_always(result == goal);                                           \
  }                                                                          \
}
FORALL_DT(TEST_FCAS_DECL)

/* Producer/consume ring tests */
#define _TEST_RING_DECL(_tcode,_op) \
GASNETT_NEVER_INLINE(_test_ring_##_op##_##_tcode, \
void _test_ring_##_op##_##_tcode(gex_AD_t ad, uint64_t max_val, int nbrhd)) {  \
  gex_TM_t testtm = gex_AD_QueryTM(ad);                                      \
  MSG0("    Producer/consumer %s ring test (" #_op ")",                   \
       (nbrhd ? "multiple" : "single"));                                  \
  const gex_Flags_t flags = nbrhd ? GEX_FLAG_AD_MY_NBRHD : 0;             \
  const gex_Rank_t tgt = nbrhd ? neighbor : peer;                         \
  const int wrap = (tgt <= myrank);                                       \
  _tcode##_type *myX = (_tcode##_type *)TEST_MYSEG();                     \
  _tcode##_type *myY = myX + 1;                                           \
  _tcode##_type *tgtX = (_tcode##_type *)TEST_SEG_TM(testtm, tgt);        \
  _tcode##_type *tgtY = tgtX + 1;                                         \
  unsigned int limit = iters/numranks;                                    \
  limit = MIN(limit, INT_MAX);                                            \
  limit = (unsigned int) MIN((uint64_t) limit, max_val);                  \
  /* Take steps to test widest possible range... */                       \
  uint64_t step = max_val / (limit + 1);                                  \
  /* ... subject to a constraint that low half cannot be zero */          \
  step -= (step & ((((uint64_t)1)<<(4*sizeof(step)))-1)) ? 0 : 1;         \
  /* start at 0, except first rank in each ring will start at 1 */        \
  { const _tcode##_type init = step * (nbrhd ? !nbrhdrank : !myrank);     \
    gex_Event_Wait(gex_AD_OpNB_##_tcode(ad,NULL,myrank,myX,GEX_OP_SET,    \
                                        init,0,GEX_FLAG_AD_MY_RANK));     \
    gex_Event_Wait(gex_AD_OpNB_##_tcode(ad,NULL,myrank,myY,GEX_OP_SET,    \
                                        init,0,GEX_FLAG_AD_MY_RANK));     \
  }                                                                       \
  BARRIER();                                                              \
  for (unsigned int i = 0; i < limit; ++i) {                              \
    const _tcode##_type expect = ((_tcode##_type)step) * (i + 1);         \
    _tcode##_type readX = 0;                                              \
    _tcode##_type readY = 0;                                              \
    /* CONSUMER: First OP uses ACQ */                                     \
    while (1) {                                                           \
      _TEST_RING_CONSUME_##_op(_tcode);                                   \
      if (readY == expect) break;                                         \
      if (readY != (expect - step)) {                                     \
        static int once = 0;                                              \
        if (!once) {                                                      \
          ERR("Read Y value %" PRIu64 " did not match expected %" PRIu64  \
              " nor previous expected value %" PRIu64, (uint64_t)readY,   \
              (uint64_t)(expect - step), (uint64_t)expect);               \
          once = 1;                                                       \
        }                                                                 \
      }                                                                   \
      test_yield_if_polite();                                             \
      gasnet_AMPoll();                                                    \
    }                                                                     \
    if (readX != expect) {                                                \
      static int once = 0;                                                \
      if (!once) {                                                        \
        ERR("Read X value %" PRIu64 " did not match expected %" PRIu64,   \
            (uint64_t)readX, (uint64_t)expect);                           \
        once = 1;                                                         \
      }                                                                   \
    }                                                                     \
    /* PRODUCER: write(X) + write(Y,REL) */                               \
    _TEST_RING_PRODUCE_##_op(_tcode);                                     \
  }                                                                       \
  BARRIER();                                                              \
}                                                                         \
void test_ring_##_op##_##_tcode(gex_AD_t ad, uint64_t max_val, int nbrhd) { \
    _test_ring_##_op##_##_tcode(ad, max_val, 0);                            \
    if (nbrhd) _test_ring_##_op##_##_tcode(ad, max_val, 1);                 \
}
//
// Producers:
// Mix SET with the OP for which the subtest is named (SET, SWAP, FCAS, ADD, XOR)
//
#define _TEST_RING_PRODUCE(_tcode,_opA,_op1A,_opB,_op1B) do { \
    const uint64_t prev = step * (i + wrap);                                                 \
    const uint64_t next = (prev + step);                                                     \
    const _tcode##_type op1A = (_tcode##_type)(_op1A);                                       \
    const _tcode##_type op1B = (_tcode##_type)(_op1B);                                       \
    const _tcode##_type op2 = (_tcode##_type)(next);                                         \
    _tcode##_type tmp;                                                                       \
    if (TEST_RAND_ONEIN(2)) {                                                                \
      gex_Event_Wait(                                                                        \
          gex_AD_OpNB_##_tcode(ad,&tmp,tgt,tgtX,GEX_OP_##_opA,op1A,op2,flags));              \
      gex_Event_Wait(                                                                        \
          gex_AD_OpNB_##_tcode(ad,&tmp,tgt,tgtY,GEX_OP_##_opB,op1B,op2,flags|GEX_FLAG_AD_REL));\
    } else {                                                                                 \
      gex_AD_OpNBI_##_tcode(ad,&tmp,tgt,tgtX,GEX_OP_##_opA,op1A,op2,flags);                  \
      gex_NBI_Wait(GEX_EC_ALL,0);                                                            \
      gex_AD_OpNBI_##_tcode(ad,&tmp,tgt,tgtY,GEX_OP_##_opB,op1B,op2,flags|GEX_FLAG_AD_REL);  \
      gex_NBI_Wait(GEX_EC_ALL,0);                                                            \
    }                                                                                        \
  } while (0)
#define _TEST_RING_PRODUCE1(_tcode,_op,_op1) _TEST_RING_PRODUCE(_tcode,_op,_op1,_op,_op1)
#define _TEST_RING_PRODUCE2(_tcode,_op,_op1) \
  switch (TEST_RAND(0,3)) {                                      \
    case 0: _TEST_RING_PRODUCE(_tcode,SET,next,SET,next); break; \
    case 1: _TEST_RING_PRODUCE(_tcode,SET,next,_op,_op1); break; \
    case 2: _TEST_RING_PRODUCE(_tcode,_op,_op1,SET,next); break; \
    case 3: _TEST_RING_PRODUCE(_tcode,_op,_op1,_op,_op1); break; \
  }
#define _TEST_RING_PRODUCE_SET(_tcode)   _TEST_RING_PRODUCE1(_tcode,SET,next)
#define _TEST_RING_PRODUCE_SWAP(_tcode)  _TEST_RING_PRODUCE2(_tcode,SWAP,next)
#define _TEST_RING_PRODUCE_FCAS(_tcode)  _TEST_RING_PRODUCE2(_tcode,FCAS,prev)
#define _TEST_RING_PRODUCE_ADD(_tcode)   _TEST_RING_PRODUCE2(_tcode,ADD,step)
#define _TEST_RING_PRODUCE_XOR(_tcode)   _TEST_RING_PRODUCE2(_tcode,XOR,(prev^next))
//
// Consumers:
//
// When producer uses SET or SWAP, consumer uses only GET.
// When producer uses FCAS, consumer mixes GET with a no-op FCAS(0,0).
// When producer uses ADD, consumer mixes GET with a no-op FADD(0).
// When producer uses XOR, consumer mixes GET with a no-op FXOR(0).
//
#define _TEST_RING_CONSUME(_tcode,_opA,_opB) do { \
    if (TEST_RAND_ONEIN(2)) {                                                     \
      gex_Event_Wait(gex_AD_OpNB_##_tcode(ad,&readY,myrank,myY,GEX_OP_##_opA,0,0, \
                                          GEX_FLAG_AD_MY_RANK|GEX_FLAG_AD_ACQ));  \
      gex_Event_Wait(gex_AD_OpNB_##_tcode(ad,&readX,myrank,myX,GEX_OP_##_opB,0,0, \
                                          GEX_FLAG_AD_MY_RANK));                  \
    } else {                                                                      \
      gex_AD_OpNBI_##_tcode(ad,&readY,myrank,myY,GEX_OP_##_opA,0,0,               \
                            GEX_FLAG_AD_MY_RANK|GEX_FLAG_AD_ACQ);                 \
      gex_NBI_Wait(GEX_EC_ALL,0);                                                 \
      gex_AD_OpNBI_##_tcode(ad,&readX,myrank,myX,GEX_OP_##_opB,0,0,               \
                            GEX_FLAG_AD_MY_RANK);                                 \
      gex_NBI_Wait(GEX_EC_ALL,0);                                                 \
    }                                                                             \
  } while (0)
#define _TEST_RING_CONSUME1(_tcode) _TEST_RING_CONSUME(_tcode,GET,GET)
#define _TEST_RING_CONSUME2(_tcode,_op) \
  switch (TEST_RAND(0,3)) {                            \
    case 0: _TEST_RING_CONSUME(_tcode,GET,GET); break; \
    case 1: _TEST_RING_CONSUME(_tcode,GET,_op); break; \
    case 2: _TEST_RING_CONSUME(_tcode,_op,GET); break; \
    case 3: _TEST_RING_CONSUME(_tcode,_op,_op); break; \
  }
#define _TEST_RING_CONSUME_SET(_tcode)   _TEST_RING_CONSUME1(_tcode)
#define _TEST_RING_CONSUME_SWAP(_tcode)  _TEST_RING_CONSUME1(_tcode)
#define _TEST_RING_CONSUME_FCAS(_tcode)  _TEST_RING_CONSUME2(_tcode,FCAS)
#define _TEST_RING_CONSUME_ADD(_tcode)   _TEST_RING_CONSUME2(_tcode,FADD)
#define _TEST_RING_CONSUME_XOR(_tcode)   _TEST_RING_CONSUME2(_tcode,FXOR)
//
#define TEST_RING_DECL(_tcode) \
       _TEST_RING_DECL(_tcode,SET) \
       _TEST_RING_DECL(_tcode,SWAP) \
       _TEST_RING_DECL(_tcode,FCAS) \
       _TEST_RING_DECL(_tcode,ADD) \
       _TEST_RING_DECL(_tcode,XOR)
FORALL_DT(TEST_RING_DECL)


GASNETT_NEVER_INLINE(doit,
void doit(gex_TM_t testtm, gex_DT_t dt)) {
  gex_OP_t all_ops =
        GEX_OP_ADD  | GEX_OP_SUB  | GEX_OP_MULT  |
        GEX_OP_MIN  | GEX_OP_MAX  |
        GEX_OP_INC  | GEX_OP_DEC  |
        GEX_OP_FADD | GEX_OP_FSUB | GEX_OP_FMULT |
        GEX_OP_FMIN | GEX_OP_FMAX |
        GEX_OP_FINC | GEX_OP_FDEC |
        GEX_OP_SET  | GEX_OP_GET  |
        GEX_OP_SWAP | GEX_OP_FCAS | GEX_OP_CAS;
  if ((dt != GEX_DT_FLT) && (dt != GEX_DT_DBL)) {
    all_ops |=
        GEX_OP_AND  | GEX_OP_OR  | GEX_OP_XOR |
        GEX_OP_FAND | GEX_OP_FOR | GEX_OP_FXOR;
  }

  #define MSG_CASE(dtcode) \
    case GEX_DT_##dtcode: MSG0("  Tests for type " _STRINGIFY(dtcode##_type)); break;
  switch (dt) { FORALL_DT(MSG_CASE) }

  // Test of AD-specific flags
  {
    gex_AD_t ad;
    gex_AD_Create(&ad, testtm, dt, GEX_OP_SET | GEX_OP_FADD, 0);

    BARRIER();

    #define FLAGS_CASE(dtcode) \
      case GEX_DT_##dtcode: test_flags_##dtcode(ad); break;
    switch (dt) { FORALL_DT(FLAGS_CASE) }

    gex_AD_Destroy(ad);
  }

  // Max values for "ring", "cntr", "fcas" tests
  // max_int: Must be an 'int' which can be represented exactly in the tested datatype
  // max_u64: Must be a 'uint64_t' which can be represented exactly in the tested datatype
  uint64_t max_u64 = 0;
  int max_int;
  switch (dt) {
    case GEX_DT_U32:
      max_u64 = ((uint32_t)-1);
      break;
    case GEX_DT_I32:
      max_u64 = ((uint32_t)-1) >> 1;
      break;
    case GEX_DT_U64:
      max_u64 = ((uint64_t)-1);
      break;
    case GEX_DT_I64:
      max_u64 = ((uint64_t)-1) >> 1;
      break;
    case GEX_DT_FLT:
      max_u64 = (uint64_t)1 << (FLT_MANT_DIG-1);
      break;
    case GEX_DT_DBL:
      max_u64 = (uint64_t)1 << (DBL_MANT_DIG-1);
      break;
  }
  max_int = (int) MIN((uint64_t)INT_MAX, max_u64);

  // Tests of ACQ/REL signaling on a ring, using several different ops to signal
  // Only run per-neighborhood rings on team0, and when there are multiple neighborhoods
  const int nbrhd = (testtm == myteam) && multinbrhd;
  #define RING_TEST(op1,op2,bitwise) \
  if (!bitwise || (dt!=GEX_DT_FLT && dt!=GEX_DT_DBL)) {  \
    gex_AD_t ad;                                         \
    gex_AD_Create(&ad, testtm, dt,                       \
                  (GEX_OP_SET   | GEX_OP_GET |           \
                   GEX_OP_##op1 | GEX_OP_##op2), 0);     \
    BARRIER();                                           \
    switch (dt) { FORALL_DT(RING_##op1##_CASE) }         \
    gex_AD_Destroy(ad);                                  \
  }
  #define RING_SET_CASE(dtcode)   case GEX_DT_##dtcode: test_ring_SET_##dtcode(ad, max_u64, nbrhd); break;
  RING_TEST(SET,SET,0)
  #define RING_SWAP_CASE(dtcode)  case GEX_DT_##dtcode: test_ring_SWAP_##dtcode(ad, max_u64, nbrhd); break;
  RING_TEST(SWAP,SWAP,0)
  #define RING_FCAS_CASE(dtcode) case GEX_DT_##dtcode: test_ring_FCAS_##dtcode(ad, max_u64, nbrhd); break;
  RING_TEST(FCAS,FCAS,0)
  #define RING_ADD_CASE(dtcode)   case GEX_DT_##dtcode: test_ring_ADD_##dtcode(ad, max_u64, nbrhd); break;
  RING_TEST(ADD,FADD,0)
  #define RING_XOR_CASE(dtcode)   case GEX_DT_##dtcode: test_ring_XOR_##dtcode(ad, max_u64, nbrhd); break;
  RING_TEST(XOR,FXOR,1)

  // Test of contended (F)ADD/(F)INC (central counter)
  {
    gex_AD_t ad;
    gex_AD_Create(&ad, testtm, dt, GEX_OP_SET | GEX_OP_GET  |
                                   GEX_OP_ADD | GEX_OP_FADD |
                                   GEX_OP_INC | GEX_OP_FINC, 0);

    BARRIER();

    #define CNTR_CASE(dtcode)   \
      case GEX_DT_##dtcode: test_cntr_##dtcode(ad, max_int); break;
    switch (dt) { FORALL_DT(CNTR_CASE) }

    gex_AD_Destroy(ad);
  }

  // Test of contended FCAS (central counter)
  {
    gex_AD_t ad;
    gex_AD_Create(&ad, testtm, dt, GEX_OP_SET | GEX_OP_GET | GEX_OP_FCAS, 0);

    BARRIER();

    #define FCAS_CASE(dtcode)   \
      case GEX_DT_##dtcode: test_fcas_##dtcode(ad,max_int); break;
    switch (dt) { FORALL_DT(FCAS_CASE) }

    gex_AD_Destroy(ad);
  }

  // Range of random numbers
  // Chosen to exercise as many bits of each type as possible within constraints:
  // + lo and hi must be int64_t (valid for use with TEST_RAND)
  // + For x in [lo,hi] 2*x*x must be representable exactly in the target type
  //   (because 2*x*x is the max value reached by the series of arithmetic ops)
  int64_t lo = 0;
  int64_t hi = 0;
  test_static_assert(FLT_RADIX == 2);

  // check that 2*hi*hi does not overflow, but same for hi+1 does
  #define CHECK_LIMIT(type,hi) do {           \
      assert( (type)(2*hi*hi) > hi );         \
      assert( (type)(2*(hi+1)*(hi+1)) < hi ); \
    } while (0)

  switch (dt) {
    case GEX_DT_U32:
      hi = 0xB504; // floor(sqrt((2^32-1)/2))
      lo = 0;
      CHECK_LIMIT(uint32_t,hi);
      break;
    case GEX_DT_I32:
      hi = (1<<15) - 1;
      lo = -hi;
      CHECK_LIMIT(int32_t,hi);
      break;
    case GEX_DT_U64:
      hi = 0xB504F333ull; // floor(sqrt((2^64-1)/2))
      lo = 0;
      CHECK_LIMIT(uint64_t,hi);
      break;
    case GEX_DT_I64:
      hi = (1U<<31) - 1;
      lo = -hi;
      CHECK_LIMIT(int64_t,hi);
      break;
    case GEX_DT_FLT:
      hi = 1 << (FLT_MANT_DIG / 2);
      lo = -hi;
      break;
    case GEX_DT_DBL:
      hi = 1 << (DBL_MANT_DIG / 2);
      lo = -hi;
      break;
  }

  // TODO: deterministic tests of bitwise ops (see testtools for examples)
  // TODO: deterministic tests of known corner cases (if/when specified)
  //         Integer overflow behaviors
  //         Floating-point +/- zero behavior
  // TODO: randomized tests of important subsets
  //         SET/GET/FCAS (universal)
  //         SET/GET/FCAS/SWAP (MCS locks and Nemesis queues)
  //         SET/GET/FADD
  // TODO: concurrent tests of correctness of more ops

  // Randomized testing
  for (int subtest = 0; subtest < 2; ++subtest) {
    gex_OP_t ops = 0;
    const char *msg = "";
    switch (subtest) {
      case 0:  // Test all ops legal for the data type
        ops = all_ops;
        break;

      case 1: // Whitebox testing of conduits with native implementations
        msg = " (conduit-specialized ops)";
        ops = all_ops;
        // This logic reduces "ops" to the conduit's specialized subset
      #if GASNET_CONDUIT_ARIES
        // 1) No MULT for any type
        ops &= ~(GEX_OP_MULT | GEX_OP_FMULT);
        // 2) No MIN or MAX for the unsigned integer types
        if ((dt == GEX_DT_U32) || (dt == GEX_DT_U64)) {
          ops &= ~(GEX_OP_MIN | GEX_OP_FMIN | GEX_OP_MAX | GEX_OP_FMAX);
        }
        // 3) No ADD (or its derivatives) for double
        if (dt == GEX_DT_DBL) {
          ops &= ~(GEX_OP_ADD | GEX_OP_FADD | GEX_OP_SUB | GEX_OP_FSUB |
                   GEX_OP_INC | GEX_OP_FINC | GEX_OP_DEC | GEX_OP_FDEC);
        }
      #elif GASNET_CONDUIT_UCX
        // 1) No FP types
        if ((dt == GEX_DT_FLT) || (dt == GEX_DT_DBL)) continue;
        // 2) No MULT, MIN or MAX
        ops &= ~(GEX_OP_MULT | GEX_OP_FMULT |
                 GEX_OP_MIN  | GEX_OP_FMIN  |
                 GEX_OP_MAX  | GEX_OP_FMAX);
      #else
        continue;  // No whitebox testing
      #endif
    }

    gex_AD_t ad;
    gex_AD_Create(&ad, testtm, dt, ops, 0);

    BARRIER();

    #define RAND_CASE(dtcode) \
      case GEX_DT_##dtcode: test_rand_##dtcode(ad,lo,hi,msg); break;
    switch (dt) { FORALL_DT(RAND_CASE) }

    BARRIER();

    gex_AD_Destroy(ad);
  }
}

void doall(gex_TM_t tm) {
  myrank   = gex_TM_QueryRank(tm);
  numranks = gex_TM_QuerySize(tm);
  peer = (myrank + 1) % numranks;
  peerseg = TEST_SEG_TM(tm, peer);

  if (tm == myteam) {
    neighbor = nbrhdinfo[(nbrhdrank + 1) % nbrhdsize].gex_jobrank;
  } else {
    // Search for a neighbor in the team (eventually finding self if no others).
    for (gex_Rank_t r = 1; r <= nbrhdsize; ++r) {
      neighbor = gex_TM_TranslateJobrankToRank(tm, nbrhdinfo[(nbrhdrank + r) % nbrhdsize].gex_jobrank);
      if (neighbor != GEX_RANK_INVALID) break;
    }
    gasneti_assert(neighbor != GEX_RANK_INVALID);
  }

  doit(tm, GEX_DT_U32);
  doit(tm, GEX_DT_I32);
  doit(tm, GEX_DT_U64);
  doit(tm, GEX_DT_I64);
  doit(tm, GEX_DT_FLT);
  doit(tm, GEX_DT_DBL);
}

/* ------------------------------------------------------------------------------------ */

int main(int argc, char **argv) {
  GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, "testratomic", &argc, &argv, 0));

  int arg = 1;
  if (argc > arg) { iters = atoi(argv[arg]); ++arg; }
  if (!iters) iters = 1000;

  unsigned int seedoffset = 0;
  if (argc > arg) { seedoffset = atoi(argv[arg]); ++arg; }

  gex_System_QueryNbrhdInfo(&nbrhdinfo, &nbrhdsize, &nbrhdrank);
  multinbrhd = (nbrhdsize != gex_TM_QuerySize(myteam));
  mynbrhd = (int)nbrhdinfo[0].gex_jobrank;

  // need scratch sizes before Attatch
  gex_TM_t subtm = GEX_TM_INVALID;
  int color = gex_TM_QueryRank(myteam) & 1; // odds & evens
  scratch_sz1 = gex_TM_Split(&subtm, myteam, color, 0, 0, 0, GEX_FLAG_TM_SCRATCH_SIZE_MIN);
  scratch_sz2 = gex_TM_Split(&subtm, myteam, mynbrhd, 0, 0, 0, GEX_FLAG_TM_SCRATCH_SIZE_MIN);

  GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ_REQUEST));

  test_init("testratomic",0,"(iters) (seed0)");
  TEST_SET_WAITMODE(1);

  gex_Rank_t self = gex_TM_QueryRank(myteam);
  if (seedoffset == 0) {
    seedoffset = (((unsigned int)TIME()) & 0xFFFF);
    TEST_BCAST(&seedoffset, 0, &seedoffset, sizeof(&seedoffset));
  }
  TEST_SRAND(self+seedoffset);

  MSG("Running %i iterations of remote atomics tests (seed = %u).", iters, self+seedoffset);

  BARRIER();
  MSG0("Running tests using all ranks");
  doall(myteam);

  // TODO: use gex_TM_Destroy(subtm) and do away with scratch_offset
  uintptr_t scratch_offset = PAGESZ;

  if (gex_TM_QuerySize(myteam) > 1) {
    BARRIER();
    MSG0("Running tests using odd/even subteams");
    gex_TM_Split(&subtm, myteam, color, 0, (void*)(scratch_offset + (uintptr_t)TEST_MYSEG()), scratch_sz1, 0);
    scratch_offset + scratch_sz1;
    assert_always(subtm != GEX_TM_INVALID);
    doall(subtm);
  }

  if (multinbrhd) {
    BARRIER();
    MSG0("Running tests using nbrhd subteams");
    gex_TM_Split(&subtm, myteam, mynbrhd, 0, (void*)(scratch_offset + (uintptr_t)TEST_MYSEG()), scratch_sz2, 0);
    scratch_offset + scratch_sz2;
    assert_always(subtm != GEX_TM_INVALID);
    doall(subtm);
  }

  int32_t errs = test_errs;
  gex_Event_Wait(
    gex_Coll_ReduceToAllNB(myteam, &errs, &errs,
                           GEX_DT_I32, sizeof(errs), 1,
                           GEX_OP_ADD, NULL, NULL, 0));
  MSG0("done. (detected %i errors)", (int)errs);

  BARRIER();

  gasnet_exit(!!errs);
  return !!errs;
}
