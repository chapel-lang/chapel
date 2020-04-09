#ifndef QTHREAD_NET_NET_H
#define QTHREAD_NET_NET_H

#include <unistd.h>

#include <qthread/qthread.h>

int qthread_internal_net_driver_initialize(void);

int qthread_internal_net_driver_get_rank(void);
int qthread_internal_net_driver_get_size(void);

int qthread_internal_net_driver_send(int    peer,
                                     int    tag,
                                     void  *start,
                                     size_t len);

int qthread_internal_net_driver_put(int                  peer,
                                    void *restrict       dest_addr,
                                    const void *restrict src_addr,
                                    size_t               size,
                                    aligned_t *restrict  feb);
int qthread_internal_net_driver_get(void *restrict       dest_addr,
                                    int                  peer,
                                    const void *restrict src_addr,
                                    size_t               size,
                                    aligned_t *restrict  feb);

typedef void (*qthread_internal_net_driver_handler)(int tag, void *start, size_t len);
int qthread_internal_net_driver_register(int                                 tag,
                                         qthread_internal_net_driver_handler handler);

int qthread_internal_net_driver_finalize(void);

int qthread_internal_net_driver_run(void);

int qthread_internal_net_driver_barrier(void);

#endif // ifndef QTHREAD_NET_NET_H
/* vim:set expandtab: */
