/*   $Source: /var/local/cvs/gasnet/tests/testvisperf.c,v $
 *     $Date: 2010/04/24 03:52:58 $
 * $Revision: 1.5.44.1 $
 * Description: GASNet VIS performance test
 * Copyright 2006 Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#include "gasnet.h"
#include "gasnet_vis.h"

uintptr_t maxsz = 0;
#ifndef TEST_SEGSZ
  #define TEST_SEGSZ_EXPR (maxsz)
#endif
#include "test.h"

int insegment = 0;
int doputs = 1;
int dogets = 1;
int remotecontig = 0;
int localcontig = 0;
size_t stridelevels = 2;

int myproc;
int numprocs;
int peerproc = -1;
int iamsender = 0;
void *Lbase, *Rbase;

int min_payload = 0, max_payload = 0;
int min_contig = 0, max_contig = 0;
int contigfactor = 8;
int datafactor = 2;
int densitysteps = 4;

typedef enum { TEST_V=0, TEST_I=1, TEST_S=2 } test_vis_t;
int dovis[] = { 1, 1, 1 };
const char *visdesc[] = { "VECTOR", "INDEXED", "STRIDED" };

gasnet_memvec_t *make_vlist(void *baseaddr, size_t stride, size_t cnt, size_t chunksz) {
  gasnet_memvec_t *retval = test_malloc(cnt*sizeof(gasnet_memvec_t));
  size_t i;
  for (i = 0; i < cnt; i++) {
    retval[i].addr = ((char*)baseaddr)+i*stride;
    retval[i].len = chunksz;
  }
  return retval;
}

void * *make_ilist(void *baseaddr, size_t stride, size_t cnt, size_t chunksz) {
  void * *retval = test_malloc(cnt*sizeof(void *));
  size_t i;
  for (i = 0; i < cnt; i++) {
    retval[i] = ((char*)baseaddr)+i*stride;
  }
  return retval;
}

int main(int argc, char **argv) {
  int iters = 0;
  int arg;
  void *alloc = NULL;
  int firstlastmode = 0;
  int fullduplexmode = 0;
  int crossmachinemode = 0;
  int singlesender = 0;
  int help = 0;   

  /* call startup */
  GASNET_Safe(gasnet_init(&argc, &argv));

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
      if (argc > arg) { stridelevels = atoi(argv[arg]); arg++; }
      else help = 1;
    } else if (!strcmp(argv[arg], "-mincontig")) {
      ++arg;
      if (argc > arg) { min_contig = atoi(argv[arg]); arg++; }
      else help = 1;
    } else if (!strcmp(argv[arg], "-maxcontig")) {
      ++arg;
      if (argc > arg) { max_contig = atoi(argv[arg]); arg++; }
      else help = 1;
    } else if (!strcmp(argv[arg], "-contigfactor")) {
      ++arg;
      if (argc > arg) { contigfactor = atoi(argv[arg]); arg++; }
      else help = 1;
    } else if (!strcmp(argv[arg], "-mindata")) {
      ++arg;
      if (argc > arg) { min_payload = atoi(argv[arg]); arg++; }
      else help = 1;
    } else if (!strcmp(argv[arg], "-maxdata")) {
      ++arg;
      if (argc > arg) { max_payload = atoi(argv[arg]); arg++; }
      else help = 1;
    } else if (!strcmp(argv[arg], "-datafactor")) {
      ++arg;
      if (argc > arg) { datafactor = atoi(argv[arg]); arg++; }
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
    } else if (argv[arg][0] == '-') {
      help = 1;
      ++arg;
    } else break;
  }

  if (argc > arg) { iters = atoi(argv[arg]); arg++; }
  if (!iters) iters = 1000;
  if (argc > arg) { TEST_SECTION_PARSE(argv[arg]); arg++; }
  if (min_contig && max_contig && min_contig > max_contig) { ERR("min_contig > max_contig"); help = 1; }
  if (min_payload && max_payload && min_payload > max_payload) { ERR("min_payload > max_payload"); help = 1; }
  if (min_payload && min_contig && min_payload < min_contig) { ERR("min_payload < min_contig"); help = 1; }
  if (max_contig && max_payload && max_contig > max_payload) { ERR("max_contig > max_payload"); help = 1; }
  if (contigfactor < 2) { ERR("contigfactor < 2"); help = 1; }
  if (datafactor < 2) { ERR("datafactor < 2"); help = 1; }

  if (!max_payload) max_payload = 2*1024*1024; /* 2 MB default */
  #ifdef GASNET_SEGMENT_EVERYTHING
    maxsz = gasnet_getMaxGlobalSegmentSize();
  #else
    maxsz = 16*1024*1024;
  #endif
  max_payload = (int)MIN(maxsz, max_payload);
  maxsz = MIN(((uint64_t)max_payload) * densitysteps,maxsz);
  if (!min_contig) min_contig = 8;
  if (!max_contig) max_contig = MIN(256*1024,max_payload);
  if (!min_payload) min_payload = min_contig;

  GASNET_Safe(gasnet_attach(NULL, 0, TEST_SEGSZ_REQUEST, TEST_MINHEAPOFFSET));
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
             "            memory is in the GASNet segment or not (default is not).\n"
             "  -a        enables full-duplex mode, where all nodes send.\n"
             "  -c        enables cross-machine pairing, default is nearest neighbor.\n"
             "  -f        enables 'first/last' mode, where the first/last\n"
             "            nodes communicate with each other, while all other nodes sit idle.");
  if (help || argc > arg) test_usage();

  /* get SPMD info */
  myproc = gasnet_mynode();
  numprocs = gasnet_nodes();

  if (!firstlastmode) {
    /* Only allow 1 or even number for numprocs */
    if (numprocs > 1 && numprocs % 2 != 0) {
      MSG("WARNING: This test requires a unary or even number of nodes. Test skipped.\n");
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
    Lbase = TEST_SEG(myproc);
  } else {
    alloc = test_calloc(maxsz+PAGESZ,1); /* use calloc to prevent valgrind warnings */
    Lbase = alignup_ptr(alloc, PAGESZ); /* ensure page alignment of base */
  }
  assert(((uintptr_t)Lbase) % PAGESZ == 0);

  if (myproc == 0) {
    MSG0("Running %i iterations of %s%s%snon-contiguous put/get%s%s\n local data %s-segment for sizes: %i...%i\n", 
    iters, 
    (firstlastmode ? "first/last " : ""),
    (fullduplexmode ? "full-duplex ": ""),
    (crossmachinemode ? "cross-machine ": ""),
    (remotecontig?"(remotely-contiguous)":""),
    (localcontig?"(locally-contiguous)":""),
    insegment ? "in" : "out", 
    min_payload, max_payload);
    printf("rows are databytes/op : bandwidth values in MB/s\n");
  }
  BARRIER();

  { int contigsz;
    int rawdatasz;
    int isget;
    test_vis_t viscat;
    for (viscat = TEST_V; viscat <= TEST_S; viscat++) {
    for (isget = 0; isget < 2; isget++) {
      if (TEST_SECTION_BEGIN_ENABLED()) {
        if (isget && !dogets) continue;
        if (!isget && !doputs) continue;
        if (!dovis[viscat]) continue;
        for (contigsz = min_contig; contigsz <= max_contig; contigsz *= contigfactor) {
          int di;
          size_t lastdatasz = 0;
          if (contigsz > max_payload) continue;
          if (!myproc) {
            printf("\n%c: %s %s CONTIGSZ = %i\n", TEST_SECTION_NAME(),
                        visdesc[(int)viscat], (isget?"GET":"PUT"), contigsz);
            printf(" density:");
            for (di = 0; di < densitysteps; di++) {
              printf("%8i%%", (int)((densitysteps-di)*100.0/densitysteps));
            }
            printf("\n");
          }
          for (rawdatasz = min_payload; rawdatasz <= max_payload; rawdatasz *= datafactor) {
            char mystr[255];
            size_t datasz = aligndown(rawdatasz,contigsz);
            if (datasz == lastdatasz) continue;
            lastdatasz = datasz;
            if (singlesender) sprintf(mystr,"%8i: ", (int)datasz);
            else  sprintf(mystr,"P%i: %6i: ", myproc, (int)datasz);
            for (di = 0; di < densitysteps; di++) {
              gasnett_tick_t begin, end;
              size_t Lcnt = (localcontig ? 1 : datasz/contigsz);
              size_t Rcnt = (remotecontig? 1 : datasz/contigsz);
              size_t Lsz = datasz/Lcnt;
              size_t Rsz = datasz/Rcnt;
              void **Lilist = NULL;
              void **Rilist = NULL;
              gasnet_memvec_t *Lvlist = NULL;
              gasnet_memvec_t *Rvlist = NULL;
              size_t *Lstrides = NULL;
              size_t *Rstrides = NULL;
              size_t *LRcount = NULL;
              size_t stride = contigsz*(((double)densitysteps)/(densitysteps-di));
              if (stride * MAX(Lcnt,Rcnt) > maxsz) { strcat(mystr,"    -   "); continue; }

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
                    Lstrides = test_malloc(sizeof(size_t)*stridelevels);
                    Rstrides = test_malloc(sizeof(size_t)*stridelevels);
                    LRcount = test_malloc(sizeof(size_t)*(stridelevels+1));
                    LRcount[0] = contigsz;
                    Lstrides[0] = (localcontig ? contigsz : stride);
                    Rstrides[0] = (remotecontig ? contigsz : stride);
                    for (dim = 1; dim < stridelevels; dim++) {
                      size_t factor = 1, fi;
                      for (fi = 1; fi <= chunkcnt/(2*(stridelevels-dim)); fi++) /* choose a reasonable factor */
                        if (chunkcnt/fi*fi == chunkcnt) factor = fi;
                      LRcount[dim] = factor;
                      chunkcnt /= factor;
                      Lstrides[dim] = LRcount[dim]*Lstrides[dim-1];
                      Rstrides[dim] = LRcount[dim]*Rstrides[dim-1];
                    }
                    LRcount[stridelevels] = chunkcnt;
                    { size_t tmp = 1;
                      for (dim = 0; dim <= stridelevels; dim++) tmp *= LRcount[dim];
                      assert(tmp == datasz);
                    }
                    break;
                  }
                }
              }
              #define DOIT(iters) do {                                                           \
                int i;                                                                           \
                switch (viscat) {                                                                \
                  case TEST_V:                                                                   \
                    for (i = 0; i < iters; i++) {                                                \
                      if (isget) gasnet_getv_nbi_bulk(Lcnt,Lvlist,peerproc,Rcnt,Rvlist);         \
                      else gasnet_putv_nbi_bulk(peerproc,Rcnt,Rvlist,Lcnt,Lvlist);               \
                    }                                                                            \
                    break;                                                                       \
                  case TEST_I:                                                                   \
                    for (i = 0; i < iters; i++) {                                                \
                      if (isget) gasnet_geti_nbi_bulk(Lcnt,Lilist,Lsz,peerproc,Rcnt,Rilist,Rsz); \
                      else gasnet_puti_nbi_bulk(peerproc,Rcnt,Rilist,Rsz,Lcnt,Lilist,Lsz);       \
                    }                                                                            \
                    break;                                                                       \
                  case TEST_S:                                                                   \
                    for (i = 0; i < iters; i++) {                                                \
                      if (isget) gasnet_gets_nbi_bulk(Lbase,Lstrides,peerproc,Rbase,Rstrides,    \
                                                      LRcount,stridelevels);                     \
                      else gasnet_puts_nbi_bulk(peerproc,Rbase,Rstrides,Lbase,Lstrides,          \
                                                LRcount,stridelevels);                           \
                    }                                                                            \
                    break;                                                                       \
                }                                                                                \
                gasnet_wait_syncnbi_all();                                                       \
              } while (0)
              if (iamsender) DOIT(1); /* pay some warm-up costs */
              BARRIER();
              if (iamsender) { 
  	        begin = gasnett_ticks_now();
                DOIT(iters);
	        end = gasnett_ticks_now();
              }
              BARRIER();
              if (iamsender) { 
                char tmp[80];
                double secs = gasnett_ticks_to_ns(end - begin)/1.0E9;
                double dataMB = ((double)datasz) * iters / (1024*1024);
                sprintf(tmp, " %8.3f", dataMB / secs);
                strcat(mystr, tmp);
              }
              if (Lilist) test_free(Lilist);
              if (Rilist) test_free(Rilist);
              if (Lvlist) test_free(Lvlist);
              if (Rvlist) test_free(Rvlist);
              if (Lstrides) test_free(Lstrides);
              if (Rstrides) test_free(Rstrides);
              if (LRcount) test_free(LRcount);
            }
            if (iamsender) { printf("%s\n", mystr); fflush(stdout); }
            BARRIER();
          }
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
