#ifndef QT_EUREKAS_H
#define QT_EUREKAS_H

#include "qt_visibility.h"
#include "qt_qthread_t.h"

/* Unexpected task destruction/cleanup */
void INTERNAL qthread_internal_assassinate(qthread_t *t);

void INTERNAL qt_eureka_worker_init(void);
void INTERNAL qt_eureka_end_criticalsect_dead(qthread_t *self);
void INTERNAL qt_eureka_check(int block);
void INTERNAL qt_eureka_disable(void);

#endif
