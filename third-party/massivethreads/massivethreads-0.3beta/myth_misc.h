#ifndef MYTH_MISC_H_
#define MYTH_MISC_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <unistd.h>
#include <time.h>
#include <sys/mman.h>
#include "myth_config.h"
#include "myth_original_lib.h"

//Variable attribute which may be unused to supress warnings
#define MAY_BE_UNUSED __attribute__((unused))

//Sanity check which can be removed for performance
//Do not write expressions with side-effect! It may not be executed!
#ifdef MYTH_SANITY_CHECK
static inline void myth_assert(expr){assert(expr);}
#else
#define myth_assert(expr)
#endif

#define GCC_VERSION (__GNUC__ * 10000 \
                               + __GNUC_MINOR__ * 100 \
                               + __GNUC_PATCHLEVEL__)


//Unreachable marker that causes segmentation fault
//useful for debugging context-switching codes
#if defined USE_MYTH_UNREACHABLE
#if (defined MYTH_ARCH_i386 || defined MYTH_ARCH_amd64)
#define myth_unreachable() asm volatile("ud2\n")
#elif GCC_VERSION >= 40500
#define myth_unreachable() __builtin_unreachable()
#else
#define myth_unreachable()
#endif
#else
#define myth_unreachable()
#endif

//Access to time stamp counter
//lfence inserted to serialize instructions
static inline uint64_t myth_get_rdtsc()
{
#if defined MYTH_ARCH_i386 || defined MYTH_ARCH_amd64
  uint32_t hi,lo;
  asm volatile("lfence\nrdtsc" : "=a"(lo),"=d"(hi));
  return ((uint64_t)hi)<<32 | lo;
#elif defined MYTH_ARCH_sparc
  uint64_t tick;
  asm volatile("rd %%tick, %0" : "=r" (tick));
  return tick;
#else
#warning "myth_get_rdtsc() not implemented" 
  return 0;
#endif
}

void myth_init_process_affinity_info(void);
int myth_get_cpu_num(void);
cpu_set_t myth_get_worker_cpuset(int rank);

//Worker-thread-safe RNG for randomized work-stealing
extern __thread unsigned int g_myth_random_temp;
static inline void myth_random_init(unsigned int seed)
{
	g_myth_random_temp=seed;
}
//Return a random integer with a range [min,max)
static inline int myth_random(int min,int max)
{
	int ret;
	if (!g_myth_random_temp){
		myth_random_init(((unsigned)time(NULL)));
	}
	ret=min+(int)(rand_r(&g_myth_random_temp)*((double)max-min)/(1.0+RAND_MAX));
	return ret;
}

//LIFO freelist macros
//Reference to the next entry is stored at the head of the entry,
//so entries must be larger than pointer size

//Freelist type definition: an alias of void**
typedef void** myth_freelist_t;
//Initialize
#define myth_freelist_init(fl) \
{fl=NULL;}
//Push "data" in a freelist as a new entry
#define myth_freelist_push(fl,data) \
{\
	void **tmp_ptr=(void**)data;\
	*tmp_ptr=(void*)fl;\
	fl=tmp_ptr;\
}
//Pop one entry and store in "ret", or NULL if the freelist is empty
#define myth_freelist_pop(fl,ret) \
{\
	if (fl){\
		void **tmp;\
		tmp=fl;\
		ret=(void*)fl;\
		fl=*tmp;\
	}\
	else{ret=NULL;}\
}

#ifdef MYTH_FLMALLOC_PROF
extern __thread uint64_t g_myth_flmalloc_cycles,g_myth_flmalloc_cnt;
extern __thread uint64_t g_myth_flfree_cycles,g_myth_flfree_cnt;
#endif

//Internal simple memory allocator using worker-thread-local freelist
//Each entry is a linked-list of 1,2,4,8,...2^(FREE_LIST_NUM-1) bytes
//All alloc/free is done with this granularity
#define FREE_LIST_NUM 31

#if SIZEOF_INT==4
//size_t=uin32_t
#define MYTH_MALLOC_SIZE_TO_INDEX(s) (32-__builtin_clz((s)-1))
#elif SIZEOF_INT==8
//size_t=uint64_t
#define MYTH_MALLOC_SIZE_TO_INDEX(s) (64-__builtin_clz((s)-1))
#else
#error
#endif
#define MYTH_MALLOC_INDEX_TO_RSIZE(s) (1<<(s))
#define MYTH_MALLOC_SIZE_TO_RSIZE(s) (MYTH_MALLOC_INDEX_TO_RSIZE(MYTH_MALLOC_SIZE_TO_INDEX(s)))

#define MYTH_MALLOC_FLSIZE_MAX (MYTH_MALLOC_INDEX_TO_RSIZE(FREE_LIST_NUM-1)-1)

#ifdef USE_MYTH_FLMALLOC

#ifdef MYTH_FLMALLOC_TLS
extern __thread myth_freelist_t *g_myth_freelist;
#else
extern myth_freelist_t **g_myth_freelist;
#endif

static inline void myth_flmalloc_init(int nthreads)
{
#ifndef MYTH_FLMALLOC_TLS
	assert(real_malloc);
	g_myth_freelist=real_malloc(sizeof(myth_freelist_t*)*nthreads);
#endif
}

static inline void myth_flmalloc_init_worker(int rank)
{
	int i;
	//Allocate freelists
#ifdef MYTH_FLMALLOC_TLS
	g_myth_freelist=real_malloc(sizeof(myth_freelist_t)*FREE_LIST_NUM);
	//Initialize
	for (i=0;i<FREE_LIST_NUM;i++){myth_freelist_init(g_myth_freelist[i]);
#else
	assert(real_malloc);
	g_myth_freelist[rank]=real_malloc(sizeof(myth_freelist_t)*FREE_LIST_NUM);
	//Initialize
	for (i=0;i<FREE_LIST_NUM;i++){myth_freelist_init(g_myth_freelist[rank][i]);}
#endif
}
	extern uint64_t g_mmap_total,g_mmap_count;
static inline void myth_flmalloc_fini()
{
#ifndef MYTH_FLMALLOC_TLS
	real_free(g_myth_freelist);
#endif
}

static inline void myth_flmalloc_fini_worker(int rank)
{
	//Release freelist contents
	/*for (i=0;i<FREE_LIST_NUM;i++){
		void **ptr=g_myth_freelist[i];

	}*/
	//Release the array
#ifndef MYTH_FLMALLOC_TLS
	real_free(g_myth_freelist[rank]);
#else
	real_free(g_myth_freelist);
#endif
#ifdef MYTH_FLMALLOC_PROF
	fprintf(stderr,"%lu mallocs : %lf cycles/malloc\n",(unsigned long)g_myth_flmalloc_cnt,g_myth_flmalloc_cycles/(double)g_myth_flmalloc_cnt);
	fprintf(stderr,"%lu frees : %lf cycles/free\n",(unsigned long)g_myth_flfree_cnt,g_myth_flfree_cycles/(double)g_myth_flfree_cnt);
#endif
}

extern __thread int g_worker_rank;

static inline void *myth_mmap(void *addr,size_t length,int prot,int flags,int fd,off_t offset);

#if 0
#define myth_flmalloc(a,b) myth_flmalloc_(a,b,__FILE__,__LINE__)
static inline void *myth_flmalloc_(int rank,size_t size,char *f,int l)
#else
static inline void *myth_flmalloc(int rank,size_t size)
#endif
{
	//Freelist-based Internal allocator
	size_t realsize;
	int idx;
	void **ptr;
#ifdef MYTH_FLMALLOC_PROF
	uint64_t t0,t1;
	t0=myth_get_rdtsc();
#endif
	if (size<8)size=8;
	idx=MYTH_MALLOC_SIZE_TO_INDEX(size);
#ifdef MYTH_FLMALLOC_TLS
	myth_freelist_pop(g_myth_freelist[idx],ptr);
#else
	myth_freelist_pop(g_myth_freelist[rank][idx],ptr);
#endif
	if (!ptr){
		//Freelist is empty, allocate
		realsize=MYTH_MALLOC_INDEX_TO_RSIZE(idx);
		if (realsize<PAGE_SIZE){
#if 1
			assert(PAGE_SIZE%realsize==0);
			char *p,*p2;
			ptr=(void**)myth_mmap(NULL,PAGE_SIZE,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
			p=(char*)ptr;
			p2=p+PAGE_SIZE;p+=realsize;
			while (p<p2){
#ifdef MYTH_FLMALLOC_TLS
				myth_freelist_push(g_myth_freelist[idx],(void**)p);
#else
				myth_freelist_push(g_myth_freelist[rank][idx],(void**)p);
#endif
				p+=realsize;
			}
#else
			ptr=(void**)malloc(realsize);
			if (!ptr){
				perror("malloc");
				assert(0);
			}
#endif
		}
		else{
			//Just allocate by mmap and return
			assert(realsize%4096==0);
			ptr=myth_mmap(NULL,realsize,PROT_READ|PROT_WRITE,MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
		}
		//ptr=real_malloc(realsize);//fprintf(stderr,"M %lu %s:%d\n",(unsigned long)realsize,f,l);
		myth_assert(ptr);
	}
	else{
	}
#ifdef MYTH_FLMALLOC_PROF
	t1=myth_get_rdtsc();
	g_myth_flmalloc_cycles+=t1-t0;g_myth_flmalloc_cnt++;
#endif
	return ptr;
}
static inline void myth_flfree(int rank,size_t size,void *ptr)
{
	//Put into the freelist
	int idx;
#ifdef MYTH_FLMALLOC_PROF
	uint64_t t0,t1;
	t0=myth_get_rdtsc();
#endif
	if (size<8)size=8;
	idx=MYTH_MALLOC_SIZE_TO_INDEX(size);
#ifdef MYTH_FLMALLOC_TLS
	myth_freelist_push(g_myth_freelist[idx],ptr)
#else
	myth_freelist_push(g_myth_freelist[rank][idx],ptr)
#endif
#ifdef MYTH_FLMALLOC_PROF
	t1=myth_get_rdtsc();
	g_myth_flfree_cycles+=t1-t0;g_myth_flfree_cnt++;
#endif
}
static inline void *myth_flrealloc(int rank,size_t oldsize,void *ptr,size_t size)
{
	void *ret;
	size_t cp_size;
	ret=myth_flmalloc(rank,size);
	myth_assert(ret);
	cp_size=(size<oldsize)?size:oldsize;
	memcpy(ret,ptr,cp_size);
	myth_flfree(rank,oldsize,ptr);
	return ret;
}
#else
static inline void myth_flmalloc_init(int nthreads)
{
}

static inline void myth_flmalloc_init_worker(int rank)
{
}

static inline void myth_flmalloc_fini(void)
{
}

static inline void myth_flmalloc_fini_worker(int rank)
{
#ifdef MYTH_FLMALLOC_PROF
	fprintf(stderr,"%lu mallocs : %lf cycles/malloc\n",(unsigned long)g_myth_flmalloc_cnt,g_myth_flmalloc_cycles/(double)g_myth_flmalloc_cnt);
	fprintf(stderr,"%lu frees : %lf cycles/free\n",(unsigned long)g_myth_flfree_cnt,g_myth_flfree_cycles/(double)g_myth_flfree_cnt);
#endif
}
static inline void *myth_malloc(size_t size);
static inline void myth_free(void *ptr,size_t size);
static inline void* myth_flmalloc(int rank,size_t s)
{
	void *ret;
#ifdef MYTH_FLMALLOC_PROF
	uint64_t t0,t1;
	t0=myth_get_rdtsc();
#endif
	ret=myth_malloc(s);
#ifdef MYTH_FLMALLOC_PROF
	t1=myth_get_rdtsc();
	g_myth_flmalloc_cycles+=t1-t0;g_myth_flmalloc_cnt++;
#endif
	return ret;
}
static inline void myth_flfree(int rank,size_t size,void *ptr)
{
#ifdef MYTH_FLMALLOC_PROF
	uint64_t t0,t1;
	t0=myth_get_rdtsc();
#endif
	myth_free(ptr,size);
#ifdef MYTH_FLMALLOC_PROF
	t1=myth_get_rdtsc();
	g_myth_flfree_cycles+=t1-t0;g_myth_flfree_cnt++;
#endif
}
#define myth_flrealloc(rank,os,p,ns) myth_realloc(p,ns)
#endif

//malloc with error checking
static inline void *myth_malloc(size_t size)
{
	void *ptr;
	if (!real_malloc)return NULL;
	ptr=real_malloc(size);
	myth_assert(ptr);
	return ptr;
}
static inline void myth_free(void *ptr,size_t size)
{
	real_free(ptr);
}
static inline void *myth_realloc(void *ptr,size_t size)
{
	void *ret;
	ret=real_realloc(ptr,size);
	myth_assert(ret);
	return ret;
}

//extern uint64_t g_mmap_total,g_mmap_count;

//#include <malloc.h>

static inline void *myth_mmap(void *addr,size_t length,int prot,int flags,int fd,off_t offset)
{
#if 1
	void *ptr=mmap(addr,length,prot,flags,fd,offset);
	//__sync_fetch_and_add(&g_mmap_total,length);
	//__sync_fetch_and_add(&g_mmap_count,1);
	if (ptr==MAP_FAILED){
		//printf("size=%llu\n",(unsigned long long)g_mmap_total);
		//printf("count=%llu\n",(unsigned long long)g_mmap_count);
		perror("mmap");
		assert(0);
	}
#else
	//void *ptr=real_malloc(length);
	void *ptr=memalign(4096,length);
	__sync_fetch_and_add(&g_mmap_total,length);
	__sync_fetch_and_add(&g_mmap_count,1);
	if (ptr==NULL){
		printf("size=%llu\n",(unsigned long long)g_mmap_total);
		printf("count=%llu\n",(unsigned long long)g_mmap_count);
		perror("malloc");
		assert(0);
	}
#endif
	return ptr;
}

static inline int myth_munmap(void *addr,size_t length)
{
#if 1
	return munmap(addr,length);
#else
	free(addr);
	return 0;
#endif
}

#endif /* MYTH_MISC_H_ */
