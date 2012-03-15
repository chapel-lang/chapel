#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_STDARG_H
# include <stdarg.h> /* for the qt_makectxt prototype */
#endif
#include <stddef.h> /* for size_t, per C89 */

#include "qthread-int.h"

#define setcontext(u) qt_setmctxt(&(u)->uc_mcontext)
#define getcontext(u) qt_getmctxt(&(u)->uc_mcontext)
typedef struct mctxt mctxt_t;
typedef struct uctxt uctxt_t;

int qt_swapctxt(uctxt_t *,
                uctxt_t *);
void qt_makectxt(uctxt_t *, void (*)(void), int, ...);
int  qt_getmctxt(mctxt_t *);
void qt_setmctxt(mctxt_t *);

struct mctxt {
    unsigned long mc_edi;      /* 0: 1st arg (mandatory) */
    unsigned long mc_ebp;      /* 1: Stack frame pointer (esi) */
    unsigned long mc_ebx;      /* 2: PIC base register, also general-purp. reg */
#ifdef __x86_64__
    unsigned long mc_r12;      /* 3: extra callee-saved registers */
    unsigned long mc_r13;      /* 4: extra callee-saved registers */
    unsigned long mc_r14;      /* 5: extra callee-saved registers */
    unsigned long mc_r15;      /* 6: extra callee-saved registers */
#else
    unsigned long mc_esi;      /* 3: general-purpose register */
#endif
    unsigned long mc_xcsr;     /* 7/4: SSE2 control and status word */
    unsigned long mc_cw;       /* 8/5: x87 control word */
    unsigned long mc_esp;      /* 9/6: machine state; stack pointer */
    unsigned long mc_eip;      /* 10/7: function pointer */
};

struct uctxt {
    mctxt_t uc_mcontext;
    struct {
        uint8_t *ss_sp;
        size_t   ss_size;
        int      ss_flags;
    } uc_stack;
};

/* vim:set expandtab: */
