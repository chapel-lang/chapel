#ifndef QTHREAD_HASH_H
#define QTHREAD_HASH_H

#include "common.h" /* important configuration options */
#include "macros.h"
#include "qthread.h" /* for uint64_t, uint8_t, and aligned_t types */

Q_STARTCXX /* */

#if (QTHREAD_BITS == 32)
#define GOLDEN_RATIO 0x9e3779b9
#else
#define GOLDEN_RATIO 0x9e3779b97f4a7c13LL
#endif

  uint64_t qt_hash64(uint64_t key);
aligned_t qt_hash_bytes(void *key_ptr, size_t bytes, aligned_t state);

Q_ENDCXX /* */
#endif   // ifndef QTHREAD_HASH_H
  /* vim:set expandtab: */
