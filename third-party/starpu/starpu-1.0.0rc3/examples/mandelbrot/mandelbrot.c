/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010, 2011  Université de Bordeaux 1
 * Copyright (C) 2010, 2011, 2012  Centre National de la Recherche Scientifique
 *
 * StarPU is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 *
 * StarPU is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU Lesser General Public License in COPYING.LGPL for more details.
 */

#include <starpu.h>
#ifdef STARPU_USE_OPENCL
#include <starpu_opencl.h>
#endif
#include <sys/time.h>
#include <math.h>
#include <limits.h>
#ifdef STARPU_HAVE_X11
#include <X11/Xlib.h>
#include <X11/Xutil.h>
int use_x11 = 1;
#endif

int demo = 0;
static double demozoom = 0.05;

/* NB: The X11 code is inspired from the http://locklessinc.com/articles/mandelbrot/ article */

static int nblocks = 20;
static int height = 400;
static int width = 640;
static int maxIt = 20000; /* max number of iteration in the Mandelbrot function */
static int niter = -1; /* number of loops in case we don't use X11, -1 means infinite */
static int use_spmd = 0;

static double leftX = -0.745;
static double rightX = -0.74375;
static double topY = .15;
static double bottomY = .14875;

/*
 *	X11 window management
 */

#ifdef STARPU_HAVE_X11
/* X11 data */
static Display *dpy;
static Window win;
static XImage *bitmap;
static GC gc;
static KeySym Left=-1, Right, Down, Up, Alt ;

static void exit_x11(void)
{
	XDestroyImage(bitmap);
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);
}

static void init_x11(int width, int height, unsigned *buffer)
{
	/* Attempt to open the display */
	dpy = XOpenDisplay(NULL);
	
	/* Failure */
	if (!dpy)
		exit(0);
	
	unsigned long white = WhitePixel(dpy,DefaultScreen(dpy));
	unsigned long black = BlackPixel(dpy,DefaultScreen(dpy));

	win = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0,
					width, height, 0, black, white);
	
	/* We want to be notified when the window appears */
	XSelectInput(dpy, win, StructureNotifyMask);
	
	/* Make it appear */
	XMapWindow(dpy, win);
	
	XTextProperty tp;
	char name[128] = "Mandelbrot - StarPU";
	char *n = name;
	Status st = XStringListToTextProperty(&n, 1, &tp);
	if (st)
		XSetWMName(dpy, win, &tp);

	/* Wait for the MapNotify event */
	XFlush(dpy);
	
	int depth = DefaultDepth(dpy, DefaultScreen(dpy));
	Visual *visual = DefaultVisual(dpy, DefaultScreen(dpy));

	/* Make bitmap */
	bitmap = XCreateImage(dpy, visual, depth,
		ZPixmap, 0, (char *)buffer,
		width, height, 32, 0);
	
	/* Init GC */
	gc = XCreateGC(dpy, win, 0, NULL);
	XSetForeground(dpy, gc, black);
	
	XSelectInput(dpy, win, ExposureMask | KeyPressMask | StructureNotifyMask);
	
	Atom wmDeleteMessage;
	wmDeleteMessage = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
	XSetWMProtocols(dpy, win, &wmDeleteMessage, 1);

        Left = XStringToKeysym ("Left");
        Right = XStringToKeysym ("Right");
        Up = XStringToKeysym ("Up");
        Down = XStringToKeysym ("Down");
        Alt = XStringToKeysym ("Alt");
}

static int handle_events(void)
{
	XEvent event;
	XNextEvent(dpy, &event);

	KeySym key;
	char text[255];

	if (event.type == KeyPress)
	{
		XLookupString(&event.xkey,text,255,&key,0);
		if (key == Left)
		{
			double widthX = rightX - leftX;
			leftX -= 0.25*widthX;
			rightX -= 0.25*widthX;
		}
		else if (key == Right)
		{
			double widthX = rightX - leftX;
			leftX += 0.25*widthX;
			rightX += 0.25*widthX;
		}
		else if (key == Up)
		{
			double heightY = topY - bottomY;
			topY += 0.25*heightY;
			bottomY += 0.25*heightY;
		}
		else if (key == Down)
		{
			double heightY = topY - bottomY;
			topY -= 0.25*heightY;
			bottomY -= 0.25*heightY;
		}
		else
		{
			double widthX = rightX - leftX;
			double heightY = topY - bottomY;

			if (text[0] == '-')
			{
				/* Zoom out */
				leftX -= 0.125*widthX;
				rightX += 0.125*widthX;
				topY += 0.125*heightY;
				bottomY -= 0.125*heightY;
			}
			else if (text[0] == '+')
			{
				/* Zoom in */
				leftX += 0.125*widthX;
				rightX -= 0.125*widthX;
				topY -= 0.125*heightY;
				bottomY += 0.125*heightY;
			}
		}

		if (text[0]=='q')
		{
			return -1;
		}
	}

	if (event.type==ButtonPress)
	{
		/* tell where the mouse Button was Pressed */
		printf("You pressed a button at (%i,%i)\n",
			event.xbutton.x,event.xbutton.y);
	}

	return 0;
}
#endif

/*
 *	OpenCL kernel
 */

#ifdef STARPU_USE_OPENCL
char *mandelbrot_opencl_src = "\
#pragma OPENCL EXTENSION cl_khr_fp64 : enable\n\
#define MIN(a,b) (((a)<(b))? (a) : (b))					\n\
__kernel void mandelbrot_kernel(__global unsigned* a,			\n\
          double leftX, double topY,					\n\
          double stepX, double stepY,					\n\
          int maxIt, int iby, int block_size, int width)		\n\
{									\n\
    size_t id_x = get_global_id(0);	\n\
    size_t id_y = get_global_id(1);	\n\
    if ((id_x < width) && (id_y < block_size))				\n\
    {									\n\
        double xc = leftX + id_x * stepX;				\n\
        double yc = topY - (id_y + iby*block_size) * stepY;		\n\
        int it;								\n\
        double x,y;							\n\
        x = y = (double)0.0;						\n\
        for (it=0;it<maxIt;it++)					\n\
        {								\n\
          double x2 = x*x;						\n\
          double y2 = y*y;						\n\
          if (x2+y2 > 4.0) break;					\n\
          double twoxy = (double)2.0*x*y;				\n\
          x = x2 - y2 + xc;						\n\
          y = twoxy + yc;						\n\
        }								\n\
       unsigned int v = MIN((1024*((float)(it)/(2000))), 256);		\n\
       a[id_x + width * id_y] = (v<<16|(255-v)<<8);			\n\
   }									\n\
}";

static struct starpu_opencl_program opencl_programs;

static void compute_block_opencl(void *descr[], void *cl_arg)
{
	int iby, block_size;
	double stepX, stepY;
	int *pcnt; /* unused for CUDA tasks */
	starpu_codelet_unpack_args(cl_arg, &iby, &block_size, &stepX, &stepY, &pcnt);

	cl_mem data = (cl_mem)STARPU_VECTOR_GET_DEV_HANDLE(descr[0]);

	cl_kernel kernel;
	cl_command_queue queue;
	cl_event event;

	int id = starpu_worker_get_id();
	int devid = starpu_worker_get_devid(id);

	starpu_opencl_load_kernel(&kernel, &queue, &opencl_programs, "mandelbrot_kernel", devid);

	clSetKernelArg(kernel, 0, sizeof(data), &data);
	clSetKernelArg(kernel, 1, sizeof(leftX), &leftX);
	clSetKernelArg(kernel, 2, sizeof(topY), &topY);
	clSetKernelArg(kernel, 3, sizeof(stepX), &stepX);
	clSetKernelArg(kernel, 4, sizeof(stepY), &stepY);
	clSetKernelArg(kernel, 5, sizeof(maxIt), &maxIt);
	clSetKernelArg(kernel, 6, sizeof(iby), &iby);
	clSetKernelArg(kernel, 7, sizeof(block_size), &block_size);
	clSetKernelArg(kernel, 8, sizeof(width), &width);

	unsigned dim = 16;
	size_t local[2] = {dim, 1};
	size_t global[2] = {width, block_size};
	clEnqueueNDRangeKernel(queue, kernel, 2, NULL, global, local, 0, NULL, &event);
	clFinish(queue);
	starpu_opencl_collect_stats(event);
	clReleaseEvent(event);
	starpu_opencl_release_kernel(kernel);
}
#endif

/*
 *	CPU kernel
 */

static void compute_block(void *descr[], void *cl_arg)
{
	int ix, iy;

	int iby, block_size;
	double stepX, stepY;
	int *pcnt; /* unused for sequential tasks */
	starpu_codelet_unpack_args(cl_arg, &iby, &block_size, &stepX, &stepY, &pcnt);

	unsigned *data = (unsigned *)STARPU_VECTOR_GET_PTR(descr[0]);

	int local_iy;
	for (local_iy = 0; local_iy < block_size; local_iy++)
	{
		iy = iby*block_size + local_iy;
		for (ix = 0; ix < width; ix++)
		{
			double cx = leftX + ix * stepX;
			double cy = topY - iy * stepY;
			/* Z = X+I*Y */
			double x = 0;
			double y = 0;
			int it;
			for (it = 0; it < maxIt; it++)
			{
				double x2 = x*x;
				double y2 = y*y;

				/* Stop iterations when |Z| > 2 */
				if (x2 + y2 > 4.0)
					break;

				double twoxy = 2.0*x*y;

				/* Z = Z^2 + C */
				x = x2 - y2 + cx;
				y = twoxy + cy;
			}
	
			unsigned int v = STARPU_MIN((1024*((float)(it)/(2000))), 256);
			data[ix + local_iy*width] = (v<<16|(255-v)<<8);
		}
	}
}

static void compute_block_spmd(void *descr[], void *cl_arg)
{

	int iby, block_size;
	double stepX, stepY;
	int *pcnt;
	starpu_codelet_unpack_args(cl_arg, &iby, &block_size, &stepX, &stepY, &pcnt);

	unsigned *data = (unsigned *)STARPU_VECTOR_GET_PTR(descr[0]);

	int ix, iy; /* global coordinates */
	int local_iy; /* current line */

	while (1)
	{
		local_iy = STARPU_ATOMIC_ADD((unsigned int *)pcnt, 1) - 1;
		if (local_iy >= block_size)
			break;

		iy = iby*block_size + local_iy;
	
		for (ix = 0; ix < width; ix++)
		{
			double cx = leftX + ix * stepX;
			double cy = topY - iy * stepY;
			/* Z = X+I*Y */
			double x = 0;
			double y = 0;
			int it;
			for (it = 0; it < maxIt; it++)
			{
				double x2 = x*x;
				double y2 = y*y;

				/* Stop iterations when |Z| > 2 */
				if (x2 + y2 > 4.0)
					break;

				double twoxy = 2.0*x*y;

				/* Z = Z^2 + C */
				x = x2 - y2 + cx;
				y = twoxy + cy;
			}
	
			unsigned int v = STARPU_MIN((1024*((float)(it)/(2000))), 256);
			data[ix + local_iy*width] = (v<<16|(255-v)<<8);
		}
	}
}



static struct starpu_codelet spmd_mandelbrot_cl =
{
	.where = STARPU_CPU|STARPU_OPENCL,
	.type = STARPU_SPMD,
	.max_parallelism = INT_MAX,
	.cpu_funcs = {compute_block_spmd, NULL},
#ifdef STARPU_USE_OPENCL
	.opencl_funcs = {compute_block_opencl, NULL},
#endif
	.nbuffers = 1
};

static struct starpu_codelet mandelbrot_cl =
{
	.where = STARPU_CPU|STARPU_OPENCL,
	.type = STARPU_SEQ,
	.cpu_funcs = {compute_block, NULL},
#ifdef STARPU_USE_OPENCL
	.opencl_funcs = {compute_block_opencl, NULL},
#endif
	.nbuffers = 1
};

static void parse_args(int argc, char **argv)
{
	int i;
	for (i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-h") == 0)
		{
			fprintf(stderr, "Usage: %s [-h] [ -width 800] [-height 600] [-nblocks 16] [-no-x11] [-pos leftx:rightx:bottomy:topy] [-niter 1000] [-spmd] [-demo] [-demozoom 0.2]\n", argv[0]);
			exit(-1);
		}

		if (strcmp(argv[i], "-width") == 0)
		{
			char *argptr;
			width = strtol(argv[++i], &argptr, 10);
		}

		if (strcmp(argv[i], "-height") == 0)
		{
			char *argptr;
			height = strtol(argv[++i], &argptr, 10);
		}

		if (strcmp(argv[i], "-nblocks") == 0)
		{
			char *argptr;
			nblocks = strtol(argv[++i], &argptr, 10);
		}

		if (strcmp(argv[i], "-niter") == 0)
		{
			char *argptr;
			niter = strtol(argv[++i], &argptr, 10);
		}

		if (strcmp(argv[i], "-pos") == 0)
		{
			int ret = sscanf(argv[++i], "%lf:%lf:%lf:%lf", &leftX, &rightX, &bottomY, &topY);
			assert(ret == 4);
		}

		if (strcmp(argv[i], "-demo") == 0)
		{
			demo = 1;
			leftX = -50.22749575062760;
			rightX = 48.73874621262927;
			topY = -49.35016705749115;
			bottomY = 49.64891691946615;

		}

		if (strcmp(argv[i], "-demozoom") == 0)
		{
			char *argptr;
			demozoom = strtof(argv[++i], &argptr);
		}

		if (strcmp(argv[i], "-no-x11") == 0)
		{
#ifdef STARPU_HAVE_X11
			use_x11 = 0;
#endif
		}

		if (strcmp(argv[i], "-spmd") == 0)
		{
			use_spmd = 1;
		}
	}
}

int main(int argc, char **argv)
{
	int ret;

	parse_args(argc, argv);

	/* We don't use CUDA in that example */
	struct starpu_conf conf;
	starpu_conf_init(&conf);
	conf.ncuda = 0;

	if (use_spmd)
		conf.sched_policy_name = "pgreedy";

	ret = starpu_init(&conf);
	STARPU_CHECK_RETURN_VALUE(ret, "starpu_init");

	unsigned *buffer;
	starpu_malloc((void **)&buffer, height*width*sizeof(unsigned));

#ifdef STARPU_HAVE_X11
	if (use_x11)
		init_x11(width, height, buffer);
#endif

	int block_size = height/nblocks;
	STARPU_ASSERT((height % nblocks) == 0);

#ifdef STARPU_USE_OPENCL
	starpu_opencl_load_opencl_from_string(mandelbrot_opencl_src, &opencl_programs, NULL);
#endif

	starpu_data_handle_t block_handles[nblocks];
	
	int iby;
	for (iby = 0; iby < nblocks; iby++)
	{
		unsigned *data = &buffer[iby*block_size*width];
		starpu_vector_data_register(&block_handles[iby], 0,
                        (uintptr_t)data, block_size*width, sizeof(unsigned));
	}

	unsigned iter = 0;

	struct timeval start, end;

	if (demo)
		gettimeofday(&start, NULL);

	while (niter-- != 0)
	{
		int ret;
		double stepX = (rightX - leftX)/width;
		double stepY = (topY - bottomY)/height;

		/* In case we have a SPMD task, each worker will grab tasks in
		 * a greedy and select which piece of image to compute by
		 * incrementing a counter shared by all the workers within the
		 * parallel task. */
		int per_block_cnt[nblocks];

		for (iby = 0; iby < nblocks; iby++)
		{
			per_block_cnt[iby] = 0;
			int *pcnt = &per_block_cnt[iby];

			ret = starpu_insert_task(use_spmd?&spmd_mandelbrot_cl:&mandelbrot_cl,
						 STARPU_VALUE, &iby, sizeof(iby),
						 STARPU_VALUE, &block_size, sizeof(block_size),
						 STARPU_VALUE, &stepX, sizeof(stepX),
						 STARPU_VALUE, &stepY, sizeof(stepY),
						 STARPU_W, block_handles[iby],
						 STARPU_VALUE, &pcnt, sizeof(int *),
						 0);
			STARPU_CHECK_RETURN_VALUE(ret, "starpu_insert_task");
		}

		for (iby = 0; iby < nblocks; iby++)
		{
#ifdef STARPU_HAVE_X11
			if (use_x11)
			{
				starpu_data_acquire(block_handles[iby], STARPU_R);
				XPutImage(dpy, win, gc, bitmap,
					0, iby*block_size,
					0, iby*block_size,
					width, block_size);
				starpu_data_release(block_handles[iby]);
			}
#endif
		}


		if (demo)
		{
			/* Zoom in */
			double zoom_factor = demozoom;
			double widthX = rightX - leftX;
			double heightY = topY - bottomY;

			iter++;

			/* If the window is too small, we reset the demo and display some statistics */
			if ((fabs(widthX) < 1e-12) || (fabs(heightY) < 1e-12))
			{
				leftX = -50.22749575062760;
				rightX = 48.73874621262927;
				topY = -49.35016705749115;
				bottomY = 49.64891691946615;

				gettimeofday(&end, NULL);
				double timing = (double)((end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec - start.tv_usec));

				fprintf(stderr, "Time to generate %u frames : %f s\n", iter, timing/1000000.0);
				fprintf(stderr, "Average FPS: %f\n", ((double)iter*1e+6)/timing);

				/* Reset counters */
				iter = 0;
				gettimeofday(&start, NULL);
			}
			else
			{
				leftX += (zoom_factor/2)*widthX;
				rightX -= (zoom_factor/2)*widthX;
				topY -= (zoom_factor/2)*heightY;
				bottomY += (zoom_factor/2)*heightY;
			}
	
		}
#ifdef STARPU_HAVE_X11
		else if (use_x11 && handle_events())
			break;
#endif
	}

#ifdef STARPU_HAVE_X11
	if (use_x11)
		exit_x11();
#endif

	for (iby = 0; iby < nblocks; iby++)
		starpu_data_unregister(block_handles[iby]);

/*	starpu_data_free_pinned_if_possible(buffer); */

	starpu_shutdown();

	return 0;
}
