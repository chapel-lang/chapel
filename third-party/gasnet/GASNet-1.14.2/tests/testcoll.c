/*   $Source: /var/local/cvs/gasnet/tests/testcoll.c,v $
 *     $Date: 2010/04/16 22:28:15 $
 * $Revision: 1.39.2.1 $
 * Description: GASNet collectives test
 * Copyright 2002-2004, Jaein Jeong and Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include "gasnet.h"
#include "gasnet_coll.h"

#if GASNET_PAR
  #define DEFAULT_THREADS 2
#else
  #define DEFAULT_THREADS 1
#endif

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
  GASNETI_UNUSED int myproc = (randomize(td), td->myproc); \
  int mythread = td->mythread; \
  GASNETI_UNUSED int peerthread = td->peerthread; \
  GASNETI_UNUSED gasnet_coll_handle_t *hndl = td->hndl

static int *R;
static int **Aw, **Bw, **Cw, **Dw, **Ew, **Fw, **Gw;
static int ***Av, ***Bv, ***Cv, ***Dv, ***Ev, ***Fv, ***Gv;

/* All nodes must get the same random sequence */
static void randomize(thread_data_t *td) {
  local_barrier();
  if (td->local_id == 0) {
    int i;
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
#define DEFN(PREFIX, DESC, FLAGS, SUFFIX)                                    \
/* NO/NO - in/out data is not generated/consumed in same barrier phase */    \
void PREFIX##_NONO(int root, thread_data_t *td) {                            \
    PROLOGUE(DESC " NO/NO");                                                 \
    int j;                                                                   \
                                                                             \
    MSG00("Starting %s test", name);                                         \
                                                                             \
    for (j = 0; j < iters; ++j) {                                            \
	gasnet_node_t i;                                                     \
                                                                             \
	*LOCAL(A) = (mythread == root) ? R[j] : -1;                          \
	*LOCAL(B) = mythread;                                                \
	for (i = 0; i < images; ++i) {                                       \
	    LOCAL(D)[i] = i * R[j] + mythread;         \
	}                                                                    \
                                                                             \
	global_barrier();                                                    \
                                                                             \
	CALL(broadcast##SUFFIX, ALL(A), ROOT(A),                             \
	     FLAGS | GASNET_COLL_IN_NOSYNC | GASNET_COLL_OUT_NOSYNC);        \
	CALL(gather##SUFFIX, ROOT(C), ALL(B),                                \
	     FLAGS | GASNET_COLL_IN_NOSYNC | GASNET_COLL_OUT_NOSYNC);        \
	CALL(scatter##SUFFIX, ALL(E), ROOT(D),                               \
	     FLAGS | GASNET_COLL_IN_NOSYNC | GASNET_COLL_OUT_NOSYNC);        \
	CALL(gather_all##SUFFIX, ALL(F), ALL(B),                             \
	     FLAGS | GASNET_COLL_IN_NOSYNC | GASNET_COLL_OUT_NOSYNC);        \
	CALL(exchange##SUFFIX, ALL(G), ALL(D),                               \
	     FLAGS | GASNET_COLL_IN_NOSYNC | GASNET_COLL_OUT_NOSYNC);        \
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
		    MSG("ERROR: %s gather validation failed %d %d", name, LOCAL(C)[i], i);         \
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
    int j;                                                                   \
                                                                             \
    MSG00("Starting %s test", name);                                         \
                                                                             \
    for (j = 0; j < iters; ++j) {                                            \
	gasnet_node_t i;                                                     \
                                                                             \
	*LOCAL(A) = (mythread == root) ? R[j] : -1;                          \
	*LOCAL(B) = mythread;                                                \
                                                                             \
	CALL(broadcast##SUFFIX, ALL(A), ROOT(A),                             \
	     FLAGS | GASNET_COLL_IN_MYSYNC | GASNET_COLL_OUT_MYSYNC);        \
	if (*LOCAL(A) != R[j]) {                                             \
	    MSG("ERROR: %s broadcast validation failed %d %d on %d", name, *LOCAL(A), R[j], mythread);              \
	    gasnet_exit(1);                                                  \
	}                                                                    \
	CALL(gather##SUFFIX, ROOT(C), ALL(B),                                \
	     FLAGS | GASNET_COLL_IN_MYSYNC | GASNET_COLL_OUT_MYSYNC);        \
	if (mythread == root) {                                              \
	    for (i = 0; i < images; ++i) {                                   \
		if (LOCAL(C)[i] != i) {                                      \
		    MSG("ERROR: %s gather validation failed", name);         \
		    gasnet_exit(1);                                          \
		}                                                            \
		LOCAL(C)[i] *= R[j];                                         \
	    }                                                                \
	}                                                                    \
	CALL(scatter##SUFFIX, ALL(B), ROOT(C),                               \
	     FLAGS | GASNET_COLL_IN_MYSYNC | GASNET_COLL_OUT_MYSYNC);        \
	if (*LOCAL(B) != mythread*R[j]) {                                    \
	    MSG("ERROR: %s scatter validation failed", name);                \
	    gasnet_exit(1);                                                  \
	}                                                                    \
	CALL(gather_all##SUFFIX, ALL(C), ALL(B),                             \
	     FLAGS | GASNET_COLL_IN_MYSYNC | GASNET_COLL_OUT_MYSYNC);        \
	for (i = 0; i < images; ++i) {                                       \
	    if (LOCAL(C)[i] != i*R[j]) {                                     \
		MSG("ERROR: %s gather_all validation failed", name);         \
		gasnet_exit(1);                                              \
	    }                                                                \
	    LOCAL(C)[i] += mythread;                                         \
	}                                                                    \
	CALL(exchange##SUFFIX, ALL(D), ALL(C),                               \
	     FLAGS | GASNET_COLL_IN_MYSYNC | GASNET_COLL_OUT_MYSYNC);        \
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
    gasnet_node_t rootproc = root/threads;                                   \
    gasnet_node_t peerproc = peerthread/threads;                             \
    int j;                                                                   \
    int tmp;                                                                 \
                                                                             \
    MSG00("Starting %s test", name);                                         \
                                                                             \
    for (j = 0; j < iters; ++j) {                                            \
	gasnet_node_t i;                                                     \
                                                                             \
	tmp = (peerthread == root) ? R[j] : -1;                              \
	gasnet_put(peerproc, REMOTE(A,peerthread), &tmp, sizeof(int));       \
                                                                             \
	CALL(broadcast##SUFFIX, ALL(A), ROOT(A),                             \
	     FLAGS | GASNET_COLL_IN_ALLSYNC | GASNET_COLL_OUT_ALLSYNC);      \
	gasnet_get(&tmp, peerproc, REMOTE(A,peerthread), sizeof(int));       \
	if (tmp != R[j]) {                                                   \
	    MSG("ERROR: %s broadcast validation failed", name);              \
	    gasnet_exit(1);                                                  \
	}                                                                    \
	tmp = peerthread;                                                    \
	gasnet_put(peerproc, REMOTE(B,peerthread), &tmp, sizeof(int));       \
	CALL(gather##SUFFIX, ROOT(C), ALL(B),                                \
	     FLAGS | GASNET_COLL_IN_ALLSYNC | GASNET_COLL_OUT_ALLSYNC);      \
	gasnet_get_bulk(LOCAL(D), rootproc, REMOTE(C,root), images*sizeof(int)); \
	for (i = 0; i < images; ++i) {                                       \
	    if (LOCAL(D)[i] != i) {                                          \
		MSG("ERROR: %s gather validation failed", name);             \
		gasnet_exit(1);                                              \
	    }                                                                \
	}                                                                    \
	global_barrier(); /* to avoid conflict on D */                       \
	tmp = mythread * R[j];                                               \
	gasnet_put(rootproc, REMOTE(D,root)+mythread, &tmp, sizeof(int));    \
	CALL(scatter##SUFFIX, ALL(B), ROOT(D),                               \
	     FLAGS | GASNET_COLL_IN_ALLSYNC | GASNET_COLL_OUT_ALLSYNC);      \
	gasnet_get(&tmp, peerproc, REMOTE(B,peerthread), sizeof(int));       \
	if (tmp != peerthread*R[j]) {                                        \
	    MSG("ERROR: %s scatter validation failed expected: %d got %d", name, peerthread*R[j], tmp);                \
	    gasnet_exit(1);                                                  \
	}                                                                    \
	global_barrier(); /* to avoid conflict on B */                       \
	tmp = peerthread*R[j] - 1;                                           \
	gasnet_put(peerproc, REMOTE(B,peerthread), &tmp, sizeof(int));       \
	CALL(gather_all##SUFFIX, ALL(C), ALL(B),                             \
	     FLAGS | GASNET_COLL_IN_ALLSYNC | GASNET_COLL_OUT_ALLSYNC);      \
	gasnet_get_bulk(LOCAL(D), peerproc, REMOTE(C,peerthread), images*sizeof(int));\
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
	gasnet_put_bulk(peerproc, REMOTE(D,peerthread), LOCAL(C), images*sizeof(int));\
	CALL(exchange##SUFFIX, ALL(C), ALL(D),                               \
	     FLAGS | GASNET_COLL_IN_ALLSYNC | GASNET_COLL_OUT_ALLSYNC);      \
	gasnet_get_bulk(LOCAL(D), peerproc, REMOTE(C,peerthread), images*sizeof(int));\
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
    gasnet_node_t i;                                                         \
    int j;                                                                   \
                                                                             \
    MSG00("Starting %s test", name);                                         \
                                                                             \
    for (j = 0; j < iters; ++j) {                                            \
	*LOCALi(j,A) = (mythread == root) ? R[j] : -1;                       \
	*LOCALi(j,B) = mythread;                                             \
	hndl[j] = CALL(broadcast##SUFFIX##_nb, ALLi(j,A), ROOTi(j,A),        \
	     FLAGS | GASNET_COLL_IN_MYSYNC | GASNET_COLL_OUT_MYSYNC);        \
    }                                                                        \
    for (j = 0; j < iters; ++j) {                                            \
        gasnet_coll_wait_sync(hndl[j]);                                      \
	if (*LOCALi(j,A) != R[j]) {                                          \
	    MSG("ERROR: %s broadcast validation failed", name);              \
	    gasnet_exit(1);                                                  \
	}                                                                    \
	hndl[j] = CALL(gather##SUFFIX##_nb, ROOTi(j,C), ALLi(j,B),           \
	     FLAGS | GASNET_COLL_IN_MYSYNC | GASNET_COLL_OUT_MYSYNC);        \
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
	hndl[j] = CALL(scatter##SUFFIX##_nb, ALLi(j,B), ROOTi(j,C),          \
	     FLAGS | GASNET_COLL_IN_MYSYNC | GASNET_COLL_OUT_MYSYNC);        \
    }                                                                        \
    for (j = 0; j < iters; ++j) {                                            \
        gasnet_coll_wait_sync(hndl[j]);                                      \
	if (*LOCALi(j,B) != mythread*R[j]) {                                 \
	    MSG("ERROR: %s scatter validation failed", name);                \
	    gasnet_exit(1);                                                  \
	}                                                                    \
	hndl[j] = CALL(gather_all##SUFFIX##_nb, ALLi(j,C), ALLi(j,B),        \
	     FLAGS | GASNET_COLL_IN_MYSYNC | GASNET_COLL_OUT_MYSYNC);        \
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
	hndl[j] = CALL(exchange##SUFFIX##_nb, ALLi(j,D), ALLi(j,C),          \
	     FLAGS | GASNET_COLL_IN_MYSYNC | GASNET_COLL_OUT_MYSYNC);        \
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

#define gasnet_coll_broadcastEMPTY	gasnet_coll_broadcast
#define gasnet_coll_gatherEMPTY		gasnet_coll_gather
#define gasnet_coll_scatterEMPTY	gasnet_coll_scatter
#define gasnet_coll_gather_allEMPTY	gasnet_coll_gather_all
#define gasnet_coll_exchangeEMPTY	gasnet_coll_exchange

#define gasnet_coll_broadcastEMPTY_nb	gasnet_coll_broadcast_nb
#define gasnet_coll_gatherEMPTY_nb	gasnet_coll_gather_nb
#define gasnet_coll_scatterEMPTY_nb	gasnet_coll_scatter_nb
#define gasnet_coll_gather_allEMPTY_nb	gasnet_coll_gather_all_nb
#define gasnet_coll_exchangeEMPTY_nb	gasnet_coll_exchange_nb

#define ALL(X)		ALLi(0,X)
#define ROOT(X)		ROOTi(0,X)
#define REMOTE(X,N)	REMOTEi(0,X,N)
#define LOCAL(X)	LOCALi(0,X)
#define LOCALi(i,X)	REMOTEi(i,X,mythread)

#define ALLi(i,X)	X##w[i]
#define ROOTi(i,X)	root, X##w[i]
#define REMOTEi(i,X,N)	(X##w[i])
DEFN(testSS, "SINGLE/single-addr", GASNET_COLL_SINGLE, EMPTY)
#undef ALLi
#undef ROOTi
#undef REMOTEi

#define ALLi(i,X)	(void*const*)X##v[i]
#define ROOTi(i,X)	root, X##v[i][root]
#define REMOTEi(i,X,N)	(X##v[i][N])
DEFN(testSM, "SINGLE/multi-addr", GASNET_COLL_SINGLE, M)
#undef ALLi
#undef ROOTi
#undef REMOTEi

#define ALLi(i,X)	X##v[i][mythread]
#define ROOTi(i,X)	root, (mythread==root)?X##v[i][root]:NULL
#define REMOTEi(i,X,N)	(X##v[i][N])
DEFN(testLS, "LOCAL/single-addr", GASNET_COLL_LOCAL, EMPTY)
#undef ALLi
#undef ROOTi
#undef REMOTEi

#define ALLi(i,X)	(void*const*)(X##v[i]+(myproc*threads))
#define ROOTi(i,X)	root, (myproc==(root/threads))?X##v[i][root]:NULL
#define REMOTEi(i,X,N)	(X##v[i][N])
DEFN(testLM, "LOCAL/multi-addr", GASNET_COLL_LOCAL, M)
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

void *thread_main(void *arg) {
  thread_data_t *td = arg;
  int i;
                
#if GASNET_PAR
  gasnet_image_t *imagearray = test_malloc(numprocs * sizeof(gasnet_image_t));
  for (i=0; i<numprocs; ++i) { imagearray[i] = threads; }
  gasnet_coll_init(imagearray, td->mythread, NULL, 0, 0);
  test_free(imagearray);
#else
  gasnet_coll_init(NULL, 0, NULL, 0, 0);
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

    if (threads > 1) {
      MSG00("Skipping SINGLE/single-addr tests (multiple threads)");
    } else if (!TEST_ALIGNED_SEGMENTS()) {
      MSG00("Skipping SINGLE/single-addr tests (unaligned segments)");
    } else {
      testSS_NONO(root, td);
      testSS_MYMY(root, td);
      testSS_ALLALL(root, td);
      testSS_NB(root, td);
    }
    testSM_NONO(root, td);
    testSM_MYMY(root, td);
    testSM_ALLALL(root, td);
    testSM_NB(root, td);

#if 1
    testLS_NONO(root, td);
    testLS_MYMY(root, td);
    testLS_ALLALL(root, td);
    testLS_NB(root, td);

    testLM_NONO(root, td);
    testLM_MYMY(root, td);
    testLM_ALLALL(root, td);
    testLM_NB(root, td);
#endif
  }
  
  test_free(td->hndl);

  return NULL;
}

int main(int argc, char **argv)
{
    static int *A, *B, *C, *D, *E, *F, *G;
    gasnet_node_t myproc, i;
    int j;
   
    /* call startup */
    GASNET_Safe(gasnet_init(&argc, &argv));

    if (argc > 1) {
      iters = atoi(argv[1]);
    }
    if (iters < 1) {
      iters = 1000;
    }

#if GASNET_PAR
    if (argc > 2) {
      threads = atoi(argv[2]);
    }
    if (threads > TEST_MAXTHREADS || threads < 1) {
      printf("ERROR: Threads must be between 1 and %d\n", TEST_MAXTHREADS);
      exit(EXIT_FAILURE);
    }
#endif

    /* get SPMD info */
    myproc = gasnet_mynode();
    numprocs = gasnet_nodes();
    images = numprocs * threads;
    datasize = iters * (3 + 4 * images);

    GASNET_Safe(gasnet_attach(NULL, 0, TEST_SEGSZ_REQUEST, TEST_MINHEAPOFFSET));
    test_init("testcoll",0,"(iters) (threadcnt)");
    TEST_SET_WAITMODE(threads);
    if (argc > 3) test_usage();
    
    MSG0("Running coll test(s) with %d iterations.", iters);

    R = test_malloc(iters*sizeof(int));
    TEST_SRAND(1);

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
    MSG("Forking %d gasnet threads", threads);
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
