#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

/* The API */
#include "qthread/qthread.h"

/* Internal Headers */
#include "qt_visibility.h"

/* functions to implement FEB-ish locking/unlocking*/

int API_FUNC qthread_lock(const aligned_t *a)
{                      /*{{{ */
    return qthread_readFE(NULL, a);
}                      /*}}} */

int API_FUNC qthread_unlock(const aligned_t *a)
{                      /*{{{ */
    return qthread_fill(a);
}                      /*}}} */

/* vim:set expandtab: */
