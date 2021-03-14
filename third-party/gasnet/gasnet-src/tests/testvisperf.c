/*   $Source: bitbucket.org:berkeleylab/gasnet.git/tests/testvisperf.c $
 * Description: GASNet VIS performance test
 * Copyright 2006 Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include <gasnetex.h>
#include "gasnet_vis.h"

uintptr_t maxsz = 0;
size_t areasz = 0;
#ifndef TEST_SEGSZ
  #define TEST_SEGSZ_EXPR (maxsz)
#endif
#include "test.h"

static gex_Client_t      myclient;
static gex_EP_t    myep;
static gex_TM_t myteam;
static gex_Segment_t     mysegment;

int insegment = 1;
int doputs = 1;
int dogets = 1;
int doNB = 0;
int remotecontig = 0;
int localcontig = 0;
size_t stridelevels = 2;

int myproc;
int numprocs;
int peerproc = -1;
int iamsender = 0;
void *Lbase, *Rbase;
gex_Event_t *evts = NULL;
size_t evtcnt = 0;

size_t min_payload = 0, max_payload = 0;
size_t min_contig = 0, max_contig = 0;
double contigfactor = 8;
double datafactor = 2;
int densitysteps = 4;

typedef enum { TEST_V=0, TEST_I=1, TEST_S=2 } test_vis_t;
int dovis[] = { 1, 1, 1 };
const char *visdesc[] = { "VECTOR", "INDEXED", "STRIDED" };

gex_Memvec_t *make_vlist(void *baseaddr, size_t stride, size_t cnt, size_t chunksz) {
  gex_Memvec_t *retval = test_malloc(cnt*sizeof(gex_Memvec_t));
  if (cnt > 1) assert(stride >= chunksz);
  for (size_t i = 0; i < cnt; i++) {
    retval[i].gex_addr = ((char*)baseaddr)+i*stride;
    retval[i].gex_len = chunksz;
  }
  return retval;
}

void * *make_ilist(void *baseaddr, size_t stride, size_t cnt, size_t chunksz) {
  void * *retval = test_malloc(cnt*sizeof(void *));
  if (cnt > 1) assert(stride >= chunksz);
  for (size_t i = 0; i < cnt; i++) {
    retval[i] = ((char*)baseaddr)+i*stride;
  }
  return retval;
}

int main(int argc, char **argv) {
  long iters = 0;
  int arg;
  void *alloc = NULL;
  int firstlastmode = 0;
  int fullduplexmode = 0;
  int crossmachinemode = 0;
  int singlesender = 0;
  int help = 0;   

  /* call startup */
  GASNET_Safe(gex_Client_Init(&myclient, &myep, &myteam, "testvisperf", &argc, &argv, 0));

  /* parse arguments */
  arg = 1;
  while (argc > arg) {
    if (!strcmp(argv[arg], "-in")) {
      insegment = 1;
      ++arg;
    } else if (!strcmp(argv[arg], "-out")) {
      insegment = 0;
      ++arg;
    } else if (!strcmp(argv[arg], "-sl")) {
      ++arg;
      if (argc > arg) { stridelevels = atol(argv[arg]); arg++; }
      else help = 1;
    } else if (!strcmp(argv[arg], "-mincontig")) {
      ++arg;
      if (argc > arg) { min_contig = atol(argv[arg]); arg++; }
      else help = 1;
    } else if (!strcmp(argv[arg], "-maxcontig")) {
      ++arg;
      if (argc > arg) { max_contig = atol(argv[arg]); arg++; }
      else help = 1;
    } else if (!strcmp(argv[arg], "-contigfactor")) {
      ++arg;
      if (argc > arg) { contigfactor = atof(argv[arg]); arg++; }
      else help = 1;
    } else if (!strcmp(argv[arg], "-mindata")) {
      ++arg;
      if (argc > arg) { min_payload = atol(argv[arg]); arg++; }
      else help = 1;
    } else if (!strcmp(argv[arg], "-maxdata")) {
      ++arg;
      if (argc > arg) { max_payload = atol(argv[arg]); arg++; }
      else help = 1;
    } else if (!strcmp(argv[arg], "-datafactor")) {
      ++arg;
      if (argc > arg) { datafactor = atof(argv[arg]); arg++; }
      else help = 1;
    } else if (!strcmp(argv[arg], "-densitysteps")) {
      ++arg;
      if (argc > arg) { densitysteps = atoi(argv[arg]); arg++; }
      else help = 1;
    } else if (!strcmp(argv[arg], "-f")) {
      firstlastmode = 1;
      ++arg;
    } else if (!strcmp(argv[arg], "-c")) {
      crossmachinemode = 1;
      ++arg;
    } else if (!strcmp(argv[arg], "-a")) {
      fullduplexmode = 1;
      ++arg;
    } else if (!strcmp(argv[arg], "-p")) {
      dogets = 0; doputs = 1;
      ++arg;
    } else if (!strcmp(argv[arg], "-g")) {
      dogets = 1; doputs = 0;
      ++arg;
    } else if (!strcmp(argv[arg], "-r")) {
      remotecontig = 1;
      ++arg;
    } else if (!strcmp(argv[arg], "-l")) {
      localcontig = 1;
      ++arg;
    } else if (!strcmp(argv[arg], "-nb")) {
      doNB = 1;
      ++arg;
    } else if (!strcmp(argv[arg], "-nbi")) {
      doNB = 0;
      ++arg;
    } else if (argv[arg][0] == '-') {
      help = 1;
      ++arg;
    } else break;
  }

  const char *iterstr = "0.1"; // default iters
  if (argc > arg) { 
    iterstr = argv[arg];
    arg++; 
  }
  if (argc > arg) { TEST_SECTION_PARSE(argv[arg]); arg++; }
  if (min_contig && max_contig && min_contig > max_contig) { ERR("min_contig > max_contig"); help = 1; }
  if (min_payload && max_payload && min_payload > max_payload) { ERR("min_payload > max_payload"); help = 1; }
  if (min_payload && min_contig && min_payload < min_contig) { ERR("min_payload < min_contig"); help = 1; }
  if (max_contig && max_payload && max_contig > max_payload) { ERR("max_contig > max_payload"); help = 1; }
  if (contigfactor <= 1) { ERR("contigfactor <= 1"); help = 1; }
  if (datafactor <= 1) { ERR("datafactor <= 1"); help = 1; }

  if (!max_payload) max_payload = 2*1024*1024; /* 2 MB default */
  areasz = gasnet_getMaxGlobalSegmentSize();
  if (insegment) areasz = aligndown(areasz/2,PAGESZ);
  max_payload = MIN(areasz, max_payload);
  areasz = aligndown(MIN(max_payload * densitysteps,areasz),PAGESZ);
  if (insegment) maxsz = areasz*2;
  else           maxsz = areasz;
  if (!min_contig) min_contig = 8;
  if (!max_contig) max_contig = MIN(256*1024,max_payload);
  if (!min_payload) min_payload = min_contig;

  GASNET_Safe(gex_Segment_Attach(&mysegment, myteam, TEST_SEGSZ_REQUEST));
  test_init("testvisperf",1, "[options] (iters) (test_sections)\n"
             "  -p/-g     selects puts only or gets only (default is both).\n"
             "  -r/-l     selects remotely contiguous or locally contiguous (default is neither).\n"
             "  -mindata/-maxdata <sz>   \n"
             "            selects sz as min/max data payload per operation.\n"
             "  -mincontig/-maxcontig <sz>   \n"
             "            selects sz as min/max contig size.\n"
             "  -datafactor/-contigfactor <f>   \n"
             "            selects f as growth factor for data/contig sizes.\n"
             "  -densitysteps <d>   \n"
             "            selects d density steps, inclusive from 100%..100/d%\n"
             "  -sl <n>   selects n striding levels (default is 2).\n"
             "  -in/-out  selects whether the initiator-side\n"
             "            memory is in the GASNet segment or not (default is in-segment).\n"
             "  -a        enables full-duplex mode, where all nodes send.\n"
             "  -c        enables cross-machine pairing, default is nearest neighbor.\n"
             "  -f        enables 'first/last' mode, where the first/last\n"
             "            nodes communicate with each other, while all other nodes sit idle.\n"
             "  -nbi      use NBI-mode synchronization (default)\n"
             "  -nb       use NB-mode synchronization"
             );
  if (help || argc > arg) test_usage();

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
  } else if (numprocs == 1) {
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
  } else { 
    peerproc = (myproc % 2) ? (myproc - 1) : (myproc + 1);
    iamsender = (fullduplexmode || myproc % 2 == 0);
  }
  singlesender = (numprocs == 1) || ((numprocs == 2 || firstlastmode) && !fullduplexmode);

  Rbase = TEST_SEG(peerproc);

  if (insegment) {
    Lbase = (uint8_t*)TEST_SEG(myproc) + areasz;
    Lbase = alignup_ptr(Lbase, PAGESZ); /* ensure page alignment of base */
  } else {
    alloc = test_calloc(areasz+PAGESZ,1); /* use calloc to prevent valgrind warnings */
    Lbase = alignup_ptr(alloc, PAGESZ); /* ensure page alignment of base */
  }
  assert(((uintptr_t)Lbase) % PAGESZ == 0);

  if (strchr(iterstr,'.')) TEST_ASI_INIT(iterstr);
  else iters = atol(iterstr); 

  if (myproc == 0) {
    MSG0("Running %s iterations of %s%s%snon-contiguous put/get %s %s%s\n"
         "   local data %s-segment for payload sizes: %" PRIuSZ "...%" PRIuSZ "  (datafactor=%.2f)\n"
         "   contig sizes: %" PRIuSZ "...%" PRIuSZ "  (contigfactor=%.2f)\n"
         "   densitysteps=%i stridelevels=%" PRIuSZ 
         , 
    iterstr, 
    (firstlastmode ? "first/last " : ""),
    (fullduplexmode ? "full-duplex ": ""),
    (crossmachinemode ? "cross-machine ": ""),
    (doNB?"NB":"NBI"),
    (remotecontig?"(remotely-contiguous)":""),
    (localcontig?"(locally-contiguous)":""),
    insegment ? "in" : "out", 
    min_payload, max_payload, datafactor,
    min_contig, max_contig, contigfactor,
    densitysteps, stridelevels);

    // estimate total test run time
   if (strchr(iterstr,'.')) {
    int measurements = 0;
    char sec = 'A';
    for (test_vis_t viscat = TEST_V; viscat <= TEST_S; viscat++) {
      for (int isget = 0; isget < 2; isget++) {
        if (!strchr(test_sections,sec++) && test_sections[0]) continue;
        if (isget && !dogets) continue;
        if (!isget && !doputs) continue;
        if (!dovis[viscat]) continue;
        size_t lastcontigsz = 0;
        for (double rawcontigsz = min_contig; rawcontigsz <= max_contig; rawcontigsz *= contigfactor) {
          size_t contigsz = (size_t)rawcontigsz;
          if (contigsz == lastcontigsz) continue;
          if (contigsz > max_payload) continue;
          lastcontigsz = contigsz;
          size_t lastdatasz = 0;
          for (double rawdatasz = min_payload; rawdatasz <= max_payload; rawdatasz *= datafactor) {
            size_t datasz = aligndown((size_t)rawdatasz,contigsz);
            if (datasz == lastdatasz) continue;
            lastdatasz = datasz;
            for (int di = 0; di < densitysteps; di++) {
              measurements++;
            }
          }
        }
      }
    }
    double ttime = 2 * MAX(TEST_ASI_MININTERVAL,atof(iterstr)) * measurements;
    MSG0("ESTIMATED test run time: ~ %d sec (%0.1f min)",(int)ttime,ttime/60);
   }
   MSG0("rows are databytes/op : bandwidth values in MiB/s");
  }
  BARRIER();

  for (test_vis_t viscat = TEST_V; viscat <= TEST_S; viscat++) {
    for (int isget = 0; isget < 2; isget++) {
      if (TEST_SECTION_BEGIN_ENABLED()) {
        if (isget && !dogets) continue;
        if (!isget && !doputs) continue;
        if (!dovis[viscat]) continue;
        size_t lastcontigsz = 0;
        for (double rawcontigsz = min_contig; rawcontigsz <= max_contig; rawcontigsz *= contigfactor) {
          size_t contigsz = (size_t)rawcontigsz;
          if (contigsz == lastcontigsz) continue;
          if (contigsz > max_payload) continue;
          lastcontigsz = contigsz;
          TEST_ASI_NEW_TEST();
          if (!myproc) {
            printf("\n%c: %s %s CONTIGSZ = %"PRIuSZ"\n", TEST_SECTION_NAME(),
                        visdesc[(int)viscat], (isget?"GET":"PUT"), contigsz);
            printf(" density:");
            for (int di = 0; di < densitysteps; di++) {
              printf("%8i%%", (int)((densitysteps-di)*100.0/densitysteps));
            }
            printf("\n");
          }
          size_t lastdatasz = 0;
          for (double rawdatasz = min_payload; rawdatasz <= max_payload; rawdatasz *= datafactor) {
            size_t datasz = aligndown((size_t)rawdatasz,contigsz);
            if (datasz == lastdatasz) continue;
            lastdatasz = datasz;
            char mystr[255];
            if (singlesender) snprintf(mystr, sizeof(mystr), "%8"PRIuSZ": ", datasz);
            else  snprintf(mystr, sizeof(mystr), "P%i: %6"PRIuSZ": ", myproc, datasz);
            for (int di = 0; di < densitysteps; di++) {
              gasnett_tick_t begin=0, end=0;
              size_t Lcnt = (localcontig ? 1 : datasz/contigsz);
              size_t Rcnt = (remotecontig? 1 : datasz/contigsz);
              size_t Lsz = datasz/Lcnt;
              size_t Rsz = datasz/Rcnt;
              void **Lilist = NULL;
              void **Rilist = NULL;
              gex_Memvec_t *Lvlist = NULL;
              gex_Memvec_t *Rvlist = NULL;
              ptrdiff_t *Lstrides = NULL;
              ptrdiff_t *Rstrides = NULL;
              size_t *_LRcount = NULL;
              size_t *LRcount = NULL;
              size_t stride = contigsz*(((double)densitysteps)/(densitysteps-di));
              assert(stride >= contigsz);
              if (stride * MAX(Lcnt,Rcnt) > areasz) { strcat(mystr,"    -   "); continue; }

              if (iamsender) { /* setup metadata */
                switch (viscat) {
                  case TEST_V: 
                    Lvlist = make_vlist(Lbase, stride, Lcnt, Lsz);
                    Rvlist = make_vlist(Rbase, stride, Rcnt, Rsz);
                    break;
                  case TEST_I: 
                    Lilist = make_ilist(Lbase, stride, Lcnt, Lsz);
                    Rilist = make_ilist(Rbase, stride, Rcnt, Rsz);
                    break;
                  case TEST_S: {
                    size_t chunkcnt = datasz/contigsz;
                    int dim;
                    Lstrides = test_malloc(sizeof(ptrdiff_t)*stridelevels);
                    Rstrides = test_malloc(sizeof(ptrdiff_t)*stridelevels);
                    _LRcount = test_malloc(sizeof(size_t)*(stridelevels+1));
                    _LRcount[0] = contigsz; // temporary using legacy count format for convenience
                    Lstrides[0] = (localcontig ? contigsz : stride);
                    Rstrides[0] = (remotecontig ? contigsz : stride);
                    for (dim = 1; dim < stridelevels; dim++) {
                      size_t factor = 1;
                      for (size_t fi = 1; fi <= chunkcnt/(2*(stridelevels-dim)); fi++) /* choose a reasonable factor */
                        if (chunkcnt/fi*fi == chunkcnt) factor = fi;
                      _LRcount[dim] = factor;
                      chunkcnt /= factor;
                      Lstrides[dim] = _LRcount[dim]*Lstrides[dim-1];
                      Rstrides[dim] = _LRcount[dim]*Rstrides[dim-1];
                    }
                    _LRcount[stridelevels] = chunkcnt;
                    LRcount = _LRcount+1;

                    { size_t tmp = contigsz;
                      for (dim = 0; dim < stridelevels; dim++) tmp *= LRcount[dim];
                      assert(tmp == datasz);
                    }
                    break;
                  }
                }
              }
              #define DOIT(iters) do {                                                           \
                if (doNB && iters > evtcnt) {                                                    \
                  test_free(evts);                                                               \
                  evts = test_malloc(iters * sizeof(gex_Event_t));                               \
                  evtcnt = iters;                                                                \
                }                                                                                \
                switch (viscat) {                                                                \
                  case TEST_V:                                                                   \
                    if (doNB) {                                                                  \
                      if (isget) for (long i = 0; i < iters; i++)                                \
                        evts[i] = gex_VIS_VectorGetNB(myteam,Lcnt,Lvlist,peerproc,Rcnt,Rvlist,0);\
                      else for (long i = 0; i < iters; i++)                                      \
                        evts[i] = gex_VIS_VectorPutNB(myteam,peerproc,Rcnt,Rvlist,Lcnt,Lvlist,0);\
                    } else {                                                                     \
                      if (isget) for (long i = 0; i < iters; i++)                                \
                        gex_VIS_VectorGetNBI(myteam,Lcnt,Lvlist,peerproc,Rcnt,Rvlist,0);         \
                      else for (long i = 0; i < iters; i++)                                      \
                        gex_VIS_VectorPutNBI(myteam,peerproc,Rcnt,Rvlist,Lcnt,Lvlist,0);         \
                    }                                                                            \
                    break;                                                                       \
                  case TEST_I:                                                                   \
                    if (doNB) {                                                                  \
                      if (isget) for (long i = 0; i < iters; i++)                                \
                        evts[i] =                                                                \
                         gex_VIS_IndexedGetNB(myteam,Lcnt,Lilist,Lsz,peerproc,Rcnt,Rilist,Rsz,0);\
                      else for (long i = 0; i < iters; i++)                                      \
                        evts[i] =                                                                \
                         gex_VIS_IndexedPutNB(myteam,peerproc,Rcnt,Rilist,Rsz,Lcnt,Lilist,Lsz,0);\
                    } else {                                                                     \
                      if (isget) for (long i = 0; i < iters; i++)                                \
                        gex_VIS_IndexedGetNBI(myteam,Lcnt,Lilist,Lsz,peerproc,Rcnt,Rilist,Rsz,0);\
                      else for (long i = 0; i < iters; i++)                                      \
                        gex_VIS_IndexedPutNBI(myteam,peerproc,Rcnt,Rilist,Rsz,Lcnt,Lilist,Lsz,0);\
                    }                                                                            \
                    break;                                                                       \
                  case TEST_S:                                                                   \
                    if (doNB) {                                                                  \
                      if (isget) for (long i = 0; i < iters; i++)                                \
                        evts[i] =                                                                \
                         gex_VIS_StridedGetNB(myteam,Lbase,Lstrides,peerproc,Rbase,Rstrides,     \
                                                      contigsz,LRcount,stridelevels,0);          \
                      else for (long i = 0; i < iters; i++)                                      \
                        evts[i] =                                                                \
                         gex_VIS_StridedPutNB(myteam,peerproc,Rbase,Rstrides,Lbase,Lstrides,     \
                                                contigsz,LRcount,stridelevels,0);                \
                    } else {                                                                     \
                      if (isget) for (long i = 0; i < iters; i++)                                \
                        gex_VIS_StridedGetNBI(myteam,Lbase,Lstrides,peerproc,Rbase,Rstrides,     \
                                                      contigsz,LRcount,stridelevels,0);          \
                      else for (long i = 0; i < iters; i++)                                      \
                        gex_VIS_StridedPutNBI(myteam,peerproc,Rbase,Rstrides,Lbase,Lstrides,     \
                                                contigsz,LRcount,stridelevels,0);                \
                    }                                                                            \
                    break;                                                                       \
                }                                                                                \
                if (doNB) gex_Event_WaitAll(evts, iters, 0);                                     \
                else gex_NBI_Wait(GEX_EC_ALL,0);                                                 \
              } while (0)
              if (iamsender) DOIT(1); /* pay some warm-up costs */
              BARRIER();
              if (iamsender) { 
                TEST_ASI_BEGIN(name,iters,datasz,MIN(di,TEST_ASI_BANKS-1));
  	        begin = gasnett_ticks_now();
                DOIT(iters);
	        end = gasnett_ticks_now();
                TEST_ASI_END(name,iters);
              }
              BARRIER();
              if (iamsender) { 
                char tmp[80];
                double secs = gasnett_ticks_to_ns(end - begin)/1.0E9;
                double dataMB = ((double)datasz) * iters / (1024*1024);
                snprintf(tmp, sizeof(tmp), " %8.3f", dataMB / secs);
                strcat(mystr, tmp);
              }
              if (Lilist) test_free(Lilist);
              if (Rilist) test_free(Rilist);
              if (Lvlist) test_free(Lvlist);
              if (Rvlist) test_free(Rvlist);
              if (Lstrides) test_free(Lstrides);
              if (Rstrides) test_free(Rstrides);
              if (_LRcount) test_free(_LRcount);
            }
            if (iamsender) { printf("%s\n", mystr); fflush(stdout); }
            BARRIER();
          }
        }
      }
    }
  }

  BARRIER();
  if (alloc) test_free(alloc);

  gasnet_exit(0);

  return 0;
}


/* ------------------------------------------------------------------------------------ */
