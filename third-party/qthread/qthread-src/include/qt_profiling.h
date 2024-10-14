#ifndef QT_PROFILING_H
#define QT_PROFILING_H

#ifdef QTHREAD_COUNT_THREADS
#define QTHREAD_COUNT_THREADS_BINCOUNTER(TYPE, BIN)                            \
  qthread_internal_incr(&TYPE##_stripes[(BIN)], &TYPE##_stripes_locks[(BIN)], 1)
#else
#define QTHREAD_COUNT_THREADS_BINCOUNTER(TYPE, BIN)                            \
  do {                                                                         \
  } while (0)
#endif

#endif // ifndef QT_PROFILING_H
/* vim:set expandtab: */
