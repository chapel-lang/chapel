
/* $Id: pmsg.h,v 1.5 2001/02/07 23:15:51 pvmsrc Exp $ */

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
 *	pmsg.h
 *
 *	Libpvm and pvmd message descriptors.
 *
 * $Log: pmsg.h,v $
 * Revision 1.5  2001/02/07 23:15:51  pvmsrc
 * 2nd Half of CYGWIN Check-ins...
 * (Spanker=kohl)
 *
 * Revision 1.4  1999/07/08 19:00:22  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.3  1997/06/27  17:32:50  pvmsrc
 * Updated for WIN32 header files & Authors.
 *
 * Revision 1.2  1997/01/28  19:28:17  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.1  1996/09/23  23:43:24  pvmsrc
 * Initial revision
 *
 *
 */

/*
* vector of pack/unpack routines for an encoding
*/

#ifdef WIN32
#include "..\xdr\types.h"
#include "..\xdr\xdr.h"
#endif

struct encvec {
	int (*enc_init)();	/* initialize write */
	int (*dec_init)();	/* initialize read */
	int (*enc_byte)();	/* encode bytes */
	int (*dec_byte)();	/* decode bytes */
	int (*enc_short)();	/* encode shorts */
	int (*dec_short)();	/* ... */
	int (*enc_int)();
	int (*dec_int)();
	int (*enc_long)();
	int (*dec_long)();
	int (*enc_ushort)();
	int (*dec_ushort)();
	int (*enc_uint)();
	int (*dec_uint)();
	int (*enc_ulong)();
	int (*dec_ulong)();
	int (*enc_float)();
	int (*dec_float)();
	int (*enc_double)();
	int (*dec_double)();
	int (*enc_cplx)();
	int (*dec_cplx)();
	int (*enc_dcplx)();
	int (*dec_dcplx)();
};

/*
* descriptor for a message
*/

struct pmsg {
	struct pmsg *m_link;	/* chain or 0 */
	struct pmsg *m_rlink;
	struct encvec *m_codef;	/* data encoders/decoders */
	struct frag *m_frag;	/* master frag or 0 if we're list head */
	struct frag *m_cfrag;	/* pack/unpack position */
	int m_ref;				/* refcount */
	int m_mid;				/* message id */
	int m_len;				/* total body length */
	int m_ctx;				/* communication context */
	int m_tag;				/* type tag */
	int m_wid;				/* wait id */
	int m_src;				/* src address */
	int m_dst;				/* dst address */
	int m_enc;				/* encoding signature */
	int m_flag;
	int m_cpos;				/* pack/unpack position */
	int m_crc;				/* CRC from header */
	XDR m_xdr;
};

#define	MM_PACK		1		/* initialized for packing */
#define	MM_UPACK	2		/* initialized for unpacking */
#define	MM_PRIO		4		/* put message at head of queue */
#ifdef IMA_CSPP
#define	MM_DIFFNODE	256		/* message came from different (node?) */
#endif

int pvmgetdsig __ProtoGlarp__ (( ));
struct pmsg * pmsg_new __ProtoGlarp__ (( int master ));
void pmsg_unref __ProtoGlarp__ (( struct pmsg *mp ));
int pmsg_extend __ProtoGlarp__ (( struct pmsg *mp ));
int pmsg_decmore __ProtoGlarp__ (( struct pmsg *mp ));
int pmsg_setlen __ProtoGlarp__ (( struct pmsg *mp ));
int pmsg_setenc __ProtoGlarp__ (( struct pmsg *mp, int enc ));
int pmsg_dump __ProtoGlarp__ (( struct pmsg *mp, int lvl ));

