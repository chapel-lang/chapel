#ifndef FUTURELIB_INNARDS_H
#define FUTURELIB_INNARDS_H

#include <pthread.h>

typedef struct location_s location_t;

struct location_s
{
    aligned_t vp_count;
    pthread_mutex_t vp_count_lock;
    unsigned int vp_max;
    qthread_shepherd_id_t id;
    qthread_shepherd_id_t sched_shep;
};

#endif
