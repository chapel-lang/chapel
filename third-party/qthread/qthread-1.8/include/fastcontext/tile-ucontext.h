#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stddef.h> /* for size_t, per C89 */

#include "qt_visibility.h"

#define setcontext(u) qt_setmctxt(&(u)->mc)
#define getcontext(u) qt_getmctxt(&(u)->mc)
typedef struct mctxt mctxt_t;
typedef struct uctxt uctxt_t;
/*
 * This struct defines the way the registers are stored on the stack during a
 * system call/exception.  It should be a multiple of 8 bytes to preserve
 * normal stack alignment rules.
 *
 */
struct mctxt {
    /* Saved main processor registers; 56..63 are special. */
    /* tp, sp, and lr must immediately follow regs[] for aliasing. */
    unsigned long regs[23]; /* callee saves r30-r52 */
    unsigned long tp;       /* thread-local data pointer (23*4) */
    unsigned long sp;       /* stack pointer (grows DOWNWARD) (23*4)+4 */
    unsigned long lr;       /* aka link register (where to go when returning from a function)
                             * (23*4)+(2*4) */

    /* Saved special registers. */
    unsigned long pc;       /* (23*4)+(3*4) */
    unsigned long r0;       /* (23*4)+(4*4) */
    // unsigned long ex1;      /* stored in EX_CONTEXT_1_1 (PL and ICS bit) */
    unsigned long arg0;     /* (23*4)+(5*4) only used for first function invocation */
    unsigned long first;    /* (23*4)+(6*4) */
};

struct uctxt {
    struct {
        void  *ss_sp;
        size_t ss_size;
    } uc_stack;
    // sigset_t uc_sigmask;
    mctxt_t       mc;
    struct uctxt *uc_link;      /* unused */
};

int INTERNAL qt_swapctxt(uctxt_t *,
                uctxt_t *);
void INTERNAL qt_makectxt(uctxt_t *, void (*)(void), int, ...);
int  INTERNAL qt_getmctxt(mctxt_t *);
void INTERNAL qt_setmctxt(mctxt_t *);
/* vim:set expandtab: */
