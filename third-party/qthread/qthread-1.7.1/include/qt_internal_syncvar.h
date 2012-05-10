#ifndef QTHREAD_INTERNAL_FEB
#define QTHREAD_INTERNAL_FEB

#include "qt_visibility.h"

int INTERNAL qthread_syncvar_writeEF_nb(syncvar_t *restrict const      dest,
                                        const uint64_t *restrict const src);
int INTERNAL qthread_syncvar_writeEF_const_nb(syncvar_t *restrict const dest,
                                              const uint64_t            src);
int INTERNAL qthread_syncvar_readFF_nb(uint64_t *restrict const  dest,
                                       syncvar_t *restrict const src);
int INTERNAL qthread_syncvar_readFE_nb(uint64_t *restrict const  dest,
                                       syncvar_t *restrict const src);

#endif // ifndef QTHREAD_INTERNAL_FEB
/* vim:set expandtab: */
