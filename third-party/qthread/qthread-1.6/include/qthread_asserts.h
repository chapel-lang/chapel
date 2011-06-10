#ifndef QTHREAD_ASSERTS_H
#define QTHREAD_ASSERTS_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifndef QTHREAD_NO_ASSERTS
# include <assert.h> /* for assert() */
#endif

#ifdef QTHREAD_NO_ASSERTS
# define qassert(op, val) op
# define qassertnot(op, val) op
# ifdef assert
#  undef assert
# endif
# define assert(foo)
# define qassert_ret(assertion, retval) do { if (!(assertion)) { return retval; } } while (0)
# define qassert_retvoid(assertion) do { if (!(assertion)) { return; } } while (0)
# define qassert_goto(assertion, tag) do { if (!(assertion)) { goto tag; } } while (0)
#define qgoto(tag) tag:
#else
# define qassert(op, val) assert(op == val)
# define qassertnot(op, val) assert(op != val)
# define qassert_ret(assertion, retval) assert(assertion)
# define qassert_retvoid(assertion) assert(assertion)
# define qassert_goto(assertion, tag) assert(assertion)
# define qgoto(tag)
#endif

#endif
