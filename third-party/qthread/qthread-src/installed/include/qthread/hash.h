#ifndef QTHREAD_HASH_H
#define QTHREAD_HASH_H

#include "macros.h"
#include "common.h"            /* important configuration options */
#include "qthread.h"           /* for uint64_t, uint8_t, and aligned_t types */

Q_STARTCXX /* */

#if (QTHREAD_SIZEOF_ALIGNED_T == 4)
# define GOLDEN_RATIO 0x9e3779b9
#else
# define GOLDEN_RATIO 0x9e3779b97f4a7c13LL
#endif

uint64_t  qt_hash64(uint64_t key);
aligned_t qt_hash_bytes(void     *key_ptr,
                        size_t    bytes,
                        aligned_t state);

Q_ENDCXX /* */
#endif // ifndef QTHREAD_HASH_H
/* vim:set expandtab: */
