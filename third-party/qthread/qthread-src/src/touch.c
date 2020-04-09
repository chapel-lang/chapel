#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* System Headers */

/* Internal Headers */
#include "qt_qthread_mgmt.h" /* for qthread_internal_self() */
#include "qt_shepherd_innards.h"
#include "qt_threadqueues.h"
#include "qt_touch.h"

void qthread_run_needed_task(syncvar_t *value)
{
    qthread_shepherd_t *shep   = qthread_internal_getshep();
    qthread_t          *target = NULL;
    qthread_t          *orig_t = qthread_internal_self();

    //  ucontext_t my_context;

    if ((target = qt_threadqueue_dequeue_specific(shep->ready, value))) {
        // switch to task and run -- else missing and return
        //    qthread_t * t = n->value;

        // getcontext(&my_context); // done inside qthread_exec
        // t->rdata->return_context = &my_context;  // done inside qthread_exec
        // qassert(qthread_writeEF_const(t, (aligned_t*)t->ret, (t->f) (t->arg, t->arg)), QTHREAD_SUCCESS);
        /* note: there's a good argument that the following should
         * be: (*t->f)(t), however the state management would be
         * more complex
         */
        //    qthread_exec(t, &my_context);
        qthread_back_to_master(orig_t);
    }
}

/* vim:set expandtab: */
