/* 
 * myth_init.h
 */
#pragma once
#ifndef MYTH_INIT_H
#define MYTH_INIT_H

#include <pthread.h>

#include "myth/myth.h"
#include "myth_config.h"

//Environment value of worker threads
#define ENV_MYTH_WORKER_NUM    "MYTH_WORKER_NUM"
#define ENV_MYTH_DEF_STKSIZE   "MYTH_DEF_STKSIZE"
#define ENV_MYTH_DEF_GUARDSIZE "MYTH_DEF_GUARDSIZE"
#define ENV_MYTH_BIND_WORKERS  "MYTH_BIND_WORKERS"
#define ENV_MYTH_CHILD_FIRST   "MYTH_CHILD_FIRST"

enum {
  myth_init_state_uninit,
  myth_init_state_initializing,
  myth_init_state_initialized
};
extern myth_globalattr_t g_attr;
extern volatile int g_myth_init_state;

/* defined in myth_init_func.h */
static inline int myth_ensure_init_ex(myth_globalattr_t * attr);
static inline int myth_ensure_init(void);

/* defined in myth_init.c */
int myth_init_ex_body(const myth_globalattr_t * attr);
int myth_fini_body(void);

#endif
