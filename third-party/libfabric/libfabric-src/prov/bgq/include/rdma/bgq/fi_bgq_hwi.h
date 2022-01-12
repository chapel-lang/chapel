/*
 * Copyright (C) 2016 by Argonne National Laboratory.
 *
 * This software is available to you under a choice of one of two
 * licenses.  You may choose to be licensed under the terms of the GNU
 * General Public License (GPL) Version 2, available from the file
 * COPYING in the main directory of this source tree, or the
 * BSD license below:
 *
 *     Redistribution and use in source and binary forms, with or
 *     without modification, are permitted provided that the following
 *     conditions are met:
 *
 *      - Redistributions of source code must retain the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer.
 *
 *      - Redistributions in binary form must reproduce the above
 *        copyright notice, this list of conditions and the following
 *        disclaimer in the documentation and/or other materials
 *        provided with the distribution.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef _FI_PROV_BGQ_HWI_H_
#define _FI_PROV_BGQ_HWI_H_

/*
 * The bgq system software, specifically 'hwi/include/common/compiler_support.h',
 * will define the __INLINE__ macro if it is not already defined to the following:
 *
 *   #define __INLINE__ extern inline __attribute__((always_inline))
 *
 * This is the non-portable "gnu 89" style which easily results in undefined
 * symbols or multiple defined symbols when used by software coded to a more
 * recent C standard.
 *
 * As a workaround the __INLINE__ macro will be defined to the more appropriate
 * 'static inline' style only for the bgq system software includes and then
 * undefined at the end of this file. This seems to fix the problem without
 * requiring any changes to the installed bgq system software files.
 */
#ifdef __INLINE__
#error __INLINE__ already defined!
#else
#define __INLINE__ static inline
#endif

#include <firmware/include/personality.h>
#include <hwi/include/bqc/A2_inlines.h>
#include <hwi/include/bqc/A2_inlines.h>
#include <hwi/include/bqc/MU_Descriptor.h>
#include <hwi/include/bqc/MU_Macros.h>
#include <hwi/include/bqc/MU_PacketHeader.h>
#include <hwi/include/bqc/nd_500_dcr.h>
#include <hwi/include/common/bgq_alignment.h>

#undef __INLINE__

#endif /* _FI_PROV_BGQ_HWI_H_ */
