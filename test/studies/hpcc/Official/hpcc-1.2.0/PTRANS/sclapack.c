/* -*- mode: C; tab-width: 2; indent-tabs-mode: nil; -*- */

#include <hpcc.h>

#include "cblacslt.h"

/* Common Block Declarations */

struct {
    double cpusec[64], wallsec[64], cpustart[64], wallstart[64];
    int disabled;
} sltimer00_;

#define sltimer00_1 sltimer00_

/* Table of constant values */

static int c__1 = 1;
static int c_n1 = -1;
static int c__0 = 0;

int
iceil_(int *inum, int *idenom) {
    /* System generated locals */
    int ret_val;


/*  -- ScaLAPACK tools routine (version 1.7) -- */
/*     University of Tennessee, Knoxville, Oak Ridge National Laboratory, */
/*     and University of California, Berkeley. */
/*     May 1, 1997 */

/*  Purpose */

/*  ICEIL returns the ceiling of the division of two integers. */

/*  Arguments */

/*  INUM     (local input) INTEGER */
/*           The numerator, */

/*  IDENOM   (local input) INTEGER */
/*           and the denominator of the fraction to be evaluated. */

    ret_val = (*inum + *idenom - 1) / *idenom;

    return ret_val;
} /* iceil_ */

int
numroc_(int *n, int *nb, int *iproc, int *isrcproc, int *nprocs)
{
    /* System generated locals */
    int ret_val;

    /* Local variables */
    int extrablks, mydist, nblocks;


/*  -- ScaLAPACK tools routine (version 1.7) -- */
/*     University of Tennessee, Knoxville, Oak Ridge National Laboratory, */
/*     and University of California, Berkeley. */
/*     May 1, 1997 */

/*  Purpose */

/*  NUMROC computes the NUMber of Rows Or Columns of a distributed */
/*  matrix owned by the process indicated by IPROC. */

/*  Arguments */

/*  N         (global input) INTEGER */
/*            The number of rows/columns in distributed matrix. */

/*  NB        (global input) INTEGER */
/*            Block size, size of the blocks the distributed matrix is */
/*            split into. */

/*  IPROC     (local input) INTEGER */
/*            The coordinate of the process whose local array row or */
/*            column is to be determined. */

/*  ISRCPROC  (global input) INTEGER */
/*            The coordinate of the process that possesses the first */
/*            row or column of the distributed matrix. */

/*  NPROCS    (global input) INTEGER */
/*            The total number processes over which the matrix is */
/*            distributed. */

/*     Figure PROC's distance from source process */

    mydist = (*nprocs + *iproc - *isrcproc) % *nprocs;

/*     Figure the total number of whole NB blocks N is split up into */

    nblocks = *n / *nb;

/*     Figure the minimum number of rows/cols a process can have */

    ret_val = nblocks / *nprocs * *nb;

/*     See if there are any extra blocks */

    extrablks = nblocks % *nprocs;

/*     If I have an extra block */

    if (mydist < extrablks) {
	ret_val += *nb;

/*         If I have last block, it may be a partial block */

    } else if (mydist == extrablks) {
	ret_val += *n % *nb;
    }

    return ret_val;
} /* numroc_ */

int ilcm_(int *m, int *n)
{
  /* System generated locals */
  int ret_val;

  /* Local variables */
  int ia, iq, ir;


/*  -- ScaLAPACK tools routine (version 1.7) -- */
/*     University of Tennessee, Knoxville, Oak Ridge National Laboratory, */
/*     and University of California, Berkeley. */
/*     May 1, 1997 */

/*  Purpose */

/*  ILCM computes and returns the Least Common Multiple (LCM) of two */
/*  positive integers M and N. In fact the routine computes the greatest */
/*  common divisor (GCD) and use the fact that M*N = GCD*LCM. */

/*  Arguments */

/*  M       (input) INTEGER */
/*          On entry, M >=0. Unchanged on exit. */

/*  N       (input) INTEGER */
/*          On entry, N >=0. Unchanged on exit. */

  if (*m >= *n) {
    ia = *m;
    ret_val = *n;
  } else {
    ia = *n;
    ret_val = *m;
  }

  for (;;) {
    iq = ia / ret_val;
    ir = ia - iq * ret_val;
    if (ir == 0) {
      ret_val = *m * *n / ret_val;
      return ret_val;
    }
    ia = ret_val;
    ret_val = ir;
  }
} /* ilcm_ */

int
pxerbla(int *ictxt, char *srname, int *info) {
    /* Format strings */
    char fmt_9999[] = "{ %5d, %5d }:  On entry "
	    "to %s() parameter number %4d had an illegal value"
	    "\n";

    int npcol, mycol, nprow, myrow;

/*  -- ScaLAPACK auxiliary routine (version 2.0) -- */
/*     University of Tennessee, Knoxville, Oak Ridge National Laboratory, */
/*     and University of California, Berkeley. */
/*     April 1, 1998 */

/*  Purpose */

/*  PXERBLA is an error handler for the ScaLAPACK routines.  It is called */
/*  by a ScaLAPACK routine if an input parameter has an invalid value.  A */
/*  message is printed. Installers may consider modifying this routine in */
/*  order to call system-specific exception-handling facilities. */

/*  Arguments */

/*  ICTXT   (local input) INTEGER */
/*          On entry,  ICTXT  specifies the BLACS context handle, indica- */
/*          ting the global  context of the operation. The context itself */
/*          is global, but the value of ICTXT is local. */

/*  SRNAME  (global input) CHARACTER*(*) */
/*          On entry, SRNAME specifies the name of the routine which cal- */
/*          ling PXERBLA. */

/*  INFO    (global input) INTEGER */
/*          On entry, INFO  specifies the position of the invalid parame- */
/*          ter in the parameter list of the calling routine. */

/*  -- Written on April 1, 1998 by */
/*     Antoine Petitet, University  of  Tennessee, Knoxville 37996, USA. */

    Cblacs_gridinfo(*ictxt, &nprow, &npcol, &myrow, &mycol);

    printf( fmt_9999, myrow, mycol, srname, *info );
    fflush( stdout );
    return 0;
} /* pxerbla */

int
slboot_(void) {
    static int i__;


/*  -- ScaLAPACK tools routine (version 1.7) -- */
/*     University of Tennessee, Knoxville, Oak Ridge National Laboratory, */
/*     and University of California, Berkeley. */
/*     May 1, 1997 */

/*  Purpose */

/*  SLBOOT (re)sets all timers to 0, and enables SLtimer. */

    sltimer00_1.disabled = 0;
    for (i__ = 1; i__ <= 64; ++i__) {
	sltimer00_1.cpusec[i__ - 1] = 0.;
	sltimer00_1.wallsec[i__ - 1] = 0.;
	sltimer00_1.cpustart[i__ - 1] = -5.;
	sltimer00_1.wallstart[i__ - 1] = -5.;
/* L10: */
    }

    return 0;
} /* slboot_ */


int
sltimer_(int *i__) {
/*  -- ScaLAPACK tools routine (version 1.7) -- */
/*     University of Tennessee, Knoxville, Oak Ridge National Laboratory, */
/*     and University of California, Berkeley. */
/*     May 1, 1997 */

/*  Purpose */

/*  SLtimer provides a "stopwatch" functionality cpu/wall timer */
/*  (in seconds).  Up to 64 separate timers can be functioning at once. */
/*  The first call starts the timer, and the second stops it.  This */
/*  routine can be disenabled, so that calls to the timer are ignored. */
/*  This feature can be used to make sure certain sections of code do */
/*  not affect timings, even if they call routines which have SLtimer */
/*  calls in them. */

/*  Arguments */

/*  I       (global input) INTEGER */
/*          The timer to stop/start. */

/*     If timing disabled, return */

  if (sltimer00_1.disabled) {
    return 0;
  }

  if (sltimer00_1.wallstart[*i__ - 1] == -5.) {

/*        If timer has not been started, start it */

	sltimer00_1.wallstart[*i__ - 1] = dwalltime00();
	sltimer00_1.cpustart[*i__ - 1] = dcputime00();

    } else {

/*        Stop timer and add interval to count */

	sltimer00_1.cpusec[*i__ - 1] = sltimer00_1.cpusec[*i__ - 1] +
		dcputime00() - sltimer00_1.cpustart[*i__ - 1];
	sltimer00_1.wallsec[*i__ - 1] = sltimer00_1.wallsec[*i__ - 1] +
		dwalltime00() - sltimer00_1.wallstart[*i__ - 1];
	sltimer00_1.wallstart[*i__ - 1] = -5.;

    }
    return 0;
} /* sltimer_ */

int
slcombine_(int *ictxt, char *scope, char *op, char * timetype, int *n, int *ibeg, double *times) {
    /* System generated locals */
    int i__1;

    /* Local variables */
    static int i__;
    static int tmpdis;


/*  -- ScaLAPACK tools routine (version 1.7) -- */
/*     University of Tennessee, Knoxville, Oak Ridge National Laboratory, */
/*     and University of California, Berkeley. */
/*     May 1, 1997 */

/*  Purpose */

/*  SLCOMBINE takes the timing information stored on a scope of processes */
/*  and combines them into the user's TIMES array. */

/*  Arguments */

/*  ICTXT    (local input) INTEGER */
/*           The BLACS context handle. */

/*  SCOPE    (global input) CHARACTER */
/*           Controls what processes in grid participate in combine. */
/*           Options are 'Rowwise', 'Columnwise', or 'All'. */

/*  OP       (global input) CHARACTER */
/*           Controls what combine should be done: */
/*           = '>': get maximal time on any process (default), */
/*           = '<': get minimal time on any process, */
/*           = '+': get sum of times across processes. */

/*  TIMETYPE (global input) CHARACTER */
/*           Controls what time will be returned in TIMES: */
/*           = 'W': wall clock time, */
/*           = 'C': CPU time (default). */

/*  N        (global input) INTEGER */
/*           The number of timers to combine. */

/*  IBEG     (global input) INTEGER */
/*           The first timer to be combined. */

/*  TIMES    (global output) DOUBLE PRECISION array, dimension (N) */
/*           The requested timing information is returned in this array. */

/*     Disable timer for combine operation */

    /* Parameter adjustments */
    --times;

    /* Function Body */
    tmpdis = sltimer00_1.disabled;
    sltimer00_1.disabled = 1;

/*     Copy timer information into user's times array */

    if (*timetype == 'W') {

/*        If walltime not available on this machine, fill in times */
/*        with -1 flag, and return */

	if (dwalltime00() == -1.) {
	    i__1 = *n;
	    for (i__ = 1; i__ <= i__1; ++i__) {
		times[i__] = -1.;
/* L10: */
	    }
	    return 0;
	} else {
	    i__1 = *n;
	    for (i__ = 1; i__ <= i__1; ++i__) {
		times[i__] = sltimer00_1.wallsec[*ibeg + i__ - 2];
/* L20: */
	    }
	}
    } else {
	if (dcputime00() == -1.) {
	    i__1 = *n;
	    for (i__ = 1; i__ <= i__1; ++i__) {
		times[i__] = -1.;
/* L30: */
	    }
	    return 0;
	} else {
	    i__1 = *n;
	    for (i__ = 1; i__ <= i__1; ++i__) {
		times[i__] = sltimer00_1.cpusec[*ibeg + i__ - 2];
/* L40: */
	    }
	}
    }

/*     Combine all nodes' information, restore disabled, and return */

    if (*(unsigned char *)op == '>') {
	Cdgamx2d(*ictxt,scope," ",*n,c__1,&times[1],*n,&c_n1,&c_n1,c_n1,c_n1,c__0);
    } else if (*(unsigned char *)op == '<') {
	Cdgamn2d(*ictxt,scope," ",*n,c__1,&times[1],*n,&c_n1,&c_n1,c_n1,c_n1,c__0);
    } else if (*(unsigned char *)op == '+') {
	Cdgsum2d(*ictxt,scope," ",*n,c__1,&times[1],*n,c_n1,c__0);
    } else {
	Cdgamx2d(*ictxt,scope," ",*n,c__1,&times[1],*n,&c_n1,&c_n1,c_n1,c_n1,c__0);
    }

    sltimer00_1.disabled = tmpdis;

    return 0;
} /* slcombine_ */
