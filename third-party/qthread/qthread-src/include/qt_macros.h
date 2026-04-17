#ifndef QT_MACROS_H
#define QT_MACROS_H

// Work around OSX currently refusing to support threads.h
#if 201112L <= __STDC_VERSION__ && __STDC_VERSION__ < 202311L
#ifndef __STDC_NO_THREADS__
#include <threads.h>
#else
#define thread_local _Thread_local
#endif
#elif __STDC_VERSION__ < 201112L
#error "C11 is required"
#endif

#ifdef __GNUC__
#define Q_UNUSED(x) __attribute__((unused)) x
#else
#define Q_UNUSED(x) x
#endif

#if defined(__has_feature)
#if __has_feature(memory_sanitizer)
#define QTHREAD_MSAN
#endif
#endif
#ifdef QTHREAD_MSAN
#define QTHREAD_SUPPRESS_MSAN __attribute__((no_sanitize("memory")))
#else
#define QTHREAD_SUPPRESS_MSAN
#endif

#define TLS_DECL(type, name) thread_local type name
#define TLS_DECL_INIT(type, name) thread_local type name = 0
#define TLS_GET(name) name
#define TLS_SET(name, val) name = (val)
#define TLS_INIT(name)
#define TLS_INIT2(name, func)
#define TLS_DELETE(name) name = 0

#endif // ifndef QT_MACROS_H
/* vim:set expandtab: */
