
static char rcsid[] =
	"$Id: bwtest.c,v 1.12 2009/01/23 01:12:48 pvmsrc Exp $";

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

/* bwtest.c - bandwidth tester
 * This is yet another bandwidth tester.
 * It measures user-space to user-space times.
 *
 * The measurement is a standard ping-pong with roundtrip time measured
 * from the originating sender.
 *
 * This is a PVM test code
 */

#include <stdio.h>
#ifdef HASSTDLIB
#include <stdlib.h>
#endif

#include "pvm3.h"

#ifndef SAMPLE
#define SAMPLE  20 	/* sample rate */
#endif
#ifndef MAXSIZE
#define MAXSIZE	100000	/* must be a power of 10 */
#endif

#define VTAG 10
#define MTAG 100

#ifndef ENCODE
#define ENCODE	PvmDataInPlace
#endif

#define GETTIME(_x,_y) pvmgetclock((_x)) 

#ifndef WIN32
#include <sys/time.h>
#else
#include <winsock.h>
#endif

struct timeval tv1, tv2;
#define TIMER_CLEAR     (tv1.tv_sec = tv1.tv_usec = tv2.tv_sec = tv2.tv_usec =0)
#define TIMER_START     GETTIME(&tv1, (struct timezone*)0)
#define TIMER_STOP      GETTIME(&tv2, (struct timezone*)0)
#define TIMER_ELAPSED \
	( ( tv2.tv_usec - tv1.tv_usec ) \
		+ ( ( tv2.tv_sec - tv1.tv_sec) * 1.E6 ) )


#define GNAME "bwtest"

void pingpong();	/* where all the work is done and measured */


main(argc, argv)
int argc;
char *argv[];
{
	int mytid;
	int myinst;
	int othertid;
	int otherinst;

	if ((mytid = pvm_mytid()) < 0)
		exit(-1);

	if((myinst = pvm_joingroup(GNAME)) < 0) {
		pvm_perror("Error joining bwtest group");
		exit(-1);
	}


	printf(" %x myinst is %d \n", mytid, myinst);
	switch (myinst % 2) {
	case 0: 

		otherinst = myinst + 1;
		while ((othertid = pvm_gettid(GNAME, otherinst)) < 0)
			pvmsleep(1);

		pingpong(mytid, othertid);
		break;
	case 1:
		otherinst = myinst - 1;
		while ((othertid = pvm_gettid(GNAME, otherinst)) < 0)
			pvmsleep(1);

		pingpong(othertid, mytid);
		break;
	}

	pvm_lvgroup(GNAME);
	pvm_exit();

}
void
validate(data, size)
	double data[];
	int size;
{
	int i;
	int mytid;

	mytid = pvm_mytid();

	for (i = 0; i < size; i++)
		if ((i*i - data[i]) > 0.01) {
			printf("error: data[%d] = %g\n", i, data[i]);
			break;
		}

	if (i == size)
		printf("t%x: %d doubles received correctly\n\n\n", mytid, i);
}


/* exchange messages and measure the transit time */
void
pingpong(mastertid, slavetid)
int mastertid;
int slavetid;
{
	int ismaster;
	int mytid;
	int n;
	int size;
	int t;

	static double data[MAXSIZE];

	char str[32];

	/* test node-to-node send */

	mytid = pvm_mytid();


	ismaster = ( mytid ==  mastertid );


	if (ismaster)
	{
#ifdef PACK
		print_header(ENCODE, MAXSIZE*sizeof(double), SAMPLE);
#else
		print_header(-1, MAXSIZE*sizeof(double), SAMPLE);
#endif
	}

	printf("%x -- I am the %s \n", mytid, (ismaster ? "master" : "slave"));


	/* first validate that all doubles are sent and returned correctly */
	if (ismaster)
	{

		for (n = 0; n < MAXSIZE; n++)
			data[n] = n*n;
#ifdef PACK
		pvm_initsend(ENCODE);
		pvm_pkdouble(data, MAXSIZE, 1);
		pvm_send(slavetid, VTAG); /* send the data */
		
		pvm_recv(slavetid, VTAG); /* recv it back and validate */
		pvm_upkdouble(data, MAXSIZE, 1);
#else
		pvm_psend(slavetid, VTAG, data, MAXSIZE, PVM_DOUBLE);
		pvm_precv(slavetid, VTAG, data, MAXSIZE, PVM_DOUBLE, (int*)0, 
			(int*)0, (int*)0);
#endif
		
	}
	else
	{
#ifdef PACK
		
		pvm_recv(mastertid, VTAG);	/* recv and unpack */ 
		pvm_upkdouble(data, MAXSIZE, 1);

		pvm_initsend(ENCODE);		/* echo */
		pvm_pkdouble(data, MAXSIZE, 1);
		pvm_send(mastertid, VTAG); /* send the data */
#else
		pvm_precv(mastertid, VTAG, data, MAXSIZE, PVM_DOUBLE, (int*)0, 
			(int*)0, (int*)0);
		pvm_psend(mastertid, VTAG, data, MAXSIZE, PVM_DOUBLE);
#endif
	}

	validate(data, MAXSIZE);
	sprintf(str, "%d doubles from t%x", MAXSIZE, slavetid);
	

	/* now do the ping-pong */

	for (size = 0; size <= MAXSIZE; size = ( size ? 10*size : 1) )
	{
		TIMER_CLEAR;
		TIMER_START;
		for (n = 0; n < SAMPLE; n++) 
		{
			if (ismaster)
			{
#ifdef PACK
				pvm_initsend(ENCODE);
				pvm_pkdouble(data, size, 1);
				pvm_send(slavetid, MTAG);
				pvm_recv(slavetid, MTAG);
				pvm_upkdouble(data, size, 1);
#else
				pvm_psend(slavetid, MTAG, data, size, PVM_DOUBLE);
				pvm_precv(slavetid, MTAG, data, size, PVM_DOUBLE, 
						(int*)0, (int*)0, (int*)0);
#endif /*PACK*/
			}
			else
			{
#ifdef PACK
				pvm_recv(mastertid, MTAG);
				pvm_upkdouble(data, size, 1);
				pvm_initsend(ENCODE);
				pvm_pkdouble(data, size, 1);
				pvm_send(mastertid, MTAG);
#else
				pvm_precv(mastertid, MTAG, data, size, PVM_DOUBLE, 
						(int*)0, (int*)0, (int*)0);
				pvm_psend(mastertid, MTAG, data, size, PVM_DOUBLE);
#endif /*PACK*/
			}
		}
		if (ismaster)
		{
			TIMER_STOP;
			t = (int) TIMER_ELAPSED / SAMPLE;
			printf("Roundtrip T = %d (us)  (%.4f MB/s)  Data size: %d\n",
				t, 2.0*8.0*(float)size/(float)t, ((int)sizeof(double))*size);
		}
	}
}


int
print_header(packtype, size, niter)
int packtype;
int size;
int niter;
{
	printf("--- Simple PVM Bandwidth Test ----\n");
	printf(" Using pack option: %s \n",
		(packtype == PvmDataRaw ? "PvmDataRaw" : 
		(packtype == PvmDataInPlace ? "PvmDataInPlace" :
		(packtype == PvmDataDefault ? "PvmDataDefault" :
		(packtype == -1 ? "Psend/Precv" : "Unknown")))));
	printf(" Max data size is: %d \n", size);
	printf(" Number of iterations/sample: %d \n", niter);
	printf(" \n\n");
	printf(" Roundtrip time is measured from user-space to user-space.\n");
	if ( packtype != -1)
	{
		printf(" For packed messages this includes the combined time of: \n"); 
		printf("    inst 0: pvm_initsend() \n"); 
		printf("    inst 0: pvm_pack() \n"); 
		printf("    inst 0: pvm_send() \n"); 
		printf("    inst 1: pvm_recv() \n");
		printf("    inst 1: pvm_unpack() \n");
		printf("    inst 1: pvm_initsend() \n");
		printf("    inst 1: pvm_pack() \n");
		printf("    inst 1: pvm_send() \n");
		printf("    inst 0: pvm_recv() \n");
		printf("    inst 0: pvm_unpack() \n");
	}
	else
	{
		printf(" For pvm_psend/precv this includes the combined time of: \n"); 
		printf("    inst 0: pvm_psend() \n"); 
		printf("    inst 1: pvm_precv() \n"); 
		printf("    inst 1: pvm_psend() \n"); 
		printf("    inst 0: pvm_precv() \n");
	}
	
	printf("\n\n---------------------------------------\n");
	
	return 0;
}
