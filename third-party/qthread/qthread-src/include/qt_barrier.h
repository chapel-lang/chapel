#ifndef QT_BARRIER_H
#define QT_BARRIER_H

#include "qt_visibility.h"

/* these two calls assume that we're using a/the global barrier */
void qt_global_barrier(void);
void qt_global_barrier_init(size_t size, int debug);
void qt_global_barrier_destroy(void);
void qt_global_barrier_resize(size_t size);
//#define qt_barrier(x) qt_global_barrier()

void INTERNAL qt_barrier_internal_init(void);

#endif
