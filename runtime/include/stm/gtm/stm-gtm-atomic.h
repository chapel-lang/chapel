#ifndef _stm_gtm_atomic_h_
#define _stm_gtm_atomic_h_

//
// FIXME: Re-implement these macros using gasnet atomics
//

#include <atomic_ops.h>
typedef AO_t atomic_t;
#define ATOMIC_CAS_MB(addr, e, v) \
  (AO_compare_and_swap_full((volatile AO_t *)(addr), (AO_t)(e), (AO_t)(v)))

#define ATOMIC_FETCH_AND_INC_MB(addr) \
  (AO_fetch_and_add1_full((volatile AO_t *)(addr)))

#define ATOMIC_FETCH_AND_DEC_MB(addr) \
  (AO_fetch_and_sub1_full((volatile AO_t *)(addr)))

#define ATOMIC_LOAD_MB(addr) \
  (AO_load_full((volatile AO_t *)(addr)))

#define ATOMIC_LOAD(addr) \
  ATOMIC_LOAD_MB(addr)

#define ATOMIC_STORE_MB(addr, v) \
  (AO_store_full((volatile AO_t *)(addr), (AO_t)(v)))

#define ATOMIC_STORE(addr, v) \
  ATOMIC_STORE_MB(addr, v)

#endif 
