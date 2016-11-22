/*
 * myth_tls.h
 */
#pragma once
#ifndef MYTH_TLS_H_
#define MYTH_TLS_H_

#include "myth/myth.h"
#include "myth_config.h"

/* nuts and bolts for thread specific key
   (1) data structure to maintain unused keys and to allocate a free key
       (myth_tls_key_allocator)
   (2) data structure to maintain thread-local storage and to support
       fast access to it

 */

/* some parameters determining the maximum number of thread local
   keys we support.  Linux man page says there is a static limit,
   so I don't bother to support a huge number of them. 
   in the following setting, it will be 1024 and it matches what
   we usually have in Linux.  details of these parameters are 
   described later */

enum {
  myth_tls_tree_node_log_n_children = 2,
  myth_tls_tree_node_n_children = 1 << myth_tls_tree_node_log_n_children,
  myth_tls_tree_node_log_n_entries_in_leaf = 4,
  myth_tls_tree_node_n_entries_in_leaf = 1 << myth_tls_tree_node_log_n_entries_in_leaf,
  myth_tls_tree_depth = 3,
  myth_tls_n_keys = 1 << (myth_tls_tree_node_log_n_entries_in_leaf
			  + myth_tls_tree_node_log_n_children
			  * myth_tls_tree_depth),
};

/* 
 * data structure to maintain unused keys and to allocate a free key  
 */

/* typedef destructor */
typedef void (*myth_tls_destructor_fun_t)(void *);

/* unused keys are maintained as a free list */
typedef struct myth_tls_key_entry {
  struct myth_tls_key_entry * next;
  myth_tls_destructor_fun_t destructor;
} myth_tls_key_entry_t;

/* the toplevel data structure to allocate unsed keys from */
typedef struct myth_tls_key_allocator {
  myth_tls_key_entry_t * free;	/* head of free list */
  myth_tls_key_entry_t keys[myth_tls_n_keys]; /* cells in the free list */
} myth_tls_key_allocator_t;

/* 
 * data structure to maintain thread-local storage and support fast access
 * to it.
 * if we don't worry about wasting space, it would be just a flat array
 * of myth_tls_n_keys, but it is too wasteful (e.g., 8 * 1024 = 8192 bytes
 * for each thread).  so a page table-like structure is used.
 * since the key is always a small integer, we can descend the tree using
 * bits in the key.
 */

/* 

   myth_tls_tree_t  {     root    }
                           |
                        +-------+
                        |       |  d=0
                        +-------+
                         / / \ \

                          .....

                +-------+ +-------+
                | leaf  | | leaf  | ...   d=depth
                +-------+ +-------+

 */

/* a single thread local storage (just a void* pointer) */
typedef struct myth_tls_entry {
  void * value;
} myth_tls_entry_t;

#define MYTH_TLS_DBG 1

#if MYTH_TLS_DBG
enum {
  myth_tls_tree_node_type_leaf,
  myth_tls_tree_node_type_internal
};
#endif

typedef struct myth_tls_tree_node {
#if MYTH_TLS_DBG
  int type;
#endif
  union {
    struct myth_tls_tree_node * children[myth_tls_tree_node_n_children];
    myth_tls_entry_t entries[1];
  };
} myth_tls_tree_node_t;

typedef struct {
  myth_tls_tree_node_t * root;
} myth_tls_tree_t;

static inline myth_thread_t myth_self_body(void);
static inline void myth_tls_tree_init(myth_tls_tree_t * t);
static inline void myth_tls_tree_fini(myth_tls_tree_t * t,
				      myth_tls_key_allocator_t * ka);
static inline void myth_tls_init(int nworkers);

extern myth_tls_key_allocator_t g_myth_tls_key_allocator[1];

#endif /* MYTH_TLS_H_ */
