
static char rcsid[] =
	"$Id: pvmdpack.c,v 1.12 2005/08/22 15:13:20 pvmsrc Exp $";

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
 *	pvmdpack.c
 *
 *	Pvmd message packing and unpacking.
 *
 * $Log: pvmdpack.c,v $
 * Revision 1.12  2005/08/22 15:13:20  pvmsrc
 * Added #include <pvmtev.h> for #include global.h...  :-Q
 * 	- submitted by Skipper Hartley <charles.l.hartley@bankofamerica.com>
 * (Spanker=kohl)
 *
 * Revision 1.11  2001/02/07 23:15:53  pvmsrc
 * 2nd Half of CYGWIN Check-ins...
 * (Spanker=kohl)
 *
 * Revision 1.10  2000/02/16 21:59:49  pvmsrc
 * Fixed up #include <sys/types.h> stuff...
 * 	- use <bsd/sys/types.h> for IMA_TITN...
 * 	- #include before any NEEDMENDIAN #includes...
 * (Spanker=kohl)
 *
 * Revision 1.9  1999/07/08 19:00:10  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.8  1998/11/20  20:06:40  pvmsrc
 * Changes so that win32 will compile & build. Also, common
 * Changes so that compiles & builds on NT. Also
 * common source on win32 & unix.
 * (spanker=sscott)
 *
 * Revision 1.7  1997/11/04  23:21:41  pvmsrc
 * Added SYSVSTR stuff.
 * (Spanker=kohl)
 *
 * Revision 1.6  1997/06/27  17:32:51  pvmsrc
 * Updated for WIN32 header files & Authors.
 *
 * Revision 1.5  1997/06/24  20:39:18  pvmsrc
 * Eliminated unnecessary global externs.
 *
 * Revision 1.4  1997/04/30  21:26:26  pvmsrc
 * SGI Compiler Warning Cleanup.
 *
 * Revision 1.3  1997/01/28  19:27:22  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.2  1996/10/24  21:04:49  pvmsrc
 * Moved #include of "global.h" down below other headers:
 * 	- need to have all of the structures / types declared before
 * 		the globals can be declared...
 *
 * Revision 1.1  1996/09/23  23:44:32  pvmsrc
 * Initial revision
 *
 *
 */

#ifdef NEEDMENDIAN
#include <sys/types.h>
#include <machine/endian.h>
#endif
#ifdef NEEDENDIAN
#include <endian.h>
#endif
#ifdef NEEDSENDIAN
#include <sys/endian.h>
#endif

#include <pvm3.h>

#if defined(WIN32) || defined(CYGWIN)
#include "..\xdr\types.h"
#include "..\xdr\xdr.h"
#else
#include <rpc/types.h>
#include <rpc/xdr.h>
#endif

#ifdef  SYSVSTR
#include <string.h>
#else
#include <strings.h>
#endif

#include "pvmalloc.h"
#include "pvmfrag.h"
#include "pmsg.h"
#include "listmac.h"
#include <pvmtev.h>
#include "global.h"


/***************
 **  Private  **
 **           **
 ***************/


/*****************
 **  Interface  **
 **             **
 *****************/


/*	pkbyte()
*
*	Pack bytes into a message.
*	Returns 0 if ok.
*/

int
pkbyte(mp, p, n)
	struct pmsg *mp;		/* message to pack */
	char *p;
	int n;
{
	return (mp->m_codef->enc_byte) (mp, (void*)p, n, 1, 1);
}


/*	upkbyte()
*
*	Unpack bytes from a message.
*	Returns 0 if ok.
*/

int
upkbyte(mp, p, n)
	struct pmsg *mp;		/* message to unpack */
	char *p;				/* unpack into */
	int n;
{
/*
	pvmlogprintf("upkbyte(0x%lx, 0x%lx, %d)\n", (long)mp, (long)p, n);
*/
	return (mp->m_codef->dec_byte) (mp, (void*)p, n, 1, 1);
}


/*	pkint()
*
*	Pack a integer into a message.
*	Returns 0 if ok.
*/

int
pkint(mp, i)
	struct pmsg *mp;		/* message to pack */
	int i;					/* int to pack */
{
	return (mp->m_codef->enc_int) (mp, (void*)&i, 1, 1, sizeof(int));
}


/*	upkint()
*
*	Unpack a signed integer from a message.
*	Returns 0 if ok.
*/

int
upkint(mp, np)
	struct pmsg *mp;		/* message to unpack */
	int *np;				/* int to unpack into */
{
	return (mp->m_codef->dec_int) (mp, (void*)np, 1, 1, sizeof(int));
}


/*	upkuint()
*
*	Unpack an unsigned integer from a message.
*	Returns 0 if ok.
*/

int
upkuint(mp, np)
	struct pmsg *mp;		/* message to unpack */
	int *np;				/* int to unpack into */
{
	return (mp->m_codef->dec_uint) (mp, (void*)np, 1, 1, sizeof(int));
}


/*	pkstr()
*
*	Pack a null-term string into a message.
*	Returns 0 if ok.
*/

int
pkstr(mp, s)
	struct pmsg *mp;		/* message to pack */
	char *s;				/* string to pack */
{
	int cc;
	int l = strlen(s) + 1;

	if (!(cc = (mp->m_codef->enc_int) (mp, (void*)&l, 1, 1, sizeof(int))))
		cc = (mp->m_codef->enc_byte) (mp, (void*)s, l, 1, 1);
	return cc;
}


/*	upkstr()
*
*	Unpack a string from a message.  Result is null-terminated.
*	Any length greater than mlen is discarded from message..
*	Returns 0 if ok.
*/

int
upkstr(mp, s, mlen)
	struct pmsg *mp;		/* message to unpack */
	char *s;				/* space to unpack in */
	int mlen;				/* max bytes to unpack incl null */
{
	int cc;
	int l;

	if (!(cc = (mp->m_codef->dec_int) (mp, (void*)&l, 1, 1, sizeof(int)))) {
		if (l <= 0)
			cc = PvmNoData;
		else if (l <= mlen)
			cc = (mp->m_codef->dec_byte) (mp, (void*)s, l, 1, 1);
		else
			cc = PvmNoMem;
	}
	return cc;
}


/*	upkstralloc()
*
*	Unpack a string from a message.  Result is null-terminated,
*	and in dynamic space..
*	Returns 0 if ok.
*/

int
upkstralloc(mp, ss)
	struct pmsg *mp;		/* message to unpack */
	char **ss;				/* return pointer */
{
	int cc;
	int l;

	if (!(cc = (mp->m_codef->dec_int) (mp, (void*)&l, 1, 1, sizeof(int)))) {
		if (l <= 0)
			cc = PvmNoData;
		else {
			*ss = TALLOC(l, char, "ustr");
			if ((cc = (mp->m_codef->dec_byte) (mp, (void*)*ss, l, 1, 1)) < 0) {
				PVM_FREE(*ss);
				*ss = 0;
			}
		}
	}
	return cc;
}


/*	mesg_rewind()
*
*	A hack to rewind a message.
*/

void
mesg_rewind(mp)
	struct pmsg *mp;
{
	mp->m_flag &= ~MM_UPACK;
	(mp->m_codef->dec_init)(mp);
}


