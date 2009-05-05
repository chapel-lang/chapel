
static char rcsid[] =
	"$Id: gs.c,v 1.3 1997/08/11 14:39:18 pvmsrc Exp $";

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
	Using pvm_gsize()
*/

#include <stdio.h>
#include "pvm3.h"

#define GSGROUP "gsgroup"

int
main(argc, argv)
int argc;
char *argv[];
{
	int mytid, mygid, ctid[32];
	int nproc, i;

	mytid = pvm_mytid();
	fprintf(stderr, "%s 0x%x enrolled\n", argv[0], mytid);
	if (mytid < 0) {
		pvm_perror(argv[0]);
		return -1;
		}

	if (argc != 2) goto usage;
	if ((nproc = atoi(argv[1])) < 1) goto usage;
	if (nproc > 32) goto usage;

	/* join a group */
	mygid = pvm_joingroup(GSGROUP);

	/* if I'm the first to join then start the others */
	if (mygid == 0) {
		/* start a bunch of children */
    	pvm_spawn(argv[0], argv+1, 0, "", nproc-1, ctid);
		/* check them */
		for (i = 0; i < nproc-1; i++)
			fprintf(stderr, "0x%x\n", ctid[i]);
		}

	while(pvm_gsize(GSGROUP) < nproc)
		if (mygid == 0)
			fputs("waiting on kids to join\n", stderr);

	/* sync */
	pvm_barrier(GSGROUP, nproc);

	/* leave the group */
	if(pvm_lvgroup(GSGROUP) < 0)
			pvm_perror(argv[0]);

	pvm_exit();
	return 0;
usage:
	fprintf(stderr, "usage: %s <nproc>\n", argv[0]);
	return -1;
}
