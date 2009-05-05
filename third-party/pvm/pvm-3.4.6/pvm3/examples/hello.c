
static char rcsid[] =
	"$Id: hello.c,v 1.3 2009/01/23 01:12:48 pvmsrc Exp $";

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

#include <stdio.h>
#ifdef HASSTDLIB
#include <stdlib.h>
#endif
#include "pvm3.h"

main()
{
	int cc, tid;
	char buf[100];

	printf("i'm t%x\n", pvm_mytid());

	cc = pvm_spawn("hello_other", (char**)0, 0, "", 1, &tid);

	if (cc == 1) {
		cc = pvm_recv(-1, -1);
		pvm_bufinfo(cc, (int*)0, (int*)0, &tid);
		pvm_upkstr(buf);
		printf("from t%x: %s\n", tid, buf);

	} else
		printf("can't start hello_other\n");

	pvm_exit();
	exit(0);
}


