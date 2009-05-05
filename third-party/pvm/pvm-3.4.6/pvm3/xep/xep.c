
static char rcsid[] =
	"$Id: xep.c,v 1.5 1999/03/16 16:01:46 pvmsrc Exp $";

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
 *	xep.c
 *
 *	Display pixmap calculated by tiled workers in an X window.
 *
 *	Nov 92 Manchek
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
#include <pvm3.h>
#include "../src/bfunc.h"
#include "myalloc.h"
#include "hostc.h"
#include "imp.h"
#include "bars.xbm"
#include "color.xbm"
#include "reset.xbm"
#include "quit.xbm"
#include "into.xbm"
#include "outof.xbm"


#ifndef	min
#define	min(a,b)	((a)<(b)?(a):(b))
#endif
#ifndef	max
#define	max(a,b)	((a)>(b)?(a):(b))
#endif


static void pick();
static void zoom();
static void redraw();
static void configure();
static void pvm_cb();
static void zin_cb();
static void zout_cb();
static void reset_cb();


/***************
 **  Globals  **
 **           **
 ***************/


Display *xDisp;
XtAppContext context;
Widget topLevel = 0;		/* main widget */
Widget thelabel1 = 0;
Widget thelabel2 = 0;
int xScrn;
Window xRootW;
GC rubGc;				/* rubberbox gc */
Cursor crossCr;
struct canvas imCan;	/* just one image for now */

char *workerfile = 0;
int nworkers = 0;			/* number of active+idle work servers */
int dobars = 0;				/* label tiles with worker number */


/***************
 **  Xt Gorp  **
 **           **
 ***************/

char canvasTl[] =
"*canvas.translations:\
<Btn1Down>:pick(start)\\n\
<Btn1Motion>:pick(adjust)\\n\
<Btn1Up>:pick(end)\\n\
<Btn2Down>:zoom()\\n\
<Btn3Down>:pick(modify)\\n\
<Btn3Motion>:pick(adjust)\\n\
<Btn3Up>:pick(end)\\n\
<Expose>:redraw()\\n\
<ConfigureNotify>:configure()\\n\
";


/* Widget default values */

static char *fallbacks[] = {
	"*allowShellResize:true",
	"*quitButton.label:Quit",
	"*recalcButton.label:Redo",
	"*workersButton.label:NewWorkers",
	canvasTl,
	0
};

/* To get custom resources */

typedef struct {
	Bool mono;			/* force monochrome display */
	Bool fc;			/* use false-color graymap */
	String worker;		/* worker a.out name */
	Bool bars;			/* display processor id bars */
	int n;				/* number of workers */
} app_res_t, *app_resp_t;

static app_res_t app_res;

static XtResource res_list[] = {
	{ "worker", "Worker", XtRString, sizeof(String),
		XtOffset(app_resp_t, worker), XtRString, "mtile" },
	{ "monochrome", "Monochrome", XtRBool, sizeof(Bool),
		XtOffset(app_resp_t, mono), XtRString, "off" },
	{ "falsecolor", "Falsecolor", XtRBool, sizeof(Bool),
		XtOffset(app_resp_t, fc), XtRString, "on" },
	{ "bars", "Bars", XtRBool, sizeof(Bool),
		XtOffset(app_resp_t, bars), XtRString, "off" },
	{ "nWorkers", "NWorkers", XtRInt, sizeof(int),
		XtOffset(app_resp_t, n), XtRString, "-1" },
};

static XrmOptionDescRec knownargs[] = {
	{ "-mono", ".monochrome", XrmoptionNoArg, "on" },
	{ "+mono", ".monochrome", XrmoptionNoArg, "off" },
	{ "-fc", ".falsecolor", XrmoptionNoArg, "on" },
	{ "+fc", ".falsecolor", XrmoptionNoArg, "off" },
	{ "-worker", ".worker", XrmoptionSepArg, 0 },
	{ "-bars", ".bars", XrmoptionNoArg, "on" },
	{ "+bars", ".bars", XrmoptionNoArg, "off" },
	{ "-n", ".nWorkers", XrmoptionSepArg, 0 },
};

static XtCallbackRec callback[2] = { { 0, 0 }, { 0, 0 } };
static Arg args[16];

static XtActionsRec actbl[] = {
	{ "pick", pick },
	{ "zoom", zoom },
	{ "redraw", redraw },
	{ "configure", configure },
};


main(argc, argv)
	int argc;
	char **argv;
{
	int n;
	XGCValues xgcv;
	int *fds;

	srandom(getpid());
	n = 0;
	topLevel = XtAppInitialize(&context, "xep",
		knownargs, XtNumber(knownargs),
		&argc, argv,
		fallbacks,
		args, n);

	if (argc > 1) {
		for (n = 1; n < argc; n++)
			fprintf(stderr, "unknown option <%s>\n", argv[n]);
		fputs("options:\n", stderr);
		fputs("  -worker filename  set calculation task name\n", stderr);
		fputs("  -/+mono           force mono mode\n", stderr);
		fputs("  -/+fc             use false-color graymap\n", stderr);
		fputs("  -/+bars           display/don't display processor bars\n", stderr);
/*
		fputs("  -n count          set number of workers\n", stderr);
*/
		exit(1);
	}

	XtGetApplicationResources(topLevel, (caddr_t)&app_res,
		res_list, XtNumber(res_list), 0, 0);

	workerfile = app_res.worker;
	dobars = app_res.bars;

	pvminit();

	XtAppAddActions(context, actbl, XtNumber(actbl));

	xDisp = XtDisplay(topLevel);
	xScrn = DefaultScreen(xDisp);
	xRootW = RootWindow(xDisp, xScrn);

	crossCr = XCreateFontCursor(xDisp, XC_tcross);

	xgcv.function = GXxor;
	xgcv.background = BlackPixel(xDisp, xScrn);
/*
	xgcv.foreground = WhitePixel(xDisp, xScrn) ^ BlackPixel(xDisp, xScrn);
*/
	xgcv.foreground = ~0;
	rubGc = XCreateGC(xDisp, xRootW,
			GCBackground|GCForeground|GCFunction, &xgcv);

	create_xep_widget();

	more_workers();
	do_recalc();

	n = pvm_getfds(&fds);
#if 0
	while (n-- > 0) {
#ifdef	DEBUG
		fprintf(stderr, "adding fd %d as input\n", fds[n]);
#endif
		XtAppAddInput(context, fds[n], (XtPointer)XtInputReadMask,
				pvm_cb, (XtPointer)0);
	}
#endif
	addaninputfile(fds[0], (XtInputId *)0);

	XtAppMainLoop(context);
}


int
addaninputfile(fd, xiip)
	int fd;
	XtInputId *xiip;
{
	XtInputId xii;

	xii = XtAppAddInput(context, fd, (XtPointer)XtInputReadMask,
			pvm_cb, (XtPointer)0);
	fprintf(stderr, "addaninputfile() fd %d xii %ld\n", fd, xii);
	if (xiip)
		*xiip = xii;
	return 0;
}


int
removeaninputfile(xii)
	XtInputId xii;
{
	XtRemoveInput(xii);
	return 0;
}


static void
pvm_cb(cli, src, id)
	XtPointer cli;
	int *src;
	XtInputId *id;
{
	while (pvm_nrecv(-1, -1) > 0)
		claim_message();
}


static void
quit_cb(wgt, cli, cd)
	Widget wgt;
	caddr_t cli;
	caddr_t cd;
{
	stop_workers();
	pvm_exit();
	exit(0);
}


static void
reset_cb(wgt, cli, cd)
	Widget wgt;
	caddr_t cli;
	caddr_t cd;
{
	if (imCan.cn_wd < imCan.cn_ht) {
		imCan.cn_re1 = -2.0;
		imCan.cn_im1 = (-2.0 * imCan.cn_ht) / imCan.cn_wd;
		imCan.cn_re2 = 2.0;
		imCan.cn_im2 = (2.0 * imCan.cn_ht) / imCan.cn_wd;

	} else {
		imCan.cn_re1 = (-2.0 * imCan.cn_wd) / imCan.cn_ht;
		imCan.cn_im1 = -2.0;
		imCan.cn_re2 = (2.0 * imCan.cn_wd) / imCan.cn_ht;
		imCan.cn_im2 = 2.0;
	}
	splat_out(imCan.cn_dat, imCan.cn_wd, imCan.cn_ht, 1);
	repaint_region(&imCan, 0, 0, imCan.cn_wd - 1, imCan.cn_ht - 1);
	refresh_region(&imCan, 0, 0, imCan.cn_wd - 1, imCan.cn_ht - 1);
	imCan.cn_x1 = 0;
	imCan.cn_y1 = 0;
	imCan.cn_x2 = 0;
	imCan.cn_y2 = 0;
	do_recalc();
}


static void
zout_cb(wgt, cli, cd)
	Widget wgt;
	caddr_t cli;
	caddr_t cd;
{
	double d;

	d = (imCan.cn_re2 - imCan.cn_re1) / 2;
	imCan.cn_re1 -= d;
	imCan.cn_re2 += d;
	d = (imCan.cn_im2 - imCan.cn_im1) / 2;
	imCan.cn_im1 -= d;
	imCan.cn_im2 += d;

	splat_out(imCan.cn_dat, imCan.cn_wd, imCan.cn_ht, 1);
	repaint_region(&imCan, 0, 0, imCan.cn_wd - 1, imCan.cn_ht - 1);
	refresh_region(&imCan, 0, 0, imCan.cn_wd - 1, imCan.cn_ht - 1);
	imCan.cn_x1 = 0;
	imCan.cn_y1 = 0;
	imCan.cn_x2 = 0;
	imCan.cn_y2 = 0;

	do_recalc();
}


static void
zin_cb(wgt, cli, cd)
	Widget wgt;
	caddr_t cli;
	caddr_t cd;
{
	double re1, im1, re2, im2;
	int wd = imCan.cn_wd;
	int ht = imCan.cn_ht;
	int x1 = imCan.cn_x1;
	int y1 = imCan.cn_y1;
	int x2 = imCan.cn_x2;
	int y2 = imCan.cn_y2;
	int t;

	if (x1 != x2 && y1 != y2) {

		if (x1 > x2) {
			t = x1;
			x1 = x2;
			x2 = t;
		}
		if (y1 > y2) {
			t = y1;
			y1 = y2;
			y2 = t;
		}
		re1 = imCan.cn_re1 + x1 * (imCan.cn_re2 - imCan.cn_re1) / wd;
		im1 = imCan.cn_im1 + y1 * (imCan.cn_im2 - imCan.cn_im1) / ht;
		re2 = re1 + (x2 - x1) * (imCan.cn_re2 - imCan.cn_re1) / wd;
		im2 = im1 + (x2 - x1) * (imCan.cn_im2 - imCan.cn_im1) / wd;
		imCan.cn_re1 = re1;
		imCan.cn_im1 = im1;
		imCan.cn_re2 = re2;
		imCan.cn_im2 = im2;

		splat_out(imCan.cn_dat, imCan.cn_wd, imCan.cn_ht, 1);
		repaint_region(&imCan, 0, 0, imCan.cn_wd - 1, imCan.cn_ht - 1);
		refresh_region(&imCan, 0, 0, imCan.cn_wd - 1, imCan.cn_ht - 1);
		imCan.cn_x1 = 0;
		imCan.cn_y1 = 0;
		imCan.cn_x2 = 0;
		imCan.cn_y2 = 0;

		do_recalc();
	}
}


static void
bars_cb(wgt, cli, cd)
	Widget wgt;
	caddr_t cli;
	caddr_t cd;
{
	dobars = !dobars;
}


static void
color_cb(wgt, cli, cd)
	Widget wgt;
	caddr_t cli;
	caddr_t cd;
{
	app_res.fc = !app_res.fc;
	setup_color(&imCan, app_res.mono, app_res.fc);
	repaint_region(&imCan, 0, 0, imCan.cn_wd - 1, imCan.cn_ht - 1);
	refresh_region(&imCan, 0, 0, imCan.cn_wd - 1, imCan.cn_ht - 1);
}


create_xep_widget()
{
	Widget box, box2;
	Widget w;
	int n;
	char buf[128];
	Pixmap pm;

	n = 0;
	box = XtCreateManagedWidget("box", formWidgetClass, topLevel, args, n);

	imCan.cn_re1 = -2.0;
	imCan.cn_im1 = -2.0;
	imCan.cn_re2 = 2.0;
	imCan.cn_im2 = 2.0;

	n = 0;
	XtSetArg(args[n], XtNdefaultDistance, 2); n++;
	XtSetArg(args[n], XtNborderWidth, 0); n++;
	XtSetArg(args[n], XtNleft, XtChainLeft); n++;
	XtSetArg(args[n], XtNright, XtChainLeft); n++;
	XtSetArg(args[n], XtNtop, XtChainTop); n++;
	XtSetArg(args[n], XtNbottom, XtChainTop); n++;
	box2 = XtCreateManagedWidget("box3", formWidgetClass, box, args, n);

	n = 0;
	XtSetArg(args[n], XtNborderWidth, 0); n++;
	w = XtCreateManagedWidget("hosts", labelWidgetClass,
			box2, args, n);

	n = 0;
	XtSetArg(args[n], XtNfromHoriz, w); n++;
	XtSetArg(args[n], XtNborderWidth, 0); n++;
	XtSetArg(args[n], XtNlabel, "0"); n++;
/*
	sprintf(buf, "(%.2e,%.2e) (%.2e,%.2e)", 
			imCan.cn_re1, imCan.cn_im1, imCan.cn_re2, imCan.cn_im2);
	XtSetArg(args[n], XtNlabel, buf); n++;
*/
	thelabel1 = w = XtCreateManagedWidget("thelabel1", labelWidgetClass,
			box2, args, n);

	n = 0;
	XtSetArg(args[n], XtNfromHoriz, w); n++;
	XtSetArg(args[n], XtNborderWidth, 0); n++;
	w = XtCreateManagedWidget("workers", labelWidgetClass,
			box2, args, n);

	n = 0;
	XtSetArg(args[n], XtNfromHoriz, w); n++;
	XtSetArg(args[n], XtNborderWidth, 0); n++;
	XtSetArg(args[n], XtNlabel, "0"); n++;
	thelabel2 = w = XtCreateManagedWidget("thelabel2", labelWidgetClass,
			box2, args, n);

	n = 0;
	XtSetArg(args[n], XtNfromVert, box2); n++;
	XtSetArg(args[n], XtNdefaultDistance, 2); n++;
	XtSetArg(args[n], XtNborderWidth, 0); n++;
	XtSetArg(args[n], XtNleft, XtChainLeft); n++;
	XtSetArg(args[n], XtNright, XtChainLeft); n++;
	XtSetArg(args[n], XtNtop, XtChainTop); n++;
	XtSetArg(args[n], XtNbottom, XtChainTop); n++;
	box2 = XtCreateManagedWidget("box2", formWidgetClass, box, args, n);

	n = 0;
/*
	XtSetArg(args[n], XtNhorizDistance, 0); n++;
	XtSetArg(args[n], XtNvertDistance, 0); n++;
*/
	callback[0].callback = (XtCallbackProc)quit_cb;
	callback[0].closure = (caddr_t)0;
	XtSetArg(args[n], XtNcallback, callback); n++;
	pm = XCreatePixmapFromBitmapData(xDisp, xRootW,
			quit_bits, quit_width, quit_height, 1, 0, 1);
	XtSetArg(args[n], XtNbitmap, (XtArgVal)pm); n++;
	w =  XtCreateManagedWidget("quitButton", commandWidgetClass,
			box2, args, n);

	n = 0;
/*
	XtSetArg(args[n], XtNhorizDistance, 0); n++;
	XtSetArg(args[n], XtNvertDistance, 0); n++;
*/
	callback[0].callback = (XtCallbackProc)reset_cb;
	callback[0].closure = (caddr_t)0;
	XtSetArg(args[n], XtNcallback, callback); n++;
	XtSetArg(args[n], XtNfromHoriz, w); n++;
	pm = XCreatePixmapFromBitmapData(xDisp, xRootW,
			reset_bits, reset_width, reset_height, 1, 0, 1);
	XtSetArg(args[n], XtNbitmap, (XtArgVal)pm); n++;
	w =  XtCreateManagedWidget("resetButton", commandWidgetClass,
			box2, args, n);

	n = 0;
	callback[0].callback = (XtCallbackProc)zin_cb;
	callback[0].closure = (caddr_t)0;
	XtSetArg(args[n], XtNcallback, callback); n++;
	XtSetArg(args[n], XtNfromHoriz, w); n++;
	pm = XCreatePixmapFromBitmapData(xDisp, xRootW,
			into_bits, into_width, into_height, 1, 0, 1);
	XtSetArg(args[n], XtNbitmap, (XtArgVal)pm); n++;
	w =  XtCreateManagedWidget("recalcButton", commandWidgetClass,
			box2, args, n);

	n = 0;
	callback[0].callback = (XtCallbackProc)zout_cb;
	callback[0].closure = (caddr_t)0;
	XtSetArg(args[n], XtNcallback, callback); n++;
	XtSetArg(args[n], XtNfromHoriz, w); n++;
	pm = XCreatePixmapFromBitmapData(xDisp, xRootW,
			outof_bits, outof_width, outof_height, 1, 0, 1);
	XtSetArg(args[n], XtNbitmap, (XtArgVal)pm); n++;
	w =  XtCreateManagedWidget("zoomoutButton", commandWidgetClass,
			box2, args, n);

	n = 0;
	callback[0].callback = (XtCallbackProc)color_cb;
	callback[0].closure = (caddr_t)0;
	XtSetArg(args[n], XtNcallback, callback); n++;
	XtSetArg(args[n], XtNfromHoriz, w); n++;
	XtSetArg(args[n], XtNstate, (app_res.fc ? 1 : 0)); n++;
	pm = XCreatePixmapFromBitmapData(xDisp, xRootW,
			color_bits, color_width, color_height, 1, 0, 1);
	XtSetArg(args[n], XtNbitmap, (XtArgVal)pm); n++;
	w =  XtCreateManagedWidget("colorButton", toggleWidgetClass,
			box2, args, n);

	n = 0;
	callback[0].callback = (XtCallbackProc)bars_cb;
	callback[0].closure = (caddr_t)0;
	XtSetArg(args[n], XtNcallback, callback); n++;
	XtSetArg(args[n], XtNfromHoriz, w); n++;
	XtSetArg(args[n], XtNstate, dobars); n++;
	pm = XCreatePixmapFromBitmapData(xDisp, xRootW,
			bars_bits, bars_width, bars_height, 1, 0, 1);
	XtSetArg(args[n], XtNbitmap, (XtArgVal)pm); n++;
	w =  XtCreateManagedWidget("barsButton", toggleWidgetClass,
			box2, args, n);

	imCan.cn_wd = 200;
	imCan.cn_ht = 200;

	n = 0;
	XtSetArg(args[n], XtNleft, XtChainLeft); n++;
	XtSetArg(args[n], XtNright, XtChainRight); n++;
	XtSetArg(args[n], XtNtop, XtChainTop); n++;
	XtSetArg(args[n], XtNbottom, XtChainBottom); n++;
	XtSetArg(args[n], XtNwidth, imCan.cn_wd); n++;
	XtSetArg(args[n], XtNheight, imCan.cn_ht); n++;
	XtSetArg(args[n], XtNfromVert, box2); n++;
	imCan.cn_wgt = XtCreateManagedWidget("canvas", widgetClass,
		box, args, n);

	XtRealizeWidget(topLevel);

	imCan.cn_win = XtWindow(imCan.cn_wgt);

/*
	testothervisual(TrueColor);
*/

	setup_color(&imCan, app_res.mono, app_res.fc);
	imCan.cn_x1 = 0;
	imCan.cn_y1 = 0;
	imCan.cn_x2 = 0;
	imCan.cn_y2 = 0;
	imCan.cn_zoom = 1;
	imCan.cn_ox = 0;
	imCan.cn_oy = 0;
	imCan.cn_dat = TALLOC(imCan.cn_wd * imCan.cn_ht, u_char, "data");
	splat_out(imCan.cn_dat, imCan.cn_wd, imCan.cn_ht, 1);
	cre_xim(&imCan);
	repaint_region(&imCan, 0, 0, imCan.cn_wd - 1, imCan.cn_ht - 1);

	XDefineCursor(xDisp, imCan.cn_win, crossCr);
	return 0;
}


testothervisual(clas)
	int clas;
{
	XVisualInfo *vinf;
	int nv;
	int i;
	XSetWindowAttributes xswat;

	vinf = XGetVisualInfo(xDisp, VisualNoMask, (XVisualInfo*)0, &nv);
	for (i = 0; i < nv; i++)
		if (vinf[i].class == clas)
			break;
	if (i == nv) {
		fprintf(stderr, "couldn't find the right visual\n");
		exit(1);
	}
	fprintf(stderr,
			"found visual %d: np %d cs %d r 0x%lx g 0x%lx b 0x%lx\n", i,
			vinf[i].depth,
			vinf[i].colormap_size,
			(long) vinf[i].red_mask,
			(long) vinf[i].blue_mask,
			(long) vinf[i].green_mask);
	xswat.colormap = XCreateColormap(xDisp, xRootW, vinf[i].visual, AllocNone);
	xswat.border_pixmap = XCreatePixmap(xDisp, xRootW, 16, 16, 8);
	imCan.cn_win = XCreateWindow(xDisp,
    		xRootW,
    		0,
    		0,
    		300,
    		300,
    		1,
    		8,
    		InputOutput,
    		vinf[i].visual,
    		CWColormap|CWBorderPixmap,
    		&xswat);
	XMapWindow(xDisp, imCan.cn_win);
	return 0;
}


setlabel()
{
	int nh;
	struct hostc *hp;
	char buf[64];

	hp = 0;
	nh = 0;
	while (hp = host_next(hp))
		nh++;

	sprintf(buf, "%d", nh);
	XtSetArg(args[0], XtNlabel, buf);
	XtSetValues(thelabel1, args, 1);
	sprintf(buf, "%d", nworkers);
	XtSetArg(args[0], XtNlabel, buf);
	XtSetValues(thelabel2, args, 1);
	return 0;
}


static void
configure(wgt, xev, par, nump)
	Widget wgt;
	XEvent *xev;
	String *par;
	int *nump;
{
	int wd, ht;				/* size of new canvas */
	double re1, im1, re2, im2;
	int ox, oy;				/* offset of new image in old */
	u_char *dat;			/* new canvas image */
	u_char *b1, *b2;		/* for copying image data */
	int w, h;

	if (xev->type == ConfigureNotify) {
		wd = xev->xconfigure.width;
		ht = xev->xconfigure.height;

		ox = (imCan.cn_wd - wd) / 2;
		re1 = imCan.cn_re1
				+ ((imCan.cn_re2 - imCan.cn_re1) * ox) / imCan.cn_wd;
		re2 = imCan.cn_re1
				+ ((imCan.cn_re2 - imCan.cn_re1) * (ox + wd)) / imCan.cn_wd;
		oy = (imCan.cn_ht - ht) / 2;
		im1 = imCan.cn_im1
				+ ((imCan.cn_re2 - imCan.cn_re1) * oy) / imCan.cn_wd;
		im2 = imCan.cn_im1
				+ ((imCan.cn_re2 - imCan.cn_re1) * (oy + ht)) / imCan.cn_wd;
		imCan.cn_re1 = re1;
		imCan.cn_re2 = re2;
		imCan.cn_im1 = im1;
		imCan.cn_im2 = im2;
/*
		fprintf(stderr, "%dx%d  %e,%e  %e,%e\n", wd, ht, re1, im1, re2, im2);
		fprintf(stderr, "%e : %e\n", (re2 - re1) / wd, (im2 - im1) / ht);
*/

		dat = TALLOC(wd * ht, u_char, "data");
		splat_out(dat, wd, ht, 1);

		w = min(imCan.cn_wd, wd);
		h = min(imCan.cn_ht, ht);
		b1 = imCan.cn_dat + max(0, ox) + max(0, oy) * imCan.cn_wd;
		b2 = dat + max(0, -ox) + max(0, -oy) * wd;
		while (h-- > 0) {
			BCOPY(b1, b2, w);
			b1 += imCan.cn_wd;
			b2 += wd;
		}
		MY_FREE(imCan.cn_dat);

		imCan.cn_dat = dat;
		imCan.cn_wd = wd;
		imCan.cn_ht = ht;
		imCan.cn_x1 = 0;
		imCan.cn_y1 = 0;
		imCan.cn_x2 = 0;
		imCan.cn_y2 = 0;
		cre_xim(&imCan);
		repaint_region(&imCan, 0, 0, imCan.cn_wd - 1, imCan.cn_ht - 1);
		refresh_region(&imCan, 0, 0, imCan.cn_wd - 1, imCan.cn_ht - 1);
		do_recalc();
	}
}


static void
redraw(wgt, xev, par, nump)
	Widget wgt;
	XEvent *xev;
	String *par;
	int *nump;
{
	if (xev->type == Expose) {
		rubbox(imCan.cn_win, imCan.cn_x1, imCan.cn_y1,
				imCan.cn_x2, imCan.cn_y2);
		refresh_region(&imCan, xev->xexpose.x, xev->xexpose.y,
				xev->xexpose.x + xev->xexpose.width - 1,
				xev->xexpose.y + xev->xexpose.height - 1);
		rubbox(imCan.cn_win, imCan.cn_x1, imCan.cn_y1,
				imCan.cn_x2, imCan.cn_y2);
	}
}


static void
zoom(wgt, xev, par, nump)
	Widget wgt;
	XEvent *xev;
	String *par;
	int *nump;
{
	zin_cb(wgt, (char *)0, (char *)0);
}


/*	pick()
*
*	s - start
*	a - adjust
*	e - end
*	m - modify
*/

static void
pick(wgt, xev, par, nump)
	Widget wgt;
	XEvent *xev;
	String *par;
	int *nump;
{
	Window w = XtWindow(wgt);
	int wd = imCan.cn_wd;
	int ht = imCan.cn_ht;
	int x1 = imCan.cn_x1;
	int y1 = imCan.cn_y1;
	int x2 = imCan.cn_x2;
	int y2 = imCan.cn_y2;
	int x, y;

	if (*nump == 1 && get_event_xy(xev, &x, &y)) {
		switch (par[0][0]) {

		case 's':
			if (x1 != -1)
				rubbox(w, x1, y1, x2, y2);
			x2 = x1 = x;
			y2 = y1 = y;
			rubbox(w, x1, y1, x2, y2);
			break;

		case 'a':
			if (x1 != -1) {
				rubbox(w, x1, y1, x2, y2);
				x2 = (abs(y - y1) * wd) / ht;
				x2 = x - x1 > 0 ? x1 + x2 : x1 - x2;
				y2 = (abs(x - x1) * ht) / wd;
				y2 = y - y1 > 0 ? y1 + y2 : y1 - y2;
				if (abs(x2 - x1) > abs(x - x1))
					y2 = y;
				else
					x2 = x;
				rubbox(w, x1, y1, x2, y2);
			}
			break;

		case 'e':
			if (x1 != -1) {
				x2 = (abs(y - y1) * wd) / ht;
				x2 = x - x1 > 0 ? x1 + x2 : x1 - x2;
				y2 = (abs(x - x1) * ht) / wd;
				y2 = y - y1 > 0 ? y1 + y2 : y1 - y2;
				if (abs(x2 - x1) > abs(x - x1))
					y2 = y;
				else
					x2 = x;
/*
	fprintf(stderr, "%d, %d -> %d, %d\n", x1, y1, x2, y2);
*/
			}
			break;

		case 'm':
			if (x1 != -1) {
				rubbox(w, x1, y1, x2, y2);
				if (abs(x - x1) < abs(x - x2))
					x1 = x2;
				if (abs(y - y1) < abs(y - y2))
					y1 = y2;
				x2 = (abs(y - y1) * wd) / ht;
				x2 = x - x1 > 0 ? x1 + x2 : x1 - x2;
				y2 = (abs(x - x1) * ht) / wd;
				y2 = y - y1 > 0 ? y1 + y2 : y1 - y2;
				if (abs(x2 - x1) > abs(x - x1))
					y2 = y;
				else
					x2 = x;
				rubbox(w, x1, y1, x2, y2);
			}
			break;

		default:
			break;
		}
		imCan.cn_x1 = x1;
		imCan.cn_y1 = y1;
		imCan.cn_x2 = x2;
		imCan.cn_y2 = y2;
	}
}


int
get_event_xy(xev, xp, yp)
	XEvent *xev;
	int *xp, *yp;
{
	switch (xev->type) {
	case KeyPress:
	case KeyRelease:
		*xp = xev->xkey.x;
		*yp = xev->xkey.y;
		return 1;
		/* break; */

	case ButtonPress:
	case ButtonRelease:
		*xp = xev->xbutton.x;
		*yp = xev->xbutton.y;
		return 1;
		/* break; */

	case MotionNotify:
		*xp = xev->xmotion.x;
		*yp = xev->xmotion.y;
		return 1;
		/* break; */

	case EnterNotify:
	case LeaveNotify:
		*xp = xev->xcrossing.x;
		*yp = xev->xcrossing.y;
		return 1;
		/* break; */

	default:
		return 0;
	}
}


/*	rubbox()
*
*	Draw a rubberband box XOR in a window.  Call again to undraw.
*/

rubbox(w, x1, y1, x2, y2)
	Window w;
	int x1, x2, y1, y2;
{
	register int x, y, h, v;

	if (x1 < x2)
		{ x = x1; h = x2 - x1; }
	else
		{ x = x2; h = x1 - x2; }
	if (y1 < y2)
		{ y = y1; v = y2 - y1; }
	else
		{ y = y2; v = y1 - y2; }

	XDrawRectangle(xDisp, w, rubGc, x, y, h, v);
	return 0;
}


splat_out(ba, wd, ht, dir)
	u_char *ba;
	int wd, ht;
	int dir;
{
	int x = wd;
	int o = 0;
	int n = wd * ht;

	dir = dir ? 7 : 1;
	while (--n >= 0) {
		*ba++ = x & 7 ? 0 : 255;
		if (--x <= o) {
			o = (o + dir) & 7;
			x = wd + o;
		}
	}
	return 0;
}


label_row(y1, y2, n)
	int y1, y2;
	int n;
{
	int wd = imCan.cn_wd;
	int h = y2 - y1;
	int y, x;
	u_char *ba;
	int c = 240;

	ba = imCan.cn_dat + y1 * wd;
	for (y = h; y-- > 0; ) {
		BZERO(ba, 21);
		ba += wd;
	}
	ba = imCan.cn_dat + y1 * wd;
	for (y = h - 2; y-- > 0; ) {
		ba += wd;
		for (x = 0; x < 20; x++)
			if (x & 3)
				ba[x] = (n & (1 << x / 4)) ? 255 : c;
	}
	repaint_region(&imCan, 0, y1, 20, y2 - 1);
	refresh_region(&imCan, 0, y1, 20, y2 - 1);
	return 0;
}


