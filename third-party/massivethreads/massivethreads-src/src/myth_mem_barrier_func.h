/* 
 * myth_mem_barrier_func.h
 */
#pragma once
#ifndef MYTH_MEM_BARRIER_FUNC_H_
#define MYTH_MEM_BARRIER_FUNC_H_

#include "myth_config.h"

//A macro for supressing optimization
static inline void myth_loop_barrier() {
  asm volatile("" ::: "memory");
}

//MEMO:Architecture-Dependent Code
#if MYTH_ARCH == MYTH_ARCH_i386 || MYTH_ARCH == MYTH_ARCH_amd64 || MYTH_ARCH == MYTH_ARCH_amd64_knc

#if MYTH_BARRIER == MYTH_BARRIER_FENCES
//Memory barriers by x86 fence instructions
//Guarantees successive reads to be executed after this
static inline void myth_rbarrier() {
  asm volatile("lfence" ::: "memory");
}
//Guarantees former writes to be executed before this
static inline void myth_wbarrier() {
  asm volatile("sfence" ::: "memory");
}
//rbarrier+wbarrier
static inline void myth_rwbarrier() {
  asm volatile("mfence" ::: "memory");
}

#elif MYTH_BARRIER == MYTH_BARRIER_CILK
//Memory barriers used by Cilk
//Guarantees successive reads to be executed after this
static inline void myth_rbarrier() {
  int x=0, y;
  asm volatile("xchgl %0,%1":"=r"(x):"m"(y),"0"(x):"memory");
}
//Guarantees former writes to be executed before this
static inline void myth_wbarrier() {
  asm volatile("":::"memory");
}
//rbarrier+wbarrier
static inline void myth_rwbarrier() {
  myth_rbarrier();
}

#elif MYTH_BARRIER == MYTH_BARRIER_CILK_WEAK
//R->R orderings are guaranteed by x86[_64] architecture
static inline void myth_rbarrier() {
  asm volatile("":::"memory");
}
//W->W orderings are guaranteed by x86[_64] architecture
static inline void myth_wbarrier() {
  asm volatile("":::"memory");
}
//R->W orderings are NOT guaranteed. Need to serialize by atomic or fence insns
//the initialization y=0 is not necessary but desirable to suppress 
//may-be-used-before-defined warning by icc
static inline void myth_rwbarrier() {
  int x,y;
  asm volatile("xchgl %0,%1":"=r"(x),"=m"(y)::"memory");
}
//#define myth_rwbarrier() asm volatile("mfence":::"memory")
#elif MYTH_BARRIER == MYTH_BARRIER_INTRINSIC

static inline void myth_rbarrier() {
  __sync_synchronize();
}
static inline void myth_wbarrier() {
  __sync_synchronize();
}
static inline void myth_rwbarrier() {
  __sync_synchronize();
}

#else
#error "invalid MYTH_BARRIER"
#endif	
/* end of branch
   MYTH_ARCH == MYTH_ARCH_i386 || MYTH_ARCH == MYTH_ARCH_amd64 || MYTH_ARCH == MYTH_ARCH_amd64_knc */

#elif MYTH_ARCH == MYTH_ARCH_sparc_v9 || MYTH_ARCH == MYTH_ARCH_sparc_v8

/* TODO: currently almost all memory barriers are membar 15,
   which is equivalent to __sync_synchronize.  they are necessary
   as fccpx does not support __sync_synchronize. it is sometimes 
   too strong. */

#if MYTH_BARRIER == MYTH_BARRIER_FENCES
//Memory barriers by x86 fence instructions
//Guarantees successive reads to be executed after this
static inline void myth_rbarrier() {
  asm volatile("membar 15" ::: "memory");
}
//Guarantees former writes to be executed before this
static inline void myth_wbarrier() {
  asm volatile("membar 15" ::: "memory");
}
//rbarrier+wbarrier
static inline void myth_rwbarrier() {
  asm volatile("membar 15" ::: "memory");
}

#elif MYTH_BARRIER == MYTH_BARRIER_CILK
//Memory barriers used by Cilk
//Guarantees successive reads to be executed after this
static inline void myth_rbarrier() {
  asm volatile("membar 15" ::: "memory");
}
//Guarantees former writes to be executed before this
static inline void myth_wbarrier() {
  asm volatile("membar 15" ::: "memory");
}
//rbarrier+wbarrier
static inline void myth_rwbarrier() {
  asm volatile("membar 15" ::: "memory");
}

#elif MYTH_BARRIER == MYTH_BARRIER_CILK_WEAK
//R->R orderings are guaranteed by x86[_64] architecture
static inline void myth_rbarrier() {
  asm volatile("" ::: "memory");
}
//W->W orderings are guaranteed by x86[_64] architecture
static inline void myth_wbarrier() {
  asm volatile("" ::: "memory");
}
//R->W orderings are NOT guaranteed. Need to serialize by atomic or fence insns
//the initialization y=0 is not necessary but desirable to suppress 
//may-be-used-before-defined warning by icc
static inline void myth_rwbarrier() {
  asm volatile("membar 15" ::: "memory");
}
//#define myth_rwbarrier() asm volatile("mfence":::"memory")
#elif MYTH_BARRIER == MYTH_BARRIER_INTRINSIC

static inline void myth_rbarrier() {
  __sync_synchronize();
}
static inline void myth_wbarrier() {
  __sync_synchronize();
}
static inline void myth_rwbarrier() {
  __sync_synchronize();
}

#else  /* other architectures */
#error "invalid MYTH_BARRIER"
#endif
/* end of branch
   MYTH_ARCH == MYTH_ARCH_sparc_v9 || MYTH_ARCH == MYTH_ARCH_sparc_v8 */

#else  /* other archs */

#warning "Architecture-dependent memory barrier is not defined, substituted by GCC extension"

//Default
static inline void myth_rbarrier() {
  __sync_synchronize();
}
static inline void myth_wbarrier() {
  __sync_synchronize();
}
static inline void myth_rwbarrier() {
  __sync_synchronize();
}

#endif

#endif /* MYTH_BARRIER_H_ */
