#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#include <limits.h>		       /* for INT_MAX */
#include <qthread/qthread.h>
#include <qthread/futurelib.h>

/* prototypes */
void qthread_wrapper(const qthread_f f, const void *arg, aligned_t * ret);
void qthread_future_wrapper(const qthread_f f, const void *arg,
			    aligned_t * ret);
int qthread_fork_future_to(const qthread_f f, const void *arg,
			   aligned_t * ret, qthread_shepherd_id_t shep);
void qthread_assertnotfuture(qthread_t * t);
unsigned int qthread_isfuture(const qthread_t * t);

void qthread_wrapper(const qthread_f f, const void *arg, aligned_t * ret)
{
    if (ret == NULL) {
	f((void *)arg);
    } else {
	aligned_t retval = f((void *)arg);

	qthread_writeEF_const(ret, retval);
    }
}

void qthread_future_wrapper(const qthread_f f, const void *arg,
			    aligned_t * ret)
{
    PIM_writeSpecial(PIM_CMD_SET_FUTURE, 1);
    if (ret == NULL) {
	f((void *)arg);
    } else {
	qthread_writeEF_const(ret,
			      f((void *)arg));
    }
    future_exit();
    PIM_threadExitFree();	       // Why here and not in qthread_wrapper() ?
}

int qthread_fork_to(const qthread_f f, const void *arg, aligned_t * ret,
		    qthread_shepherd_id_t shep)
{
    if (ret) {
	PIM_feb_empty(ret);
    }
    PIM_loadAndSpawnToLocaleStack(shep == NO_SHEPHERD ? -1 : shep,
					 (void *)qthread_wrapper, (void *)f,
					 (void *)arg, ret, NULL, NULL);
    return 0;
}

int qthread_fork_future_to(const qthread_f f, const void *arg,
			   aligned_t * ret, qthread_shepherd_id_t shep)
{
    if (ret) {
	PIM_feb_empty(ret);
    }
    return PIM_loadAndSpawnToLocaleStack(shep == NO_SHEPHERD ? -1 : shep,
					(void *)qthread_future_wrapper,
					(void *)f, (void *)arg, ret, NULL,
					NULL);
}

void qthread_assertnotfuture(qthread_t * t)
{
/* XXX: this does NOTHING! This is a bug! */
}

unsigned int qthread_isfuture(const qthread_t * t)
{
    /* XXX: this does NOTHING! This is a bug! */
    return 0;
}
