
static char rcsid[] =
	"$Id: mmain.c,v 1.3 2009/01/30 21:24:06 pvmsrc Exp $";

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
 *	main.c
 *
 *	Mandelbrot calculator.
 *	Uses tile processes to calculate strips from the requested
 *	region, cats the strips together.
 *
 *	Usage:
 *		main width height x1 y1 x2 y2 filename
 *		( main 256 256 -2.0 -2.0 2.0 2.0 whole_mandelbrot )
 *
 *	3 Jun 1991  Robert Manchek  manchek@CS.UTK.EDU.
 *	22 Sep 91 - took out ncpus command arg because we can find out from pvm
 *	02 Sep 92  - ported to v3
 */

#ifdef HASSTDLIB
#include <stdlib.h>
#endif
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <math.h>
#include "pvm3.h"
#include "../src/bfunc.h"

char *mandelbrot();

int nprocessors = 1;		/* number of processors to use */
int *prtids = 0;			/* processor task ids */

main(argc, argv)
	int argc;
	char **argv;
{
	int mytid;				/* my task id */
	int wd = 16, ht = 16;	/* size of image to calculate */
	double x1, y1, x2, y2;	/* image corner coordinates */
	char *pix;				/* image data */
	int d;					/* output file */
	int i;					/* gp */
	char *dfn;				/* file to write to */

	if (argc != 8) goto usage;
	wd = atoi(argv[1]);
	ht = atoi(argv[2]);
	x1 = atof(argv[3]);
	y1 = atof(argv[4]);
	x2 = atof(argv[5]);
	y2 = atof(argv[6]);
	dfn = argv[7];
	if (wd < 1 || wd > 2048 || ht < 1 || ht > 2048) {
		fputs("width and heigh must be between 1 and 2048\n", stderr);
		goto usage;
	}

	/* enroll in pvm */
	
	if ((mytid = pvm_mytid()) < 0) {
		exit(1);
	}
/*
	printf("i'm t%x\n", mytid);
*/

	pvm_config(&nprocessors, (int*)0, (struct pvmhostinfo**)0);
	if (nprocessors > wd)
		nprocessors = wd;
	fprintf(stderr, "using %d tile servers in calculation\n", nprocessors);

	/* create n instances of tile */

	prtids = (int*)malloc(nprocessors * sizeof(int));
/*
	if (pvm_spawn("mtile", (char**)0, 0, "", nprocessors, prtids) < 0) {
		fputs("can't initiate calculators\n", stderr);
		pvm_exit();
		exit(1);
	}
*/
	for (i = 0; i < nprocessors; i++) {
		if (pvm_spawn("mtile", (char**)0, 0, "", 1, &prtids[i]) < 0) {
			fputs("can't initiate calculator\n", stderr);
			pvm_exit();
			exit(1);
		}
/*
		printf("task t%x\n", prtids[i]);
*/
	}

	pix = mandelbrot(x1, y1, x2, y2, wd, ht);

	fputs("writing\n", stderr);
	if ((d = open(dfn, O_WRONLY|O_CREAT|O_EXCL|O_TRUNC, 0666)) == -1) {
		perror(dfn);

	} else {
		(void)write(d, pix, wd * ht);
		(void)close(d);
		fprintf(stderr, "%s: written\n", dfn);
	}

	/* kill processors we created */

	for (i = 0; i < nprocessors; i++)
		pvm_kill(prtids[i]);

	/* un-enroll from pvm */

	pvm_exit();
	exit(0);

usage:
	fputs("usage: mmain width height x1 y1 x2 y2 filename\n",
		stderr);
	exit(1);
}

char*
mandelbrot(x1, y1, x2, y2, wd, ht)
	double x1, y1, x2, y2;
	int wd, ht;
{
	char *pix = 0;		/* calculated image */
	char *tile = 0;		/* data from a single strip */
	int *tpos;			/* tile positions */
	int maxwd = 0;		/* maximum width of strip */
	int slavetid;		/* processor id responding */
	int twd;			/* width of a tile */
	double xyxy[4];		/* gp */
	int wdht[2];		/* gp */
	int i, j;			/* gp */
	int y;				/* gp */
	char *ba1, *ba2;	/* gp */

	pix = (char*)malloc(wd * ht);

	/* divide up area into strips and assign them to processors */

	tpos = (int*)malloc((nprocessors + 1) * sizeof(int));
	x2 -= x1;
	xyxy[0] = x1;
	xyxy[1] = y1;
	xyxy[3] = y2;
	wdht[1] = ht;
	tpos[0] = 0;
	fputs("sending work to processors\n", stderr);
	for (i = 0; i < nprocessors; i++) {
		pvm_initsend(PvmDataDefault);
		tpos[i + 1] = ((i + 1) * wd) / nprocessors;
		wdht[0] = tpos[i + 1] - tpos[i];
		if (wdht[0] > maxwd)
			maxwd = wdht[0];
		xyxy[2] = (tpos[i + 1] * x2) / wd + x1;
		pvm_pkdouble(xyxy, 4, 1);
		pvm_pkint(wdht, 2, 1);
		if (pvm_send(prtids[i], 1)) {
			fprintf(stderr, "error sending to <%x>\n", prtids[i]);
			pvm_exit();
			exit(1);
		}
		xyxy[0] = xyxy[2];
	}

	/* collect results and patch together */

	tile = (char*)malloc(maxwd * ht);
	fputs("processors responding:", stderr); fflush(stderr);
	for (i = 0; i < nprocessors; i++) {
		if (pvm_recv(-1, 2) < 1) {
			fprintf(stderr, "error receiving message type 2\n");
			pvm_exit();
			exit(1);
		}
		pvm_bufinfo(pvm_getrbuf(), &j, (int*)0, &slavetid);
/*
	fprintf(stderr, "len %d\n", j);
*/
		for (j = 0; j < nprocessors; j++)
			if (prtids[j] == slavetid) break;
		if (j < nprocessors) {
			fprintf(stderr, " %d", j);
			fflush(stderr);
			twd = tpos[j + 1] - tpos[j];
			pvm_upkbyte(tile, twd * ht, 1);
			ba1 = tile;
			ba2 = pix + tpos[j];
			for (y = ht; y-- > 0; ) {
				BCOPY(ba1, ba2, twd);
				ba1 += twd;
				ba2 += wd;
			}

		} else {
			fprintf(stderr, "got response from unknown processor %x?\n",
				slavetid);
		}
	}
	fputs("\n", stderr);
	free(tpos);
	free(tile);
	return pix;
}

