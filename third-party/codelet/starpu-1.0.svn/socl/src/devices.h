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

#ifndef SOCL_DEVICES_H
#define SOCL_DEVICES_H

// OpenCL 1.0 : Mandatory format: major_number.minor_number
const char * SOCL_DRIVER_VERSION;

const cl_uint SOCL_DEVICE_VENDOR_ID;

struct _cl_device_id {
   cl_device_type    type;
   cl_uint           max_compute_units;        //OpenCL 1.0: minimum value is 1
   cl_uint           max_work_item_dimensions; //OpenCL 1.0: minimum value is 3
   size_t            max_work_item_sizes[3];   //array size should be set accordingly to the maximum max_work_item_dimensions
   size_t            max_work_group_size;      //OpenCL 1.0: minimum value is 1
   cl_uint           preferred_vector_widths[6]; //Char, Short, Int, Long, Float, Double
   cl_uint           max_clock_frequency;
   cl_uint           address_bits;             //OpenCL 1.0: 32 or 64
   cl_ulong          max_mem_alloc_size;       //OpenCL 1.0: minimum value is max(CL_DEVICE_GLOBAL_MEM_SIZE/4, 128*1024*1024)
   cl_bool           image_support;
   //image fields not present
   size_t            max_parameter_size;       //OpenCL 1.0: minimum is 256
   cl_uint           mem_base_addr_align;
   cl_uint           min_data_type_align_size;
   cl_device_fp_config single_fp_config;       //OpenCL 1.0: CL_FP_ROUND_TO_NEAREST and CL_FP_INF_NAN are mandatory
   cl_device_mem_cache_type global_mem_cache_type;
   cl_uint           global_mem_cacheline_size;
   cl_ulong          global_mem_cache_size;
   cl_ulong          global_mem_size;
   cl_ulong          max_constant_buffer_size; //OpenCL 1.0: minimum value is 64KB
   cl_uint           max_constant_args;        //OpenCL 1.0: minimum value is 8
   cl_device_local_mem_type local_mem_type;
   cl_ulong          local_mem_size;           //OpenCL 1.0: minimum value is 16KB
   cl_bool           error_correction_support;
   size_t            profiling_timer_resolution;
   cl_bool           endian_little;
   cl_bool           available;
   cl_bool           compiler_available;       //OpenCL 1.0: mandatory for FULL_PROFILE platforms
   cl_device_exec_capabilities execution_capabilities; //OpenCL 1.0: CL_EXEC_KERNEL is mandatory
   cl_command_queue_properties queue_properties; //OpenCL 1.0: CL_QUEUE_PROFILING_ENABLE is mandatory
   char              name[40];                 //Array size has been arbitrarily defined
   //versions, profile and vendor are statically defined for all devices
   char              extensions[100];          //Array size has been arbitrarily defined
};

const struct _cl_device_id socl_devices[100];

const int socl_device_count;

#endif /* SOCL_DEVICES_H */
