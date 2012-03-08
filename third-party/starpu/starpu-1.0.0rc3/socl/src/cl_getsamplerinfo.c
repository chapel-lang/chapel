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

CL_API_ENTRY cl_int CL_API_CALL
soclGetSamplerInfo(cl_sampler       UNUSED(sampler),
                 cl_sampler_info    UNUSED(param_name),
                 size_t             UNUSED(param_value_size),
                 void *             UNUSED(param_value),
                 size_t *           UNUSED(param_value_size_ret)) CL_API_SUFFIX__VERSION_1_0
{
   return CL_INVALID_OPERATION;
}
