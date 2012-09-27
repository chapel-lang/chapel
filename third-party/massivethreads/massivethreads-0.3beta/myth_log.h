#ifndef MYTH_LOG_H_
#define MYTH_LOG_H_

#include <stdint.h>
#include "myth_config.h"

//Special pointer describing status
#define THREAD_PTR_SCHED NULL //scheduler running
#define TRREAD_PTR_SCHED_SLEEP ((myth_thread_t)0xFFFFFFFF) //scheduler sleeping
#define THREAD_PTR_SCHED_TERM ((myth_thread_t)0xFFFFFFFE) //scheduler entering termination

typedef enum
{
	MYTH_LOG_THREAD_ANNOTATION=0,//thread annotation
	MYTH_LOG_SWITCH,//context switch
}myth_log_type_t;

//ログをとるための構造体
typedef struct myth_log_entry
{
	uint64_t tsc;			//Time stamp counter from rdtsc
	myth_log_type_t type;
	int rank;
	union{
		struct{
			struct myth_thread *th;
			int recycle_count;
		}ctx_switch;
		struct{
			struct myth_thread *th;
			int recycle_count;
			char str[MYTH_THREAD_ANNOTATION_MAXLEN];
		}annotation;
	}u;
}myth_log_entry,*myth_log_entry_t;

extern uint64_t g_tsc_base;

#endif /* MYTH_LOG_H_ */
