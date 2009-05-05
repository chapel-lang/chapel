
static char rcsid[] =
	"$Id: lpvmpack.c,v 1.21 2001/02/07 23:14:08 pvmsrc Exp $";

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
 *	lpvmpack.c
 *
 *	Typed packing/unpacking, message buffer manip.
 *
 * $Log: lpvmpack.c,v $
 * Revision 1.21  2001/02/07 23:14:08  pvmsrc
 * First Half of CYGWIN Check-ins...
 * (Spanker=kohl)
 *
 * Revision 1.20  2001/02/05 23:05:55  pvmsrc
 * Fixed va_arg() issue.
 * 	- shouldn't use "char" or "short", always use at least "int"...
 * 	- error reported by gcc on latest Linux, documentation also
 * 		indicates this guideline on SGI and other arches...
 * 	- new test suite test validates functionality on SGI (of course,
 * 		it worked before, too :-).
 * 	- here's hoping this won't break anything...  :-o
 * (Spanker=kohl)
 *
 * Revision 1.19  2000/02/16 21:59:46  pvmsrc
 * Fixed up #include <sys/types.h> stuff...
 * 	- use <bsd/sys/types.h> for IMA_TITN...
 * 	- #include before any NEEDMENDIAN #includes...
 * (Spanker=kohl)
 *
 * Revision 1.18  1999/11/08 17:44:34  pvmsrc
 * SGI compiler cleanup.
 * (Spanker=kohl)
 *
 * Revision 1.17  1999/08/12 15:12:19  pvmsrc
 * Fixed error in arg to check_for_exit().
 * 	- mp freed before mp->m_src passed in...!  D-Oh!
 * 	- bug submitted by Einar Arne Soerheim <Einar.Sorheim@ife.no>.
 * (Spanker=kohl)
 *
 * Revision 1.16  1999/07/08 19:00:01  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.15  1998/11/23  20:03:38  pvmsrc
 * In umbuf_free() must check_for_exit() after pmsg_unref() or else we
 * might splat a message structure before we try decrement it ref
 * count.. i.e sigfault.
 * Ops.
 * (Spanker=fagg)
 *
 * Revision 1.14  1998/11/20  20:04:08  pvmsrc
 * Changes so that win32 will compile & build. Also, common
 * Changes so that compiles & builds on NT. Also
 * common source on win32 & unix.
 * (Spanker=sscott)
 *
 * Revision 1.13  1998/11/20  19:30:09  pvmsrc
 * TC_TASKEXIT messages were setting pp->p_exited but check_for_exit()
 * was not being called as it got lost when umbuf_free() was updated
 * to use pmsg functions.
 * Added check_for_exit() back so that it could call peer_detach()
 * and thus free shared memory segments again...
 * A happy bug fix for Sun(tm).
 * (Spanker=fagg)
 *
 * Revision 1.12  1997/11/04  23:21:38  pvmsrc
 * Added SYSVSTR stuff.
 * (Spanker=kohl)
 *
 * Revision 1.11  1997/08/25  21:47:11  pvmsrc
 * Fixed pvm_setsbuf() problem with user-defined trace events.
 * 	- setsbuf was re-calling enc_trc_init(), pushing second header.
 * 	- gutted enc_trc_init() -> extracted into new enc_trc_hdr() routine
 * 		that pvm_mkbuf() calls ONCE...
 * (Spanker=kohl)
 *
 * Revision 1.10  1997/06/27  17:32:29  pvmsrc
 * Updated for WIN32 header files & Authors.
 *
 * Revision 1.9  1997/05/07  21:23:29  pvmsrc
 * Added new pvmupkstralloc() for dynamic string unpacking lunacy.
 * 	- with proto in header.
 *
 * Revision 1.8  1997/05/01  20:05:30  pvmsrc
 * Made pvmmidh and pvmmidhsiz global so that lpvmshmem can see
 * them..... [GEF]
 *
 * Revision 1.7  1997/05/01  14:11:48  pvmsrc
 * SGI Compiler Warning Cleanup.
 *
 * Revision 1.6  1997/01/28  19:26:50  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.5  1996/10/25  13:57:29  pvmsrc
 * Replaced old #includes for protocol headers:
 * 	- <pvmsdpro.h>, "ddpro.h", "tdpro.h"
 * With #include of new combined header:
 * 	- <pvmproto.h>
 *
 * Revision 1.4  1996/10/24  22:25:59  pvmsrc
 * Moved #include "global.h" below other #include's for typing.
 * Added #include of new "lpvm.h" header to replace explicit externs.
 * Modified trace event generation for message / packing routines:
 * 	- replaced old TEV_*[01] constants with new TEV_EVENT_ENTRY/EXIT.
 * 	- replaced standard pvm_pk*() calls with new TEV_PACK_*() macro
 * 		calls and TEV_DID_* identification.
 * 	- in pvm_pkstr(), don't pack string length for user-defined packing
 * 		(PvmDataTrace encoding).
 *
 * Revision 1.3  1996/10/23  15:57:25  pvmsrc
 * Changed pvm_freebuf so that freeing the null buffer '0' no longer
 * causes an error message. Many old 3.3 codes showed this through the
 * use of pvm_freebuf(pvm_setsbuf(x)) etc where there was no old send
 * buffer and hense under 3.4 an error was caused.
 *
 * This saves having to change BLACS, ParckBench and some collective
 * Group operations.
 *
 * When freeing buffer 0, PvmOK is returned.
 *
 * Revision 1.2  1996/09/23  23:28:55  pvmsrc
 * Initial Creation - original lpvmpack.c.
 *
 * Revision 1.12  1995/11/02  16:13:16  manchek
 * added NEEDSENDIAN switch
 *
 * Revision 1.11  1995/09/05  19:17:03  manchek
 * fp sometimes wasn't set in enc_xdr_init.
 * overflow test was wrong in enc_xdr_long
 *
 * Revision 1.10  1995/07/28  16:04:06  manchek
 * switch endian includes on flag, not arch name
 *
 * Revision 1.9  1995/07/19  20:15:45  manchek
 * enc*init should append a frag to message if there's already one
 *
 * Revision 1.8  1995/06/27  21:51:13  manchek
 * update fr_num_unpacked before pre_bcopy in byteupk
 *
 * Revision 1.7  1995/05/17  16:19:34  manchek
 * added support for CSPP shared memory
 *
 * Revision 1.6  1995/02/01  21:22:27  manchek
 * new function enc_xdr_ulong.
 * added overflow tests to enc_xdr_long, enc_xdr_ulong, return PvmOverflow
 *
 * Revision 1.5  1994/11/09  15:01:58  manchek
 * pvm_pkstr and pvm_packf(%s) must fail with PvmNotImpl if PvmDataInPlace
 * is used.
 *
 * Revision 1.4  1994/06/03  21:15:12  manchek
 * oops, missed a bcopy
 *
 * Revision 1.3  1994/06/03  20:38:17  manchek
 * version 3.3.0
 *
 * Revision 1.2  1993/10/04  19:20:22  manchek
 * fixed vpackf() - floats get passed as doubles, must unpack that way
 *
 * Revision 1.1  1993/08/30  23:26:48  manchek
 * Initial revision
 *
 */
#ifdef WIN32
#include "pvmwin.h"
#endif
#include <stdio.h>
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

#include <ctype.h>
#ifdef	__STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif
#ifdef  SYSVSTR
#include <string.h>
#else
#include <strings.h>
#endif

#include <pvmproto.h>
#include <pvmtev.h>
#include "pvmalloc.h"
#include "pvmfrag.h"
#include "pmsg.h"
#include "listmac.h"
#include "bfunc.h"
#include "lpvm.h"
#include "tevmac.h"
#include "global.h"

/* message heap element */

struct mhp {
	int m_free;					/* free list (next or 0) */
	struct pmsg *m_umb;			/* message or null if on free list */
};


/***************
 **  Globals  **
 **           **
 ***************/

/* XXX these moved here from Private list so lpvmshmem can find them */
struct mhp *pvmmidh = 0;			/* heap of all msgs */
int pvmmidhsiz = 0;				/* length of pvmmidh */

/***************
 **  Private  **
 **           **
 ***************/

static int pvmmidhfree = 0;				/* head of midh free list */


/***********************************************
 **  Umbuf (pmsg wrapper with mid) Functions  **
 **                                           **
 ***********************************************/


/*	mid_new()
*
*	Allocate a message id for a new message.
*	Returns mid (>0), or PvmNoMem if can't get memory.
*/

static int
mid_new(mp)
	struct pmsg *mp;
{
	static int nxtmidhsiz;
	int mid;
	int ns;
	struct mhp *newh;

	/* check free list empty */

	if (!pvmmidhfree) {

	/* extend midh array */

		if (pvmmidhsiz) {
			ns = nxtmidhsiz;
			if (!(newh = TREALLOC(pvmmidh, ns, struct mhp)))
				return PvmNoMem;
			nxtmidhsiz += pvmmidhsiz;

		} else {
			nxtmidhsiz = 13;
			ns = 8;
			if (!(newh = TALLOC(ns, struct mhp, "mid")))
				return PvmNoMem;
		}
		pvmmidh = newh;

	/* put new entries on free list */

		while (pvmmidhsiz < ns) {
			pvmmidh[pvmmidhsiz].m_umb = 0;
			pvmmidh[pvmmidhsiz].m_free = pvmmidhfree;
			pvmmidhfree = pvmmidhsiz++;
		}
	}

	/* take next from free list */

	mid = pvmmidhfree;
	pvmmidhfree = pvmmidh[pvmmidhfree].m_free;

	pvmmidh[mid].m_umb = mp;
	mp->m_mid = mid;
	return mid;
}


/*	mid_free()
*
*	Return a message id to the free list.
*/

static void
mid_free(mid)
	int mid;
{
	if (mid <= 0 || mid >= pvmmidhsiz || !pvmmidh[mid].m_umb)
		return;
	pvmmidh[mid].m_umb = 0;
	pvmmidh[mid].m_free = pvmmidhfree;
	pvmmidhfree = mid;
}


/*	struct ()
*
*	Create a new empty umbuf.
*/

struct pmsg *
umbuf_new()
{
	struct pmsg *mp;

	if (mp = pmsg_new(0)) {
		if (mid_new(mp) < 0) {
			pmsg_unref(mp);
			mp = 0;
		}
	}
	return mp;
}

void check_for_free();
/* we need the def here before we call it from umbuf_free() */


/*	umbuf_free()
*
*	Free an umbuf
*/

int
umbuf_free(mp)
	struct pmsg *mp;
{
	int src;

	mid_free( mp->m_mid );
	mp->m_mid = 0;

	src = mp->m_src;

	pmsg_unref( mp );

	/* check to see if an TC_TASKEXIT message has been caught */
	check_for_exit( src );

	return 0;
}


/*	midtobuf()
*
*	Return pointer to message buffer given int message id.
*/

struct pmsg *
midtobuf(mid)
	int mid;
{
	return ((mid > 0 && mid < pvmmidhsiz) ? pvmmidh[mid].m_umb : 0);
}


int
umbuf_dump(mid, lvl)
	int mid;
	int lvl;	/* 0 - summary, 1 - frag lengths, 2 - frag data */
{
	struct pmsg *mp;
	struct frag *fp;

	if (mid <= 0) {
		pvmlogprintf("umbuf_dump() BadParam\n");
		return PvmBadParam;
	}
	if (!(mp = midtobuf(mid))) {
		pvmlogprintf("umbuf_dump() NoSuchBuf\n");
		return PvmNoSuchBuf;
	}

	mp = pvmmidh[mid].m_umb;
	pmsg_dump(mp, lvl);
	return 0;
}


int
umbuf_list(lvl)
	int lvl;
{
	int i;
	struct frag *fp;
	int rlen;

	for (i = 1; i < pvmmidhsiz; i++)
		if (pvmmidh[i].m_umb)
			umbuf_dump(i, lvl);
	return 0;
}


/************************
 **  Libpvm Functions  **
 **                    **
 ************************/


int
pvm_mkbuf(enc)
	int enc;		/* data format */
{
	struct pmsg *mp;
	struct frag *fp;
	int dotrchdr = 0;
	int cc = 0;
	int tmp;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_MKBUF,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_ME, TEV_DATA_SCALAR, &enc, 1, 1 );
			TEV_FIN;
		}
	}

	switch (enc) {								/* verify encoding */
	case PvmDataDefault:
		enc = 0x10000000;		/* XXX this just forces XDR */
		break;

	case PvmDataRaw:
		enc = pvmmydsig;
		break;

	case PvmDataInPlace:
		enc = 0x20000000;
		break;

	case PvmDataTrace:
		enc = 0x40000000;
		dotrchdr++;
		break;

	default:
		cc = PvmBadParam;
	}

	if (!cc) {
		if (mp = umbuf_new()) {
			mp->m_src = pvmmytid;
			pmsg_setenc(mp, enc);
			cc = mp->m_mid;
			if (dotrchdr) {
				tmp = enc_trc_hdr(mp);
				cc = ( tmp < 0 ) ? tmp : cc;
			}
		} else
			cc = PvmNoMem;
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_MKBUF,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_MB, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_mkbuf", cc);
	return cc;
}


int
pvm_freebuf(mid)
	int mid;		/* message handle */
{
	int cc;
	struct pmsg *mp;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_FREEBUF,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_MB, TEV_DATA_SCALAR, &mid, 1, 1 );
			TEV_FIN;
		}
	}

	if (mid < 0) {
		cc = PvmBadParam;
	} else if (mp = midtobuf(mid)) {
		if (pvmsbuf == mp)
			pvmsbuf = 0;
		if (pvmrbuf == mp)
			pvmrbuf = 0;
		umbuf_free(mp);
		cc = 0;
	} else if (mid)
		cc = PvmNoSuchBuf;
	else
		cc = PvmOk;	/* don't complain when freeing the null/0 buffer */
				/* many 3.3 codes switch and free without checking */

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_FREEBUF,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_freebuf", cc);
	return cc;
}


int
pvm_setsbuf(mid)
	int mid;
{
	struct pmsg *mp = 0;
	int cc = 0;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_SETSBUF,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_MB, TEV_DATA_SCALAR, &mid, 1, 1 );
			TEV_FIN;
		}
	}

	if (mid < 0) {
		cc = PvmBadParam;
	} else if (mid == 0) {
		cc = pvmsbuf ? pvmsbuf->m_mid : 0;
		pvmsbuf = 0;
	} else if (mp = midtobuf(mid)) {
		if (mp == pvmrbuf)
			pvmrbuf = 0;
		cc = pvmsbuf ? pvmsbuf->m_mid : 0;
		pvmsbuf = mp;
		(mp->m_codef->enc_init)(mp);
	} else
		cc = PvmNoSuchBuf;

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_SETSBUF,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_MSB, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_setsbuf", cc);
	return cc;
}


int
pvm_getsbuf()
{
	int cc;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_GETSBUF,TEV_EVENT_ENTRY))
			TEV_FIN;
	}

	cc = pvmsbuf ? pvmsbuf->m_mid : 0;

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_GETSBUF,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_MSB, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	return cc;
}


int
pvm_setrbuf(mid)
	int mid;
{
	struct pmsg *mp = 0;
	int cc = 0;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_SETRBUF,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_MB, TEV_DATA_SCALAR, &mid, 1, 1 );
			TEV_FIN;
		}
	}

	if (mid < 0) {
		cc = PvmBadParam;
	} else if (mid == 0) {
		cc = pvmrbuf ? pvmrbuf->m_mid : 0;
		pvmrbuf = 0;
	} else if (mp = midtobuf(mid)) {
		if (mp == pvmsbuf)
			pvmsbuf = 0;
		cc = pvmrbuf ? pvmrbuf->m_mid : 0;
		pvmrbuf = mp;
		(mp->m_codef->dec_init)(mp);
	} else
		cc = PvmNoSuchBuf;

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_SETRBUF,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_MRB, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_setrbuf", cc);
	return cc;
}


int
pvm_getrbuf()
{
	int cc;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_GETRBUF,TEV_EVENT_ENTRY))
			TEV_FIN;
	}

	cc = pvmrbuf ? pvmrbuf->m_mid : 0;

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_GETRBUF,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_MRB, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	return cc;
}


int
pvm_initsend(enc)
	int enc;
{
	int cc;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_INITSEND,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_ME, TEV_DATA_SCALAR, &enc, 1, 1 );
			TEV_FIN;
		}
	}

	if ((cc = pvm_mkbuf(enc)) >= 0) {
		if (pvmsbuf)
			pvm_freebuf(pvmsbuf->m_mid);
		pvm_setsbuf(cc);
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_INITSEND,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_MSB, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_initsend", cc);
	return cc;
}


int
pvm_bufinfo(mid, len, code, tid)
	int mid;
	int *len;
	int *code;
	int *tid;
{
	struct pmsg *mp;
	int cc;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_BUFINFO,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_MB, TEV_DATA_SCALAR, &mid, 1, 1 );
			TEV_FIN;
		}
	}

	if (mid <= 0)
		cc = PvmBadParam;
	else
		if (mp = midtobuf(mid)) {
			cc = PvmOk;
			if (len) {
				if (mp->m_flag & MM_PACK)
					pmsg_setlen(mp);
				*len = mp->m_len;
			}
			if (code)
				*code = mp->m_tag;
			if (tid)
				*tid = mp->m_src;
		} else
			cc = PvmNoSuchBuf;

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_BUFINFO,TEV_EVENT_EXIT)) {
			int ln, tg, sc;
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			if (!cc) {
				ln = mp->m_len;
				tg = mp->m_tag;
				sc = mp->m_src;
			}
			else
				ln = tg = sc = cc;
			TEV_PACK_INT( TEV_DID_INB, TEV_DATA_SCALAR, &ln, 1, 1 );
			TEV_PACK_INT( TEV_DID_IMC, TEV_DATA_SCALAR, &tg, 1, 1 );
			TEV_PACK_INT( TEV_DID_IST, TEV_DATA_SCALAR, &sc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_bufinfo", cc);
	return cc;
}


int
pvm_pkbyte(cp, cnt, std)
	char *cp;
	int cnt;
	int std;
{
	int cc;
	long ad;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_PKBYTE,TEV_EVENT_ENTRY)) {
			ad = (long)cp;
			TEV_PACK_LONG( TEV_DID_PDA, TEV_DATA_SCALAR, &ad, 1, 1 );
			TEV_PACK_INT( TEV_DID_PC, TEV_DATA_SCALAR, &cnt, 1, 1 );
			TEV_PACK_INT( TEV_DID_PSD, TEV_DATA_SCALAR, &std, 1, 1 );
			TEV_FIN;
		}
	}

	if (cnt < 0)
		cc = PvmBadParam;
	else if (!pvmsbuf)
		cc = PvmNoBuf;
	else
		cc = (pvmsbuf->m_codef->enc_byte) (pvmsbuf, (void*)cp, cnt, std, 1);

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_PKBYTE,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}
	return (cc < 0 ? lpvmerr("pvm_pkbyte", cc) : PvmOk);
}


int
pvm_upkbyte(cp, cnt, std)
	char *cp;
	int cnt;
	int std;
{
	int cc;
	long ad;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_UPKBYTE,TEV_EVENT_ENTRY)) {
			ad = (long)cp;
			TEV_PACK_LONG( TEV_DID_PDA, TEV_DATA_SCALAR, &ad, 1, 1 );
			TEV_PACK_INT( TEV_DID_PC, TEV_DATA_SCALAR, &cnt, 1, 1 );
			TEV_PACK_INT( TEV_DID_PSD, TEV_DATA_SCALAR, &std, 1, 1 );
			TEV_FIN;
		}
	}

	if (cnt < 0)
		cc = PvmBadParam;
	else if (!pvmrbuf)
		cc = PvmNoBuf;
	else
		cc = (pvmrbuf->m_codef->dec_byte) (pvmrbuf, (void*)cp, cnt, std, 1);

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_UPKBYTE,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}
	return (cc < 0 ? lpvmerr("pvm_upkbyte", cc) : PvmOk);
}


int
pvm_pkcplx(xp, cnt, std)
	float *xp;
	int cnt;
	int std;
{
	int cc;
	long ad;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_PKCPLX,TEV_EVENT_ENTRY)) {
			ad = (long)xp;
			TEV_PACK_LONG( TEV_DID_PDA, TEV_DATA_SCALAR, &ad, 1, 1 );
			TEV_PACK_INT( TEV_DID_PC, TEV_DATA_SCALAR, &cnt, 1, 1 );
			TEV_PACK_INT( TEV_DID_PSD, TEV_DATA_SCALAR, &std, 1, 1 );
			TEV_FIN;
		}
	}

	if (cnt < 0)
		cc = PvmBadParam;
	else if (!pvmsbuf)
		cc = PvmNoBuf;
	else
		cc = (pvmsbuf->m_codef->enc_cplx)
				(pvmsbuf, (void*)xp, cnt, std, sizeof(float)*2);

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_PKCPLX,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}
	return (cc < 0 ? lpvmerr("pvm_pkcplx", cc) : PvmOk);
}


int
pvm_upkcplx(xp, cnt, std)
	float *xp;
	int cnt;
	int std;
{
	int cc;
	long ad;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_UPKCPLX,TEV_EVENT_ENTRY)) {
			ad = (long)xp;
			TEV_PACK_LONG( TEV_DID_PDA, TEV_DATA_SCALAR, &ad, 1, 1 );
			TEV_PACK_INT( TEV_DID_PC, TEV_DATA_SCALAR, &cnt, 1, 1 );
			TEV_PACK_INT( TEV_DID_PSD, TEV_DATA_SCALAR, &std, 1, 1 );
			TEV_FIN;
		}
	}

	if (cnt < 0)
		cc = PvmBadParam;
	else if (!pvmrbuf)
		cc = PvmNoBuf;
	else
		cc = (pvmrbuf->m_codef->dec_cplx)
				(pvmrbuf, (void*)xp, cnt, std, sizeof(float)*2);

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_UPKCPLX,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}
	return (cc < 0 ? lpvmerr("pvm_upkcplx", cc) : PvmOk);
}


int
pvm_pkdcplx(zp, cnt, std)
	double *zp;
	int cnt;
	int std;
{
	int cc;
	long ad;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_PKDCPLX,TEV_EVENT_ENTRY)) {
			ad = (long)zp;
			TEV_PACK_LONG( TEV_DID_PDA, TEV_DATA_SCALAR, &ad, 1, 1 );
			TEV_PACK_INT( TEV_DID_PC, TEV_DATA_SCALAR, &cnt, 1, 1 );
			TEV_PACK_INT( TEV_DID_PSD, TEV_DATA_SCALAR, &std, 1, 1 );
			TEV_FIN;
		}
	}

	if (cnt < 0)
		cc = PvmBadParam;
	else if (!pvmsbuf)
		cc = PvmNoBuf;
	else
		cc = (pvmsbuf->m_codef->enc_dcplx)
				(pvmsbuf, (void*)zp, cnt, std, sizeof(double)*2);

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_PKDCPLX,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}
	return (cc < 0 ? lpvmerr("pvm_pkdcplx", cc) : PvmOk);
}


int
pvm_upkdcplx(zp, cnt, std)
	double *zp;
	int cnt;
	int std;
{
	int cc;
	long ad;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_UPKDCPLX,TEV_EVENT_ENTRY)) {
			ad = (long)zp;
			TEV_PACK_LONG( TEV_DID_PDA, TEV_DATA_SCALAR, &ad, 1, 1 );
			TEV_PACK_INT( TEV_DID_PC, TEV_DATA_SCALAR, &cnt, 1, 1 );
			TEV_PACK_INT( TEV_DID_PSD, TEV_DATA_SCALAR, &std, 1, 1 );
			TEV_FIN;
		}
	}

	if (cnt < 0)
		cc = PvmBadParam;
	else if (!pvmrbuf)
		cc = PvmNoBuf;
	else
		cc = (pvmrbuf->m_codef->dec_dcplx)
				(pvmrbuf, (void*)zp, cnt, std, sizeof(double)*2);

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_UPKDCPLX,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}
	return (cc < 0 ? lpvmerr("pvm_upkdcplx", cc) : PvmOk);
}


int
pvm_pkdouble(dp, cnt, std)
	double *dp;
	int cnt;
	int std;
{
	int cc;
	long ad;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_PKDOUBLE,TEV_EVENT_ENTRY)) {
			ad = (long)dp;
			TEV_PACK_LONG( TEV_DID_PDA, TEV_DATA_SCALAR, &ad, 1, 1 );
			TEV_PACK_INT( TEV_DID_PC, TEV_DATA_SCALAR, &cnt, 1, 1 );
			TEV_PACK_INT( TEV_DID_PSD, TEV_DATA_SCALAR, &std, 1, 1 );
			TEV_FIN;
		}
	}

	if (cnt < 0)
		cc = PvmBadParam;
	else if (!pvmsbuf)
		cc = PvmNoBuf;
	else
		cc = (pvmsbuf->m_codef->enc_double)
				(pvmsbuf, (void*)dp, cnt, std, sizeof(double));

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_PKDOUBLE,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}
	return (cc < 0 ? lpvmerr("pvm_pkdouble", cc) : PvmOk);
}


int
pvm_upkdouble(dp, cnt, std)
	double *dp;
	int cnt;
	int std;
{
	int cc;
	long ad;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_UPKDOUBLE,TEV_EVENT_ENTRY)) {
			ad = (long)dp;
			TEV_PACK_LONG( TEV_DID_PDA, TEV_DATA_SCALAR, &ad, 1, 1 );
			TEV_PACK_INT( TEV_DID_PC, TEV_DATA_SCALAR, &cnt, 1, 1 );
			TEV_PACK_INT( TEV_DID_PSD, TEV_DATA_SCALAR, &std, 1, 1 );
			TEV_FIN;
		}
	}

	if (cnt < 0)
		cc = PvmBadParam;
	else if (!pvmrbuf)
		cc = PvmNoBuf;
	else
		cc = (pvmrbuf->m_codef->dec_double)
				(pvmrbuf, (void*)dp, cnt, std, sizeof(double));

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_UPKDOUBLE,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}
	return (cc < 0 ? lpvmerr("pvm_upkdouble", cc) : PvmOk);
}


int
pvm_pkfloat(fp, cnt, std)
	float *fp;
	int cnt;
	int std;
{
	int cc;
	long ad;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_PKFLOAT,TEV_EVENT_ENTRY)) {
			ad = (long)fp;
			TEV_PACK_LONG( TEV_DID_PDA, TEV_DATA_SCALAR, &ad, 1, 1 );
			TEV_PACK_INT( TEV_DID_PC, TEV_DATA_SCALAR, &cnt, 1, 1 );
			TEV_PACK_INT( TEV_DID_PSD, TEV_DATA_SCALAR, &std, 1, 1 );
			TEV_FIN;
		}
	}

	if (cnt < 0)
		cc = PvmBadParam;
	else if (!pvmsbuf)
		cc = PvmNoBuf;
	else
		cc = (pvmsbuf->m_codef->enc_float)
				(pvmsbuf, (void*)fp, cnt, std, sizeof(float));

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_PKFLOAT,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}
	return (cc < 0 ? lpvmerr("pvm_pkfloat", cc) : PvmOk);
}


int
pvm_upkfloat(fp, cnt, std)
	float *fp;
	int cnt;
	int std;
{
	int cc;
	long ad;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_UPKFLOAT,TEV_EVENT_ENTRY)) {
			ad = (long)fp;
			TEV_PACK_LONG( TEV_DID_PDA, TEV_DATA_SCALAR, &ad, 1, 1 );
			TEV_PACK_INT( TEV_DID_PC, TEV_DATA_SCALAR, &cnt, 1, 1 );
			TEV_PACK_INT( TEV_DID_PSD, TEV_DATA_SCALAR, &std, 1, 1 );
			TEV_FIN;
		}
	}

	if (cnt < 0)
		cc = PvmBadParam;
	else if (!pvmrbuf)
		cc = PvmNoBuf;
	else
		cc = (pvmrbuf->m_codef->dec_float)
				(pvmrbuf, (void*)fp, cnt, std, sizeof(float));

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_UPKFLOAT,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}
	return (cc < 0 ? lpvmerr("pvm_upkfloat", cc) : PvmOk);
}


int
pvm_pkint(np, cnt, std)
	int *np;
	int cnt;
	int std;
{
	int cc;
	long ad;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_PKINT,TEV_EVENT_ENTRY)) {
			ad = (long)np;
			TEV_PACK_LONG( TEV_DID_PDA, TEV_DATA_SCALAR, &ad, 1, 1 );
			TEV_PACK_INT( TEV_DID_PC, TEV_DATA_SCALAR, &cnt, 1, 1 );
			TEV_PACK_INT( TEV_DID_PSD, TEV_DATA_SCALAR, &std, 1, 1 );
			TEV_FIN;
		}
	}

	if (cnt < 0)
		cc = PvmBadParam;
	else if (!pvmsbuf)
		cc = PvmNoBuf;
	else
		cc = (pvmsbuf->m_codef->enc_int) (pvmsbuf, (void*)np, cnt, std, sizeof(int));

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_PKINT,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}
	return (cc < 0 ? lpvmerr("pvm_pkint", cc) : PvmOk);
}


int
pvm_pkuint(np, cnt, std)
	unsigned int *np;
	int cnt;
	int std;
{
	int cc;
	long ad;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_PKUINT,TEV_EVENT_ENTRY)) {
			ad = (long)np;
			TEV_PACK_LONG( TEV_DID_PDA, TEV_DATA_SCALAR, &ad, 1, 1 );
			TEV_PACK_INT( TEV_DID_PC, TEV_DATA_SCALAR, &cnt, 1, 1 );
			TEV_PACK_INT( TEV_DID_PSD, TEV_DATA_SCALAR, &std, 1, 1 );
			TEV_FIN;
		}
	}

	if (cnt < 0)
		cc = PvmBadParam;
	else if (!pvmsbuf)
		cc = PvmNoBuf;
	else
		cc = (pvmsbuf->m_codef->enc_uint) (pvmsbuf, (void*)np, cnt, std, sizeof(int));

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_PKUINT,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}
	return (cc < 0 ? lpvmerr("pvm_pkuint", cc) : PvmOk);
}


int
pvm_upkint(np, cnt, std)
	int *np;
	int cnt;
	int std;
{
	int cc;
	long ad;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_UPKINT,TEV_EVENT_ENTRY)) {
			ad = (long)np;
			TEV_PACK_LONG( TEV_DID_PDA, TEV_DATA_SCALAR, &ad, 1, 1 );
			TEV_PACK_INT( TEV_DID_PC, TEV_DATA_SCALAR, &cnt, 1, 1 );
			TEV_PACK_INT( TEV_DID_PSD, TEV_DATA_SCALAR, &std, 1, 1 );
			TEV_FIN;
		}
	}

	if (cnt < 0)
		cc = PvmBadParam;
	else if (!pvmrbuf)
		cc = PvmNoBuf;
	else
		cc = (pvmrbuf->m_codef->dec_int) (pvmrbuf, (void*)np, cnt, std, sizeof(int));

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_UPKINT,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}
	return (cc < 0 ? lpvmerr("pvm_upkint", cc) : PvmOk);
}


int
pvm_upkuint(np, cnt, std)
	unsigned int *np;
	int cnt;
	int std;
{
	int cc;
	long ad;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_UPKUINT,TEV_EVENT_ENTRY)) {
			ad = (long)np;
			TEV_PACK_LONG( TEV_DID_PDA, TEV_DATA_SCALAR, &ad, 1, 1 );
			TEV_PACK_INT( TEV_DID_PC, TEV_DATA_SCALAR, &cnt, 1, 1 );
			TEV_PACK_INT( TEV_DID_PSD, TEV_DATA_SCALAR, &std, 1, 1 );
			TEV_FIN;
		}
	}

	if (cnt < 0)
		cc = PvmBadParam;
	else if (!pvmrbuf)
		cc = PvmNoBuf;
	else
		cc = (pvmrbuf->m_codef->dec_uint) (pvmrbuf, (void*)np, cnt, std, sizeof(int));

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_UPKUINT,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}
	return (cc < 0 ? lpvmerr("pvm_upkuint", cc) : PvmOk);
}


int
pvm_pklong(np, cnt, std)
	long *np;
	int cnt;
	int std;
{
	int cc;
	long ad;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_PKLONG,TEV_EVENT_ENTRY)) {
			ad = (long)np;
			TEV_PACK_LONG( TEV_DID_PDA, TEV_DATA_SCALAR, &ad, 1, 1 );
			TEV_PACK_INT( TEV_DID_PC, TEV_DATA_SCALAR, &cnt, 1, 1 );
			TEV_PACK_INT( TEV_DID_PSD, TEV_DATA_SCALAR, &std, 1, 1 );
			TEV_FIN;
		}
	}

	if (cnt < 0)
		cc = PvmBadParam;
	else if (!pvmsbuf)
		cc = PvmNoBuf;
	else
		cc = (pvmsbuf->m_codef->enc_long) (pvmsbuf, (void*)np, cnt, std, sizeof(long));

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_PKLONG,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}
	return (cc < 0 ? lpvmerr("pvm_pklong", cc) : PvmOk);
}


int
pvm_pkulong(np, cnt, std)
	unsigned long *np;
	int cnt;
	int std;
{
	int cc;
	long ad;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_PKULONG,TEV_EVENT_ENTRY)) {
			ad = (long)np;
			TEV_PACK_LONG( TEV_DID_PDA, TEV_DATA_SCALAR, &ad, 1, 1 );
			TEV_PACK_INT( TEV_DID_PC, TEV_DATA_SCALAR, &cnt, 1, 1 );
			TEV_PACK_INT( TEV_DID_PSD, TEV_DATA_SCALAR, &std, 1, 1 );
			TEV_FIN;
		}
	}

	if (cnt < 0)
		cc = PvmBadParam;
	else if (!pvmsbuf)
		cc = PvmNoBuf;
	else
		cc = (pvmsbuf->m_codef->enc_ulong) (pvmsbuf, (void*)np, cnt, std, sizeof(long));

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_PKULONG,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}
	return (cc < 0 ? lpvmerr("pvm_pkulong", cc) : PvmOk);
}


int
pvm_upklong(np, cnt, std)
	long *np;
	int cnt;
	int std;
{
	int cc;
	long ad;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_UPKLONG,TEV_EVENT_ENTRY)) {
			ad = (long)np;
			TEV_PACK_LONG( TEV_DID_PDA, TEV_DATA_SCALAR, &ad, 1, 1 );
			TEV_PACK_INT( TEV_DID_PC, TEV_DATA_SCALAR, &cnt, 1, 1 );
			TEV_PACK_INT( TEV_DID_PSD, TEV_DATA_SCALAR, &std, 1, 1 );
			TEV_FIN;
		}
	}

	if (cnt < 0)
		cc = PvmBadParam;
	else if (!pvmrbuf)
		cc = PvmNoBuf;
	else
		cc = (pvmrbuf->m_codef->dec_long) (pvmrbuf, (void*)np, cnt, std, sizeof(long));

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_UPKLONG,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}
	return (cc < 0 ? lpvmerr("pvm_upklong", cc) : PvmOk);
}


int
pvm_upkulong(np, cnt, std)
	unsigned long *np;
	int cnt;
	int std;
{
	int cc;
	long ad;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_UPKULONG,TEV_EVENT_ENTRY)) {
			ad = (long)np;
			TEV_PACK_LONG( TEV_DID_PDA, TEV_DATA_SCALAR, &ad, 1, 1 );
			TEV_PACK_INT( TEV_DID_PC, TEV_DATA_SCALAR, &cnt, 1, 1 );
			TEV_PACK_INT( TEV_DID_PSD, TEV_DATA_SCALAR, &std, 1, 1 );
			TEV_FIN;
		}
	}

	if (cnt < 0)
		cc = PvmBadParam;
	else if (!pvmrbuf)
		cc = PvmNoBuf;
	else
		cc = (pvmrbuf->m_codef->dec_ulong) (pvmrbuf, (void*)np, cnt, std, sizeof(long));

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_UPKULONG,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}
	return (cc < 0 ? lpvmerr("pvm_upkulong", cc) : PvmOk);
}


int
pvm_pkshort(np, cnt, std)
	short *np;
	int cnt;
	int std;
{
	int cc;
	long ad;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_PKSHORT,TEV_EVENT_ENTRY)) {
			ad = (long)np;
			TEV_PACK_LONG( TEV_DID_PDA, TEV_DATA_SCALAR, &ad, 1, 1 );
			TEV_PACK_INT( TEV_DID_PC, TEV_DATA_SCALAR, &cnt, 1, 1 );
			TEV_PACK_INT( TEV_DID_PSD, TEV_DATA_SCALAR, &std, 1, 1 );
			TEV_FIN;
		}
	}

	if (cnt < 0)
		cc = PvmBadParam;
	else if (!pvmsbuf)
		cc = PvmNoBuf;
	else
		cc = (pvmsbuf->m_codef->enc_short) (pvmsbuf, (void*)np, cnt, std, sizeof(short));

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_PKSHORT,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}
	return (cc < 0 ? lpvmerr("pvm_pkshort", cc) : PvmOk);
}


int
pvm_pkushort(np, cnt, std)
	unsigned short *np;
	int cnt;
	int std;
{
	int cc;
	long ad;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_PKUSHORT,TEV_EVENT_ENTRY)) {
			ad = (long)np;
			TEV_PACK_LONG( TEV_DID_PDA, TEV_DATA_SCALAR, &ad, 1, 1 );
			TEV_PACK_INT( TEV_DID_PC, TEV_DATA_SCALAR, &cnt, 1, 1 );
			TEV_PACK_INT( TEV_DID_PSD, TEV_DATA_SCALAR, &std, 1, 1 );
			TEV_FIN;
		}
	}

	if (cnt < 0)
		cc = PvmBadParam;
	else if (!pvmsbuf)
		cc = PvmNoBuf;
	else
		cc = (pvmsbuf->m_codef->enc_ushort) (pvmsbuf, (void*)np, cnt, std, sizeof(short));

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_PKUSHORT,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}
	return (cc < 0 ? lpvmerr("pvm_pkushort", cc) : PvmOk);
}


int
pvm_upkshort(np, cnt, std)
	short *np;
	int cnt;
	int std;
{
	int cc;
	long ad;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_UPKSHORT,TEV_EVENT_ENTRY)) {
			ad = (long)np;
			TEV_PACK_LONG( TEV_DID_PDA, TEV_DATA_SCALAR, &ad, 1, 1 );
			TEV_PACK_INT( TEV_DID_PC, TEV_DATA_SCALAR, &cnt, 1, 1 );
			TEV_PACK_INT( TEV_DID_PSD, TEV_DATA_SCALAR, &std, 1, 1 );
			TEV_FIN;
		}
	}

	if (cnt < 0)
		cc = PvmBadParam;
	else if (!pvmrbuf)
		cc = PvmNoBuf;
	else
		cc = (pvmrbuf->m_codef->dec_short) (pvmrbuf, (void*)np, cnt, std, sizeof(short));

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_UPKSHORT,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}
	return (cc < 0 ? lpvmerr("pvm_upkshort", cc) : PvmOk);
}


int
pvm_upkushort(np, cnt, std)
	unsigned short *np;
	int cnt;
	int std;
{
	int cc;
	long ad;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_UPKUSHORT,TEV_EVENT_ENTRY)) {
			ad = (long)np;
			TEV_PACK_LONG( TEV_DID_PDA, TEV_DATA_SCALAR, &ad, 1, 1 );
			TEV_PACK_INT( TEV_DID_PC, TEV_DATA_SCALAR, &cnt, 1, 1 );
			TEV_PACK_INT( TEV_DID_PSD, TEV_DATA_SCALAR, &std, 1, 1 );
			TEV_FIN;
		}
	}

	if (cnt < 0)
		cc = PvmBadParam;
	else if (!pvmrbuf)
		cc = PvmNoBuf;
	else
		cc = (pvmrbuf->m_codef->dec_ushort) (pvmrbuf, (void*)np, cnt, std, sizeof(short));

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_UPKUSHORT,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}
	return (cc < 0 ? lpvmerr("pvm_upkushort", cc) : PvmOk);
}


int
pvm_pkstr(cp)
	char *cp;
{
	int l = strlen(cp) + 1;
	int cc;
	long ad;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_PKSTR,TEV_EVENT_ENTRY)) {
			ad = (long)cp;
			TEV_PACK_LONG( TEV_DID_PDA, TEV_DATA_SCALAR, &ad, 1, 1 );
			TEV_FIN;
		}
	}

	if (!pvmsbuf)
		cc = PvmNoBuf;
	else {
		/* can't do InPlace without a lot of grief */
		if (pvmsbuf->m_enc == 0x20000000)
			cc = PvmNotImpl;
		/* user defined, don't pack len, don't pack '\0' */
		else if (pvmsbuf->m_enc == 0x40000000)
			cc = (pvmsbuf->m_codef->enc_byte)
				(pvmsbuf, (void*)cp, l - 1, 1, 1);
		else
			if (!(cc = (pvmsbuf->m_codef->enc_int)
					(pvmsbuf, (void*)&l, 1, 1, sizeof(int))))
				cc = (pvmsbuf->m_codef->enc_byte)
						(pvmsbuf, (void*)cp, l, 1, 1);
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_PKSTR,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}
	return (cc < 0 ? lpvmerr("pvm_pkstr", cc) : PvmOk);
}


int
pvm_upkstr(cp)
	char *cp;
{
	int l;
	int cc;
	long ad;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_UPKSTR,TEV_EVENT_ENTRY)) {
			ad = (long)cp;
			TEV_PACK_LONG( TEV_DID_PDA, TEV_DATA_SCALAR, &ad, 1, 1 );
			TEV_FIN;
		}
	}

	if (!pvmrbuf)
		cc = PvmNoBuf;
	else {
		if (!(cc = (pvmrbuf->m_codef->dec_int)
				(pvmrbuf, (void*)&l, 1, 1, sizeof(int))))
			cc = (pvmrbuf->m_codef->dec_byte)
					(pvmrbuf, (void*)cp, l, 1, 1);
	}

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_UPKSTR,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}
	return (cc < 0 ? lpvmerr("pvm_upkstr", cc) : PvmOk);
}


/*	pvmupkstralloc()  (from pvmdpack.c upkstralloc(), spanked slightly)
*
*	Unpack a string from current message.  Result is null-terminated,
*	and in dynamic space..
*	Returns 0 if ok.
*/

int
pvmupkstralloc(ss)
	char **ss;				/* return pointer */
{
	int cc;
	int l;

	if (!pvmrbuf)
		cc = PvmNoBuf;
	else {
		if (!(cc = (pvmrbuf->m_codef->dec_int)
				(pvmrbuf, (void*)&l, 1, 1, sizeof(int)))) {
			*ss = TALLOC(l, char, "pustr");
			if ((cc = (pvmrbuf->m_codef->dec_byte)
					(pvmrbuf, (void*)*ss, l, 1, 1)) < 0) {
				PVM_FREE(*ss);
				*ss = 0;
			}
		}
	}
	return cc;
}


int
pvm_vpackf(fmt, ap)
	char *fmt;
	va_list ap;
{
	char *p = fmt;
	int cc;
	int cnt, std;			/* count and stride of element */
	int vh, vl, vu;			/* short, long, unsigned variety */
	int isv;				/* param is a value not address */
	struct encvec *codef;
	char tc, *cp;			/* each type and a pointer */
	short th, *hp;
	int tn, *np;
	long tl, *lp;
	float tf, *fp;
	double td, *dp;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_PACKF,TEV_EVENT_ENTRY)) {
			TEV_PACK_STRING( TEV_DID_PF, TEV_DATA_SCALAR, fmt, 1, 1 );
			TEV_FIN;
		}
	}

	if (p[0] == '%' && p[1] == '+') {
		cc = va_arg(ap, int);
		if ((cc = pvm_initsend(cc)) < 0)
			goto hork;
		p += 2;
	}

	if (!pvmsbuf) {
		cc = PvmNoBuf;
		goto hork;
	}

	codef = pvmsbuf->m_codef;

	while (*p) {
		if (*p++ == '%') {
			cnt = 1;
			std = 1;
			isv = 1;
			vl = vh = vu = 0;
			if (*p == '*') {
				cnt = va_arg(ap, int);
				isv = 0;
				p++;
			} else
				if (isdigit(*p)) {
					cnt = atoi(p);
					isv = 0;
					while (isdigit(*++p)) ;
				}
			if (*p == '.') {
				isv = 0;
				if (*++p == '*') {
					std = va_arg(ap, int);
					p++;
				} else
					if (isdigit(*p)) {
						std = atoi(p);
						while (isdigit(*++p)) ;
					}
			}
			for (cc = 1; cc; ) {
				switch (*p++) {

				case 'h':
					vh = 1;
					break;

				case 'l':
					vl = 1;
					break;

				case 'u':
					vu = 1;
					break;

				default:
					p--;
					cc = 0;
				}
			}

			switch (*p++) {

			case 'c':
				if (isv) {
					tc = (char) va_arg(ap, int);
					cp = &tc;
				} else
					cp = va_arg(ap, char *);
				cc = (codef->enc_byte) (pvmsbuf, (void*)cp, cnt, std, 1);
#ifdef	DEBUGPACKF
				printf("%d %d %s%schar\n", cnt, std, (vu ? "unsigned " : ""),
						(isv ? "" : "&"));
#else
				vu = vu; /* sgi compiler */
#endif
				break;

			case 'd':
				if (vl) {
					if (isv) {
						tl = va_arg(ap, long);
						lp = &tl;
					} else
						lp = va_arg(ap, long *);
					cc = (codef->enc_long)
							(pvmsbuf, (void*)lp, cnt, std, sizeof(long));
				} else
					if (vh) {
						if (isv) {
							th = (short) va_arg(ap, int);
							hp = &th;
						} else
							hp = va_arg(ap, short *);
						cc = (codef->enc_short)
								(pvmsbuf, (void*)hp, cnt, std, sizeof(short));
					} else {
						if (isv) {
							tn = va_arg(ap, int);
							np = &tn;
						} else
							np = va_arg(ap, int *);
						cc = (codef->enc_int)
								(pvmsbuf, (void*)np, cnt, std, sizeof(int));
					}
#ifdef	DEBUGPACKF
				printf("%d %d %s%s%s%sint\n", cnt, std,
						(vu ? "unsigned " : ""),
						(vl ? "long " : ""),
						(vh ? "short " : ""),
						(isv ? "" : "&"));
#endif
				break;

			case 'f':
				if (vl) {
					if (isv) {
						td = va_arg(ap, double);
						dp = &td;
					} else
						dp = va_arg(ap, double *);
					cc = (codef->enc_double)
							(pvmsbuf, (void*)dp, cnt, std, sizeof(double));
				} else {
					if (isv) {
/*
						tf = va_arg(ap, float);
*/
						td = va_arg(ap, double);
						tf = td;
						fp = &tf;
					} else
						fp = va_arg(ap, float *);
					cc = (codef->enc_float)
							(pvmsbuf, (void*)fp, cnt, std, sizeof(float));
				}
#ifdef	DEBUGPACKF
				printf("%d %d %s%s\n", cnt, std, (isv ? "" : "&"),
						(vl ? "double" : "float"));
#endif
				break;

			case 'x':
				if (vl) {
					dp = va_arg(ap, double *);
					cc = (codef->enc_dcplx)
							(pvmsbuf, (void*)dp, cnt, std, sizeof(double)*2);
				} else {
					fp = va_arg(ap, float *);
					cc = (codef->enc_cplx)
							(pvmsbuf, (void*)fp, cnt, std, sizeof(float)*2);
				}
#ifdef	DEBUGPACKF
				printf("%d %d %s&cplx\n", cnt, std, (vl ? "double " : ""));
#endif
				break;

			case 's':
				if (pvmsbuf->m_enc == 2)	/* oops, can't do this */
					cc = PvmNotImpl;
				else {
					cp = va_arg(ap, char *);
					cnt = strlen(cp) + 1;
					if (!(cc = (codef->enc_int) (pvmsbuf, (void*)&cnt, 1, 1, sizeof(int))))
						cc = (codef->enc_byte) (pvmsbuf, (void*)cp, cnt, 1, 1);
#ifdef	DEBUGPACKF
					printf("string\n");
#endif
				}
				break;

			default:
				cc = PvmBadParam;
				break;
			}

			if (cc)
				goto hork;
		}
/*
		else
			printf("'%c'\n", *(p - 1));
*/
	}
	cc = PvmOk;

hork:
	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_PACKF,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_packf", cc);
	return cc;
}


int
#ifdef __STDC__
pvm_packf(const char *fmt, ...)
#else
pvm_packf(va_alist)
	va_dcl
#endif
{
	va_list ap;
	int cc;

#ifdef __STDC__
	va_start(ap, fmt);
#else
	char    *fmt;

	va_start(ap);
	fmt = va_arg(ap, char *);
#endif

	cc = pvm_vpackf(fmt, ap);
	va_end(ap);

	return cc;
}


int
pvm_vunpackf(fmt, ap)
	char *fmt;
	va_list ap;
{
	char *p = fmt;
	int cc;
	int cnt, std;		/* count and stride of element */
	int vh, vl, vu;		/* short, long, unsigned variety */
	struct encvec *codef;
	char *cp;			/* pointers for each type */
	short *hp;
	int *np;
	long *lp;
	float *fp;
	double *dp;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_UNPACKF,TEV_EVENT_ENTRY)) {
			TEV_PACK_STRING( TEV_DID_PF, TEV_DATA_SCALAR, fmt, 1, 1 );
			TEV_FIN;
		}
	}

	if (!pvmrbuf) {
		cc = PvmNoBuf;
		goto hork;
	}

	codef = pvmrbuf->m_codef;

	while (*p) {
		if (*p++ == '%') {
			cnt = 1;
			std = 1;
			vl = vh = vu = 0;
			if (*p == '*') {
				cnt = va_arg(ap, int);
				p++;
			} else
				if (isdigit(*p)) {
					cnt = atoi(p);
					while (isdigit(*++p)) ;
				}
			if (*p == '.') {
				if (*++p == '*') {
					std = va_arg(ap, int);
					p++;
				} else
					if (isdigit(*p)) {
						std = atoi(p);
						while (isdigit(*++p)) ;
					}
			}
			for (cc = 1; cc; ) {
				switch (*p++) {

				case 'h':
					vh = 1;
					break;

				case 'l':
					vl = 1;
					break;

				case 'u':
					vu = 1;
					break;

				default:
					p--;
					cc = 0;
				}
			}

			switch (*p++) {

			case 'c':
				cp = va_arg(ap, char *);
				cc = (codef->dec_byte) (pvmrbuf, (void*)cp, cnt, std, 1);
				break;

			case 'd':
				if (vl) {
					lp = va_arg(ap, long *);
					cc = (vu ? codef->dec_ulong : codef->dec_long)
							(pvmrbuf, (void*)lp, cnt, std, sizeof(long));
				} else
					if (vh) {
						hp = va_arg(ap, short *);
						cc = (vu ? codef->dec_ushort : codef->dec_short)
								(pvmrbuf, (void*)hp, cnt, std, sizeof(short));
					} else {
						np = va_arg(ap, int *);
						cc = (vu ? codef->dec_uint : codef->dec_int)
								(pvmrbuf, (void*)np, cnt, std, sizeof(int));
					}
				break;

			case 'f':
				if (vl) {
					dp = va_arg(ap, double *);
					cc = (codef->dec_double)
							(pvmrbuf, (void*)dp, cnt, std, sizeof(double));
				} else {
					fp = va_arg(ap, float *);
					cc = (codef->dec_float)
							(pvmrbuf, (void*)fp, cnt, std, sizeof(float));
				}
				break;

			case 'x':
				if (vl) {
					dp = va_arg(ap, double *);
					cc = (codef->dec_dcplx)
							(pvmrbuf, (void*)dp, cnt, std, sizeof(double)*2);
				} else {
					fp = va_arg(ap, float *);
					cc = (codef->dec_cplx)
							(pvmrbuf, (void*)fp, cnt, std, sizeof(float)*2);
				}
				break;

			case 's':
				cp = va_arg(ap, char *);
				if (!(cc = (codef->dec_int) (pvmrbuf, (void*)&cnt, 1, 1, sizeof(int))))
					cc = (codef->dec_byte) (pvmrbuf, (void*)cp, cnt, 1, 1);
				break;

			default:
				cc = PvmBadParam;
				break;
			}

			if (cc)
				goto hork;
		}
	}
	cc = PvmOk;

hork:
	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_UNPACKF,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_unpackf", cc);
	return cc;
}


int
#ifdef __STDC__
pvm_unpackf(const char *fmt, ...)
#else
pvm_unpackf(va_alist)
	va_dcl
#endif
{
	va_list ap;
	int cc;

#ifdef __STDC__
	va_start(ap, fmt);
#else
	char    *fmt;

	va_start(ap);
	fmt = va_arg(ap, char *);
#endif

	cc = pvm_vunpackf(fmt, ap);
	va_end(ap);

	return cc;
}


int
pvm_getminfo(mid, info)
	int mid;
	struct pvmminfo *info;
{
	struct pmsg *mp;
	int cc;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_GETMINFO,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_MB, TEV_DATA_SCALAR, &mid, 1, 1 );
			TEV_FIN;
		}
	}

	if (mid <= 0)
		cc = PvmBadParam;
	else if (mp = midtobuf(mid)) {
		if (mp->m_flag & MM_PACK)
			pmsg_setlen(mp);
		info->len = mp->m_len;
		info->ctx = mp->m_ctx;
		info->tag = mp->m_tag;
		info->wid = mp->m_wid;
		info->enc = mp->m_enc;
		info->crc = mp->m_crc;
		info->src = mp->m_src;
		info->dst = mp->m_dst;
		cc = PvmOk;
	} else
		cc = PvmNoSuchBuf;

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_GETMINFO,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_getminfo", cc);
	return cc;
}


int
pvm_setminfo(mid, info)
	int mid;
	struct pvmminfo *info;
{
	struct pmsg *mp;
	int cc;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_SETMINFO,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_MB, TEV_DATA_SCALAR, &mid, 1, 1 );
			TEV_FIN;
		}
	}

	if (mid <= 0)
		cc = PvmBadParam;
	else if (mp = midtobuf(mid)) {
		mp->m_ctx = info->ctx;
		mp->m_tag = info->tag;
		mp->m_wid = info->wid;
		mp->m_src = info->src;
		mp->m_dst = info->dst;
		cc = PvmOk;
	} else
		cc = PvmNoSuchBuf;

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_SETMINFO,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_setminfo", cc);
	return cc;
}


int
pvm_pkmesg(mid)
	int mid;
{
	int cc = 0;
	struct pmsg *mp;

	if (mid <= 0)
		cc = PvmBadParam;
	else if (mp = midtobuf(mid)) {
		if (!pvmsbuf)
			cc = PvmNoBuf;
		else if (mp == pvmsbuf)
			cc = PvmBadParam;
		else
			cc = pmsg_pack(pvmsbuf, mp);
	} else
		cc = PvmNoSuchBuf;

	return cc;
}


int
pvm_pkmesgbody(mid)
	int mid;
{
	int cc = 0;
	struct pmsg *mp;

	if (mid <= 0)
		cc = PvmBadParam;
	else if (mp = midtobuf(mid)) {
		if (!pvmsbuf)
			cc = PvmNoBuf;
		else if (mp == pvmsbuf)
			cc = PvmBadParam;
		else
			cc = pmsg_packbody(pvmsbuf, mp);
	} else
		cc = PvmNoSuchBuf;

	return cc;
}


int
pvm_upkmesg()
{
	int cc = 0;
	struct pmsg *mp;

	if (!pvmrbuf)
		cc = PvmNoBuf;
	else {
		if (mp = umbuf_new()) {
			if (!(cc = pmsg_unpack(pvmrbuf, mp)))
				cc = mp->m_mid;
		} else
			cc = PvmNoMem;
	}
	return cc;
}


#ifdef	PVM33COMPAT
/***************************************************************
 **  Crusty functions that will fall off in the next version  **
 **                                                           **
 ***************************************************************/

/*
*	use pvm_getminfo for pvm_getmwid
*	use pvm_setminfo for pvm_setmwid
*/

int
pvm_getmwid(mid)
	int mid;
{
	struct pmsg *mp = 0;
	int cc = 0;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_GETMWID,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_MB, TEV_DATA_SCALAR, &mid, 1, 1 );
			TEV_FIN;
		}
	}

	if (mid < 0)
		cc = PvmBadParam;
	else if (mp = midtobuf(mid))
		cc = mp->m_wid;
	else
		cc = PvmNoSuchBuf;

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_GETMWID,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_WID, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_getmwid", cc);
	return cc;
}


int
pvm_setmwid(mid, wid)
	int mid;
	int wid;
{
	struct pmsg *mp = 0;
	int cc = 0;
	TEV_DECLS

	if (TEV_EXCLUSIVE) {
		if (TEV_DO_TRACE(TEV_SETMWID,TEV_EVENT_ENTRY)) {
			TEV_PACK_INT( TEV_DID_MB, TEV_DATA_SCALAR, &mid, 1, 1 );
			TEV_PACK_INT( TEV_DID_WID, TEV_DATA_SCALAR, &wid, 1, 1 );
			TEV_FIN;
		}
	}

	if (mid < 0)
		cc = PvmBadParam;
	else if (mp = midtobuf(mid))
		mp->m_wid = wid;
	else
		cc = PvmNoSuchBuf;

	if (TEV_AMEXCL) {
		if (TEV_DO_TRACE(TEV_SETMWID,TEV_EVENT_EXIT)) {
			TEV_PACK_INT( TEV_DID_CC, TEV_DATA_SCALAR, &cc, 1, 1 );
			TEV_FIN;
		}
		TEV_ENDEXCL;
	}

	if (cc < 0)
		lpvmerr("pvm_setmwid", cc);
	return cc;
}

#endif	/*PVM33COMPAT*/

