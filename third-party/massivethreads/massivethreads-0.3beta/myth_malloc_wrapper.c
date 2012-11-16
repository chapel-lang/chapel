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
	assert(real_malloc);
	g_myth_malloc_wrapper_fl=real_malloc(sizeof(myth_freelist_t*)*nthreads);
}

void myth_malloc_wrapper_init_worker(int rank)
{
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
	real_free(g_myth_malloc_wrapper_fl);
}

void myth_malloc_wrapper_fini_worker(int rank)
{
	//Release freelist contents
	/*for (i=0;i<FREE_LIST_NUM;i++){
	}*/
	//Release the array
	real_free(g_myth_malloc_wrapper_fl[rank]);
	__sync_fetch_and_sub(&g_alloc_hook_ok,1);
}

void *calloc(size_t nmemb,size_t size)
{
	void *ptr;
	ptr=malloc(nmemb*size);
	if (!ptr)return NULL;
	memset(ptr,0,nmemb*size);
	return ptr;
}
void *malloc(size_t size)
{
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
	n=n0+alignment-1;
	n/=alignment;n*=alignment;
	ptr=(malloc_wrapper_header_t)n;ptr--;
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
	void *ret;
	errno=posix_memalign(&ret,PAGE_SIZE,size);
	return ret;
}

void free(void *ptr)
{
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
