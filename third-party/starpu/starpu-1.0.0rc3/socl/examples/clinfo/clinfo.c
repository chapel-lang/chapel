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

#include <CL/cl.h>

static inline void 
checkErr(cl_int err, const char * name) {
    if (err != CL_SUCCESS) {
        fprintf(stderr, "ERROR: %s (%d)\n", name, err);
        exit(1);
    }
}

int
main(void) {
   cl_int err;
   cl_uint num_platforms;
   cl_platform_id *platforms;

   // Plaform info
   err = clGetPlatformIDs(0, NULL, &num_platforms);
   if (num_platforms == 0) {
      printf("No OpenCL platform found. If you use SOCL, this could mean StarPU wasn't configured for OpenCL. Try disabling CUDA support in StarPU (export STARPU_NCUDA=0).\n");
      exit(77);
   }
   checkErr(err, "Unable to get platform count");

   platforms = (cl_platform_id*)malloc(sizeof(cl_platform_id)*num_platforms);
   err = clGetPlatformIDs(num_platforms, platforms, NULL);
   checkErr(err, "Unable to get platform list");
   
   
   // Iteratate over platforms
   printf("Number of platforms:\t\t\t\t %d\n", num_platforms);

   {
      unsigned int i;
      for (i=0; i<num_platforms; i++) {
         char str[256];
         err = clGetPlatformInfo(platforms[i], CL_PLATFORM_PROFILE, sizeof(str), &str, NULL);
         checkErr(err, "clGetPlatformInfo(CL_PLATFORM_PROFILE)");
         printf("  Plaform Profile:\t\t\t\t %s\n", str);    

         err= clGetPlatformInfo(platforms[i], CL_PLATFORM_VERSION, sizeof(str), &str, NULL);
         checkErr(err, "clGetPlatformInfo(CL_PLATFORM_VERSION)");
         printf("  Plaform Version:\t\t\t\t %s\n", str);    

         err = clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, sizeof(str), &str, NULL);
         checkErr(err, "clGetPlatformInfo(CL_PLATFORM_NAME)");
         printf("  Plaform Name:\t\t\t\t\t %s\n", str);    

         err = clGetPlatformInfo(platforms[i], CL_PLATFORM_VENDOR, sizeof(str), &str, NULL);
         checkErr(err, "clGetPlatformInfo(CL_PLATFORM_VENDOR)");
         printf("  Plaform Vendor:\t\t\t\t %s\n", str);    

         err = clGetPlatformInfo(platforms[i], CL_PLATFORM_EXTENSIONS, sizeof(str), &str, NULL);
         checkErr(err, "clGetPlatformInfo(CL_PLATFORM_EXTENSIONS)");
         printf("  Plaform Extensions:\t\t\t %s\n", str);    
      }
   }

   printf("\n\n");

   // Now Iteratate over each platform and its devices
   {
      unsigned int i;
      for (i=0; i<num_platforms; i++) {
         char str[256];
         cl_device_id * devices;
         cl_uint num_devices;

         err = clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, sizeof(str), &str, NULL);
         checkErr(err, "clGetPlatformInfo(CL_PLATFORM_NAME)");
         printf("  Plaform Name:\t\t\t\t\t %s\n", str);    

         err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, 0, NULL, &num_devices);
         checkErr(err, "clGetDeviceIds(CL_DEVICE_TYPE_ALL)");
         devices = (cl_device_id*)malloc(sizeof(cl_device_id)*num_devices);
         
         err = clGetDeviceIDs(platforms[i], CL_DEVICE_TYPE_ALL, num_devices, devices, NULL);
         checkErr(err, "clGetDeviceIds(CL_DEVICE_TYPE_ALL)");

         printf("  Number of devices:\t\t\t\t %d\n", num_devices);
         {
            unsigned int j;
            for (j=0; j<num_devices; j++) {
               cl_device_type dev_type;
               printf("\n  DEVICE %d\n", j);
               
               err = clGetDeviceInfo(devices[j], CL_DEVICE_TYPE, sizeof(dev_type), &dev_type, NULL);
               checkErr(err, "clGetDeviceInfo(CL_DEVICE_TYPE)");

               printf("  Device Type:\t\t\t\t\t ");
               if (dev_type & CL_DEVICE_TYPE_ACCELERATOR)
                  printf("CL_DEVICE_TYPE_ACCELERATOR ");
               else if (dev_type & CL_DEVICE_TYPE_CPU)
                  printf("CL_DEVICE_TYPE_CPU ");
               else if (dev_type & CL_DEVICE_TYPE_GPU)
                  printf("CL_DEVICE_TYPE_GPU ");
               else if (dev_type & CL_DEVICE_TYPE_DEFAULT)
                  printf("CL_DEVICE_TYPE_DEFAULT ");

               printf("\n");

               {
                  cl_uint vendor_id;
                  err = clGetDeviceInfo(devices[j], CL_DEVICE_VENDOR_ID, sizeof(vendor_id), &vendor_id, NULL);
                  checkErr(err, "clGetDeviceInfo(CL_DEVICE_VENDOR_ID)");
                  printf("  Device ID:\t\t\t\t\t %d\n", vendor_id); 
               }
               {
                  cl_uint units;
                  err = clGetDeviceInfo(devices[j], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(units), &units, NULL);
                  checkErr(err, "clGetDeviceInfo(CL_DEVICE_MAX_COMPUTE_UNITS)");
                  printf("  Max compute units:\t\t\t\t %d\n", units); 
               }

               {
                  cl_uint dims;
                  size_t *sizes;
                  err = clGetDeviceInfo(devices[j], CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(dims), &dims, NULL);
                  checkErr(err, "clGetDeviceInfo(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS)");
                  printf("  Max work item dimensions:\t\t\t %d\n", dims); 

                  sizes = (size_t*)malloc(dims * sizeof(size_t));
                  err = clGetDeviceInfo(devices[j], CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(size_t)*dims, sizes, NULL);
                  checkErr(err, "clGetDeviceInfo(CL_DEVICE_MAX_WORK_ITEM_SIZES)");
                  printf("  Max work item dimensions:\t\t\t %d\n", dims); 

                  {
                     unsigned int k;
                     printf("    Max work items:\t\t\t\t (");
                     for (k=0; k<dims; k++) {
                        printf("%u", (unsigned int)sizes[k]);
                        if (k != dims-1)
                           printf(",");
                     }
                     printf(")\n");
                  }
               }

#define GET_SIZET(CL_D,str) { \
   size_t val; \
   err = clGetDeviceInfo(devices[j], CL_D, sizeof(val), &val, NULL); \
   checkErr(err, "clGetDeviceInfo(" #CL_D ")"); \
   printf(str, (unsigned int)val); \
}

#define GET_STRING(CL_D,str,size) { \
   char val[size]; \
   err = clGetDeviceInfo(devices[j], CL_D, sizeof(val), &val, NULL); \
   checkErr(err, "clGetDeviceInfo(" #CL_D ")"); \
   printf(str, val); \
}

#define GET_UINT(CL_D,str) { \
   cl_uint val; \
   err = clGetDeviceInfo(devices[j], CL_D, sizeof(val), &val, NULL); \
   checkErr(err, "clGetDeviceInfo(" #CL_D ")"); \
   printf(str, val); \
}

#define GET_ULONG(CL_D,str) { \
   cl_ulong val; \
   err = clGetDeviceInfo(devices[j], CL_D, sizeof(val), &val, NULL); \
   checkErr(err, "clGetDeviceInfo(" #CL_D ")"); \
   printf(str, val); \
}

#define GET_BOOL(CL_D,str) { \
   cl_bool val; \
   err = clGetDeviceInfo(devices[j], CL_D, sizeof(val), &val, NULL); \
   checkErr(err, "clGetDeviceInfo(" #CL_D ")"); \
   printf(str, (val == CL_TRUE ? "Yes" : "No")); \
}

#define GET_BOOL_CUSTOM(CL_D,str,t,f) { \
   cl_bool val; \
   err = clGetDeviceInfo(devices[j], CL_D, sizeof(val), &val, NULL); \
   checkErr(err, "clGetDeviceInfo(" #CL_D ")"); \
   printf(str, (val == CL_TRUE ? t : f)); \
}

#define GET_BITSET_AND(TYPE,CL_D,test,str) { \
   TYPE val; \
   err = clGetDeviceInfo(devices[j], CL_D, sizeof(val), &val, NULL); \
   checkErr(err, "clGetDeviceInfo(" #CL_D ")"); \
   printf(str, ((val & test) == CL_TRUE ? "Yes" : "No")); \
}
      
               GET_SIZET(CL_DEVICE_MAX_WORK_GROUP_SIZE, "  Max work group size:\t\t\t\t %u\n")
               
               GET_UINT(CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR, "  Preferred vector width char:\t\t\t %u\n")
               GET_UINT(CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT, "  Preferred vector width short:\t\t\t %u\n")
               GET_UINT(CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT, "  Preferred vector width int:\t\t\t %u\n")
               GET_UINT(CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG, "  Preferred vector width long:\t\t\t %u\n")
               GET_UINT(CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT, "  Preferred vector width float:\t\t\t %u\n")
               GET_UINT(CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE, "  Preferred vector width double:\t\t %u\n")
               GET_UINT(CL_DEVICE_MAX_CLOCK_FREQUENCY, "  Max clock frequency:\t\t\t\t %uMHz\n")
               GET_UINT(CL_DEVICE_ADDRESS_BITS, "  Address bits:\t\t\t\t\t %ubits\n")
               GET_ULONG(CL_DEVICE_MAX_MEM_ALLOC_SIZE, "  Max memory allocation:\t\t\t %lu bytes\n")

               GET_BOOL(CL_DEVICE_IMAGE_SUPPORT, "  Image support:\t\t\t\t %s\n")

               GET_SIZET(CL_DEVICE_MAX_PARAMETER_SIZE, "  Max size of kernel argument:\t\t\t %u\n")
               GET_UINT(CL_DEVICE_MEM_BASE_ADDR_ALIGN, "  Alignment of base addres:\t\t\t %u bits\n")
               GET_UINT(CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE, "  Minimum alignment for any datatype:\t\t %u bytes\n")

               printf("  Single precision floating point capability\n");
               GET_BITSET_AND(cl_device_fp_config,CL_DEVICE_SINGLE_FP_CONFIG, CL_FP_DENORM, "    Denorms:\t\t\t\t\t %s\n")
               GET_BITSET_AND(cl_device_fp_config,CL_DEVICE_SINGLE_FP_CONFIG, CL_FP_INF_NAN, "    Quiet NaNs:\t\t\t\t\t %s\n")
               GET_BITSET_AND(cl_device_fp_config,CL_DEVICE_SINGLE_FP_CONFIG, CL_FP_ROUND_TO_NEAREST, "    Round to nearest even:\t\t\t %s\n")
               GET_BITSET_AND(cl_device_fp_config,CL_DEVICE_SINGLE_FP_CONFIG, CL_FP_ROUND_TO_ZERO, "    Round to zero:\t\t\t\t %s\n")
               GET_BITSET_AND(cl_device_fp_config,CL_DEVICE_SINGLE_FP_CONFIG, CL_FP_ROUND_TO_INF, "    Round to +ve and infinity:\t\t\t %s\n")
               GET_BITSET_AND(cl_device_fp_config,CL_DEVICE_SINGLE_FP_CONFIG, CL_FP_FMA, "    IEEE754-2008 fused multiply-add:\t\t %s\n")

               {
                  cl_device_mem_cache_type cache;
                  err = clGetDeviceInfo(devices[j], CL_DEVICE_GLOBAL_MEM_CACHE_TYPE, sizeof(cache), &cache, NULL);
                  checkErr(err, "clGetDeviceInfo(CL_DEVICE_GLOBAL_MEM_CACHE_TYPE)");
                  printf("  Cache type:\t\t\t\t\t ");
                  switch (cache) {
                     case CL_NONE:
                        printf("None\n");
                        break;
                     case CL_READ_ONLY_CACHE:
                        printf("Read only\n");
                        break;
                     case CL_READ_WRITE_CACHE:
                        printf("Read/Write\n");
                        break;
                  }
               }

               GET_UINT(CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE, "  Cache line size:\t\t\t\t %u bytes\n")
               GET_ULONG(CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, "  Cache size:\t\t\t\t\t %lu bytes\n")
               GET_ULONG(CL_DEVICE_GLOBAL_MEM_SIZE, "  Global memory size:\t\t\t\t %lu bytes\n")
               GET_ULONG(CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, "  Constant buffer size:\t\t\t\t %lu bytes\n")
               GET_UINT(CL_DEVICE_MAX_CONSTANT_ARGS, "  Max number of constant args:\t\t\t %u\n")

               {
                  cl_device_local_mem_type cache;
                  err = clGetDeviceInfo(devices[j], CL_DEVICE_LOCAL_MEM_TYPE, sizeof(cache), &cache, NULL);
                  checkErr(err, "clGetDeviceInfo(CL_DEVICE_LOCAL_MEM_TYPE)");
                  printf("  Local memory type:\t\t\t\t ");
                  switch (cache) {
                     case CL_LOCAL:
                        printf("Local\n");
                        break;
                     case CL_GLOBAL:
                        printf("Global\n");
                        break;
                  }
               }

               GET_ULONG(CL_DEVICE_LOCAL_MEM_SIZE, "  Local memory size:\t\t\t\t %lu bytes\n")
               GET_SIZET(CL_DEVICE_PROFILING_TIMER_RESOLUTION, "  Profiling timer resolution:\t\t\t %u\n")
               GET_BOOL_CUSTOM(CL_DEVICE_ENDIAN_LITTLE, "  Device endianess:\t\t\t\t %s\n", "Little", "Big")
               GET_BOOL(CL_DEVICE_AVAILABLE, "  Available:\t\t\t\t\t %s\n")
               GET_BOOL(CL_DEVICE_COMPILER_AVAILABLE, "  Compiler available:\t\t\t\t %s\n")

               printf("  Execution capabilities:\t\t\t\t \n");
               GET_BITSET_AND(cl_device_exec_capabilities, CL_DEVICE_EXECUTION_CAPABILITIES, CL_EXEC_KERNEL, "  Execute OpenCL kernels:\t\t\t %s\n")
               GET_BITSET_AND(cl_device_exec_capabilities, CL_DEVICE_EXECUTION_CAPABILITIES, CL_EXEC_NATIVE_KERNEL, "  Execute native kernels:\t\t\t %s\n")

               printf("  Queue properties:\t\t\t\t\n ");
               GET_BITSET_AND(cl_command_queue_properties, CL_DEVICE_QUEUE_PROPERTIES, CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, "   Out-of-Order:\t\t\t\t %s\n")
               GET_BITSET_AND(cl_command_queue_properties, CL_DEVICE_QUEUE_PROPERTIES, CL_QUEUE_PROFILING_ENABLE, "    Profiling:\t\t\t\t\t %s\n")


               GET_STRING(CL_DEVICE_NAME, "  Name:\t\t\t\t\t\t %s\n", 256);
               GET_STRING(CL_DEVICE_VENDOR, "  Vendor:\t\t\t\t\t %s\n", 256);
               GET_STRING(CL_DRIVER_VERSION, "  Driver version:\t\t\t\t %s\n", 10);
               GET_STRING(CL_DEVICE_PROFILE, "  Profile:\t\t\t\t\t %s\n", 30);
               GET_STRING(CL_DEVICE_VERSION, "  Version:\t\t\t\t\t %s\n", 50);
               GET_STRING(CL_DEVICE_EXTENSIONS, "  Extensions:\t\t\t\t\t %s\n", 4096);
            
               printf("\n");
            }
         }
      }
   }

   return 0;
}
