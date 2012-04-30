/* StarPU --- Runtime system for heterogeneous multicore architectures.
 *
 * Copyright (C) 2010,2011 University of Bordeaux
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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Uncomment this to activate X11 display */
//#define USE_X11

#define SHORT_LOG 1
#define ROUND_ROBIN

#ifdef USE_X11
#include <X11/Xlib.h>
#include <X11/Xutil.h>
int use_x11 = 1;
#else
int use_x11 = 0;
#endif

int demo = 0;
int frames = -1;


#include <pthread.h>
#include <assert.h>
#include <sys/time.h>

#include <CL/cl.h>

#define error(...) do { fprintf(stderr, "Error: " __VA_ARGS__); exit(EXIT_FAILURE); } while(0)
#define check(err, str) do { if(err != CL_SUCCESS) { fprintf(stderr, "OpenCL Error (%d): %s\n",err, str); exit(EXIT_FAILURE); }} while(0)

#ifdef UNUSED
#elif defined(__GNUC__)
# define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#else
# define UNUSED(x) x
#endif

const char * kernel_src = "\
#pragma OPENCL EXTENSION cl_khr_fp64 : enable\n\
#define TYPE double \n\
#define MIN(a,b) (((a)<(b))? (a) : (b))\n\
      __kernel void mandelbrot_kernel(__global uint * a,\n\
          TYPE leftX, TYPE topY,\n\
          TYPE stepX, TYPE stepY,\n\
          uint maxIt, uint iby, uint block_size)\n\
{\n\
  TYPE xc = leftX + get_global_id(0) * stepX;\n\
  TYPE yc = iby*block_size*stepY + topY  + get_global_id(1) * stepY;\n\
  int it;\n\
  TYPE x,y;\n\
  x = y = (TYPE)0.0;\n\
  for (it=0;it<maxIt;it++)\n\
  {\n\
    TYPE x2 = x*x;\n\
    TYPE y2 = y*y;\n\
    if (x2+y2 > (TYPE)4) break; \n\
    TYPE twoxy = (TYPE)2*x*y;\n\
    x = x2 - y2 + xc;\n\
    y = twoxy + yc;\n\
  }\n\
  uint v = MIN((1024*((float)(it)/(2000))), 256);\n\
  a[get_global_id(0) + get_global_id(1)*get_global_size(0)] = (v<<16|(255-v)<<8); \n\
}";

static cl_uint nblocks = 8;
static cl_uint height = 768;
static cl_uint width = 1024;
static cl_uint maxIt = 20000;

static cl_uint group_size = 64;

static double leftX = -0.745;
static double rightX = -0.74375;
static double topY = .15;
static double bottomY = .14875;

#ifdef USE_X11
      /* X11 data */
      static Display *dpy;
      static Window win;
      static XImage *bitmap;
      static GC gc;
      static KeySym Left=-1, Right, Down, Up, Alt ;
      static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void exit_x11(void)
{
  XDestroyImage(bitmap);
  XDestroyWindow(dpy, win);
  XCloseDisplay(dpy);
}

static void init_x11(int width, int height, cl_uint *buffer)
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
  char name[128] = "Mandelbrot";
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

  double coef = 0.05;

  if (event.type == KeyPress)
  {
    XLookupString(&event.xkey,text,255,&key,0);
    if (key == Left)
    {
      double widthX = rightX - leftX;
      leftX -= coef*widthX;
      rightX -= coef*widthX;
    }
    else if (key == Right)
    {
      double widthX = rightX - leftX;
      leftX += coef*widthX;
      rightX += coef*widthX;
    }
    else if (key == Down)
    {
      double heightY = topY - bottomY;
      topY += coef*heightY;
      bottomY += coef*heightY;
    }
    else if (key == Up)
    {
      double heightY = topY - bottomY;
      topY -= coef*heightY;
      bottomY -= coef*heightY;
    }
    else {
      double widthX = rightX - leftX;
      double heightY = topY - bottomY;

      if (text[0] == '-')
      {
        /* Zoom out */
        leftX -= (coef/2)*widthX;
        rightX += (coef/2)*widthX;
        topY += (coef/2)*heightY;
        bottomY -= (coef/2)*heightY;
      }
      else if (text[0] == '+')
      {
        /* Zoom in */
        leftX += (coef/2)*widthX;
        rightX -= (coef/2)*widthX;
        topY -= (coef/2)*heightY;
        bottomY += (coef/2)*heightY;
      }
    }

    if (text[0]=='q') {
      return -1;
    }
  }

  if (event.type==ButtonPress) {
    /* tell where the mouse Button was Pressed */
    printf("You pressed a button at (%i,%i)\n",
        event.xbutton.x,event.xbutton.y);
  }

  return 0;
}
#endif //USE_X11

static void parse_args(int argc, char **argv)
{
	int i;
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-h") == 0) {
			fprintf(stderr, "Usage: %s [-h] [ -width 1024] [-height 768] [-nblocks 16] [-group_size 64] [-no-x11] [-demo] [-frames N] [-pos leftx:rightx:bottomy:topy]\n", argv[0]);
			exit(-1);
		}

		if (strcmp(argv[i], "-width") == 0) {
			char *argptr;
			width = strtol(argv[++i], &argptr, 10);
		}

		if (strcmp(argv[i], "-frames") == 0) {
			char *argptr;
			frames = strtol(argv[++i], &argptr, 10);
		}

		if (strcmp(argv[i], "-height") == 0) {
			char *argptr;
			height = strtol(argv[++i], &argptr, 10);
		}

		if (strcmp(argv[i], "-group_size") == 0) {
			char *argptr;
			group_size = strtol(argv[++i], &argptr, 10);
		}

		if (strcmp(argv[i], "-nblocks") == 0) {
			char *argptr;
			nblocks = strtol(argv[++i], &argptr, 10);
		}

		if (strcmp(argv[i], "-pos") == 0) {
			int ret = sscanf(argv[++i], "%lf:%lf:%lf:%lf", &leftX, &rightX, &bottomY, &topY);
			assert(ret == 4);
		}

		if (strcmp(argv[i], "-demo") == 0) {
			demo = 1;
			leftX = -50.22749575062760;
			rightX = 48.73874621262927;
			topY = -49.35016705749115;
			bottomY = 49.64891691946615;

		}

		if (strcmp(argv[i], "-no-x11") == 0) {
#ifdef USE_X11
			use_x11 = 0;
#endif
		}
	}
}

int main(int argc, char **argv) {
#define MAX_DEVICES 20
  cl_platform_id platforms[15];
  cl_uint num_platforms;
  cl_device_id devices[15];
  cl_uint num_devices;
  cl_context context;
  cl_program program;
  cl_kernel kernel;
  cl_command_queue cq[MAX_DEVICES];
  cl_int err;
  cl_uint i;

  parse_args(argc, argv);

  cl_uint block_size = height/nblocks;
  assert((height % nblocks) == 0);
  assert((width % group_size) == 0);

  err = clGetPlatformIDs(0, NULL, &num_platforms);
  if (num_platforms == 0) {
    printf("No OpenCL platform found\n");
    exit(0);
  }
  err = clGetPlatformIDs(sizeof(platforms)/sizeof(cl_platform_id), platforms, NULL);
  check(err, "clGetPlatformIDs");

  unsigned int platform_idx;
  for (platform_idx=0; platform_idx<num_platforms; platform_idx++) {
    err = clGetDeviceIDs(platforms[platform_idx], CL_DEVICE_TYPE_GPU, sizeof(devices)/sizeof(cl_device_id), devices, &num_devices);
    check(err, "clGetDeviceIDs");
    if (num_devices != 0)
      break;
  }
  if (num_devices == 0)
    error("No OpenCL device found\n");

  cl_context_properties properties[] = {CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[platform_idx], 0};
  context = clCreateContext(properties, num_devices, devices, NULL, NULL, &err);
  check(err, "clCreateContext");

  program = clCreateProgramWithSource(context, 1, &kernel_src, NULL, &err);
  check(err, "clCreateProgram");

  err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
  check(err, "clBuildProgram");

  kernel = clCreateKernel(program, "mandelbrot_kernel", &err);
  check(err, "clCreateKernel");


  for (i=0; i<num_devices; i++)
    cq[i] = clCreateCommandQueue(context, devices[i],  CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, &err);
  check(err, "clCreateCommandQueue");

  cl_uint *buffer;
  buffer = malloc(height*width*sizeof(cl_uint));

#ifdef USE_X11
  if (use_x11)
    init_x11(width, height, buffer);
#endif // USE_X11



  cl_mem block_handles[nblocks];

  cl_uint iby;

  for (iby = 0; iby < nblocks; iby++) {
    cl_uint *data = &buffer[iby*block_size*width];
    block_handles[iby] = clCreateBuffer(context, CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR, block_size*width*sizeof(cl_uint), data, &err);
  }

  int stop = 0;
  int frame = 0;

  while (!stop) {
    struct timeval start, end;
    gettimeofday(&start, NULL);

    if (frames != -1) {
      frame++;
      stop = (frame == frames);
    }

    double stepX = (rightX - leftX)/width;
    double stepY = (topY - bottomY)/height;
    cl_event ker_events[nblocks];
    void * ptrs[nblocks];

    for (iby = 0; iby < nblocks; iby++) {
      err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &block_handles[iby]);
      check(err, "clSetKernelArg out");
      err = clSetKernelArg(kernel, 1, sizeof(cl_double), &leftX);
      check(err, "clSetKernelArg leftX");
      err = clSetKernelArg(kernel, 2, sizeof(cl_double), &topY);
      check(err, "clSetKernelArg topY");
      err = clSetKernelArg(kernel, 3, sizeof(cl_double), &stepX);
      check(err, "clSetKernelArg leftX");
      err = clSetKernelArg(kernel, 4, sizeof(cl_double), &stepY);
      check(err, "clSetKernelArg topY");
      err = clSetKernelArg(kernel, 5, sizeof(cl_uint), &maxIt);
      check(err, "clSetKernelArg maxIt");
      err = clSetKernelArg(kernel, 6, sizeof(cl_uint), &iby);
      check(err, "clSetKernelArg iby");
      err = clSetKernelArg(kernel, 7, sizeof(cl_uint), &block_size);
      check(err, "clSetKernelArg block_size");

      size_t local[3] = {group_size, 1, 1};
      size_t global[3] = {width, block_size, 1};
#ifdef ROUND_ROBIN
      int dev = iby % num_devices;
#else
      int dev = 0;
#endif
      err = clEnqueueNDRangeKernel(cq[dev], kernel, 3, NULL, global, local, 0, NULL, &ker_events[iby]);
      check(err, "clEnqueueNDRangeKernel");
    }

    for (iby = 0; iby < nblocks; iby++) {
#ifdef ROUND_ROBIN
      int dev = iby % num_devices;
#else
      int dev = 0;
#endif
      ptrs[iby] = clEnqueueMapBuffer(cq[dev], block_handles[iby], CL_FALSE,CL_MAP_READ, 0, block_size*width*sizeof(cl_uint), 1, &ker_events[iby], NULL, NULL);
    }

#ifdef ROUND_ROBIN
    for (i = 0; i < num_devices; i++)
      clFinish(cq[i]);
#else
    clFinish(cq[0]);
#endif

    gettimeofday(&end, NULL);
    double timing = (double)((end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec - start.tv_usec));

#ifdef SHORT_LOG
    fprintf(stderr, "%f\n", timing/1000.0);
#else
    fprintf(stderr, "Time to generate frame : %f ms\n", timing/1000.0);
    fprintf(stderr, "%14.14f:%14.14f:%14.14f:%14.14f\n", leftX, rightX, bottomY, topY);
#endif

#ifdef USE_X11
    if (use_x11) {
      for (iby = 0; iby < nblocks; iby++) {
        pthread_mutex_lock(&mutex);
        XPutImage(dpy, win, gc, bitmap,
            0, iby*block_size,
            0, iby*block_size,
            width, block_size);
        pthread_mutex_unlock(&mutex);
      }
    }
#endif

    for (iby = 0; iby < nblocks; iby++) {
#ifdef ROUND_ROBIN
      int dev = iby % num_devices;
#else
      int dev = 0;
#endif
      clEnqueueUnmapMemObject(cq[dev], block_handles[iby], ptrs[iby], 0, NULL, NULL);
      clReleaseEvent(ker_events[iby]);
    }



    if (demo) {
      /* Zoom in */
      double zoom_factor = 0.05;
      double widthX = rightX - leftX;
      double heightY = topY - bottomY;
      leftX += (zoom_factor/2)*widthX;
      rightX -= (zoom_factor/2)*widthX;
      topY -= (zoom_factor/2)*heightY;
      bottomY += (zoom_factor/2)*heightY;
    }
    else {
#ifdef USE_X11
      if (use_x11) {
        handle_events();
      }
#else
      stop = 1;
#endif
    }
  }

#ifdef USE_X11
  if (use_x11)
    exit_x11();
#endif

  for (iby = 0; iby < nblocks; iby++) {
    clReleaseMemObject(block_handles[iby]);
  }

  for (i=0; i<num_devices; i++)
    clReleaseCommandQueue(cq[i]);

  clReleaseKernel(kernel);
  clReleaseProgram(program);
  clReleaseContext(context);

  return 0;
}
