#ifndef QT_CONTEXT_H
#define QT_CONTEXT_H

#if defined(HAVE_UCONTEXT_H) && defined(HAVE_NATIVE_MAKECONTEXT)
# include <ucontext.h>                 /* for ucontext_t */
typedef ucontext_t qt_context_t;
#else
# include "fastcontext/taskimpl.h"
typedef uctxt_t qt_context_t;
#endif

#endif // ifndef QT_CONTEXT_H
/* vim:set expandtab: */
