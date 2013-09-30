#ifndef MYTH_SCHED_PROTO_H_
#define MYTH_SCHED_PROTO_H_

#include "myth_worker.h"
#include "myth_desc.h"

static inline void myth_env_init(void);
static inline void myth_env_fini(void);
static inline void myth_set_current_env(myth_running_env_t e);
static inline myth_running_env_t myth_get_current_env(void);
static inline void init_myth_thread_struct(myth_running_env_t env,myth_thread_t th);
static inline myth_running_env_t myth_env_get_first_busy(myth_running_env_t e);
static inline myth_thread_t myth_self_body(void);
MYTH_CTX_CALLBACK void myth_create_1(void *arg1,void *arg2,void *arg3);
static inline myth_thread_t myth_create_body(myth_func_t func,void *arg,size_t stack_size);
MYTH_CTX_CALLBACK void myth_yield_1(void *arg1,void *arg2,void *arg3);
static inline void myth_yield_body(int force_worksteal);
static inline void myth_yield2_body(void);
static inline void myth_join_1(myth_running_env_t e,myth_thread_t th,void **result);
MYTH_CTX_CALLBACK void myth_join_2(void *arg1,void *arg2,void *arg3);
MYTH_CTX_CALLBACK void myth_join_3(void *arg1,void *arg2,void *arg3);
static inline void myth_join_body(myth_thread_t th,void **result);
static inline void myth_detach_body(myth_thread_t th);
static void myth_sched_loop(void);
MYTH_CTX_CALLBACK void myth_entry_point_1(void *arg1,void *arg2,void *arg3);
MYTH_CTX_CALLBACK void myth_entry_point_2(void *arg1,void *arg2,void *arg3);
static void __attribute__((unused)) myth_entry_point(void);
static inline void myth_entry_point_cleanup(myth_thread_t this_thread);
static inline void myth_log_start_body(void);
static inline void myth_log_pause_body(void);
static inline void myth_sched_prof_start_body(void);
static inline void myth_sched_prof_pause_body(void);

static inline myth_thread_t get_new_myth_thread_struct_desc(myth_running_env_t env);
static inline void *get_new_myth_thread_struct_stack(myth_running_env_t env,size_t size_in_bytes);
static inline void init_myth_thread_struct(myth_running_env_t env,myth_thread_t th);
static inline void free_myth_thread_struct_desc(myth_running_env_t e,myth_thread_t th);
static inline void free_myth_thread_struct_stack(myth_running_env_t e,myth_thread_t th);

#endif /* MYTH_PROTO_H_ */
