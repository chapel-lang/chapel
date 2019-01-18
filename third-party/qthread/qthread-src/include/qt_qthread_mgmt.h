#ifndef QT_QTHREAD_MGMT_H
#define QT_QTHREAD_MGMT_H

#include "qt_visibility.h"
#include "qt_qthread_t.h"

void INTERNAL       qthread_thread_free(qthread_t *t);
qthread_t INTERNAL *qthread_internal_self(void);

#endif
/* vim:set expandtab: */
