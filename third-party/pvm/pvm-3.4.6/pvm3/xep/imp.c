
static char rcsid[] =
	"$Id: imp.c,v 1.4 2000/02/16 20:07:00 pvmsrc Exp $";

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
 *	imp.c
 *
 *	X device-independent image display
 *
 *	Oct 95 Manchek
 */


#ifdef HASSTDLIB
#include <stdlib.h>
#endif
#include <stdio.h>
#include <math.h>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Toggle.h>
#include <X11/Xaw/Form.h>
#include "../src/bfunc.h"
#include "myalloc.h"
#include "imp.h"

#define	MTALLOC(n,t,g)		(t*)malloc((n)*sizeof(t))

#ifndef	min
#define	min(a,b)	((a)<(b)?(a):(b))
#endif
#ifndef	max
#define	max(a,b)	((a)>(b)?(a):(b))
#endif

#ifndef	NCMV
#define	NCMV	128
#endif

#define	byteswide(w) (((w)+7)/8)

#ifdef __ProtoGlarp__
#undef __ProtoGlarp__
#endif
#if defined(__STDC__) || defined(__cplusplus)
#define __ProtoGlarp__(x) x
#else
#define __ProtoGlarp__(x) ()
#endif

static int mkrbow __ProtoGlarp__ (( unsigned int *, unsigned int *,
		unsigned int *, int, int ));

/***************
 **  Globals  **
 **           **
 ***************/

extern Display *xDisp;			/* from xep.c */

static Colormap myCmap = 0;
static Visual *myVisual = 0;
static GC myGC = 0;				/* canvas painting gc */
static unsigned long *cmapColors = 0;
static unsigned int *lutr = 0;	/* lut for non-colormapped displays */
static unsigned int *lutg = 0;
static unsigned int *lutb = 0;

static int ncmv = 0;
static int isMono = 0;
static int nPlanes = 0;			/* display depth */
static int revByte = 0;			/* X server byte order is opposite ours */
static int bimbo = 0;			/* bitmap bit order */
static int xBpp = 0;			/* ximage bits per pixel */
static int xBypp = 0;			/* ximage bytes per pixel */
static int redMask = 0;
static int redShift = 0;
static int greenMask = 0;
static int greenShift = 0;
static int blueMask = 0;
static int blueShift = 0;


/***************
 **  Xt Gorp  **
 **           **
 ***************/

/*
*	Ordered dither for worst case / binary displays
*/

u_char ditclass[8][8] = {
	2,   130, 34,  162, 10,  138, 42,  170,
	194, 66,  226, 98,  202, 74,  234, 106,
	50,  178, 18,  146, 58,  186, 26,  154,
	242, 114, 210, 82,  250, 122, 218, 90,
	14,  142, 46,  174, 6,   134, 38,  166,
	206, 78,  238, 110, 198, 70,  230, 102,
	62,  190, 30,  158, 54,  182, 22,  150,
	254, 126, 222, 94,  246, 118, 214, 86
};


setup_color(cnp, forcemono, falsecolor)
	struct canvas *cnp;
	int forcemono;
	int falsecolor;
{
	int i, j;
	XColor colr;
	unsigned int rbor[NCMV], rbog[NCMV], rbob[NCMV];
	unsigned int mbo = 0x04030201;	/* to test machine byte order */
	int xScrn = DefaultScreen(xDisp);
	XGCValues xgcv;
	XWindowAttributes wat;

	XGetWindowAttributes(xDisp, cnp->cn_win, &wat);

	nPlanes = wat.depth;
	myVisual = wat.visual;
	myCmap = wat.colormap;

	if (cmapColors && ncmv > 0) {
		XFreeColors(xDisp, myCmap, cmapColors, ncmv, (long)0);
		MY_FREE(cmapColors);
	}
	cmapColors = 0;
	ncmv = 0;
	if (lutr)
		MY_FREE(lutr);
	if (lutg)
		MY_FREE(lutg);
	if (lutb)
		MY_FREE(lutb);
	lutr = lutg = lutb = 0;

	if (myGC)
		XFreeGC(xDisp, myGC);
	xgcv.function = GXcopy;
	xgcv.background = BlackPixel(xDisp, xScrn);
	xgcv.foreground = WhitePixel(xDisp, xScrn);
	myGC = XCreateGC(xDisp, cnp->cn_win,
			GCBackground|GCForeground|GCFunction, &xgcv);

	revByte = (ImageByteOrder(xDisp) == MSBFirst) ? 1 : 0;
	if (*(char *)&mbo == 4)
		revByte = !revByte;

	bimbo = BitmapBitOrder(xDisp);

	if (!forcemono && nPlanes > 1) {
		isMono = 0;

		if (myVisual->class == TrueColor || myVisual->class == DirectColor) {
			XPixmapFormatValues *pfv;
			int i;

			if (!(pfv = XListPixmapFormats(xDisp, &i))) {
				fprintf(stderr, "can't get pixmap format list for screen\n");
				exit(1);
			}
			while (--i >= 0)
				if (pfv[i].depth == nPlanes) break;
			if (i < 0) {
				fprintf(stderr, "no pixmap format matches screen depth?\n");
				exit(1);
			}
			xBpp = pfv[i].bits_per_pixel;
			xBypp = xBpp / 8;
			redMask = myVisual->red_mask;
			redShift = ffs(redMask & ~(redMask >> 1)) - 8;
			greenMask = myVisual->green_mask;
			greenShift = ffs(greenMask & ~(greenMask >> 1)) - 8;
			blueMask = myVisual->blue_mask;
			blueShift = ffs(blueMask & ~(blueMask >> 1)) - 8;
#ifdef	DEBUG
			fprintf(stderr,
				"np %d  r 0x%x %d  g 0x%x %d  b 0x%x %d  bpp %d bypp %d\n",
				nPlanes,
				redMask, redShift, greenMask, greenShift, blueMask, blueShift,
				xBpp, xBypp);
#endif
			lutr = TALLOC(256, unsigned int, "lut");
			lutg = TALLOC(256, unsigned int, "lut");
			lutb = TALLOC(256, unsigned int, "lut");
			if (falsecolor)
				mkrbow(lutr, lutg, lutb, 256, 256);
			else
				for (i = 0; i < 256; i++)
					lutr[i] = lutg[i] = lutb[i] = i;
			return 0;

		} else {
			cmapColors = TALLOC(NCMV, unsigned long, "cmap");
			ncmv = NCMV;
			while (ncmv > 3) {
#ifdef	DEBUG
				fprintf(stderr, "setup_color() trying %d cells\n", ncmv);
#endif
				j = 17 - ffs(ncmv);
				if (falsecolor)
					mkrbow(rbor, rbog, rbob, 65536, ncmv);
				for (i = 0; i < ncmv; i++) {
					if (falsecolor) {
						colr.red = rbor[i];
						colr.green = rbog[i];
						colr.blue = rbob[i];
					} else
						colr.red = colr.green = colr.blue = (i * 65536) / ncmv;
					if (!XAllocColor(xDisp, myCmap, &colr)) {
						if (i > 0) {
#ifdef	DEBUG
							fprintf(stderr, "recycling %d color cells\n", i);
#endif
							XFreeColors(xDisp, myCmap, cmapColors, i, (long)0);
						}
						break;
					}
					cmapColors[i] = colr.pixel;
				}
				if (i == ncmv)
					return 0;
/*
				ncmv = i;
*/
				ncmv /= 2;
			}
			fputs("couldn't alloc enough color cells\n", stderr);
			MY_FREE(cmapColors);
			cmapColors = 0;
		}
	}

	isMono = 1;
	fputs("display is binary, it's not gonna look great...\n", stderr);
	return 0;
}


/*	mkrbow()
*
*	Generate a rainbow lookup table.  0 is black, len-1 is white, and entries
*	between those two go through the spectrum from red to violet.
*	Output values are from 0 to mxval-1.
*/

static int
mkrbow(r, g, b, mxval, len)
	unsigned int *r, *g, *b;	/* red, grn, blu lut return */
	int mxval;					/* max value */
	int len;					/* number of entries (length of r, g, b) */
{
	int i, j;
	double d, e;

	for (i = 1; i < len - 1; i++) {
		j = len - 1 - i;
		d = (d = cos((double)((j - len * 0.16) * (3.1415926535 / len)))) < 0.0
			? 0.0 : d;
		b[i] = d * mxval;
		d = (d = cos((double)((j - len * 0.52) * (3.1415926535 / len)))) < 0.0
			? 0.0 : d;
		g[i] = d * mxval;
		d = (d = cos((double)((j - len * .83) * (3.1415926535 / len)))) < 0.0
			? 0.0 : d;
		e = (e = cos((double)(j * (3.1415926535 / len)))) < 0.0
			? 0.0 : e;
		r[i] = d * mxval + e * (mxval / 2);
	}
	r[i] = g[i] = b[i] = mxval - 1;
	r[0] = g[0] = b[0] = 0;
	return 0;
}


/*	bsw()
*
*	Byteswap an integer.
*/

bsw(p, n)
	char *p;
	int n;
{
	char c;
	char *q;

	switch (n) {
	case 4:
		c = p[0];
		p[0] = p[3];
		p[3] = c;
		c = p[1];
		p[1] = p[2];
		p[2] = c;
		break;

	case 8:
		c = p[0];
		p[0] = p[7];
		p[7] = c;
		c = p[1];
		p[1] = p[6];
		p[6] = c;
		c = p[2];
		p[2] = p[5];
		p[5] = c;
		c = p[3];
		p[3] = p[4];
		p[4] = c;
		break;

	case 2:
		c = p[0];
		p[0] = p[1];
		p[1] = c;
		break;

	default:
		q = p + n - 1;
		while (p < q) {
			c = *p;
			*p++ = *q;
			*q-- = c;
		}
		break;
	}
	return 0;
}


/*	repaint_region()
*
*	Replot XImage from its underlying image.
*/

repaint_region(cnp, x1, y1, x2, y2)
	struct canvas *cnp;
	int x1, y1, x2, y2;				/* image coordinates */
{
	char *ximbuf;					/* ximage data */
	u_char *ribuf;					/* src image data */

	if (x1 < cnp->cn_ox)
		x1 = cnp->cn_ox;
	if (y1 < cnp->cn_oy)
		y1 = cnp->cn_oy;
	if (++x2 > cnp->cn_wd)
		x2 = cnp->cn_wd;
	if (++y2 > cnp->cn_ht)
		y2 = cnp->cn_ht;

	ximbuf = cnp->cn_xim->data;
	ribuf = cnp->cn_dat;

	if (isMono) {

	/*
	* binary display - XXX region not done yet
	*/

		int sx, sy;						/* source x, y counter */
		int dx, dy;						/* dest x, y counter */
		int xa, ya;
		unsigned char *sa;
		char *da;
		u_char pxa;
		u_char msks[8];
		int lbs;

		for (lbs = 0; lbs < 8; lbs++)
			if (bimbo == LSBFirst)
				msks[lbs] = 1 << lbs;
			else
				msks[7 - lbs] = 1 << lbs;
		sy = cnp->cn_oy;
		dy = 0;
		ya = 0;
		while (sy < cnp->cn_ht && dy < cnp->cn_ht) {
			sx = cnp->cn_ox;
			dx = 0;
			xa = 0;
			sa = ribuf + sy * cnp->cn_wd + sx;
			da = ximbuf + dy * cnp->cn_xim->bytes_per_line;
			pxa = 0;
			while (sx < cnp->cn_wd && dx < cnp->cn_wd) {
				if (*sa > ditclass[dx & 7][dy & 7])
					pxa |= msks[dx & 7];
				if (!(++dx & 7)) {
					*da++ = pxa;
					pxa = 0;
				}
				if (++xa >= cnp->cn_zoom) {
					xa = 0;
					sx++;
					sa++;
				}
			}
			if (dx & 7)
				*da++ = pxa;
			dy++;
			if (++ya >= cnp->cn_zoom) {
				ya = 0;
				sy++;
			}
		}

	} else if (cmapColors) {

	/*
	* colormapped display
	*/

		int sx, sy;						/* source x, y counter */
		int dx, dy;						/* dest x, y counter */
		int xa, ya;
		unsigned char *sa;
		char *da;

		sy = y1;
		dy = (y1 - cnp->cn_oy) * cnp->cn_zoom;
		ya = 0;
		while (sy < y2 && dy < cnp->cn_ht) {
			sx = x1;
			dx = (x1 - cnp->cn_ox) * cnp->cn_zoom;
			xa = 0;
			sa = ribuf + sy * cnp->cn_wd + sx;
			da = ximbuf + dy * cnp->cn_xim->bytes_per_line + dx;
			while (sx < x2 && dx < cnp->cn_wd) {
				*da = cmapColors[(*sa * ncmv) / 256];
				da++;
				dx++;
				if (++xa >= cnp->cn_zoom) {
					xa = 0;
					sx++;
					sa++;
				}
			}
			dy++;
			if (++ya >= cnp->cn_zoom) {
				ya = 0;
				sy++;
			}
		}

	} else {

	/*
	* non-colormapped display
	*/
		int sx, sy;						/* source x, y counter */
		int dx, dy;						/* dest x, y counter */
		int xa, ya;
		unsigned char *sa;
		char *da;

		if (xBypp == sizeof(unsigned int)) {
			unsigned int pixv;
			unsigned int ps, pixr, pixg, pixb;

			sy = y1;
			dy = (y1 - cnp->cn_oy) * cnp->cn_zoom;
			ya = 0;
			while (sy < y2 && dy < cnp->cn_ht) {
				sx = x1;
				dx = (x1 - cnp->cn_ox) * cnp->cn_zoom;
				xa = 0;
				sa = ribuf + sy * cnp->cn_wd + sx;
				da = ximbuf + dy * cnp->cn_xim->bytes_per_line + dx * xBypp;
				while (sx < x2 && dx < cnp->cn_wd) {
					ps = 0xff & *sa;
					pixr = lutr[ps];
					pixg = lutg[ps];
					pixb = lutb[ps];
					if (redShift >= 0)
						pixv = redMask & (pixr << redShift);
					else
						pixv = redMask & (pixr >> -redShift);
					if (greenShift >= 0)
						pixv |= greenMask & (pixg << greenShift);
					else
						pixv |= greenMask & (pixg >> -greenShift);
					if (blueShift >= 0)
						pixv |= blueMask & (pixb << blueShift);
					else
						pixv |= blueMask & (pixb >> -blueShift);
					if (revByte)
						bsw((char *)&pixv, (int) sizeof(pixv));
					*((unsigned int *)da) = pixv;
		
					da += xBypp;
					dx++;
					if (++xa >= cnp->cn_zoom) {
						xa = 0;
						sx++;
						sa++;
					}
				}
				dy++;
				if (++ya >= cnp->cn_zoom) {
					ya = 0;
					sy++;
				}
			}

		} else if (xBypp == sizeof(unsigned short)) {
			unsigned short pixv;
			unsigned int ps, pixr, pixg, pixb;

			sy = y1;
			dy = (y1 - cnp->cn_oy) * cnp->cn_zoom;
			ya = 0;
			while (sy < y2 && dy < cnp->cn_ht) {
				sx = x1;
				dx = (x1 - cnp->cn_ox) * cnp->cn_zoom;
				xa = 0;
				sa = ribuf + sy * cnp->cn_wd + sx;
				da = ximbuf + dy * cnp->cn_xim->bytes_per_line + dx * xBypp;
				while (sx < x2 && dx < cnp->cn_wd) {
					ps = 0xff & *sa;
					pixr = lutr[ps];
					pixg = lutg[ps];
					pixb = lutb[ps];
					if (redShift >= 0)
						pixv = redMask & (pixr << redShift);
					else
						pixv = redMask & (pixr >> -redShift);
					if (greenShift >= 0)
						pixv |= greenMask & (pixg << greenShift);
					else
						pixv |= greenMask & (pixg >> -greenShift);
					if (blueShift >= 0)
						pixv |= blueMask & (pixb << blueShift);
					else
						pixv |= blueMask & (pixb >> -blueShift);
					if (revByte)
						bsw((char *)&pixv, (int) sizeof(pixv));
					*((unsigned short *)da) = pixv;
		
					da += xBypp;
					dx++;
					if (++xa >= cnp->cn_zoom) {
						xa = 0;
						sx++;
						sa++;
					}
				}
				dy++;
				if (++ya >= cnp->cn_zoom) {
					ya = 0;
					sy++;
				}
			}

		} else if (xBypp == sizeof(unsigned char)) {
			unsigned char pixv;
			unsigned int ps, pixr, pixg, pixb;

			sy = y1;
			dy = (y1 - cnp->cn_oy) * cnp->cn_zoom;
			ya = 0;
			while (sy < y2 && dy < cnp->cn_ht) {
				sx = x1;
				dx = (x1 - cnp->cn_ox) * cnp->cn_zoom;
				xa = 0;
				sa = ribuf + sy * cnp->cn_wd + sx;
				da = ximbuf + dy * cnp->cn_xim->bytes_per_line + dx * xBypp;
				while (sx < x2 && dx < cnp->cn_wd) {
					ps = 0xff & *sa;
					pixr = lutr[ps];
					pixg = lutg[ps];
					pixb = lutb[ps];
					if (redShift >= 0)
						pixv = redMask & (pixr << redShift);
					else
						pixv = redMask & (pixr >> -redShift);
					if (greenShift >= 0)
						pixv |= greenMask & (pixg << greenShift);
					else
						pixv |= greenMask & (pixg >> -greenShift);
					if (blueShift >= 0)
						pixv |= blueMask & (pixb << blueShift);
					else
						pixv |= blueMask & (pixb >> -blueShift);
					*((unsigned char *)da) = pixv;
		
					da += xBypp;
					dx++;
					if (++xa >= cnp->cn_zoom) {
						xa = 0;
						sx++;
						sa++;
					}
				}
				dy++;
				if (++ya >= cnp->cn_zoom) {
					ya = 0;
					sy++;
				}
			}

		} else {
			fprintf(stderr, "aaugh, i can't do %d byte true color pixels\n",
					xBypp);
			exit(1);
		}
	}
	return 0;
}


/*	cre_xim()
*
*	Create (or change) XImage for a canvas.
*/

cre_xim(cnp)
	struct canvas *cnp;
{
	char *ximbuf;
	int wd = cnp->cn_wd;
	int ht = cnp->cn_ht;

	if (cnp->cn_xim)
		XDestroyImage(cnp->cn_xim);

	if (isMono) {
		ximbuf = MTALLOC(byteswide(wd) * ht, char, "imb");
		cnp->cn_xim = XCreateImage(xDisp, myVisual, 1, XYBitmap, 0,
			ximbuf, wd, ht, 8, byteswide(wd));

	} else if (cmapColors) {
		ximbuf = MTALLOC(wd * ht, char, "imb");
		cnp->cn_xim = XCreateImage(xDisp, myVisual, nPlanes, ZPixmap, 0,
			ximbuf, wd, ht, 8, wd);

	} else {
		ximbuf = MTALLOC(wd * ht * xBypp, char, "imb");
		cnp->cn_xim = XCreateImage(xDisp, myVisual, nPlanes, ZPixmap, 0,
			ximbuf, wd, ht, xBpp, wd * xBypp);
	}
	return 0;
}


/*	refresh_region()
*
*	Refresh a canvas window.
*/

refresh_region(cnp, x1, y1, x2, y2)
	struct canvas *cnp;
	int x1, y1, x2, y2;
{
	XPutImage(xDisp, cnp->cn_win, myGC, cnp->cn_xim,
			x1, y1, x1, y1, x2 - x1 + 1, y2 - y1 + 1);
	return 0;
}


