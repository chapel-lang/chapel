
static char rcsid[] =
	"$Id: pmsg.c,v 1.17 2001/02/07 23:15:50 pvmsrc Exp $";

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
 *	pmsg.c
 *
 *	Libpvm and pvmd message descriptors
 *
 * $Log: pmsg.c,v $
 * Revision 1.17  2001/02/07 23:15:50  pvmsrc
 * 2nd Half of CYGWIN Check-ins...
 * (Spanker=kohl)
 *
 * Revision 1.16  2001/02/06 22:26:57  pvmsrc
 * Added handling for -DUSE_XDR_LONGLONG.
 * 	- for Alphas with broken XDR...
 * (Spanker=kohl)
 *
 * Revision 1.15  2000/02/16 21:59:48  pvmsrc
 * Fixed up #include <sys/types.h> stuff...
 * 	- use <bsd/sys/types.h> for IMA_TITN...
 * 	- #include before any NEEDMENDIAN #includes...
 * (Spanker=kohl)
 *
 * Revision 1.14  2000/02/11 20:42:09  pvmsrc
 * Added support for rare Strongarm float byte order to fbol()...
 * 	- hope this doesn't break anything else!!!
 * 	- submitted by Alexander Schulz <un23@rz.uni-karlsruhe.de>.
 * (Spanker=kohl)
 *
 * Revision 1.13  1999/07/08 19:00:05  kohl
 * Fixed "Log" keyword placement.
 * 	- indent with " * " for new CVS.
 *
 * Revision 1.12  1998/11/20  20:06:34  pvmsrc
 * Changes so that win32 will compile & build. Also, common
 * Changes so that compiles & builds on NT. Also
 * common source on win32 & unix.
 * (spanker=sscott)
 *
 * Revision 1.11  1998/03/10  20:32:31  pvmsrc
 * Fixed tracing bogusness in struct timeval on 64-bit arches...
 * 	- need to cast to (int) before packing, else could crop off usecs...
 * (Spanker=kohl)
 *
 * Revision 1.10  1998/02/24  15:36:28  pvmsrc
 * Oops...  leftover typo from umbuf -> pmsg renaming way back when.
 * 	- UB_DIFFNODE -> MM_DIFFNODE for IMA_CSPP.
 * (Spanker=kohl)
 *
 * Revision 1.9  1997/11/04  23:21:40  pvmsrc
 * Added SYSVSTR stuff.
 * (Spanker=kohl)
 *
 * Revision 1.8  1997/08/25  21:47:13  pvmsrc
 * Fixed pvm_setsbuf() problem with user-defined trace events.
 * 	- setsbuf was re-calling enc_trc_init(), pushing second header.
 * 	- gutted enc_trc_init() -> extracted into new enc_trc_hdr() routine
 * 		that pvm_mkbuf() calls ONCE...
 * (Spanker=kohl)
 *
 * Revision 1.7  1997/06/27  17:32:33  pvmsrc
 * Updated for WIN32 header files & Authors.
 *
 * Revision 1.6  1997/04/30  21:32:14  pvmsrc
 * SGI Compiler Warning Cleanup.
 * 	- fixed up return codes for trace message packing stuff.
 *
 * Revision 1.5  1997/04/09  14:40:32  pvmsrc
 * PVM patches from the base 3.3.10 to 3.3.11 versions where applicable.
 * Originals by Bob Manchek. Altered by Graham Fagg where required.
 *
 * Fixes bogus XDR en/decoding when passing short values.
 * Original patch was meant for lpvmpack.c but code is now in pmsg.c
 *
 * Revision 1.4  1997/01/28  19:26:57  pvmsrc
 * New Copyright Notice & Authors.
 *
 * Revision 1.3  1996/10/25  13:57:32  pvmsrc
 * Replaced old #includes for protocol headers:
 * 	- <pvmsdpro.h>, "ddpro.h", "tdpro.h"
 * With #include of new combined header:
 * 	- <pvmproto.h>
 *
 * Revision 1.2  1996/10/24  22:09:21  pvmsrc
 * Moved #include "global.h" below other #include's for typing.
 * Added #include <pvmtev.h> & new "lpvm.h" for tracing & libpvm globals.
 * Added new user-defined trace event packing encoders:
 * 	- new enc_trc_init(), enc_trc_*() routines.
 * 	- new dec_trc_init(), dec_trc_*() routines.
 * 	- added new trace encoding vector in struct encvec encoders[],
 * 		with new selection in pmsg_setenc() for enc = 4 from
 * 		0x40000000 for user-defined tracing.
 *
 * Revision 1.1  1996/09/23  23:44:26  pvmsrc
 * Initial revision
 *
 *
 */

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

#ifdef  SYSVSTR
#include <string.h>
#else
#include <strings.h>
#endif

#include <pvmproto.h>
#include "pvmalloc.h"
#include "pvmfrag.h"
#include "pmsg.h"
#include "listmac.h"
#include "bfunc.h"
#include "lpvm.h"
#include <pvmtev.h>
#include "global.h"


/***************
 **  Globals  **
 **           **
 ***************/


/***************
 **  Private  **
 **           **
 ***************/

#ifdef	CLUMP_ALLOC
#ifndef	PMSG_CLUMP
#define	PMSG_CLUMP	50
#endif
static struct pmsg freepmsgs;			/* cache of pmsgs */
static int numpmsgs = 0;
#endif


/****************************
*  Generate data signature  *
*                           *
****************************/

static int
ibol(o, p, n)
	int o;
	char *p;
	int n;
{
	int i, j;

	if (p[0] == 0) {
		i = 0;	/* ll */
	} else if (p[0] == n - 1) {
		i = 3;	/* hh */
	} else if (p[0] == n / 2) {
		i = 2;	/* hl */
	} else if (p[0] == n / 2 - 1) {
		i = 1;	/* lh */
	} else {
		fprintf(stderr, "can't generate signature for my integer byte order\n");
		abort();
	}

	j = ffs(n) - 1;
/*
	printf(".%d%d.%d%d%d", (i & 2) ? 1 : 0, (i & 1) ? 1 : 0,
			(j & 4) ? 1 : 0, (j & 2) ? 1 : 0, (j & 1) ? 1 : 0);
*/
	return ((i << 3) | j) << o;
}


/*
*  (float)1.0 =
*  IEEE single ll   00 00 80 3f
*  IEEE double ll   00 00 00 00 00 00 f0 3f
*  IEEE single hh   3f 80 00 00
*  IEEE double hh   3f f0 00 00 00 00 00 00
*  CRAY       (hh)  40 01 80 00 00 00 00 00
*  Convex s   (hh)  40 80 00 00
*  Convex d   (hh)  40 10 00 00 00 00 00 00
*  IBM370 s
*  IBM370 d
*  VAX xxxx?
*/

struct floatsig {
	int length;
	unsigned char *bytes;
	char *name;
};

/*
* signatures for float = 1.0 in hh byte order, all formats
*/

static unsigned char fs1[] = { 0x3f, 0x80, 0x00, 0x00 };
static unsigned char fs2[] = { 0x3f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static unsigned char fs4[] = { 0x40, 0x01, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00 };
static unsigned char fs5[] = { 0x40, 0x80, 0x00, 0x00 };
static unsigned char fs6[] = { 0x40, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

static struct floatsig thesigs[] = {
	{ 0, 0, "UNKNOWN" },
	{ 4, fs1, "IEEE_single" },
	{ 8, fs2, "IEEE_double" },
	{ 0, 0, "UNDEFINED" },
	{ 8, fs4, "Cray" },
	{ 4, fs5, "Convex_single" },
	{ 8, fs6, "Convex_double" },
	{ 0, 0, "UNDEFINED" },
	{ 0, 0, "UNDEFINED" },
	{ 0, 0, "UNDEFINED" },
	{ 0, 0, "UNDEFINED" },
	{ 0, 0, "UNDEFINED" },
	{ 0, 0, "UNDEFINED" },
	{ 0, 0, "UNDEFINED" },
	{ 0, 0, "UNDEFINED" },
	{ 0, 0, "UNDEFINED" },
};


static int
fbol(o, p, n)
	int o;
	unsigned char *p;
	int n;
{
	int i, j;

	for (i = 0; i < 16; i++) {
		if (thesigs[i].length == n) {
			for (j = 0; j < n; j++)
				if (p[j] != thesigs[i].bytes[j])
					break;
			if (j == n)
				return ((3 << 4) | i) << o;

			for (j = 0; j < n; j++)
				if (p[n - 1 - j] != thesigs[i].bytes[j])
					break;
			if (j == n)
				return i << o;

			/* weird float byte order for Strongarm?! */
			/* submitted Alexander Schulz <un23@rz.uni-karlsruhe.de> */
			for (j = 0; j < n; j++)
				if (p[(((j/4)*8)+3)-j] != thesigs[i].bytes[j])
					break;
			if (j == n)
				return ((2 << 4) | i) << o;
		}
	}
	fprintf(stderr, "can't generate signature for my float byte order\n");
	abort();
	return 0;
}


/*	pvm_getdsig()
*
*	Return data signature for executable, describing representations
*	of integers and floats.
*
*	Bits:
*    0:2  short int size
*    3:4  short int byte order
*    5:7  int size
*    8:9  int byte order
*   10:12  long int size
*   13:14  long int byte order
*   15:18  float format
*   19:20  float byte order
*   21:24  double float format
*   25:26  double float byte order
*   27:31  0
*
*	Sizes are 2 ** (0..7) bytes
*	Byte order specifies overall order and half-folding:
*		00  Little endian (e.g. 0 1 2 3)
*		01  Little, but half-swapped (1 0 3 2 or 3 2 1 0 7 6 5 4)
*		10  Big, but half swapped (2 3 0 1 or 4 5 6 7 0 1 2 3)
*		11  Big endian (3 2 1 0)
*/

int
pvmgetdsig()
{
	static int myfmt = -1;

	short i0;
	int i1;
	long i2;
	float f0;
	double f1;
	int i, j;
	int fmt;

	if (myfmt != -1)
		return myfmt;

	fmt = 0;

	i0 = 0;
	for (i = 0; i < sizeof(i0); i++)
		i0 += (short)i << (i * 8);
	fmt |= ibol(0, (char *) &i0, (int) sizeof(i0));

	i1 = 0;
	for (i = 0; i < sizeof(i1); i++)
		i1 += (int)i << (i * 8);
	fmt |= ibol(5, (char *) &i1, (int) sizeof(i1));

	i2 = 0;
	for (i = 0; i < sizeof(i2); i++)
		i2 += (long)i << (i * 8);
	fmt |= ibol(10, (char *) &i2, (int) sizeof(i2));

	f0 = 1.0;
	fmt |= fbol(15, (unsigned char *) &f0, (int) sizeof(f0));

	f1 = 1.0;
	fmt |= fbol(21, (unsigned char *) &f1, (int) sizeof(f1));

	myfmt = fmt;
	return fmt;
}


/****************************
 **  Message manipulation  **
 **                        **
 ****************************/


static struct pmsg *
pmsg_get()
{
	struct pmsg *mp;
	int n;

#ifdef	CLUMP_ALLOC
	if (numpmsgs == 0) {
		freepmsgs.m_link = freepmsgs.m_rlink = &freepmsgs;
		if (!(mp = TALLOC(PMSG_CLUMP, struct pmsg, "pmsgs")))
			return (struct pmsg *)0;
		for (n = PMSG_CLUMP; n-- > 0; ) {
			LISTPUTBEFORE(&freepmsgs, mp, m_link, m_rlink);
			mp++;
		}
		numpmsgs = PMSG_CLUMP;
	}
	numpmsgs--;
	mp = freepmsgs.m_link;
	LISTDELETE(mp, m_link, m_rlink);

#else
	mp = TALLOC(1, struct pmsg, "pmsg");
#endif

	return mp;
}


static void
pmsg_put(mp)
	struct pmsg *mp;
{
#ifdef	CLUMP_ALLOC
	if (numpmsgs == 0)
		freepmsgs.m_link = freepmsgs.m_rlink = &freepmsgs;
	LISTPUTBEFORE(&freepmsgs, mp, m_link, m_rlink);
	numpmsgs++;

#else
	PVM_FREE(mp);
#endif
}


/*	pmsg_new()
*
*	Create a new message.
*	Returns pointer to message struct or NULL if no memory available.
*/

struct pmsg *
pmsg_new(master)
	int master;			/* true if a master (no data) node */
{
	struct pmsg *mp;

	if (!(mp = pmsg_get()))
		goto fail;

	mp->m_ref = 1;
	if (master) {
		mp->m_link = mp->m_rlink = mp;
		mp->m_frag = 0;
	} else {
		mp->m_link = mp->m_rlink = 0;
		if (!(mp->m_frag = fr_new(0))) {
			PVM_FREE(mp);
			goto fail;
		}
	}
	mp->m_codef = 0;
	mp->m_cfrag = 0;
	mp->m_mid = 0;
	mp->m_len = 0;
	mp->m_ctx = 0;
	mp->m_tag = 0;
	mp->m_wid = 0;
	mp->m_src = 0;
	mp->m_dst = 0;
	mp->m_enc = 0;
	mp->m_flag = 0;
	mp->m_cpos = 0;
	mp->m_crc = 0;
	return mp;

fail:
	return (struct pmsg *)0;
}


/*	pmsg_free()
*
*	Free a message or list of messages.  If m_frag is nonzero, this is
*	a slave node; unlink and free it and its frag list.  Else, it's
*	a master node; free all slave nodes too.
*/

static void
pmsg_free(mp)
	struct pmsg *mp;
{
	if (mp->m_frag) {	/* slave message */
		if (mp->m_link && mp->m_rlink) {
			LISTDELETE(mp, m_link, m_rlink);
		}
		fr_unref(mp->m_frag);

	} else {	/* master */
		while (mp->m_link != mp)
			pmsg_free(mp->m_link);
	}
	pmsg_put(mp);
}


/*	pmsg_unref()
*
*	Decrement message ref count, and free message if ref count reaches 0.
*/

void
pmsg_unref(mp)
	struct pmsg *mp;
{
	if (--mp->m_ref < 1)
		pmsg_free(mp);
}


/*	pmsg_extend()
*
*	Add a new empty fragment to the end of a message.
*/

int
pmsg_extend(mp)
	struct pmsg *mp;
{
	struct frag *fp;

	if (fp = fr_new(pvmfrgsiz)) {
		fp->fr_dat += MAXHDR;
		LISTPUTBEFORE(mp->m_frag, fp, fr_link, fr_rlink);
		return 0;
	}
	return PvmNoMem;
}


/*	pmsg_decmore()
*
*	Step to the next fragment to unpack.
*/

int
pmsg_decmore(mp)
	struct pmsg *mp;
{
#if 0
	mp->m_cpos = 0;
	if (mp->m_cfrag == mp->m_frag)		/* no message left */
		return PvmNoData;
	mp->m_cfrag = mp->m_cfrag->fr_link;
	if (mp->m_cfrag == mp->m_frag)
		return PvmNoData;
	return 0;
#endif
	mp->m_cpos = 0;
	if (mp->m_cfrag == mp->m_frag)		/* no message left */
		return PvmNoData;
	while ((mp->m_cfrag = mp->m_cfrag->fr_link) != mp->m_frag)
		if (mp->m_cfrag->fr_len > 0)
			break;
	if (mp->m_cfrag == mp->m_frag)
		return PvmNoData;
	return 0;
}


/*	bytepk()
*
*	Insert a stream of bytes into message.  Allocate more fragments as
*	necessary.
*	Returns 0 else PvmNoMem if malloc fails.
*/

static int
bytepk(mp, cp, num, siz, lnc)
	struct pmsg *mp;
	char *cp;			/* base of data */
	int num;			/* num of chunks */
	int siz;			/* size of chunk */
	int lnc;			/* lead to next chunk */
{
	struct frag *fp;			/* working frag */
	int togo;					/* bytes left in chunk */
	int r;						/* bytes (space) left in frag */

	if (siz == lnc) {		/* if contiguous, treat as single chunk */
		lnc = (siz *= num);
		num = 1;
	}
	lnc -= siz;		/* now bytes between chunks */

	while (num-- > 0) {		/* copy chunks until done */

		for (togo = siz; togo > 0; ) {
			fp = mp->m_frag->fr_rlink;
			r = fp->fr_max - (fp->fr_dat - fp->fr_buf) - fp->fr_len;

			if (togo <= r) {	/* space in frag for entire chunk */
				BCOPY(cp, fp->fr_dat + fp->fr_len, togo);
				fp->fr_len += togo;
				cp += togo;
				togo = 0;

			} else {
				if (r > 0) {	/* space for part of chunk */
					BCOPY(cp, fp->fr_dat + fp->fr_len, r);
					fp->fr_len += r;
					togo -= r;
					cp += r;

				} else {		/* no space, add new frag */
					if (r = pmsg_extend(mp))
						return r;
				}
			}
		}
		cp += lnc;
	}
	return 0;
}


/*	byteupk()
*
*	Extract bytes from message, continuing to new fragments as necessary.
*	Returns 0 else PvmNoData if end of message reached early.
*/

#ifdef IMA_CSPP
static void pre_bcopy(unsigned int s1, unsigned int s2, int n);
#endif

static int
byteupk(mp, cp, num, siz, lnc)
	struct pmsg *mp;
	char *cp;			/* base of data */
	int num;			/* num of chunks */
	int siz;			/* size of chunk */
	int lnc;			/* lead to next chunk */
{
	struct frag *fp;			/* working frag */
	int togo;					/* bytes left in chunk */
	int r;						/* bytes (data) left in frag */
#ifdef IMA_CSPP
	int diff_node = mp->m_flag & MM_DIFFNODE;
#endif

	if (siz == lnc) {		/* if contiguous, treat as single chunk */
		lnc = (siz *= num);
		num = 1;
	}
	lnc -= siz;		/* now bytes between chunks */

	while (num-- > 0) {		/* copy chunks until done */

		for (togo = siz; togo > 0; ) {
			fp = mp->m_cfrag;
			r = fp->fr_len - mp->m_cpos;

			if (togo <= r) {	/* frag contains rest of chunk */
#ifdef IMA_CSPP
				fp->fr_num_unpacked += togo;
				if (diff_node) {
					pre_bcopy((unsigned int)(fp->fr_dat + mp->m_cpos),
							(unsigned int)cp, togo);
				} else
#endif
				BCOPY(fp->fr_dat + mp->m_cpos, cp, togo);
				mp->m_cpos += togo;
				cp += togo;
				togo = 0;

			} else {
				if (r > 0) {	/* frag contains part of chunk */
#ifdef IMA_CSPP
					fp->fr_num_unpacked += r;
					if (diff_node) {
						pre_bcopy((unsigned int)(fp->fr_dat + mp->m_cpos),
								(unsigned int)cp, r);
					} else
#endif
					BCOPY(fp->fr_dat + mp->m_cpos, cp, r);
					mp->m_cpos += r;
					togo -= r;
					cp += r;

				} else {		/* no space, add new frag */
					if (r = pmsg_decmore(mp))
						return r;
				}
			}
		}
		cp += lnc;
	}
	return 0;
}


/***************************
 **  Raw-format encoders  **
 **                       **
 ***************************/

/*	enc_raw_init()
*
*	Initialize a message buffer to pack raw data.
*/

static int
enc_raw_init(mp)
	struct pmsg *mp;
{
	if (!(mp->m_flag & MM_PACK)) {
		mp->m_flag &= ~MM_UPACK;
		mp->m_flag |= MM_PACK;
		if (mp->m_frag->fr_link == mp->m_frag)
			return pmsg_extend(mp);
	}
	return 0;
}


/*	dec_raw_init()
*
*	Initialize a message buffer to unpack raw data.
*/

static int
dec_raw_init(mp)
	struct pmsg *mp;
{
	if (!(mp->m_flag & MM_UPACK)) {
		mp->m_flag &= ~MM_PACK;
		mp->m_flag |= MM_UPACK;
		mp->m_cfrag = mp->m_frag->fr_link;
		mp->m_cpos = 0;
	}
	return 0;
}


/*	enc_raw_any()
*
*	Encode any data type into a raw format message buffer.
*/

static int
enc_raw_any(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	return bytepk(mp, (char*)vp, cnt, siz, std * siz);
}


/*	dec_raw_any()
*
*	Decode any data type from a raw format message buffer.
*/

static int
dec_raw_any(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	return byteupk(mp, (char*)vp, cnt, siz, std * siz);
}


/***************************
 **  XDR-format encoders  **
 **                       **
 ***************************/

#ifdef FAKEXDRFLOAT

/*
** These two are missing on some machines.
*/

static int
xdr_float(xdrp, fp)
	XDR *xdrp;
	float *fp;
{
	return xdr_long(xdrp, (long*)fp);
}


static int
xdr_double(xdrp, dp)
	XDR *xdrp;
	double *dp;
{
	return xdr_long(xdrp, (long*)dp + 1) && xdr_long(xdrp, (long*)dp);
}

#endif/*FAKEXDRFLOAT*/

#if	defined(BOGUSXDR)

/*
** Encoders for machines without XDR routines but with XDR-format
** data.
*/

static int
enc_xdr_init(mp)
	struct pmsg *mp;
{
    return enc_raw_init(mp);
}

static int
enc_xdr_byte(mp, vp, cnt, std, siz)
	struct pmsg *mp;
    void *vp;
    int cnt, std, siz;
{
	struct frag *fp;
	int cc;

	fp = mp->m_frag->fr_rlink;
	if (cc = bytepk(mp, (char*)vp, cnt, siz, std * siz))
		return cc;
	fp->fr_len = (fp->fr_len + 3) & ~3;
	return 0;
}

static int
enc_xdr_short(mp, vp, cnt, std, siz)
	struct pmsg *mp;
    void *vp;
    int cnt, std, siz;
{
    register short *np;
    register char *cp;
    char     buf[4];
    int cc = 0;

    for (np = (short*)vp; cnt-- > 0; np += std) {
        cp = (char *)np;
        buf[3] = *cp;
        buf[2] = *(cp+1);
        if (cc = bytepk(mp, buf, 4, 1, 1))
            return cc;
    }
    return 0;
}

static int
enc_xdr_int(mp, vp, cnt, std, siz)
	struct pmsg *mp;
    void *vp;
    int cnt, std, siz;
{
    register int *np;
    register char *cp;
    char     buf[4];
    int cc = 0;

    for (np = (int *)vp; cnt-- > 0; np += std) {
        cp = (char *)np;
        buf[3] = *cp;
        buf[2] = *(cp+1);
        buf[1] = *(cp+2);
        buf[0] = *(cp+3);
        if (cc = bytepk(mp, buf, 4, 1, 1))
            return cc;
    }
    return 0;
}

static int
enc_xdr_long(mp, vp, cnt, std, siz)
	struct pmsg *mp;
    void *vp;
    int cnt, std, siz;
{
    register long *np;
    register char *cp;
    char     buf[8];
    int cc = 0;

    for (np = (long *)vp; cnt-- > 0; np += std) {
        cp = (char *)np;
#ifdef USE_XDR_LONGLONG
        buf[7] = *cp;
        buf[6] = *(cp+1);
        buf[5] = *(cp+2);
        buf[4] = *(cp+3);
        buf[3] = *(cp+4);
        buf[2] = *(cp+5);
        buf[1] = *(cp+6);
        buf[0] = *(cp+7);
#else
        buf[3] = *cp;
        buf[2] = *(cp+1);
        buf[1] = *(cp+2);
        buf[0] = *(cp+3);
#endif
        if (cc = bytepk(mp, buf,
#ifdef USE_XDR_LONGLONG
				8,
#else
				4,
#endif
				1, 1))
            return cc;
    }
    return 0;
}

static int
enc_xdr_float(mp, vp, cnt, std, siz)
	struct pmsg *mp;
    void *vp;
    int cnt, std, siz;
{
    register float *np;
    register char *cp;
    char     buf[4];
    int cc = 0;

    for (np = (float *)vp; cnt-- > 0; np += std) {
        cp = (char *)np;
        buf[3] = *cp;
        buf[2] = *(cp+1);
        buf[1] = *(cp+2);
        buf[0] = *(cp+3);
        if (cc = bytepk(mp, buf, 4, 1, 1))
            return cc;
    }
    return 0;
}

static int
enc_xdr_double(mp, vp, cnt, std, siz)
	struct pmsg *mp;
    void *vp;
    int cnt, std, siz;
{
    register double *np;
    register char *cp;
    char     buf[8];
    int cc = 0;

    for (np = (double *)vp; cnt-- > 0; np += std) {
        cp = (char *)np;
        buf[7] = *cp;
        buf[6] = *(cp+1);
        buf[5] = *(cp+2);
        buf[4] = *(cp+3);
        buf[3] = *(cp+4);
        buf[2] = *(cp+5);
        buf[1] = *(cp+6);
        buf[0] = *(cp+7);
        if (cc = bytepk(mp, buf, 8, 1, 1))
            return cc;
    }
    return 0;
}

typedef struct {float r, i;} complex;
typedef struct {double r, i;} dcplx;

static int
enc_xdr_cplx(mp, vp, cnt, std, siz)
	struct pmsg *mp;
    void *vp;
    int cnt, std, siz;
{
    register complex *np;
    register char *cp;
    char buf[8];
    int cc = 0;

    for (np = (complex *)vp; cnt-- > 0; np += std) {
        cp = (char *)np;
        buf[3] = *cp;
        buf[2] = *(cp+1);
        buf[1] = *(cp+2);
        buf[0] = *(cp+3);
        buf[7] = *(cp+4);
        buf[6] = *(cp+5);
        buf[5] = *(cp+6);
        buf[4] = *(cp+7);
        if (cc = bytepk(mp, buf, 8, 1, 1))
            return cc;
    }
    return 0;
}

static int
enc_xdr_dcplx(mp, vp, cnt, std, siz)
	struct pmsg *mp;
    void *vp;
    int cnt, std, siz;
{
    register dcplx *np;
    register char *cp;
    char buf[16];
    int cc = 0;

    for (np = (dcplx *)vp; cnt-- > 0; np += std) {
        cp = (char *)np;
        buf[7] = *cp;
        buf[6] = *(cp+1);
        buf[5] = *(cp+2);
        buf[4] = *(cp+3);
        buf[3] = *(cp+4);
        buf[2] = *(cp+5);
        buf[1] = *(cp+6);
        buf[0] = *(cp+7);
        buf[15] = *(cp+8);
        buf[14] = *(cp+9);
        buf[13] = *(cp+10);
        buf[12] = *(cp+11);
        buf[11] = *(cp+12);
        buf[10] = *(cp+13);
        buf[9] = *(cp+14);
        buf[8] = *(cp+15);
        if (cc = bytepk(mp, buf, 16, 1, 1))
            return cc;
    }
    return 0;
}

static int
dec_xdr_init(mp)
	struct pmsg *mp;
{
    return dec_raw_init(mp);
}

static int
dec_xdr_byte(mp, vp, cnt, std, siz)
	struct pmsg *mp;
    void *vp;
    int cnt, std, siz;
{
	int cc;

	if (cc = byteupk(mp, (char*)vp, cnt, siz, std * siz))
		return cc;
	mp->m_cpos = (mp->m_cpos + 3) & ~3;
	return 0;
}

static int
dec_xdr_short(mp, vp, cnt, std, siz)
	struct pmsg *mp;
    void *vp;
    int cnt, std, siz;
{
    register short *np;
    register char *cp;
    char     buf[4];
    int cc = 0;

    for (np = (short*)vp; cnt-- > 0; np += std) {
        if (cc = byteupk(mp, buf, 4, 1, 1))
            return cc;
        cp = (char *)np;
        *cp = buf[3];
        *(cp+1) = buf[2];
    }
    return 0;
}

static int
dec_xdr_int(mp, vp, cnt, std, siz)
	struct pmsg *mp;
    void *vp;
    int cnt, std, siz;
{
    register int *np;
    register char *cp;
    char     buf[4];
    int cc = 0;

    for (np = (int *)vp; cnt-- > 0; np += std) {
        if (cc = byteupk(mp, buf, 4, 1, 1))
            return cc;
        cp = (char *)np;
        *cp = buf[3];
        *(cp+1) = buf[2];
        *(cp+2) = buf[1];
        *(cp+3) = buf[0];
    }
    return 0;
}

static int
dec_xdr_long(mp, vp, cnt, std, siz)
	struct pmsg *mp;
    void *vp;
    int cnt, std, siz;
{
    register long *np;
    register char *cp;
    char     buf[8];
    int cc = 0;

    for (np = (long *)vp; cnt-- > 0; np += std) {
        if (cc = byteupk(mp, buf,
#ifdef USE_XDR_LONGLONG
				8,
#else
				4,
#endif
				1, 1))
            return cc;
        cp = (char *)np;
#ifdef USE_XDR_LONGLONG
        *cp     = buf[7];
        *(cp+1) = buf[6];
        *(cp+2) = buf[5];
        *(cp+3) = buf[4];
        *(cp+4) = buf[3];
        *(cp+5) = buf[2];
        *(cp+6) = buf[1];
        *(cp+7) = buf[0];
#else
        *cp = buf[3];
        *(cp+1) = buf[2];
        *(cp+2) = buf[1];
        *(cp+3) = buf[0];
#endif
    }
    return 0;
}

static int
dec_xdr_float(mp, vp, cnt, std, siz)
	struct pmsg *mp;
    void *vp;
    int cnt, std, siz;
{
    register float *np;
    register char *cp;
    char     buf[4];
    int cc = 0;

    for (np = (float *)vp; cnt-- > 0; np += std) {
        if (cc = byteupk(mp, buf, 4, 1, 1))
            return cc;
        cp = (char *)np;
        *cp = buf[3];
        *(cp+1) = buf[2];
        *(cp+2) = buf[1];
        *(cp+3) = buf[0];
    }
    return 0;
}

static int
dec_xdr_double(mp, vp, cnt, std, siz)
	struct pmsg *mp;
    void *vp;
    int cnt, std, siz;
{
    register double *np;
    register char *cp;
    char     buf[8];
    int cc = 0;

    for (np = (double *)vp; cnt-- > 0; np += std) {
        if (cc = byteupk(mp, buf, 8, 1, 1))
            return cc;
        cp = (char *)np;
        *cp = buf[7];
        *(cp+1) = buf[6];
        *(cp+2) = buf[5];
        *(cp+3) = buf[4];
        *(cp+4) = buf[3];
        *(cp+5) = buf[2];
        *(cp+6) = buf[1];
        *(cp+7) = buf[0];
    }
    return 0;
}

static int
dec_xdr_cplx(mp, vp, cnt, std, siz)
	struct pmsg *mp;
    void *vp;
    int cnt, std, siz;
{
    register complex *np;
    register char *cp;
    char     buf[8];
    int cc = 0;

    for (np = (complex *)vp; cnt-- > 0; np += std) {
        if (cc = byteupk(mp, buf, 8, 1, 1))
            return cc;
        cp = (char *)np;
        *cp = buf[3];
        *(cp+1) = buf[2];
        *(cp+2) = buf[1];
        *(cp+3) = buf[0];
        *(cp+4) = buf[7];
        *(cp+5) = buf[6];
        *(cp+6) = buf[5];
        *(cp+7) = buf[4];
    }
    return 0;
}

static int
dec_xdr_dcplx(mp, vp, cnt, std, siz)
	struct pmsg *mp;
    void *vp;
    int cnt, std, siz;
{
    register dcplx *np;
    register char *cp;
    char     buf[16];
    int cc = 0;

    for (np = (dcplx *)vp; cnt-- > 0; np += std) {
        if (cc = byteupk(mp, buf, 16, 1, 1))
            return cc;
        cp = (char *)np;
        *cp     = buf[7];
        *(cp+1) = buf[6];
        *(cp+2) = buf[5];
        *(cp+3) = buf[4];
        *(cp+4) = buf[3];
        *(cp+5) = buf[2];
        *(cp+6) = buf[1];
        *(cp+7) = buf[0];
        *(cp+8) = buf[15];
        *(cp+9) = buf[14];
        *(cp+10) = buf[13];
        *(cp+11) = buf[12];
        *(cp+12) = buf[11];
        *(cp+13) = buf[10];
        *(cp+14) = buf[9];
        *(cp+15) = buf[8];
    }
    return 0;
}

#else	/*BOGUSXDR*/

/*
** These are the normal XDR encoders for machines with XDR functions.
*/

/*	enc_xdr_init()
*
*	Initialize a message buffer to pack XDR format data
*/

static int
enc_xdr_init(mp)
	struct pmsg *mp;
{
	struct frag *fp;
	int cc;

	if (!(mp->m_flag & MM_PACK)) {
		mp->m_flag &= ~MM_UPACK;
		mp->m_flag |= MM_PACK;
		if ((fp = mp->m_frag->fr_link) == mp->m_frag) {
			if (cc = pmsg_extend(mp))
				return cc;
			fp = fp->fr_link;
		}

		xdrmem_create(&mp->m_xdr,
				fp->fr_dat,
				(unsigned)(fp->fr_max - (fp->fr_dat - fp->fr_buf)),
				XDR_ENCODE);
	}
	return 0;
}


/*	enc_xdr_step()
*
*	Add a new fragment to an XDR format message
*/

static int
enc_xdr_step(mp)
	struct pmsg *mp;
{
	struct frag *fp;
	int cc;

	if (cc = pmsg_extend(mp))
		return cc;
	fp = mp->m_frag->fr_rlink;
	xdrmem_create(&mp->m_xdr,
			fp->fr_dat,
			(unsigned)(fp->fr_max - (fp->fr_dat - fp->fr_buf)),
			XDR_ENCODE);
	return 0;
}


static int
enc_xdr_byte(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	struct frag *fp;
	int cc;

	fp = mp->m_frag->fr_rlink;
	if (cc = bytepk(mp, (char*)vp, cnt, 1, std))
		return cc;
	if (fp != mp->m_frag->fr_rlink) {
		fp = mp->m_frag->fr_rlink;
		xdrmem_create(&mp->m_xdr,
				fp->fr_dat,
				(unsigned)(fp->fr_max - (fp->fr_dat - fp->fr_buf)),
				XDR_ENCODE);
	}
	fp->fr_len = (fp->fr_len + 3) & ~3;
	xdr_setpos(&mp->m_xdr, fp->fr_len);
	return 0;
}


static int
enc_xdr_short(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	register short *np;
	int cc = 0;

	for (np = (short*)vp; cnt-- > 0; np += std)
		if (!xdr_short(&mp->m_xdr, np)) {
			mp->m_frag->fr_rlink->fr_len = xdr_getpos(&mp->m_xdr);
			if (cc = enc_xdr_step(mp))
				break;
			else
				if (!xdr_short(&mp->m_xdr, np)) {
					cc = PvmNoMem;
					break;
				}
		}
	mp->m_frag->fr_rlink->fr_len = xdr_getpos(&mp->m_xdr);
	return cc;
}


static int
enc_xdr_int(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	register int *np;
	int cc = 0;

	for (np = (int*)vp; cnt-- > 0; np += std)
		if (!xdr_int(&mp->m_xdr, np)) {
			mp->m_frag->fr_rlink->fr_len = xdr_getpos(&mp->m_xdr);
			if (cc = enc_xdr_step(mp))
				break;
			else
				if (!xdr_int(&mp->m_xdr, np)) {
					cc = PvmNoMem;
					break;
				}
		}
	mp->m_frag->fr_rlink->fr_len = xdr_getpos(&mp->m_xdr);
	return cc;
}


static int
enc_xdr_long(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	register long *np;
	int cc = 0;

	for (np = (long*)vp; cnt-- > 0; np += std)
#ifdef USE_XDR_LONGLONG
		if (!xdr_longlong_t(&mp->m_xdr, np))
#else
		if (!xdr_long(&mp->m_xdr, np))
#endif
		{
#ifdef USE_XDR_LONGLONG
			if ((*np & ~(long)0x7fffffffffffffff)
			&& (*np & ~(long)0x7fffffffffffffff)
					!= ~(long)0x7fffffffffffffff)
#else
			if ((*np & ~(long)0x7fffffff)
			&& (*np & ~(long)0x7fffffff) != ~(long)0x7fffffff)
#endif
			{
				cc = PvmOverflow;
				break;
			}
			mp->m_frag->fr_rlink->fr_len = xdr_getpos(&mp->m_xdr);
			if (cc = enc_xdr_step(mp))
				break;
			else
#ifdef USE_XDR_LONGLONG
				if (!xdr_longlong_t(&mp->m_xdr, np))
#else
				if (!xdr_long(&mp->m_xdr, np))
#endif
				{
					cc = PvmNoMem;
					break;
				}
		}
	mp->m_frag->fr_rlink->fr_len = xdr_getpos(&mp->m_xdr);
	return cc;
}


static int
enc_xdr_ulong(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	register unsigned long *np;
	int cc = 0;

	for (np = (unsigned long*)vp; cnt-- > 0; np += std)
#ifdef USE_XDR_LONGLONG
		if (!xdr_u_longlong_t(&mp->m_xdr, np))
#else
		if (!xdr_u_long(&mp->m_xdr, np))
#endif
		{
#ifdef USE_XDR_LONGLONG
			if (*np & ~(long)0xffffffffffffffff)
#else
			if (*np & ~(long)0xffffffff)
#endif
			{
				cc = PvmOverflow;
				break;
			}
			mp->m_frag->fr_rlink->fr_len = xdr_getpos(&mp->m_xdr);
			if (cc = enc_xdr_step(mp))
				break;
			else
#ifdef USE_XDR_LONGLONG
				if (!xdr_u_longlong_t(&mp->m_xdr, np))
#else
				if (!xdr_u_long(&mp->m_xdr, np))
#endif
				{
					cc = PvmNoMem;
					break;
				}
		}
	mp->m_frag->fr_rlink->fr_len = xdr_getpos(&mp->m_xdr);
	return cc;
}


static int
enc_xdr_float(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	register float *fp;
	int cc = 0;

	for (fp = (float*)vp; cnt-- > 0; fp += std)
		if (!xdr_float(&mp->m_xdr, fp)) {
			mp->m_frag->fr_rlink->fr_len = xdr_getpos(&mp->m_xdr);
			if (cc = enc_xdr_step(mp))
				break;
			else
				if (!xdr_float(&mp->m_xdr, fp)) {
					cc = PvmNoMem;
					break;
				}
		}
	mp->m_frag->fr_rlink->fr_len = xdr_getpos(&mp->m_xdr);
	return cc;
}


static int
enc_xdr_double(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	register double *dp;
	int cc = 0;
	int n;

	for (dp = (double*)vp; cnt-- > 0; dp += std) {
		if (!xdr_double(&mp->m_xdr, dp)) {
			if (cc = enc_xdr_step(mp))
				break;
			else
				if (!xdr_double(&mp->m_xdr, dp)) {
					cc = PvmNoMem;
					break;
				}
		}
		mp->m_frag->fr_rlink->fr_len = xdr_getpos(&mp->m_xdr);
	}
	return cc;
}


static int
enc_xdr_cplx(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	register float *xp;
	int cc = 0;

	std = std * 2 - 1;
	for (xp = (float*)vp; cnt-- > 0; xp += std) {
		if (!xdr_float(&mp->m_xdr, xp)) {
			mp->m_frag->fr_rlink->fr_len = xdr_getpos(&mp->m_xdr);
			if (cc = enc_xdr_step(mp))
				break;
			else
				if (!xdr_float(&mp->m_xdr, xp)) {
					cc = PvmNoMem;
					break;
				}
		}
		xp++;
		if (!xdr_float(&mp->m_xdr, xp)) {
			mp->m_frag->fr_rlink->fr_len = xdr_getpos(&mp->m_xdr);
			if (cc = enc_xdr_step(mp))
				break;
			else
				if (!xdr_float(&mp->m_xdr, xp)) {
					cc = PvmNoMem;
					break;
				}
		}
	}
	mp->m_frag->fr_rlink->fr_len = xdr_getpos(&mp->m_xdr);
	return cc;
}


static int
enc_xdr_dcplx(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	register double *zp;
	int cc = 0;

	std = std * 2 - 1;
	for (zp = (double*)vp; cnt-- > 0; zp += std) {
		if (!xdr_double(&mp->m_xdr, zp)) {
			if (cc = enc_xdr_step(mp))
				break;
			else
				if (!xdr_double(&mp->m_xdr, zp)) {
					cc = PvmNoMem;
					break;
				}
		}
		mp->m_frag->fr_rlink->fr_len = xdr_getpos(&mp->m_xdr);
		zp++;
		if (!xdr_double(&mp->m_xdr, zp)) {
			if (cc = enc_xdr_step(mp))
				break;
			else
				if (!xdr_double(&mp->m_xdr, zp)) {
					cc = PvmNoMem;
					break;
				}
		}
		mp->m_frag->fr_rlink->fr_len = xdr_getpos(&mp->m_xdr);
	}
	return cc;
}


/*	dec_xdr_init()
*
*	Initialize a message buffer to unpack XDR format data.
*/

static int
dec_xdr_init(mp)
	struct pmsg *mp;
{
	struct frag *fp = mp->m_frag->fr_link;

	if (!(mp->m_flag & MM_UPACK)) {
		mp->m_flag &= ~MM_PACK;
		mp->m_flag |= MM_UPACK;
		mp->m_cfrag = fp;
		mp->m_cpos = 0;
		xdrmem_create(&mp->m_xdr, fp->fr_dat, (unsigned)(fp->fr_len),
				XDR_DECODE);
	}
	return 0;
}


/*	dec_xdr_step()
*
*	Step to the next fragment of an XDR format message.
*
*	The current fragment might not be fully consumed - messages are
*	refragmented on 4-byte boundaries, but XDR 'atomic' sizes for
*	us are 4 bytes and 8 bytes (doubles).
*	If the next fragment exists, copy the remaining bytes to the head
*	of its buffer and continue unpacking.
*	XXX 1. There might not be enough headroom (unlikely).
*	XXX 2. If we share databufs we can't modify them (maybe?).
*/

static int
dec_xdr_step(mp)
	struct pmsg *mp;
{
	struct frag *fp;
	int cc;
	int l;
	char *p;

	if (mp->m_cpos != mp->m_cfrag->fr_len) {
		p = mp->m_cfrag->fr_dat + mp->m_cpos;
		l = mp->m_cfrag->fr_len - mp->m_cpos;

		if (cc = pmsg_decmore(mp))
			return cc;

		fp = mp->m_cfrag;
		if (fp->fr_dat - fp->fr_buf < l) {
			pvmlogerror("aaugh, no space for fixup, kill me\n");
			return PvmBadMsg;

		} else {
			/*
			pvmlogprintf("fragment has %d bytes left, doing fixup\n",
					l);
			*/
			fp->fr_dat -= l;
			fp->fr_len += l;
			BCOPY(p, fp->fr_dat, l);
		}

	} else {
		if (cc = pmsg_decmore(mp))
			return cc;
		fp = mp->m_cfrag;
	}
	xdrmem_create(&mp->m_xdr, fp->fr_dat, (unsigned)(fp->fr_len),
			XDR_DECODE);
	return 0;
}


static int
dec_xdr_byte(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	struct frag *fp;
	int cc;

	fp = mp->m_cfrag;
	if (cc = byteupk(mp, (char*)vp, cnt, 1, std))
		return cc;
	if (fp != mp->m_cfrag) {
		fp = mp->m_cfrag;
		xdrmem_create(&mp->m_xdr, fp->fr_dat, (unsigned)(fp->fr_len),
				XDR_DECODE);
	}
	mp->m_cpos = (mp->m_cpos + 3) & ~3;
	xdr_setpos(&mp->m_xdr, mp->m_cpos);
	return 0;
}


static int
dec_xdr_short(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	register short *np;
	int cc = 0;

	for (np = (short*)vp; cnt-- > 0; np += std)
		if (!xdr_short(&mp->m_xdr, np)) {
			mp->m_cpos = xdr_getpos(&mp->m_xdr);
			if (cc = dec_xdr_step(mp))
				break;
			else
				if (!xdr_short(&mp->m_xdr, np)) {
					cc = PvmNoData;
					break;
				}
		}
	mp->m_cpos = xdr_getpos(&mp->m_xdr);
	return cc;
}


static int
dec_xdr_int(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	register int *np;
	int cc = 0;

	for (np = (int*)vp; cnt-- > 0; np += std)
		if (!xdr_int(&mp->m_xdr, np)) {
			mp->m_cpos = xdr_getpos(&mp->m_xdr);
			if (cc = dec_xdr_step(mp))
				break;
			else
				if (!xdr_int(&mp->m_xdr, np)) {
					cc = PvmNoData;
					break;
				}
		}
	mp->m_cpos = xdr_getpos(&mp->m_xdr);
	return cc;
}


static int
dec_xdr_long(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	register long *np;
	int cc = 0;

	for (np = (long*)vp; cnt-- > 0; np += std)
#ifdef USE_XDR_LONGLONG
		if (!xdr_longlong_t(&mp->m_xdr, np))
#else
		if (!xdr_long(&mp->m_xdr, np))
#endif
		{
			mp->m_cpos = xdr_getpos(&mp->m_xdr);
			if (cc = dec_xdr_step(mp))
				break;
			else
#ifdef USE_XDR_LONGLONG
				if (!xdr_longlong_t(&mp->m_xdr, np))
#else
				if (!xdr_long(&mp->m_xdr, np))
#endif
				{
					cc = PvmNoData;
					break;
				}
		}
	mp->m_cpos = xdr_getpos(&mp->m_xdr);
	return cc;
}


static int
dec_xdr_ushort(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	register unsigned short *np;
	int cc = 0;

	for (np = (unsigned short*)vp; cnt-- > 0; np += std)
		if (!xdr_u_short(&mp->m_xdr, np)) {
			mp->m_cpos = xdr_getpos(&mp->m_xdr);
			if (cc = dec_xdr_step(mp))
				break;
			else
				if (!xdr_u_short(&mp->m_xdr, np)) {
					cc = PvmNoData;
					break;
				}
		}
	mp->m_cpos = xdr_getpos(&mp->m_xdr);
	return cc;
}


static int
dec_xdr_uint(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	register unsigned int *np;
	int cc = 0;

	for (np = (unsigned int*)vp; cnt-- > 0; np += std)
		if (!xdr_u_int(&mp->m_xdr, np)) {
			mp->m_cpos = xdr_getpos(&mp->m_xdr);
			if (cc = dec_xdr_step(mp))
				break;
			else
				if (!xdr_u_int(&mp->m_xdr, np)) {
					cc = PvmNoData;
					break;
				}
		}
	mp->m_cpos = xdr_getpos(&mp->m_xdr);
	return cc;
}


static int
dec_xdr_ulong(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	register unsigned long *np;
	int cc = 0;

	for (np = (unsigned long*)vp; cnt-- > 0; np += std)
#ifdef USE_XDR_LONGLONG
		if (!xdr_u_longlong_t(&mp->m_xdr, np))
#else
		if (!xdr_u_long(&mp->m_xdr, np))
#endif
		{
			mp->m_cpos = xdr_getpos(&mp->m_xdr);
			if (cc = dec_xdr_step(mp))
				break;
			else
#ifdef USE_XDR_LONGLONG
				if (!xdr_u_longlong_t(&mp->m_xdr, np))
#else
				if (!xdr_u_long(&mp->m_xdr, np))
#endif
				{
					cc = PvmNoData;
					break;
				}
		}
	mp->m_cpos = xdr_getpos(&mp->m_xdr);
	return cc;
}


static int
dec_xdr_float(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	register float *fp;
	int cc = 0;

	for (fp = (float*)vp; cnt-- > 0; fp += std)
		if (!xdr_float(&mp->m_xdr, fp)) {
			mp->m_cpos = xdr_getpos(&mp->m_xdr);
			if (cc = dec_xdr_step(mp))
				break;
			else
				if (!xdr_float(&mp->m_xdr, fp)) {
					cc = PvmNoData;
					break;
				}
		}
	mp->m_cpos = xdr_getpos(&mp->m_xdr);
	return cc;
}


static int
dec_xdr_double(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	register double *dp;
	int cc = 0;

	for (dp = (double*)vp; cnt-- > 0; dp += std) {
		if (!xdr_double(&mp->m_xdr, dp)) {
			if (cc = dec_xdr_step(mp))
				break;
			else
				if (!xdr_double(&mp->m_xdr, dp)) {
					cc = PvmNoData;
					break;
				}
		}
		mp->m_cpos = xdr_getpos(&mp->m_xdr);
	}
	return cc;
}


static int
dec_xdr_cplx(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	register float *xp;
	int cc = 0;

	std = std * 2 - 1;
	for (xp = (float*)vp; cnt-- > 0; xp += std) {
		if (!xdr_float(&mp->m_xdr, xp)) {
			mp->m_cpos = xdr_getpos(&mp->m_xdr);
			if (cc = dec_xdr_step(mp))
				break;
			else
				if (!xdr_float(&mp->m_xdr, xp)) {
					cc = PvmNoData;
					break;
				}
		}
		xp++;
		if (!xdr_float(&mp->m_xdr, xp)) {
			mp->m_cpos = xdr_getpos(&mp->m_xdr);
			if (cc = dec_xdr_step(mp))
				break;
			else
				if (!xdr_float(&mp->m_xdr, xp)) {
					cc = PvmNoData;
					break;
				}
		}
	}
	mp->m_cpos = xdr_getpos(&mp->m_xdr);
	return cc;
}


static int
dec_xdr_dcplx(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	register double *zp;
	int cc = 0;

	std = std * 2 - 1;
	for (zp = (double*)vp; cnt-- > 0; zp += std) {
		if (!xdr_double(&mp->m_xdr, zp)) {
			if (cc = dec_xdr_step(mp))
				break;
			else
				if (!xdr_double(&mp->m_xdr, zp)) {
					cc = PvmNoData;
					break;
				}
		}
		mp->m_cpos = xdr_getpos(&mp->m_xdr);
		zp++;
		if (!xdr_double(&mp->m_xdr, zp)) {
			if (cc = dec_xdr_step(mp))
				break;
			else
				if (!xdr_double(&mp->m_xdr, zp)) {
					cc = PvmNoData;
					break;
				}
		}
		mp->m_cpos = xdr_getpos(&mp->m_xdr);
	}
	return cc;
}

#endif	/*BOGUSXDR*/

/*************************
 **  In-place encoders  **
 **                     **
 *************************/

static int
enc_inp_init(mp)
	struct pmsg *mp;
{
	return 0;
}


static int
enc_inp_any(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	struct frag *fp;

	if (std != 1)
		return PvmNotImpl;
	if (vp && cnt && siz) {
		fp = fr_snew((char *)vp, cnt * siz);
		LISTPUTBEFORE(mp->m_frag, fp, fr_link, fr_rlink);
	}
#if 0
	if (std != 1) {
		if (std < 1)
			return PvmBadParam;
		fp->fr_max = fp->fr_len = siz + (cnt - 1) * std * siz;
		fp->fr_csz = siz;
		fp->fr_lnc = std * siz;
		fp->fr_u.spr = 1;
	}
#endif
	return 0;
}


/*********************
 **  Trace encoders **
 **                 **
 *********************/

static int
enc_trc_init(mp)
	struct pmsg *mp;
{
    return enc_xdr_init(mp);
}

int /* not static */
enc_trc_hdr(mp)
	struct pmsg *mp;
{
	struct timeval timestamp;

	int tsec, tusec;
	int tmp;
	int cc;

	gettimeofday( &timestamp, (struct timezone *) 0 );

	tsec = (int) timestamp.tv_sec;
	tusec = (int) timestamp.tv_usec;

	if ( (cc = enc_xdr_init(mp)) )
		return( cc );

	tmp = TEV_MARK_USER_EVENT_RECORD;
	if ((cc = enc_xdr_int(mp, (void *) &tmp, 1, 1, (int) sizeof(int))))
		return( cc );

	tmp = TEV_USER_DEFINED;
	if ((cc = enc_xdr_int(mp, (void *) &tmp, 1, 1, (int) sizeof(int))))
		return( cc );

	tmp = strlen( pvmtevinfo[TEV_USER_DEFINED].name ) + 1;
	if ((cc = enc_xdr_int(mp, (void *) &tmp, 1, 1, (int) sizeof(int))))
		return( cc );

	if ((cc = enc_xdr_byte(mp,
			(void *)(pvmtevinfo[TEV_USER_DEFINED].name), tmp, 1, 1)))
		return( cc );

	if ((cc = enc_xdr_int(mp, (void *) &tsec, 1, 1,
			(int) sizeof(int))))
		return( cc );
	if ((cc = enc_xdr_int(mp, (void *) &tusec, 1, 1,
			(int) sizeof(int))))
		return( cc );

	return( 0 );
}

int /* not static */
enc_trc_fin(mp)
	struct pmsg *mp;
{
	int tmp;

	tmp = TEV_MARK_USER_EVENT_RECORD_END;
	return( enc_xdr_int(mp, (void *) &tmp, 1, 1, (int) sizeof(int)) );
}

static int
enc_trc_byte(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	int type;
	int cc;

	if ( cnt )
	{
		if ( cnt == 1 ) {
			type = TEV_DATA_BYTE | TEV_DATA_SCALAR;
			if ( (cc = enc_xdr_int(mp, (void *) &type, 1, 1,
					(int) sizeof(int))) )
				return cc;
		}
		else {
			type = TEV_DATA_BYTE | TEV_DATA_ARRAY;
			if ( (cc = enc_xdr_int(mp, (void *) &type, 1, 1,
					(int) sizeof(int))) )
				return cc;
			if ( (cc = enc_xdr_int(mp, (void *) &cnt, 1, 1,
					(int) sizeof(int))) )
				return cc;
		}
		return enc_xdr_byte(mp, vp, cnt, std, siz);
	}
	return 0;
}

static int
enc_trc_short(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	int type;
	int cc;

	if ( cnt )
	{
		if ( cnt == 1 ) {
			type = TEV_DATA_SHORT | TEV_DATA_SCALAR;
			if ( (cc = enc_xdr_int(mp, (void *) &type, 1, 1,
					(int) sizeof(int))) )
				return cc;
		}
		else {
			type = TEV_DATA_SHORT | TEV_DATA_ARRAY;
			if ( (cc = enc_xdr_int(mp, (void *) &type, 1, 1,
					(int) sizeof(int))) )
				return cc;
			if ( (cc = enc_xdr_int(mp, (void *) &cnt, 1, 1,
					(int) sizeof(int))) )
				return cc;
		}
		return enc_xdr_short(mp, vp, cnt, std, siz);
	}
	return 0;
}

static int
enc_trc_int(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	int type;
	int cc;

	if ( cnt )
	{
		if ( cnt == 1 ) {
			type = TEV_DATA_INT | TEV_DATA_SCALAR;
			if ( (cc = enc_xdr_int(mp, (void *) &type, 1, 1,
					(int) sizeof(int))) )
				return cc;
		}
		else {
			type = TEV_DATA_INT | TEV_DATA_ARRAY;
			if ( (cc = enc_xdr_int(mp, (void *) &type, 1, 1,
					(int) sizeof(int))) )
				return cc;
			if ( (cc = enc_xdr_int(mp, (void *) &cnt, 1, 1,
					(int) sizeof(int))) )
				return cc;
		}
		return enc_xdr_int(mp, vp, cnt, std, siz);
	}

	/* Weasel Hack to Sneak in Trace Markers, with cnt == 0...  :-)   */
	/* Need to be able to insert markers without other data type and  */
	/* array size crapola - it's either this or a new pvm_pkmarker()  */
	/* routine and an enc_mrk for each encoding (which is more bogus).*/
	else if ( vp != NULL && *((int *) vp) < 0 ) {
		return enc_xdr_int(mp, vp, 1, 1, (int) sizeof(int));
	}

	return 0;
}

static int
enc_trc_long(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	int type;
	int cc;

	if ( cnt )
	{
		if ( cnt == 1 ) {
			type = TEV_DATA_LONG | TEV_DATA_SCALAR;
			if ( (cc = enc_xdr_int(mp, (void *) &type, 1, 1,
					(int) sizeof(int))) )
				return cc;
		}
		else {
			type = TEV_DATA_LONG | TEV_DATA_ARRAY;
			if ( (cc = enc_xdr_int(mp, (void *) &type, 1, 1,
					(int) sizeof(int))) )
				return cc;
			if ( (cc = enc_xdr_int(mp, (void *) &cnt, 1, 1,
					(int) sizeof(int))) )
				return cc;
		}
		return enc_xdr_long(mp, vp, cnt, std, siz);
	}
	return 0;
}

static int
enc_trc_ulong(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	int type;
	int cc;

	if ( cnt )
	{
		if ( cnt == 1 ) {
			type = TEV_DATA_ULONG | TEV_DATA_SCALAR;
			if ( (cc = enc_xdr_int(mp, (void *) &type, 1, 1,
					(int) sizeof(int))) )
				return cc;
		}
		else {
			type = TEV_DATA_ULONG | TEV_DATA_ARRAY;
			if ( (cc = enc_xdr_int(mp, (void *) &type, 1, 1,
					(int) sizeof(int))) )
				return cc;
			if ( (cc = enc_xdr_int(mp, (void *) &cnt, 1, 1,
					(int) sizeof(int))) )
				return cc;
		}
#if	defined(I860_NODE)
		/* XXX are handwritten anyhow */
		return enc_xdr_long(mp, vp, cnt, std, siz);
#else
		return enc_xdr_ulong(mp, vp, cnt, std, siz);
#endif
	}
	return 0;
}

static int
enc_trc_float(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	int type;
	int cc;

	if ( cnt )
	{
		if ( cnt == 1 ) {
			type = TEV_DATA_FLOAT | TEV_DATA_SCALAR;
			if ( (cc = enc_xdr_int(mp, (void *) &type, 1, 1,
					(int) sizeof(int))) )
				return cc;
		}
		else {
			type = TEV_DATA_FLOAT | TEV_DATA_ARRAY;
			if ( (cc = enc_xdr_int(mp, (void *) &type, 1, 1,
					(int) sizeof(int))) )
				return cc;
			if ( (cc = enc_xdr_int(mp, (void *) &cnt, 1, 1,
					(int) sizeof(int))) )
				return cc;
		}
		return enc_xdr_float(mp, vp, cnt, std, siz);
	}
	return 0;
}

static int
enc_trc_double(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	int type;
	int cc;

	if ( cnt )
	{
		if ( cnt == 1 ) {
			type = TEV_DATA_DOUBLE | TEV_DATA_SCALAR;
			if ( (cc = enc_xdr_int(mp, (void *) &type, 1, 1,
					(int) sizeof(int))) )
				return cc;
		}
		else {
			type = TEV_DATA_DOUBLE | TEV_DATA_ARRAY;
			if ( (cc = enc_xdr_int(mp, (void *) &type, 1, 1,
					(int) sizeof(int))) )
				return cc;
			if ( (cc = enc_xdr_int(mp, (void *) &cnt, 1, 1,
					(int) sizeof(int))) )
				return cc;
		}
		return enc_xdr_double(mp, vp, cnt, std, siz);
	}
	return 0;
}

static int
enc_trc_cplx(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	int type;
	int cc;

	if ( cnt )
	{
		if ( cnt == 1 ) {
			type = TEV_DATA_CPLX | TEV_DATA_SCALAR;
			if ( (cc = enc_xdr_int(mp, (void *) &type, 1, 1,
					(int) sizeof(int))) )
				return cc;
		}
		else {
			type = TEV_DATA_CPLX | TEV_DATA_ARRAY;
			if ( (cc = enc_xdr_int(mp, (void *) &type, 1, 1,
					(int) sizeof(int))) )
				return cc;
			if ( (cc = enc_xdr_int(mp, (void *) &cnt, 1, 1,
					(int) sizeof(int))) )
				return cc;
		}
		return enc_xdr_cplx(mp, vp, cnt, std, siz);
	}
	return 0;
}

static int
enc_trc_dcplx(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	int type;
	int cc;

	if ( cnt )
	{
		if ( cnt == 1 ) {
			type = TEV_DATA_DCPLX | TEV_DATA_SCALAR;
			if ( (cc = enc_xdr_int(mp, (void *) &type, 1, 1,
					(int) sizeof(int))) )
				return cc;
		}
		else {
			type = TEV_DATA_DCPLX | TEV_DATA_ARRAY;
			if ( (cc = enc_xdr_int(mp, (void *) &type, 1, 1,
					(int) sizeof(int))) )
				return cc;
			if ( (cc = enc_xdr_int(mp, (void *) &cnt, 1, 1,
					(int) sizeof(int))) )
				return cc;
		}
		return enc_xdr_dcplx(mp, vp, cnt, std, siz);
	}
	return 0;
}

static int
dec_trc_init(mp)
	struct pmsg *mp;
{
	return dec_xdr_init(mp);
}

static int
dec_trc_byte(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	return dec_xdr_byte(mp, vp, cnt, std, siz);
}

static int
dec_trc_short(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	return dec_xdr_short(mp, vp, cnt, std, siz);
}

static int
dec_trc_int(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	return dec_xdr_int(mp, vp, cnt, std, siz);
}

static int
dec_trc_long(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	return dec_xdr_long(mp, vp, cnt, std, siz);
}

static int
dec_trc_ushort(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
#if	defined(I860_NODE)
	/* XXX are handwritten anyhow */
	return dec_xdr_short(mp, vp, cnt, std, siz);
#else
	return dec_xdr_ushort(mp, vp, cnt, std, siz);
#endif
}

static int
dec_trc_uint(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
#if	defined(I860_NODE)
	/* XXX are handwritten anyhow */
	return dec_xdr_int(mp, vp, cnt, std, siz);
#else
	return dec_xdr_uint(mp, vp, cnt, std, siz);
#endif
}

static int
dec_trc_ulong(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
#if	defined(I860_NODE)
	/* XXX are handwritten anyhow */
	return dec_xdr_long(mp, vp, cnt, std, siz);
#else
	return dec_xdr_ulong(mp, vp, cnt, std, siz);
#endif
}

static int
dec_trc_float(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	return dec_xdr_float(mp, vp, cnt, std, siz);
}

static int
dec_trc_double(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	return dec_xdr_double(mp, vp, cnt, std, siz);
}

static int
dec_trc_cplx(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	return dec_xdr_cplx(mp, vp, cnt, std, siz);
}

static int
dec_trc_dcplx(mp, vp, cnt, std, siz)
	struct pmsg *mp;
	void *vp;
	int cnt, std, siz;
{
	return dec_xdr_dcplx(mp, vp, cnt, std, siz);
}


/*******************************************
 **  Alien (can't do it) en/decoder stub  **
 **                                       **
 *******************************************/

static int
enc_alien()
{
	return PvmBadMsg;
}


/****************************
 **  Encoder switch table  **
 **                        **
 ****************************/

static struct encvec encoders[] = {

	{	enc_raw_init, dec_raw_init,		/* Raw (native) 0 */
		enc_raw_any, dec_raw_any,
		enc_raw_any, dec_raw_any,
		enc_raw_any, dec_raw_any,
		enc_raw_any, dec_raw_any,
		enc_raw_any, dec_raw_any,
		enc_raw_any, dec_raw_any,
		enc_raw_any, dec_raw_any,
		enc_raw_any, dec_raw_any,
		enc_raw_any, dec_raw_any,
		enc_raw_any, dec_raw_any,
		enc_raw_any, dec_raw_any },

	{	enc_xdr_init, dec_xdr_init,		/* XDR 1 */
		enc_xdr_byte, dec_xdr_byte,
		enc_xdr_short, dec_xdr_short,
		enc_xdr_int, dec_xdr_int,
		enc_xdr_long, dec_xdr_long,
#if	defined(I860_NODE)
		enc_xdr_short, dec_xdr_short,	/* XXX are handwritten anyhow */
		enc_xdr_int, dec_xdr_int,
		enc_xdr_long, dec_xdr_long,
#else
		enc_xdr_short, dec_xdr_ushort,
		enc_xdr_int, dec_xdr_uint,
		enc_xdr_ulong, dec_xdr_ulong,
#endif
		enc_xdr_float, dec_xdr_float,
		enc_xdr_double, dec_xdr_double,
		enc_xdr_cplx, dec_xdr_cplx,
		enc_xdr_dcplx, dec_xdr_dcplx },

	{	enc_inp_init, enc_alien,		/* In place (encode only) 2 */
		enc_inp_any, enc_alien,
		enc_inp_any, enc_alien,
		enc_inp_any, enc_alien,
		enc_inp_any, enc_alien,
		enc_inp_any, enc_alien,
		enc_inp_any, enc_alien,
		enc_inp_any, enc_alien,
		enc_inp_any, enc_alien,
		enc_inp_any, enc_alien,
		enc_inp_any, enc_alien,
		enc_inp_any, enc_alien },

	{	enc_trc_init, dec_trc_init,		/* Trace (a la XDR + annot) 4 */
		enc_trc_byte, dec_trc_byte,
		enc_trc_short, dec_trc_short,
		enc_trc_int, dec_trc_int,
		enc_trc_long, dec_trc_long,
		enc_trc_short, dec_trc_ushort,
		enc_trc_int, dec_trc_uint,
		enc_trc_ulong, dec_trc_ulong,
		enc_trc_float, dec_trc_float,
		enc_trc_double, dec_trc_double,
		enc_trc_cplx, dec_trc_cplx,
		enc_trc_dcplx, dec_trc_dcplx },

	{	enc_alien, dec_raw_init,		/* Alien (can't deal) 5 */
		enc_alien, dec_raw_any,			/* unpacking bytes allowed */
		enc_alien, enc_alien,
		enc_alien, enc_alien,
		enc_alien, enc_alien,
		enc_alien, enc_alien,
		enc_alien, enc_alien,
		enc_alien, enc_alien,
		enc_alien, enc_alien,
		enc_alien, enc_alien,
		enc_alien, enc_alien,
		enc_alien, enc_alien },
};


/*	pmsg_setlen()
*
*	Set m_len field of pmsg (after packing because we don't keep track
*	of it).
*/

int
pmsg_setlen(mp)
	struct pmsg *mp;
{
	struct frag *fp, *fp2;
	int l = 0;

	fp = fp2 = mp->m_frag;
	while ((fp = fp->fr_link) != fp2)
		l += fp->fr_len;
	mp->m_len = l;
	return l;
}


/*	pmsg_setenc()
*
*	Set encoding parameter for message, initialize encode/decode vector.
*/

int
pmsg_setenc(mp, enc)
	struct pmsg *mp;
	int enc;
{
	mp->m_enc = enc;
	if (enc == pvmmydsig)		/* native */
		enc = 0;
	else if (enc == 0x10000000)	/* xdr */
		enc = 1;
	else if (enc == 0x20000000)	/* inplace */
		enc = 2;
	else if (enc == 0x40000000) /* trace */
		enc = 3;
	else						/* alien */
		enc = 4;
	mp->m_codef = &encoders[enc];
	return 0;
}


int
pmsg_dump(mp, lvl)
	struct pmsg *mp;
	int lvl;			/* 0 - summary, 1 - frag lengths, 2 - frag data */
{
	struct frag *fp;

	if (mp->m_flag & MM_PACK)
		pmsg_setlen(mp);
#ifdef	MCHECKSUM
	pvmlogprintf(
	"pmsg_dump(0x%x) ref=%d mid=%d len=%d ctx=0x%x tag=%d wid=0x%x src=0x%x dst=0x%x enc=0x%x flag=%d crc=0x%x\n",
			mp, mp->m_ref, mp->m_mid, mp->m_len,
			mp->m_ctx, mp->m_tag, mp->m_wid, mp->m_src, mp->m_dst,
			mp->m_enc, mp->m_flag, pmsg_crc(mp));

#else
	pvmlogprintf(
	"pmsg_dump(0x%x) ref=%d mid=%d len=%d ctx=0x%x tag=%d wid=0x%x src=0x%x dst=0x%x enc=0x%x flag=%d\n",
			mp, mp->m_ref, mp->m_mid, mp->m_len,
			mp->m_ctx, mp->m_tag, mp->m_wid, mp->m_src, mp->m_dst,
			mp->m_enc, mp->m_flag);
#endif
	if (lvl > 0) {
		for (fp = mp->m_frag->fr_link; fp != mp->m_frag; fp = fp->fr_link) {
			pvmlogprintf(" frag=0x%x max=%d ofs=%d len=%d\n",
				fp, fp->fr_max, fp->fr_dat - fp->fr_buf, fp->fr_len);
			if (lvl > 1)
				pvmhdump(fp->fr_dat, fp->fr_len, "  ");
		}
	}
	return 0;
}


/*	pmsg_pack()
*
*	Pack a message into another message, including header fields.
*/

int
pmsg_pack(mp, mp2)
	struct pmsg *mp;		/* message to pack into */
	struct pmsg *mp2;		/* message to include */
{
	struct frag *fp;
	int cc;

	if (mp2->m_flag & MM_PACK)
		pmsg_setlen(mp2);

	if (cc = (mp->m_codef->enc_int) (mp, (void*)&mp2->m_len, 1, 1, sizeof(int)))
		return cc;
	if (cc = (mp->m_codef->enc_int) (mp, (void*)&mp2->m_ctx, 1, 1, sizeof(int)))
		return cc;
	if (cc = (mp->m_codef->enc_int) (mp, (void*)&mp2->m_tag, 1, 1, sizeof(int)))
		return cc;
	if (cc = (mp->m_codef->enc_int) (mp, (void*)&mp2->m_wid, 1, 1, sizeof(int)))
		return cc;
	if (cc = (mp->m_codef->enc_int) (mp, (void*)&mp2->m_enc, 1, 1, sizeof(int)))
		return cc;
	if (cc = (mp->m_codef->enc_int) (mp, (void*)&mp2->m_crc, 1, 1, sizeof(int)))
		return cc;
	if (cc = (mp->m_codef->enc_int) (mp, (void*)&mp2->m_src, 1, 1, sizeof(int)))
		return cc;
	if (cc = (mp->m_codef->enc_int) (mp, (void*)&mp2->m_dst, 1, 1, sizeof(int)))
		return cc;
	if (fp = mp2->m_frag)
		while ((fp = fp->fr_link) != mp2->m_frag) {
			if (cc = (mp->m_codef->enc_int)
					(mp, (void*)&fp->fr_len, 1, 1, sizeof(int)))
				return cc;
			if (cc = (mp->m_codef->enc_byte)
					(mp, (void*)fp->fr_dat, fp->fr_len, 1, 1))
				return cc;
		}
	return cc;
}


/*	pmsg_packbody()
*
*	Pack body of a message into another message.
*	Shares fragments between the messages.
*/

int
pmsg_packbody(mp, mp2)
	struct pmsg *mp;		/* message to pack into */
	struct pmsg *mp2;		/* message to include body of */
{
	struct frag *fp, *fp2;
	int cc;

	if (mp->m_enc != mp2->m_enc)
		cc = PvmMismatch;
	else {
		cc = 0;
		if ((fp = mp2->m_frag) && fp->fr_link != fp) {
			fp2 = mp->m_frag->fr_rlink;
			if (fp2 != mp->m_frag && fp2->fr_len == 0) {
				LISTDELETE(fp2, fr_link, fr_rlink);
				fr_unref(fp2);
			}
			while ((fp = fp->fr_link) != mp2->m_frag) {
				fp2 = fr_new(0);
				fp2->fr_buf = fp->fr_buf;
				fp2->fr_max = fp->fr_max;
				fp2->fr_dat = fp->fr_dat;
				fp2->fr_len = fp->fr_len;
				da_ref(fp->fr_buf);
				LISTPUTBEFORE(mp->m_frag, fp2, fr_link, fr_rlink);
			}
		}
	}
	return cc;
}


/*	pmsg_unpack()
*
*	Unpack a message from a message.
*/

int
pmsg_unpack(mp, mp2)
	struct pmsg *mp;		/* message to unpack from */
	struct pmsg *mp2;		/* blank message to write on */
{
	struct frag *fp;
	int cc;
	int mlen;
	int frl;

	if (cc = (mp->m_codef->dec_int)
			(mp, (void*)&mlen, 1, 1, sizeof(int)))
		return cc;
	if (cc = (mp->m_codef->dec_int)
			(mp, (void*)&mp2->m_ctx, 1, 1, sizeof(int)))
		return cc;
	if (cc = (mp->m_codef->dec_int)
			(mp, (void*)&mp2->m_tag, 1, 1, sizeof(int)))
		return cc;
	if (cc = (mp->m_codef->dec_int)
			(mp, (void*)&mp2->m_wid, 1, 1, sizeof(int)))
		return cc;
	if (cc = (mp->m_codef->dec_int)
			(mp, (void*)&mp2->m_enc, 1, 1, sizeof(int)))
		return cc;
	if (cc = (mp->m_codef->dec_int)
			(mp, (void*)&mp2->m_crc, 1, 1, sizeof(int)))
		return cc;
	if (cc = (mp->m_codef->dec_int)
			(mp, (void*)&mp2->m_src, 1, 1, sizeof(int)))
		return cc;
	if (cc = (mp->m_codef->dec_int)
			(mp, (void*)&mp2->m_dst, 1, 1, sizeof(int)))
		return cc;

	mp2->m_len = 0;

	if (mlen < 0)
		cc = PvmBadMsg;
	else {
		while (mlen > 0) {
			if (cc = (mp->m_codef->dec_int)
					(mp, (void*)&frl, 1, 1, sizeof(int)))
				break;
			if (!(fp = fr_new(frl + MAXHDR))) {
				cc = PvmNoMem;
				break;
			}
			fp->fr_dat += MAXHDR;
			fp->fr_len = frl;
			if (cc = (mp->m_codef->dec_byte)
					(mp, (void*)fp->fr_dat, frl, 1, 1))
				break;
			LISTPUTBEFORE(mp2->m_frag, fp, fr_link, fr_rlink);
			mp2->m_len += frl;
			mlen -= frl;
		}
		pmsg_setenc(mp2, mp2->m_enc);
	}
	return cc;
}


/**************************
 **  Vendor Custom Code  **
 **                      **
 **************************/


#ifdef IMA_CSPP
static void internal_pre_bcopy(unsigned int, unsigned int, int);
void read_prefetch_region(unsigned int, int);

static void
pre_bcopy(s1, s2, n)
	unsigned int s1;
	unsigned int s2;
	int n;
{
	int offset;

	/* make life easy.  only work on chunks of at least 4 lines */
	if (n < 320) {
		read_prefetch_region(s1 & ~0x3f, n + (s1 & 0x3f));
		BCOPY((void *)s1, (void *)s2, n);
		return;
	}
	/* force starting alignment */
	offset = (unsigned int)s1 & 0x3f;
	if (offset) {
		offset = 64 - offset;
		BCOPY((void *)s1, (void *)s2, offset);
		s1 += offset;
		s2 += offset;
		n -= offset;
	}

	/* deal with case where n is not a multiple of 64 */
	offset = n & 0x3f;
	if (offset) {
		n &= ~0x3f;
		internal_pre_bcopy(s1, s2, n);
		s1 += n;
		s2 += n;
		BCOPY((void *)s1, (void *)s2, offset);
		return;
	}

	/* n is a multiple of 64 */
	internal_pre_bcopy(s1, s2, n);
	return;
}

static void
internal_pre_bcopy(s1, s2, n)
	unsigned int s1;
	unsigned int s2;
	int n;
{
	read_prefetch_region(s1, 192);				/* prefetch 0, 1 & 2 */
	while (1) {
		read_prefetch_region(s1+192, 64);		/* prefetch 3 */
		if (n == 256)
			break;
		BCOPY((void *)s1, (void *)s2, 64);						/* copy 0 */
		s1 += 64; s2 += 64; n -= 64;
		read_prefetch_region(s1+192, 64);		/* prefetch 0 */
		if (n == 256)
			break;
		BCOPY((void *)s1, (void *)s2, 64);						/* copy 1 */
		s1 += 64; s2 += 64; n -= 64;
		read_prefetch_region(s1+192, 64);		/* prefetch 1 */
		if (n == 256)
			break;
		BCOPY((void *)s1, (void *)s2, 64);						/* copy 2 */
		s1 += 64; s2 += 64; n -= 64;
		read_prefetch_region(s1+192, 64);		/* prefetch 2 */
		if (n == 256)
			break;
		BCOPY((void *)s1, (void *)s2, 64);						/* copy 3 */
		s1 += 64; s2 += 64; n -= 64;
	}
	BCOPY((void *)s1, (void *)s2, 256); /* last 4 lines */
}
#endif

