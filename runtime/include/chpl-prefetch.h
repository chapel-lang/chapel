#ifndef _chpl_prefetch_h_
#define _chpl_prefetch_h_

#include "chpl-comp-detect-macros.h"

static inline void chpl_prefetch(void* addr) {
#if (RT_COMP_CC & (~RT_COMP_PGI))
  // cray, intel, gcc, clang
  __builtin_prefetch(addr);
#elif (RT_COMP_CC & RT_COMP_PGI)
  // PGI seems to only support prefetch through a pragma. The dereference is
  // necessary. This does not do the exact the same thing as the other
  // compilers, but I'm not certain how to get perfectly equivalent
  // functionality outside of emitting the asm myself.
  #pragma mem prefetch *((char *)addr)
#else
  // Unknown compiler, we don't know how to generate a prefetch, just NOOP
  // It's not that much of a difference anyways
  (void)0;
#endif
}

#endif // _chpl_prefetch_h_
