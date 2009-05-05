
/* $Id: imp.h,v 1.2 1997/07/09 13:56:54 pvmsrc Exp $ */

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

/* describes an image canvas */

struct canvas {
	Widget cn_wgt;			/* widget */
	Window cn_win;			/* window */
	int cn_wd;				/* image size */
	int cn_ht;
	u_char *cn_dat;			/* image data */
	XImage *cn_xim;			/* ximage struct */
	int cn_zoom;			/* display mag */
	int cn_ox;				/* offset for zoom */
	int cn_oy;
	double cn_re1;			/* corner coords */
	double cn_im1;
	double cn_re2;
	double cn_im2;
	int cn_x1;				/* pick coords */
	int cn_y1;
	int cn_x2;
	int cn_y2;
};

