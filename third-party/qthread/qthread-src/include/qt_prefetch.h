#ifndef QTHREAD_PREFETCH_H
#define QTHREAD_PREFETCH_H

#ifdef HAS_BUILTIN_PREFETCH
#define Q_PREFETCH(x, ...) __builtin_prefetch((x) , ##__VA_ARGS__)
#else
#define Q_PREFETCH(x, ...)
#endif

#endif
