#ifndef QTHREAD_INTERNAL_FEB
#define QTHREAD_INTERNAL_FEB

#include "qt_visibility.h"

int INTERNAL qthread_writeEF_nb(aligned_t *restrict const       dest,
                                const aligned_t *restrict const src);
int INTERNAL qthread_writeEF_const_nb(aligned_t *const dest,
                                      const aligned_t  src);
int INTERNAL qthread_readFF_nb(aligned_t *restrict const       dest,
                               const aligned_t *restrict const src);
int INTERNAL qthread_readFE_nb(aligned_t *restrict const       dest,
                               const aligned_t *restrict const src);

#endif // ifndef QTHREAD_INTERNAL_FEB
/* vim:set expandtab: */
