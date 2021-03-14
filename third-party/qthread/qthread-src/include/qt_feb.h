#ifndef QTHREAD_INTERNAL_FEB_H
#define QTHREAD_INTERNAL_FEB_H

#include "qt_visibility.h"
#include "qt_hash.h" /* for qt_key_t */
#include "qt_qthread_t.h"
#include "qt_filters.h" /* for filter_code */

typedef void (*qt_feb_callback_f)(qt_key_t     addr,
                                  qthread_f    f,
                                  void        *arg,
                                  void        *retloc,
                                  unsigned int thread_id,
                                  void        *tls,
                                  void        *callarg);
typedef filter_code (*qt_feb_taskfilter_f)(qt_key_t            addr,
                                           qthread_t *restrict waiter,
                                           void *restrict      arg);

void INTERNAL qt_feb_subsystem_init(uint_fast8_t);

int API_FUNC qthread_writeEF_nb(aligned_t *restrict const       dest,
                                const aligned_t *restrict const src);
int API_FUNC qthread_writeEF_const_nb(aligned_t *const dest,
                                      const aligned_t  src);
int API_FUNC qthread_readFF_nb(aligned_t *restrict const       dest,
                               const aligned_t *restrict const src);
int API_FUNC qthread_readFE_nb(aligned_t *restrict const       dest,
                               const aligned_t *restrict const src);
int INTERNAL qthread_check_feb_preconds(qthread_t *t);

void API_FUNC qthread_feb_callback(qt_feb_callback_f cb,
                                   void             *arg);
void INTERNAL qthread_feb_taskfilter(qt_feb_taskfilter_f tf,
                                     void               *arg);
void INTERNAL qthread_feb_taskfilter_serial(qt_feb_taskfilter_f tf,
                                            void               *arg);
#endif // ifndef QTHREAD_INTERNAL_FEB
/* vim:set expandtab: */
