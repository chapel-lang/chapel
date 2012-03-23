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

#ifndef SOCL_GETINFO_H
#define SOCL_GETINFO_H

#define INFO_CASE_EX2(var) if (param_value != NULL) { \
      if (param_value_size < sizeof(var)) \
         return CL_INVALID_VALUE; \
      memcpy(param_value, &var, sizeof(var)); \
   } \
   if (param_value_size_ret != NULL) \
      *param_value_size_ret = sizeof(var); \
   break;

#define INFO_CASE(param, var) case param: \
   INFO_CASE_EX2(var)

#define INFO_CASE_STRING_EX2(var) if (param_value != NULL) { \
      if (param_value_size < strlen(var)) \
         return CL_INVALID_VALUE; \
      strcpy(param_value, var); \
   } \
   if (param_value_size_ret != NULL) \
      *param_value_size_ret = strlen(var); \
   break;

#define INFO_CASE_STRING(param, var) case param: \
   INFO_CASE_STRING_EX2(var)

#define INFO_CASE_VALUE(param, type, value) case param: {\
      type tmp = (value);\
      INFO_CASE_EX2(tmp);\
   }

//warning: var is a reference
#define INFO_CASE_EX(param, var, size) case param: \
   if (param_value != NULL) { \
      if (param_value_size < size) \
         return CL_INVALID_VALUE; \
      memcpy(param_value, var, size); \
   } \
   if (param_value_size_ret != NULL) \
      *param_value_size_ret = size; \
   break;

#endif /* SOCL_GETINFO_H */
