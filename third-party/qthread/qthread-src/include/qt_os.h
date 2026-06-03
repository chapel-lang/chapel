#ifndef QT_OS_H
#define QT_OS_H

// Not all Operating systems listed here are actually supported.

#if defined(__linux__) || defined(QTHREADS_LINUX)
#ifndef QTHREADS_LINUX
#define QTHREADS_LINUX
#endif

#elif defined(__APPLE__) || defined(QTHREADS_APPLE)
#ifndef QTHREADS_APPLE
#define QTHREADS_APPLE
#endif

#elif defined(_WIN32) || defined(QTHREADS_WINDOWS)
#ifndef QTHREADS_WINDOWS
#define QTHREADS_WINDOWS
#endif

#elif defined(__FreeBSD__) || defined(QTHREADS_FREEBSD)
#ifndef QTHREADS_FREEBSD
#define QTHREADS_FREEBSD
#endif

#elif defined(__NetBSD__) || defined(QTHREADS_NETBSD)
#ifndef QTHREADS_NETBSD
#define QTHREADS_NETBSD
#endif

#elif defined(__OpenBSD__) || defined(QTHREADS_OPENBSD)
#ifndef QTHREADS_OPENBSD
#define QTHREADS_OPENBSD
#endif

#elif defined(__DragonFly__) || defined(QTHREADS_DRAGONFLYBSD)
#ifndef QTHREADS_DRAGONFLYBSD
#define QTHREADS_DRAGONFLYBSD
#endif

#elif defined(__sun)
#error "Sun OS not currently supported."

#else
#error "Unrecognized OS"

#endif

#endif
