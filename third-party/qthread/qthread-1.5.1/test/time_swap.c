#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#include <assert.h>                    /* for assert() */
#include <stdlib.h>                    /* for malloc() */
#include <stdio.h>                     /* for printf() */
#if defined(HAVE_UCONTEXT_H) && defined(HAVE_NATIVE_MAKECONTEXT)
# include <ucontext.h>
#else
# include "osx_compat/taskimpl.h"
#endif
#include <qthread/qtimer.h>
#include "argparsing.h"

size_t ITERATIONS = 1000000;

ucontext_t child, parent;
qtimer_t timer;
double total_single_time = 0.0;

static void inout(
    void *msg)
{
    long expected = 0;

    expected = (1L << 16);
    expected = expected << 16;
    expected += 1L;

    if ((long)msg == expected) {
        msg = (void *)1;
    } else {
        msg = (void *)2;
    }
#if !defined(HAVE_UCONTEXT_H) || !defined(HAVE_NATIVE_MAKECONTEXT)
    assert(swapcontext(&child, &parent) == 0);
#endif
}

static void single(
    void)
{
    qtimer_stop(timer);
    total_single_time += qtimer_secs(timer);
    qtimer_start(timer);
#if !defined(HAVE_UCONTEXT_H) || !defined(HAVE_NATIVE_MAKECONTEXT)
    assert(swapcontext(&child, &parent) == 0);
#endif
}

int main(
    int argc,
    char *argv[])
{
    /* big enough to ignore stack direction */
    char *stack = malloc(1024 * 1024 * 16);
    long msg = 0;

    double total_inout_time = 0.0;
    int i;

    CHECK_VERBOSE();
    NUMARG(ITERATIONS, "ITERATIONS");
    timer = qtimer_create();
    getcontext(&child);
    getcontext(&parent);
    child.uc_stack.ss_sp = stack + 1024 * 1024 * 8;
    child.uc_stack.ss_size = 1024 * 1024 * 8 - 64;
#if defined(HAVE_UCONTEXT_H) && defined(HAVE_NATIVE_MAKECONTEXT)
    child.uc_link = &parent;
#endif
    /* keep compiler quiet */
    msg = (1L << 16);
    msg = msg << 16;
    msg += 1L;

    for (i = 0; i < ITERATIONS; i++) {
        makecontext(&child, (void (*)(void))inout, 1, msg);
        qtimer_start(timer);
        assert(swapcontext(&parent, &child) == 0);
        qtimer_stop(timer);
        total_inout_time += qtimer_secs(timer);
    }

    for (i = 0; i < ITERATIONS; i++) {
        makecontext(&child, single, 1, 0);
        qtimer_start(timer);
        assert(swapcontext(&parent, &child) == 0);
        qtimer_stop(timer);
        total_single_time += qtimer_secs(timer);
    }

    printf("Testing swap time (average over %lu iterations)\n",
           (unsigned long int)ITERATIONS);
    printf("\t  Total in-out time: %g secs\n", total_inout_time);
    printf("\tAverage in-out time: %g secs\n", total_inout_time / ITERATIONS);
    printf("\t  Total single time: %g secs\n", total_single_time);
    printf("\tAverage single time: %g secs\n",
           total_single_time / (ITERATIONS * 2));
    printf("Context is %u bytes\n", (unsigned)sizeof(ucontext_t));

    qtimer_destroy(timer);
    return 0;
}

/* vim:set expandtab */