#ifndef QT_LOCKS_H
#define QT_LOCKS_H

typedef struct qthread_lock_s qthread_lock_t;
struct qthread_queue_s {
    qthread_t          *head;
    qthread_t          *tail;
    qthread_shepherd_t *creator_ptr;
    pthread_mutex_t     lock;
    pthread_cond_t      notempty;
};

#endif // ifndef QT_LOCKS_H
/* vim:set expandtab: */
