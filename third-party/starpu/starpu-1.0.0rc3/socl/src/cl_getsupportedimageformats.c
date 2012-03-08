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

CL_API_ENTRY cl_int CL_API_CALL
soclGetSupportedImageFormats(cl_context           UNUSED(context),
                           cl_mem_flags         UNUSED(flags),
                           cl_mem_object_type   UNUSED(image_type),
                           cl_uint              UNUSED(num_entries),
                           cl_image_format *    UNUSED(image_formats),
                           cl_uint *            UNUSED(num_image_formats)) CL_API_SUFFIX__VERSION_1_0
{
   return CL_INVALID_OPERATION;
}
