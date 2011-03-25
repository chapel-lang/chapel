#define	setcontext(u)	_setmcontext(&(u)->mc)
#define	getcontext(u)	_getmcontext(&(u)->mc)
typedef struct mcontext mcontext_t;
typedef struct ucontext ucontext_t;
/*
 * This struct defines the way the registers are stored on the stack during a
 * system call/exception.  It should be a multiple of 8 bytes to preserve
 * normal stack alignment rules.
 *
 */
struct mcontext {
    /* Saved main processor registers; 56..63 are special. */
    /* tp, sp, and lr must immediately follow regs[] for aliasing. */
    unsigned long regs[23]; /* callee saves r30-r52 */
    unsigned long tp;       /* thread-local data pointer (23*4) */
    unsigned long sp;       /* stack pointer (grows DOWNWARD) (23*4)+4 */
    unsigned long lr;       /* aka link register (where to go when returning from a function)
                               (23*4)+(2*4) */

    /* Saved special registers. */
    unsigned long pc;       /* (23*4)+(3*4) */
    unsigned long r0;       /* (23*4)+(4*4) */
    //unsigned long ex1;      /* stored in EX_CONTEXT_1_1 (PL and ICS bit) */
    unsigned long arg0;     /* (23*4)+(5*4) only used for first function invocation */
    unsigned long first;    /* (23*4)+(6*4) */
};


struct ucontext
{
	struct {
		void *ss_sp;
		size_t ss_size;
	} uc_stack;
	//sigset_t uc_sigmask;
	mcontext_t mc;
	struct ucontext * uc_link; /* unused */
};

int _getmcontext(mcontext_t*);
void _setmcontext(mcontext_t*);
