/* 
 * myth_tls_func.h
 */
#pragma once
#ifndef MYTH_TLS_FUNC_H_
#define MYTH_TLS_FUNC_H_

#include <errno.h>

#include "myth/myth.h"

#include "myth_config.h"
#include "myth_init.h"
#include "myth_misc.h"
#include "myth_worker.h"
#include "myth_thread.h"
#include "myth_tls.h"

#include "myth_init_func.h"
#include "myth_misc_func.h"
#include "myth_worker_func.h"

static inline myth_tls_tree_node_t * myth_tls_tree_node_alloc_node() {
  myth_tls_tree_node_t * n = myth_malloc(sizeof(myth_tls_tree_node_t));
  int i;
#if MYTH_TLS_DBG
  n->type = myth_tls_tree_node_type_internal;
#endif
  for (i = 0; i < myth_tls_tree_node_n_children; i++) {
    n->children[i] = 0;
  }
  return n;
}

static inline myth_tls_tree_node_t * myth_tls_tree_node_alloc_leaf() {
  size_t sz = (size_t)(&(((myth_tls_tree_node_t *)0)->entries[myth_tls_tree_node_n_entries_in_leaf]));
  myth_tls_tree_node_t * n = myth_malloc(sz);
  int i;
#if MYTH_TLS_DBG
  n->type = myth_tls_tree_node_type_leaf;
#endif
  for (i = 0; i < myth_tls_tree_node_n_entries_in_leaf; i++) {
    n->entries[i].value = 0;
  }
  return n;
}

static inline void myth_tls_tree_init(myth_tls_tree_t * t) {
  t->root = 0;
}

/* ---------------------------------------
   
 */

static inline int
myth_tls_call_destructors_rec(myth_tls_tree_node_t * n,
			      int depth, myth_key_t base, myth_key_t stride,
			      myth_tls_key_allocator_t * ka) {
  assert(n);
  if (depth == myth_tls_tree_depth) {
    /* leaf */
#if MYTH_TLS_DBG
    assert(n->type == myth_tls_tree_node_type_leaf);
    assert(stride == (1 << myth_tls_tree_node_log_n_entries_in_leaf));
#endif
    int i;
    int s = 0;
    myth_key_t k = base;
    for (i = 0; i < myth_tls_tree_node_n_entries_in_leaf; i++, k++) {
      void * val = n->entries[i].value;
      void (*destructor)(void *) = ka->keys[k].destructor;
      if (destructor) {
	n->entries[i].value = 0;
	destructor(val);
	s++;
      }
    }
    return s;
  } else {
    /* internal node */
#if MYTH_TLS_DBG
    assert(n->type == myth_tls_tree_node_type_internal);
#endif
    int i;
    int s = 0;
    int c_stride = stride >> myth_tls_tree_node_log_n_children;
    int c_base = base;
    for (i = 0; i < myth_tls_tree_node_n_children; i++) {
      myth_tls_tree_node_t * c = n->children[i];
      if (!c) break;
      s += myth_tls_call_destructors_rec(c, depth + 1, c_base, c_stride, ka);
      c_base += stride;
    }
    return s;
  }
}

static inline int
myth_tls_call_destructors(myth_tls_tree_node_t * n,
			  myth_tls_key_allocator_t * ka) {
  return myth_tls_call_destructors_rec(n, 0, 0, myth_tls_n_keys, ka);
}

static inline int
myth_tls_tree_destroy_rec(myth_tls_tree_node_t * n,
			  int depth, myth_key_t base, myth_key_t stride) {
  assert(n);
  if (depth == myth_tls_tree_depth) {
    myth_free(n);
  } else {
    int i;
    int c_stride = stride >> myth_tls_tree_node_log_n_children;
    int c_base = base;
    for (i = 0; i < myth_tls_tree_node_n_children; i++) {
      myth_tls_tree_node_t * c = n->children[i];
      if (!c) break;
      myth_tls_tree_destroy_rec(c, depth + 1, c_base, c_stride);
      c_base += stride;
    }
  }
  return 0;
}

static inline int
myth_tls_tree_destroy(myth_tls_tree_node_t * n) {
  return myth_tls_tree_destroy_rec(n, 0, 0, myth_tls_n_keys);
}

static inline void
myth_tls_tree_fini(myth_tls_tree_t * t, myth_tls_key_allocator_t * ka) {
  myth_tls_tree_node_t * r = t->root;
  if (r) {
    myth_tls_call_destructors(r, ka);
    myth_tls_tree_destroy(r);
  }
}

static inline void * myth_tls_tree_get(myth_tls_tree_t * t, int idx) {
  if (idx < 0 || idx >= myth_tls_n_keys) {
    return 0;
  }
  myth_tls_tree_node_t * n = t->root;
  int i;
  if (!n) return 0;
  for (i = 0; i < myth_tls_tree_depth; i++) {
    int shift = (myth_tls_tree_depth - i - 1)
      * myth_tls_tree_node_log_n_children
      + myth_tls_tree_node_log_n_entries_in_leaf;
    int cidx = (idx >> shift) & (myth_tls_tree_node_n_children - 1);
    assert(n->type == myth_tls_tree_node_type_internal);
    n = n->children[cidx];
    if (!n) return 0;
  }
  assert(n->type == myth_tls_tree_node_type_leaf);
  return n->entries[idx & (myth_tls_tree_node_n_entries_in_leaf - 1)].value;
}

static inline int myth_tls_tree_set(myth_tls_tree_t * t, int idx,
				    const void * v) {
  if (idx < 0 || idx >= myth_tls_n_keys) {
    return EINVAL;
  }
  myth_tls_tree_node_t * n = t->root;
  if (!n) {
    if (myth_tls_tree_depth == 0) {
      t->root = n = myth_tls_tree_node_alloc_leaf();
    } else {
      t->root = n = myth_tls_tree_node_alloc_node();
    }
  }
  int i;
  for (i = 0; i < myth_tls_tree_depth; i++) {
    int shift = (myth_tls_tree_depth - i - 1)
      * myth_tls_tree_node_log_n_children
      + myth_tls_tree_node_log_n_entries_in_leaf;
    int cidx = (idx >> shift) & (myth_tls_tree_node_n_children - 1);
    assert(n->type == myth_tls_tree_node_type_internal);
    myth_tls_tree_node_t * c = n->children[cidx];
    if (!c) {
      if (i < myth_tls_tree_depth - 1) {
	c = myth_tls_tree_node_alloc_node();
      } else {
	c = myth_tls_tree_node_alloc_leaf();
      }
      n->children[cidx] = c;
    }
    n = c;
  }
  assert(n->type == myth_tls_tree_node_type_leaf);
  /* a cast to discard const qualifier  */
  n->entries[idx & (myth_tls_tree_node_n_entries_in_leaf - 1)].value = (void *)v;
  return 0;
}


static inline void myth_tls_key_allocator_init(myth_tls_key_allocator_t * s) {
  int i;
  for (i = 0; i < myth_tls_n_keys - 1; i++) {
    s->keys[i].next = &s->keys[i + 1];
  }
  s->keys[myth_tls_n_keys - 1].next = 0;
  s->free = &s->keys[0];
}

static inline void myth_tls_key_allocator_fini(myth_tls_key_allocator_t * s) {
  (void)s;
}

myth_tls_key_allocator_t g_myth_tls_key_allocator[1];
static inline void myth_tls_init(int n_workers) {
  (void)n_workers;
  myth_tls_key_allocator_init(g_myth_tls_key_allocator);
}

myth_tls_key_allocator_t g_myth_tls_key_allocator[1];
static inline void myth_tls_fini() {
  myth_tls_key_allocator_fini(g_myth_tls_key_allocator);
}

/* allocate a new key */
static inline int
myth_tls_key_allocator_alloc(myth_tls_key_allocator_t * s,
			     myth_tls_destructor_fun_t destructor) {
  while (1) {
    /* try to pull the element from the free list */
    myth_tls_key_entry_t * ke = s->free;
    if (ke) {
      myth_tls_key_entry_t * next = ke->next;
      if (__sync_bool_compare_and_swap(&s->free, ke, next)) {
	/* mark the key as used */
	ke->next = (myth_tls_key_entry_t *)-1;
	ke->destructor = destructor;
	return ke - s->keys;
      }
    } else {
      return -1;
    }
  }
}

/* deallocate a key */
static inline myth_tls_destructor_fun_t
myth_tls_key_allocator_dealloc(myth_tls_key_allocator_t * s, int key) {
  if (key < 0 || key >= myth_tls_n_keys) {
    return (myth_tls_destructor_fun_t)-1;
  }
  myth_tls_key_entry_t * ke = &s->keys[key];
  /* make sure the key is being used */
  if (ke->next != (myth_tls_key_entry_t *)-1) {
    return (myth_tls_destructor_fun_t)-1;
  }
  myth_tls_destructor_fun_t f = ke->destructor;
  while (1) {
    /* try to push the cell to the free list */
    myth_tls_key_entry_t * head = s->free;
    ke->next = head;
    if (__sync_bool_compare_and_swap(&s->free, head, ke)) {
      return f;
    }
  }
}

static inline int myth_key_create_body(myth_key_t * key,
				       myth_tls_destructor_fun_t destructor) {
  int _ = myth_ensure_init();
  myth_key_t k = myth_tls_key_allocator_alloc(g_myth_tls_key_allocator,
					      destructor);
  (void)_;
  if (k == -1) {
    return EINVAL;
  } else {
    *key = k;
    return 0;
  }
}

static inline int myth_key_delete_body(myth_key_t key) {
  int _ = myth_ensure_init();
  myth_tls_destructor_fun_t f
    = myth_tls_key_allocator_dealloc(g_myth_tls_key_allocator, key);
  (void)_;
  if (f == (myth_tls_destructor_fun_t)-1) {
    return EINVAL;
  } else {
    return 0;
  }
}

static inline myth_thread_t myth_self_body(void) {
  int _ = myth_ensure_init();	/* TODO: hoist this outside */
  myth_running_env_t env = myth_get_current_env();
  (void)_;
  return env->this_thread;
}

static inline int myth_equal_body(myth_thread_t t1, myth_thread_t t2) {
  return t1 == t2;
}

static inline int myth_setspecific_body(myth_key_t key, const void * val) {
  myth_thread_t th = myth_self_body();
  return myth_tls_tree_set(th->tls, key, val);
}

static inline void * myth_getspecific_body(myth_key_t key) {
  myth_thread_t th = myth_self_body();
  return myth_tls_tree_get(th->tls, key);
}

#endif /* MYTH_TLS_FUNC_H_ */
