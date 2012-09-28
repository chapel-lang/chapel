#include "myth_log.h"

/*
int myth_textlog_entry_compare(const void *pa,const void *pb)
{
	myth_textlog_entry_t a,b;
	a=(myth_textlog_entry_t)pa;b=(myth_textlog_entry_t)pb;
	if (a->te > b->te)return 1;
	if (a->te < b->te)return -1;
	if (a->ts > b->ts)return 1;
	if (a->ts < b->ts)return -1;
	return 0;
}
*/

int myth_log_entry_compare(const void *pa,const void *pb)
{
	myth_log_entry_t a,b;
	a=(myth_log_entry_t)pa;b=(myth_log_entry_t)pb;
	//1st:type
	if (a->type>b->type)return 1;if (a->type<b->type)return -1;
	switch (a->type){
	case MYTH_LOG_THREAD_ANNOTATION:
		//2nd:timestamp
		if (a->tsc > b->tsc)return 1;if (a->tsc < b->tsc)return -1;
		break;
	case MYTH_LOG_SWITCH:
		//2nd:worker thread index
		if (a->rank > b->rank)return 1;if (a->rank < b->rank)return -1;
		//3rd:timestamp
		if (a->tsc > b->tsc)return 1;if (a->tsc < b->tsc)return -1;
		break;
	}
	return 0;
}

uint64_t g_tsc_base=0;
