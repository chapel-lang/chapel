#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "qthread/qthread.h" /* for aligned_t */
#include "qt_atomics.h" /* for SPINLOCK_BODY() */
#include "qthread/qtimer.h"

#include <stdlib.h>                    /* for calloc() */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#ifdef HAVE_SN_MMTIMER_H
# include <sn/mmtimer.h>
#elif defined(HAVE_LINUX_MMTIMER_H)
# include <linux/mmtimer.h>
#endif
#include <unistd.h>
#include <errno.h>
#ifndef MMTIMER_FULLNAME
# define MMTIMER_FULLNAME "/dev/mmtimer"
#endif

#include "qt_debug.h" /* for malloc debug wrappers */

static aligned_t               inited = 0;
static double                  timer_freq_conv;
static volatile unsigned long *timer_address = NULL;
static unsigned long          *mmdev_map     = NULL;

struct qtimer_s {
    unsigned long start;
    unsigned long stop;
};

static int qtimer_init(void)
{
    int           fd, ret;
    unsigned long val;
    long          offset;

    fd = open(MMTIMER_FULLNAME, O_RDONLY);
    if (fd < 0) {
        return -1;
    }

    /* make sure we can map the timer */
    ret = ioctl(fd, MMTIMER_MMAPAVAIL, 0);
    if (1 != ret) {
        return -1;
    }

    /* find the frequency of the timer */
    ret = ioctl(fd, MMTIMER_GETFREQ, &val);
    if (ret == -ENOSYS) {
        return -1;
    }
    timer_freq_conv = 1.0 / val;

    /* find the address of the counter */
    ret = ioctl(fd, MMTIMER_GETOFFSET, 0);
    if (ret == -ENOSYS) {
        return -1;
    }
    offset = ret;

    mmdev_map = mmap(0, getpagesize(), PROT_READ, MAP_SHARED, fd, 0);
    if (NULL == mmdev_map) {
        return -1;
    }
    timer_address = mmdev_map + offset;
    close(fd);

    return 0;
}

void qtimer_start(qtimer_t q)
{
    q->start = *timer_address;
}

unsigned long qtimer_fastrand(void)
{
    if (NULL == timer_address) {
        if (0 != qtimer_init()) {
            return 0;
        }
    }

    return *timer_address;
}

double qtimer_wtime(void)
{
    return ((double)(*timer_address)) * timer_freq_conv;
}

double qtimer_res(void)
{
    return timer_freq_conv;
}

void qtimer_stop(qtimer_t q)
{
    q->stop = *timer_address;
}

double qtimer_secs(qtimer_t q)
{
    return ((double)(q->stop - q->start)) * timer_freq_conv;
}

qtimer_t qtimer_create(void)
{
    if (NULL == timer_address) {
        if (qthread_cas_ptr(&timer_address, NULL, (void*)1) == NULL) {
            if (0 != qtimer_init()) {
                timer_address = NULL;
                return NULL;
            }
        } else {
            while (timer_address == (void *)1) SPINLOCK_BODY();
        }
    }

    return qt_calloc(1, sizeof(struct qtimer_s));
}

void qtimer_destroy(qtimer_t q)
{
    FREE(q, sizeof(struct qtimer_s));
}

/* vim:set expandtab: */
