#ifndef MYTH_WORKER_PROTO_H_
#define MYTH_WORKER_PROTO_H_

#include "myth_desc.h"

static void myth_sched_loop(void);

static inline void myth_env_init(void);
static inline void myth_env_fini(void);
static inline void myth_set_current_env(myth_running_env_t e);
static inline myth_running_env_t myth_get_current_env(void);
static inline myth_running_env_t myth_env_get_first_busy(myth_running_env_t e);

static inline void myth_worker_start_ex_body(int rank);
static inline void myth_startpoint_init_ex_body(int rank);
static inline void myth_startpoint_exit_ex_body(int rank);
static void *myth_worker_thread_fn(void *args);

#endif /* MYTH_WORKER_PROTO_H_ */
