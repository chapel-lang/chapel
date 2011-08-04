#include <stdio.h>
#include <assert.h>
#include <qthread/qthread.h>
#include <qthread/qtimer.h>
#include <sys/time.h>
#include "argparsing.h"

int main(int argc,
         char *argv[])
{
    qtimer_t t;

    assert(qthread_initialize() == QTHREAD_SUCCESS);

    CHECK_VERBOSE();

    t = qtimer_create();
    assert(t);
    qtimer_start(t);
    qtimer_stop(t);
#ifndef SST                            /* SST can do this blazingly fast */
    if (qtimer_secs(t) == 0) {
        fprintf(stderr, "qtimer_secs(t) reported zero length time.\n");
    } else if (qtimer_secs(t) < 0) {
        fprintf(stderr, "qtimer_secs(t) thinks time went backwards (%g).\n",
                qtimer_secs(t));
    }
#endif /* ifndef SST */
    iprintf("time to find self and assert it: %g secs\n", qtimer_secs(t));

    qtimer_start(t);
    qtimer_stop(t);
    assert(qtimer_secs(t) >= 0.0);
    if (qtimer_secs(t) == 0.0) {
        iprintf("inlining reduces calltime to zero (apparently)\n");
    } else {
        iprintf("smallest measurable time: %g secs\n", qtimer_secs(t));
    }

    qtimer_destroy(t);

    return 0;
}

/* vim:set expandtab */
