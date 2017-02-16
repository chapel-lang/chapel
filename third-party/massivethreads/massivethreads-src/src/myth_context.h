/* 
 * myth_context.h ---
 *  data structure related to context and
 *  must be included by the user
 */
#pragma once
#ifndef MYTH_CONTEXT_H_
#define MYTH_CONTEXT_H_

#include <stdint.h>
#include "myth_config.h"

/* define MYTH_CONTEXT */

#if MYTH_FORCE_UCONTEXT || MYTH_ARCH == MYTH_ARCH_UNIVERSAL
#define MYTH_CONTEXT MYTH_CONTEXT_UCONTEXT
#include <ucontext.h>

#elif MYTH_ARCH == MYTH_ARCH_i386
#define MYTH_CONTEXT MYTH_CONTEXT_i386

#elif MYTH_ARCH == MYTH_ARCH_amd64
#define MYTH_CONTEXT MYTH_CONTEXT_amd64

#elif MYTH_ARCH == MYTH_ARCH_amd64_knc
#define MYTH_CONTEXT MYTH_CONTEXT_amd64_knc

#elif MYTH_ARCH == MYTH_ARCH_sparc_v9
#define MYTH_CONTEXT MYTH_CONTEXT_sparc_v9
#define FRAMESIZE 176   /* Keep consistent with myth_context.S */
#define STACKBIAS 2047  /* Do not change this */
#define SAVE_FP   128
#define SAVE_I7   136

#elif MYTH_ARCH == MYTH_ARCH_sparc_v8
#define MYTH_CONTEXT MYTH_CONTEXT_sparc_v8
#define FRAMESIZE 92
#define SAVE_FP   68
#define SAVE_I7   72

#else
#error "invalid MYTH_ARCH"
#endif

/* Execution context */
typedef struct myth_context {
#if MYTH_CONTEXT == MYTH_CONTEXT_i386
  uint32_t esp;
#elif MYTH_CONTEXT == MYTH_CONTEXT_amd64 || MYTH_CONTEXT == MYTH_CONTEXT_amd64_knc
  uint64_t rsp;
#elif MYTH_CONTEXT == MYTH_CONTEXT_sparc_v9
  uint64_t sp;
#elif MYTH_CONTEXT == MYTH_CONTEXT_sparc_v8
  uint32_t sp;
#elif MYTH_CONTEXT == MYTH_CONTEXT_UCONTEXT
  ucontext_t uc;
#else
#error "MYTH_CONTEXT not defined"
#endif
} myth_context, *myth_context_t;

/* typedef required to pass params to swap_context */
#if MYTH_CONTEXT == MYTH_CONTEXT_UCONTEXT
typedef struct myth_ctx_withcall_param {
  void(*fn)(void*, void*, void*);
  void *arg1, *arg2, *arg3;
} myth_ctx_withcall_param, *myth_ctx_withcall_param_t;

extern volatile __thread myth_ctx_withcall_param g_ctx_withcall_params;
#endif

//Attributes of functions called after context switch
#if MYTH_CONTEXT == MYTH_CONTEXT_i386
#define MYTH_CTX_CALLBACK static __attribute__((used,noinline,regparm(0)))
#define USE_AVOID_OPTIMIZE 1

#elif MYTH_CONTEXT == MYTH_CONTEXT_amd64 || MYTH_CONTEXT == MYTH_CONTEXT_amd64_knc

#if HAVE_ATTR_SYSV_ABI
#define MYTH_CTX_CALLBACK static __attribute__((used,noinline,sysv_abi))
#else
#define MYTH_CTX_CALLBACK static __attribute__((used,noinline))
#endif

#define USE_AVOID_OPTIMIZE 1

#elif MYTH_CONTEXT == MYTH_CONTEXT_sparc_v9 || MYTH_CONTEXT == MYTH_CONTEXT_sparc_v8
#include <string.h>

#define MYTH_CTX_CALLBACK static __attribute__((used,noinline))

#elif MYTH_CONTEXT == MYTH_CONTEXT_UCONTEXT
#include <string.h>
#define MYTH_CTX_CALLBACK static __attribute__((used,noinline))

#else 
#error "define MYTH_CONTEXT"
#endif	/* MYTH_CONTEXT == */


#endif	/* MYTH_CONTEXT_H_ */
