#ifdef HAVE_STDARG_H
# include <stdarg.h> /* for the qt_makectxt prototype */
#endif

#include "qt_visibility.h"

#define setcontext(u) qt_setmctxt(&(u)->mc)
#define getcontext(u) qt_getmctxt(&(u)->mc)
typedef struct mctxt mctxt_t;
typedef struct uctxt uctxt_t;

int INTERNAL qt_swapctxt(uctxt_t *,
                uctxt_t *);
void INTERNAL qt_makectxt(uctxt_t *, void (*)(void), int, ...);
int  INTERNAL qt_getmctxt(mctxt_t *);
void INTERNAL qt_setmctxt(mctxt_t *);

struct mctxt {
    unsigned long pc;                   /* lr */
    unsigned long cr;                   /* condition register (mfcr) */
    unsigned long ctr;                  /* count register, for branching (mfcr) */
    unsigned long xer;                  /* xer register, an optional-condition register (mfcr) */
    unsigned long sp;                   /* callee saved: r1 */
    unsigned long toc;                  /* callee saved: r2 */
    unsigned long r3;                   /* first arg to function, return register: r3 */
    unsigned long gpr[19];              /* callee saved: r13-r31 */
    /* double-aligned for performance */
    unsigned long pad;
    double        fpregs[18];           /* callee saved: r14-r31 */
/*
 * // XXX: currently do not save vector registers
 * //	unsigned long	vr[4*12];	/ * callee saved: v20-v31, 256-bits each * /
 * //      unsigned long   vrsave;         / * which v regs should be saved? * /
 */
};

struct uctxt {
    struct {
        void        *ss_sp;
        unsigned int ss_size;
    } uc_stack;
    mctxt_t       mc;
    struct uctxt *uc_link;      /* unused */
};

/* vim:set expandtab: */
