
static char rcsid[] =
	"$Id: imalloc.c,v 1.5 1998/03/24 20:16:17 pvmsrc Exp $";

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
 *	imalloc.c
 *
 *	Instrumented malloc filter.
 *
 *	7 Jun 1991  Robert Manchek  manchek@CS.UTK.EDU.
 *	6 Sep 1991  added static glob and check features.
 *	17 Dec 1991 added i_realloc, cleaned up.
 *	8 Sep 1992  added total byte count.
 *	30 Oct 1992 added object type tag
 *
 *	To use, add something like the following to your code:
 *		#define malloc(n) i_malloc(n)
 *		#define realloc(p,n) i_realloc(p,n)
 *		#define free(p) i_free(p)
 *	recompile, and link.
 *
 *	Facilities:
 *
 *	$ All errors written to fd 2 (not using stdio).
 *
 *	$ Configurable to die on error for debugger.
 *		Define DIEONERROR as 0 or 1.
 *
 *	$ Configurable to use statically allocated space for recordkeeping.
 *		This makes imalloc more immune to bad heap space trashing.
 *		Define STATICGLOBS as nonzero.
 *
 *	$ I_malloc():
 *		Error on [adjustable] unreasonble length or malloc() failed.
 *		Hashes descriptors of all chunks for i_free() checking.
 *		Numbers requests to indicate order received.
 *		Writes [adjustable] pseudo-random head and tail pads to
 *			aid in checking overrun of chunk.
 *
 *	$ I_free(ptr):
 *		Error if chunk not i_malloc()d.
 *		Checks head and tail pads.
 *		[Optionally] zeros chunk to aid in detecting late use.
 *		Define ZEROONFREE as 0 or 1.
 *
 *	$ I_dump(how):
 *		Callable from user program.
 *		Dumps the hash table to see what chunks are active.
 *		If how is 1, checks each chunk's head and tail pads.
 *
 */

#ifdef	HASSTDLIB
#include <stdlib.h>
#endif
#include <stdio.h>
#ifdef	SYSVSTR
#include <string.h>
#else
#include <strings.h>
#endif
/*
#include <malloc.h>  XXX fails on next
*/
#include "bfunc.h"

#ifndef	DIEONERROR
#define	DIEONERROR	1
#endif
#ifndef	ZEROONFREE
#define	ZEROONFREE	1
#endif
#ifndef	STATICGLOBS
#define	STATICGLOBS	0
#endif
#ifndef	DUMPHDR
#define	DUMPHDR	"i_dump()"
#endif
#ifndef	LET0BE1
#define	LET0BE1	1
#endif

#define	NEXTRN(x)	(x = (x) + (x) + (((x ^ (x >> 3)) & 0x2000) ? 1 : 0))
#define	HASH(p)		(((long)(p) ^ ((long)(p) >> 8) ^ ((long)(p) >> 16) ^ ((long)(p) >> 24)) & 0xff)

/* write a null-term string */

#if 0
#define SWRITE(fd,s) write((fd),(s),strlen(s))
#endif
#define SWRITE(fd,s) pvmlogerror(s)

/* this describes a chunk of memory */

struct glob {
	struct glob *next;	/* next glob in hash bucket */
	char *base;			/* baseaddr of user chunk */
	int len;			/* len of user chunk */
	int id;				/* chunk id */
	int lop;			/* nbytes head padding */
	int hip;			/* nbytes tail padding */
	int rst;			/* starting random state */
	int flg;
	char tag[4];		/* content tag */
};

#define	OBALLOC		1	/* ob was just alloced - cleared by dump() */
#define	OBREALLOC	2	/* ob was just realloced - cleared by dump() */


/* default values */

#if 0
static int debfd = 2;				/* debug file descriptor */
#endif
static int lengthlimit = 1048576;	/* max length malloc allowed */
static int lopad = 16;				/* chunk head pad */
static int hipad = 16;				/* chunk tail pad */

/* globals */

static int totlnbyts = 0;			/* total bytes allocated */
static int rnstate = 1;				/* random sequence gen. */
static struct glob *hashtbl[256];	/* chunk hash table */
static char msbuf[256];				/* error message buffer */
static int firsttime = 1;
static int globid = 0;				/* chunk id counter */
#if	STATICGLOBS > 0
static struct glob globheap[STATICGLOBS];
static struct glob *globfl = 0;
static int globavail = STATICGLOBS;
#endif


/*	i_choke()
*
*	Found an inconsistency; bail.
*/

void
i_choke()
{
#if	DIEONERROR
	abort();
#endif
}


/*	i_malloc()
*
*	Allocate a buffer of given length.
*/

char*
i_malloc(len, tag)
	unsigned len;	/* number of bytes */
	char *tag;		/* content description */
{
	char *ptr;
	struct glob *ob;	/* hash tbl entry */
	struct glob **he;
	int i;				/* gp */

	if (firsttime) {
		firsttime = 0;
		BZERO((char*)hashtbl, sizeof(hashtbl));

#if	STATICGLOBS > 0
	/* initialize the glob freelist */
		ob = 0;
		for (i = STATICGLOBS-1; i >= 0; i--) {
			globheap[i].next = ob;
			ob = &globheap[i];
		}
		globfl = ob;
#endif
	}

	/* check req length */

#if LET0BE1 > 0
	if (!len)
		len = 1;
#endif
	if (len < 1 || len > lengthlimit) {
		(void)sprintf(msbuf, "i_malloc: bogus len=%d\n", len);
		(void)SWRITE(debfd, msbuf);
		i_choke();
		return (char*)0;
	}

	/* do actual malloc */

	if (!(ptr = (char*)malloc(len + lopad + hipad))) {
		(void)sprintf(msbuf, "i_malloc: malloc failed len=%d\n", len);
		(void)SWRITE(debfd, msbuf);
		i_choke();
		return (char*)0;
	}

	/* get descriptor */

#if STATICGLOBS > 0
	if (ob = globfl) {
		globfl = globfl->next;
		globavail--;

	} else {
		(void)sprintf(msbuf, "i_malloc: glob allocate failed (max %d)\n",
			STATICGLOBS);
		(void)SWRITE(debfd, msbuf);
		i_choke();
		return (char*)0;
	}
#else
	if (!(ob = (struct glob*)malloc(sizeof(struct glob)))) {
		(void)sprintf(msbuf, "i_malloc: malloc failed for glob\n");
		(void)SWRITE(debfd, msbuf);
		i_choke();
		return (char*)0;
	}
#endif

	/* enter descriptor, write head and tail pads */

	ob->flg = OBALLOC;
	ob->id = ++globid;
	ob->tag[0] = 0;
	if (tag)
		strncpy(ob->tag, tag, 4);
	ob->len = len;
	ob->lop = lopad;
	ob->hip = hipad;
	ob->rst = rnstate;
	for (i = lopad; i-- > 0; *ptr++ = NEXTRN(rnstate));
	ob->base = ptr;
	he = &hashtbl[HASH(ptr)];
	for (i = hipad, ptr += len; i-- > 0; *ptr++ = NEXTRN(rnstate));
	ob->next = *he;
	*he = ob;
	totlnbyts += len;
	return ob->base;
}


/*	i_free()
*
*	Give up a previously allocated buffer.
*/

i_free(loc)
	char *loc;		/* ptr to buffer */
{
	struct glob *ob;	/* freeing this object */
	struct glob *preob;	/* object before in chain */
	int rs;				/* reproduced random seqn */
	char *ptr = loc;	/* gp */
	struct glob **he;	/* hash tbl entry */
	int i;				/* gp */

	/* sanity check */

	if (firsttime) {
		char *s = "i_free: called before i_malloc?\n";
		SWRITE(debfd, s);
		i_choke();
		return 0;
	}

	/* delete from hash tbl */

	he = &hashtbl[HASH(loc)];
	for (preob = 0, ob = *he; ob && ob->base != loc; preob = ob, ob = ob->next);
	if (!ob) {
		(void)sprintf(msbuf, "i_free: bogus loc=0x%lx\n", (long) loc);
		(void)SWRITE(debfd, msbuf);
		i_choke();
		return 0;
	}
	rs = ob->rst;

	/* check head and tail pads */

	for (i = ob->lop, ptr -= i; i > 0; i--)
		if ((0xff & (int)(*ptr++)) != (0xff & NEXTRN(rs))) {
			(void)sprintf(msbuf, "i_free: scribbled in 0x%lx[%d]\n",
				(long) loc, -i);
			(void)SWRITE(debfd, msbuf);
			i_choke();
		}
	for (i = ob->hip, ptr += ob->len; i > 0; i--)
		if ((0xff & (int)(*ptr++)) != (0xff & NEXTRN(rs))) {
			(void)sprintf(msbuf, "i_free: scribbled in 0x%lx[%d+%d]\n",
				(long) loc, ob->len, ob->hip - i);
			(void)SWRITE(debfd, msbuf);
			i_choke();
		}

	/* do actual free */

#if	ZEROONFREE
	BZERO(loc - ob->lop, ob->len + ob->lop + ob->hip);
#endif
	free(loc - ob->lop);
	totlnbyts -= ob->len;

	/* reclaim descriptor */

	if (preob)
		preob->next = ob->next;
	else
		*he = ob->next;
#if STATICGLOBS > 0
	ob->next = globfl;
	globfl = ob;
	globavail++;
#else
	free((char*)ob);
#endif
	return 0;
}


/*	i_realloc()
*
*	Resize a previously allocated buffer (possibly relocate as well)
*	and leave the contents unchanged up to the minimum of the old
*	and new lengths.
*/

char*
i_realloc(loc, len)
	char *loc;			/* old buffer */
	unsigned len;		/* length of new buffer */
{
	struct glob *ob;	/* freeing this object */
	struct glob *preob;	/* object before in chain */
	int rs;				/* reproduced random seqn */
	char *ptr = loc;	/* gp */
	struct glob **he;	/* hash tbl entry */
	int i;				/* gp */

	/* sanity check */

	if (firsttime) {
		char *s = "i_realloc: called before i_malloc?\n";
		SWRITE(debfd, s);
		i_choke();
		return (char*)0;
	}

	/* check req length */

#if LET0BE1 > 0
	if (!len)
		len = 1;
#endif
	if (len < 1 || len > lengthlimit) {
		(void)sprintf(msbuf, "i_realloc: bogus len=%d\n", len);
		(void)SWRITE(debfd, msbuf);
		i_choke();
		return (char*)0;
	}

	/* delete from hash tbl */

	he = &hashtbl[HASH(loc)];
	for (preob = 0, ob = *he; ob && ob->base != loc; preob = ob, ob = ob->next);
	if (!ob) {
		(void)sprintf(msbuf, "i_realloc: bogus loc=0x%lx\n",
			(long) loc);
		(void)SWRITE(debfd, msbuf);
		i_choke();
		return (char*)0;
	}
	rs = ob->rst;

	/* check head and tail pads */

	for (i = ob->lop, ptr -= i; i > 0; i--)
		if ((0xff & (int)(*ptr++)) != (0xff & NEXTRN(rs))) {
			(void)sprintf(msbuf, "i_realloc: scribbled in 0x%lx[%d]\n",
				(long) loc, -i);
			(void)SWRITE(debfd, msbuf);
			i_choke();
		}
	for (i = ob->hip, ptr += ob->len; i > 0; i--)
		if ((0xff & (int)(*ptr++)) != (0xff & NEXTRN(rs))) {
			(void)sprintf(msbuf,
				"i_realloc: scribbled in 0x%lx[%d+%d]\n",
				(long) loc, ob->len, ob->hip - i);
			(void)SWRITE(debfd, msbuf);
			i_choke();
		}

	/* remove descriptor */

	if (preob)
		preob->next = ob->next;
	else
		*he = ob->next;

	/* realloc */

	if (!(ptr = (char*)realloc(loc - ob->lop, len + lopad + hipad))) {
		(void)sprintf(msbuf, "i_realloc: malloc failed len=%d\n", len);
		(void)SWRITE(debfd, msbuf);
		i_choke();
		return (char*)0;
	}

	/* rewrite descriptor, write head and tail pads */

	totlnbyts += len - ob->len;
	ob->flg = OBREALLOC;
	ob->id = ++globid;	/* XXX ? */
	ob->len = len;
	ob->lop = lopad;
	ob->hip = hipad;
	ob->rst = rnstate;
	for (i = lopad; i-- > 0; *ptr++ = NEXTRN(rnstate));
	ob->base = ptr;
	he = &hashtbl[HASH(ptr)];
	for (i = hipad, ptr += len; i-- > 0; *ptr++ = NEXTRN(rnstate));
	ob->next = *he;
	*he = ob;
	return ob->base;
}


/*	ascdump()
*
*	Convert byte string to printable characters.
*/

static void
ascdump(o, p, e)
	char **o;		/* addr of pointer to output space (passed back) */
	char *p;		/* string to convert */
	int e;			/* length of string */
{
	char *r = *o;
	char c;

	while (e-- > 0) {
		c = 0x7f & *p;
		if (c < ' ' || c == 0x7f) {
			c = (c + '@') & 0x7f;
			*r++ = '^';
		} else
			*r++ = ' ';
		*r++ = c;
		p++;
	}
	*r++ = '\n';
	*r = 0;
	*o = r;
}


/*	i_dump()
*
*	Dump the table of contents of allocated buffers.
*/

void
i_dump(how)
	int how;			/* != 0 also do sanity checking */
{
	int ht;				/* hash table index */
	struct glob *ob;
	char *r;
	int i;
	int rs;				/* reproduced random seqn */
	char *ptr;			/* gp */
	int err;
	char tagstr[5];

#if	STATICGLOBS > 0
	sprintf(msbuf, "%s %d bytes total/%d globs free\n",
			DUMPHDR, totlnbyts, globavail);
	(void)SWRITE(debfd, msbuf);
#else
	sprintf(msbuf, "%s %d bytes total\n", DUMPHDR, totlnbyts);
	(void)SWRITE(debfd, msbuf);
#endif
	for (ht = 0; ht < 256; ht++) {
		for (ob = hashtbl[ht]; ob; ob = ob->next) {
			rs = ob->rst;
			ptr = ob->base;
			err = 0;
			if (how) {
				for (i = ob->lop, ptr -= i; i > 0; i--)
					if ((0xff & (int)(*ptr++)) != (0xff & NEXTRN(rs))) {
						(void)sprintf(msbuf,
							"%5d 0x%08lx[%d]: scribbled in [%d]\n",
							ob->id, (long) ob->base, ob->len, -i);
						(void)SWRITE(debfd, msbuf);
						err++;
					}
				for (i = ob->hip, ptr += ob->len; i > 0; i--)
					if ((0xff & (int)(*ptr++)) != (0xff & NEXTRN(rs))) {
						(void)sprintf(msbuf,
							"%5d 0x%08lx[%d]: scribbled in [%d+%d]\n",
							ob->id, (long) ob->base, ob->len, ob->len,
							ob->hip - i);
						(void)SWRITE(debfd, msbuf);
						err++;
					}
			}
			if (!err) {
				strncpy(tagstr, ob->tag, 4);
				tagstr[4] = 0;
				sprintf(msbuf, "%5d%c%4s 0x%08lx[%4d]",
					ob->id,
					(ob->flg & OBALLOC ? '*' :
						(ob->flg & OBREALLOC ? '+' : ' ')),
					tagstr,
					(long) ob->base, ob->len);
				r = msbuf + strlen(msbuf);
				*r++ = ' ';
				if ((i = ob->len) > 24)
					i = 24;
				ascdump(&r, ob->base, i);
				(void)SWRITE(debfd, msbuf);
			}
			ob->flg &= ~(OBALLOC|OBREALLOC);
		}
	}
}


