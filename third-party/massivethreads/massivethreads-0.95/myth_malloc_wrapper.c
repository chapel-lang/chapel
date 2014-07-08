#include <dlfcn.h>
#include <stdlib.h>
#include "myth_config.h"

#include "myth_sched.h"

#include "myth_sched_func.h"

#ifdef MYTH_WRAP_MALLOC

int g_alloc_hook_ok=0;

myth_freelist_t **g_myth_malloc_wrapper_fl;

typedef union malloc_wrapper_header{
	struct{
		uint64_t fl_index;
		void *org_ptr;
	}s;
	uint8_t c[16];
}malloc_wrapper_header,*malloc_wrapper_header_t;

void myth_malloc_wrapper_init(int nthreads)
{
#ifdef MYTH_WRAP_MALLOC_RUNTIME
          /* check if the user wants to wrap malloc.
	     if not return without doing anything */
          if (!g_wrap_malloc) return;
#endif
	  assert(real_malloc);
	  g_myth_malloc_wrapper_fl=real_malloc(sizeof(myth_freelist_t*)*nthreads);
}

void myth_malloc_wrapper_init_worker(int rank)
{
#ifdef MYTH_WRAP_MALLOC_RUNTIME
       /* is it possible to come here before myth_malloc_wrapper_init is called? */
       if (!g_wrap_malloc) return;
#endif
       int i;
       //allocate freelist
       assert(real_malloc);
       g_myth_malloc_wrapper_fl[rank]=real_malloc(sizeof(myth_freelist_t)*FREE_LIST_NUM);
       //initialize
       for (i=0;i<FREE_LIST_NUM;i++){myth_freelist_init(g_myth_malloc_wrapper_fl[rank][i]);}
       __sync_fetch_and_add(&g_alloc_hook_ok,1);
}

void myth_malloc_wrapper_fini()
{
#ifdef MYTH_WRAP_MALLOC_RUNTIME
       /* is it possible to come here before myth_malloc_wrapper_init is called? */
       if (!g_wrap_malloc) return;
#endif
       real_free(g_myth_malloc_wrapper_fl);
}

void myth_malloc_wrapper_fini_worker(int rank)
{
#ifdef MYTH_WRAP_MALLOC_RUNTIME
       /* is it possible to come here before myth_malloc_wrapper_init is called? */
       if (!g_wrap_malloc) return;
#endif
       //Release freelist contents
       /*for (i=0;i<FREE_LIST_NUM;i++){
	 }*/
       //Release the array
       real_free(g_myth_malloc_wrapper_fl[rank]);
       __sync_fetch_and_sub(&g_alloc_hook_ok,1);
}

#ifdef MYTH_WRAP_MALLOC_RUNTIME

/* the fall-back region used by malloc/calloc/realloc/valloc/posix_memalign
   when called before wrappers are in place. in particular they are used
   when called by dlsym.
   if we run out of memory for them, we are simply out of luck.
   below we allocate 16KB, but it may be too much.
   (in one experiment, it only allocated 32 bytes before wrapping
   is complete).
   TODO: we should make it dynamically adjustable.
 */
#define SYS_ALLOC_REGION_SIZE (1 << 14)
char g_sys_alloc_region[SYS_ALLOC_REGION_SIZE];
char * volatile g_sys_alloc_region_ptr = g_sys_alloc_region;
char * g_sys_alloc_region_end = g_sys_alloc_region + SYS_ALLOC_REGION_SIZE;

/* return true if ptr is in the g_sys_alloc_region array. */
int sys_alloc_region(void * ptr) {
  if ((char *)ptr < g_sys_alloc_region) return 0;
  if ((char *)ptr >= g_sys_alloc_region + SYS_ALLOC_REGION_SIZE) return 0;
  return 1;
}

/* a generic, fall-back allocator used until malloc wrapping is incomplete.
   it is the simplest pointer bumping allocator, which cannot free.
   NOTE: you should not use any function that may in turn call any memory
   allocator (malloc etc.). doing so may result in infinite recursions
   (and stack overflow). */
void * sys_alloc_align(size_t alignment, size_t size) {
  while (1) {
    char * p = g_sys_alloc_region_ptr;
    char * q = p + alignment - 1;
    q = q - (long)q % alignment;
    char * r = q + size;
    if (r > g_sys_alloc_region_end) {
      /* Ah, out of luck! 
	 should increase SYS_ALLOC_REGION_SIZE */
      return NULL;
    }
    char * s = __sync_val_compare_and_swap(&g_sys_alloc_region_ptr, p, r);
    if (p == s) {
      return q;
    }
  }
}

#endif

void *calloc(size_t nmemb,size_t size)
{
#ifdef MYTH_WRAP_MALLOC_RUNTIME
  /* fall back to the bump allocator before wrapping completed */
  if (!g_wrap_malloc_completed) {
    void *ptr = sys_alloc_align(16, nmemb * size);
    memset(ptr, 0, nmemb * size);
    return ptr;
  }
  /* no wrap. call the real one */
  if (!g_wrap_malloc) {
    return real_calloc(nmemb, size);
  }
#endif
	void *ptr;
	ptr=malloc(nmemb*size);
	if (!ptr)return NULL;
	memset(ptr,0,nmemb*size);
	return ptr;
}
void *malloc(size_t size)
{
#ifdef MYTH_WRAP_MALLOC_RUNTIME
  /* fall back to the bump allocator before wrapping completed */
  if (!g_wrap_malloc_completed) {
    void *ptr = sys_alloc_align(16, size);
    return ptr;
  }
  /* no wrap. call the real one */
  if (!g_wrap_malloc) {
    return real_malloc(size);
  }
#endif

	//fprintf(stderr,"malloc %d\n",size);
	malloc_wrapper_header_t ptr;
	size_t realsize;
	int idx;
	if (size<16)size=16;
	if (!real_malloc){
		static int load_malloc_protect=0;
		if (load_malloc_protect==0){
			load_malloc_protect=1;
			real_malloc=dlsym(RTLD_NEXT,"malloc");
		}
		else return NULL;
		assert(real_malloc);
	}
	if ((!g_worker_thread_num) || (g_alloc_hook_ok!=g_worker_thread_num) || (size>MYTH_MALLOC_FLSIZE_MAX)){
		ptr=real_malloc(size+sizeof(malloc_wrapper_header));
		if (!ptr){
			fprintf(stderr,"size=%llu\n",(unsigned long long)size);
		}
		assert(ptr);
		ptr->s.fl_index=FREE_LIST_NUM;
		ptr->s.org_ptr=ptr;
		//fprintf(stderr,"malloc A,%p,%d\n",ptr,FREE_LIST_NUM);
		return (void*)(ptr+1);
	}
	idx=MYTH_MALLOC_SIZE_TO_INDEX(size);
	realsize=MYTH_MALLOC_INDEX_TO_RSIZE(idx);
	void **fl_ptr;
	myth_running_env_t env;
	env=myth_get_current_env();
	int rank=env->rank;
	myth_freelist_pop(g_myth_malloc_wrapper_fl[rank][idx],fl_ptr);
	if (!fl_ptr){
		//Freelist is empty, allocate
		ptr=real_malloc(realsize+sizeof(malloc_wrapper_header));
		//fprintf(stderr,"malloc B,%p,%d\n",ptr,idx);
		assert(ptr);
	}
	else{
	  ptr=(malloc_wrapper_header_t)fl_ptr;
	}
	ptr->s.fl_index=idx;
	ptr->s.org_ptr=ptr;
	return (void*)(ptr+1);
}
int posix_memalign(void **memptr,size_t alignment,size_t size)
{
#ifdef MYTH_WRAP_MALLOC_RUNTIME
  /* fall back to the bump allocator before wrapping completed */
  if (!g_wrap_malloc_completed) {
    void *ptr = sys_alloc_align(alignment, size);
    if (ptr) {
      *memptr = ptr;
      return 0;
    } else {
      return ENOMEM;
    }
  }
  /* no wrap. call the real one */
  if (!g_wrap_malloc) {
    return real_posix_memalign(memptr, alignment, size);
  }
#endif
	if (size==0){*memptr=NULL;return 0;}
	malloc_wrapper_header_t ptr;
	if (size<16)size=16;
	if (!real_malloc){
		static int load_malloc_protect=0;
		if (load_malloc_protect==0){
			load_malloc_protect=1;
			real_malloc=dlsym(RTLD_NEXT,"malloc");
		}
		else {*memptr=NULL;return 0;}
		assert(real_malloc);
	}
	uintptr_t n0,n;
	n0=(uintptr_t)real_malloc(size+alignment+sizeof(malloc_wrapper_header));
	if (!n0){
		fprintf(stderr,"size=%llu\n",(unsigned long long)size);
		return ENOMEM;
	}
	//align
	n = n0 + sizeof(malloc_wrapper_header) + alignment - 1;
	n = n - n % alignment;
	ptr=(malloc_wrapper_header_t)n;ptr--;
	assert(n0 <= (uintptr_t)ptr);
	assert(n + size <= n0 + size+alignment+sizeof(malloc_wrapper_header));
	ptr->s.fl_index=FREE_LIST_NUM;
	ptr->s.org_ptr=(void*)n0;
	//fprintf(stderr,"memalign A,%p,%p,%p,%d\n",(void*)n0,ptr,(void*)n,FREE_LIST_NUM);
	*memptr=(void*)n;
	return 0;
}

void *aligned_alloc(size_t alignment, size_t size)
{
	void *ret;
	errno=posix_memalign(&ret,alignment,size);
	return ret;
}
void *valloc(size_t size)
{
#ifdef MYTH_WRAP_MALLOC_RUNTIME
  /* fall back to the bump allocator before wrapping completed */
  if (!g_wrap_malloc_completed) {
    void *ptr = sys_alloc_align(PAGE_SIZE, size);
    return ptr;
  }
  /* no wrap. call the real one */
  if (!g_wrap_malloc) {
    return real_valloc(size);
  }
#endif
	void *ret;
	errno=posix_memalign(&ret,PAGE_SIZE,size);
	return ret;
}

void free(void *ptr)
{
#ifdef MYTH_WRAP_MALLOC_RUNTIME
  /* before wrapping completed, we simply forget about it.
     (real_free not available yet, so we cannot call it.
     the problem may be deeper. the ptr may have been allocated
     by yet another function (not the original system malloc),
     so even passing it to real_free may not be the right action */
  if (!g_wrap_malloc_completed) {
    /* leak */
    return;
  }
  if (!g_wrap_malloc) {
    /* we call real_free, except for region we have allocated
       before wrapping is complete */
    if (!sys_alloc_region(ptr)) {
      return real_free(ptr);
    }
  }
#endif
	if (!ptr)return;
	if (!real_free){
		real_free=dlsym(RTLD_NEXT,"free");
		assert(real_free);
	}
#ifdef MYTH_WRAP_MALLOC_DLSYM_ENABLED
	//do nothing if in dlsym region
	intptr_t s,e;
	s=(intptr_t)s_malloc_dlsym_region;
	e=s+MYTH_WRAP_MALLOC_DLSYM_SIZE;
	if (s<=((intptr_t)ptr) && ((intptr_t)ptr)<e)return;
#endif
	malloc_wrapper_header_t rptr=(malloc_wrapper_header_t)ptr;
	rptr--;
	uint64_t idx=rptr->s.fl_index;
	if (idx>=FREE_LIST_NUM){
		//fprintf(stderr,"free A,%p,%d\n",rptr->s.org_ptr,(int)idx);
		real_free(rptr->s.org_ptr);
		return;
	}
	if (g_worker_thread_num && (g_alloc_hook_ok==g_worker_thread_num)){
		myth_running_env_t env;
		env=myth_get_current_env();
		int rank=env->rank;
		myth_freelist_push(g_myth_malloc_wrapper_fl[rank][idx],(void**)rptr);
		return ;
	}
	//fprintf(stderr,"free B,%p,%d\n",rptr->s.org_ptr,(int)idx);
	real_free(rptr->s.org_ptr);
}
void *realloc(void *ptr,size_t size)
{
#ifdef MYTH_WRAP_MALLOC_RUNTIME
  /* fall back to the bump allocator before wrapping completed */
  if (!g_wrap_malloc_completed) {
    /* leak old ptr */
    void *new_ptr = sys_alloc_align(16, size);
    memcpy(new_ptr, ptr, size);
    return new_ptr;
  }
  /* no wrap. call the real one */
  if (!g_wrap_malloc) {
    return real_realloc(ptr, size);
  }
#endif
	if (size==0){free(ptr);return NULL;}
	if (!ptr)return malloc(size);
	uint64_t *rptr=(uint64_t*)ptr;rptr-=16/8;
	MAY_BE_UNUSED size_t nrsize;
	size_t orsize;
	int oidx,nidx;
	oidx=*rptr;
	if (size<16)size=16;
	nidx=MYTH_MALLOC_SIZE_TO_INDEX(size);
	if (oidx==nidx)return ptr;
	nrsize=MYTH_MALLOC_INDEX_TO_RSIZE(nidx);
	orsize=MYTH_MALLOC_INDEX_TO_RSIZE(*rptr);
	void *nptr=malloc(size);
	if (!nptr)return NULL;
	size_t btc=(size<orsize)?size:orsize;
	memcpy(nptr,ptr,btc);
	free(ptr);
	return nptr;
}
#else

void myth_malloc_wrapper_init(int nthreads)
{
}

void myth_malloc_wrapper_init_worker(int rank)
{
}

void myth_malloc_wrapper_fini()
{
}

void myth_malloc_wrapper_fini_worker(int rank)
{
}

#endif
