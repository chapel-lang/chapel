/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testfaddperf.c $
 * Description: GASNet remote atomics performance test
 *   Measures average round-trip time and average flood throughput
 *   of the GEX_OP_(F)ADD remote atomic operations
 * Copyright 2002, Jaein Jeong and Dan Bonachea <bonachea@cs.berkeley.edu>
 * Copyright (c) 2017, The Regents of the University of California
 * Terms of use are as specified in license.txt
 */

#include <gasnetex.h>
#include "test.h"

static int iters = 0;

static gex_Client_t   myclient;
static gex_EP_t       myep;
static gex_TM_t       myteam;
static gex_Segment_t  mysegment;

#include <gasnet_ratomic.h>

static int myproc;
static int numprocs;
static int peerproc = -1;
static int iamsender = 0;
static int hotspotmode = 0;

static void* myseg;
static void* peerseg;

static gex_AD_t myad;


#define PRINT_LATENCY 0
#define PRINT_THROUGHPUT 1

typedef struct {
	int iters;
	uint64_t time;
} stat_struct_t;

#define init_stat \
  GASNETT_TRACE_SETSOURCELINE(__FILE__,__LINE__), _init_stat
#define update_stat \
  GASNETT_TRACE_SETSOURCELINE(__FILE__,__LINE__), _update_stat
#define print_stat \
  GASNETT_TRACE_SETSOURCELINE(__FILE__,__LINE__), _print_stat

void _init_stat(stat_struct_t *st)
{
	st->iters = 0;
	st->time = 0;
}

void _update_stat(stat_struct_t *st, uint64_t temptime, int iters)
{
	st->iters += iters;
	st->time += temptime;
}

void _print_stat(int myproc, stat_struct_t *st, const char *type, const char *opstr, int operation)
{
	switch (operation) {
	case PRINT_LATENCY:
		printf("%c: %2i : %7i iters, latency %10i us total, %9.3f us ave. (%s %s latency)\n",
                        TEST_SECTION_NAME(), myproc, st->iters,
                        (int) st->time, ((double)st->time) / st->iters,
			type, opstr);
		fflush(stdout);
		break;
	case PRINT_THROUGHPUT:
		printf("%c: %2i : %7i iters, throughput             %15.3f ops/sec (%s %s throughput)\n",
                        TEST_SECTION_NAME(), myproc, st->iters,
			((int)st->time == 0 ? 0.0
                                            : ((1.e6 * (double)st->iters / st->time))),
			type, opstr);
		fflush(stdout);
		break;
	default:
		break;
	}
}

#define TEST_DECL(_tcode, _type) \
void _tcode##_lat_test(gex_OP_t opcode, const char *opstr)                     \
{GASNET_BEGIN_FUNCTION();                                                      \
    int64_t begin, end;                                                        \
    stat_struct_t st;                                                          \
    _type result;                                                              \
                                                                               \
    /* initialize statistics */                                                \
    init_stat(&st);                                                            \
                                                                               \
    * (_type *) myseg = 0;                                                     \
                                                                               \
    BARRIER();                                                                 \
                                                                               \
    if (iamsender) {                                                           \
        /* measure the round-trip time of (F)ADD/NB */                         \
        begin = TIME();                                                        \
        for (int i = 0; i < iters; i++) {                                      \
            gex_Event_Wait(                                                    \
                gex_AD_OpNB_##_tcode(myad, &result, peerproc, peerseg,         \
                                     opcode, i&15, 9999, 0));                  \
        }                                                                      \
        end = TIME();                                                          \
        update_stat(&st, (end - begin), iters);                                \
    }                                                                          \
                                                                               \
    BARRIER();                                                                 \
                                                                               \
    if (iamsender) {                                                           \
        print_stat(myproc, &st, #_tcode, opstr, PRINT_LATENCY);                \
    }                                                                          \
}                                                                              \
void _tcode##_tput_test(gex_OP_t opcode, const char *opstr)                    \
{GASNET_BEGIN_FUNCTION();                                                      \
    int64_t begin, end;                                                        \
    stat_struct_t st;                                                          \
    _type *results = test_malloc(iters * sizeof(_type));                       \
                                                                               \
    /* initialize statistics */                                                \
    init_stat(&st);                                                            \
                                                                               \
    BARRIER();                                                                 \
                                                                               \
    if (iamsender) {                                                           \
        /* measure the throughput of (F)ADD/NBI */                             \
        begin = TIME();                                                        \
        for (int i = 0; i < iters; i++) {                                      \
            gex_AD_OpNBI_##_tcode(myad, results + i, peerproc, peerseg,        \
                                  opcode, i&15, 9999, 0);                      \
        }                                                                      \
        gex_NBI_Wait(GEX_EC_RMW, 0);                                           \
        if (hotspotmode) BARRIER();                                            \
        end = TIME();                                                          \
        update_stat(&st, (end - begin), iters);                                \
    } else {                                                                   \
        if (hotspotmode) BARRIER();                                            \
    }                                                                          \
                                                                               \
    BARRIER();                                                                 \
                                                                               \
    if (iamsender) {                                                           \
        print_stat(myproc, &st, #_tcode, opstr, PRINT_THROUGHPUT);             \
    }                                                                          \
                                                                               \
    test_free(results);                                                        \
}                                                                              \
void do_##_tcode(int skipwarmup, gex_OP_t opcode, const char *opstr)           \
{GASNET_BEGIN_FUNCTION();                                                      \
    gex_AD_Create(&myad, myteam, GEX_DT_##_tcode, opcode, 0);                  \
    if (iamsender && !skipwarmup) {                                            \
        _type result;                                                          \
        int warm_iters = MIN(iters, 10000);                                    \
        gex_Event_t *ph = test_malloc(warm_iters * sizeof(gex_Event_t));       \
        for (int i = 0; i < warm_iters; ++i) {                                 \
            ph[i] = gex_AD_OpNB_##_tcode(myad, &result, peerproc, peerseg,     \
                                         opcode, 1, 9999, 0);                  \
            gex_AD_OpNBI_##_tcode(myad, &result, peerproc, peerseg,            \
                                  opcode, 1, 9999, 0);                         \
        }                                                                      \
        gex_NBI_Wait(GEX_EC_RMW,0);                                            \
        gex_Event_WaitAll(ph, warm_iters, 0);                                  \
        test_free(ph);                                                         \
    }                                                                          \
                                                                               \
    BARRIER();                                                                 \
                                                                               \
    if (TEST_SECTION_BEGIN_ENABLED()) _tcode##_lat_test(opcode, opstr);        \
    if (TEST_SECTION_BEGIN_ENABLED()) _tcode##_tput_test(opcode, opstr);       \
                                                                               \
    BARRIER();                                                                 \
                                                                               \
    gex_AD_Destroy(myad);                                                      \
}

TEST_DECL(U32, uint32_t)
TEST_DECL(U64, uint64_t)
TEST_DECL(FLT, float)
TEST_DECL(DBL, double)

int main(int argc, char **argv)
{
    int arg;
    int firstlastmode = 0;
    int fullduplexmode = 0;
    int crossmachinemode = 0;
    int skipwarmup = 0;
    int help = 0;

    /* call startup */
    GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, "testfaddperf", &argc, &argv, 0));

    /* parse arguments */
    arg = 1;
    while (argc > arg) {
      if (!strcmp(argv[arg], "-f")) {
        firstlastmode = 1;
        ++arg;
      } else if (!strcmp(argv[arg], "-c")) {
        crossmachinemode = 1;
        ++arg;
      } else if (!strcmp(argv[arg], "-a")) {
        fullduplexmode = 1;
        ++arg;
      } else if (!strcmp(argv[arg], "-S")) {
        hotspotmode = 1;
        ++arg;
      } else if (!strcmp(argv[arg], "-s")) {
        skipwarmup = 1;
        ++arg;
      } else if (argv[arg][0] == '-') {
        help = 1;
        ++arg;
      } else break;
    }

    if (argc > arg) { iters = atoi(argv[arg]); arg++; }
    if (!iters) iters = 10000;
    if (argc > arg) { TEST_SECTION_PARSE(argv[arg]); arg++; }

    GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ_REQUEST));
    test_init("testfaddperf",1, "[options] (iters) (test_sections)\n"
               "  The -s option skips warm-up iterations\n"
               "  The -a option enables full-duplex mode, where all nodes send.\n"
               "  The -S option enables single-counter mode, where all nodes send to node zero.\n"
               "  The -c option enables cross-machine pairing, default is nearest neighbor.\n"
               "  The -f option enables 'first/last' mode, where the first/last\n"
               "   nodes communicate with each other, while all other nodes sit idle.\n");
    if (help || argc > arg) test_usage();

    // TODO: -p for polling threads in PAR builds

    /* get SPMD info */
    myproc = gex_TM_QueryRank(myteam);
    numprocs = gex_TM_QuerySize(myteam);

    if (!firstlastmode) {
      /* Only allow 1 or even number for numprocs */
      if (numprocs > 1 && numprocs % 2 != 0) {
        MSG0("WARNING: This test requires a unary or even number of nodes. Test skipped.\n");
        gasnet_exit(0); /* exit 0 to prevent false negatives in test harnesses for smp-conduit */
      }
    }

    /* Setting peer thread rank */
    if (firstlastmode) {
      peerproc = (myproc == 0 ? numprocs-1 : 0);
      iamsender = (fullduplexmode ? myproc == 0 || myproc == numprocs-1 : myproc == 0);
    }  else if (numprocs == 1) {
      peerproc = 0;
      iamsender = 1;
    } else if (crossmachinemode) {
      if (myproc < numprocs / 2) {
        peerproc = myproc + numprocs/2;
        iamsender = 1;
      } else {
        peerproc = myproc - numprocs/2;
        iamsender = fullduplexmode;
      }
    } else if (hotspotmode) {
      peerproc = 0;
      iamsender = 1;
    } else {
      peerproc = (myproc % 2) ? (myproc - 1) : (myproc + 1);
      iamsender = (fullduplexmode || myproc % 2 == 0);
    }

    myseg   = TEST_MYSEG();
    peerseg = TEST_SEG(peerproc);

        MSG0("Running %i iterations of %s%s%s%sremote atomic (F)ADD perfomance tests\n",
          iters,
          (firstlastmode ? "first/last " : ""),
          (fullduplexmode ? "full-duplex ": ""),
          (crossmachinemode ? "cross-machine ": ""),
          (hotspotmode ? "single-counter ": ""));
        BARRIER();

        do_U32(skipwarmup, GEX_OP_FADD, "FADD");
        do_U32(skipwarmup, GEX_OP_ADD,  "ADD");

        do_U64(skipwarmup, GEX_OP_FADD, "FADD");
        do_U64(skipwarmup, GEX_OP_ADD,  "ADD");

        do_FLT(skipwarmup, GEX_OP_FADD, "FADD");
        do_FLT(skipwarmup, GEX_OP_ADD,  "ADD");

        do_DBL(skipwarmup, GEX_OP_FADD, "FADD");
        do_DBL(skipwarmup, GEX_OP_ADD,  "ADD");

    gasnet_exit(0);

    return 0;
}
/* ------------------------------------------------------------------------------------ */
