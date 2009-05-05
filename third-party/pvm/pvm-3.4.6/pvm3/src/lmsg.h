
/* $Id: lmsg.h,v 1.4 1997/06/25 22:08:51 pvmsrc Exp $ */

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
 *	lmsg.h
 *
 * structures and protos to handle message structures
 *
 */

#ifndef __LMSG_H__
#define __LMSG_H__

#include "mppmsg.h"
struct msgid {
	int magic;			/* magic number of this structure */
	msgmid_t id;		/* handle of message */
	int tag;			/* tag of message */
	int otid;			/* task id of "other" tid (sendto or recvfrom) */
	int ctxt;			/* context of the message */
	int complete;		/* message is complete */
	int len;
	info_t info[MPPINFOSIZE];
	char *ubuf;
	MSGFUNC_PTR mfunc;	/* low level messaging used for this message id */
	struct msgid *ms_link; /* might put these in a dll */
	struct msgid *ms_rlink;
};

struct msgid * msgid_new __ProtoGlarp__(( ));
void msgid_free __ProtoGlarp__( (struct msgid *) );

#endif
