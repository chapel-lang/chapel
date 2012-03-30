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


/**
 * \brief Return one device of each kind
 *
 * \param[in] platform Must be StarPU platform ID or NULL
 */
CL_API_ENTRY cl_int CL_API_CALL
soclGetDeviceIDs(cl_platform_id   platform,
               cl_device_type   device_type, 
               cl_uint          num_entries, 
               cl_device_id *   devices, 
               cl_uint *        num_devices) CL_API_SUFFIX__VERSION_1_0
{
   if (platform != NULL && platform != &socl_platform)
      return CL_INVALID_PLATFORM;

   if ((devices != NULL && num_entries == 0)
      || (devices == NULL && num_devices == NULL))
      return CL_INVALID_VALUE;

   if (!(device_type & (CL_DEVICE_TYPE_CPU | CL_DEVICE_TYPE_GPU | CL_DEVICE_TYPE_ACCELERATOR | CL_DEVICE_TYPE_DEFAULT))
      && (device_type != CL_DEVICE_TYPE_ALL))
      return CL_INVALID_DEVICE_TYPE;

   {
      int i;
      unsigned int num = 0;
      for (i=0; i<socl_device_count; i++) {
         if (socl_devices[i].type & device_type) {
            if (devices != NULL && num < num_entries)
               devices[num] = (cl_device_id)&socl_devices[i];
            num++;
         }
      }
      if (num_devices != NULL)
         *num_devices = num;
   }

   return CL_SUCCESS;
}
