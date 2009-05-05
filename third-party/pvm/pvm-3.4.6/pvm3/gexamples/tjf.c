
static char rcsid[] =
	"$Id: tjf.c,v 1.3 1999/03/12 17:57:35 pvmsrc Exp $";

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
	test join fail,  cause join to fail
*/

#include <stdio.h>
#include "pvm3.h"

#define MAXGNAME 32 /* used to be a constant, length not important */
#define MAXGTIDS 32 /* used to be a constant, Number not important */
#define MAXNGROUPS 32 /* used to be constant, Number not important */
int
main(argc, argv)
int argc;
char *argv[];
{
	int mytid, ctid[MAXGTIDS];
	char g[MAXGNAME];
	int cc;
	int gid[MAXGTIDS], gs;
	int i;

	mytid = pvm_mytid();
	if (mytid < 0) {
		pvm_perror("th");
		return -1;
		}

	/* join one too many groups */
	for (i = 0; i < MAXNGROUPS+1; i++) {
		sprintf(g, "group%d", i);
		if((gid[i] = pvm_joingroup(g)) < 0) {
			pvm_perror("joining");
			}
		fprintf(stderr, "joined %s\n", g);
		}

	/* test that i'm in the groups */
	for (i = MAXNGROUPS-1; i  >= 0; i--) {
		sprintf(g, "group%d", i);
		if(gid[i] != (cc = pvm_getinst(g, mytid))) {
			fprintf(stderr, "I'm not myself %d %s\n", cc, g);
			pvm_perror("getinst");
			}
		if(mytid != (cc = pvm_gettid(g, gid[i]))) {
			fprintf(stderr, "I'm not myself %d %s\n", cc, g);
			pvm_perror("gettid");
			}
		}

	for (i = 0; i < MAXNGROUPS; i++) {
		sprintf(g, "group%d", i);
		if(pvm_lvgroup(g) < 0)
			pvm_perror("pvm_lvgroup");
		}

	pvm_exit();
	return 0;
}
