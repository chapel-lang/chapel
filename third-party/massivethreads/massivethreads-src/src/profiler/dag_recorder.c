/* 
 * dag_recorder.c
 */

/* 
   this file primarily implements dr_start and dr_stop
 */
#define DAG_RECORDER 2
#include "dag_recorder_impl.h"

/* the (only) shared global variable */

dr_global_state GS = {
  0,				/* initialized */
  0,				/* num_workers */
  0,				/* root */
  0,				/* start_clock */
  0,				/* generation */
  0,				/* thread_start_hook */
  0,				/* worker_specific_state_array */
  0,				/* worker_specific_state_list */
  0,				/* worker_specific_state_key */
  0,				/* worker_specific_state_key_valid */
  0,				/* worker_id_key */
  0,				/* worker_id_key_valid */
  0,				/* worker_id_counter */
  { /* options */
    0,	 /* dag_file_prefix */
    0,	 /* dag_file */
    0,	 /* stat_file */
    0,	 /* gpl_file */
    0,	 /* dot_file */
    0,	 /* sqlite_file */
    0,	 /* text_file */
    0,	 /* nodes_file */	
    0,	 /* edges_file */	
    0,	 /* strings_file */	
    0,	 /* text_file_sep */
    0,	 /* uncollapse_min; obsolete. */
    0,	 /* collapse_max used to be (1L << 60), */
    0,	 /* node_count_target */
    0,	 /* prune_threshold */
    0,	 /* collapse_max_count */
    0,	 /* alloc unit in MB */
    0,	 /* pre-alloc per worker */
    0,	 /* pre-alloc */
    {	 /* hooks */
      0, 0, 0, 0, 0, 0, 0, 0, 0
    },
    0,	 /* gpl_sz */
    0,	 /* worker_specific_state_array */
    0,	 /* dbg_level */
    0,	 /* verbose_level */
    0,	 /* chk_level */
    0,	 /* record cpu */
  }
};

/* --------------------- dr_get_worker ------------------- */

/* initialize free list of dag_nodes */
static void 
dr_dag_node_freelist_init(dr_dag_node_freelist * fl) {
  fl->tail = fl->head = 0;
  fl->pages = 0;
}

/* 
 prune stack --- 
 the stack used for non-recursively contracting
 subgraphs.
 */

/* initialize prune stack */
static void 
dr_prune_nodes_stack_init(dr_prune_nodes_stack * S) {
  S->sz = 1000;
  S->entries 
    = (dr_prune_nodes_stack_ent*)
    dr_malloc(S->sz * sizeof(dr_prune_nodes_stack_ent));
  S->n = 0;
}

static void 
dr_prune_nodes_stack_cleanup(dr_prune_nodes_stack * S) {
  dr_free(S->entries, S->sz * sizeof(dr_prune_nodes_stack_ent));
}

/* destroy prune stack */
static void 
dr_prune_nodes_stack_destroy(dr_prune_nodes_stack * S) {
  dr_free(S->entries, S->sz * sizeof(dr_prune_nodes_stack_ent));
}

/* thread specific state for dag recorder */
static void 
insert_worker_specific_state(dr_worker_specific_state * wss) {
  dr_worker_specific_state * volatile * hp 
  = &GS.worker_specific_state_list;
  while (1) {
    dr_worker_specific_state * h = *hp;
    wss->next = h;
    if (__sync_bool_compare_and_swap(hp, h, wss)) break;
  }
}

static int
dr_init_worker_specific_state(dr_worker_specific_state * ts,
			      int worker) {
  /* pre-allocate pages to the free list */
  long pages_per_worker = GS.opts.pre_alloc_per_worker;
  size_t alloc_sz = GS.opts.alloc_unit_mb << 20;
  long j;
  dr_dag_node_freelist * fl = ts->freelist;
  dr_dag_node_freelist_init(fl);
  for (j = 0; j < pages_per_worker; j++) {
    dr_dag_node_freelist_add_page(fl, alloc_sz);
  }
  dr_prune_nodes_stack_init(ts->prune_stack);
  /* this field is actually overwritten by the
     thread that calls dr_start */
  ts->task = (dr_dag_node*)0;
  ts->parent = (dr_dag_node*)0;
  ts->worker = worker;
  if (GS.thread_start_hook) {
    GS.thread_start_hook(worker);
  }
  return 1;
}

static void
dr_cleanup_worker_specific_state(dr_worker_specific_state * ts) {
  dr_prune_nodes_stack_cleanup(ts->prune_stack);
}

dr_worker_specific_state *
dr_make_worker_specific_state(int worker) {
  dr_worker_specific_state * wss 
    = dr_malloc(sizeof(dr_worker_specific_state));
  dr_init_worker_specific_state(wss, worker);
  insert_worker_specific_state(wss);
  return wss;
}

/* thread specific state for dag recorder */
static dr_worker_specific_state *
dr_make_worker_specific_state_array(int num_workers) {
  (void)dr_check(num_workers != -1);
  dr_worker_specific_state * wss
    = dr_malloc(sizeof(dr_worker_specific_state) * num_workers);
  int i;
  for (i = 0; i < num_workers; i++) {
    dr_init_worker_specific_state(&wss[i], i);
  }
  return wss;
}

static void
dr_free_worker_specific_state_list() {
  dr_worker_specific_state * wss;
  dr_worker_specific_state * next;
  for (wss = GS.worker_specific_state_list; wss; wss = next) {
    next = wss->next;
    dr_cleanup_worker_specific_state(wss);
    dr_free(wss, sizeof(dr_worker_specific_state));
  }
  GS.worker_specific_state_list = 0;
}

static void
dr_free_worker_specific_state_array() {
  int i;
  (void)dr_check(GS.worker_specific_state_array_sz);
  for (i = 0; i < GS.worker_specific_state_array_sz; i++) {
    dr_cleanup_worker_specific_state(&GS.worker_specific_state_array[i]);
  }
  dr_free(GS.worker_specific_state_array, 
	  sizeof(dr_worker_specific_state) 
	  * GS.worker_specific_state_array_sz);
  GS.worker_specific_state_array = 0;
  GS.worker_specific_state_array_sz = 0;
}

/* initialize dag recorder, when called 
   for the first time.
   second or later invocations have no effects
 */

static void dr_create_worker_specific_state_key() {
  pthread_key_t key;
  int err = pthread_key_create(&key, NULL);
  (void)dr_check(err == 0);
  GS.worker_specific_state_key = key;
  GS.worker_specific_state_key_valid = 1;
}

static void dr_create_worker_id_key() {
  pthread_key_t key;
  int err = pthread_key_create(&key, NULL);
  (void)dr_check(err == 0);
  GS.worker_id_key = key;
  GS.worker_id_key_valid = 1;
}

void dr_ensure_worker_id_key() {
  if (!GS.worker_id_key_valid) {
    dr_create_worker_id_key();
  }
}


/* check env variable v and read it as int.
   return 0 if v is not found in the environment 
   or it canot be parsed as an integer */
static int dr_get_max_workers_by_env(const char * v) {
  char * s = getenv(v);
  int x;
  if (!s) {
    fprintf(stderr, 
	    "error: could not get number of workers\n"
	    "set environment variable %s\n", v);
    return 0;
  }
  x = atoi(s);
  if (x <= 0) {
    fprintf(stderr, 
	    "error: invalid value in environment varible %s (%s)\n"
	    "set a positive integer\n", v, s);
    return 0;
  }
  return x;
} 

int dr_tbb_max_workers() {
  //return dr_get_max_workers_by_env("TBB_NTHREADS");
  return -1;
}

/* get the number of workers of nanos++ programs */
int dr_nanox_max_workers() {
  return dr_get_max_workers_by_env("NX_PES");
}

/* initialize dag recorder.

   if num_workers is not -1, it is the number of
   workers that participates in computation. -1
   means it is unknown. 
 */
static int
dr_init_(dr_options * opts, int num_workers) {
  if (!GS.initialized) {
    GS.initialized = 1;
    dr_opts_init(opts);
    if (GS.opts.on) {
      dr_opts_print(opts);
      dr_ensure_worker_id_key();
      dr_create_worker_specific_state_key();
      if (GS.opts.worker_specific_state_array
	  && num_workers != -1) {
	GS.worker_specific_state_array 
	  = dr_make_worker_specific_state_array(num_workers);
	GS.worker_specific_state_array_sz = num_workers;
      } else {
	/* we use linear list for worker-specific states */
	if (GS.opts.worker_specific_state_array) {
	  fprintf(stderr, "dr_init_ : warning : "
		  "an array is requested for worker-specific state,"
		  " yet the number of workers is unknown."
		  " resort to linear list\n");
	}
      }
    }
  }
  return GS.opts.on;			/* already initialized. go ahead */
}

/* stop profiling */
void dr_stop__(const char * file, int line, int worker) {
  dr_end_task__(file, line, worker);
  (void)dr_check(GS.generation % 2);
  /* when dr is running, turn it off */
  if (GS.generation % 2) 
    GS.generation++;
}

/* --- free all descendants of g (and optionally g also) --- */

void 
dr_dag_node_free(dr_dag_node * n, 
		 dr_dag_node_freelist * fl) {
#if DAG_RECORDER_VALGRIND_MEM_DBG
  return dr_free(n, sizeof(dr_dag_node));
#else
  n->next = fl->head;
  if (!fl->head)
    fl->tail = n;
  fl->head = n;
#endif
}

void 
dr_free_dag(dr_dag_node * g, int free_root,
	    dr_dag_node_freelist * fl) {
  dr_dag_node_stack s[1];
  dr_dag_node_stack_init(s);
  dr_dag_node_stack_push(s, g);
  while (s->top) {
    dr_dag_node * x = dr_dag_node_stack_pop(s);
    if (x->info.kind == dr_dag_node_kind_create_task) {
      if (x->child) {
	dr_dag_node_stack_push(s, x->child);
      }
    } else if (x->info.kind >= dr_dag_node_kind_section) {
      dr_dag_node_stack_push_children(s, x);
    }
    if (x != g) dr_dag_node_free(x, fl);
  }
  if (free_root) {
    dr_dag_node_free(g, fl);
  } else {
    dr_dag_node_list_init(g->subgraphs);
  }
  dr_dag_node_stack_fini(s);
}

int dr_register_thread_start_hook(int (*f)(int worker)) {
  (void)dr_check(!GS.thread_start_hook);
  GS.thread_start_hook = f;
  return 1;
}

/* initialize when called for the first time;
   and start profiling */
void dr_start__(dr_options * opts, const char * file, int line,
		int worker, int num_workers) {
  if (dr_init_(opts, num_workers)) {
    if (GS.root) {
      /* GS.root is the root task of the previous profiling
	 period. clear it before we start the current period */
      dr_worker_specific_state * wss 
	= dr_get_worker_specific_state(worker);
      dr_free_dag(GS.root, 1, wss->freelist);
    }
    (void)dr_check(GS.generation % 2 == 0);
    GS.generation++;
    GS.start_clock = dr_get_tsc();
    dr_start_task__(0, file, line, worker);

    dr_worker_specific_state * wss 
      = dr_get_worker_specific_state(worker);
    GS.root = dr_get_cur_task_(wss);
  }
}


/* free free list of the worker */
static void 
dr_free_freelist(dr_dag_node_freelist * fl) {
  dr_dag_node_page * head = fl->pages;
  dr_dag_node_page * page;
  dr_dag_node_page * next;
  for (page = head; page; page = next) {
    next = page->next;
    dr_free(page, page->sz);
  }
}

static void 
dr_free_freelists() {
  if (GS.worker_specific_state_array) {
    int i;
    for (i = 0; i < GS.worker_specific_state_array_sz; i++) {
      dr_free_freelist(GS.worker_specific_state_array[i].freelist);
    }
  } else {
    dr_worker_specific_state * ts;
    for (ts = GS.worker_specific_state_list; ts; ts = ts->next) {
      dr_free_freelist(ts->freelist); /* ?? */
    }
  }
}

static void
dr_destroy_prune_stacks() {
  if (GS.worker_specific_state_array) {
    int i;
    (void)dr_check(GS.worker_specific_state_array_sz > 0);
    for (i = 0; i < GS.worker_specific_state_array_sz; i++) {
      dr_prune_nodes_stack_destroy(GS.worker_specific_state_array[i].prune_stack);
    }
  } else {
    dr_worker_specific_state * ts;
    for (ts = GS.worker_specific_state_list; ts; ts = ts->next) {
      dr_prune_nodes_stack_destroy(ts->prune_stack); /* ?? */
    }
  }
}

/* completely uninitialize */
void dr_cleanup__(const char * file, int line, 
		  int worker, int num_workers) {
  (void)num_workers;
  if (GS.initialized) {
    if (GS.generation % 2) {
      dr_stop__(file, line, worker);
    }
    /* get dag tree back into the free list of the calling worker */
    if (GS.generation > 0) {
      (void)dr_check(GS.root);
      dr_worker_specific_state * wss 
	= dr_get_worker_specific_state(worker);
      dr_free_dag(GS.root, 1, wss->freelist); 
      GS.root = 0;
      /* get everybody's freelists back into the underlying malloc */
      dr_free_freelists();
      /* free thread specific data structure */
      dr_free_worker_specific_state_array();
      dr_free_worker_specific_state_list();
      /* destroy prune stacks */
      dr_destroy_prune_stacks();
    }
    GS.initialized = 0;
  }
}

