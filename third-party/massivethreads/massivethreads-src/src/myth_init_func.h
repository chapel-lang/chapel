/* 
 * myth_init_func.h
 */
#pragma once
#ifndef MYTH_INIT_FUNC_H
#define MYTH_INIT_FUNC_H

#include "myth_config.h"
#include "myth_init.h"
#include "myth_misc.h"
#include "myth_misc_func.h"

static inline int myth_ensure_init_ex(myth_globalattr_t * attr) {
  if (g_myth_init_state == myth_init_state_initialized) {
    return 1;
  } else {
    return myth_init_ex_body(attr);
  }
}

static inline int myth_ensure_init(void) {
  return myth_ensure_init_ex(0);
}

static inline int myth_globalattr_get_default(myth_globalattr_t * attr) {
  myth_globalattr_t a;
  {
    /* default stack size */
    size_t sz = 0;
    char * env = getenv(ENV_MYTH_DEF_STKSIZE);
    if (env) {
      sz = atoi(env);
    }
    if (sz <= 0) {
      sz = MYTH_DEF_STACK_SIZE;
    }
    a.stacksize = sz;
  }
  {
    size_t sz = 0;
    char * env = getenv(ENV_MYTH_DEF_GUARDSIZE);
    if (env) {
      sz = atoi(env);
    }
    if (sz <= 0) {
      sz = MYTH_DEF_GUARD_SIZE;
    }
    a.guardsize = sz;
  }
  {
    /* number of workers */
    int nw = 0;
    char * env = getenv(ENV_MYTH_WORKER_NUM);
    if (env) {
      nw = atoi(env);
    }
    if (nw <= 0) {
      nw = myth_get_cpu_num();
    }
    a.n_workers = nw;
  }
  {
    /* bind workers */
    int bw = MYTH_BIND_WORKERS;
    char * env = getenv(ENV_MYTH_BIND_WORKERS);
    if (env){
      bw = atoi(env);
    }
    a.bind_workers = bw;
  }
  {
    /* child first */
    int cf = MYTH_CHILD_FIRST;
    char * env = getenv(ENV_MYTH_CHILD_FIRST);
    if (env){
      cf = atoi(env);
    }
    a.child_first = cf;
  }
  *attr = a;
  return 0;
}

static inline int myth_globalattr_init_body(myth_globalattr_t * attr) {
  return myth_globalattr_get_default(attr);
}

static inline int myth_globalattr_destroy_body(myth_globalattr_t * attr) {
  (void)attr;
  return 0;
}

static inline int myth_globalattr_set_default_body(const myth_globalattr_t * attr) {
  if (attr) {
    g_attr = *attr;
    return 0;
  } else {
    return myth_globalattr_get_default(&g_attr);
  }
}

static inline int
myth_globalattr_get_stacksize_body(myth_globalattr_t * attr,
				   size_t *stacksize) {
  if (!attr) attr = &g_attr;
  *stacksize = attr->stacksize;
  return 0;
}

static inline int
myth_globalattr_set_stacksize_body(myth_globalattr_t * attr,
				   size_t stacksize) {
  if (!attr) attr = &g_attr;
  attr->stacksize = stacksize;
  return 0;
}

static inline int
myth_globalattr_get_guardsize_body(myth_globalattr_t * attr,
				   size_t *guardsize) {
  if (!attr) attr = &g_attr;
  *guardsize = attr->guardsize;
  return 0;
}

static inline int
myth_globalattr_set_guardsize_body(myth_globalattr_t * attr,
				   size_t guardsize) {
  if (!attr) attr = &g_attr;
  attr->guardsize = guardsize;
  return 0;
}

static inline int
myth_globalattr_get_n_workers_body(myth_globalattr_t * attr,
				   size_t *n_workers) {
  if (!attr) attr = &g_attr;
  *n_workers = attr->n_workers;
  return 0;
}

static inline int
myth_globalattr_set_n_workers_body(myth_globalattr_t * attr,
				   size_t n_workers) {
  if (!attr) attr = &g_attr;
  attr->n_workers = n_workers;
  return 0;
}

static inline int
myth_globalattr_get_bind_workers_body(myth_globalattr_t * attr,
				      int *bind_workers) {
  if (!attr) attr = &g_attr;
  *bind_workers = attr->bind_workers;
  return 0;
}

static inline int
myth_globalattr_set_bind_workers_body(myth_globalattr_t * attr,
				      int bind_workers) {
  if (!attr) attr = &g_attr;
  attr->bind_workers = bind_workers;
  return 0;
}

static inline int
myth_globalattr_get_child_first_body(myth_globalattr_t * attr,
				     int *child_first) {
  if (!attr) attr = &g_attr;
  *child_first = attr->child_first;
  return 0;
}

static inline int
myth_globalattr_set_child_first_body(myth_globalattr_t * attr,
				     int child_first) {
  if (!attr) attr = &g_attr;
  attr->child_first = child_first;
  return 0;
}

#endif
