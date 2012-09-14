#ifndef QTHREAD_INTERNAL_FEB
#define QTHREAD_INTERNAL_FEB

#include "qt_visibility.h"

void INTERNAL qt_feb_subsystem_init(void);

int INTERNAL qthread_writeEF_nb(aligned_t *restrict const       dest,
                                const aligned_t *restrict const src);
int INTERNAL qthread_writeEF_const_nb(aligned_t *const dest,
                                      const aligned_t  src);
int INTERNAL qthread_readFF_nb(aligned_t *restrict const       dest,
                               const aligned_t *restrict const src);
int INTERNAL qthread_readFE_nb(aligned_t *restrict const       dest,
                               const aligned_t *restrict const src);
int INTERNAL qthread_check_feb_preconds(qthread_t *t);

#endif // ifndef QTHREAD_INTERNAL_FEB
/* vim:set expandtab: */
