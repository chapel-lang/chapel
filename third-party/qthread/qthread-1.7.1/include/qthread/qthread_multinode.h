#ifndef QTHREAD_QTHREAD_MULTINODE_H
#define QTHREAD_QTHREAD_MULTINODE_H

#include <stdint.h>

#include "qt_visibility.h"
#include "qthread.h"

int INTERNAL qthread_multinode_initialize(void);

int qthread_multinode_run(void);
int qthread_multinode_rank(void);
int qthread_multinode_size(void);
int qthread_multinode_register(uint32_t uid, qthread_f f);

int qthread_fork_remote(qthread_f f,
                        const void *arg,
                        aligned_t *ret,
                        int rank,
                        size_t arg_len);

#endif
