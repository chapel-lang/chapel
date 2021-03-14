#ifndef _QT_THREADSTATE_H_
#define _QT_THREADSTATE_H_

typedef enum threadstate {
    QTHREAD_STATE_NASCENT,              /* awaiting preconds */
    QTHREAD_STATE_NEW,                  /* first ready-to-run state */
    QTHREAD_STATE_RUNNING,              /* ready-to-run */
    QTHREAD_STATE_YIELDED,              /* reschedule, otherwise ready-to-run */
    QTHREAD_STATE_YIELDED_NEAR,         /* reschedule, otherwise ready-to-run */
    QTHREAD_STATE_QUEUE,                /* insert me into a qthread_queue_t */
    QTHREAD_STATE_FEB_BLOCKED,          /* waiting for feb */
    QTHREAD_STATE_PARENT_YIELD,         /* parent is moving into QTHREAD_STATE_PARENT_BLOCKED */
    QTHREAD_STATE_PARENT_BLOCKED,       /* waiting for child to take this execution */
    QTHREAD_STATE_PARENT_UNBLOCKED,     /* child is picking up parent execution */
    QTHREAD_STATE_ASSASSINATED,         /* thread destroyed via signal; needs cleanup */
    QTHREAD_STATE_TERMINATED,           /* thread function returned */
    QTHREAD_STATE_MIGRATING,            /* thread needs to be moved, otherwise ready-to-run */
    QTHREAD_STATE_SYSCALL,              /* thread performing external blocking operation */
    QTHREAD_STATE_ILLEGAL,              /* illegal state */
    QTHREAD_STATE_TERM_SHEP,            /* special flag to terminate the shepherd */
    QTHREAD_STATE_NUM_STATES            /* tell performance data how many states there are */
} threadstate_t;

#endif // ifndef _QT_THREADSTATE_H_
/* vim:set expandtab: */
