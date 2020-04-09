#ifndef QTHREAD_INTERNAL_SYNCVAR_H
#define QTHREAD_INTERNAL_SYNCVAR_H

#include "qt_visibility.h"
#include "qt_hash.h" /* for qt_key_t */
#include "qt_qthread_t.h"
#include "qt_filters.h"

typedef void (*qt_syncvar_callback_f)(qt_key_t     addr,
                                      qthread_f    f,
                                      void        *arg,
                                      void        *retloc,
                                      unsigned int thread_id,
                                      void        *tls,
                                      void        *callarg);
typedef filter_code (*qt_syncvar_taskfilter_f)(qt_key_t            addr,
                                               qthread_t *restrict waiter,
                                               void *restrict      arg);

void INTERNAL qt_syncvar_subsystem_init(uint_fast8_t need_sync);
int API_FUNC  qthread_syncvar_writeEF_nb(syncvar_t *restrict const      dest,
                                         const uint64_t *restrict const src);
int API_FUNC qthread_syncvar_writeEF_const_nb(syncvar_t *restrict const dest,
                                              const uint64_t            src);
int API_FUNC qthread_syncvar_readFF_nb(uint64_t *restrict const  dest,
                                       syncvar_t *restrict const src);
int API_FUNC qthread_syncvar_readFE_nb(uint64_t *restrict const  dest,
                                       syncvar_t *restrict const src);

void API_FUNC qthread_syncvar_callback(qt_syncvar_callback_f cb,
                                       void                 *arg);
void INTERNAL qthread_syncvar_taskfilter(qt_syncvar_taskfilter_f tf,
                                         void                   *arg);
void INTERNAL qthread_syncvar_taskfilter_serial(qt_syncvar_taskfilter_f tf,
                                                void                   *arg);
#endif // ifndef QTHREAD_INTERNAL_FEB
/* vim:set expandtab: */
