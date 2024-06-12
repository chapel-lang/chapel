/* $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testtoken.c $
 * Copyright (c) 2023, The Regents of the University of California
 *
 * Description: Tests of gex_Token_Info()
 */

#include <gasnetex.h>
#include <gasnet_coll.h>

#include "string.h"

#ifndef TEST_SEGSZ
#define TEST_SEGSZ PAGESZ
#endif

#include <test.h>

//  ------------------------------------------------------------------------------------

static gex_Client_t  myclient;
static gex_EP_t      myep;
static gex_TM_t      myteam;
static gex_Segment_t mysegment;
static gex_Rank_t    myrank;

static gex_Rank_t    prev_rank,  next_rank;
static void         *prev_base, *next_base;

//  ------------------------------------------------------------------------------------

// Even though srcrank is "known", we send it one way to ensure gex_nargs is non-constant
static void SReq(gex_Token_t token, gex_AM_Arg_t srcrank);
static void SRep(gex_Token_t token);
static void MReq(gex_Token_t token, void *buf, size_t nbytes, gex_AM_Arg_t srcrank);
static void MRep(gex_Token_t token, void *buf, size_t nbytes);
static void LReq(gex_Token_t token, void *buf, size_t nbytes, gex_AM_Arg_t srcrank);
static void LRep(gex_Token_t token, void *buf, size_t nbytes);

enum { am_short = 0, am_medium, am_long };
const gex_Flags_t cflags[] = { GEX_FLAG_AM_SHORT, GEX_FLAG_AM_MEDIUM, GEX_FLAG_AM_LONG };
const int mycdata[6] = { 42, 511, 911, 0, -1, 777 };

#define hidx_SReq  (GEX_AM_INDEX_BASE + 2*am_short  + 0)
#define hidx_SRep  (GEX_AM_INDEX_BASE + 2*am_short  + 1)
#define hidx_MReq  (GEX_AM_INDEX_BASE + 2*am_medium + 0)
#define hidx_MRep  (GEX_AM_INDEX_BASE + 2*am_medium + 1)
#define hidx_LReq  (GEX_AM_INDEX_BASE + 2*am_long   + 0)
#define hidx_LRep  (GEX_AM_INDEX_BASE + 2*am_long   + 1)

gex_AM_Entry_t htable[] = {
  { hidx_SReq, SReq, GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_SHORT,  1, &mycdata[0], "Alpha" },
  { hidx_SRep, SRep, GEX_FLAG_AM_REPLY  |GEX_FLAG_AM_SHORT,  0, &mycdata[1], "Bravo" },
  { hidx_MReq, MReq, GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_MEDIUM, 1, &mycdata[2], "Charlie" },
  { hidx_MRep, MRep, GEX_FLAG_AM_REPLY  |GEX_FLAG_AM_MEDIUM, 0, &mycdata[3], "Delta" },
  { hidx_LReq, LReq, GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_LONG,   1, &mycdata[4], "Echo" },
  { hidx_LRep, LRep, GEX_FLAG_AM_REPLY  |GEX_FLAG_AM_LONG,   0, &mycdata[5], "Foxtrot" }
};
#define HANDLER_TABLE_SIZE (sizeof(htable)/sizeof(gex_AM_Entry_t))

//  ------------------------------------------------------------------------------------

gex_TI_t supported = GEX_TI_ALL; // will remove bits for any queries which fail
gasnett_atomic_t error_cnt = gasnett_atomic_init(0);
gasnett_atomic_t reply_cnt = gasnett_atomic_init(0);

static void common(gex_Token_t token, gex_Rank_t srcrank, int is_req, int category, gex_AM_Fn_t fnptr)
{
  gex_Token_Info_t info;
  gex_TI_t mask = gex_Token_Info(token, &info, GEX_TI_ALL);

  static gex_HSL_t lock = GEX_HSL_INITIALIZER;
  gex_HSL_Lock(&lock);
    supported &= mask;
  gex_HSL_Unlock(&lock);

  #define CHECK(cond) do { \
    if (!(cond)) { \
      fprintf(stderr, "Check failed: " #cond "\n"); \
      gasnett_atomic_increment(&error_cnt, 0); \
    } \
  } while (0)

  CHECK(mask & GEX_TI_SRCRANK);
  CHECK(info.gex_srcrank == srcrank);

  CHECK(mask & GEX_TI_EP);
  CHECK(info.gex_ep == myep);

#if (GASNET_SUPPORTS_TI_IS_REQ == 1)
  CHECK(mask & GEX_TI_IS_REQ);
#elif defined(GASNET_SUPPORTS_TI_IS_REQ)
  #error GASNET_SUPPORTS_TI_IS_REQ is neither '1' nor undefined
#endif
  if (mask & GEX_TI_IS_REQ)  CHECK(info.gex_is_req  == is_req);

#if (GASNET_SUPPORTS_TI_IS_LONG == 1)
  CHECK(mask & GEX_TI_IS_LONG);
#elif defined(GASNET_SUPPORTS_TI_IS_LONG)
  #error GASNET_SUPPORTS_TI_IS_LONG is neither '1' nor undefined
#endif
  if (mask & GEX_TI_IS_LONG) CHECK(info.gex_is_long == (category == am_long));

#if (GASNET_SUPPORTS_TI_ENTRY == 1)
  CHECK(mask & GEX_TI_ENTRY);
#elif defined(GASNET_SUPPORTS_TI_ENTRY)
  #error GASNET_SUPPORTS_TI_ENTRY is neither '1' nor undefined
#endif
  if (mask & GEX_TI_ENTRY) {
    unsigned int myindex = 2*category + !is_req;
    gex_Flags_t flags = (is_req ? GEX_FLAG_AM_REQUEST : GEX_FLAG_AM_REPLY)
                      | cflags[category];
    CHECK(info.gex_entry->gex_index == GEX_AM_INDEX_BASE + myindex);
    CHECK(info.gex_entry->gex_fnptr == fnptr);
    CHECK(info.gex_entry->gex_flags == flags);
    CHECK(info.gex_entry->gex_nargs == is_req);
    CHECK(info.gex_entry->gex_cdata == mycdata + myindex);
    CHECK(info.gex_entry->gex_name  == htable[myindex].gex_name); // pointer equality, not strcmp()
  }

  #undef CHECK
}

static void SReq(gex_Token_t token, gex_AM_Arg_t srcrank) {
  common(token, srcrank, 1, am_short, &SReq);
  gex_AM_ReplyShort0(token, hidx_SRep, 0);
}

static void SRep(gex_Token_t token) {
  common(token, next_rank, 0, am_short, &SRep);
  gasnett_atomic_increment(&reply_cnt, 0);
}

static void MReq(gex_Token_t token, void *buf, size_t nbytes, gex_AM_Arg_t srcrank) {
  common(token, srcrank, 1, am_medium, &MReq);
  gex_AM_ReplyMedium0(token, hidx_MRep, NULL, 0, GEX_EVENT_NOW, 0);
}

static void MRep(gex_Token_t token, void *buf, size_t nbytes) {
  common(token, next_rank, 0, am_medium, &MRep);
  gasnett_atomic_increment(&reply_cnt, 0);
}

static void LReq(gex_Token_t token, void *buf, size_t nbytes, gex_AM_Arg_t srcrank) {
  common(token, srcrank, 1, am_long, &LReq);
  gex_AM_ReplyLong0(token, hidx_LRep, NULL, 0, prev_base, GEX_EVENT_NOW, 0);
}

static void LRep(gex_Token_t token, void *buf, size_t nbytes) {
  common(token, next_rank, 0, am_long, &LRep);
  gasnett_atomic_increment(&reply_cnt, 0);
}

//  ------------------------------------------------------------------------------------

int main(int argc, char **argv)
{
  GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, "testtoken", &argc, &argv, 0));

  int help = (argc != 1);

  test_init("testtoken", 0, "");

  gex_Rank_t nranks = gex_TM_QuerySize(myteam);
  myrank = gex_TM_QueryRank(myteam);
  next_rank = (myrank + 1) % nranks;
  prev_rank = (myrank + nranks - 1) % nranks;

  GASNET_Safe(gex_EP_RegisterHandlers(myep, htable, sizeof(htable)/sizeof(gex_AM_Entry_t)));

  GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ_REQUEST));
  gex_Event_Wait(gex_EP_QueryBoundSegmentNB(myteam, next_rank, &next_base, NULL, NULL, 0));
  gex_Event_Wait(gex_EP_QueryBoundSegmentNB(myteam, prev_rank, &prev_base, NULL, NULL, 0));

  gex_Event_Wait(gex_Coll_BarrierNB(myteam,0));

  gex_AM_RequestShort1 (myteam, next_rank, hidx_SReq,                                    0, myrank);
  gex_AM_RequestMedium1(myteam, next_rank, hidx_MReq, NULL, 0,            GEX_EVENT_NOW, 0, myrank);
  gex_AM_RequestLong1  (myteam, next_rank, hidx_LReq, NULL, 0, next_base, GEX_EVENT_NOW, 0, myrank);

  GASNET_BLOCKUNTIL(3 == gasnett_atomic_read(&reply_cnt, 0));

  {
    gex_Rank_t num_nbrhd;
    gex_System_QueryMyPosition(&num_nbrhd, NULL, NULL, NULL);
    if (num_nbrhd == 1) {
      MSG0("WARNING: unable to test optional query support due to having only shared-memory peers");
    } else {
      // Since PSHM (currently) implements all optional queries,
      // we use a reduction to look for *any* unsupported cases.
      uint64_t tmp = supported; // overkill rather than matching width of gex_TI_t
      gex_Event_Wait(gex_Coll_ReduceToOneNB(myteam, 0, &tmp, &tmp,
                                            GEX_DT_U64, sizeof(tmp), 1,
                                            GEX_OP_AND, NULL, NULL, 0));
      if (!myrank) {
        #define CHECKDEF(query) do { \
          if (0 == (supported & query)) { \
            MSG0("ERROR: Optional " #query " query is not supported, but feature macro is defined"); \
            gasnett_atomic_increment(&error_cnt, 0); \
          } \
        } while (0)
        #define CHECKUNDEF(query) do { \
          if (supported & query) { \
            MSG0("WARNING: Optional " #query " query appears to be supported, but feature macro is undefined"); \
          } else { \
            MSG0("INFO: Optional " #query " query is not supported"); \
          } \
        } while (0)

      #ifdef GASNET_SUPPORTS_TI_ENTRY
        CHECKDEF(GEX_TI_ENTRY);
      #else
        CHECKUNDEF(GEX_TI_ENTRY);
      #endif

      #ifdef GASNET_SUPPORTS_TI_IS_REQ
        CHECKDEF(GEX_TI_IS_REQ);
      #else
        CHECKUNDEF(GEX_TI_IS_REQ);
      #endif

      #ifdef GASNET_SUPPORTS_TI_IS_LONG
        CHECKDEF(GEX_TI_IS_LONG);
      #else
        CHECKUNDEF(GEX_TI_IS_LONG);
      #endif

        #undef CHECKDEF
        #undef CHECKUNDEF
      }
    }
  }

  int errors = (int)gasnett_atomic_read(&error_cnt,0);
  if (errors) {
     MSG("ERROR: %d errors detected", errors);
  }

  gex_Event_Wait(gex_Coll_BarrierNB(myteam,0));
  MSG0("done.");

  gasnet_exit(errors);
  return 0;
}
