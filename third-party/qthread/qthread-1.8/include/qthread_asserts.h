/* This does not do the whole file, because assert.h doesn't avoid re-defining itself either */
#ifndef QTHREAD_ASSERTS_H
# define QTHREAD_ASSERTS_H
#endif

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <assert.h>  /* for assert() */

#ifdef qassert
# undef qassert
#endif
#ifdef qassertnot
# undef qassertnot
#endif
#ifdef qassert_ret
# undef qassert_ret
#endif
#ifdef qassert_retvoid
# undef qassert_retvoid
#endif
#ifdef qassert_goto
# undef qassert_goto
#endif
#ifdef qgoto
# undef qgoto
#endif

#ifdef QTHREAD_NO_ASSERTS
# define qassert(op, val)    op
# define qassertnot(op, val) op
# ifdef assert
#  undef assert
# endif
# define assert(foo)
# define qassert_ret(assertion, retval) do { if (!(assertion)) { return retval; } } while (0)
# define qassert_retvoid(assertion)     do { if (!(assertion)) { return; } } while (0)
# define qassert_goto(assertion, tag)   do { if (!(assertion)) { goto tag; } } while (0)
# define qassert_aligned(variable, alignment)
# define qgoto(tag) tag :
#else // ifdef QTHREAD_NO_ASSERTS
# define qassert(op, val)                     assert(op == val)
# define qassertnot(op, val)                  assert(op != val)
# define qassert_ret(assertion, retval)       assert(assertion)
# define qassert_retvoid(assertion)           assert(assertion)
# define qassert_goto(assertion, tag)         assert(assertion)
# define qassert_aligned(variable, alignment) do {       \
        void *a = &(variable);                           \
        assert((((uintptr_t)a) & (alignment - 1)) == 0); \
} while (0)
# define qgoto(tag)
#endif // ifdef QTHREAD_NO_ASSERTS

/* vim:set expandtab: */
