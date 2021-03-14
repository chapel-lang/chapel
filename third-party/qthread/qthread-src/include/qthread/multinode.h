#ifndef QTHREAD_QTHREAD_MULTINODE_H
#define QTHREAD_QTHREAD_MULTINODE_H

#include <stdint.h>

#include "qthread.h"

Q_STARTCXX /* */

#define FORK_MSG_PAYLOAD 44

int qthread_multinode_run(void);
int qthread_multinode_multistart(void);
int qthread_multinode_multistop(void);
int qthread_multinode_rank(void);
int qthread_multinode_size(void);

int qthread_multinode_register(uint32_t  tag,
                               qthread_f f);
int qthread_multinode_action_uid(qthread_f f,
                                 uint32_t *tag);
int qthread_multinode_action_ptr(uint32_t   tag,
                                 qthread_f *ptr);

int qthread_fork_remote(qthread_f   f,
                        const void *arg,
                        aligned_t  *ret,
                        int         rank,
                        size_t      arg_len);

Q_ENDCXX /* */

#endif // ifndef QTHREAD_QTHREAD_MULTINODE_H
/* vim:set expandtab: */
