
/* $Id: job.h,v 1.8 2000/02/15 18:06:21 pvmsrc Exp $ */

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
 *	job.h
 *
 *	Job descriptors.
 *
 * $Log: job.h,v $
 * Revision 1.8  2000/02/15 18:06:21  pvmsrc
 * Modified jobs_cmd() to dump task ids for traced jobs...
 * 	- can't use output buffers, must use TRC_ID->tevtask_list list.
 * 	- snagged necessary state constants from trclib.h, can't just
 * 		#include it, as it #includes fricking everything...  :-Q
 * (Spanker=kohl)
 *
 * Revision 1.7  1999/07/08 18:59:41  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.6  1998/01/28  23:03:43  pvmsrc
 * Fixed tracing bogusness!
 * 	- host add / del notifies for tracing were getting scrunched by
 * 		console host add notify, etc...
 * 	- moved message / notify tag constants to job.h and USED THEM.
 * 	- initialized nextjob properly...
 * 	- added new "joboffset" global in cons.c for dumping more
 * 		reasonable job numbers to user.
 * (Spanker=kohl)
 *
 * Revision 1.5  1997/07/09  13:21:09  pvmsrc
 * Fixed Author Header.
 *
 * Revision 1.4  1997/01/28  19:13:23  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.3  1996/10/24  23:04:56  pvmsrc
 * Added new #include "trcdef.h" for tracer library.
 * 	- added TRC_ID j_trcid field to job struct.
 *
 * Revision 1.2  1996/09/26  21:13:42  pvmsrc
 * Spanked the stupid CVS "Id" strings.
 * 	- comments for headers, not rcsid's stupid...  :-Q
 *
 * Revision 1.1  1996/09/23  20:25:42  pvmsrc
 * Initial revision
 *
 * Revision 1.2  1994/06/03  20:01:52  manchek
 * version 3.3.0
 *
 * Revision 1.1  1993/08/30  23:30:32  manchek
 * Initial revision
 *
 */


/* include new tracer library header */

#include "trcdef.h"


struct job {
	struct job *j_link, *j_rlink;		/* chain of all jobs */
	int j_jid;							/* unique job id */
	int j_flag;							/* flags */
#define	JOB_TRACE	1
	TRC_ID j_trcid;						/* tracer library handler ID */
	struct obuf *j_obufs;				/* head of obuf list, null if trace */
	FILE *j_ff;							/* if writing to file, else -1 */
};

#define	GOTSPAWN	1
#define	GOTEOF		2

/* Task Trace Status Constants - copied from trclib.h */
#define TRC_TASK_NOLIFE     0
#define TRC_TASK_ALIVE      1
#define TRC_TASK_DEAD       2
#define TRC_TASK_IGNORE     3
#define TRC_TASK_WAITEND    4

/* Task Output Status Constants - also copied from trclib.h */
#define TRC_TASK_NOOUT      0
#define TRC_TASK_OUT        1
#define TRC_TASK_EOF        2
#define TRC_TASK_WAITEOF    3

#define	HostsAddedTag	0
#define	MyExitTag		1
#define TrcHostAddTag	2
#define TrcHostDelTag	3
#define	FirstJobTag		4

/*
*	used to collect output from a task
*/

struct obuf {
	struct obuf *o_link, *o_rlink;		/* chain of all tasks in job */
	int o_tid;							/* task id */
	char *o_buf;						/* buffered partial line */
	int o_len;							/* length of buf */
	int o_maxl;							/* space in buf */
	int o_flag;							/* task state */
};

int job_init __ProtoGlarp__(( void ));
struct job *job_new __ProtoGlarp__(( int jid ));
struct job *job_find __ProtoGlarp__(( int jid ));
int job_free __ProtoGlarp__(( struct job *jp ));
struct obuf * obuf_new __ProtoGlarp__(( struct job *jp, int tid ));
struct obuf * obuf_find __ProtoGlarp__(( struct job *jp, int tid ));
int obuf_free __ProtoGlarp__(( struct job *jp, struct obuf *op ));
int obuf_dump __ProtoGlarp__(( struct job *jp ));
int checkoutput __ProtoGlarp__(( struct job *jp, int cc, int len, int cod, int src ));

