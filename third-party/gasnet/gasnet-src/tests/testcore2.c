/* $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testcore2.c $
 * Copyright 2007, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 *
 * Description: GASNet Core checksum test
 * This stress tests the ability of the core to successfully send
 * AM Requests/Replies (fixed- and negotiated-payload) with correct data delivery
 * testing is run 'iters' times with Medium/Long payload sizes ranging from 1..'max_payload',
 *  with up to 'depth' AMs in-flight from a given node at any moment
 *
 */

#include <stddef.h> // for size_t

size_t max_payload = 0;
int depth = 0;
#ifndef TEST_SEGSZ
  #define TEST_SEGSZ_EXPR ((uintptr_t)max_payload*depth*5)
#endif

#include "test.h"

static gex_Client_t      myclient;
static gex_EP_t    myep;
static gex_TM_t myteam;
static gex_Segment_t     mysegment;

int myproc;
int peerproc;
int fromproc;
int numprocs;
int iters = 0;
size_t maxmed;
size_t maxlong;
size_t least_payload_req_med;
size_t least_payload_rep_med;
size_t least_payload_req_long;
size_t least_payload_rep_long;
size_t *all_sizes;
volatile int done = 0;
int allowretry = 1;
uint8_t *myseg;      /* my segment */
int doinseg = 1;
int dooutseg = 1;
#define INSEG(iter) ((doinseg&&dooutseg)?(iter&0x1):doinseg)
#define ITERSEG(iter) (INSEG(iter)?localseg:privateseg)
uint8_t *peerreqseg; /* long request landing zone */
uint8_t *peerrepseg; /* long reply landing zone */
uint8_t *localseg;
uint8_t *privateseg;
uint8_t *longreplysrc;

// Division of AM argument into two bit fields.
// Note that 8 would probably be more than sufficient for either.
#define CHUNK_BITS 16
#define SZ_BITS 16
test_static_assert_file((CHUNK_BITS + SZ_BITS) <= 32);

// Test three injection modes
#define INJMODE(iter) ((iter)%3)

GASNETT_THREADKEY_DECLARE(mythread);
GASNETT_THREADKEY_DEFINE(mythread);

#define ELEM_VALUE(iter,chunkidx,elemidx) \
        ((((uint8_t)(iter)&0x3) << 6) | (((uint8_t)(chunkidx)&0x3) << 4) | (((uint8_t)(elemidx))&0xF))

void init_chunk(uint8_t *buf, size_t sz, int iter, int chunkidx) {
  size_t elemidx;
  for (elemidx = 0; elemidx < sz; elemidx++) {
    buf[chunkidx*sz+elemidx] = ELEM_VALUE(iter,chunkidx,elemidx);
  }
}

void validate_chunk(const char *context, uint8_t *buf, size_t sz, int iter, int chunkidx) {
  size_t elemidx;
  int errcnt = 0;
  int doretry = 0;
retry:
  for (elemidx = 0; elemidx < sz; elemidx++) {
    uint8_t actual = buf[elemidx];
    uint8_t expected = ELEM_VALUE(iter,chunkidx,elemidx);
    if (actual != expected) {
      int id = (uintptr_t)gasnett_threadkey_get(mythread); 
      ERR("TH%i data mismatch at sz=%i iter=%i chunk=%i elem=%i : actual=%02x expected=%02x in %s",
           id, (int)sz,iter,chunkidx,(int)elemidx,
           (unsigned int)actual,(unsigned int)expected,
           context);
      errcnt++;
    }
  }
  if (errcnt && allowretry && !doretry) {
    doretry = 1; errcnt = 0;
    sleep(1);
    goto retry;
  } else if (doretry) {
    if (errcnt == 0) MSG("retry DID clear errors");
    else MSG("retry DID NOT clear errors");
  }
}

size_t MIN4(size_t a, size_t b, size_t c, size_t d) {
  size_t x1 = MIN(a,b);
  size_t x2 = MIN(c,d);
  return MIN(x1,x2);
}

/* Test handlers */
#define hidx_ping_medhandler     203
#define hidx_pong_medhandler     204

#define hidx_ping_longhandler    205
#define hidx_pong_longhandler    206

gasnett_atomic_t pong_recvd;

#define INIT_CHECKS(is_req) do {                               \
    assert_always(test_msgsource(token) == (is_req ? fromproc     \
                                                   : peerproc));  \
    assert_always(iter < iters);                                  \
    assert_always(nbytes <= max_payload);                         \
  } while (0)


void ping_medhandler(gex_Token_t token, void *buf, size_t nbytes, 
                     gex_AM_Arg_t iter, gex_AM_Arg_t arg1) {
  INIT_CHECKS(1);
  int chunkidx = arg1 & ((1<<CHUNK_BITS)-1);
  validate_chunk("Medium Request (pre-reply)", buf, nbytes, iter, chunkidx);
  gex_AM_SrcDesc_t sd;
  int imm = 0;
  gex_Flags_t flags = TEST_RAND_ONEIN(5) ? GEX_FLAG_IMMEDIATE : 0;
  size_t most_payload = TEST_RAND(nbytes, 2*nbytes);
  size_t least_payload = TEST_RAND(nbytes - nbytes/2, MIN(most_payload, least_payload_rep_med));
  size_t len = TEST_RAND(nbytes - nbytes/2, nbytes);
retry:
  switch (INJMODE(iter)) { // [0..2]
    case 0: // Fixed-payload
      imm = gex_AM_ReplyMedium2(token, hidx_pong_medhandler, buf, len, GEX_EVENT_NOW, flags, iter, arg1);
      break;

    case 1: // Negotiated-payload with client-provided buffer
      // TODO: (lc_opt = &event) is legal, but we lack logic to test/wait outside handler context
      //       additionally, we could not safely send buf with async LC
      sd = gex_AM_PrepareReplyMedium(token, buf, least_payload, most_payload, GEX_EVENT_NOW, flags, 2);
      imm = (sd == GEX_AM_SRCDESC_NO_OP); // IMMEDIATE was NO OP
      if (imm) break;
      assert(gex_AM_SrcDescSize(sd) >= least_payload);
      assert(gex_AM_SrcDescSize(sd) <= most_payload);
      assert(gex_AM_SrcDescAddr(sd) == buf);
      len = MIN(len, gex_AM_SrcDescSize(sd));
      gex_AM_CommitReplyMedium2(sd, hidx_pong_medhandler, len, iter, arg1);
      break;

    case 2: // Negotiated-payload without client-provided buffer
      sd = gex_AM_PrepareReplyMedium(token, NULL, least_payload, most_payload, NULL, flags, 2);
      imm = (sd == GEX_AM_SRCDESC_NO_OP); // IMMEDIATE was NO OP
      if (imm) break;
      assert(gex_AM_SrcDescSize(sd) >= least_payload);
      assert(gex_AM_SrcDescSize(sd) <= most_payload);
      len = MIN(len, gex_AM_SrcDescSize(sd));
      memcpy(gex_AM_SrcDescAddr(sd), buf, len);
      gex_AM_CommitReplyMedium2(sd, hidx_pong_medhandler, len, iter, arg1);
      break;
  }
  if (imm) {
    assert(flags & GEX_FLAG_IMMEDIATE);
    flags &= ~GEX_FLAG_IMMEDIATE;
    goto retry;
  }
  validate_chunk("Medium Request (post-reply)", buf, nbytes, iter, arg1);
}

void pong_medhandler(gex_Token_t token, void *buf, size_t nbytes,
                     gex_AM_Arg_t iter, gex_AM_Arg_t arg1) {
  INIT_CHECKS(0);
  int chunkidx = arg1 & ((1<<CHUNK_BITS)-1);
  validate_chunk("Medium Reply", buf, nbytes, iter, chunkidx);
  gasnett_atomic_increment(&pong_recvd,0);
}

void ping_longhandler(gex_Token_t token, void *buf, size_t nbytes,
                     gex_AM_Arg_t iter, gex_AM_Arg_t arg1) {
  INIT_CHECKS(1);
  int chunkidx = arg1 & ((1<<CHUNK_BITS)-1);
  size_t curr_sz = all_sizes[arg1 >> CHUNK_BITS];
  validate_chunk("Long Request", buf, nbytes, iter, chunkidx);
  gex_AM_SrcDesc_t sd;
  uint8_t *srcbuf = INSEG(iter) ? buf : longreplysrc+chunkidx*curr_sz;
  uint8_t *dstbuf = peerrepseg+chunkidx*curr_sz;
  int imm = 0;
  gex_Flags_t flags = TEST_RAND_ONEIN(5) ? GEX_FLAG_IMMEDIATE : 0;
  void * maybe_dest = TEST_RAND_ONEIN(2) ? dstbuf : NULL; // Passing dest_addr to Prepare is optional
  size_t most_payload = TEST_RAND(nbytes, 2*nbytes);
  size_t least_payload = TEST_RAND(nbytes, MIN(most_payload, least_payload_rep_long));
  size_t len = TEST_RAND(nbytes - nbytes/2, nbytes);
retry:
  switch (INJMODE(iter)) { // [0..2]
    case 0: // Fixed-payload
      if (srcbuf != buf) memcpy(srcbuf, buf, len);
      imm = gex_AM_ReplyLong2(token, hidx_pong_longhandler, srcbuf, len, dstbuf, GEX_EVENT_NOW, flags, iter, arg1);
      break;

    case 1: // Negotiated-payload with client-provided buffer
      // TODO: (lc_opt = &event) is legal, but we lack logic to test/wait outside handler context
      //       additionally, we could not safely send buf with async LC
      sd = gex_AM_PrepareReplyLong(token, srcbuf, least_payload, most_payload, maybe_dest, GEX_EVENT_NOW, flags, 2);
      imm = (sd == GEX_AM_SRCDESC_NO_OP); // IMMEDIATE was NO OP
      if (imm) break;
      assert(gex_AM_SrcDescSize(sd) >= least_payload);
      assert(gex_AM_SrcDescSize(sd) <= most_payload);
      assert(gex_AM_SrcDescAddr(sd) == srcbuf);
      len = MIN(len, gex_AM_SrcDescSize(sd));
      if (srcbuf != buf) memcpy(srcbuf, buf, len); // according to INSEG - not due to Prepare
      gex_AM_CommitReplyLong2(sd, hidx_pong_longhandler, len, dstbuf, iter, arg1);
      break;

    case 2: // Negotiated-payload without client-provided buffer
      sd = gex_AM_PrepareReplyLong(token, NULL, least_payload, most_payload, maybe_dest, NULL, flags, 2);
      imm = (sd == GEX_AM_SRCDESC_NO_OP); // IMMEDIATE was NO OP
      if (imm) break;
      assert(gex_AM_SrcDescSize(sd) >= least_payload);
      assert(gex_AM_SrcDescSize(sd) <= most_payload);
      len = MIN(len, gex_AM_SrcDescSize(sd));
      memcpy(gex_AM_SrcDescAddr(sd), buf, len);
      gex_AM_CommitReplyLong2(sd, hidx_pong_longhandler, len, dstbuf, iter, arg1);
      break;
  }
  if (imm) {
    assert(flags & GEX_FLAG_IMMEDIATE);
    flags &= ~GEX_FLAG_IMMEDIATE;
    goto retry;
  }
}

void pong_longhandler(gex_Token_t token, void *buf, size_t nbytes,
                     gex_AM_Arg_t iter, gex_AM_Arg_t arg1) {
  INIT_CHECKS(0);
  int chunkidx = arg1 & ((1<<CHUNK_BITS)-1);
  validate_chunk("Long Reply", buf, nbytes, iter, chunkidx);
  gasnett_atomic_increment(&pong_recvd,0);
}


void *doit(void *id);

int doprime = 0;
int dosizesync = 1;
int domultith = 1;
int domed = 1;
int dolong = 1;
int amopt = 0;

int main(int argc, char **argv) {
  int arg = 1, help = 0;
  gex_AM_Entry_t htable[] = {
    { hidx_ping_medhandler,  ping_medhandler,  GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_MEDIUM, 2 },
    { hidx_pong_medhandler,  pong_medhandler,  GEX_FLAG_AM_REPLY|GEX_FLAG_AM_MEDIUM, 2 },
    { hidx_ping_longhandler, ping_longhandler, GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_LONG, 2 },
    { hidx_pong_longhandler, pong_longhandler, GEX_FLAG_AM_REPLY|GEX_FLAG_AM_LONG, 2 },
  };

  /* call startup */
  GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, "testcore2", &argc, &argv, 0));

  #define AMOPT() if (!amopt) { amopt = 1; domed = 0; dolong = 0; }
  while (argc > arg) {
    if (!strcmp(argv[arg], "-p")) {
      doprime = 1;
      ++arg;
    } else if (!strcmp(argv[arg], "-u")) {
      dosizesync = 0;
      ++arg;
    } else if (!strcmp(argv[arg], "-s")) {
      domultith = 0;
      ++arg;
    } else if (!strcmp(argv[arg], "-n")) {
      allowretry = 0;
      ++arg;
    } else if (!strcmp(argv[arg], "-in")) {
      doinseg = 1; dooutseg = 0;
      ++arg;
    } else if (!strcmp(argv[arg], "-out")) {
      doinseg = 0; dooutseg = 1;
      ++arg;
    } else if (!strcmp(argv[arg], "-m")) {
      AMOPT();
      domed = 1; 
      ++arg;
    } else if (!strcmp(argv[arg], "-l")) {
      AMOPT();
      dolong = 1;
      ++arg;
    } else if (argv[arg][0] == '-') {
      help = 1;
      ++arg;
    } else break;
  }

  if (argc > arg) { iters = atoi(argv[arg]); arg++; }
  if (!iters) iters = 30;
  if (argc > arg) { max_payload = atoi(argv[arg]); arg++; }
  if (!max_payload) max_payload = 1024*1024;
  if (argc > arg) { depth = atoi(argv[arg]); arg++; }
  if (!depth) depth = 16;
  depth = MIN(depth, (1<<CHUNK_BITS)-1);

  /* round down to largest payload AM allows with 2 arguments */
  maxmed  = MIN4(gex_AM_MaxRequestMedium(myteam,GEX_RANK_INVALID,GEX_EVENT_NOW,0,2),
                 gex_AM_MaxRequestMedium(myteam,GEX_RANK_INVALID,GEX_EVENT_NOW,GEX_FLAG_IMMEDIATE,2),
                 gex_AM_MaxReplyMedium  (myteam,GEX_RANK_INVALID,GEX_EVENT_NOW,0,2),
                 gex_AM_MaxReplyMedium  (myteam,GEX_RANK_INVALID,GEX_EVENT_NOW,GEX_FLAG_IMMEDIATE,2));
  maxlong = MIN4(gex_AM_MaxRequestLong  (myteam,GEX_RANK_INVALID,GEX_EVENT_NOW,0,2),
                 gex_AM_MaxRequestLong  (myteam,GEX_RANK_INVALID,GEX_EVENT_NOW,GEX_FLAG_IMMEDIATE,2),
                 gex_AM_MaxReplyLong    (myteam,GEX_RANK_INVALID,GEX_EVENT_NOW,0,2),
                 gex_AM_MaxReplyLong    (myteam,GEX_RANK_INVALID,GEX_EVENT_NOW,GEX_FLAG_IMMEDIATE,2));
  max_payload = MIN(max_payload,MAX(maxmed,maxlong));

  if (!domed)  max_payload = MIN(max_payload, maxlong);
  if (!dolong) max_payload = MIN(max_payload, maxmed);

  /* largest least_payload values */
  gex_Flags_t f1 = GEX_FLAG_AM_PREPARE_LEAST_CLIENT;
  gex_Flags_t f2 = GEX_FLAG_AM_PREPARE_LEAST_ALLOC;
  least_payload_req_med = MIN4(
                 gex_AM_MaxRequestMedium(myteam,GEX_RANK_INVALID,GEX_EVENT_NOW,f1,2),
                 gex_AM_MaxRequestMedium(myteam,GEX_RANK_INVALID,GEX_EVENT_NOW,f1|GEX_FLAG_IMMEDIATE,2),
                 gex_AM_MaxRequestMedium(myteam,GEX_RANK_INVALID,GEX_EVENT_NOW,f2,2),
                 gex_AM_MaxRequestMedium(myteam,GEX_RANK_INVALID,GEX_EVENT_NOW,f2|GEX_FLAG_IMMEDIATE,2));
  least_payload_rep_med = MIN4(
                 gex_AM_MaxReplyMedium  (myteam,GEX_RANK_INVALID,GEX_EVENT_NOW,f1,2),
                 gex_AM_MaxReplyMedium  (myteam,GEX_RANK_INVALID,GEX_EVENT_NOW,f1|GEX_FLAG_IMMEDIATE,2),
                 gex_AM_MaxReplyMedium  (myteam,GEX_RANK_INVALID,GEX_EVENT_NOW,f2,2),
                 gex_AM_MaxReplyMedium  (myteam,GEX_RANK_INVALID,GEX_EVENT_NOW,f2|GEX_FLAG_IMMEDIATE,2));
  least_payload_req_long = MIN4(
                 gex_AM_MaxRequestLong  (myteam,GEX_RANK_INVALID,GEX_EVENT_NOW,f1,2),
                 gex_AM_MaxRequestLong  (myteam,GEX_RANK_INVALID,GEX_EVENT_NOW,f1|GEX_FLAG_IMMEDIATE,2),
                 gex_AM_MaxRequestLong  (myteam,GEX_RANK_INVALID,GEX_EVENT_NOW,f2,2),
                 gex_AM_MaxRequestLong  (myteam,GEX_RANK_INVALID,GEX_EVENT_NOW,f2|GEX_FLAG_IMMEDIATE,2));
  least_payload_rep_long = MIN4(
                 gex_AM_MaxReplyLong    (myteam,GEX_RANK_INVALID,GEX_EVENT_NOW,f1,2),
                 gex_AM_MaxReplyLong    (myteam,GEX_RANK_INVALID,GEX_EVENT_NOW,f1|GEX_FLAG_IMMEDIATE,2),
                 gex_AM_MaxReplyLong    (myteam,GEX_RANK_INVALID,GEX_EVENT_NOW,f2,2),
                 gex_AM_MaxReplyLong    (myteam,GEX_RANK_INVALID,GEX_EVENT_NOW,f2|GEX_FLAG_IMMEDIATE,2));

  // TODO:
  // Once conduits begin returning values for the least_payload_* which are less than
  // the respective FP maximums, there are TEST_RAND() call that may end up with bad
  // (lo > hi) ranges.  For instance:
  //   least_payload = TEST_RAND(sz - sz/2, MIN(most_payload, least_payload_req_med))
  // could have lo=maxmed/2, which could be larger than hi=least_payload_req_med.

  GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ_REQUEST));
  GASNET_Safe(gex_EP_RegisterHandlers(myep, htable, sizeof(htable)/sizeof(gex_AM_Entry_t)));
  test_init("testcore2",0,"[options] (iters) (max_payload) (depth)\n"
                 "  -m   test AMMedium    (defaults to all types)\n"
                 "  -l   test AMLong      (defaults to all types)\n"
                 "  -p   prime the AMLong transfer areas with puts, to encourage pinning\n"
                 "  -u   loosen sychronization to allow diff payload sizes to be in flight at once\n"
                 "  -s   single-threaded PAR mode (default is to start a polling thread in PAR mode)\n"
                 "  -n   no retry on failure\n"
                 "  -in/-out use only in- or out-of-segment sources for AMLong (default is both)\n"
                 );
  if (help || argc > arg) test_usage();

  TEST_PRINT_CONDUITINFO();

  /* get SPMD info */
  myproc = gex_TM_QueryRank(myteam);
  numprocs = gex_TM_QuerySize(myteam);

  if (numprocs%2) {
    // w/ odd # of ranks, last one talks to self
    int last = numprocs - 1;
    if (myproc == last) {
      peerproc = fromproc = myproc;
    } else {
      peerproc = (myproc + 1) % last;
      fromproc = (myproc + last - 1) % last;
    }
  } else {
    peerproc = (myproc + 1) % numprocs;
    fromproc = (myproc + numprocs - 1) % numprocs;
  }

  myseg = TEST_MYSEG();
  peerreqseg = TEST_SEG(peerproc);
  peerrepseg = (uint8_t*)TEST_SEG(fromproc) + max_payload*depth*2;
  localseg = myseg + max_payload*depth*4;
  assert_always(TEST_SEGSZ >= max_payload*depth*5);
  privateseg = test_malloc(max_payload*depth*2); /* out-of-seg request src, long reply src */
  longreplysrc = privateseg+max_payload*depth;

  #ifdef GASNET_PAR
    if (domultith) test_createandjoin_pthreads(2,doit,NULL,0);
    else
  #endif
      doit(0);

  BARRIER();
  test_free(privateseg);
  MSG("done. (detected %i errs)", test_errs);
  gasnet_exit(test_errs > 0 ? 1 : 0);
  return 0;
}

void *doit(void *id) {
  gasnett_threadkey_set(mythread,id); 
  if ((uintptr_t)id != 0) { /* additional threads polling, to encourage handler concurrency */
    while (!done) {
      gasnet_AMPoll();
      gasnett_sched_yield();
    }
    return 0;
  } 

  MSG0("Running %sAM%s%s%s correctness test %s%swith %i iterations, max_payload=%" PRIu64 ", depth=%i...",
#if GASNET_PAR
    (domultith?"multi-threaded ":"single-threaded "),
#else
    "",
#endif
    (amopt?(domed?" Medium":""):""),(amopt?(dolong?" Long":""):""),
    ((doinseg^dooutseg)?(doinseg?" in-segment":" out-of-segment"):""),
    (dosizesync?"":"loosely-synced "),
    (doprime?"with priming ":""),
    iters,(uint64_t)max_payload,depth);

  BARRIER();
  if (doprime) { /* issue some initial puts that cover the Long regions, to try and trigger dynamic pinning */
    int chunkidx;
    for (chunkidx = 0; chunkidx < depth; chunkidx++) {
      /* AMRequestLong primer */
      gex_RMA_PutBlocking(myteam, peerproc, peerreqseg+chunkidx*max_payload, privateseg+chunkidx*max_payload, max_payload, 0);
      gex_RMA_PutBlocking(myteam, peerproc, peerreqseg+chunkidx*max_payload, localseg+chunkidx*max_payload, max_payload, 0);
      /* AMReplyLong primer */
      gex_RMA_PutBlocking(myteam, peerproc, peerrepseg+chunkidx*max_payload, myseg+chunkidx*max_payload, max_payload, 0);
      gex_RMA_PutBlocking(myteam, peerproc, peerrepseg+chunkidx*max_payload, longreplysrc+chunkidx*max_payload, max_payload, 0);
    }
    BARRIER();
  }

  // Build sorted array of sizes
  int num_sz = 3;  // maxmed, maxlong and max_payload
  for (size_t sz = max_payload; sz; sz >>= 1) { ++num_sz; }
  all_sizes = test_calloc(num_sz, sizeof(size_t));
  {
    size_t max1 = MIN(max_payload, MIN(maxmed, maxlong));
    size_t max2 = MIN(max_payload, MAX(maxmed, maxlong));
    int i = 0;
    size_t save_sz = 1;
    for (size_t sz = 1; sz <= max_payload; ) {
      all_sizes[i++] = sz;
      assert(i <= num_sz);
      /* double sz each time, but make sure to also exactly hit MaxMedium, MaxLong and max payload */
      size_t next_sz = save_sz * 2;
      if      (sz < max1        && next_sz > max1)        sz = max1;
      else if (sz < max2        && next_sz > max2)        sz = max2;
      else if (sz < max_payload && next_sz > max_payload) sz = max_payload;
      else { sz = save_sz = next_sz; }
    }
    num_sz = i;
  }
  assert_always(num_sz < (1<<SZ_BITS));

  for (int sz_idx = 0; sz_idx < num_sz; ++sz_idx) {
      size_t sz = all_sizes[sz_idx];
      if (dosizesync) BARRIER(); /* optional barrier, to synchronize tests at each payload size across nodes */
      
      MSG0("payload = %"PRIuSZ,sz);

      for (int iter = 0; iter < iters; iter++) {
        int chunkidx;
        uint8_t *srcseg = ITERSEG(iter);

        /* initialize local seg to known values */
        for (chunkidx = 0; chunkidx < depth; chunkidx++) {
          init_chunk(srcseg,sz,iter,chunkidx);
        }
        if (domed && sz <= maxmed) { /* test Medium AMs */
          gasnett_atomic_set(&pong_recvd,0,0);
          for (chunkidx = 0; chunkidx < depth; chunkidx++) {
            gex_AM_Arg_t arg1 = chunkidx | (sz_idx << CHUNK_BITS);
            gex_AM_SrcDesc_t sd;
            void *srcbuf = srcseg+chunkidx*sz;
            int imm = 0;
            gex_Flags_t flags = TEST_RAND_ONEIN(5) ? GEX_FLAG_IMMEDIATE : 0;
            size_t most_payload = TEST_RAND(sz, 2*sz);
            size_t least_payload = TEST_RAND(sz - sz/2, MIN(most_payload, least_payload_req_med));
            size_t len = TEST_RAND(sz - sz/2, sz);
          retry_med:
            switch (INJMODE(iter)) { // [0..2]
              case 0: // Fixed-payload
                imm = gex_AM_RequestMedium2(myteam, peerproc, hidx_ping_medhandler, srcbuf,
                                            len, GEX_EVENT_NOW, flags, iter, arg1);
                break;

              case 1: // Negotiated-payload with client-provided buffer
              {
                gex_Event_t lc = GEX_EVENT_NO_OP;
                gex_Event_t *lc_opt = NULL;
                switch (TEST_RAND(0,2)) {
                  case 0: lc_opt = &lc;             break;
                  case 1: lc_opt = GEX_EVENT_NOW;   break;
                  case 2: lc_opt = GEX_EVENT_GROUP; break;
                }
                sd = gex_AM_PrepareRequestMedium(myteam, peerproc, srcbuf, least_payload, most_payload, lc_opt, flags, 2);
                imm = (sd == GEX_AM_SRCDESC_NO_OP); // IMMEDIATE was NO OP
                if (imm) break;
                assert(gex_AM_SrcDescSize(sd) >= least_payload);
                assert(gex_AM_SrcDescSize(sd) <= most_payload);
                assert(gex_AM_SrcDescAddr(sd) == srcbuf);
                len = MIN(len, gex_AM_SrcDescSize(sd));
                gex_AM_CommitRequestMedium2(sd, hidx_ping_medhandler, len, iter, arg1);
                if (lc_opt == GEX_EVENT_GROUP) {
                  gex_NBI_Wait(GEX_EC_AM,0);
                } else if (lc_opt != GEX_EVENT_NOW) {
                  gex_Event_Wait(lc);
                }
                break;
              }

              case 2: // Negotiated-payload without client-provided buffer
                sd = gex_AM_PrepareRequestMedium(myteam, peerproc, NULL, least_payload, most_payload, NULL, flags, 2);
                imm = (sd == GEX_AM_SRCDESC_NO_OP); // IMMEDIATE was NO OP
                if (imm) break;
                assert(gex_AM_SrcDescSize(sd) >= least_payload);
                assert(gex_AM_SrcDescSize(sd) <= most_payload);
                len = MIN(len, gex_AM_SrcDescSize(sd));
                memcpy(gex_AM_SrcDescAddr(sd), srcbuf, len);
                gex_AM_CommitRequestMedium2(sd, hidx_ping_medhandler, len, iter, arg1);
                break;
            }
            if (imm) {
              assert(flags & GEX_FLAG_IMMEDIATE);
              flags &= ~GEX_FLAG_IMMEDIATE;
              goto retry_med;
            }
          }
          /* wait for completion */
          GASNET_BLOCKUNTIL(gasnett_atomic_read(&pong_recvd,0) == depth);
        }

        if (sz <= maxlong) { 
         if (dolong) { /* test Long AMs */
          gasnett_atomic_set(&pong_recvd,0,0);
          for (chunkidx = 0; chunkidx < depth; chunkidx++) {
            gex_AM_Arg_t arg1 = chunkidx | (sz_idx << CHUNK_BITS);
            gex_AM_SrcDesc_t sd;
            void *srcbuf = srcseg+chunkidx*sz;
            void *dstbuf = peerreqseg+chunkidx*sz;
            int imm = 0;
            gex_Flags_t flags = TEST_RAND_ONEIN(5) ? GEX_FLAG_IMMEDIATE : 0;
            void * maybe_dest = TEST_RAND_ONEIN(2) ? dstbuf : NULL; // Passing dest_addr to Prepare is optional
            size_t most_payload = TEST_RAND(sz, 2*sz);
            size_t least_payload = TEST_RAND(sz - sz/2, MIN(most_payload, least_payload_req_long));
            size_t len = TEST_RAND(sz - sz/2, sz);
          retry_long:
            switch (INJMODE(iter)) { // [0..2]
              case 0: // Fixed-payload
                imm = gex_AM_RequestLong2(myteam, peerproc, hidx_ping_longhandler, srcbuf, len,
                                          dstbuf, GEX_EVENT_NOW, flags, iter, arg1);
                break;

              case 1: // Negotiated-payload with client-provided buffer
              {
                gex_Event_t lc = GEX_EVENT_NO_OP;
                gex_Event_t *lc_opt = NULL;
                switch (TEST_RAND(0,2)) {
                  case 0: lc_opt = &lc;             break;
                  case 1: lc_opt = GEX_EVENT_NOW;   break;
                  case 2: lc_opt = GEX_EVENT_GROUP; break;
                }
                sd = gex_AM_PrepareRequestLong(myteam, peerproc, srcbuf, least_payload, most_payload, maybe_dest, lc_opt, flags, 2);
                imm = (sd == GEX_AM_SRCDESC_NO_OP); // IMMEDIATE was NO OP
                if (imm) break;
                assert(gex_AM_SrcDescSize(sd) >= least_payload);
                assert(gex_AM_SrcDescSize(sd) <= most_payload);
                assert(gex_AM_SrcDescAddr(sd) == srcbuf);
                len = MIN(len, sz);
                gex_AM_CommitRequestLong2(sd, hidx_ping_longhandler, len, dstbuf, iter, arg1);
                if (lc_opt == GEX_EVENT_GROUP) {
                  gex_NBI_Wait(GEX_EC_AM,0);
                } else if (lc_opt != GEX_EVENT_NOW) {
                  gex_Event_Wait(lc);
                }
                break;
              }

              case 2: // Negotiated-payload without client-provided buffer
                sd = gex_AM_PrepareRequestLong(myteam, peerproc, NULL, least_payload, most_payload, maybe_dest, NULL, flags, 2);
                imm = (sd == GEX_AM_SRCDESC_NO_OP); // IMMEDIATE was NO OP
                if (imm) break;
                assert(gex_AM_SrcDescSize(sd) >= least_payload);
                assert(gex_AM_SrcDescSize(sd) <= most_payload);
                len = MIN(len, sz);
                memcpy(gex_AM_SrcDescAddr(sd), srcbuf, len);
                gex_AM_CommitRequestLong2(sd, hidx_ping_longhandler, len, dstbuf, iter, arg1);
                break;
            }
            if (imm) {
              assert(flags & GEX_FLAG_IMMEDIATE);
              flags &= ~GEX_FLAG_IMMEDIATE;
              goto retry_long;
            }
          }
          /* wait for completion */
          GASNET_BLOCKUNTIL(gasnett_atomic_read(&pong_recvd,0) == depth);
         }
        }
      }
  }

  BARRIER();
  done = 1;

  return(0);
}
