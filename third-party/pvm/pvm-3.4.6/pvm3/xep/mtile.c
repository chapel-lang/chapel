
static char rcsid[] =
	"$Id: mtile.c,v 1.2 1997/07/09 13:56:56 pvmsrc Exp $";

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
 *	mtile.c
 *
 *	Mandelbrot tile calculation server.
 *	Receives a request to calculate a tile, generates the tile, and
 *	sends it back.
 *
 *	3 Jun 1991  Robert Manchek  manchek@CS.UTK.EDU.
 *	2 Dec 91  check for null argv[0]
 *	02 Sep 92 - ported to v3
 */

#ifdef HASSTDLIB
#include <stdlib.h>
#endif
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include "pvm3.h"

char* calc_tile();


main(argc, argv)
	int argc;
	char **argv;
{
	int mytid;				/* my task id */
	int mastertid;			/* who sent us this tile */
	double x1, y1, x2, y2;	/* tile corner coordinates */
	int wd, ht;				/* size of tile */
	char *pix;				/* calculated image */
	int mid;

	mytid = pvm_mytid();

	/* loop forever reading sizes and coords of tiles we should calculate */

	while ((mid = pvm_recv(-1, 1)) > 0) {
		pvm_bufinfo(mid, (int*)0, (int*)0, &mastertid);
		pvm_unpackf("%lf %lf %lf %lf %d %d", &x1, &y1, &x2, &y2, &wd, &ht);

/*
		fprintf(stderr, "%dx%d tile between %f,%f and %f,%f\n",
			wd, ht, x1, y1, x2, y2);
*/

		pix = calc_tile(x1, y1, x2, y2, wd, ht);

		pvm_packf("%+ %*c", PvmDataDefault, wd * ht, pix);
/*
		umbuf_dump(pvm_getsbuf(), 1);
*/
/*
		pvm_packf("%+ %*c", PvmDataInPlace, wd * ht, pix);
*/

		if (pvm_send(mastertid, 2)) {
			fprintf(stderr, "error sending image back\n");
		}

		free(pix);
	}
	fprintf(stderr, "error receiving work to do\n");
	pvm_exit();
	exit(1);
}


char*
calc_tile(x1, y1, x2, y2, wd, ht)
	double x1, y1, x2, y2;	/* tile corner coords */
	int wd, ht;				/* size of tile */
{
	char *pix;						/* calculated image */
	int ix, iy;						/* pixel coords */
	double x, y;					/* re, im coords */
	register double ar, ai;			/* accumulator */
	register double a1, a2;
	register int ite;				/* number of iter until divergence */

	if (wd < 1 || wd > 2048 || ht < 1 || ht > 2048) {
		fputs("insane wd/ht\n", stderr);
		pvm_exit();
		exit(1);
	}
	pix = (char*)malloc(wd * ht);
	x2 -= x1;
	y2 -= y1;
	for (iy = ht; iy-- > 0; ) {
		y = (iy * y2) / ht + y1;
		for (ix = wd; ix-- > 0; ) {
			x = (ix * x2) / wd + x1;
			ar = x;
			ai = y;
			for (ite = 0; ite < 255; ite++) {
				a1 = (ar * ar);
				a2 = (ai * ai);
				if (a1 + a2 > 4.0)
					break;
				ai = 2 * ai * ar + y;
				ar = a1 - a2 + x;
			}
			pix[iy * wd + ix] = ~ite;
		}
	}
	return pix;
}


