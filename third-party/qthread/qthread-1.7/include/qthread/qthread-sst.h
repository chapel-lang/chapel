#ifndef _QTHREAD_SST_H_
#define _QTHREAD_SST_H_

#include <errno.h>		       /* for ENOMEM */

#include <qthread/qthread-int.h>       /* for uint32_t and uint64_t */
#include <qthread/common.h>	       /* important configuration options */

#include <string.h>		       /* for memcpy() */
#include <ppcPimCalls.h>

/*****************************************************************************
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *
 * !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  NOTE  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! *
 *                                                                           *
 *    The most complete documentaton is going to be in the man pages. The    *
 *    documentation here is just to give you a general idea of what each     *
 *    function does.                                                         *
 *                                                                           *
 *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *   *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *****************************************************************************/

#ifdef __cplusplus
#define Q_STARTCXX extern "C" {
#define Q_ENDCXX }
#else
#define Q_STARTCXX
#define Q_ENDCXX
#endif

Q_STARTCXX /* */
typedef unsigned short qthread_shepherd_id_t;	/* doubt we'll run more than 65k shepherds */

/* for convenient arguments to qthread_fork */
typedef aligned_t(*qthread_f) (void *arg);

/* While this function is *required* for UNIX, in a PIM environment, it serves
 * primarily to prove that qthreads are being used (thus the quickPrint call
 * with the "scalable" argument) */
#define qthread_init(x) PIM_quickPrint(0x5ca1ab1e,x,PIM_readSpecial(PIM_CMD_LOC_COUNT))
#define qthread_initialize() PIM_quickPrint(0x5ca1ab1e,0,PIM_readSpecial(PIM_CMD_LOC_COUNT))

/* XXX: not sure how to handle this in a truly multithreaded environment */
#define qthread_finalize() PIM_quickPrint(0xaced,0,0)

/* means nothing in a truly multithreaded environment */
#define qthread_yield(x)

/* cannot be done without hardware support */
#define qthread_disable_shepherd(x)
#define qthread_enable_shepherd(x)

/* these are the functions for generating a new qthread.
 *
 * Using qthread_fork() and variants:
 *
 *     The specified function will be run to completion. You can detect that a
 *     thread has finished by specifying a location to store the return value
 *     (which will be stored with a qthread_writeF call). The qthread_fork_to
 *     function spawns the thread to a specific shepherd.
 *
 * In the SST PIM environment, the shepherd is the CPU ID number.
 */
#define qthread_fork(f, arg, ret) qthread_fork_to((f), (arg), (ret), NO_SHEPHERD)
#define qthread_fork_syncvar(f, arg, ret) qthread_fork_to((f), (arg), (aligned_t*)(ret), NO_SHEPHERD)
int qthread_fork_to(const qthread_f f, const void *arg, aligned_t * ret,
		    const qthread_shepherd_id_t shepherd);
#define qthread_fork_syncvar_to(f, arg, ret, shep) qthread_fork_to((f), (arg), (aligned_t*)(ret), (shep))

static inline
unsigned qthread_migrate_to(const int shepherd)
{
    __asm__ __volatile__ (
	    "mr r3, %1\n\t" /* put the stack ptr in the argument */
	    "li r0, %0\n\t"
	    "sc"
	    ::"M"(SS_PIM_MOVE_TO),
	    "r"(shepherd)
	    :"r0","r3");
    return 0;
}

/* these are accessor functions for use by the qthreads to retrieve information
 * about themselves */
static inline
unsigned qthread_id(void)
{
    return PIM_readSpecial(PIM_CMD_THREAD_SEQ);
}
static inline
qthread_shepherd_id_t qthread_shep(void)
{
    return PIM_readSpecial(PIM_CMD_PROC_NUM);
}
static inline
size_t qthread_stackleft(void)
{
    return 0;			       /* XXX: this is a bug! */
}
static inline
aligned_t *qthread_retloc(void)
{
    return 0;			       /* XXX: this is a bug! */
}
static inline
int qthread_shep_ok(void)
{
    return 1;
}

/* returns the distance from one shepherd to another */
static inline
int qthread_distance(const qthread_shepherd_id_t src,
		     const qthread_shepherd_id_t dest)
{
    return 0;			       /* XXX: this is a bug! */
}
/* returns a list of shepherds, sorted by their distance from either this
 * qthread or the specified shepherd */
static inline
const qthread_shepherd_id_t *qthread_sorted_sheps(void)
{
    return NULL;
}
static inline
const qthread_shepherd_id_t *qthread_sorted_sheps_remote(const
							 qthread_shepherd_id_t
							 src)
{
    return NULL;
}
/* returns the number of shepherds (i.e. one more than the largest valid shepherd id) */
#define qthread_num_shepherds() ((qthread_shepherd_id_t) PIM_readSpecial(PIM_CMD_LOC_COUNT))

/****************************************************************************
 * functions to implement FEB locking/unlocking
 ****************************************************************************
 *
 * These are the FEB functions. All but empty/fill have the potential of
 * blocking until the corresponding precondition is met. All FEB
 * blocking/reading/writing is done on a machine-word basis. Memory is assumed
 * to be full unless otherwise asserted, and as such memory that is full and
 * does not have dependencies (i.e. no threads are waiting for it to become
 * empty) does not require state data to be stored. It is expected that while
 * there may be locks instantiated at one time or another for a very large
 * number of addresses in the system, relatively few will be in a non-default
 * (full, no waiters) state at any one time.
 */

/* This function is just to assist with debugging; it returns 1 if the address
 * is full, and 0 if the address is empty */
static inline int qthread_feb_status(const aligned_t *addr)
{
    return PIM_feb_is_full((unsigned int*)addr);
}
#define qthread_syncvar_status(addr) qthread_feb_status((const aligned_t*)addr)

/* The empty/fill functions merely assert the empty or full state of the given
 * address. */
static inline
int qthread_empty(const aligned_t *dest)
{
    PIM_feb_empty((unsigned int*)dest);
    return 0;
}
#define qthread_syncvar_empty(me, dest) qthread_empty((me), (aligned_t*)(dest))
static inline
int qthread_fill(const aligned_t *dest)
{
    PIM_feb_fill((unsigned int*)dest);
    return 0;
}
#define qthread_syncvar_fill(me, dest) qthread_fill((me), (aligned_t*)(dest))

/* These functions wait for memory to become empty, and then fill it. When
 * memory becomes empty, only one thread blocked like this will be awoken. Data
 * is read from src and written to dest.
 *
 * The semantics of writeEF are:
 * 1 - destination's FEB state must be "empty"
 * 2 - data is copied from src to destination
 * 3 - the destination's FEB state gets changed from empty to full
 */
static inline
int qthread_writeEF(aligned_t * const dest,
		    const aligned_t * const src)
{
    PIM_feb_writeef(dest, *src);
    return 0;
}
static inline
int qthread_writeEF_const(aligned_t * const dest,
			  const aligned_t src)
{
    PIM_feb_writeef(dest, src);
    return 0;
}
#define qthread_syncvar_writeEF(me, dest, src) qthread_writeEF((me), (aligned_t*)(dest), (aligned_t*)(src))
#define qthread_syncvar_writeEF_const(me, dest, src) qthread_writeEF_const((me), (aligned_t*)(dest), (aligned_t*)(src))

/* This function is a cross between qthread_fill() and qthread_writeEF(). It
 * does not wait for memory to become empty, but performs the write and sets
 * the state to full atomically with respect to other FEB-based actions. Data
 * is read from src and written to dest.
 *
 * The semantics of writeF are:
 * 1 - data is copied from src to destination
 * 2 - the destination's FEB state gets set to full
 */
static inline int qthread_writeF(aligned_t * const dest,
				 const aligned_t * const src)
{
    *dest = *src;
    PIM_feb_fill(dest);
    return 0;
}
static inline int qthread_writeF_const(aligned_t * const dest,
				       const aligned_t src)
{
    *dest = src;
    PIM_feb_fill(dest);
    return 0;
}
#define qthread_syncvar_writeF(me, dest, src) qthread_writeF((me), (aligned_t*)(dest), (aligned_t*)(src))
#define qthread_syncvar_writeF_const(me, dest, src) qthread_writeF_const((me), (aligned_t*)(dest), (aligned_t)(src))

/* This function waits for memory to become full, and then reads it and leaves
 * the memory as full. When memory becomes full, all threads waiting for it to
 * become full with a readFF will receive the value at once and will be queued
 * to run. Data is read from src and stored in dest.
 *
 * The semantics of readFF are:
 * 1 - src's FEB state must be "full"
 * 2 - data is copied from src to destination
 */
static inline
int qthread_readFF(aligned_t * const dest,
		   const aligned_t * const src)
{
    if (dest != NULL && dest != src) {
	*dest = PIM_feb_readff((aligned_t * const)src);
    } else {
	PIM_feb_readff((aligned_t * const)src);
    }
    return 0;
}
#define qthread_syncvar_readFF(me, dest, src) qthread_readFF((me), (aligned_t*)(dest), (aligned_t*)(src))

/* These functions wait for memory to become full, and then empty it. When
 * memory becomes full, only one thread blocked like this will be awoken. Data
 * is read from src and written to dest.
 *
 * The semantics of readFE are:
 * 1 - src's FEB state must be "full"
 * 2 - data is copied from src to destination
 * 3 - the src's FEB bits get changed from full to empty when the data is copied
 */
static inline
int qthread_readFE(aligned_t * restrict const dest,
		   const aligned_t * restrict const src)
{
    if (dest != NULL && dest != src) {
	*dest = PIM_feb_readfe((aligned_t * const)src);
    } else {
	PIM_feb_readfe((aligned_t * const)src);
    }
    return 0;
}
#define qthread_syncvar_readFE(me, dest, src) qthread_readFE((me), (aligned_t*)(dest), (aligned_t*)(src))

/* functions to implement FEB-ish locking/unlocking
 *
 * These are atomic and functional, but do not have the same semantics as full
 * FEB locking/unlocking (namely, unlocking cannot block), however because of
 * this, they have lower overhead.
 */
static inline
int qthread_lock(const aligned_t * a)
{
    PIM_feb_lock((aligned_t * const)a);
    return 0;
}
static inline
int qthread_unlock(const aligned_t * a)
{
    PIM_feb_unlock((aligned_t * const)a);
    return 0;
}

/* the following three functions implement variations on atomic increment. It
 * is done with architecture-specific assembly (on supported architectures,
 * when possible) and does NOT use FEB's or lock/unlock unless the architecture
 * is unsupported or cannot perform atomic operations at the right granularity.
 * All of these functions return the value of the contents of the operand
 * *after* incrementing.
 */

static QINLINE float qthread_fincr(volatile float *operand, const float incr)
{				       /*{{{ */
#if defined(HAVE_GCC_INLINE_ASSEMBLY)
# if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64)
    union
    {
	float f;
	uint32_t i;
    } retval;
    register float incremented_value;
    register uint32_t scratch_int;
    uint32_t conversion_memory = conversion_memory;
    __asm__ __volatile__("1:\n\t"
	    "lwarx  %0,0,%4\n\t"
	    // convert from int to float
	    "stw    %0,%2\n\t"
	    "lfs    %1,%2\n\t"
	    // do the addition
	    "fadds  %1,%1,%5\n\t"
	    // convert from float to int
	    "stfs   %1,%2\n\t"
	    "lwz    %3,%2\n\t"
	    // store back to original location
	    "stwcx. %3,0,%4\n\t"
	    "bne-   1b\n\t"
	    "isync"
	    :"=&b" (retval.i),		/* %0 */
	     "=&f" (incremented_value),	/* %1 */
	     "=m"  (conversion_memory),	/* %2 */
	     "=&r" (scratch_int)	/* %3 */
	    :"r"   (operand),		/* %4 */
	     "f"   (incr)		/* %5 */
	    :"cc", "memory");

    return retval.f;
# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_64) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_32)
    union
    {
	float f;
	uint32_t i;
    } oldval, newval;

    /* newval.f = *operand; */
    do {
	/* you *should* be able to move the *operand reference outside the
	 * loop and use the output of the CAS (namely, newval) instead.
	 * However, there seems to be a bug in gcc 4.0.4 wherein, if you do
	 * that, the while() comparison uses a temporary register value for
	 * newval that has nothing to do with the output of the CAS
	 * instruction. (See how obviously wrong that is?) For some reason that
	 * I haven't been able to figure out, moving the *operand reference
	 * inside the loop fixes that problem, even at -O2 optimization. */
	oldval.f = *operand;
	newval.f = oldval.f + incr;
#if defined(__SUNPRO_CC)
	asm volatile
#else
	__asm__ __volatile__
#endif
	        ("membar #StoreStore|#LoadStore|#StoreLoad|#LoadLoad\n\t"
		 "cas [%1], %2, %0"
		 :"=&r"(newval.i)
		 :"r"    (operand), "r"(oldval.i), "0"(newval.i)
		 :"cc", "memory");
    } while (oldval.i != newval.i);
    return oldval.f;
# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA64)
    union
    {
	float f;
	uint32_t i;
    } oldval, newval, res;

    do {
	oldval.f = *operand;
	newval.f = oldval.f + incr;
	__asm__ __volatile__("mov ar.ccv=%0;;"::"rO"(oldval.i));
	__asm__ __volatile__("cmpxchg4.acq %0=[%1],%2,ar.ccv"
			     :"=r"(res.i)
			     :"r"    (operand), "r"(newval.i)
			     :"memory");
    } while (res.i != oldval.i);       /* if res!=old, the calc is out of date */
    return oldval.f;
# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA32)
    union
    {
	float f;
	uint32_t i;
    } oldval, newval, retval;

    do {
	oldval.f = *operand;
	newval.f = oldval.f + incr;
	__asm__ __volatile__("lock; cmpxchg %1, (%2)"
			     :"=a"(retval.i)	/* store from EAX */
			     :"r"    (newval.i),
			      "r"(operand),
			      "0"(oldval.i)	/* load into EAX */
			     :"cc", "memory");
    } while (retval.i != oldval.i);
    return oldval.f;
# endif
#elif defined (QTHREAD_MUTEX_INCREMENT)

    float retval;

    qthread_lock((aligned_t *) operand);
    retval = *operand;
    *operand += incr;
    qthread_unlock((aligned_t *) operand);
    return retval;
#else
#error "Neither atomic nor mutex increment enabled; needed for qthread_fincr"
#endif
}				       /*}}} */

static QINLINE double qthread_dincr(volatile double *operand,
				    const double incr)
{				       /*{{{ */
#if defined(HAVE_GCC_INLINE_ASSEMBLY) && (QTHREAD_ASSEMBLY_ARCH != QTHREAD_POWERPC32)
#if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64)
    register uint64_t scratch_int;
    register double incremented_value;
    union
    {
	uint64_t i;
	double d;
    } retval;
    uint64_t conversion_memory = conversion_memory;
    __asm__ __volatile__("1:\n\t"
			 "ldarx %0,0,%4\n\t"
			 /*convert from integer to floating point */
			 "std   %0,%2\n\t"	// %2 is scratch memory (NOT a register)
			 "lfd   %1,%2\n\t"	// %1 is a scratch floating point register
			 /* do the addition */
			 "fadd  %1,%1,%5\n\t"	// %4 is the input increment
			 /* convert from floating point to integer */
			 "stfd   %1,%2\n\t"
			 "ld     %3,%2\n\t"
			 /* store back to original location */
			 "stdcx. %3,0,%4\n\t"
			 "bne-   1b\n\t"
			 "isync"
			 :"=&b" (retval.i),		/* %0 */
			  "=&f" (incremented_value),	/* %1 */
			  "=m"  (conversion_memory),	/* %2 */
			  "=r&" (scratch_int)		/* %3 */
			 :"r"   (operand),		/* %4 */
			  "f"   (incr)			/* %5 */
			 :"cc", "memory");

    return retval.d;
#elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_32)
    double oldval, newval;

    newval = *operand;
    do {
	/* this allows the compiler to be as flexible as possible with register
	 * assignments */
	register uint64_t tmp1 = tmp1;
	register uint64_t tmp2 = tmp2;

	oldval = newval;
	newval = oldval + incr;
	__asm__ __volatile__("ldx %0, %1\n\t"
			     "ldx %4, %2\n\t"
			     "membar #StoreStore|#LoadStore|#StoreLoad|#LoadLoad\n\t"
			     "casx [%3], %2, %1\n\t"
			     "stx %1, %0"
			     /* h means 64-BIT REGISTER
			      * (probably unnecessary, but why take chances?) */
			     :"=m"   (newval), "=&h"(tmp1), "=&h"(tmp2)
			     :"r"    (operand), "m"(oldval)
			     :"memory");
    } while (oldval != newval);
    return oldval;
#elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_64)
    union
    {
	uint64_t i;
	double d;
    } oldval, newval;

    /*newval.d = *operand; */
    do {
	/* you *should* be able to move the *operand reference outside the
	 * loop and use the output of the CAS (namely, newval) instead.
	 * However, there seems to be a bug in gcc 4.0.4 wherein, if you do
	 * that, the while() comparison uses a temporary register value for
	 * newval that has nothing to do with the output of the CAS
	 * instruction. (See how obviously wrong that is?) For some reason that
	 * I haven't been able to figure out, moving the *operand reference
	 * inside the loop fixes that problem, even at -O2 optimization. */
	oldval.d = *operand;
	newval.d = oldval.d + incr;
#if defined(__SUNPRO_CC)
	asm volatile
#else
	__asm__ __volatile__
#endif
	        ("membar #StoreStore|#LoadStore|#StoreLoad|#LoadLoad\n\t"
		 "casx [%1], %2, %0"
		 :"=&r"(newval.i)
		 :"r"(operand), "r"(oldval.i), "0"(newval.i)
		 :"memory");
    } while (oldval.d != newval.d);
    return oldval.d;
#elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA64)
    union
    {
	uint64_t i;
	double d;
    } oldval, newval, res;

    do {
	oldval.d = *operand;
	newval.d = oldval.d + incr;
	__asm__ __volatile__("mov ar.ccv=%0;;"::"rO"(oldval.i));
	__asm__ __volatile__("cmpxchg8.acq %0=[%1],%2,ar.ccv"
			     :"=r"(res.i)
			     :"r"    (operand), "r"(newval.i)
			     :"memory");
    } while (res.i != oldval.i);       /* if res!=old, the calc is out of date */
    return oldval.d;

#elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64)
    union
    {
	double d;
	uint64_t i;
    } oldval, newval, retval;

    do {
	oldval.d = *operand;
	newval.d = oldval.d + incr;
	__asm__ __volatile__("lock; cmpxchgq %1, (%2)"
			     :"=a"(retval.i)
			     :"r"(newval.i), "r"(operand),
			      "0"(oldval.i)
			     :"memory");
    } while (retval.i != oldval.i);
    return oldval.d;

#elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA32)
    union
    {
	double d;
	uint64_t i;
	struct
	{
	    /* note: the ordering of these is both important and
	     * counter-intuitive; welcome to little-endian! */
	    uint32_t l;
	    uint32_t h;
	} s;
    } oldval, newval;
    register char test;

    do {
#ifdef __PIC__
       /* this saves off %ebx to make PIC code happy :P */
# define QTHREAD_PIC_PREFIX "xchg %%ebx, %4\n\t"
       /* this restores it */
# define QTHREAD_PIC_SUFFIX "\n\txchg %%ebx, %4"
# define QTHREAD_PIC_REG_4 "r"
#else
# define QTHREAD_PIC_PREFIX
# define QTHREAD_PIC_SUFFIX
# define QTHREAD_PIC_REG_4 "b"
#endif
	oldval.d = *operand;
	newval.d = oldval.d + incr;
	/* Yeah, this is weird looking, but it really makes sense when you
	 * understand the instruction's semantics (which make sense when you
	 * consider that it's doing a 64-bit op on a 32-bit proc):
	 *
	 *    Compares the 64-bit value in EDX:EAX with the operand
	 *    (destination operand). If the values are equal, the 64-bit value
	 *    in ECX:EBX is stored in the destination operand. Otherwise, the
	 *    value in the destination operand is loaded into EDX:EAX."
	 *
	 * So what happens is the oldval is loaded into EDX:EAX and the newval
	 * is loaded into ECX:EBX to start with (i.e. as inputs). Then
	 * CMPXCHG8B does its business, after which EDX:EAX is guaranteed to
	 * contain the value of *operand when the instruction executed. We test
	 * the ZF field to see if the operation succeeded. We *COULD* save
	 * EDX:EAX back into oldval to save ourselves a step when the loop
	 * fails, but that's a waste when the loop succeeds (i.e. in the common
	 * case). Optimizing for the common case, in this situation, means
	 * minimizing our extra write-out to the one-byte test variable.
	 */
	__asm__ __volatile__(QTHREAD_PIC_PREFIX
			     "lock; cmpxchg8b (%1)\n\t"
			     "setne %0"	/* test = (ZF==0) */
			     QTHREAD_PIC_SUFFIX
			     :"=q"(test)
			     :"r"(operand),
			     /*EAX*/ "a"(oldval.s.l),
			     /*EDX*/ "d"(oldval.s.h),
			     /*EBX*/ QTHREAD_PIC_REG_4(newval.s.l),
			     /*ECX*/ "c"(newval.s.h)
			     :"memory");
    } while (test);		       /* if ZF was cleared, the calculation is out of date */
    return oldval.d;

#else
#error "Unimplemented assembly architecture"
#endif
#elif defined (QTHREAD_MUTEX_INCREMENT) || (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32)

    double retval;

    qthread_lock((aligned_t *) operand);
    retval = *operand;
    *operand += incr;
    qthread_unlock((aligned_t *) operand);
    return retval;
#else
#error "Neither atomic nor mutex increment enabled; needed for qthread_dincr"
#endif
}				       /*}}} */

static QINLINE uint32_t qthread_incr32(volatile uint32_t * operand,
				       const int incr)
{				       /*{{{ */
#if defined(HAVE_GCC_INLINE_ASSEMBLY)

#if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32) || \
    (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64)
    uint32_t retval;
    register unsigned int incrd = incrd;	/* no initializing */
    __asm__ __volatile__("1:\tlwarx  %0,0,%1\n\t"
			 "add    %3,%0,%2\n\t"
			 "stwcx. %3,0,%1\n\t"
			 "bne-   1b\n\t"	/* if it failed, try again */
			 "isync"	/* make sure it wasn't all a dream */
			 :"=&b"  (retval)
			 :"r"    (operand), "r"(incr), "r"(incrd)
			 :"cc", "memory");

    return retval;
#elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_32) || \
      (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_64)
    register uint32_t oldval, newval;

    /* newval = *operand; */
    do {
	/* you *should* be able to move the *operand reference outside the
	 * loop and use the output of the CAS (namely, newval) instead.
	 * However, there seems to be a bug in gcc 4.0.4 wherein, if you do
	 * that, the while() comparison uses a temporary register value for
	 * newval that has nothing to do with the output of the CAS
	 * instruction. (See how obviously wrong that is?) For some reason that
	 * I haven't been able to figure out, moving the *operand reference
	 * inside the loop fixes that problem, even at -O2 optimization. */
	oldval = *operand;
	newval = oldval + incr;
	/* newval always gets the value of *operand; if it's
	 * the same as oldval, then the swap was successful */
#if defined(__SUNPRO_CC)
	asm volatile
#else
	__asm__ __volatile__
#endif
	        ("membar #StoreStore|#LoadStore|#StoreLoad|#LoadLoad\n\t"
		 "cas [%1] , %2, %0"
		 :"=&r"  (newval)
		 :"r"    (operand), "r"(oldval), "0"(newval)
		 :"cc", "memory");
    } while (oldval != newval);
    return oldval;
#elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA64)
    uint32_t res;

    if (incr == 1) {
	asm volatile ("fetchadd4.rel %0=[%1],1"
		      :"=r" (res)
		      :"r"  (operand));
    } else {
	uint32_t old, newval;

	do {
	    old = *operand;	       /* atomic, because operand is aligned */
	    newval = old + incr;
	    asm volatile ("mov ar.ccv=%0;;":	/* no output */
			  :"rO"    (old));

	    /* separate so the compiler can insert its junk */
	    asm volatile ("cmpxchg4.acq %0=[%1],%2,ar.ccv"
			  :"=r"(res)
			  :"r" (operand), "r"(newval)
			  :"memory");
	} while (res != old);	       /* if res!=old, the calc is out of date */
    }
    return res;
#elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA32) || \
      (QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64)

    uint32_t retval = incr;
    asm volatile ("lock ;  xaddl %0, (%1);"
		  :"=r" (retval)
		  :"r"  (operand), "0"(retval)
		  :"memory");

    return retval;
#else

#error "Unimplemented assembly architecture"

#endif

#elif defined(QTHREAD_MUTEX_INCREMENT)
    uint32_t retval;

    qthread_lock((aligned_t *) operand);
    retval = *operand;
    *operand += incr;
    qthread_unlock((aligned_t *) operand);
    return retval;
#else

#error "Architecture unsupported for 32-bit atomic ops, and FEB increment not enabled"

#endif
}				       /*}}} */

static QINLINE uint64_t qthread_incr64(volatile uint64_t * operand,
				       const int incr)
{				       /*{{{ */
#if defined(HAVE_GCC_INLINE_ASSEMBLY)

#if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64)
    uint64_t retval;
    register uint64_t incrd = incrd;	/* no initializing */

    asm volatile ("1:\tldarx  %0,0,%1\n\t"
		  "add    %3,%0,%2\n\t"
		  "stdcx. %3,0,%1\n\t"
		  "bne-   1b\n\t"	/* if it failed, try again */
		  "isync"	/* make sure it wasn't all a dream */
		  :"=&b"   (retval)
		  :"r"     (operand), "r"(incr), "r"(incrd)
		  :"cc", "memory");

    return retval;
#elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_32)
    uint64_t oldval, newval = *operand;

    do {
	/* this allows the compiler to be as flexible as possible with register
	 * assignments */
	register uint64_t tmp1 = tmp1;
	register uint64_t tmp2 = tmp2;

	oldval = newval;
	newval += incr;
	/* newval always gets the value of *operand; if it's
	 * the same as oldval, then the swap was successful */
	__asm__ __volatile__("ldx %0, %1\n\t"
			     "ldx %4, %2\n\t"
			     "membar #StoreStore|#LoadStore|#StoreLoad|#LoadLoad\n\t"
			     "casx [%3] , %2, %1\n\t"
			     "stx %1, %0"
			     /* h means 64-BIT REGISTER
			      * (probably unnecessary, but why take chances?) */
			     :"=m"   (newval), "=&h"(tmp1), "=&h"(tmp2)
			     :"r"    (operand), "m"(oldval)
			     :"cc", "memory");
    } while (oldval != newval);
    return oldval;
#elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_64)
    register uint64_t oldval, newval;

    /* newval = *operand; */
    do {
	/* you *should* be able to move the *operand reference outside the
	 * loop and use the output of the CAS (namely, newval) instead.
	 * However, there seems to be a bug in gcc 4.0.4 wherein, if you do
	 * that, the while() comparison uses a temporary register value for
	 * newval that has nothing to do with the output of the CAS
	 * instruction. (See how obviously wrong that is?) For some reason that
	 * I haven't been able to figure out, moving the *operand reference
	 * inside the loop fixes that problem, even at -O2 optimization. */
	oldval = *operand;
	newval = oldval + incr;
	/* newval always gets the value of *operand; if it's
	 * the same as oldval, then the swap was successful */
#if defined(__SUNPRO_CC)
	/* Sun's C++ compiler doesn't do __asm__, but their C compiler does
	 * :P */
	asm volatile
#else
	__asm__ __volatile__
#endif
	        ("membar #StoreStore|#LoadStore|#StoreLoad|#LoadLoad\n\t"
		 "casx [%1] , %2, %0"
		 :"=&r"(newval)
		 :"r"    (operand), "r"(oldval), "0"(newval)
		 :"cc", "memory");
    } while (oldval != newval);
    return oldval;
#elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA64)
    uint64_t res;

    if (incr == 1) {
	asm volatile ("fetchadd8.rel %0=%1,1"
		      :"=r" (res)
		      :"m"     (*operand));
    } else {
	uint64_t old, newval;

	do {
	    old = *operand;	       /* atomic, because operand is aligned */
	    newval = old + incr;
	    asm volatile ("mov ar.ccv=%0;;":	/* no output */
			  :"rO"    (old));

	    /* separate so the compiler can insert its junk */
	    asm volatile ("cmpxchg8.acq %0=[%1],%2,ar.ccv"
			  :"=r" (res)
			  :"r"     (operand), "r"(newval)
			  :"memory");
	} while (res != old);	       /* if res!=old, the calc is out of date */
    }
    return res;
#elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA32)
    union
    {
	uint64_t i;
	struct
	{
	    /* note: the ordering of these is both important and
	     * counter-intuitive; welcome to little-endian! */
	    uint32_t l;
	    uint32_t h;
	} s;
    } oldval, newval;
    register char test;

    do {
#ifndef QTHREAD_PIC_PREFIX
# ifdef __PIC__
	/* should share this code with the dincr stuff */
	/* this saves off %ebx to make PIC code happy :P */
#  define QTHREAD_PIC_PREFIX "xchg %%ebx, %4\n\t"
	/* this restores it */
#  define QTHREAD_PIC_SUFFIX "\n\txchg %%ebx, %4"
#  define QTHREAD_PIC_REG_4 "r"
# else
#  define QTHREAD_PIC_PREFIX
#  define QTHREAD_PIC_SUFFIX
#  define QTHREAD_PIC_REG_4 "b"
# endif
#endif
	oldval.i = *operand;
	newval.i = oldval.i + incr;
	/* Yeah, this is weird looking, but it really makes sense when you
	 * understand the instruction's semantics (which make sense when you
	 * consider that it's doing a 64-bit op on a 32-bit proc):
	 *
	 *    Compares the 64-bit value in EDX:EAX with the operand
	 *    (destination operand). If the values are equal, the 64-bit value
	 *    in ECX:EBX is stored in the destination operand. Otherwise, the
	 *    value in the destination operand is loaded into EDX:EAX."
	 *
	 * So what happens is the oldval is loaded into EDX:EAX and the newval
	 * is loaded into ECX:EBX to start with (i.e. as inputs). Then
	 * CMPXCHG8B does its business, after which EDX:EAX is guaranteed to
	 * contain the value of *operand when the instruction executed. We test
	 * the ZF field to see if the operation succeeded. We *COULD* save
	 * EDX:EAX back into oldval to save ourselves a step when the loop
	 * fails, but that's a waste when the loop succeeds (i.e. in the common
	 * case). Optimizing for the common case, in this situation, means
	 * minimizing our extra write-out to the one-byte test variable.
	 */
	__asm__ __volatile__(QTHREAD_PIC_PREFIX
			     "lock; cmpxchg8b (%1)\n\t"
			     "setne %0"	/* test = (ZF==0) */
			     QTHREAD_PIC_SUFFIX
			     :"=q"(test)
			     :"r"    (operand),
			     /*EAX*/ "a"(oldval.s.l),
			     /*EDX*/ "d"(oldval.s.h),
			     /*EBX*/ QTHREAD_PIC_REG_4(newval.s.l),
			     /*ECX*/ "c"(newval.s.h)
			     :"memory");
    } while (test);		       /* if ZF was cleared, the calculation is out of date */
    return oldval.i;
#elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64)
    uint64_t retval = incr;

    asm volatile ("lock ; xaddq %0, (%1);"
		  :"=r" (retval)
		  :"r"     (operand), "0"(retval)
		  :"memory");

    return retval;

#elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32) || \
      (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_32)

    /* In general, RISC doesn't provide a way to do 64 bit
     * operations from 32 bit code.  Sorry. */
    uint64_t retval;

    qthread_lock((aligned_t *) operand);
    retval = *operand;
    *operand += incr;
    qthread_unlock((aligned_t *) operand);
    return retval;

#else

#error "Unimplemented assembly architecture"

#endif

#elif defined(QTHREAD_MUTEX_INCREMENT)

    uint64_t retval;

    qthread_lock((aligned_t *) operand);
    retval = *operand;
    *operand += incr;
    qthread_unlock((aligned_t *) operand);
    return retval;

#else

#error "Architecture unsupported for 64-bit atomic ops, and FEB increment not enabled"

#endif
}				       /*}}} */

static QINLINE unsigned long qthread_incr_xx(volatile void *addr, const int incr,
					     const size_t length)
{
    switch (length) {
	case 4:
	    return qthread_incr32((volatile uint32_t *)addr, incr);
	case 8:
	    return qthread_incr64((volatile uint64_t *)addr, incr);
	default:
	    /* This should never happen, so deliberately cause a seg fault
	     * for corefile analysis */
	    *(int *)(0) = 0;
    }
    return 0;			       /* compiler check */
}

static QINLINE uint32_t qthread_cas32(volatile uint32_t * operand,
				      const uint32_t oldval,
				      const uint32_t newval)
{				       /*{{{ */
#if defined(HAVE_GCC_INLINE_ASSEMBLY)
# if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32) || \
      (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64)
    register uint32_t result;
    __asm__ __volatile__ ("1:\n\t"
	    "lwarx  %0,0,%3\n\t"
	    "cmpw   %0,%1\n\t"
	    "bne    2f\n\t"
	    "stwcx. %2,0,%3\n\t"
	    "bne-   1b\n"
	    "2:"
	    "isync" /* make sure it wasn't all a dream */
	    :"=&b" (result)
	    :"r"(oldval), "r"(newval), "r"(operand)
	    :"cc", "memory");
    return result;
# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_32) || \
	(QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_64)
    register uint32_t newv = newval;
#  if defined(__SUNPRO_CC)
    asm volatile
#  else
    __asm__ __volatile__
#  endif
	("membar #StoreStore|#LoadStore|#StoreLoad|#LoadLoad\n\t"
	 "cas [%1], %2, %0"
	 : "=&r" (newv)
	 : "r" (operand), "r"(oldval), "0"(newv)
	 : "cc", "memory");
    return newv;
# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA64)
    register uint32_t retval;
    __asm__ __volatile__ ("mov ar.ccv=%0;;": :"rO" (oldval));
    __asm__ __volatile__ ("cmpxchg4.acq %0=[%1],%2,ar.ccv"
	    :"=r"(retval)
	    :"r"(operand), "r"(newval)
	    :"memory");
    return retval;
# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64) || \
        (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA32)
    uint32_t retval;
    /* note that this is GNU/Linux syntax (aka AT&T syntax), not Intel syntax.
     * Thus, this instruction has the form:
     * [lock] cmpxchg reg, reg/mem
     *                src, dest
     */
    __asm__ __volatile__ ("lock; cmpxchg %1,(%2)"
	    : "=&a"(retval) /* store from EAX */
	    : "r"(newval), "r" (operand),
	      "0"(oldval) /* load into EAX */
	    :"cc","memory");
    return retval;
# else
#  error "Don't have a qthread_cas implementation for this architecture"
# endif
#else
# error "CAS needs inline assembly OR __sync_val_compare_and_swap"
#endif
}				       /*}}} */

static QINLINE uint64_t qthread_cas64(volatile uint64_t * operand,
				      const uint64_t oldval,
				      const uint64_t newval)
{				       /*{{{ */
#if defined(HAVE_GCC_INLINE_ASSEMBLY)
# if (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC64)
    register uint64_t result;
    __asm__ __volatile__ ("1:\n\t"
	    "ldarx  %0,0,%3\n\t"
	    "cmpw   %0,%1\n\t"
	    "bne    2f\n\t"
	    "stdcx. %2,0,%3\n\t"
	    "bne-   1b\n"
	    "2:"
	    "isync" /* make sure it wasn't all a dream */
	    :"=&b" (result)
	    :"r"(oldval), "r"(newval), "r"(operand)
	    :"cc", "memory");
    return result;
# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_32)
    register uint64_t tmp1=tmp1;
    register uint64_t tmp2=tmp2;
    uint64_t newv = newval;
#  if defined(__SUNPRO_CC)
    asm volatile
#  else
    __asm__ __volatile__
#  endif
	("ldx %0, %1\n\t"
	 "ldx %4, %2\n\t"
	 "membar #StoreStore|#LoadStore|#StoreLoad|#LoadLoad\n\t"
	 "casx [%3], %2, %1\n\t"
	 "stx %1, %0"
	 /* h means 64-BIT REGISTER
	  * (probably unneecessary, but why take chances?) */
	 : "=m" (newv), "=&h" (tmp1), "=&h"(tmp2)
	 : "r" (operand), "m"(oldval), "0"(newv)
	 : "cc", "memory");
    return newv;
# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_SPARCV9_64)
    register uint64_t newv = newval;
#  if defined(__SUNPRO_CC)
    asm volatile
#  else
    __asm__ __volatile__
#  endif
	("membar #StoreStore|#LoadStore|#StoreLoad|#LoadLoad\n\t"
	 "casx [%1], %2, %0"
	 : "=&r" (newv)
	 : "r" (operand), "r"(oldval), "0"(newv)
	 : "cc", "memory");
    return newv;
# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA64)
    register uint32_t retval;
    __asm__ __volatile__ ("mov ar.ccv=%0;;": :"rO" (oldval));
    __asm__ __volatile__ ("cmpxchg8.acq %0=[%1],%2,ar.ccv"
	    :"=r"(retval)
	    :"r"(operand), "r"(newval)
	    :"memory");
    return retval;
# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_IA32)
    union {
	uint64_t i;
	struct {
	    /* note: the ordering of these is both important and
	     * counter-intuitive; welcome to little-endian! */
	    uint32_t l;
	    uint32_t h;
	} s;
    } oldv, newv, ret;
    oldv.i = oldval;
    newv.i = newval;
    /* the PIC stuff is already defined above */
    __asm__ __volatile__ (
	    QTHREAD_PIC_PREFIX
	    "lock; cmpxchg8b (%2)"
	    QTHREAD_PIC_SUFFIX
	    :/*EAX*/"=a"(ret.s.l),
	    /*EDX*/"=d"(ret.s.h)
	    :"r"(operand),
	    /*EAX*/"a"(oldv.s.l),
	    /*EBX*/QTHREAD_PIC_REG_4(newv.s.l),
	    /*EDX*/"d"(oldv.s.h),
	    /*ECX*/"c"(newv.s.h)
	    :"memory");
    return ret.i;
# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_AMD64)
    register uint64_t retval;
    /* note that this is GNU/Linux syntax (aka AT&T syntax), not Intel syntax.
     * Thus, this instruction has the form:
     * [lock] cmpxchg reg, reg/mem
     *                src, dest
     */
    __asm__ __volatile__ ("lock; cmpxchg %1,(%2)"
	    : "=&a"(retval) /* store from EAX */
	    : "r"(newval), "r" (operand),
	      "0"(oldval) /* load into EAX */
	    :"cc","memory");
    return retval;
# elif (QTHREAD_ASSEMBLY_ARCH == QTHREAD_POWERPC32)
    /* In general, RISC doesn't provide a way to do 64 bit operations from 32
     * bit code. Sorry! */
    uint64_t retval;

    qthread_lock((aligned_t*)operand);
    retval = *operand;
    if (retval == oldval) {
	*operand = newval;
    }
    qthread_unlock((aligned_t*)operand);
    return retval;
# else
#  error "Don't have a qthread_cas64 implementation for this architecture"
# endif
#else
# error "CAS needs inline assembly OR __sync_val_compare_and_swap"
#endif
}				       /*}}} */

static QINLINE aligned_t qthread_cas_xx(volatile aligned_t * addr,
					const aligned_t oldval,
					const aligned_t newval, const size_t length)
{
    switch (length) {
	case 4:
	    return qthread_cas32((volatile uint32_t *)addr, oldval, newval);
	case 8:
	    return qthread_cas64((volatile uint64_t *)addr, oldval, newval);
	default:
	    /* This should never happen, so deliberately cause a seg fault
	     * for corefile analysis */
	    *(int *)(0) = 0;
    }
    return 0;			       /* compiler check */
}

static QINLINE void *qthread_cas_ptr_(void *volatile*const addr,
				     void *const oldval, void *const newval)
{
    switch (sizeof(void *)) {
	case 4:
	    return (void *)(uintptr_t) qthread_cas32((volatile uint32_t *)
						     addr,
						     (uint32_t)(uintptr_t)
						     oldval,
						     (uint32_t)(uintptr_t)
						     newval);
	case 8:
	    return (void *)(uintptr_t) qthread_cas64((volatile uint64_t *)
						     addr,
						     (uint64_t)(uintptr_t)
						     oldval,
						     (uint64_t)(uintptr_t)
						     newval);
	default:
	    /* This should never happen, so deliberately cause a seg fault for
	     * corefile analysis */
	    *(int *)(0) = 0;
    }
    return NULL;		       /* compiler check */
}

#ifdef QTHREAD_ATOMIC_CAS
# define qthread_cas(ADDR, OLDV, NEWV) \
    __sync_val_compare_and_swap((ADDR), (OLDV), (NEWV))
# define qthread_cas_ptr(ADDR, OLDV, NEWV) \
    __sync_val_compare_and_swap((ADDR), (OLDV), (NEWV))
#else
# define qthread_cas(ADDR, OLDV, NEWV) \
    qthread_cas_xx((volatile aligned_t*)(ADDR), (aligned_t)(OLDV), (aligned_t)(NEWV), sizeof(*(ADDR)))
# ifdef QTHREAD_ATOMIC_CAS_PTR
#  define qthread_cas_ptr(ADDR, OLDV, NEWV) \
    __sync_val_compare_and_swap((ADDR), (OLDV), (NEWV))
# else
#  define qthread_cas_ptr(ADDR, OLDV, NEWV) \
    qthread_cas_ptr_((void*volatile*const)(ADDR), (void*const)(OLDV), (void*const)(NEWV))
# endif
#endif

Q_ENDCXX /* */

#ifndef __cplusplus

# ifdef QTHREAD_ATOMIC_INCR
#  define qthread_incr( ADDR, INCVAL ) \
    __sync_fetch_and_add(ADDR, INCVAL)
# else
#  define qthread_incr( ADDR, INCVAL )                  \
   qthread_incr_xx( (volatile void*)(ADDR), (int)(INCVAL), sizeof(*(ADDR)) )
# endif

#else /* ifdef __cplusplus */
# include "qthread.hpp"
#endif /* __cplusplus */

#endif /* _QTHREAD_H_ */
