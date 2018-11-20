/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testcoll.c $
 * Description: GASNet collectives test
 * Copyright 2002-2004, Jaein Jeong and Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnetex.h>
#include "gasnet_coll.h"

#if GASNET_PAR
  #define DEFAULT_THREADS 2
#else
  #define DEFAULT_THREADS 1
#endif

static gex_Client_t      myclient;
static gex_EP_t    myep;
static gex_TM_t myteam;
static gex_Segment_t     mysegment;

int datasize;
int numprocs;
int iters = 0;
int images;	 /* numproc * threads */
int threads = DEFAULT_THREADS; /* per node */

#define TEST_SEGSZ_EXPR (sizeof(int)*(threads*datasize+numprocs))
#include "test.h"


#if GASNET_PAR
  #define local_barrier()	PTHREAD_LOCALBARRIER(threads)
  #define global_barrier()	PTHREAD_BARRIER(threads)
#else
  #define local_barrier()	do {} while(0)
  #define global_barrier()	BARRIER()
#endif

typedef struct {
  int local_id;
  int mythread;
  int myproc;
  int peerthread;
  gasnet_coll_handle_t *hndl;
  char _pad[GASNETT_CACHE_LINE_BYTES];
} thread_data_t;

#define PROLOGUE(NAME) \
  GASNET_BEGIN_FUNCTION(); \
  const char name[] = NAME; \
  int myproc = (randomize(td), td->myproc); \
  int mythread = td->mythread; \
  int peerthread = td->peerthread; \
  gasnet_coll_handle_t *hndl = td->hndl

static int *R;
static int **Aw, **Bw, **Cw, **Dw, **Ew, **Fw, **Gw;
static int ***Av, ***Bv, ***Cv, ***Dv, ***Ev, ***Fv, ***Gv;

/* All nodes must get the same random sequence */
static void randomize(thread_data_t *td) {
  local_barrier();
  if (td->local_id == 0) {
    int i;
    static int srand_phase = 1;
    TEST_SRAND(srand_phase++);
    for (i=0; i<iters; ++i) {
      R[i] = TEST_RAND(0,1000000000);
    }
  }
  local_barrier();
}

#define MSG00 \
    if (!td->local_id) MSG0
  

#define CALL(FUNC,DST,SRC,FLAGS) \
  gasnet_coll_##FUNC(GASNET_TEAM_ALL,DST,SRC,sizeof(int),\
			FLAGS|GASNET_COLL_SRC_IN_SEGMENT|GASNET_COLL_DST_IN_SEGMENT);
#define DEFN(PREFIX, DESC)                                                   \
/* NO/NO - in/out data is not generated/consumed in same barrier phase */    \
void PREFIX##_NONO(int root, thread_data_t *td) {                            \
    PROLOGUE(DESC " NO/NO");                                                 \
    int flags = GASNET_COLL_LOCAL| GASNET_COLL_IN_NOSYNC | GASNET_COLL_OUT_NOSYNC; \
    int j;                                                                   \
                                                                             \
    MSG00("Starting %s test", name);                                         \
                                                                             \
    for (j = 0; j < iters; ++j) {                                            \
	gex_Rank_t i;                                                     \
                                                                             \
	*LOCAL(A) = (mythread == root) ? R[j] : -1;                          \
	*LOCAL(B) = mythread;                                                \
	for (i = 0; i < images; ++i) {                                       \
	    LOCAL(D)[i] = i * R[j] + mythread;         \
	}                                                                    \
                                                                             \
	global_barrier();                                                    \
                                                                             \
	CALL(broadcast, ALL(A), ROOT(A), flags);                             \
	CALL(gather, ROOT(C), ALL(B), flags);                                \
	CALL(scatter, ALL(E), ROOT(D), flags);                               \
	CALL(gather_all, ALL(F), ALL(B), flags);                             \
	CALL(exchange, ALL(G), ALL(D), flags);                               \
                                                                             \
	global_barrier();                                                    \
                                                                             \
	if (*LOCAL(A) != R[j]) {                                             \
	    MSG("ERROR: %s broadcast validation failed", name);              \
	    gasnet_exit(1);                                                  \
	}                                                                    \
	if (mythread == root) {                                              \
	    for (i = 0; i < images; ++i) {                                   \
		if (LOCAL(C)[i] != i) {                                      \
		    MSG("ERROR: %s gather validation failed %d %d", name, LOCAL(C)[i], (int)i);\
		    gasnet_exit(1);                                          \
		}                                                            \
	    }                                                                \
	}                                                                    \
	if (*LOCAL(E) != mythread*R[j] + root) {                             \
	    MSG("ERROR: %s scatter validation failed %d %d", name, *LOCAL(E),  mythread*R[j] + root);                \
	    gasnet_exit(1);                                                  \
	}                                                                    \
	for (i = 0; i < images; ++i) {                                       \
	    if (LOCAL(F)[i] != i) {                                          \
		MSG("ERROR: %s gather_all validation failed", name);         \
		gasnet_exit(1);                                              \
	    }                                                                \
	}                                                                    \
	for (i = 0; i < images; ++i) {                                       \
	    if (LOCAL(G)[i] != i + mythread*R[j]) {                          \
		MSG("ERROR: %s exchange validation failed", name);           \
		gasnet_exit(1);                                              \
	    }                                                                \
	}                                                                    \
    }                                                                        \
                                                                             \
    global_barrier(); /* ensure validation completes before next test */     \
}                                                                            \
/* MY/MY - in/out data is generated/consumed locally in same barrier phase */\
void PREFIX##_MYMY(int root, thread_data_t *td) {                            \
    PROLOGUE(DESC " MY/MY");                                                 \
    int flags = GASNET_COLL_LOCAL| GASNET_COLL_IN_MYSYNC | GASNET_COLL_OUT_MYSYNC; \
    int j;                                                                   \
                                                                             \
    MSG00("Starting %s test", name);                                         \
                                                                             \
    for (j = 0; j < iters; ++j) {                                            \
	gex_Rank_t i;                                                     \
                                                                             \
	*LOCAL(A) = (mythread == root) ? R[j] : -1;                          \
	*LOCAL(B) = mythread;                                                \
                                                                             \
	CALL(broadcast, ALL(A), ROOT(A), flags);                             \
	if (*LOCAL(A) != R[j]) {                                             \
	    MSG("ERROR: %s broadcast validation failed %d %d on %d", name, *LOCAL(A), R[j], mythread);              \
	    gasnet_exit(1);                                                  \
	}                                                                    \
	CALL(gather, ROOT(C), ALL(B), flags);                                \
	if (mythread == root) {                                              \
	    for (i = 0; i < images; ++i) {                                   \
		if (LOCAL(C)[i] != i) {                                      \
		    MSG("ERROR: %s gather validation failed", name);         \
		    gasnet_exit(1);                                          \
		}                                                            \
		LOCAL(C)[i] *= R[j];                                         \
	    }                                                                \
	}                                                                    \
	CALL(scatter, ALL(B), ROOT(C), flags);                               \
	if (*LOCAL(B) != mythread*R[j]) {                                    \
	    MSG("ERROR: %s scatter validation failed", name);                \
	    gasnet_exit(1);                                                  \
	}                                                                    \
	CALL(gather_all, ALL(C), ALL(B), flags);                             \
	for (i = 0; i < images; ++i) {                                       \
	    if (LOCAL(C)[i] != i*R[j]) {                                     \
		MSG("ERROR: %s gather_all validation failed", name);         \
		gasnet_exit(1);                                              \
	    }                                                                \
	    LOCAL(C)[i] += mythread;                                         \
	}                                                                    \
	CALL(exchange, ALL(D), ALL(C), flags);                               \
	for (i = 0; i < images; ++i) {                                       \
	    if (LOCAL(D)[i] != i + mythread*R[j]) {                          \
		MSG("ERROR: %s exchange validation failed", name);           \
		gasnet_exit(1);                                              \
	    }                                                                \
	}                                                                    \
    }                                                                        \
                                                                             \
    global_barrier(); /* ensure validation completes before next test */     \
}                                                                            \
/* ALL/ALL - data is generated/consumed *remotely* in same barrier phase */  \
void PREFIX##_ALLALL(int root, thread_data_t *td) {                          \
    PROLOGUE(DESC " ALL/ALL");                                               \
    int flags = GASNET_COLL_LOCAL| GASNET_COLL_IN_ALLSYNC | GASNET_COLL_OUT_ALLSYNC; \
    gex_Rank_t rootproc = root/threads;                                   \
    gex_Rank_t peerproc = peerthread/threads;                             \
    int j;                                                                   \
    int tmp;                                                                 \
                                                                             \
    MSG00("Starting %s test", name);                                         \
                                                                             \
    for (j = 0; j < iters; ++j) {                                            \
	gex_Rank_t i;                                                     \
                                                                             \
	tmp = (peerthread == root) ? R[j] : -1;                              \
	gex_RMA_PutBlocking(myteam, peerproc, REMOTE(A,peerthread), &tmp, sizeof(int), 0);\
                                                                             \
	CALL(broadcast, ALL(A), ROOT(A), flags);                             \
	gex_RMA_GetBlocking(myteam, &tmp, peerproc, REMOTE(A,peerthread), sizeof(int), 0);\
	if (tmp != R[j]) {                                                   \
	    MSG("ERROR: %s broadcast validation failed", name);              \
	    gasnet_exit(1);                                                  \
	}                                                                    \
	tmp = peerthread;                                                    \
	gex_RMA_PutBlocking(myteam, peerproc, REMOTE(B,peerthread), &tmp, sizeof(int), 0);\
	CALL(gather, ROOT(C), ALL(B), flags);                                \
	gex_RMA_GetBlocking(myteam, LOCAL(D), rootproc, REMOTE(C,root), images*sizeof(int), 0); \
	for (i = 0; i < images; ++i) {                                       \
	    if (LOCAL(D)[i] != i) {                                          \
		MSG("ERROR: %s gather validation failed", name);             \
		gasnet_exit(1);                                              \
	    }                                                                \
	}                                                                    \
	global_barrier(); /* to avoid conflict on D */                       \
	tmp = mythread * R[j];                                               \
	gex_RMA_PutBlocking(myteam, rootproc, REMOTE(D,root)+mythread, &tmp, sizeof(int), 0);\
	CALL(scatter, ALL(B), ROOT(D), flags);                               \
	gex_RMA_GetBlocking(myteam, &tmp, peerproc, REMOTE(B,peerthread), sizeof(int), 0);\
	if (tmp != peerthread*R[j]) {                                        \
	    MSG("ERROR: %s scatter validation failed expected: %d got %d", name, peerthread*R[j], tmp);                \
	    gasnet_exit(1);                                                  \
	}                                                                    \
	global_barrier(); /* to avoid conflict on B */                       \
	tmp = peerthread*R[j] - 1;                                           \
	gex_RMA_PutBlocking(myteam, peerproc, REMOTE(B,peerthread), &tmp, sizeof(int), 0);\
	CALL(gather_all, ALL(C), ALL(B), flags);                             \
	gex_RMA_GetBlocking(myteam, LOCAL(D), peerproc, REMOTE(C,peerthread), images*sizeof(int), 0);\
	for (i = 0; i < images; ++i) {                                       \
	    if (LOCAL(D)[i] != i*R[j] - 1) {                                 \
		MSG("ERROR: %s gather_all validation failed", name);         \
		gasnet_exit(1);                                              \
	    }                                                                \
	}                                                                    \
	global_barrier(); /* to avoid conflict on C & D */                   \
	for (i = 0; i < images; ++i) {                                       \
	    LOCAL(C)[i] += peerthread;                                       \
	}                                                                    \
	gex_RMA_PutBlocking(myteam, peerproc, REMOTE(D,peerthread), LOCAL(C), images*sizeof(int), 0);\
	CALL(exchange, ALL(C), ALL(D), flags);                               \
	gex_RMA_GetBlocking(myteam, LOCAL(D), peerproc, REMOTE(C,peerthread), images*sizeof(int), 0);\
	for (i = 0; i < images; ++i) {                                       \
	    if (LOCAL(D)[i] != i + peerthread*R[j] - 1) {                    \
		MSG("ERROR: %s exchange validation failed", name);           \
		gasnet_exit(1);                                              \
	    }                                                                \
	}                                                                    \
    }                                                                        \
                                                                             \
    global_barrier(); /* ensure validation completes before next test */     \
}                                                                            \
void PREFIX##_NB(int root, thread_data_t *td) {                              \
    PROLOGUE(DESC " NB");                                                    \
    int flags = GASNET_COLL_LOCAL| GASNET_COLL_IN_MYSYNC | GASNET_COLL_OUT_MYSYNC; \
    gex_Rank_t i;                                                            \
    int j;                                                                   \
                                                                             \
    MSG00("Starting %s test", name);                                         \
                                                                             \
    for (j = 0; j < iters; ++j) {                                            \
	*LOCALi(j,A) = (mythread == root) ? R[j] : -1;                       \
	*LOCALi(j,B) = mythread;                                             \
	hndl[j] = CALL(broadcast##_nb, ALLi(j,A), ROOTi(j,A), flags);        \
    }                                                                        \
    for (j = 0; j < iters; ++j) {                                            \
        gasnet_coll_wait_sync(hndl[j]);                                      \
	if (*LOCALi(j,A) != R[j]) {                                          \
	    MSG("ERROR: %s broadcast validation failed", name);              \
	    gasnet_exit(1);                                                  \
	}                                                                    \
	hndl[j] = CALL(gather##_nb, ROOTi(j,C), ALLi(j,B), flags);           \
    }                                                                        \
    for (j = 0; j < iters; ++j) {                                            \
        gasnet_coll_wait_sync(hndl[j]);                                      \
	if (mythread == root) {                                              \
	    for (i = 0; i < images; ++i) {                                   \
		if (LOCALi(j,C)[i] != i) {                                   \
		    MSG("ERROR: %s gather validation failed", name);         \
		    gasnet_exit(1);                                          \
		}                                                            \
		LOCALi(j,C)[i] *= R[j];                                      \
	    }                                                                \
	}                                                                    \
	hndl[j] = CALL(scatter##_nb, ALLi(j,B), ROOTi(j,C), flags);          \
    }                                                                        \
    for (j = 0; j < iters; ++j) {                                            \
        gasnet_coll_wait_sync(hndl[j]);                                      \
	if (*LOCALi(j,B) != mythread*R[j]) {                                 \
	    MSG("ERROR: %s scatter validation failed", name);                \
	    gasnet_exit(1);                                                  \
	}                                                                    \
	hndl[j] = CALL(gather_all##_nb, ALLi(j,C), ALLi(j,B), flags);        \
    }                                                                        \
    for (j = 0; j < iters; ++j) {                                            \
        gasnet_coll_wait_sync(hndl[j]);                                      \
	for (i = 0; i < images; ++i) {                                       \
	    if (LOCALi(j,C)[i] != i*R[j]) {                                  \
		MSG("ERROR: %s gather_all validation failed", name);         \
		gasnet_exit(1);                                              \
	    }                                                                \
	    LOCALi(j,C)[i] += mythread;                                      \
	}                                                                    \
	hndl[j] = CALL(exchange##_nb, ALLi(j,D), ALLi(j,C), flags);          \
    }                                                                        \
    for (j = 0; j < iters; ++j) {                                            \
        gasnet_coll_wait_sync(hndl[j]);                                      \
	for (i = 0; i < images; ++i) {                                       \
	    if (LOCALi(j,D)[i] != i + mythread*R[j]) {                       \
		MSG("ERROR: %s exchange validation failed", name);           \
		gasnet_exit(1);                                              \
	    }                                                                \
	}                                                                    \
    }                                                                        \
                                                                             \
    global_barrier(); /* ensure validation completes before next test */     \
}

#define ALL(X)		ALLi(0,X)
#define ROOT(X)		ROOTi(0,X)
#define REMOTE(X,N)	REMOTEi(0,X,N)
#define LOCAL(X)	LOCALi(0,X)
#define LOCALi(i,X)	REMOTEi(i,X,mythread)

#define ALLi(i,X)      X##v[i][mythread]
#define ROOTi(i,X)	root, (mythread==root)?X##v[i][root]:NULL
#define REMOTEi(i,X,N)	(X##v[i][N])
DEFN(testLS, "LOCAL/single-addr")
#undef ALLi
#undef ROOTi
#undef REMOTEi


static void *test_malloc_2D(int count, size_t size) {
    char *tmp;
    void **result;
    int j;

    tmp = test_malloc(count * (sizeof(void*) + size));
    result = (void **)tmp;
    tmp += count * sizeof(void *);

    for (j = 0; j < count; ++j) {
      result[j] = (void *)tmp;
      tmp += size;
    }

    return (void *)result;
}
#define test_free_2D test_free

static volatile int done = 0;

void *thread_main(void *arg) {
  thread_data_t *td = arg;
  int i;
                
#if GASNET_PAR
  if (td->local_id >= threads) {
    while (!done) gasnet_AMPoll();
    return NULL;
  }
#endif

  td->hndl = test_malloc(iters*sizeof(gasnet_coll_handle_t));

  /* Run w/ root = (first, middle, last) w/o duplication */
  for (i = 0; i < 3; ++i) {
    int root;

    if (i == 0) {
      root = 0;
    } else if (i == 1) {
      if (images < 3) continue;
      root = images / 2;
    } else {
      if (images < 2) continue;
      root = images - 1;
    }

    MSG00("Running tests with root = %d", (int)root);

    testLS_NONO(root, td);
    testLS_MYMY(root, td);
    testLS_ALLALL(root, td);
    testLS_NB(root, td);
  }
  
  test_free(td->hndl);

  PTHREAD_LOCALBARRIER(threads);
  done = 1;

  return NULL;
}

int main(int argc, char **argv)
{
    static int *A, *B, *C, *D, *E, *F, *G;
    gex_Rank_t myproc, i;
    int pollers = 0;
    int j;
   
    /* call startup */
    GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, "testcoll", &argc, &argv, 0));

    int arg = 1;
    int help = 0;
    while (argc > arg) {
      if (!strcmp(argv[arg], "-p")) {
#if GASNET_PAR
        ++arg;
        if (argc > arg) { pollers = atoi(argv[arg]); arg++; }
        else help = 1;
#else
        if (0 == gex_TM_QueryRank(myteam)) {
          fprintf(stderr, "testcoll %s\n", GASNET_CONFIG_STRING);
          fprintf(stderr, "ERROR: The -p option is only available in the PAR configuration.\n");
          fflush(NULL);
        }
        sleep(1);
        gasnet_exit(1);
#endif
      } else if (argv[arg][0] == '-') {
        help = 1;
        ++arg;
      } else break;
    }

    if (argc > arg) {
      iters = atoi(argv[arg]);
      ++arg;
    }
    if (iters < 1) {
      iters = 1000;
    }

#if GASNET_PAR
    if (argc > arg) {
      threads = atoi(argv[arg]);
      ++arg;
    }
    threads = test_thread_limit(threads);
    if (threads < 1) {
      printf("ERROR: Threads must be between 1 and %d\n", TEST_MAXTHREADS);
      exit(EXIT_FAILURE);
    }
    // NO MULTI-IMAGE SUPPORT IN CURRENT COLLECTIVES
    if (threads > 1) {
      MSG0("WARNING: thread count reduced to 1 (no multi-image support)");
      threads = 1;
    }
#endif

    /* get SPMD info */
    myproc = gex_TM_QueryRank(myteam);
    numprocs = gex_TM_QuerySize(myteam);
    images = numprocs * threads;
    datasize = iters * (3 + 4 * images);

    GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ_REQUEST));

#if GASNET_PAR
  #define USAGE "[options] (iters) (threadcnt)\n" \
                "  The -p option gives the number of polling threads, specified as\n" \
                "    a non-negative integer argument (default is no polling threads).\n"
#else
  #define USAGE "(iters)\n"
#endif
    test_init("testcoll",0,USAGE);

    TEST_SET_WAITMODE(threads + pollers);
    if (argc > arg || help) test_usage();
    
    MSG0("Running coll test(s) with %d iterations.", iters);

    R = test_malloc(iters*sizeof(int));

    /* Number if ints to store */

    /* Carve some variables out of the (aligned) segment: */
    Aw = test_malloc(iters * sizeof(int *));
    Bw = test_malloc(iters * sizeof(int *));
    Cw = test_malloc(iters * sizeof(int *));
    Dw = test_malloc(iters * sizeof(int *));
    Ew = test_malloc(iters * sizeof(int *));
    Fw = test_malloc(iters * sizeof(int *));
    Gw = test_malloc(iters * sizeof(int *));
    A = (int *)TEST_MYSEG();	/* int [1*iters] */
    B = A + 1*iters;		/* int [1*iters] */
    C = B + 1*iters;		/* int [N*iters] */
    D = C + images*iters;	/* int [N*iters] */
    E = D + images*iters;	/* int [1*iters] */
    F = E + 1*iters;		/* int [N*iters] */
    G = F + images*iters;	/* int [N*iters] */
    for (j = 0; j < iters; ++j) {
      Aw[j] = A + j;
      Bw[j] = B + j;
      Cw[j] = C + j*images;
      Dw[j] = D + j*images;
      Ew[j] = E + j;
      Fw[j] = F + j*images;
      Gw[j] = G + j*images;
    }

    /* The unaligned equivalents */
    Av = test_malloc_2D(iters, images * sizeof(int *));
    Bv = test_malloc_2D(iters, images * sizeof(int *));
    Cv = test_malloc_2D(iters, images * sizeof(int *));
    Dv = test_malloc_2D(iters, images * sizeof(int *));
    Ev = test_malloc_2D(iters, images * sizeof(int *));
    Fv = test_malloc_2D(iters, images * sizeof(int *));
    Gv = test_malloc_2D(iters, images * sizeof(int *));
    for (i = 0; i < images; ++i) {
      /* Using (TEST_SEG(n) + n) yields unaligned even when the segments are aligned.
         This is to help catch any case where addresses might have been misused that
         might go undetected if the addresses were aligned */
      A = (int *)TEST_SEG(i/threads) + (i/threads) + datasize*(i%threads);
      B = A + 1*iters;
      C = B + 1*iters;
      D = C + images*iters;
      E = D + images*iters;
      F = E + 1*iters;
      G = F + images*iters;
      for (j = 0; j < iters; ++j) {
	Av[j][i] = A + j;
	Bv[j][i] = B + j;
	Cv[j][i] = C + j*images;
	Dv[j][i] = D + j*images;
	Ev[j][i] = E + j;
	Fv[j][i] = F + j*images;
	Gv[j][i] = G + j*images;
      }
    }

    BARRIER();

#if GASNET_PAR
    MSG("Forking %d gasnet threads (%d active, %d polling)", threads+pollers, threads, pollers);
    {
        int i;
	thread_data_t* tt_thread_data = test_malloc(threads*sizeof(thread_data_t));
        for (i = 0; i < threads; i++) {
	    tt_thread_data[i].myproc = myproc;
	    tt_thread_data[i].local_id = i;
	    tt_thread_data[i].mythread = i + threads * myproc;
	    tt_thread_data[i].peerthread = i + threads * (((myproc ^ 1) == numprocs) ? myproc : (myproc ^ 1));
        }
        test_createandjoin_pthreads(threads, &thread_main, tt_thread_data, sizeof(tt_thread_data[0]));
        test_free(tt_thread_data);
    }
#else
    { thread_data_t td;
      td.myproc = myproc;
      td.local_id = 0;
      td.mythread = myproc;
      td.peerthread = ((myproc ^ 1) == numprocs) ? myproc : (myproc ^ 1);

      thread_main(&td);
    }
#endif



    BARRIER();

    test_free(Aw);
    test_free(Bw);
    test_free(Cw);
    test_free(Dw);

    test_free_2D(Av);
    test_free_2D(Bv);
    test_free_2D(Cv);
    test_free_2D(Dv);

    test_free(R);

    MSG("done.");

    gasnet_exit(0);

    return 0;

}
/* ------------------------------------------------------------------------------------ */
