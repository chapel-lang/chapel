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

#ifndef SOCL_H
#define SOCL_H

#ifndef CL_HEADERS
#include "CL/cl.h"
#else
#include CL_HEADERS "CL/cl.h"
#endif

/* Additional command type */
#define CL_COMMAND_BARRIER 0x99987

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>

#include <starpu.h>
#include <starpu_opencl.h>
#include <starpu_data_interfaces.h>
#include <starpu_profiling.h>
#include <starpu_task.h>

typedef struct starpu_task * starpu_task;

#ifdef UNUSED
#elif defined(__GNUC__)
   #define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#else
   #define UNUSED(x) x
#endif


/**
 * Entity that can be managed by the garbage collector
 */
typedef struct entity * entity;

#include "command.h"
#include "command_list.h"
#include "command_queue.h"
#include "debug.h"
#include "devices.h"
#include "event.h"
#include "gc.h"
#include "mem_objects.h"
#include "task.h"
#include "util.h"


struct entity {
  /* Reference count */
  size_t refs;

  /* Callback called on release */
  void (*release_callback)(void*entity);

  /* Next entity in garbage collector queue */
  entity prev;
  entity next;
};

/* OpenCL entities (context, command queues, buffers...) must use
 * this macro as their first field */
#define CL_ENTITY struct entity _entity;

struct _cl_platform_id {};

#define RETURN_EVENT(cmd, event) \
	if (event != NULL) { \
		cl_event ev = command_event_get(cmd);\
		gc_entity_retain(ev);\
		*event = ev; \
	}

#define RETURN_CUSTOM_EVENT(src, tgt) \
	if (tgt != NULL) { \
		gc_entity_retain(src); \
		*tgt = src; \
	}

#define MAY_BLOCK(blocking) \
	if ((blocking) == CL_TRUE) {\
		cl_event ev = command_event_get(cmd);\
		soclWaitForEvents(1, &ev);\
		gc_entity_release(ev);\
	}

#define MAY_BLOCK_CUSTOM(blocking,event) \
	if ((blocking) == CL_TRUE) {\
		cl_event ev = (event);\
		soclWaitForEvents(1, &ev);\
		gc_entity_release(ev);\
	}

/* Constants */
struct _cl_platform_id socl_platform;
const char * SOCL_PROFILE;
const char * SOCL_VERSION;
const char * SOCL_PLATFORM_NAME;
const char * SOCL_VENDOR;
const char * SOCL_PLATFORM_EXTENSIONS;

struct _cl_context {
  CL_ENTITY;

  void (*pfn_notify)(const char *, const void *, size_t, void *);
  void *user_data;

  /* Associated devices */
  cl_device_id * devices;
  cl_uint num_devices;

  /* Properties */
  cl_context_properties * properties;
  cl_uint num_properties;

  /* ID  */
#ifdef DEBUG
  int id;
#endif
};


struct _cl_command_queue {
  CL_ENTITY;

  cl_command_queue_properties properties;
  cl_device_id device;
  cl_context context;

  /* Stored commands */
  command_list commands;

  /* Last enqueued barrier-like event */
  cl_command barrier;

  /* Mutex */
  pthread_mutex_t mutex;

  /* ID  */
#ifdef DEBUG
  int id;
#endif
};

struct _cl_event {
  CL_ENTITY;

  /* Command queue */
  cl_command_queue cq;

  /* Command */
  cl_command command;

  /* Event status */
  cl_int status;

  /* ID  
   * This ID is used as a tag for StarPU dependencies
   */
  int id;

  /* Profiling info are copied here */
  struct starpu_task_profiling_info *profiling_info;
};

struct _cl_mem {
  CL_ENTITY;

  /* StarPU handle */
  starpu_data_handle_t handle;

  /* Pointer to data in host memory */
  void *ptr;    

  /* Buffer size */
  size_t size;

  /* Indicates how many references (mapping, MEM_USE_HOST_PTR...) require
   * coherence in host memory. If set to zero, no coherency is maintained
   * (this is the most efficient) */
  int map_count; 

  /* Creation flags */
  cl_mem_flags flags;

  /* Creation context */
  cl_context context;

  /* Access mode */
  int mode;

  /* Host ptr */
  void * host_ptr;

  /* Fields used to store cl_mems in mem_objects list */
  cl_mem prev;
  cl_mem next;

  /* Indicates if a buffer may contain meaningful data. Otherwise
     we don't have to transfer it */
  int scratch;

  /* ID  */
#ifdef DEBUG
  int id;
#endif
};

struct _cl_program {
  CL_ENTITY;

  /* Real OpenCL Programs
   * There is one entry for each device (even non OpenCL ones)
   * in order to index this array with dev_id
   */
  cl_program *cl_programs;

  /* Context used to create this program */
  cl_context context;

  /* Options  */
  char * options;
  unsigned int options_size;

  /* ID  */
#ifdef DEBUG
  int id;
#endif
};

enum kernel_arg_type { Null, Buffer, Immediate };

struct _cl_kernel {
  CL_ENTITY;

  /* Associated program */
  cl_program program;

  /* Kernel name */
  char * kernel_name;

  /* Real OpenCL kernels */
  cl_kernel *cl_kernels;

  /* clCreateKernel return codes */
  cl_int *errcodes;

  /* Arguments */
  unsigned int num_args;
  size_t *arg_size;
  enum kernel_arg_type  *arg_type;
  void  **arg_value;

  /* ID  */
#ifdef DEBUG
  int id;
#endif
};

/* Global vars */

/* Command queues with profiling enabled
 * This allows us to disable StarPU profiling it
 * is equal to 0
 */
int profiling_queue_count;

/***************************************************************************/

/* Platform API */
extern CL_API_ENTRY cl_int CL_API_CALL
soclGetPlatformIDs(cl_uint          /* num_entries */,
                 cl_platform_id * /* platforms */,
                 cl_uint *        /* num_platforms */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL 
soclGetPlatformInfo(cl_platform_id   /* platform */, 
                  cl_platform_info /* param_name */,
                  size_t           /* param_value_size */, 
                  void *           /* param_value */,
                  size_t *         /* param_value_size_ret */) CL_API_SUFFIX__VERSION_1_0;

/* Device APIs */
extern CL_API_ENTRY cl_int CL_API_CALL
soclGetDeviceIDs(cl_platform_id   /* platform */,
               cl_device_type   /* device_type */, 
               cl_uint          /* num_entries */, 
               cl_device_id *   /* devices */, 
               cl_uint *        /* num_devices */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclGetDeviceInfo(cl_device_id    /* device */,
                cl_device_info  /* param_name */, 
                size_t          /* param_value_size */, 
                void *          /* param_value */,
                size_t *        /* param_value_size_ret */) CL_API_SUFFIX__VERSION_1_0;

/* Context APIs  */
extern CL_API_ENTRY cl_context CL_API_CALL
soclCreateContext(const cl_context_properties * /* properties */,
                cl_uint                       /* num_devices */,
                const cl_device_id *          /* devices */,
                void (*pfn_notify)(const char *, const void *, size_t, void *) /* pfn_notify */,
                void *                        /* user_data */,
                cl_int *                      /* errcode_ret */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_context CL_API_CALL
soclCreateContextFromType(const cl_context_properties * /* properties */,
                        cl_device_type                /* device_type */,
                        void (*pfn_notify)(const char *, const void *, size_t, void *) /* pfn_notify */,
                        void *                        /* user_data */,
                        cl_int *                      /* errcode_ret */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclRetainContext(cl_context /* context */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclReleaseContext(cl_context /* context */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclGetContextInfo(cl_context         /* context */, 
                 cl_context_info    /* param_name */, 
                 size_t             /* param_value_size */, 
                 void *             /* param_value */, 
                 size_t *           /* param_value_size_ret */) CL_API_SUFFIX__VERSION_1_0;

/* Command Queue APIs */
extern CL_API_ENTRY cl_command_queue CL_API_CALL
soclCreateCommandQueue(cl_context                     /* context */, 
                     cl_device_id                   /* device */, 
                     cl_command_queue_properties    /* properties */,
                     cl_int *                       /* errcode_ret */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclRetainCommandQueue(cl_command_queue /* command_queue */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclReleaseCommandQueue(cl_command_queue /* command_queue */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclGetCommandQueueInfo(cl_command_queue      /* command_queue */,
                      cl_command_queue_info /* param_name */,
                      size_t                /* param_value_size */,
                      void *                /* param_value */,
                      size_t *              /* param_value_size_ret */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclSetCommandQueueProperty(cl_command_queue              /* command_queue */,
                          cl_command_queue_properties   /* properties */, 
                          cl_bool                        /* enable */,
                          cl_command_queue_properties * /* old_properties */) CL_API_SUFFIX__VERSION_1_0;

/* Memory Object APIs  */
extern CL_API_ENTRY cl_mem CL_API_CALL
soclCreateBuffer(cl_context   /* context */,
               cl_mem_flags /* flags */,
               size_t       /* size */,
               void *       /* host_ptr */,
               cl_int *     /* errcode_ret */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_mem CL_API_CALL
soclCreateImage2D(cl_context              /* context */,
                cl_mem_flags            /* flags */,
                const cl_image_format * /* image_format */,
                size_t                  /* image_width */,
                size_t                  /* image_height */,
                size_t                  /* image_row_pitch */, 
                void *                  /* host_ptr */,
                cl_int *                /* errcode_ret */) CL_API_SUFFIX__VERSION_1_0;
                        
extern CL_API_ENTRY cl_mem CL_API_CALL
soclCreateImage3D(cl_context              /* context */,
                cl_mem_flags            /* flags */,
                const cl_image_format * /* image_format */,
                size_t                  /* image_width */, 
                size_t                  /* image_height */,
                size_t                  /* image_depth */, 
                size_t                  /* image_row_pitch */, 
                size_t                  /* image_slice_pitch */, 
                void *                  /* host_ptr */,
                cl_int *                /* errcode_ret */) CL_API_SUFFIX__VERSION_1_0;
                        
extern CL_API_ENTRY cl_int CL_API_CALL
soclRetainMemObject(cl_mem /* memobj */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclReleaseMemObject(cl_mem /* memobj */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclGetSupportedImageFormats(cl_context           /* context */,
                           cl_mem_flags         /* flags */,
                           cl_mem_object_type   /* image_type */,
                           cl_uint              /* num_entries */,
                           cl_image_format *    /* image_formats */,
                           cl_uint *            /* num_image_formats */) CL_API_SUFFIX__VERSION_1_0;
                                    
extern CL_API_ENTRY cl_int CL_API_CALL
soclGetMemObjectInfo(cl_mem           /* memobj */,
                   cl_mem_info      /* param_name */, 
                   size_t           /* param_value_size */,
                   void *           /* param_value */,
                   size_t *         /* param_value_size_ret */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclGetImageInfo(cl_mem           /* image */,
               cl_image_info    /* param_name */, 
               size_t           /* param_value_size */,
               void *           /* param_value */,
               size_t *         /* param_value_size_ret */) CL_API_SUFFIX__VERSION_1_0;

/* Sampler APIs  */
extern CL_API_ENTRY cl_sampler CL_API_CALL
soclCreateSampler(cl_context          /* context */,
                cl_bool             /* normalized_coords */, 
                cl_addressing_mode  /* addressing_mode */, 
                cl_filter_mode      /* filter_mode */,
                cl_int *            /* errcode_ret */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclRetainSampler(cl_sampler /* sampler */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclReleaseSampler(cl_sampler /* sampler */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclGetSamplerInfo(cl_sampler         /* sampler */,
                 cl_sampler_info    /* param_name */,
                 size_t             /* param_value_size */,
                 void *             /* param_value */,
                 size_t *           /* param_value_size_ret */) CL_API_SUFFIX__VERSION_1_0;
                            
/* Program Object APIs  */
extern CL_API_ENTRY cl_program CL_API_CALL
soclCreateProgramWithSource(cl_context        /* context */,
                          cl_uint           /* count */,
                          const char **     /* strings */,
                          const size_t *    /* lengths */,
                          cl_int *          /* errcode_ret */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_program CL_API_CALL
soclCreateProgramWithBinary(cl_context                     /* context */,
                          cl_uint                        /* num_devices */,
                          const cl_device_id *           /* device_list */,
                          const size_t *                 /* lengths */,
                          const unsigned char **         /* binaries */,
                          cl_int *                       /* binary_status */,
                          cl_int *                       /* errcode_ret */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclRetainProgram(cl_program /* program */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclReleaseProgram(cl_program /* program */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclBuildProgram(cl_program           /* program */,
               cl_uint              /* num_devices */,
               const cl_device_id * /* device_list */,
               const char *         /* options */, 
               void (*pfn_notify)(cl_program /* program */, void * /* user_data */),
               void *               /* user_data */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclUnloadCompiler(void) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclGetProgramInfo(cl_program         /* program */,
                 cl_program_info    /* param_name */,
                 size_t             /* param_value_size */,
                 void *             /* param_value */,
                 size_t *           /* param_value_size_ret */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclGetProgramBuildInfo(cl_program            /* program */,
                      cl_device_id          /* device */,
                      cl_program_build_info /* param_name */,
                      size_t                /* param_value_size */,
                      void *                /* param_value */,
                      size_t *              /* param_value_size_ret */) CL_API_SUFFIX__VERSION_1_0;
                            
/* Kernel Object APIs */
extern CL_API_ENTRY cl_kernel CL_API_CALL
soclCreateKernel(cl_program      /* program */,
               const char *    /* kernel_name */,
               cl_int *        /* errcode_ret */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclCreateKernelsInProgram(cl_program     /* program */,
                         cl_uint        /* num_kernels */,
                         cl_kernel *    /* kernels */,
                         cl_uint *      /* num_kernels_ret */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclRetainKernel(cl_kernel    /* kernel */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclReleaseKernel(cl_kernel   /* kernel */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclSetKernelArg(cl_kernel    /* kernel */,
               cl_uint      /* arg_index */,
               size_t       /* arg_size */,
               const void * /* arg_value */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclGetKernelInfo(cl_kernel       /* kernel */,
                cl_kernel_info  /* param_name */,
                size_t          /* param_value_size */,
                void *          /* param_value */,
                size_t *        /* param_value_size_ret */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclGetKernelWorkGroupInfo(cl_kernel                  /* kernel */,
                         cl_device_id               /* device */,
                         cl_kernel_work_group_info  /* param_name */,
                         size_t                     /* param_value_size */,
                         void *                     /* param_value */,
                         size_t *                   /* param_value_size_ret */) CL_API_SUFFIX__VERSION_1_0;

/* Event Object APIs  */
extern CL_API_ENTRY cl_int CL_API_CALL
soclWaitForEvents(cl_uint             /* num_events */,
                const cl_event *    /* event_list */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclGetEventInfo(cl_event         /* event */,
               cl_event_info    /* param_name */,
               size_t           /* param_value_size */,
               void *           /* param_value */,
               size_t *         /* param_value_size_ret */) CL_API_SUFFIX__VERSION_1_0;
                            
extern CL_API_ENTRY cl_int CL_API_CALL
soclRetainEvent(cl_event /* event */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclReleaseEvent(cl_event /* event */) CL_API_SUFFIX__VERSION_1_0;

/* Profiling APIs  */
extern CL_API_ENTRY cl_int CL_API_CALL
soclGetEventProfilingInfo(cl_event            /* event */,
                        cl_profiling_info   /* param_name */,
                        size_t              /* param_value_size */,
                        void *              /* param_value */,
                        size_t *            /* param_value_size_ret */) CL_API_SUFFIX__VERSION_1_0;
                                
/* Flush and Finish APIs */
extern CL_API_ENTRY cl_int CL_API_CALL
soclFlush(cl_command_queue /* command_queue */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclFinish(cl_command_queue /* command_queue */) CL_API_SUFFIX__VERSION_1_0;

/* Enqueued Commands APIs */
extern CL_API_ENTRY cl_int CL_API_CALL
soclEnqueueReadBuffer(cl_command_queue    /* command_queue */,
                    cl_mem              /* buffer */,
                    cl_bool             /* blocking_read */,
                    size_t              /* offset */,
                    size_t              /* cb */, 
                    void *              /* ptr */,
                    cl_uint             /* num_events_in_wait_list */,
                    const cl_event *    /* event_wait_list */,
                    cl_event *          /* event */) CL_API_SUFFIX__VERSION_1_0;
                            
extern CL_API_ENTRY cl_int CL_API_CALL
soclEnqueueWriteBuffer(cl_command_queue   /* command_queue */, 
                     cl_mem             /* buffer */, 
                     cl_bool            /* blocking_write */, 
                     size_t             /* offset */, 
                     size_t             /* cb */, 
                     const void *       /* ptr */, 
                     cl_uint            /* num_events_in_wait_list */, 
                     const cl_event *   /* event_wait_list */, 
                     cl_event *         /* event */) CL_API_SUFFIX__VERSION_1_0;
                            
extern CL_API_ENTRY cl_int CL_API_CALL
soclEnqueueCopyBuffer(cl_command_queue    /* command_queue */, 
                    cl_mem              /* src_buffer */,
                    cl_mem              /* dst_buffer */, 
                    size_t              /* src_offset */,
                    size_t              /* dst_offset */,
                    size_t              /* cb */, 
                    cl_uint             /* num_events_in_wait_list */,
                    const cl_event *    /* event_wait_list */,
                    cl_event *          /* event */) CL_API_SUFFIX__VERSION_1_0;
                            
extern CL_API_ENTRY cl_int CL_API_CALL
soclEnqueueReadImage(cl_command_queue     /* command_queue */,
                   cl_mem               /* image */,
                   cl_bool              /* blocking_read */, 
                   const size_t *       /* origin[3] */,
                   const size_t *       /* region[3] */,
                   size_t               /* row_pitch */,
                   size_t               /* slice_pitch */, 
                   void *               /* ptr */,
                   cl_uint              /* num_events_in_wait_list */,
                   const cl_event *     /* event_wait_list */,
                   cl_event *           /* event */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclEnqueueWriteImage(cl_command_queue    /* command_queue */,
                    cl_mem              /* image */,
                    cl_bool             /* blocking_write */, 
                    const size_t *      /* origin[3] */,
                    const size_t *      /* region[3] */,
                    size_t              /* input_row_pitch */,
                    size_t              /* input_slice_pitch */, 
                    const void *        /* ptr */,
                    cl_uint             /* num_events_in_wait_list */,
                    const cl_event *    /* event_wait_list */,
                    cl_event *          /* event */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclEnqueueCopyImage(cl_command_queue     /* command_queue */,
                   cl_mem               /* src_image */,
                   cl_mem               /* dst_image */, 
                   const size_t *       /* src_origin[3] */,
                   const size_t *       /* dst_origin[3] */,
                   const size_t *       /* region[3] */, 
                   cl_uint              /* num_events_in_wait_list */,
                   const cl_event *     /* event_wait_list */,
                   cl_event *           /* event */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclEnqueueCopyImageToBuffer(cl_command_queue /* command_queue */,
                           cl_mem           /* src_image */,
                           cl_mem           /* dst_buffer */, 
                           const size_t *   /* src_origin[3] */,
                           const size_t *   /* region[3] */, 
                           size_t           /* dst_offset */,
                           cl_uint          /* num_events_in_wait_list */,
                           const cl_event * /* event_wait_list */,
                           cl_event *       /* event */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclEnqueueCopyBufferToImage(cl_command_queue /* command_queue */,
                           cl_mem           /* src_buffer */,
                           cl_mem           /* dst_image */, 
                           size_t           /* src_offset */,
                           const size_t *   /* dst_origin[3] */,
                           const size_t *   /* region[3] */, 
                           cl_uint          /* num_events_in_wait_list */,
                           const cl_event * /* event_wait_list */,
                           cl_event *       /* event */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY void * CL_API_CALL
soclEnqueueMapBuffer(cl_command_queue /* command_queue */,
                   cl_mem           /* buffer */,
                   cl_bool          /* blocking_map */, 
                   cl_map_flags     /* map_flags */,
                   size_t           /* offset */,
                   size_t           /* cb */,
                   cl_uint          /* num_events_in_wait_list */,
                   const cl_event * /* event_wait_list */,
                   cl_event *       /* event */,
                   cl_int *         /* errcode_ret */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY void * CL_API_CALL
soclEnqueueMapImage(cl_command_queue  /* command_queue */,
                  cl_mem            /* image */, 
                  cl_bool           /* blocking_map */, 
                  cl_map_flags      /* map_flags */, 
                  const size_t *    /* origin[3] */,
                  const size_t *    /* region[3] */,
                  size_t *          /* image_row_pitch */,
                  size_t *          /* image_slice_pitch */,
                  cl_uint           /* num_events_in_wait_list */,
                  const cl_event *  /* event_wait_list */,
                  cl_event *        /* event */,
                  cl_int *          /* errcode_ret */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclEnqueueUnmapMemObject(cl_command_queue /* command_queue */,
                        cl_mem           /* memobj */,
                        void *           /* mapped_ptr */,
                        cl_uint          /* num_events_in_wait_list */,
                        const cl_event *  /* event_wait_list */,
                        cl_event *        /* event */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclEnqueueNDRangeKernel(cl_command_queue /* command_queue */,
                       cl_kernel        /* kernel */,
                       cl_uint          /* work_dim */,
                       const size_t *   /* global_work_offset */,
                       const size_t *   /* global_work_size */,
                       const size_t *   /* local_work_size */,
                       cl_uint          /* num_events_in_wait_list */,
                       const cl_event * /* event_wait_list */,
                       cl_event *       /* event */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclEnqueueTask(cl_command_queue  /* command_queue */,
              cl_kernel         /* kernel */,
              cl_uint           /* num_events_in_wait_list */,
              const cl_event *  /* event_wait_list */,
              cl_event *        /* event */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclEnqueueNativeKernel(cl_command_queue  /* command_queue */,
					  void (*user_func)(void *), 
                      void *            /* args */,
                      size_t            /* cb_args */, 
                      cl_uint           /* num_mem_objects */,
                      const cl_mem *    /* mem_list */,
                      const void **     /* args_mem_loc */,
                      cl_uint           /* num_events_in_wait_list */,
                      const cl_event *  /* event_wait_list */,
                      cl_event *        /* event */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclEnqueueMarker(cl_command_queue    /* command_queue */,
                cl_event *          /* event */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclEnqueueWaitForEvents(cl_command_queue /* command_queue */,
                       cl_uint          /* num_events */,
                       const cl_event * /* event_list */) CL_API_SUFFIX__VERSION_1_0;

extern CL_API_ENTRY cl_int CL_API_CALL
soclEnqueueBarrier(cl_command_queue /* command_queue */) CL_API_SUFFIX__VERSION_1_0;

/* Extension function access
 *
 * Returns the extension function address for the given function name,
 * or NULL if a valid function can not be found.  The client must
 * check to make sure the address is not NULL, before using or 
 * calling the returned function address.
 */
extern CL_API_ENTRY void * CL_API_CALL
soclGetExtensionFunctionAddress(const char * /* func_name */) CL_API_SUFFIX__VERSION_1_0;

#endif /* SOCL_H */
