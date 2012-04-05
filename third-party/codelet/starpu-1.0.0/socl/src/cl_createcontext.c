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

static void release_callback_context(void * e) {
  cl_context context = (cl_context)e;

  /* Destruct object */
  if (context->properties != NULL)
    free(context->properties);

  free(context->devices);
}



CL_API_ENTRY cl_context CL_API_CALL
soclCreateContext(const cl_context_properties * properties,
                cl_uint                       num_devices,
                const cl_device_id *          devices,
                void (*pfn_notify)(const char *, const void *, size_t, void *),
                void *                        user_data,
                cl_int *                      errcode_ret) CL_API_SUFFIX__VERSION_1_0
{

   if (pfn_notify == NULL && user_data != NULL) {
      if (errcode_ret != NULL)
         *errcode_ret = CL_INVALID_VALUE;
      return NULL;
   }

   //Check properties
   if (properties != NULL) {
      const cl_context_properties *p = properties;
      int i = 0;
      while (p[i] != 0) {
         switch (p[i]) {
            case CL_CONTEXT_PLATFORM:
               i++;
               if (p[i] != (cl_context_properties)&socl_platform) {
                  if (errcode_ret != NULL)
                     *errcode_ret = CL_INVALID_PLATFORM;
                  return NULL;
               }
               break;
         }
         i++;
      }
   }


   cl_context ctx;
   ctx = (cl_context)gc_entity_alloc(sizeof(struct _cl_context), release_callback_context);
   if (ctx == NULL) {
      if (errcode_ret != NULL)
         *errcode_ret = CL_OUT_OF_HOST_MEMORY;
      return NULL;
   }

   ctx->num_properties = 0;
   ctx->properties = NULL;

   // Cache properties
   if (properties != NULL) {
      //Count properties
      const cl_context_properties * p = properties;
      do {
         ctx->num_properties++;
         p++;
      } while (*p != 0);

      //Copy properties
      ctx->properties = malloc(sizeof(cl_context_properties) * ctx->num_properties);
      memcpy(ctx->properties, properties, sizeof(cl_context_properties) * ctx->num_properties);
   }

   ctx->pfn_notify = pfn_notify;
   ctx->user_data = user_data;
   ctx->num_devices = num_devices;

   #ifdef DEBUG
   static int id = 0;
   ctx->id = id++;
   #endif

   ctx->devices = malloc(sizeof(cl_device_id) * num_devices);
   memcpy(ctx->devices, devices, sizeof(cl_device_id)*num_devices);

   if (errcode_ret != NULL)
      *errcode_ret = CL_SUCCESS;

   return ctx;
}
