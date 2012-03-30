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

#include <CL/cl.h>

#define error(...) do { fprintf(stderr, "Error: " __VA_ARGS__); exit(EXIT_FAILURE); } while(0)
#define check(err, str) do { if(err != CL_SUCCESS) { fprintf(stderr, "OpenCL Error (%d): %s\n",err, str); exit(EXIT_FAILURE); }} while(0)

#ifdef UNUSED
#elif defined(__GNUC__)
# define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#else
# define UNUSED(x) x
#endif

#define SIZE 1024
#define TYPE float
#define REALSIZE (SIZE * sizeof(TYPE))

const char * kernel_src = "__kernel void add(__global float*s1, __global float*s2, __global float*d) { \
   size_t x = get_global_id(0);\
   size_t y = get_global_id(1);\
   size_t w = get_global_size(0); \
   int idx = y*w+x; \
   d[idx] = s1[idx] + s2[idx];\
}";



int main(int UNUSED(argc), char** UNUSED(argv)) {
   cl_platform_id platforms[15];
   cl_uint num_platforms;
   cl_device_id devices[15];
   cl_uint num_devices;
   cl_context context;
   cl_program program;
   cl_kernel kernel;
   cl_mem s1m, s2m, dm;
   cl_command_queue cq;
   cl_int err;

   TYPE s1[SIZE],s2[SIZE],d[SIZE];

   {
      int i;
      for (i=0; i<SIZE; i++) {
         s1[i] = 2.0;
         s2[i] = 7.0;
         d[i] = 98.0;
      }
   }

   printf("Querying platform...\n");
   err = clGetPlatformIDs(0, NULL, &num_platforms);
   if (num_platforms == 0) {
      printf("No OpenCL platform found.\n");
      exit(77);
   }
   err = clGetPlatformIDs(sizeof(platforms)/sizeof(cl_platform_id), platforms, NULL);
   check(err, "clGetPlatformIDs");

   printf("Querying devices...\n");
   unsigned int platform_idx;
   for (platform_idx=0; platform_idx<num_platforms; platform_idx++) {
      err = clGetDeviceIDs(platforms[platform_idx], CL_DEVICE_TYPE_GPU, sizeof(devices)/sizeof(cl_device_id), devices, &num_devices);
      check(err, "clGetDeviceIDs");
      if (num_devices != 0)
         break;
   }
   if (num_devices == 0)
      error("No OpenCL device found\n");

   printf("Creating context...\n");
   cl_context_properties properties[] = {CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[platform_idx], 0};
   context = clCreateContext(properties, num_devices, devices, NULL, NULL, &err);
   check(err, "clCreateContext");

   printf("Creating program...\n");
   program = clCreateProgramWithSource(context, 1, &kernel_src, NULL, &err);
   check(err, "clCreateProgram");

   printf("Building program...\n");
   err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
   check(err, "clBuildProgram");

   printf("Creating kernel...\n");
   kernel = clCreateKernel(program, "add", &err);
   check(err, "clCreateKernel");

   printf("Creating buffers...\n");
   s1m = clCreateBuffer(context, CL_MEM_READ_WRITE, REALSIZE, NULL, &err);
   check(err, "clCreateBuffer s1");
   s2m = clCreateBuffer(context, CL_MEM_READ_ONLY, REALSIZE, NULL, &err);
   check(err, "clCreateBuffer s2");
   dm = clCreateBuffer(context, CL_MEM_WRITE_ONLY, REALSIZE, NULL, &err);
   check(err, "clCreateBuffer d");

   printf("Creating command queue...\n");
   cl_event eventW1, eventW2, eventK, eventR;

#ifdef PROFILING
   cq = clCreateCommandQueue(context, devices[0], CL_QUEUE_PROFILING_ENABLE, &err);
#else
   cq = clCreateCommandQueue(context, devices[0], 0, &err);
#endif
   check(err, "clCreateCommandQueue");

   printf("Enqueueing WriteBuffers...\n");
   err = clEnqueueWriteBuffer(cq, s1m, CL_FALSE, 0, REALSIZE, s1, 0, NULL, &eventW1);
   check(err, "clEnqueueWriteBuffer s1");
   err = clEnqueueWriteBuffer(cq, s2m, CL_FALSE, 0, REALSIZE, s2, 0, NULL, &eventW2);
   check(err, "clEnqueueWriteBuffer s2");

   printf("Setting kernel arguments...\n");
   err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &s1m);
   check(err, "clSetKernelArg 0");
   err = clSetKernelArg(kernel, 1, sizeof(cl_mem), &s2m);
   check(err, "clSetKernelArg 1");
   err = clSetKernelArg(kernel, 2, sizeof(cl_mem), &dm);
   check(err, "clSetKernelArg 2");

   printf("Enqueueing NDRangeKernel...\n");
   size_t local[3] = {16, 1, 1};
   size_t global[3] = {1024, 1, 1};
   cl_event deps[] = {eventW1,eventW2};
   err = clEnqueueNDRangeKernel(cq, kernel, 3, NULL, global, local, 2, deps, &eventK);
   check(err, "clEnqueueNDRangeKernel");

   printf("Enqueueing ReadBuffer...\n");
   err = clEnqueueReadBuffer(cq, dm, CL_FALSE, 0, REALSIZE, d, 0, NULL, &eventR);
   check(err, "clEnqueueReadBuffer");

   printf("Finishing queue...\n");
   clFinish(cq);

   printf("Data...\n");
   {
      int i;
      for (i=0; i<SIZE; i++) {
        printf("%f ", d[i]);
      }
      printf("\n");
   }

#ifdef PROFILING
   #define DURATION(event,label) do { \
      cl_ulong t0,t1; \
      err = clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START, sizeof(cl_ulong), &t0, NULL);\
      check(err, "clGetEventProfilingInfo");\
      err = clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END, sizeof(cl_ulong), &t1, NULL);\
      check(err, "clGetEventProfilingInfo");\
      printf("Profiling %s: %lu nanoseconds\n", label, t1-t0);\
   } while (0);

   DURATION(eventW1, "first buffer writing");
   DURATION(eventW2, "second buffer writing");
   DURATION(eventK, "kernel execution");
   DURATION(eventR, "result buffer reading");
#endif


   printf("Releasing events...\n");
   err = clReleaseEvent(eventW1);
   err |= clReleaseEvent(eventW2);
   err |= clReleaseEvent(eventK);
   err |= clReleaseEvent(eventR);
   check(err, "clReleaseCommandQueue");

   printf("Releasing command queue...\n");
   err = clReleaseCommandQueue(cq);
   check(err, "clReleaseCommandQueue");

   printf("Releasing buffers...\n");
   err = clReleaseMemObject(s1m);
   check(err, "clReleaseMemObject s1");
   err = clReleaseMemObject(s2m);
   check(err, "clReleaseMemObject s2");
   err = clReleaseMemObject(dm);
   check(err, "clReleaseMemObject d");

   printf("Releasing kernel...\n");
   err = clReleaseKernel(kernel);
   check(err, "clReleaseKernel");

   printf("Releasing program...\n");
   err = clReleaseProgram(program);
   check(err, "clReleaseProgram");

   printf("Releasing context...\n");
   err = clReleaseContext(context);
   check(err, "clReleaseContext");

   return 0;
}
