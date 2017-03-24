#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_STDARG_H
# include <stdarg.h> /* for the qt_makectxt prototype */
#endif
#include <stddef.h> /* for size_t, per C89 */

#include "qthread-int.h"

#define setcontext(u) qt_setmctxt(&(u)->mc)
#define getcontext(u) qt_getmctxt(&(u)->mc)
typedef struct mctxt mctxt_t;
typedef struct uctxt uctxt_t;

int qt_swapctxt(uctxt_t *,
                uctxt_t *);
void qt_makectxt(uctxt_t *, void (*)(void), int, ...);
int  qt_getmctxt(mctxt_t *);
void qt_setmctxt(mctxt_t *);

#ifdef __x86_64__
typedef uint64_t qt_register_t;
#else
typedef uint32_t qt_register_t;
#endif

struct mctxt {
    qt_register_t mc_edi;      /* 0: 1st arg (mandatory) */
    qt_register_t mc_ebp;      /* 1: Stack frame pointer (esi) */
    qt_register_t mc_ebx;      /* 2: PIC base register, also general-purp. reg */
#ifdef __x86_64__
    qt_register_t mc_r12;      /* 3: extra callee-saved registers */
    qt_register_t mc_r13;      /* 4: extra callee-saved registers */
    qt_register_t mc_r14;      /* 5: extra callee-saved registers */
    qt_register_t mc_r15;      /* 6: extra callee-saved registers */
#else
    qt_register_t mc_esi;      /* 3: general-purpose register */
#endif
    qt_register_t mc_esp;      /* 7/4: machine state; stack pointer */
    qt_register_t mc_eip;      /* 8/5: function pointer */
    uint32_t mc_xcsr;          /* 9/6: SSE2 control and status word */
    uint32_t mc_cw;            /* 9+4/6+4: x87 control word */
};

struct uctxt {
    mctxt_t mc;
    struct {
        uint8_t *ss_sp;
        size_t   ss_size;
        int      ss_flags;
    } uc_stack;
};

/* vim:set expandtab: */
