#ifndef QT_ROSE_EXT_H
#define QT_ROSE_EXT_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef QTHREAD_USE_ROSE_EXTENSIONS
aligned_t *qthread_task_counter(void);
void       qthread_parent_yield_state(void);
void       qthread_pack_workerid(const qthread_worker_id_t w,
                                 const qthread_worker_id_t newId);
#endif
#endif // ifndef QT_ROSE_EXT_H
/* vim:set expandtab: */
