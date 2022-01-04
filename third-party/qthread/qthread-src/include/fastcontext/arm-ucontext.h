#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <stddef.h> /* for size_t, per C89 */
#include "qthread/qthread-int.h" /* for uint32_t */

#include "qt_visibility.h"

#define setcontext(u) qt_setmctxt(&(u)->mc)
#define getcontext(u) qt_getmctxt(&(u)->mc)
typedef struct mctxt mctxt_t;
typedef struct uctxt uctxt_t;

struct mctxt {
    /* Saved main processor registers. */
    uint32_t regs[16]; /* callee saves r0-r15 */
    char first;
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
int INTERNAL  qt_getmctxt(mctxt_t *);
void INTERNAL qt_setmctxt(mctxt_t *);
/* vim:set expandtab: */
