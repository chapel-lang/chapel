/*   $Source: /var/local/cvs/gasnet/tests/mpi/testmpinbr.c,v $
 *     $Date: 2005/07/23 01:40:09 $
 * $Revision: 1.6 $
 * Description: MG-like neighbor exchange
 * Copyright 2005, Christian Bell <csbell@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

/************************************************************
 * testmpineighbor.c:
 *   NAS MG modelled microbenchmark to measure the cost of neighbor ghost cell
 *   exchanges.  The benchmark replicates ghost exchanges over all dimensions
 *   (two of which generate strided data communication).
 *
*************************************************************/

#include "portable_inttypes.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/param.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <string.h>

#include <mpi.h>
#include <assert.h>

typedef struct {
	int datasize;
	int iters;
	int dims;
	int axis;
	int category;
	uint64_t time;
	double max_throughput;
} stat_struct_t;

uintptr_t topalloc = 0;
FILE *nbr_fp;
int maxlevel = 4;
int myproc;
int nprocs;
int mympitag;
int peermpitag;

#define POWER_OF_TWO(P)	(((P)&((P)-1)) == 0)

#define PX_SZ (nb->dims2*nb->dims1)
#define PY_SZ (nb->dims2*nb->dims0)
#define PZ_SZ (nb->dims1*nb->dims0)

#define AX	0
#define AY	1
#define AZ	2
#define AALL	3

#define LINEARIZEPROC(nb,k,j,i) (k*(nb)->procGrid[1]*(nb)->procGrid[0] + \
		                j*(nb)->procGrid[0] + i)

#define GHOST_TYPE_PUT	    0
#define GHOST_TYPE_AMLONG   1

#define GHOST_DIR_UPPER 0
#define GHOST_DIR_LOWER 1

#ifdef DEBUG
  #define DEBUGMSG(s) do { \
    printf("P%i: %s\n", rank, s); fflush(stdout); \
  } while(0)
#else
  #define DEBUGMSG(s)
#endif

#define MPI_SAFE(fncall) do {     \
   int retcode = (fncall);        \
   if (retcode != MPI_SUCCESS) {  \
     fprintf(stderr, "MPI Error: %s\n  returned error code %i\n", #fncall, retcode); \
     abort();                     \
   }                              \
 } while (0)

/*
 * Memory requirements for this test differ according to the type of ghost
 * exchange being performed.  They all have the per-processor data grid size
 * memory requirement based on the level of the grid.
 *
 * Over all axes x,y,z planes along the z axis (xy) planes are contiguous in
 * memory.  This requires exchanges along the x and y axes to have some form 
 * of non-contiguous communication.
 *
 *
 * The following types of ghost exchanges are done:
 *
 * 1. UPC (Parry's MG)
 *    * Over each dimension, pack boundary plane in a buffer, send the buffer
 *      and signal the neighbor with a put.
 *    * Each processor spins on the signal waiting to unpack the buffer back
 *      into local computation data.
 *    * Memory reqs: 
 *
 * 2. Non-blocking GASNet (similar to UPC version)
 *
 * 2. GASNet Active-Messages, AMLong
 *    * Pack into buffer send AMLong.
 *    * Receiver runs handler, unpacks the data and increments a counter
 */

typedef
struct _nbr_t {
    int	 dimsz;	/* global dimension size */
    /* 0 => yz plane
     * 1 => xz plane
     * 2 => xy plane (contiguous)
     */
    int  procGrid[3];
    int  idGrid[3];

    /* Upper and Lower neighbors in each dimension (grid id) */
    int  idGridUpper[3];
    int  idGridLower[3];

    /* Upper and Lower neighbors in each dimension (GASNet node ids) */
    int  nodeidUpper[3];
    int  nodeidLower[3];

    /* Cache dims in all Lower neighbors */
    int  dimsLower[3];

    /* blocks per grid element in each dimension */
    int  elemsPerDim;
    int  dims0;
    int  dims1;
    int  dims2;
    int  dims[3];
    int  facesz[3]; /* Face sz for each axis in elements */

    uintptr_t totalSize;

    /* Different mechanisms for Ghost exchanges */
    double	*Ldata;	    /* Local computation data */
    double	*Dir;	    /* Remote computation data, flattened 3d cube */
    double	*Diryz;	    /* Target for yz boundary exchanges */
    double	*Dirxz;	    /* Target for xz boundary exchanges */
    double	*Dirxy;	    /* Target for xy boundary exchanges */


    /* Arrays into communicaiton buffers, for low/up neighbor in each dim */
    double    *dimBufs[3][2];

    /* Three local communication buffers for non-contiguous planes */
    double *xyCommBuffer;
    double *yzCommBuffer;
    double *xzCommBuffer;

    /* For computing medians at node 0 */
    stat_struct_t   *stats0;
}
nbr_t;

/* Only one-level for now */
nbr_t	Nbr;

#define AREF(nb,k,j,i) (nb->Ldata[(k)*(nb)->dims1*(nb)->dims0 + \
		                  (j)*(nb)->dims0 + i])

void setupGrid(nbr_t *nb, int level);
void allocMultiGrid(nbr_t *nb);
void estimateMemSegment(nbr_t *nb, uintptr_t *local, uintptr_t *segment);

/*
 * MPI-style ghost exchange (as in NAS MG 2.4)
 */
void ghostExchMPI(nbr_t *nb, int iters, int axis);
void ge_take3(nbr_t *nb, double *src, size_t destp, int axis, MPI_Request *req);
MPI_Request *ge_post3(nbr_t *nb, int dir, int axis, MPI_Request *req);
void ge_give3(nbr_t *nb, int dir, int axis, MPI_Request *req);

void BARRIER() {
  DEBUGMSG("entering barrier");
  MPI_SAFE(MPI_Barrier(MPI_COMM_WORLD));
  DEBUGMSG("leaving barrier");
}

int64_t getMicrosecondTimeStamp() {
  int64_t retval;
  struct timeval tv;
  if (gettimeofday(&tv, NULL)) {
    perror("gettimeofday");
    abort();
  }
  retval = ((int64_t)tv.tv_sec) * 1000000 + tv.tv_usec;
  return retval;
}

#define TIME()	getMicrosecondTimeStamp()


void init_stat(nbr_t *nb, stat_struct_t *st, int axis, int dims, int sz)
{
	st->iters = 0;
	st->dims = dims;
	st->datasize = sz;
	st->axis = axis;
	st->time = 0;
	if (st->axis != AALL)
	    st->category = (nb->nodeidUpper[axis] != myproc) + 
		           (nb->nodeidLower[axis] != myproc);
	else
	    st->category = 3;
}

void update_stat(stat_struct_t *st, uint64_t temptime, int iters)
{
	st->iters += iters;
	st->time += temptime;
} 

void print_stat(nbr_t *nb, int myproc, stat_struct_t *st, const char *name)
{
	int	i,j,c;
	float	cattimes[4] = { 0.0 };
	int	catcount[4] = { 0 };
	double	stdev[4];
	double  ttime;

	/* Update statistics at zero.
	 * If we are doing a per-axis test, we separate the printed values
	 * within three categories based on the type of neighbor updates that
	 * were completed.
	 *
	 * Updates to Upper/Lower neighbor can be
	 *  1. Global/Global (both updates required communication)
	 *  2. Global/Local or Local/Global (only one update req'd comm).
	 *  3. Local/Local (no updates required communication)
	 *  4. Don't care (either local/global)
	 */
	MPI_SAFE(
	  MPI_Gather(st, sizeof(stat_struct_t), MPI_CHAR,
		         nb->stats0, sizeof(stat_struct_t), MPI_CHAR,
			 0, MPI_COMM_WORLD));
	BARRIER();

	if (myproc)
	    return;

	/* Find average in each category of face updates */
	for (i = 0; i < nprocs; i++) {
	    c = nb->stats0[i].category;
	    assert(c >= 0 && c <= 3);
	    /*
	    printf("%d> iter=%d, time=%lf\n", myproc, nb->stats0[i].iters,
			    (float)nb->stats0[i].time);
			    */
	    cattimes[c] += ((float)nb->stats0[i].time) / nb->stats0[i].iters;
	    catcount[c]++;
	}
	/* Calculate average */
	for (i = 0; i < 4; i++) {
	    if (catcount[i] > 0)
		cattimes[i] /= (float) catcount[i];
	    else
		cattimes[i] = .0;
	}
	/* Calculate stdev for each category*/
	for (c = 0; c < 4; c++) {
	    if (catcount[c] < 2)
		stdev[c] = .0;
	    else {
		double sumsq = .0;
		double devmean = .0;
		double procavg;
		double divm;
		for (i = 0; i < nprocs; i++) {
		    if (nb->stats0[i].category != c)
			continue;
		    procavg = ((double)nb->stats0[i].time)/nb->stats0[i].iters;
		    devmean = (double)cattimes[c] - procavg;
		    sumsq += devmean*devmean; /*pow((double)cattimes[c] - procavg, 2.0);*/
		}
		divm = sumsq / (catcount[c]-1);
		stdev[c] = sqrt(divm);
	    }
	}

	if (catcount[3] > 0) {
	    /* Don't care about various global/local distinctions */
	    printf("DIM %4i fullexch %8i byte : %5i iters, %9.2f +/- %8.2f us ave (%s)\n",
	     st->dims, st->datasize, st->iters, cattimes[3], stdev[3], name);

	}
	else {
	    printf("DIM %4i  axis %c %8i byte : %5i iters, %9.2f +/- %8.2f us ave (%s)\n",
	      st->dims, 'x'+st->axis, st->datasize, st->iters, cattimes[2], stdev[2], name
	    );
	}
	if (nbr_fp != NULL) {
	    int cat = catcount[3] > 0 ? 3 : 2;
	    fprintf(nbr_fp, "%-11s %c %4i %8i %9.2f %8.2f ",
	        name, cat == 3 ? 'F' : st->axis + 'x', st->dims, st->datasize, 
		cattimes[cat], stdev[cat]);
	    for (i = 0; i < nprocs; i++) {
	        if (nb->stats0[i].category != cat) {
		    ttime = .0;
		}
		else
		    ttime = ((float)nb->stats0[i].time) / nb->stats0[i].iters;
		fprintf(nbr_fp, " %9.2f", ttime);
	    }
	    fprintf(nbr_fp, "\n");
	    fflush(nbr_fp);
	}

	fflush(stdout);
}

int level_dims[][20] = {	
	{ 16,32,48,64,80,96,112,128,0 },
	{ 16,32,48,64,80,96,112,128,144,160,176,192,208,224,240,256,0 },
	{ 32,64,96,128,160,192,224,256,288,320,352,384,416,448,480,512,0 },
	{ 64,128,192,256,320,384,448,512,576,640,704,768,832,896,960,1024,0 }
};


void
usage()
{
    if (myproc != 0)
	return;

    printf("\ntestneighbor Neighbor-to-Neighbor microbenchmark\n\n");
    printf("testneighbor [-f] [iters] [level]\n\n");
    printf("-f      run full neighbor exchange (NAS MG) instead of per axis\n");
    printf("[iters] How many iterations per exchange (default = 150)\n");
    printf("[level] select level of dimensions (default level = 0)\n");
    printf("   level=0 dims=<16,32,48,64,80,96,112,128>\n");
    printf("   level=1 dims=<16,32,48,64, .. 128,160,192,224,256>\n");
    printf("   level=2 dims=<32,64,96,128, .. 320,352,384,416,448,480,512>\n");
    printf("   level=3 dims=<32,64,96,128, .. 928,960,992,1024>\n\n");
    fflush(stdout);
    exit(1);
}

void startupMPI(int* argc, char ***argv) {

  MPI_SAFE(MPI_Init(argc, argv));
  MPI_SAFE(MPI_Comm_rank(MPI_COMM_WORLD, &myproc));
  MPI_SAFE(MPI_Comm_size(MPI_COMM_WORLD, &nprocs));

  printf("P%i/%i starting...\n", myproc, nprocs); fflush(stdout);

  { /* compute tags -
     * base MPI tag on pid to prevent receiving cross-talk messages 
     * sent to dead processes (yes, this actually happens with some less robust MPI implementations)
     */
    int pid = getpid();
    int *allmpitags = (int *)malloc(nprocs * sizeof(int));
    mympitag = abs(pid) % (MPI_TAG_UB+1);
    if (mympitag == MPI_ANY_TAG) mympitag = (mympitag + 1) % (MPI_TAG_UB+1);

    /* exchange tags */
    MPI_SAFE(MPI_Allgather(&mympitag, 1, MPI_INT, 
      allmpitags, 1, MPI_INT, MPI_COMM_WORLD));
    assert(allmpitags[myproc] == mympitag);
    free(allmpitags);
    DEBUGMSG("tag computation complete");
  } 
}

void shutdownMPI() {
  DEBUGMSG("shutting down");
  MPI_SAFE(MPI_Finalize());

  printf("P%i exiting...\n", myproc); fflush(stdout);
}


int
main(int argc, char **argv)
{
    int	level = 0, i;
    int alldimensions = 1;
    char *nbrf;
    uintptr_t insegsz, outsegsz;
    int iters = 150;
    int dim;
    int maxdim = 0;
    int axis;
    int argn = 1;
    void *myseg;
    void *alloc;

    /* call startup */
    startupMPI(&argc, &argv);

    /* XXX parse args: iters min max */
    if (argc > argn) {
	if (*argv[argn] == '-') {
	    if (argv[argn][1] == 'f') 
		alldimensions = 0;
	    else {
		usage();
		shutdownMPI();
		exit(1);
	    }
	    argn++;
	}
	if (argc > argn) {
	    iters = atoi(argv[argn++]);
	    if (!iters)
		usage();
	}
	if (argc > argn) {
	    level = atoi(argv[argn++]);
	    if (!(level >= 0 && level < 4))
		usage();
	}
    }

    for (i = 0; i < level_dims[level][i]; i++) 
	maxdim = MAX(level_dims[level][i], maxdim);

    if (!POWER_OF_TWO(nprocs)) {
	fprintf(stderr, "%s only runs on a power of two processors\n", argv[0]);
	shutdownMPI();
	exit(1);
    }

    if (!myproc) {
      printf("=====> testmpineighbor nprocs=%d config=MPI\n", nprocs);
      fflush(stdout);
    }

    /* setup max grid we intend to use, so we can get enough 
     * memory per proc at startup */
    setupGrid(&Nbr, maxdim);

    BARRIER();

    /* Run test over all axes:
     * 0 -> x: yz planes
     * 1 -> y: xz planes
     * 2 -> z: xy planes
     * 3 -> x,y,z Full MG-like Neighbor exchange
     */
    /* We may want to gather extended info in a file */
    if (!myproc && (nbrf = getenv("NBRTEST_FILE")) != NULL) {
	nbr_fp = fopen(nbrf, "w");
	if (nbr_fp == NULL) {
	    fprintf(stderr, "Can't open NBRTEST_FILE %s\n", nbrf);
	    shutdownMPI();
	    exit(1);
	}
	printf("Saving extended output to %s\n", nbrf);
    }
    else
	nbr_fp = NULL;

    if (!myproc) {
        printf("\ntestneighbor running %d %s"
	       " (%d procs over processor grid = %2i x %2i x %2i)\n",
		iters, alldimensions ? "ghost exchanges per axis" :
		                       "full (NAS MG-like) ghost exchanges",
		nprocs, Nbr.procGrid[0], Nbr.procGrid[1], Nbr.procGrid[2]);

	printf(
	       "\nReported times are the medians across all processors only"
	       " for ghost exchanges that incur network communication\n");
    }

    BARRIER();

    if (alldimensions) {
	for (axis = 0; axis <= 2; axis++) {

	    if (!myproc) {
		if (axis == 2)
		    printf("\nExchange over 'z' contiguous axis, grid = %d procs\n",
			    Nbr.procGrid[2]);
		else
		    printf("\nExchange over '%c' non-contiguous axis, grid = "
			   "%d procs (DIM%s x stride %s)\n", 'x' + axis,
			   Nbr.procGrid[axis], axis==0 ? "^2" : "",
						 axis==0 ? "DIM" : "1");
		fflush(stdout);
	    } 
	    BARRIER();

	    for (i = 0; level_dims[level][i] != 0; i++) {
		dim = level_dims[level][i];
		setupGrid(&Nbr, dim);
		allocMultiGrid(&Nbr);
		BARRIER();
		/* In the alldimensions test, run only the non-blocking
		 * pairwise and the AMLong versions */
		ghostExchMPI(&Nbr, 1, axis); /* Dry run */
		ghostExchMPI(&Nbr, iters, axis);
	    }
	    BARRIER();
	}
    }
    else {
	axis = 3;   /* Full ghost exchange, no individual axis */

	BARRIER();

	for (i = 0; level_dims[level][i] != 0; i++) {
	    dim = level_dims[level][i];
	    setupGrid(&Nbr, dim);
	    allocMultiGrid(&Nbr);
	    BARRIER();
	    ghostExchMPI(&Nbr, 1, axis); /* Dry run */
	    ghostExchMPI(&Nbr, iters, axis);
	}
    }

    BARRIER();

    if (nbr_fp != NULL)
	fclose(nbr_fp);

    shutdownMPI();
    return 0;
}

void
setupGrid(nbr_t *nb, int dimsz)
{
    int t_grid = 1;
    int axis;
    int elemsPerDim, totelemsPerDim;

    nb->procGrid[0] = 1;
    nb->procGrid[1] = 1;
    nb->procGrid[2] = 1;

    /* setup the processor grid */
    while (t_grid*2 <= nprocs) {
	nb->procGrid[0] *= 2;
	t_grid *= 2;
	if (t_grid*2 <= nprocs) {
	    nb->procGrid[1] *= 2;
	    t_grid *= 2;
	    if (t_grid*2 <= nprocs) {
		nb->procGrid[2] *= 2;
		t_grid *= 2;
	    }
	}
    }
    assert(t_grid == nprocs);

    /* Setup the proc id in the grid */
    t_grid = myproc;
    nb->idGrid[0] = (myproc % (nb->procGrid[0]*nb->procGrid[1])) 
	                      % nb->procGrid[0];
    nb->idGrid[1] = (myproc % (nb->procGrid[0]*nb->procGrid[1])) 
	                      / nb->procGrid[0];
    nb->idGrid[2] =  myproc/(nb->procGrid[0]*nb->procGrid[1]);

    /* Setup the number of blocks per grid element in each dimension. Total
     * elements per dimension contains an extra two boundary elements */
    nb->elemsPerDim = elemsPerDim = dimsz;/*(2<<(unsigned)level);*/
    totelemsPerDim = elemsPerDim + 2;
    nb->totalSize = 1;

    /* Setup lower and upper neighbors in each dimension */
    for (axis = 0; axis <= 2; axis++) {
	int blocksz = elemsPerDim / nb->procGrid[axis];

	/* We don't handle corner cases, yet */
	assert(blocksz > 0);
	assert(elemsPerDim > nb->procGrid[axis]);
	assert(elemsPerDim % nb->procGrid[axis] == 0);

	nb->idGridUpper[axis] = 
	    nb->idGrid[axis] == nb->procGrid[axis]-1 
	                      ? 0 : nb->idGrid[axis]+1;
	nb->idGridLower[axis] = 
	    nb->idGrid[axis] == 0 
	    ? nb->procGrid[axis]-1 : nb->idGrid[axis]-1;

	/* Don't forget boundary elements in each dimension */
	blocksz += 2;

	/* XXX assumption of equal block distribution */
	nb->dimsLower[axis] = blocksz;
	nb->totalSize *= (long) blocksz;
	
	/* Now map the grid onto actual nodes */
	switch (axis) {
	    case 0: /* X axis */
		nb->nodeidUpper[0] = LINEARIZEPROC(nb,
			    nb->idGrid[2],nb->idGrid[1],nb->idGridUpper[0]);
		nb->nodeidLower[0] = LINEARIZEPROC(nb,
			    nb->idGrid[2],nb->idGrid[1],nb->idGridLower[0]);
		nb->dims0 = blocksz;
		break;
	    case 1: /* Y axis */
		nb->nodeidUpper[1] = LINEARIZEPROC(nb,
			    nb->idGrid[2],nb->idGridUpper[1],nb->idGrid[0]);
		nb->nodeidLower[1] = LINEARIZEPROC(nb,
			    nb->idGrid[2],nb->idGridLower[1],nb->idGrid[0]);
		nb->dims1 = blocksz;
		break;
	    case 2: /* Z axis */
		nb->nodeidUpper[2] = LINEARIZEPROC(nb,
			    nb->idGridUpper[2],nb->idGrid[1],nb->idGrid[0]);
		nb->nodeidLower[2] = LINEARIZEPROC(nb,
			    nb->idGridLower[2],nb->idGrid[1],nb->idGrid[0]);
		nb->dims2 = blocksz;
		break;

	    default:
		break;
	}

	nb->dims[axis] = blocksz;
    }

    nb->facesz[0] = nb->dims1*nb->dims2;
    nb->facesz[1] = nb->dims0*nb->dims2;
    nb->facesz[2] = nb->dims0*nb->dims1;

    nb->dimsz = dimsz;

    if (0) {
	fprintf(stdout,
	    "%2d> level %2d [%1d,%1d,%1d] in grid [%1d,%1d,%1d] has "
	    "[%1d,%1d],[%1d,%1d],[%1d,%1d] OR"
	    "[%1d,%1d],[%1d,%1d],[%1d,%1d]\n",
	    myproc, dimsz,
	    nb->idGrid[0], nb->idGrid[1], nb->idGrid[2],
	    nb->procGrid[0], nb->procGrid[1], nb->procGrid[2],
	    nb->idGridLower[0], nb->idGridUpper[0],
	    nb->idGridLower[1], nb->idGridUpper[1],
	    nb->idGridLower[2], nb->idGridUpper[2],
	    nb->nodeidLower[0], nb->nodeidUpper[0],
	    nb->nodeidLower[1], nb->nodeidUpper[1],
	    nb->nodeidLower[2], nb->nodeidUpper[2]);

    }
}

/*
 * Carve out our segment according to the grid dimensions currently set in Nb
 */
void
allocMultiGrid(nbr_t *nb)
{
    int i;
    char *segaddr;

    /* First allocate my space */

    nb->Dir   = (double *) calloc(nb->totalSize, sizeof(double));
    nb->Diryz = (double *) calloc(2*PX_SZ, sizeof(double));
    nb->Dirxz = (double *) calloc(2*PY_SZ, sizeof(double));
    nb->Dirxy = (double *) calloc(2*PZ_SZ, sizeof(double));
    nb->Ldata = (double *) nb->Dir;

    nb->dimBufs[0][0] = nb->Diryz;
    nb->dimBufs[0][1] = nb->dimBufs[0][0] + PX_SZ;
    nb->dimBufs[1][0] = nb->Dirxz;
    nb->dimBufs[1][1] = nb->dimBufs[1][0] + PY_SZ;
    nb->dimBufs[2][0] = nb->Dirxy;
    nb->dimBufs[2][1] = nb->dimBufs[2][0] + PZ_SZ;

    nb->yzCommBuffer = (double *) calloc(2*PX_SZ, sizeof(double));
    nb->xzCommBuffer = (double *) calloc(2*PY_SZ, sizeof(double));
    nb->xyCommBuffer = (double *) calloc(2*PZ_SZ, sizeof(double));

    nb->stats0 = (stat_struct_t *)calloc(nprocs, sizeof(stat_struct_t));
}

void
ge_take3(nbr_t *nb, double *src, size_t destp, int axis, MPI_Request *req)
{
    int n,i,j,k;
    int dk = nb->dims2;
    int dj = nb->dims1;
    int di = nb->dims0;

    /* Wait until data arrives */
    assert(req != NULL);
    MPI_SAFE(MPI_Wait(req, MPI_STATUS_IGNORE));

    n=0;
    switch (axis) {
      case 0:	/* X axis, yz plane, n * stride n */
        for (k=0; k < dk; k++)
	    for (j = 0; j < dj; j++)
	        AREF(nb,k,j,destp) = src[n++];
	break;
      case 1:	/* Y axis, xz plane, n * stride 1 */
	for (k=0; k < dk; k++)
	    for (i=0; i < di; i++)
		AREF(nb,k,destp,i) = src[n++];
	break;
      case 2: /* Z axis, xy plane, 1 * stride 1 */
	for (k=0; k < dk; k++)
	    for (j=0; j < dj; j++)
		AREF(nb,destp,j,i) = src[n++];
	break;
      default:
	break;
    }
    return;
}


/*
 * if (axis == AALL), do all axis (full ghost exchange)
 *
 */
void 
ghostExchMPI(nbr_t *nb, int iters, int axis_in)
{
    int i, j, axis, dest;
    int axis_tot;

    MPI_Request	    sreq[2];
    MPI_Request	    req[2];
    MPI_Request	    *takereq[2];

    uint64_t	    begin, end;
    stat_struct_t   stcomm3;
    int		    axes[3];

    if (axis_in == AALL) {
	axes[0] = 0; axes[1] = 1; axes[2] = 2;
	axis_tot = 3;
	init_stat(nb, &stcomm3, axis_in, nb->dimsz, (PX_SZ+PY_SZ+PZ_SZ)*sizeof(double)*2);
    }
    else {
	axes[0] = axis_in;
	axis_tot = 1;
	init_stat(nb, &stcomm3, axis_in, nb->dimsz, nb->facesz[axis_in]*sizeof(double)*2);
    }

    BARRIER();
    
    for (i = 0; i < iters; i++) {
	begin = TIME();
	for (j = 0; j < axis_tot; j++) {
	    axis = axes[j];

	    takereq[0] = ge_post3(nb, GHOST_DIR_UPPER, axis, &req[0]);
	    takereq[1] = ge_post3(nb, GHOST_DIR_LOWER, axis, &req[1]);

	    ge_give3(nb, GHOST_DIR_UPPER, axis, &sreq[0]);
	    ge_give3(nb, GHOST_DIR_LOWER, axis, &sreq[1]);

	    MPI_SAFE(
		MPI_Waitall(2, sreq, MPI_STATUSES_IGNORE));

	    ge_take3(nb, nb->dimBufs[axis][GHOST_DIR_UPPER], 0, axis, takereq[0]);
	    ge_take3(nb, nb->dimBufs[axis][GHOST_DIR_LOWER], nb->dims[axis]-1, 
		     axis, takereq[1]);
	}
	end = TIME();
	update_stat(&stcomm3, (end-begin), 1);
	BARRIER();
    }

    if (iters > 1) {
	print_stat(nb, myproc, &stcomm3, "MPI");
    }

    BARRIER();

    return;
}

/*
 * Prepost a receive buffer
 */
MPI_Request *
ge_post3(nbr_t *nb, int dir, int axis, MPI_Request *req)
{
    double *buf;
    size_t  sz;
    int	    node;

    switch (axis) {
	case AX:
	    buf = (double *) nb->Diryz;
	    if (dir == GHOST_DIR_LOWER)
		buf += PX_SZ;
	    sz = PX_SZ*sizeof(double);
	    break;
	case AY:
	    buf = (double *) nb->Dirxz;
	    if (dir == GHOST_DIR_LOWER)
		buf += PY_SZ;
	    sz = PY_SZ*sizeof(double);
	    break;
	case AZ:
	    buf = (double *) nb->Dirxy;
	    if (dir == GHOST_DIR_LOWER)
		buf += PZ_SZ;
	    sz = PZ_SZ*sizeof(double);
	    break;
	default:
	    break;
    }

    if (dir == GHOST_DIR_UPPER)
	node = nb->nodeidUpper[axis];
    else
	node = nb->nodeidLower[axis];

    MPI_SAFE(
	MPI_Irecv(buf, sz, MPI_BYTE, MPI_ANY_SOURCE, MPI_ANY_TAG,
		  MPI_COMM_WORLD, req));
	return req;
}

void
ge_give3(nbr_t *nb, int dir, int axis, MPI_Request *req)
{
    int	n=0,i,j,k;
    int dk = nb->dims2;
    int dj = nb->dims1;
    int di = nb->dims0;

    double  *src, *dest;
    int	    node, srcp, destp;
    size_t  len;

    /*printf("%d> in give dir=%d, axis=%d\n", myproc, dir, axis);*/
    if (dir == GHOST_DIR_UPPER) {
	node = nb->nodeidUpper[axis];
	srcp = nb->dims[axis]-2;
	destp = 0;
    }
    else { /* GHOST_DIR_LOWER */
	node = nb->nodeidLower[axis];
	srcp = 1;
	destp = nb->dimsLower[axis]-1;
    }

    /* take care of both axes that require packing */
    switch(axis) {
	case AX:
	    for (k = 0; k < dk; k++) 
		for (j = 0; j < dj; j++) 
		    nb->yzCommBuffer[n++] = AREF(nb,k,j,srcp);
	    src = nb->yzCommBuffer;
	    len = PX_SZ*sizeof(double);
	    break;
	case AY:
	    for (k = 0; k < dk; k++)
		for (i = 0; i < di; i++)
		    nb->xzCommBuffer[n++] = AREF(nb,k,srcp,i);
	    src = nb->xzCommBuffer;
	    len = PY_SZ*sizeof(double);
	    break;
	case AZ:
	    len = PZ_SZ*sizeof(double);
	    src = nb->xyCommBuffer;
	    memcpy(src,(double*)nb->Ldata + srcp*dj*di,len);
	    break;
	default:
	    break;
    }
	MPI_Isend(src, len, MPI_BYTE, node, myproc, MPI_COMM_WORLD, req);

    return;
}
