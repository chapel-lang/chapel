#ifndef _QT_THREADQUEUE_SCHEDULER_H_
#define _QT_THREADQUEUE_SCHEDULER_H_
#include "qt_shepherd_innards.h"

qthread_shepherd_id_t INTERNAL
qt_threadqueue_choose_dest(qthread_shepherd_t *curr_shep);

#endif // _QT_THREADQUEUE_SCHEDULER_H_

