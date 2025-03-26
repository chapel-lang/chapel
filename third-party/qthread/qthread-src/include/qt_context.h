#ifndef QT_CONTEXT_H
#define QT_CONTEXT_H

#if defined(USE_SYSTEM_SWAPCONTEXT)
#include <ucontext.h> /* for ucontext_t */
typedef ucontext_t qt_context_t;
#else
#include "fastcontext/taskimpl.h"
typedef uctxt_t qt_context_t;
#endif

#endif // ifndef QT_CONTEXT_H
/* vim:set expandtab: */
