
static char rcsid[] =
	"$Id: timing.c,v 1.5 2009/01/23 01:12:57 pvmsrc Exp $";

/*
 *         PVM version 3.4:  Parallel Virtual Machine System
 *               University of Tennessee, Knoxville TN.
 *           Oak Ridge National Laboratory, Oak Ridge TN.
 *                   Emory University, Atlanta GA.
 *      Authors:  J. J. Dongarra, G. E. Fagg, M. Fischer
 *          G. A. Geist, J. A. Kohl, R. J. Manchek, P. Mucci,
 *         P. M. Papadopoulos, S. L. Scott, and V. S. Sunderam
 *                   (C) 1997 All Rights Reserved
 *
 *                              NOTICE
 *
 * Permission to use, copy, modify, and distribute this software and
 * its documentation for any purpose and without fee is hereby granted
 * provided that the above copyright notice appear in all copies and
 * that both the copyright notice and this permission notice appear in
 * supporting documentation.
 *
 * Neither the Institutions (Emory University, Oak Ridge National
 * Laboratory, and University of Tennessee) nor the Authors make any
 * representations about the suitability of this software for any
 * purpose.  This software is provided ``as is'' without express or
 * implied warranty.
 *
 * PVM version 3 was funded in part by the U.S. Department of Energy,
 * the National Science Foundation and the State of Tennessee.
 */

/*
*	timing.c
*
*	Does a few communication timing tests on pvm.
*	Uses `timing_slave' to echo messages.
* ----------------------------------------------------------------------------
*     If this test is run over machines with differnet data formats
*     Then change 'ENCODING' to PvmDataDefault in timing and timing_slave
* ----------------------------------------------------------------------------
*
*	9 Dec 1991  Manchek
*  14 Oct 1992  Geist  - revision to pvm3
*   6 Mar 1994  Geist  - synch tasks and add direct route
*/

#include <stdio.h>
#ifdef HASSTDLIB
#include <stdlib.h>
#endif

#ifndef WIN32
#include <sys/time.h>
#endif

#include <time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <math.h>
#include "pvm3.h"


#define SLAVENAME "timing_slave"
#define ENCODING  PvmDataRaw

main(argc, argv)
	int argc;
	char **argv;
{
	int mytid;                  /* my task id */
	int stid = 0;				/* slave task id */
	int reps = 20;				/* number of samples per test */
	struct timeval tv1, tv2;	/* for timing */
	int dt1, dt2;				/* time for one iter */
	int at1, at2;				/* accum. time */
	int numint;					/* message length */
	int n;
	int i;
	int *iarray = 0;

	/* enroll in pvm */

	if ((mytid = pvm_mytid()) < 0) {
		exit(1);
	}
	printf("i'm t%x\n", mytid);

	/* start up slave task */

	if (pvm_spawn(SLAVENAME, (char**)0, 0, "", 1, &stid) < 0 || stid < 0) {
		fputs("can't initiate slave\n", stderr);
		goto bail;
	}

    /* Wait for slave task to start up */
    pvm_setopt(PvmRoute, PvmRouteDirect);
    pvm_recv( stid, 0 );
	printf("slave is task t%x\n", stid);

	/*
	*  round-trip timing test
	*/

	puts("Doing Round Trip test, minimal message size\n");
	at1 = 0;

	/* pack buffer */

	pvm_initsend(ENCODING);
	pvm_pkint(&stid, 1, 1);

	puts(" N     uSec");
	for (n = 1; n <= reps; n++) {
		gettimeofday(&tv1, (struct timezone*)0);

		if (pvm_send(stid, 1)) {
			fprintf(stderr, "can't send to \"%s\"\n", SLAVENAME);
			goto bail;
		}

		if (pvm_recv(-1, -1) < 0) {
			pvm_perror( "recv error" );
			goto bail;
		}

		gettimeofday(&tv2, (struct timezone*)0);

		dt1 = (tv2.tv_sec - tv1.tv_sec) * 1000000 + tv2.tv_usec - tv1.tv_usec;
		printf("%2d %8d\n", n, dt1);
		at1 += dt1;
	}
	printf("RTT Avg uSec %d\n", at1 / reps);

	/*
	*  bandwidth test for different message lengths
	*/

	puts("\nDoing Bandwidth tests\n");

	for (numint = 25; numint < 1000000; numint *= 10) {
		printf("\nMessage size %d\n", numint * 4);
		at1 = at2 = 0;
		iarray = (int*)malloc(numint * sizeof(int));
		puts(" N  Pack uSec  Send uSec");
		for (n = 1; n <= reps; n++) {
			gettimeofday(&tv1, (struct timezone*)0);

			pvm_initsend(ENCODING);
			pvm_pkint(iarray, numint, 1);

			gettimeofday(&tv2, (struct timezone*)0);
			dt1 = (tv2.tv_sec - tv1.tv_sec) * 1000000
				+ tv2.tv_usec - tv1.tv_usec;

			gettimeofday(&tv1, (struct timezone*)0);

			if (pvm_send(stid, 1)) {
				fprintf(stderr, "can't send to \"%s\"\n", SLAVENAME);
				goto bail;
			}

			if (pvm_recv(-1, -1) < 0) {
				pvm_perror( "recv error" );
				goto bail;
			}

			gettimeofday(&tv2, (struct timezone*)0);
			dt2 = (tv2.tv_sec - tv1.tv_sec) * 1000000
				+ tv2.tv_usec - tv1.tv_usec;

			printf("%2d   %8d   %8d\n", n, dt1, dt2);
			at1 += dt1;
			at2 += dt2;
		}

		if (!(at1 /= reps))
			at1 = 1;
		if (!(at2 /= reps))
			at2 = 1;
		puts("Avg uSec");
		printf("     %8d   %8d\n", at1, at2);
		puts("Avg Byte/uSec");
		printf("     %8f   %8f\n",
			(numint * 4) / (double)at1,
			(numint * 4) / (double)at2);
	}

	/* we have to do this because the last message might be taking
	*  up all the shared memory pages.
	*/
	pvm_freebuf(pvm_getsbuf());

	puts("\ndone");

bail:
	if (stid > 0)
		pvm_kill(stid);
	pvm_exit();
	exit(1);
}

