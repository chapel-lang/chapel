#ifndef MYTH_INTERNAL_LOCK_H_
#define MYTH_INTERNAL_LOCK_H_

#include "myth_config.h"
#include "myth_mem_barrier.h"
#include "myth_original_lib.h"

//Internal locks
#ifdef MYTH_INTERNAL_LOCK_MUTEX
//pthread_mutex
typedef pthread_mutex_t myth_internal_lock_t;
static inline void myth_internal_lock_init(myth_internal_lock_t *ptr){
	assert(real_pthread_mutex_init(ptr,NULL)==0);
}
static inline void myth_internal_lock_destroy(myth_internal_lock_t *ptr){
	assert(real_pthread_mutex_destroy(ptr)==0);
}
static inline void myth_internal_lock_lock(myth_internal_lock_t *ptr){
	assert(real_pthread_mutex_lock(ptr)==0);
}
static inline void myth_internal_lock_unlock(myth_internal_lock_t *ptr){
	assert(real_pthread_mutex_unlock(ptr)==0);
}
static inline int myth_internal_lock_trylock(myth_internal_lock_t *lock)
{
	return real_pthread_mutex_trylock(lock)==0;
}
#elif defined MYTH_INTERNAL_LOCK_SPINLOCK1
//pthread_spin
typedef pthread_spinlock_t myth_internal_lock_t;
static inline void myth_internal_lock_init(myth_internal_lock_t *ptr){
	assert(real_pthread_spin_init(ptr,PTHREAD_PROCESS_PRIVATE)==0);
}
static inline void myth_internal_lock_destroy(myth_internal_lock_t *ptr){
	assert(real_pthread_spin_destroy(ptr)==0);
}
static inline void myth_internal_lock_lock(myth_internal_lock_t *ptr){
	assert(real_pthread_spin_lock(ptr)==0);
}
static inline void myth_internal_lock_unlock(myth_internal_lock_t *ptr){
	assert(real_pthread_spin_unlock(ptr)==0);
}
static inline int myth_internal_lock_trylock(myth_internal_lock_t *lock)
{
	return real_pthread_spin_trylock(lock)==0;
}
#elif defined MYTH_INTERNAL_LOCK_SPINLOCK2
//Architecture-dependent Inlined spinlock
//MEMO:Architecture-Dependent Code
#if (defined MYTH_ARCH_i386) || (defined MYTH_ARCH_amd64)
typedef volatile int myth_internal_lock_t;
static inline void myth_internal_lock_init(myth_internal_lock_t *ptr)
{
	*ptr=0;
}
static inline void myth_internal_lock_destroy(myth_internal_lock_t *ptr){*ptr=0;}
static inline int myth_internal_lock_trylock(myth_internal_lock_t *ptr)
{
	int ret;
	asm volatile(
			"xor %%eax,%%eax\n"
			"mov $1,%%edx\n"
			"lock cmpxchg %%edx,(%1)\n"//if (*ptr==0)*ptr=1
			:"=&a"(ret):"r"(ptr):"cc","%rdx","memory");
	return ret==0;
}
static inline void myth_internal_lock_lock(myth_internal_lock_t *ptr)
{
	asm volatile(
			"1:cmp $0,(%0)\n"
			"je 2f\n"
			"rep;nop\n"
			"jmp 1b\n"
			"2:xor %%eax,%%eax\n"//eax=0
			"lock cmpxchg %1,(%0)\n"//if (*ptr==0)*ptr=1
			"jne 1b\n"
			::"r"(ptr),"r"(1):"%eax","cc","memory");
}
static inline void myth_internal_lock_unlock(myth_internal_lock_t *ptr)
{
	//Serialize write instructions
	myth_wbarrier();
	//Reset value
	*ptr=0;
}

#elif (defined MYTH_ARCH_sparc)
#warning "Inlined spinlock is not provided in this architecture, substituted by pthread_spin"
typedef pthread_spinlock_t myth_internal_lock_t;
static inline void myth_internal_lock_init(myth_internal_lock_t *ptr){
	assert(real_pthread_spin_init(ptr,PTHREAD_PROCESS_PRIVATE)==0);
}
static inline void myth_internal_lock_destroy(myth_internal_lock_t *ptr){
	assert(real_pthread_spin_destroy(ptr)==0);
}
static inline void myth_internal_lock_lock(myth_internal_lock_t *ptr){
	assert(real_pthread_spin_lock(ptr)==0);
}
static inline void myth_internal_lock_unlock(myth_internal_lock_t *ptr){
	assert(real_pthread_spin_unlock(ptr)==0);
}
static inline int myth_internal_lock_trylock(myth_internal_lock_t *lock)
{
	return real_pthread_spin_trylock(lock)==0;
}

#else
#warning "Inlined spinlock is not provided in this architecture, substituted by pthread_spin"

typedef pthread_spinlock_t myth_internal_lock_t;
static inline void myth_internal_lock_init(myth_internal_lock_t *ptr){
	assert(real_pthread_spin_init(ptr,PTHREAD_PROCESS_PRIVATE)==0);
}
static inline void myth_internal_lock_destroy(myth_internal_lock_t *ptr){
	assert(real_pthread_spin_destroy(ptr)==0);
}
static inline void myth_internal_lock_lock(myth_internal_lock_t *ptr){
	assert(real_pthread_spin_lock(ptr)==0);
}
static inline void myth_internal_lock_unlock(myth_internal_lock_t *ptr){
	assert(real_pthread_spin_unlock(ptr)==0);
}
static inline int myth_internal_lock_trylock(myth_internal_lock_t *lock)
{
	return real_pthread_spin_trylock(lock)==0;
}

#endif
#else
#error "Please choose internal locking method"
#endif

#endif /* MYTH_LOCK_H_ */
