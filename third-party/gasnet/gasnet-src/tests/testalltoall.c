/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testalltoall.c $
 * Description: GASNet AM all-to-all correctness tests
 * Copyright (c) 2020, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#include <gasnetex.h>
#include <gasnet_tools.h>

#include <test.h>

static int do_reply = 0;
static gasnett_atomic_t counter;

enum {
  hidx_ping_shorthandler = GEX_AM_INDEX_BASE,
  hidx_ping_medhandler,
  hidx_pong_shorthandler,
  hidx_pong_medhandler
};

void ping_shorthandler(gex_Token_t token) {
  if (do_reply) gex_AM_ReplyShort0(token, hidx_pong_shorthandler, 0);
  else gasnett_atomic_increment(&counter,0);
}
void ping_medhandler(gex_Token_t token, void *buf, size_t nbytes) {
  if (do_reply) gex_AM_ReplyMedium0(token, hidx_pong_medhandler, buf, nbytes, GEX_EVENT_NOW, 0);
  else gasnett_atomic_increment(&counter,0);
}

void pong_shorthandler(gex_Token_t token) {
  gasnett_atomic_increment(&counter,0);
}
void pong_medhandler(gex_Token_t token, void *buf, size_t nbytes) {
  gasnett_atomic_increment(&counter,0);
}

gex_AM_Entry_t htable[] = {
  { hidx_ping_shorthandler, ping_shorthandler, GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_SHORT,  0 },
  { hidx_ping_medhandler,   ping_medhandler,   GEX_FLAG_AM_REQUEST|GEX_FLAG_AM_MEDIUM, 0 },
  { hidx_pong_shorthandler, pong_shorthandler, GEX_FLAG_AM_REPLY  |GEX_FLAG_AM_SHORT,  0 },
  { hidx_pong_medhandler,   pong_medhandler,   GEX_FLAG_AM_REPLY  |GEX_FLAG_AM_MEDIUM, 0 },
};

/* ------------------------------------------------------------------------------------ */

static gex_Client_t      myclient;
static gex_EP_t          myep;
static gex_TM_t          myteam;
static gex_Segment_t     mysegment;

static gex_Rank_t        myrank;
static gex_Rank_t        numranks;

/* ------------------------------------------------------------------------------------ */
// Produce a sequence of ranks to send to according to one of three paterns
static enum {
  SEQUENCE_POLITE,  // Each rank starts sending to self
  SEQUENCE_HOTSPOT, // Each rank starts sending to 0
  SEQUENCE_RANDOM   // Each rank uses a distinct random sequence
} seq_type = SEQUENCE_RANDOM;

static gex_Rank_t *rank_array = NULL;

void init_ranks(void) {
  int first_call = !rank_array;

  if (first_call) {
    rank_array = test_malloc(numranks * sizeof(gex_Rank_t));

    // consecutive entries, starting from 0 or self
    gex_Rank_t start = (seq_type == SEQUENCE_POLITE) ? myrank : 0;
    for (gex_Rank_t r = 0; r < numranks; ++r) {
      rank_array[r] = (r+start) % numranks;
    }
  }

  if (seq_type == SEQUENCE_RANDOM) {
    // permute the array on *every* call
    // TODO: Currently this is used to shuffle the array once every "round".
    // However, there are alternatives which would provide greater randomness,
    // and could even create a bias that favors consecutive sends to the same
    // peer or to peers on the same node.  Use of the same space to count AMs
    // sent to each peer should be sufficient for that purpose.
    for (gex_Rank_t r = 0; r < numranks-1; ++r) {
      gex_Rank_t p = TEST_RAND(r, numranks - 1);
      gex_Rank_t tmp = rank_array[r];
      rank_array[r] = rank_array[p];
      rank_array[p] = tmp;
    }
  }
}

/* ------------------------------------------------------------------------------------ */

int main(int argc, char **argv) {
  int progress = 0;
  int min_size = 0;
  int step_size = 0;
  int help = 0;
  uint8_t payload_byte = 0;
  
  GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, "testalltoall", &argc, &argv, 0));

  myrank = gex_TM_QueryRank(myteam);
  numranks = gex_TM_QuerySize(myteam);

  //GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ_REQUEST));  UNUSED
  GASNET_Safe(gex_EP_RegisterHandlers(myep, htable, sizeof(htable)/sizeof(gex_AM_Entry_t)));

  int argi = 1;
  while (argc > argi) {
    if (!strcmp(argv[argi], "-random")) {
      seq_type = SEQUENCE_RANDOM;
      ++argi;
    } else if (!strcmp(argv[argi], "-polite")) {
      seq_type = SEQUENCE_POLITE;
      ++argi;
    } else if (!strcmp(argv[argi], "-hotspot")) {
      seq_type = SEQUENCE_HOTSPOT;
      ++argi;
    } else if (!strcmp(argv[argi], "-progress")) { // UNDOCUMENTED
      progress = 1;
      ++argi;
    } else if (!strcmp(argv[argi], "-min-size")) {
      ++argi;
      if (argc > argi) { min_size = atoi(argv[argi]); argi++; }
      else help = 1;
    } else if (!strcmp(argv[argi], "-step-size")) {
      ++argi;
      if (argc > argi) { step_size = atoi(argv[argi]); argi++; }
      else help = 1;
    } else if (!strcmp(argv[argi], "-reply")) {
      do_reply = 1;
      ++argi;
    } else if (!strcmp(argv[argi], "-no-reply")) {
      do_reply = 0;
      ++argi;
    } else if (!strcmp(argv[argi], "-payload-byte")) {
      ++argi;
      if (argc > argi) { payload_byte = atoi(argv[argi]); argi++; }
      else help = 1;
    } else if (argv[argi][0] == '-') {
      help = 1;
      ++argi;
    } else break;
  }

  int iters = 0;
  if (argc > argi) { iters = atoi(argv[argi]); ++argi; }
  if (!iters) iters = 1000;

  size_t med_sz = 0;
  if (argc > argi) { med_sz = atoi(argv[argi]); ++argi; }
  if (!med_sz) med_sz = MIN(gex_AM_LUBRequestMedium(),gex_AM_LUBReplyMedium());

  unsigned int seed = 0;
  if (argc > argi) { seed = atoi(argv[argi]); ++argi; }
  if (!seed) seed = ((unsigned int)TIME() ^ myrank) & 0xFFFF;
  TEST_SRAND(seed);

  if ((min_size < 0) || (min_size > med_sz)) min_size = med_sz;
  if ((step_size < 0) || (step_size > med_sz)) step_size = 0;

  // TODO: test sections
  test_init("testalltoall",0,"[options] (iters) (maxsz) (seed)\n"
             "  -payload-byte N\n"
             "      Medium payload buffer is initialized to this value mod 256.\n"
             "      The default is zero.\n"
             "  The following options determine the communication pattern:\n"
             "      -random:  each process sends to others in a distinct random order\n"
             "      -polite:  each process sends round-robin starting with itself\n"
             "      -hotspot: each process sends round-robin starting with process 0\n"
             "      The default for this option group is '-random'.\n"
             "  The following options control the use of AM Replies:\n"
             "      -no-reply: no Replies will be sent\n"
             "      -reply:    every Request will send a corresponding Reply\n"
             "      The default for this option group is '-no-reply'.\n"
             "  The following options determine the Medium payload size(s):\n"
             "      -min-size N\n"
             "           Sets the minimum payload size.\n"
             "           Invalid values (less than zero or larger than the maximum)\n"
             "           will set the minimum equal to the maximum.\n"
             "      -step-size N\n"
             "           Sets a value by which the payload size increases at each\n"
             "           successive Request, starting from the minimum and wrapping\n"
             "           back to the minimum when the size would exceed the maximum.\n"
             "           Invalid values (zero or larger than the maximum) select\n"
             "           uniformly distributed random payload sizes, rather than\n"
             "           fixed-width steps.\n"
             "      The default for this option group is '-min-size 0 -step-size 0'.");
  if (help || argc > argi) test_usage();

  int rounds = (iters + numranks - 1) / numranks;
  iters = rounds * numranks;
  int tick = iters/10;

  void *payload = test_malloc(med_sz);
  memset(payload, payload_byte, med_sz);

  gasnett_atomic_set(&counter,0,0);

  fflush(stdout); fflush(stderr); sleep(1);
  BARRIER();

  MSG0("Running %d iterations", iters);
  switch (seq_type) {
    case SEQUENCE_RANDOM:
      MSG0("    peer sequence: random");
      break;
    case SEQUENCE_POLITE:
      MSG0("    peer sequence: polite");
      break;
    case SEQUENCE_HOTSPOT:
      MSG0("    peer sequence: hotspot");
      break;
  }
  MSG0("    replies: %s", do_reply?"YES":"NO");
  MSG0("    payload byte: 0x%02x", payload_byte);

  MSG0("Starting Short0 test");
  for (int r = 0, sent = 1; r < rounds; ++r) {
    init_ranks();
    for (gex_Rank_t i = 0; i < numranks; ++i, ++sent) {
      gex_AM_RequestShort0(myteam, rank_array[i], hidx_ping_shorthandler, 0);
      if (progress && !((iters-sent) % tick)) {
        MSG0("Sent %d of %d (%d %s received)",
             sent, iters, (int)gasnett_atomic_read(&counter,0), do_reply ? "replies" : "requests");
      }
    }
  }

  GASNET_BLOCKUNTIL(gasnett_atomic_read(&counter,0) == iters);
  gasnett_atomic_set(&counter,0,0);
  BARRIER();

  MSG0("Starting Medium0 test");
  if (min_size == med_sz) {
    MSG0("    payload size: %"PRIuSZ, med_sz);
  } else if (step_size) {
    MSG0("    payload size: in [%d, %"PRIuSZ"] with step size of %d",
         min_size, med_sz, step_size);
  } else {
    MSG0("    payload size: in [%d, %"PRIuSZ"] with uniform random distribution",
         min_size, med_sz);
  }

  size_t prev_sz = med_sz; // wrap to min in first iteration
  for (int r = 0, sent = 1; r < rounds; ++r) {
    init_ranks();
    for (gex_Rank_t i = 0; i < numranks; ++i, ++sent) {
      size_t sz;
      if (step_size) {
        sz = prev_sz + step_size;
        if (sz > med_sz) sz = min_size;
      } else {
        sz = TEST_RAND(min_size, med_sz);
      }
      assert(sz <= med_sz);
      assert(sz >= min_size);
      prev_sz = sz;
      gex_AM_RequestMedium0(myteam, rank_array[i], hidx_ping_medhandler, payload, sz, GEX_EVENT_GROUP, 0);
      if (progress && !((iters-sent) % tick)) {
        MSG0("Sent %d of %d (%d %s received)",
             sent, iters, (int)gasnett_atomic_read(&counter,0), do_reply ? "replies" : "requests");
      }
    }
  }
  gex_NBI_Wait(GEX_EC_AM,0);

  GASNET_BLOCKUNTIL(gasnett_atomic_read(&counter,0) == iters);
  gasnett_atomic_set(&counter,0,0);
  BARRIER();

  // TODO: Long0

  MSG0("done.");

  test_free(rank_array);
  test_free(payload);

  gasnet_exit(0);
  return 0;
}
