#include "myth_misc.h"

#ifdef MYTH_FLMALLOC_TLS
__thread myth_freelist_t *g_myth_freelist;
#else
myth_freelist_t **g_myth_freelist;
#endif

//Global variabled declaration

//uint64_t g_mmap_total=0,g_mmap_count=0;

__thread unsigned int g_myth_random_temp=0;

__thread uint64_t g_myth_flmalloc_cycles=0,g_myth_flmalloc_cnt=0;
__thread uint64_t g_myth_flfree_cycles=0,g_myth_flfree_cnt=0;

static cpu_set_t worker_cpusets[CPU_SETSIZE];
static int available_cores=-1;

void myth_init_process_affinity_info(void)
{
	cpu_set_t cset;
	sched_getaffinity(getpid(),sizeof(cpu_set_t),&cset);
	int i;
	for (i=0;i<CPU_SETSIZE;i++){
		CPU_ZERO(&worker_cpusets[i]);
	}
	available_cores=0;
	for (i=0;i<CPU_SETSIZE;i++){
		if (CPU_ISSET(i,&cset)){
			CPU_SET(i,&worker_cpusets[available_cores]);
			available_cores++;
		}
	}
}

//Return the number of CPU cores
int myth_get_cpu_num(void)
{
	assert(available_cores>0);
	return available_cores;
}

//Return cpu_set
cpu_set_t myth_get_worker_cpuset(int rank)
{
	assert(available_cores>0);
	return worker_cpusets[rank%available_cores];
}
