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

#include "socl.h"
#include "getinfo.h"


/**
 * \brief Return dummy infos
 */
CL_API_ENTRY cl_int CL_API_CALL
soclGetDeviceInfo(cl_device_id    device,
                cl_device_info  param_name, 
                size_t          param_value_size, 
                void *          param_value,
                size_t *        param_value_size_ret) CL_API_SUFFIX__VERSION_1_0 
{
   int i, found=0;
   for (i=0; i<socl_device_count; i++) {
      if (device == &socl_devices[i]) {
        found = 1;
        break;
      }
   }

   if (!found)
      return CL_INVALID_DEVICE;

   switch (param_name) {
      INFO_CASE(CL_DEVICE_TYPE, device->type)
      INFO_CASE(CL_DEVICE_VENDOR_ID, SOCL_DEVICE_VENDOR_ID)
      INFO_CASE(CL_DEVICE_MAX_COMPUTE_UNITS, device->max_compute_units)
      INFO_CASE(CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, device->max_work_item_dimensions)
      INFO_CASE(CL_DEVICE_MAX_WORK_ITEM_SIZES, device->max_work_item_sizes)
      INFO_CASE(CL_DEVICE_MAX_WORK_GROUP_SIZE, device->max_work_group_size)
      INFO_CASE(CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR, device->preferred_vector_widths[0])
      INFO_CASE(CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT, device->preferred_vector_widths[1])
      INFO_CASE(CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT, device->preferred_vector_widths[2])
      INFO_CASE(CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG, device->preferred_vector_widths[3])
      INFO_CASE(CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT, device->preferred_vector_widths[4])
      INFO_CASE(CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE, device->preferred_vector_widths[5])
      INFO_CASE(CL_DEVICE_MAX_CLOCK_FREQUENCY, device->max_clock_frequency)
      INFO_CASE(CL_DEVICE_ADDRESS_BITS, device->address_bits)
      INFO_CASE(CL_DEVICE_MAX_MEM_ALLOC_SIZE, device->max_mem_alloc_size)
      INFO_CASE(CL_DEVICE_IMAGE_SUPPORT, device->image_support)
      INFO_CASE(CL_DEVICE_MAX_PARAMETER_SIZE, device->max_parameter_size)
      INFO_CASE(CL_DEVICE_MEM_BASE_ADDR_ALIGN, device->mem_base_addr_align)
      INFO_CASE(CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE, device->min_data_type_align_size)
      INFO_CASE(CL_DEVICE_SINGLE_FP_CONFIG, device->single_fp_config)
      INFO_CASE(CL_DEVICE_GLOBAL_MEM_CACHE_TYPE, device->global_mem_cache_type)
      INFO_CASE(CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE, device->global_mem_cacheline_size)
      INFO_CASE(CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, device->global_mem_cache_size)
      INFO_CASE(CL_DEVICE_GLOBAL_MEM_SIZE, device->global_mem_size)
      INFO_CASE(CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, device->max_constant_buffer_size)
      INFO_CASE(CL_DEVICE_MAX_CONSTANT_ARGS, device->max_constant_args)
      INFO_CASE(CL_DEVICE_LOCAL_MEM_TYPE, device->local_mem_type)
      INFO_CASE(CL_DEVICE_LOCAL_MEM_SIZE, device->local_mem_size)
      INFO_CASE(CL_DEVICE_ERROR_CORRECTION_SUPPORT, device->error_correction_support)
      INFO_CASE(CL_DEVICE_PROFILING_TIMER_RESOLUTION, device->profiling_timer_resolution)
      INFO_CASE(CL_DEVICE_ENDIAN_LITTLE, device->endian_little)
      INFO_CASE(CL_DEVICE_AVAILABLE, device->available)
      INFO_CASE(CL_DEVICE_COMPILER_AVAILABLE, device->compiler_available)
      INFO_CASE(CL_DEVICE_EXECUTION_CAPABILITIES, device->execution_capabilities)
      INFO_CASE(CL_DEVICE_QUEUE_PROPERTIES, device->queue_properties)
      case CL_DEVICE_PLATFORM: {
         cl_platform_id p = &socl_platform;
         INFO_CASE_EX2(p);
      }
      INFO_CASE_STRING(CL_DEVICE_NAME, device->name)
      INFO_CASE_STRING(CL_DEVICE_VENDOR, SOCL_VENDOR)
      INFO_CASE_STRING(CL_DRIVER_VERSION, SOCL_DRIVER_VERSION)
      INFO_CASE_STRING(CL_DEVICE_PROFILE, SOCL_PROFILE)
      INFO_CASE_STRING(CL_DEVICE_VERSION, SOCL_VERSION)
      INFO_CASE_STRING(CL_DEVICE_EXTENSIONS, device->extensions)
      default:
         return CL_INVALID_VALUE;
   }

   return CL_SUCCESS;
}
