/* $Source: /var/local/cvs/gasnet/gm-conduit/gasnet_core_help.h,v $
 * $Date: 2009/09/18 23:33:30 $
 * $Revision: 1.45 $
 * Description: GASNet gm conduit core Header Helpers (Internal code, not for client use)
 * Copyright 2002, Christian Bell <csbell@cs.berkeley.edu>
 * Copyright 2002, Dan Bonachea <bonachea@cs.berkeley.edu>
 * Terms of use are as specified in license.txt
 */

#ifndef _IN_GASNET_H
  #error This file is not meant to be included directly- clients should include gasnet.h
#endif

#ifndef _GASNET_CORE_HELP_H
#define _GASNET_CORE_HELP_H

GASNETI_BEGIN_EXTERNC

#include <gasnet_help.h>
#include <gm.h>

/* -------------------------------------------------------------------------- */
/* Tunable Compile-time Parameters */

/* GASNETC_AM_SIZE (default=16). Log2 size of the AM buffers used in the core.
 *       By default, an amount of 64k buffers equal to the number of GM tokens
 *       are allocated.  This value cannot be less than the MTU where 4096 = 12
 */
   #ifndef GASNETC_AM_SIZE
      #define GASNETC_AM_SIZE 16
   #endif

/*
 * GASNETE_GETPUT_MEDIUM_LONG_THRESHOLD (default=8192). Size at which AMLongs
 * will be preferred to AMMedium for sending payload.  This setting affects the
 * core internals and typically should not be changed.
 */
#ifndef GASNETE_GETPUT_MEDIUM_LONG_THRESHOLD
#define GASNETE_GETPUT_MEDIUM_LONG_THRESHOLD	8192
#endif

/* -------------------------------------------------------------------------- */
/* Non-Tunable Compile-time Parameters */
/*
 * RDMA PUTS
 *
 * GM 2.x API revision supports gm_put as a synonym for
 * gm_directed_send_with_callback
 */
#if defined(GM_API_VERSION_2_0) && GM_API_VERSION >= GM_API_VERSION_2_0
#define GASNETC_GM_2
#define GASNETC_GM_PUT	gm_put
#else
#define GASNETC_GM_PUT	gm_directed_send_with_callback
#endif

/*
 * RDMA GETS
 *
 * Appeared with GM API version 2.x but there are serious problems for versions
 * prior to 2.0.12 in 2.0 series and prior to 2.1.2 in 2.1 series
 *
 * GASNET_GM_RDMA_GETS compile time setting controls the default behavior,
 * which can also be toggled at runtime using the environment variable 
 * of the same name (GASNET_GM_RDMA_GETS)
 *
 * Users can predefine GASNET_GM_RDMA_GETS to whatever they want in order
 * to bypass the automatic defaulting mechanism.
 *
 */

#if defined(GASNETC_GM_2) && /* GM 2.x -- some versions have problems */ \
    ((GM_API_VERSION_2_1_0 && GM_API_VERSION < 0x20102) || GM_API_VERSION < 0x2000c) 
  /* GM gets are this broken on this version - fail to compile unless we've been forced to do so */
  #if !defined(GASNETC_GM_ENABLE_BROKEN_VERSIONS)
    #error GASNet/GM RDMA gets are broken on this GM 2.x version (see gm-conduit README)
  #endif
  #define GASNETC_BROKEN_GM2_VERSION 1
#else
  #define GASNETC_BROKEN_GM2_VERSION 0
#endif

#if defined(GASNETC_GM_2)
  #ifndef GASNET_GM_RDMA_GETS /* gets not explicitly enabled/disabled */
     #if GASNETC_BROKEN_GM2_VERSION
         /* we're being forced to use a broken GM version - so disable gm gets by default */
         #define GASNET_GM_RDMA_GETS 0
     #else 
         /* working versions of GM2 - enable rdma gets by default */
         #define GASNET_GM_RDMA_GETS 1
     #endif
  #endif /* GASNET_GM_RDMA_GETS already defined */
#else /* GM 1.x -- no gets in the API, it's an error if someone tries to use them */
  #if defined(GASNET_GM_RDMA_GETS) && GASNET_GM_RDMA_GETS
    #error GASNET_GM_RDMA_GETS not supported on GM 1.x - no GM RDMA gets!
  #endif
  #undef GASNET_GM_RDMA_GETS
  #define GASNET_GM_RDMA_GETS 0
#endif

#ifndef GASNET_GM_RDMA_GETS
  #error Internal define error
#endif

/* A few more obscure configurable parameters.                                */
#define GASNETC_GM_MAXPORTS	gm_num_ports(NULL)
#define GASNETC_GM_MAXBOARDS	3

#define GASNETC_SEGMENT_ALIGN	GASNET_PAGESIZE

/*
 * GASNETC_DEFAULT_PHYSMEM_PINNABLE_RATIO (float default=0.7).  Which fraction
 *       of the determined physical memory can be made pinnable.  This value is
 *       extracted from Myricom guarantees that 70% of memory can be pinned for
 *       correct operation.  This parameter has a runtime counterpart,
 *       GASNET_PHYSMEM_PINNABLE_RATIO which can be set at runtime. */
   #ifndef GASNETC_DEFAULT_PHYSMEM_PINNABLE_RATIO
     #define GASNETC_DEFAULT_PHYSMEM_PINNABLE_RATIO 0.7
   #endif

/* -------------------------------------------------------------------------- */
/* These should not be modified */
/* AM Header stores the following fields
 *
 * 0b76543210
 *
 * 7-6: AM Message type (00=small, 01=medium, 10=long, 11=system)
 * 5-1: AM Number of arguments (00000=0, 00001=1, 00010=2, ...)
 * 0:   AM Request/Reply (0=reply, 1=request)
 *
 * AM Index stores the handler index and is one byte next to the 
 * Header
 */

#define GASNETC_AM_SHORT	0x00
#define GASNETC_AM_MEDIUM	0x40
#define GASNETC_AM_LONG		0x80
#define GASNETC_AM_SYSTEM	0xc0

#define	GASNETC_AM_REPLY	0x00
#define GASNETC_AM_REQUEST	0x01

#define GASNETC_AM_LEN		(1<<GASNETC_AM_SIZE)
#define GASNETC_AM_PACKET	(GASNETC_AM_LEN-8)

#define GASNETC_LONG_OFFSET	GASNET_PAGESIZE

#define GASNETC_AM_MAX_ARGS	16
#define GASNETC_AM_MAX_HANDLERS 256

#define GASNETC_AM_SHORT_ARGS_OFF	4
#define GASNETC_AM_MEDIUM_ARGS_OFF	4
#define GASNETC_AM_SYSTEM_ARGS_OFF	4
#define GASNETC_AM_LONG_ARGS_OFF	(8+sizeof(uintptr_t))

#define GASNETC_AM_MEDIUM_HEADER_PAD(numargs) ((((numargs)&0x1)==1) ? 0 : 4)
#if PLATFORM_ARCH_32
#define GASNETC_AM_LONG_PAD(numargs) ((((numargs)&0x1)==1) ? 0 : 4)
#elif PLATFORM_ARCH_64
#define GASNETC_AM_LONG_PAD(numargs) ((((numargs)&0x1)==0) ? 0 : 4)
#endif

#define GASNETC_AM_SHORT_HEADER_LEN(numargs)                     \
			((numargs)*4 + GASNETC_AM_SHORT_ARGS_OFF)
#define GASNETC_AM_MEDIUM_HEADER_LEN(numargs)                     \
			((numargs)*4 + GASNETC_AM_MEDIUM_ARGS_OFF \
			+ GASNETC_AM_MEDIUM_HEADER_PAD(numargs))
#define GASNETC_AM_LONG_HEADER_LEN(numargs)                     \
			((numargs)*4 + GASNETC_AM_LONG_ARGS_OFF \
			+ GASNETC_AM_LONG_PAD(numargs))
/* system is same as AM Medium */
#define GASNETC_AM_SYSTEM_HEADER_LEN(n) GASNETC_AM_MEDIUM_HEADER_LEN(n)

/* -------------------------------------------------------------------------- */
/* Maximum sizes for mediums and Longs */
#define GASNETC_AM_MEDIUM_MAX_                                    \
		(GASNETC_AM_PACKET -                              \
		 GASNETC_AM_MEDIUM_HEADER_LEN(GASNETC_AM_MAX_ARGS))
#if GASNET_PSHM
  #define GASNETC_AM_MEDIUM_MAX MIN(GASNETC_AM_MEDIUM_MAX_, GASNETI_MAX_MEDIUM_PSHM)
#else
  #define GASNETC_AM_MEDIUM_MAX GASNETC_AM_MEDIUM_MAX_
#endif 
#define GASNETC_AM_LONG_REPLY_MAX   (GASNETC_AM_LEN - GASNETC_LONG_OFFSET)
#define GASNETC_AM_LONG_REQUEST_MAX (3*GASNETC_AM_LEN - GASNETC_LONG_OFFSET)

#define GASNETC_AM_SYSTEM_MAX GASNETC_AM_MEDIUM_MAX
/* -------------------------------------------------------------------------- */
#define GASNETC_AM_NUMARGS(c)   (((c) >> 1) & 0x1f)
#define GASNETC_AM_TYPE(c)      ((c) & 0xc0)
#define GASNETC_AM_IS_SYSTEM(c)  (GASNETC_AM_TYPE(c) == GASNETC_AM_SYSTEM)
#define GASNETC_AM_IS_REQUEST(c) (!GASNETC_AM_IS_SYSTEM((c)) && \
		                  ((c) & GASNETC_AM_REQUEST))
#define GASNETC_AM_IS_REPLY(c)   (!GASNETC_AM_IS_SYSTEM((c)) && \
		                  !((c) & GASNETC_AM_REQUEST))
#define GASNETC_AM_TYPE_STRING(buf)                                            \
                (GASNETC_AM_TYPE(buf) == GASNETC_AM_SHORT ? "Short" :	       \
                        (GASNETC_AM_TYPE(buf) == GASNETC_AM_MEDIUM ? "Medium": \
                                (GASNETC_AM_TYPE(buf) == GASNETC_AM_LONG ?     \
				 "Long" : "Error!")))
/* -------------------------------------------------------------------------- */
#define GASNETC_ASSERT_AMSHORT(buf, type, handler, args, req)  \
        do {    gasneti_assert(buf != NULL);                   \
                gasneti_assert(type >= GASNETC_AM_SHORT &&     \
                                type <= GASNETC_AM_SYSTEM);    \
                gasneti_assert(req == 0 || req == 1);          \
                gasneti_assert(numargs >= 0 && numargs <= 16); \
	} while (0)

#define GASNETC_ASSERT_AMMEDIUM(buf, type, handler, args, req, len, src) \
	    GASNETC_ASSERT_AMSHORT(buf, type, handler, args, req); 

#define GASNETC_ASSERT_AMLONG(buf, type, handler, args, req, len, src, dest) \
	    GASNETC_ASSERT_AMMEDIUM(buf, type, handler, args, req, len,  src);

/* -------------------------------------------------------------------------- */
/* Debug, tracing */
#if GASNET_TRACE
  #define GASNETC_AMTRACE_ReplyLong(str) \
     GASNETI_TRACE_PRINTF(C,("%s%s\t%d token="GASNETI_LADDRFMT" index=%d "   \
        "args=%d src="GASNETI_LADDRFMT" dst="GASNETI_LADDRFMT" len=%d", #str, "AMReplyLong",    \
        dest, GASNETI_LADDRSTR(token), handler, numargs, GASNETI_LADDRSTR(source_addr), \
        GASNETI_LADDRSTR(dest_addr), (int)(nbytes)))
#else
/* NO tracing enabled */
  #define GASNETC_AMTRACE_ReplyLong(str) 
#endif

/* Need GASNETC_DPRINTF for init functions since gasnet tracing
 * is not enabled yet
 */
#if	GASNET_DEBUG
#define GASNETC_DPRINTF(x)	printf x
#else
#define GASNETC_DPRINTF(x)      ((void)0)
#endif

/* -------------------------------------------------------------------------- */
#define GASNETC_ASSERT_BUFDESC_PTR(bufd, ptr) do {                \
                gasneti_assert((bufd)->id ==                      \
                (((uintptr_t)(ptr) - (uintptr_t)_gmc.dma_bufs) >> \
                        GASNETC_AM_SIZE));                        \
		} while (0)
#define GASNETC_BUFDESC_PTR(x) ((gasnetc_bufdesc_t *) &_gmc.bd_ptr[	       \
				(((uintptr_t)(x) - (uintptr_t)_gmc.dma_bufs)>> \
				 GASNETC_AM_SIZE)])
#define GASNETC_GM_RECV_PTR(e,fast)				\
	(fast) ? (uint8_t *) gm_ntohp((e)->recv.message) :	\
	    (uint8_t *) gm_ntohp((e)->recv.buffer)

#define GASNETC_SYSHEADER_WRITE(buf, index)				\
	*((uint8_t *)(buf)) = (GASNETC_AM_SYSTEM | ((index) & 0x3f))
#define GASNETC_SYSHEADER_READ(buf) (uint8_t)(*((uint8_t *)(buf)) & 0x3f)

#define GASNETC_AMHEADER_WRITE(buf, type, args, req) 			\
	*((uint8_t *)(buf)) = (((uint8_t)(type) & 0xc0) | 		\
			       ((uint8_t)(args)<< 1) | ((uint8_t)(req) & 0x01) )
#define GASNETC_AMHANDLER_WRITE(buf, handler)				\
	*((uint8_t *)(buf)) = (uint8_t)(handler)
#define GASNETC_AMHANDLER_READ(buf, handler)				\
	(uint8_t)(handler) = *((uint8_t *)(buf))

#define GASNETC_AMDESTADDR_WRITE(buf, dest_addr)         		\
	*((uintptr_t *)(buf)) = (uintptr_t)(dest_addr)
#define GASNETC_AMDESTADDR_READ(buf, dest_addr)                     	\
	(uintptr_t)(dest_addr) = *((uintptr_t *)(buf))

#define GASNETC_AMLENGTH_WRITE(buf, len) *((uint16_t *)(buf)) = (uint16_t)(len)
#define GASNETC_AMLENGTH_READ(buf, len)	 (uint16_t)(len) = *((uint16_t *)(buf))
#define GASNETC_AMLENGTH_WRITE4(buf, len) *((uint32_t *)(buf)) = (uint32_t)(len)
#define GASNETC_AMLENGTH_READ4(buf, len) (uint32_t)(len) = *((uint32_t *) (buf))

#define GASNETC_ARGS_WRITE(buf, argptr, numargs)			\
	do { 	int _i; int32_t *_pbuf = (int32_t *) buf;		\
		for (_i = 0; _i < (numargs); _i++)	{		\
			_pbuf[_i] = (int32_t) va_arg((argptr), int);  	\
		}							\
	   } while (0)

#define GASNETC_AMPAYLOAD_WRITE(d,s,l)	GASNETE_FAST_UNALIGNED_MEMCPY(d,s,l)
#define GASNETC_AMPAYLOAD_READ GASNETC_AMPAYLOAD_WRITE

#if PLATFORM_ARCH_32
#define GASNETC_ARGPTR_NUM	1
#define GASNETC_ARGPTR(ptr, addr)	(*((int32_t *)(ptr)) = (int32_t) addr)
#elif PLATFORM_ARCH_64
#define GASNETC_ARGPTR_NUM	2
#define GASNETC_ARGPTR(ptr, addr)				    \
	do { *((int32_t *)(ptr)) = (int32_t) GASNETI_HIWORD(addr);  \
	     *((int32_t *)(ptr)+1) = (int32_t) GASNETI_LOWORD(addr);\
	   } while (0)
#endif

/* -------------------------------------------------------------------------- */

GASNETI_END_EXTERNC

#endif
