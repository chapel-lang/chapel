/*
 * dag recorder 2.0
 */

#pragma once

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>

/* 
     task ::= section* end
     section ::= task_group (section|create)* wait 

 */

/* never turn this flag on unless you know what 
   you are doing.  if set, it does not use 
   a custom free list but resort to malloc
   every time it needs memory.  it is used
   to make it easy to debug leak and invalid
   access by valgrind  */
#define DAG_RECORDER_VALGRIND_MEM_DBG 0

#if !defined(DAG_RECORDER_VERBOSE_LEVEL)
#define DAG_RECORDER_VERBOSE_LEVEL GS.opts.verbose_level
#endif

#if !defined(DAG_RECORDER_DBG_LEVEL)
#define DAG_RECORDER_DBG_LEVEL GS.opts.dbg_level
#endif

#if !defined(DAG_RECORDER_CHK_LEVEL)
#define DAG_RECORDER_CHK_LEVEL GS.opts.chk_level
#endif

#if !defined(DAG_RECORDER_RECORD_CPU)
#define DAG_RECORDER_RECORD_CPU GS.opts.record_cpu
#endif

#ifdef __cplusplus
extern "C" {
#endif

  /* a kind of nodes.
     a node is of kind k when it finishes
     by issuing k.
     in many places, the code assumes
     k < dr_dag_node_kind_section
     iff k is a primitive node */
  typedef enum {
    dr_dag_node_kind_create_task, /* ended by create_task */
    dr_dag_node_kind_wait_tasks,  /* ended by wait_tasks */
    dr_dag_node_kind_other,	  /* ended by other reasons */
    dr_dag_node_kind_end_task,	  /* ended by ending a task */
    dr_dag_node_kind_section,
    dr_dag_node_kind_task,
  } dr_dag_node_kind_t;
  
  /* the state of a node.
     a node is either 
     (i) not ready,
     (ii) ready but not running, 
     (iii) running, or 
     (iv) finished
     we do not explicitly count (i) or (iv).
     ready tasks (category (ii)) are further
     classified by the event that made them
     ready.
     
     create : a task created a task and the
     node is the first node the create task
     create_cont : a task created a task and the
     node is the first in the parent task after 
     create_task
     end : a task issued a wait_task at one
     point, when one of the waited children
     has not yet finished. later the last
     child finished and the task is now ready.
     the node is the first node after wait_tasks
     wait_cont : a task issued a wait_task,
     when all waited children have finished.
     the node is the first node after wait_tasks
  */
  typedef enum {
    dr_dag_edge_kind_end,    /* end -> parent */
    dr_dag_edge_kind_create, /* create -> child */
    dr_dag_edge_kind_create_cont, /* create -> next */
    dr_dag_edge_kind_wait_cont,	/* wait -> next */
    dr_dag_edge_kind_other_cont, /* other -> next */
    dr_dag_edge_kind_max,
  } dr_dag_edge_kind_t;

  typedef struct dr_dag_node_list dr_dag_node_list;

  /* list of dag nodes */
  struct dr_dag_node_list {
    long n;			/* number of elements */
    dr_dag_node * head;
    dr_dag_node * tail;
  };

  /* code position (file,line_no) */
  typedef struct {
    /* pointer to filename. valid in dr_dag_node */
    const char * file;
    /* index in dr_flat_string_table. valid in dr_pi_dag_node */    
    long file_idx;
    /* line number */
    long line;
  } code_pos;

  /* code position + clock */
  enum { dr_max_counters = 4 };
  typedef struct {
    dr_clock_t t;		/* clock */
    long long counters[dr_max_counters];	/*  */
    int worker;
    int cpu;
    code_pos pos;		/* code position */
  } dr_clock_pos;

  typedef struct dr_dag_node_info {
    /* start clock,position */
    dr_clock_pos start;
    /* end clock,position */
    dr_clock_pos end;
    /* earliest start time */
    dr_clock_t est;
    /* work */
    dr_clock_t t_1;
    /* critical path */
    dr_clock_t t_inf;
    /* time at which this node became ready */
    dr_clock_t first_ready_t;
    /* time at which the last node started */
    dr_clock_t last_start_t;
    /* weighted sum of ready tasks */
    dr_clock_t t_ready[dr_dag_edge_kind_max];
    /* "logical" number of nodes in this subgraph.
       "logical" means we keep track of collapsed nodes */
    long logical_node_counts[dr_dag_node_kind_section];
    /* number of edges connecting nodes in this subgraph */
    long logical_edge_counts[dr_dag_edge_kind_max];
    /* actual node count under it. 
       for leaf or collapsed nodes, they are 1.
       for create_task node, it does NOT include 
       nodes in children.
       it is however include in the count of the
       section that contains it */
    long cur_node_count;
    /* minimum number of nodes if all collapsable nodes are collapsed */
    long min_node_count;
    /* direct children of create_task type */
    long n_child_create_tasks;
    /* worker */
    int worker;
    /* cpu */
    int cpu;
    dr_dag_node_kind_t kind;
    dr_dag_edge_kind_t in_edge_kind;
    long long counters_1[dr_max_counters];
    long long counters_inf[dr_max_counters];
  } dr_dag_node_info;

  typedef struct dr_pi_dag_node dr_pi_dag_node;

  /* a node of the in-memory, growing/shrinking dag */
  struct dr_dag_node {
    dr_dag_node_info info;
    /* a pointer to the next node in lists */
    dr_dag_node * next;
    /* a pointer used to recursively
       convert the graph into the 
       position independent format */
    dr_pi_dag_node * forward;
    union {
      /* pointer to child. valid when info.kind == create_task */
      dr_dag_node * child;
      struct {
	/* list of subgraphs. valid when info.kind == section/task */
	dr_dag_node_list subgraphs[1];
	union {
	  /* pointer to the immediately enclosing section */
	  dr_dag_node * parent_section; /* kind == section */
	  /* pointer to the currently active section (or itself) */
	  dr_dag_node * active_section; /* kind == task */
	};
      };
    };
  };

  /* nodes are allocated in the unit of page */
  typedef struct dr_dag_node_page {
    struct dr_dag_node_page * next; /* next pointer in a page free list */
    long sz;			    /* size of the page in bytes */
    dr_dag_node nodes[2]; /* this is the minimum size. we allocate more */
  } dr_dag_node_page;

  /* free list of nodes */
  typedef struct {
    dr_dag_node * head;
    dr_dag_node * tail;
    dr_dag_node_page * pages;
  } dr_dag_node_freelist;

  /* an entry in the stack used by 
     the non recursive version 
     of dr_prune_nodes */
  typedef struct {
    dr_dag_node * x;		/* pointer to the node */
    long budget;
    long budget_left;
    long nodes_left;
    dr_dag_node * last_child;
    long visit_count;
  } dr_prune_nodes_stack_ent;

  typedef struct {
    dr_prune_nodes_stack_ent * entries;
    long sz;
    long n;
  } dr_prune_nodes_stack;

  typedef struct dr_worker_specific_state {
    union {
      struct {
	/* pointer next */
	struct dr_worker_specific_state * next;
	dr_dag_node * task;		/* current task */
	dr_dag_node_freelist freelist[1];
	dr_prune_nodes_stack prune_stack[1];
	/* only used in Cilk: it holds a pointer to 
	   the interval that just created a task */
	dr_dag_node * parent;
	int worker;		/* worker id */
      };
      char minimum_size[64];
    };
  } dr_worker_specific_state;

  typedef struct dr_global_state {
    int initialized;
    /* root of the task graph. 
       used (only) by print_task_graph */
    dr_dag_node * root;
    /* the clock when dr_start() was called */
    dr_clock_t start_clock;
    /* generation is incremented everytime we start/stop
       dag recorder. it is odd iff profiling is on */
    long generation;
    /* callback */
    int (*thread_start_hook)(int worker);
    /* two methods to maintain worker-specific states
       (1) fixed-sized array
       (2) linear list
       the former requires the maximum number of workers
       to be given upon initialization. some systems (e.g., TBB)
       do not provide it. */

    /* (1) fixed-sized array */
    dr_worker_specific_state * worker_specific_state_array;
    int worker_specific_state_array_sz;
    /* (2) linear list */
    dr_worker_specific_state * volatile worker_specific_state_list;

    /* key for worker-specific state */
    pthread_key_t worker_specific_state_key;
    pthread_key_t worker_specific_state_key_valid;

    /* key for worker id */
    pthread_key_t worker_id_key;
    int worker_id_key_valid;
    int worker_id_counter;

    dr_options opts;
  } dr_global_state;

  static int dr_check_(int condition, const char * condition_s, 
		       const char * __file__, int __line__, 
		       const char * func) {
    if (!condition) {
      fprintf(stderr, "%s:%d:%s: dag recorder check failed : %s\n", 
	      __file__, __line__, func, condition_s); 
      exit(1);
    }
    return 1;
  }

#define dr_check(x) (!DAG_RECORDER_CHK_LEVEL || dr_check_(((x)?1:0), #x, __FILE__, __LINE__, __func__))

  extern dr_global_state GS;

  /* malloc and free. with some debug support */
  static void * 
  dr_malloc(size_t sz) {
    void * a = malloc(sz);
    if (DAG_RECORDER_CHK_LEVEL) {
      if (!a) { perror("malloc"); exit(1); }
    }
    if (DAG_RECORDER_VERBOSE_LEVEL>=4) {
      printf("dr_malloc(%ld) -> %p\n", sz, a);
    }
    if (DAG_RECORDER_DBG_LEVEL>=2) {
      memset(a, 255, sz);
    }
    return a;
  }

  static void
  dr_free(void * a, size_t sz) {
    if (DAG_RECORDER_VERBOSE_LEVEL>=4) {
      printf("dr_free(%p, %ld)\n", a, sz);
    }
    if (DAG_RECORDER_CHK_LEVEL) {
      if (a) {
	if (DAG_RECORDER_DBG_LEVEL>=1) memset(a, 222, sz);
	free(a);
      } else {
	(void)dr_check(sz == 0);
      }
    } else {
      free(a);
    }
  }

  /* allocate a page of sz_ bytes and put 
     nodes into free list fl */
  static dr_dag_node *
  dr_dag_node_freelist_add_page(dr_dag_node_freelist * fl, 
				size_t sz_) {
    size_t sz = (sz_ > sizeof(dr_dag_node_page) 
		 ? sz_ : sizeof(dr_dag_node_page));
    dr_dag_node_page * page = (dr_dag_node_page *)dr_malloc(sz);
    int n = (sz - sizeof(dr_dag_node_page)) / sizeof(dr_dag_node) + 2;
    int i;
    /* push the page to the top of the pages list */
    page->next = fl->pages;
    page->sz = sz;
    fl->pages = page;
    /* chop page into nodes */
    (void)dr_check(n > 0);
    /* not necessary, but just in case */
    (void)dr_check(n > 1);
    for (i = 0; i < n; i++) {
      page->nodes[i].next = &page->nodes[i+1];
#if 0
      int j;
      for (j = 0; j < dr_max_counters; j++) {
	page->nodes[i].info.start.counters[j] = 0;
	page->nodes[i].info.end.counters[j] = 0;
      }
#endif
    }
    page->nodes[n - 1].next = 0;
    /* append the list of nodes to the nodes list */
    if (fl->head) {
      (void)dr_check(fl->tail);
      fl->tail->next = &page->nodes[0];
    } else {
      fl->head = &page->nodes[0];
    }
    fl->tail = &page->nodes[n - 1];
    return fl->head;
  }

  /* allocate a node from free list */
  static dr_dag_node *
  dr_dag_node_alloc(dr_dag_node_freelist * fl) {
    dr_dag_node * ch; int j;
#if DAG_RECORDER_VALGRIND_MEM_DBG
    /* when debugging with valgrind,
       do not manage memory by ourselves.
       let malloc does everything */
    ch = (dr_dag_node *)dr_malloc(sizeof(dr_dag_node));
    ch->next = 0;
#else
    ch = fl->head;
    if (!ch) {
      if (DAG_RECORDER_DBG_LEVEL>=1) {
	fprintf(stderr, 
		"dr_dag_node_alloc(%p) allocate page\n", fl);
      }
      /* free list empty. supplement it with a page */
      ch = dr_dag_node_freelist_add_page(fl, 
					 (GS.opts.alloc_unit_mb << 20));
    }

    {
      /* pop the head element off the list */
      dr_dag_node * next = ch->next;
      (void)dr_check(fl->tail);
      fl->head = next;
      if (!next) fl->tail = 0;
      ch->next = 0;
      if (DAG_RECORDER_VERBOSE_LEVEL>=4) {
	printf("dr_dag_node_alloc(%p) -> %p\n", fl, ch);
      }
    }
#endif
    /* initialize the node */
    for (j = 0; j < dr_max_counters; j++) {
      ch->info.start.counters[j] = 0;
      ch->info.end.counters[j] = 0;
    }
    return ch;
  }

  static void 
  dr_dag_node_list_init(dr_dag_node_list * l) {
    l->n = 0;
    l->tail = l->head = 0;
  }

  /* list */

  static int
  dr_dag_node_list_empty(dr_dag_node_list * l) {
    return l->n == 0;
  }

#if ! __CILK__
  __attribute__((unused)) 
#endif
  static long
  dr_dag_node_list_size(dr_dag_node_list * l) {
    return l->n;
  }

  /* the first element of l */
  static dr_dag_node *
  dr_dag_node_list_first(dr_dag_node_list * l) {
    if (DAG_RECORDER_CHK_LEVEL) {
      if (l->head) {
	(void)dr_check(l->tail);
      } else {
	(void)dr_check(!l->tail);
      }
    }
    return l->head;
  }

  /* the last element of l */
  static dr_dag_node *
  dr_dag_node_list_last(dr_dag_node_list * l) {
    if (DAG_RECORDER_CHK_LEVEL) {
      if (l->head) {
	(void)dr_check(l->tail);
      } else {
	(void)dr_check(!l->tail);
      }
    }
    return l->tail;
  }

  /* extend l by one element and return a pointer
     to the new element */
  static dr_dag_node *
  dr_dag_node_list_push_back(dr_dag_node_list * l,
			     dr_dag_node_freelist * fl) {
    dr_dag_node * n = dr_dag_node_alloc(fl);
    (void)dr_check(!n->next);
    if (l->head) {
      (void)dr_check(l->tail);
      l->tail->next = n;
    } else {
      (void)dr_check(!l->tail);
      l->head = n;
    }
    l->tail = n;
    (void)dr_check(l->head);
    (void)dr_check(l->tail);
    l->n++;
    return n;
  }

#if defined(__x86_64__)

  static unsigned long long dr_rdtsc() {
    unsigned long long u;
    asm volatile ("rdtsc;shlq $32,%%rdx;orq %%rdx,%%rax":"=a"(u)::"%rdx");
    return u;
  }
  
#elif defined(__sparc__) && defined(__arch64__)
  
  static unsigned long long dr_rdtsc(void) {
    unsigned long long u;
    asm volatile("rd %%tick, %0" : "=r" (u));
    return u;
  }

#else
  
  static unsigned long long dr_rdtsc() {
    unsigned long long u;
    asm volatile ("rdtsc" : "=A" (u));
    return u;
  }
  
#endif

  static dr_clock_t 
  dr_get_tsc() {
    return dr_rdtsc();
  }

  dr_worker_specific_state * dr_make_worker_specific_state(int worker);

  static dr_worker_specific_state * 
  dr_get_worker_specific_state(int worker) {
    if (GS.worker_specific_state_array) {
      (void)dr_check(worker >= 0);
      (void)dr_check(worker < GS.worker_specific_state_array_sz);
      return &GS.worker_specific_state_array[worker];
    } else {
      pthread_key_t wss_key = GS.worker_specific_state_key;
      if (dr_check(GS.worker_specific_state_key_valid)) {
	void * wss = pthread_getspecific(wss_key);
	if (wss) {
	  /* aleady initialized */
	  return (dr_worker_specific_state *)wss;
	} else {
	  /* this is the first time the thread calls it.
	     get a sequence number from a shared counter */
	  dr_worker_specific_state * new_wss
	    = dr_make_worker_specific_state(worker);
	  pthread_setspecific(wss_key, (void *)new_wss);
	  return new_wss;
	}
      } else {
	return 0;		/* never reach here */
      }
    }
  }

  static int worker_id_counter_get_next() {
    return __sync_fetch_and_add(&GS.worker_id_counter, 1);
  }
  
  void dr_ensure_worker_id_key();

  /* get my worker number */
  static inline int dr_get_worker_id_by_pthread_key() {
    dr_ensure_worker_id_key();
    if (dr_check(GS.worker_id_key_valid)) {
      pthread_key_t wik = GS.worker_id_key;
      void * x = pthread_getspecific(GS.worker_id_key);
      if (x) {
	/* aleady initialized */
	int w = (long)x - 1;
	return w;
      } else {
	/* this is the first time the thread calls it.
	   get a sequence number from a shared counter */
	int w = worker_id_counter_get_next();
	pthread_setspecific(wik, (void *)((long)w + 1));
	return w;
      }
    } else {
      return -1;		/* never reach */
    }
  }

  static inline int dr_tbb_get_worker() {
    return dr_get_worker_id_by_pthread_key();
  }

  static inline int dr_nanox_get_worker() {
    return dr_get_worker_id_by_pthread_key();
  }

  int dr_tbb_max_workers();
  int dr_nanox_max_workers();

  /* the current task of this worker */
  static dr_dag_node * 
  dr_get_cur_task_(dr_worker_specific_state * wss) {
    return wss->task;
  }

  /* set the current task of this worker to t */
  static void 
  dr_set_cur_task_(dr_worker_specific_state * wss, dr_dag_node * t) {
    wss->task = t;
  }

  /* initialize dag node n to become a section- or a task-type node */
  static void 
  dr_dag_node_init_section_or_task(dr_dag_node * n,
				   dr_dag_node_kind_t kind,
				   dr_dag_node * p) {
    (void)dr_check(kind >= dr_dag_node_kind_section);
    n->info.kind = kind;
    /* empty subgraphs list */
    dr_dag_node_list_init(n->subgraphs);

    if (kind == dr_dag_node_kind_section) {
      n->parent_section = p;
    } else {
      (void)dr_check(kind == dr_dag_node_kind_task);
      n->active_section = 0;
    }
  }

  static dr_dag_node * 
  dr_task_active_node(dr_dag_node * t);

  /* add a new section as a child of s (either a section or task) */
  static dr_dag_node *
  dr_push_back_section(dr_dag_node * t, dr_dag_node * s, 
		       dr_dag_node_freelist * fl) {
    if (dr_check(s->info.kind >= dr_dag_node_kind_section)) {
      dr_dag_node * new_s 
	= dr_dag_node_list_push_back(s->subgraphs, fl);
      dr_dag_node_init_section_or_task(new_s, dr_dag_node_kind_section, s);
      t->active_section = new_s;
      (void)dr_check(dr_task_active_node(t) == t->active_section);
      return new_s;
    } else {
      return (dr_dag_node *)0;
    }
  }

  /* allocate a new dag node of a task type */
  static dr_dag_node * 
  dr_mk_dag_node_task(dr_dag_node_freelist * fl) {
    dr_dag_node * t = dr_dag_node_alloc(fl);
    dr_dag_node_init_section_or_task(t, dr_dag_node_kind_task, 0);
    t->active_section = t;
    return t;
  }

#if __cplusplus 
  extern "C" {
#endif
    int sched_getcpu();
#if __cplusplus 
  }
#endif

  static int dr_getcpu() {
    if (DAG_RECORDER_RECORD_CPU) {
      return sched_getcpu();
    } else {
      return 0;
    }
  }

  static inline dr_clock_t
  dr_max_clock(dr_clock_t x, dr_clock_t y) {
    return (x < y ? y : x);
  }

  static inline dr_clock_t
  dr_min_clock(dr_clock_t x, dr_clock_t y) {
    return (x < y ? x : y);
  }

  static inline long long //dr_clock_t
  dr_max_count(long long x, long long y) {
    return (x < y ? y : x);
  }

  static inline int
  dr_meet_ints(int x, int y) {
    return (x == y ? x : -1);
  }

  static const char * 
  dr_dag_node_kind_to_str(dr_dag_node_kind_t nk) {
    switch (nk) {
    case dr_dag_node_kind_create_task: return "create_task";
    case dr_dag_node_kind_wait_tasks:  return "wait_tasks";
    case dr_dag_node_kind_other:       return "other";
    case dr_dag_node_kind_end_task:    return "end_task";
    case dr_dag_node_kind_section:     return "section";
    case dr_dag_node_kind_task:        return "task";
    default : (void)dr_check(0);
    }
    return (const char *)0;
  }

  static const char * 
  dr_dag_edge_kind_to_str(dr_dag_edge_kind_t ek) {
    switch (ek) {
    case dr_dag_edge_kind_end:
      return "end";
    case dr_dag_edge_kind_create:
      return "create";
    case dr_dag_edge_kind_create_cont:
      return "create_cont";
    case dr_dag_edge_kind_wait_cont:
      return "wait_cont";
    case dr_dag_edge_kind_other_cont:
      return "other_cont";
    default:
      (void)dr_check(0);
    }
    return 0;
  }

  /* end an interval, 
     called by start_{task_group,create_task,wait_tasks} */
  static void 
  dr_end_interval_(dr_dag_node * dn, int worker, 
		   dr_dag_node_kind_t kind,
		   dr_dag_edge_kind_t edge_kind,
		   /* time at which the interval ended */
		   dr_clock_t end_t, 
		   /* est of this inerval */
		   dr_clock_t est,
		   /* time at which the interval became ready */
		   dr_clock_t ready_t,
		   const char * file, int line,
		   dr_clock_pos start
		   ) {
    int k, ek;
    (void)dr_check(kind < dr_dag_node_kind_section);
    dn->info.start = start;
    dn->info.kind = kind;
    dn->info.est = est;
    dn->info.n_child_create_tasks = 0;
    dn->info.t_inf = dn->info.t_1 = end_t - start.t;
    dn->info.end.t = end_t;
    dn->info.end.pos.file = file;
    dn->info.end.pos.line = line;
    for (k = 0; k < dr_dag_node_kind_section; k++) {
      dn->info.logical_node_counts[k] = 0;
    }
    dn->info.logical_node_counts[kind] = 1;
    for (ek = 0; ek < dr_dag_edge_kind_max; ek++) {
      dn->info.logical_edge_counts[ek] = 0;
    }
    dn->info.first_ready_t = ready_t;
    dn->info.last_start_t = start.t;
    /* the time this node was ready */
    for (ek = 0; ek < dr_dag_edge_kind_max; ek++) {
      dn->info.t_ready[ek] = 0;
    }
    if (DAG_RECORDER_VERBOSE_LEVEL>=3) {
      printf(" [%p].t_ready[%s] = %llu\n", 
	     dn, dr_dag_edge_kind_to_str((dr_dag_edge_kind_t)edge_kind), 
	     start.t - ready_t);
    }
    dn->info.t_ready[edge_kind] = start.t - ready_t;
    dn->info.in_edge_kind = edge_kind;
    dn->info.cur_node_count = 1;
    dn->info.min_node_count = 1;
    dn->info.end.worker = worker;
    dn->info.end.cpu    = dr_getcpu();
    /* by construction, the worker should not change */
    (void)dr_check(dn->info.start.worker == dn->info.end.worker);
    dn->info.worker = worker;
    /* OS may migrate workers */
    dn->info.cpu = dr_meet_ints(dn->info.start.cpu, dn->info.end.cpu);
  }

  /* auxiliary functions that modify or query task and section */

  /* 

     return the current (unfinished) section of section s.  in other
     words, it returns the section to which a new element should added
     when the program calls create_task or task_group next time.

     section ::= task_group (section|create)* wait 
  */


  /* it returns a dag node to which a new interval 
     should be added when the program performs an action
     that needs one.
     starting from t, it descends the rightmost child,
     until it finds that a node's rightmost child is
     not an unfinished section.
  */

  static dr_dag_node * 
  dr_task_active_node(dr_dag_node * t) {
    return t->active_section;
  }

  /* get the task's last node. it is
     (i) a section node, if the task has just returned from wait_tasks
     (ii) a create_task node, if the task has just returned from create_task
     (iii) a other node, if the task has just returned from other */
  static dr_dag_node * 
  dr_task_last_node(dr_dag_node * t) {
    if (dr_check(t->info.kind == dr_dag_node_kind_task)) {
      dr_dag_node * s = dr_task_active_node(t);
      dr_dag_node * i = dr_dag_node_list_last(s->subgraphs);
      (void)dr_check(i->info.kind == dr_dag_node_kind_section
		     || i->info.kind == dr_dag_node_kind_create_task
		     || i->info.kind == dr_dag_node_kind_other);
      return i;
    } else {
      return 0;
    }
  }

  /* ensure that the task node t has a an open section.
     it does not when a task has just been created
     or has just finished wait_tasks. it is called when
     a task performs an action in this state */
  static dr_dag_node * 
  dr_task_ensure_section(dr_dag_node * t, 
			 dr_dag_node_freelist * fl) {
    dr_dag_node * s = dr_task_active_node(t);
    if (s == t) {
      (void)dr_check(s->info.kind == dr_dag_node_kind_task);
      s = dr_push_back_section(t, s, fl);
    }
    (void)dr_check(s->info.kind == dr_dag_node_kind_section);
    return s;
  }

  /* ------------- main instrumentation functions ------------- 
     start_task
     start_task_group
     end_task_group
     start_create_task
     end_create_task
     start_wait_tasks
     end_wait_tasks
     end_task
  */


  /* called when we start a task */

  static void
  dr_set_start_info(dr_clock_pos * p, 
		    int worker, const char * file, int line) {
    p->pos.file = file;
    p->pos.line = line;
    p->t = dr_get_tsc();
    p->worker = worker;
    p->cpu = dr_getcpu();
  }

  /* 
     task    ::= section* end 

     called when a task just started
  */

  static_if_inline void 
  dr_start_task__(dr_dag_node * p, 
		  const char * file, int line, int worker) {
    if (GS.generation % 2) {
      /* make a task, section, and interval */
      /* create a task node */
      dr_worker_specific_state * wss 
	= dr_get_worker_specific_state(worker);
      dr_dag_node * nt = dr_mk_dag_node_task(wss->freelist);
      if (DAG_RECORDER_VERBOSE_LEVEL>=3) {
	printf("dr_start_task(parent=%p) by %d new task=%p\n", 
	       p, wss->worker, nt);
      }
      if (p) {
	/* we have a parent p.
	   register this task as the child of p */
	(void)dr_check(p->info.kind == dr_dag_node_kind_create_task);
	(void)dr_check(p->child == 0);
	p->child = nt;
	nt->info.est = p->info.est + p->info.t_inf;
	nt->info.first_ready_t = p->info.end.t; /* ??? */
      } else {
	/* no parent. this is the root task */
	nt->info.est = 0;
	nt->info.first_ready_t = GS.start_clock; /* ???? */
      }
      (void)dr_check(nt->info.first_ready_t > 0);
      nt->info.in_edge_kind = dr_dag_edge_kind_create;
      /* set current task to the node just created */
      dr_set_cur_task_(wss, nt);
      /* call hook */
      if (GS.opts.hooks.start_task) {
	GS.opts.hooks.start_task(nt);
      }
      /* record info on the point of start */
      dr_set_start_info(&nt->info.start, wss->worker, file, line);
    }
  }
  
  /* in MIT-Cilk, the user needs to insert this
     at the beginning of a cilk function. 
     on other systems, a call to dr_start_task__ 
     is inserted by the system */
  static_if_inline int 
  dr_start_cilk_proc__(const char * file, int line, int worker) {
    if (GS.generation % 2) {
      dr_worker_specific_state * wss 
	= dr_get_worker_specific_state(worker);
      dr_start_task__(wss->parent, file, line, worker);
    }
    return 0;
  }

  /* begin a new section */
  static_if_inline void
  dr_begin_section__(int worker) {
    if (GS.generation % 2) {
      dr_worker_specific_state * wss 
	= dr_get_worker_specific_state(worker);
      dr_dag_node * t = dr_get_cur_task_(wss);
      /* the current section of this task */
      dr_dag_node * s = dr_task_active_node(t);
      /* push a new section as a child of the current section */
      dr_dag_node * new_s = dr_push_back_section(t, s, wss->freelist);
      /* call hook */
      if (GS.opts.hooks.begin_section) {
	GS.opts.hooks.begin_section(new_s);
      }
      if (DAG_RECORDER_VERBOSE_LEVEL>=3) {
	printf("dr_begin_section() by %d task=%p,"
	       " parent section=%p, new section = %p\n", 
	       wss->worker, t, s, new_s);
      }
    }
  }


  /*
    called right before a program creates a task.
    end the current node and push the node into
    the graph.
  */
  static_if_inline dr_dag_node * 
  dr_enter_create_task__(dr_dag_node ** c, 
			 const char * file, int line, int worker) {
    if (GS.generation % 2) {
      dr_worker_specific_state * wss 
	= dr_get_worker_specific_state(worker);
      /* get time stamp */
      dr_clock_t end_t = dr_get_tsc();
      /* the current task of this worker */
      dr_dag_node * t = dr_get_cur_task_(wss);
      /* ensure t has a session */
      dr_dag_node * s = dr_task_ensure_section(t, wss->freelist);
      /* add a new node as a child of s */
      dr_dag_node * ct 
	= dr_dag_node_list_push_back(s->subgraphs, wss->freelist);
      /* set info about the new node */
      ct->info.kind = dr_dag_node_kind_create_task;
      /* child is set when the child gets started */
      ct->child = 0;
      // dr_dag_node * ct = dr_task_add_create(t);
      if (DAG_RECORDER_VERBOSE_LEVEL>=3) {
	printf("dr_enter_create_task() by %d task=%p, section=%p, new interval=%p\n", 
	       wss->worker, t, s, ct);
      }
      /* put various info of ct */
      dr_end_interval_(ct, wss->worker, 
		       dr_dag_node_kind_create_task,
		       t->info.in_edge_kind, 
		       end_t, t->info.est, 
		       t->info.first_ready_t, 
		       file, line, t->info.start);
      /* tell the caller the node just created */
      *c = ct;
      /* call hook */
      if (GS.opts.hooks.enter_create_task) {
	GS.opts.hooks.enter_create_task(ct);
      }
      return t;
    } else {
      return (dr_dag_node *)0;
    }
  }

  /* 
     in cilk, this must be called by the user program
     when it spawns a cilk procedure.
     in other systems, create_task macros handle that for you.
   */
  static_if_inline dr_dag_node *
  dr_enter_create_cilk_proc_task__(const char * file, int line, int worker) {
    if (GS.generation % 2) {
      dr_worker_specific_state * wss 
	= dr_get_worker_specific_state(worker);
      return dr_enter_create_task__(&wss->parent, file, line, worker);
    } else {
      return (dr_dag_node *)0;
    }
  }

  /* 
     called when it returns from create_task
  */
  static_if_inline void 
  dr_return_from_create_task__(dr_dag_node * t, 
			       const char * file, int line, int worker) {
    if (GS.generation % 2) {
      /* get the node that called create_task
	 from which we have just returned.
	 it must be the last node of the current
	 section */
      dr_worker_specific_state * wss 
	= dr_get_worker_specific_state(worker);
      dr_dag_node * ct = dr_task_last_node(t);
      if (DAG_RECORDER_VERBOSE_LEVEL>=3) {
	printf("dr_return_from_create_task(task=%p) by %d interval=%p\n", 
	       t, wss->worker, ct);
      }
      (void)dr_check(ct->info.kind == dr_dag_node_kind_create_task);
      /* set this worker's current task */
      dr_set_cur_task_(wss, t);
      /* this node's est is the preceeding node's est
	 + its critical path len */
      t->info.est = ct->info.est + ct->info.t_inf;
      /* this node became ready when the preceeding
	 node finished */
      t->info.first_ready_t = ct->info.end.t;
      (void)dr_check(t->info.first_ready_t > 0);
      /* its incoming edge must be create_cont type */
      t->info.in_edge_kind = dr_dag_edge_kind_create_cont;
      /* call hook */
      if (GS.opts.hooks.return_from_create_task) {
	GS.opts.hooks.return_from_create_task(t);
      }
      /* record an interval just started */
      dr_set_start_info(&t->info.start, wss->worker, file, line);
    }
  }

  /* 
     called when a program calls wait_tasks.
  */
  static_if_inline dr_dag_node *
  dr_enter_wait_tasks__(const char * file, int line, int worker) {
    if (GS.generation % 2) {
      dr_clock_t end_t = dr_get_tsc();
      /* the current task of this worker */
      dr_worker_specific_state * wss 
	= dr_get_worker_specific_state(worker);
      dr_dag_node * t = dr_get_cur_task_(wss);
      /* it may be the first node of this task, in
	 which case we ensure to have a section */
      dr_dag_node * s = dr_task_ensure_section(t, wss->freelist);
      /* create and append a new node, which becomes
	 an wait_task node */
      dr_dag_node * i 
	= dr_dag_node_list_push_back(s->subgraphs, wss->freelist);
      if (DAG_RECORDER_VERBOSE_LEVEL>=3) {
	printf("dr_enter_wait_tasks() by %d task=%p, "
	       "section=%p, new interval=%p\n", 
	       wss->worker, t, s, i);
      }
      t->active_section = s->parent_section;
      (void)dr_check(dr_task_active_node(t) == t->active_section);
      /* record an interval has ended */
      dr_end_interval_(i, wss->worker, 
		       dr_dag_node_kind_wait_tasks,
		       t->info.in_edge_kind,
		       end_t, t->info.est, 
		       t->info.first_ready_t, 
		       file, line, 
		       t->info.start);
      /* call hook */
      if (GS.opts.hooks.enter_wait_tasks) {
	GS.opts.hooks.enter_wait_tasks(i);
      }
      return t;
    } else {
      return (dr_dag_node *)0;
    }
  }

  /* accumulate results from s's subgraphs into s */
  static void 
  dr_accumulate_stats(dr_dag_node * s) {
    if (DAG_RECORDER_VERBOSE_LEVEL>=3) {
      printf("dr_accumulate_stats(%p)\n", s);
    }

    {
      dr_dag_node * first = dr_dag_node_list_first(s->subgraphs);
      dr_dag_node * last = dr_dag_node_list_last(s->subgraphs);
      int i;
      /* initialize the result */
      /* s's start time is it's first nodes' start time */
      s->info.start   = first->info.start;
      /* s's end time is it's last nodes' end time */
      s->info.end     = last->info.end;      
      /* s's worker is temporarily first node's worker.
         later we may set it to -1 when other nodes' workers
         are different */
      s->info.worker  = first->info.worker;
      /* similar to worker */
      s->info.cpu     = first->info.cpu;
      /* s's earliest start time is its first node's 
         earliest start time */
      s->info.est     = first->info.est;
      /* s's ready time is its first node's
         ready time */
      s->info.in_edge_kind = first->info.in_edge_kind;
      s->info.first_ready_t = first->info.first_ready_t;
      s->info.last_start_t = last->info.start.t;
      (void)dr_check(s->info.first_ready_t > 0);
      
      s->info.t_1     = 0;
      s->info.t_inf   = 0;
      int j;
      for (j = 0; j < dr_max_counters; j++) {
        s->info.counters_1[j] = s->info.counters_inf[j] = 0;
      }
      /* accumulate ready times */
      for (i = 0; i < dr_dag_edge_kind_max; i++) {
        s->info.t_ready[i] = 0;
      }
      /* initialize node/edge counts (we later accumulate 
         subgraphs's results into them) */
      for (i = 0; i < dr_dag_node_kind_section; i++) {
        s->info.logical_node_counts[i] = 0;
      }
      s->info.logical_node_counts[s->info.kind] = 1;
      for (i = 0; i < dr_dag_edge_kind_max; i++) {
        s->info.logical_edge_counts[i] = 0;
      }
      s->info.cur_node_count = 1;
      s->info.min_node_count = 1;
      s->info.n_child_create_tasks = 0;
      
      {
        /* look through all subgraphs */
        dr_clock_t t_inf = 0;
        long long counters_inf[dr_max_counters];
        for (j = 0; j < dr_max_counters; j++) {
          counters_inf[j] = 0;
        }
        dr_dag_node * head = s->subgraphs->head;
        dr_dag_node * ch;
        for (ch = head; ch; ch = ch->next) {
          dr_dag_node * x = ch;
          int k;
          /* accumulate t_1 */
          s->info.t_1     += x->info.t_1;
          /* accumulate t_inf along the sequential chain */
          s->info.t_inf   += x->info.t_inf;
          for (j = 0; j < dr_max_counters; j++) {
            s->info.counters_1[j] += x->info.counters_1[j];
            s->info.counters_inf[j] += x->info.counters_inf[j];
          }
          
          /* accumulate ready along the sequential chain */
          for (k = 0; k < dr_dag_edge_kind_max; k++) {
            if (DAG_RECORDER_VERBOSE_LEVEL>=3) {
              printf(" [%p].t_ready[%s] (%llu) += [%p].t_ready[%s] (%llu) -> %llu\n", 
                     s, dr_dag_edge_kind_to_str((dr_dag_edge_kind_t)k), 
                     s->info.t_ready[k], 
                     x, dr_dag_edge_kind_to_str((dr_dag_edge_kind_t)k), 
                     x->info.t_ready[k], 
                     s->info.t_ready[k] + x->info.t_ready[k]);
            }
            s->info.t_ready[k] += x->info.t_ready[k];
          }
          
          /* meet workers and cpus */
          s->info.worker = dr_meet_ints(s->info.worker, x->info.worker);
          s->info.cpu    = dr_meet_ints(s->info.cpu, x->info.cpu);
          /* accumulate node counts of each type */
          for (k = 0; k < dr_dag_node_kind_section; k++) {
            s->info.logical_node_counts[k] += x->info.logical_node_counts[k];
          }
          /* accumulate edge counts of each type */
          for (k = 0; k < dr_dag_edge_kind_max; k++) {
            s->info.logical_edge_counts[k] += x->info.logical_edge_counts[k];
          }
          s->info.cur_node_count += x->info.cur_node_count;
          /* at this point, we assume s is not collapsable,
             so its minimum number of nodes is the sum of its children */
          s->info.min_node_count += x->info.min_node_count;
          switch (x->info.kind) {
          case dr_dag_node_kind_create_task: {
            /* besides, we need to count edges
               from x to its successor */
            dr_dag_node * c = x->child;
            (void)dr_check(x->next);
            s->info.logical_edge_counts[dr_dag_edge_kind_create]++;
            s->info.logical_edge_counts[dr_dag_edge_kind_create_cont]++;
            s->info.n_child_create_tasks++;
            /* similar accumulation for x's child task */
            (void)dr_check(c);
            /* s is a section; s's last node may
               have finished eariler than one of its
               children */
            s->info.end.t = dr_max_clock(c->info.end.t, s->info.end.t);
            /* ******************************** */
            s->info.last_start_t 
              = dr_max_clock(c->info.last_start_t, s->info.last_start_t);
            s->info.t_1     += c->info.t_1;
            t_inf = dr_max_clock(s->info.t_inf + c->info.t_inf, t_inf);
            for (j = 0; j < dr_max_counters; j++) {
              s->info.counters_1[j] += c->info.counters_1[j];
              counters_inf[j] = dr_max_count(counters_inf[j], s->info.counters_inf[j] + c->info.counters_inf[j]);
            }
            
            /* count "ready" tasks.  the task c is
               "ready" from the point it is created
               (x->info.end.t) to the point it is
               finished c->info.end.t */
            for (k = 0; k < dr_dag_edge_kind_max; k++) {
              if (DAG_RECORDER_VERBOSE_LEVEL>=3) {
                printf("  [%p].t_ready[%s] (%llu)"
                       " += [%p].t_ready[%s] (%llu) -> %llu\n", 
                       s, dr_dag_edge_kind_to_str((dr_dag_edge_kind_t)k), 
                       s->info.t_ready[k], 
                       c, dr_dag_edge_kind_to_str((dr_dag_edge_kind_t)k), 
                       c->info.t_ready[k],
                       s->info.t_ready[k] + c->info.t_ready[k]);
              }
              s->info.t_ready[k] += c->info.t_ready[k];
            }
            
            s->info.worker = dr_meet_ints(s->info.worker, c->info.worker);
            s->info.cpu    = dr_meet_ints(s->info.cpu, c->info.cpu);
            /* if a section contains a create task node,
               count its children as well */
            for (k = 0; k < dr_dag_node_kind_section; k++) {
              s->info.logical_node_counts[k] += c->info.logical_node_counts[k];
            }
            for (k = 0; k < dr_dag_edge_kind_max; k++) {
              s->info.logical_edge_counts[k] += c->info.logical_edge_counts[k];
            }
            s->info.cur_node_count += c->info.cur_node_count;
            s->info.min_node_count += c->info.min_node_count;
            break;
          }
          case dr_dag_node_kind_wait_tasks: 
          case dr_dag_node_kind_end_task: {
            (void)dr_check(!x->next);
            break;
          }
          case dr_dag_node_kind_other: 
            break;
          case dr_dag_node_kind_section:
            if (x->next) {
              s->info.logical_edge_counts[dr_dag_edge_kind_wait_cont]++;
              s->info.logical_edge_counts[dr_dag_edge_kind_end] 
                += x->info.n_child_create_tasks;
            }
            break;
          default:
            (void)dr_check(0);
            break;
          }
        }
        s->info.t_inf = dr_max_clock(t_inf, s->info.t_inf);
        for (j = 0; j < dr_max_counters; j++) {
          s->info.counters_inf[j] = dr_max_count(counters_inf[j], s->info.counters_inf[j]);
        }
      }
      /* turned out we can collapse this node. */
      if (s->info.worker != -1) s->info.min_node_count = 1;
    }
  }

/* --- free all descendants of g (and optionally g also) --- */

  void 
  dr_free_dag(dr_dag_node * g, int free_root,
	      dr_dag_node_freelist * fl);

  static void 
  dr_collapse_subgraph(dr_dag_node * s, 
		       dr_dag_node_freelist * fl) {
    (void)dr_check(s->info.kind >= dr_dag_node_kind_section);
    /* free s's descendants, but not s */
    dr_free_dag(s, 0, fl);
    s->info.cur_node_count = 1;
  }

  /* sum of all logical node counts */
  static long dr_get_logical_node_counts(dr_dag_node * s) {
    int k;
    long c = 0;
    for (k = 0; k < dr_dag_node_kind_section; k++) {
      c += s->info.logical_node_counts[k];
    }
    return c;
  }

  /* node count of s and its descendants */
  static long dr_cur_nodes_below(dr_dag_node * s) {
    if (s->info.kind == dr_dag_node_kind_create_task) {
      /* create_task node's cur_node_count does NOT
	 include the task's it created */
      (void)dr_check(s->info.cur_node_count == 1);
      return 1 + s->child->info.cur_node_count;
    } else {
      return s->info.cur_node_count;
    }
  }

  /* node count of s and its descendants */
  static long dr_min_nodes_below(dr_dag_node * s) {
    if (s->info.kind == dr_dag_node_kind_create_task) {
      (void)dr_check(s->info.min_node_count == 1);
      /* create_task node's min_node_count does NOT
	 include the task's it created */
      return 1 + s->child->info.min_node_count;
    } else {
      return s->info.min_node_count;
    }
  }

  /* check if actual_node_count field is correct,
     for n and all descendants of n 
     (for debugging) */
  static long
  dr_check_cur_node_count(dr_dag_node * n) {
    switch (n->info.kind) {
    case dr_dag_node_kind_create_task:
      (void)dr_check(n->info.cur_node_count == 1);
      return 1 + dr_check_cur_node_count(n->child);
    case dr_dag_node_kind_wait_tasks:
    case dr_dag_node_kind_other:
    case dr_dag_node_kind_end_task:
      (void)dr_check(n->info.cur_node_count == 1);
      return 1;
    case dr_dag_node_kind_section:
    case dr_dag_node_kind_task: {
      dr_dag_node * ch;
      long x = 1;
      for (ch = n->subgraphs->head; ch; ch = ch->next) {
	x += dr_check_cur_node_count(ch);
      }
      (void)dr_check(n->info.cur_node_count == x);
      return x;
    }
    default:
      (void)dr_check(0);
      return 0;
    }
  }

  /* check if min_node_count field is correct,
     for n and all descendants of n */
  static long
  dr_check_min_node_count(dr_dag_node * n) {
    switch (n->info.kind) {
    case dr_dag_node_kind_create_task:
      (void)dr_check(n->info.min_node_count == 1);
      return 1 + dr_check_min_node_count(n->child);
    case dr_dag_node_kind_wait_tasks:
    case dr_dag_node_kind_other:
    case dr_dag_node_kind_end_task:
      (void)dr_check(n->info.min_node_count == 1);
      return 1;
    case dr_dag_node_kind_section:
    case dr_dag_node_kind_task: {
      dr_dag_node * ch;
      long x = 1;
      for (ch = n->subgraphs->head; ch; ch = ch->next) {
	x += dr_check_min_node_count(ch);
      }
      if (n->info.worker == -1) {
	(void)dr_check(n->info.min_node_count == x);
	return x;
      } else {
	(void)dr_check(n->info.min_node_count == 1);
	return 1;
      }
    }
    default:
      (void)dr_check(0);
      return 0;
    }
  }

  /* check both min_node_count and actual_node_count */
  static long
  dr_check_node_counts(dr_dag_node * n) {
    (void)dr_check_min_node_count(n);
    return dr_check_cur_node_count(n);
  }

  static long
  dr_prune_nodes(dr_prune_nodes_stack * S,
		 dr_dag_node * s, long budget,
		 dr_dag_node_freelist * fl, 
		 int indent);

  static void spaces(int indent) {
    int i;
    for (i = 0; i < indent; i++) putchar(' ');
  }

  /* prune_stack manipulation functions
     prune_stack is used to collapse a subgraph,
     without recursive calls (which may overflow
     stack) */


  /* empty stack */
  static int
  dr_prune_nodes_stack_empty(dr_prune_nodes_stack * S) {
    return S->n == 0;
  }

  /* top element of S */
  static dr_prune_nodes_stack_ent *
  dr_prune_nodes_stack_top(dr_prune_nodes_stack * S) {
    (void)dr_check(S->n > 0);
    return &S->entries[S->n - 1];
  }

  /* pop top element of S */
  static dr_prune_nodes_stack_ent *
  dr_prune_nodes_stack_pop(dr_prune_nodes_stack * S,
			   dr_prune_nodes_stack_ent * e) {
    dr_prune_nodes_stack_ent * f = &S->entries[S->n - 1];
    (void)dr_check(S->n > 0);
    (void)dr_check(e == f);
    S->n--;
    return f;
  }

  /* ensure S has at least x elements */
  static void 
  dr_prune_nodes_stack_ensure(dr_prune_nodes_stack * S, 
			      long x) {
    if (S->sz < x) {
      if (DAG_RECORDER_VERBOSE_LEVEL>=3) {
	printf("dr_prune_nodes_stack_ensure(%ld)\n", x);
      }      
      {
	long new_sz = S->sz * 2;
	dr_prune_nodes_stack_ent * new_entries;
	if (x > new_sz) new_sz = x;
	new_entries = (dr_prune_nodes_stack_ent *)
	  dr_malloc(new_sz * sizeof(dr_prune_nodes_stack_ent));
	if (S->entries) {
	  memcpy(new_entries, S->entries, 
		 S->sz * sizeof(dr_prune_nodes_stack_ent));
	  dr_free(S->entries, 
		  S->sz * sizeof(dr_prune_nodes_stack_ent));
	}
	S->entries = new_entries;
	S->sz = new_sz;
      }
    }
  }

  /* push a new entry to S */
  static dr_prune_nodes_stack_ent *
  dr_prune_nodes_stack_push_ent(dr_prune_nodes_stack * S) {
    dr_prune_nodes_stack_ensure(S, S->n + 1);
    {
      dr_prune_nodes_stack_ent * e = &S->entries[S->n];
      S->n++;
      return e;
    }
  }

  /* push x to S.
     budget represents how much nodes we can afford
     for x and its descendants */
  static dr_prune_nodes_stack_ent *
  dr_prune_nodes_stack_push(dr_prune_nodes_stack * S,
			    dr_dag_node * x, 
			    long budget) {
    dr_prune_nodes_stack_ent * e 
      = dr_prune_nodes_stack_push_ent(S);
    e->x = x;
    e->budget = budget;
    e->budget_left = budget;
    e->nodes_left = x->info.cur_node_count;
    e->last_child = 0;
    e->visit_count = 0;
    return e;
  }

  static const char * dr_kind_to_str(dr_dag_node_kind_t k) {
    switch (k) {
    case dr_dag_node_kind_create_task: 
      return "create_task";
    case dr_dag_node_kind_wait_tasks: 
      return "wait_tasks";
    case dr_dag_node_kind_other: 
      return "other";
    case dr_dag_node_kind_end_task: 
      return "end_task";
    case dr_dag_node_kind_section: 
      return "section";
    case dr_dag_node_kind_task: 
      return "task";
    default:
      (void)dr_check(0);
    }
    (void)dr_check(0);
    return 0;
  }

  /* sum of min_nodes over nodes in l */
  static long dr_dag_node_list_min_nodes(dr_dag_node_list * l) {
    dr_dag_node * x;
    long n = 0;
    for (x = l->head; x; x = x->next) {
      n += dr_min_nodes_below(x);
    }
    return n;
  }

  /* prune some nodes under s, to make the number
     of nodes within budget */
  static long
  dr_prune_nodes_norec(dr_prune_nodes_stack * S,
		       dr_dag_node * s, long budget,
		       dr_dag_node_freelist * fl) {
    /* push the root node (s) to the stack,
       along with budget */
    dr_prune_nodes_stack_push(S, s, budget);
    /* repeat checking the top element and see if
       we can possibly have its children 
       to meet the budget */
    while (!dr_prune_nodes_stack_empty(S)) {
      dr_prune_nodes_stack_ent * e = dr_prune_nodes_stack_top(S);
      dr_dag_node * x = e->x;
      const char * pop = 0;
      if (DAG_RECORDER_VERBOSE_LEVEL>=3
	   && e->visit_count == 0) {
	  /* this is the first time e has been processed */
	  spaces(S->n);
	  printf("dr_prune_nodes(%p (%ld/%ld nodes, %s),"
		 " budget=%ld)\n", 
		 x, 
		 dr_cur_nodes_below(x), 
		 dr_min_nodes_below(x), 
		 dr_kind_to_str(x->info.kind), 
		 e->budget);
      }

      switch (x->info.kind) {
      case dr_dag_node_kind_wait_tasks:	
      case dr_dag_node_kind_other:
      case dr_dag_node_kind_end_task: {
	(void)dr_check(x->info.cur_node_count == 1);
	/* leaf. done. pop it */
	pop = "leaf";
	break;
      }
      case dr_dag_node_kind_create_task: {
	dr_dag_node * child = x->child;
	if (e->visit_count == 0) {
	  /* the first time this entry is popped,
	     push child (e is still in the stack)
	     with budget = budget - 1 */
	  e->visit_count++;
	  dr_prune_nodes_stack_push(S, child, e->budget - 1);
	} else {
	  pop = "create";
	}
	break;
      }
      case dr_dag_node_kind_section:
      case dr_dag_node_kind_task: 
	{
	  if (e->visit_count == 0) {
	    long nc_cur = dr_cur_nodes_below(x);
	    long nc_min = dr_min_nodes_below(x);
	    if (nc_cur <= e->budget) {
	      /* it's already within budget. done. 
		 we can have the entire descendants.
		 pop it */
	      pop = "within budget";
	    } else if (nc_min >= nc_cur) {
	      (void)dr_check(nc_min == nc_cur);
	      /* s has been maximally collapsed. 
		 we may overrun the budget, but
		 nothing we can do. done */
	      pop = "already minimum";
	    } else {
	      long min_nodes_children 
		= dr_dag_node_list_min_nodes(x->subgraphs);
	      if (e->budget < min_nodes_children + 1 
		  && x->info.min_node_count == 1) {
		/* the budget overflows unless we collapse s
		   (even if all children become single nodes) */
		/* TODO: consider how to collapse a subgraph involving
		   multiple workers */
		dr_collapse_subgraph(x, fl);
		pop = "just collapsed";
	      } else {
		/* start recursing on the children */
		e->last_child = 0;
		e->visit_count++;
		e->nodes_left--;	/* count x itself */
		e->budget_left--;	/* count x itself */
	      }
	    }
	  } else {
	    /* this is not the first time we see
	       this entry on top of the stack. that is,
	       we are in the middle of processing children */
	    dr_dag_node * ch;
	    if (e->last_child) {
	      e->budget_left -= dr_cur_nodes_below(e->last_child);
	    }
	    ch = (e->last_child ? e->last_child->next : x->subgraphs->head);
	    if (!ch) {
	      x->info.cur_node_count = e->budget - e->budget_left;
	      pop = "done recursions";
	    } else {
	      /* nodes for this child, when no collaption occurs */
	      long nodes_ch = dr_cur_nodes_below(ch);
	      long b;
	      /* budget for this child (a fair proportion of the 
		 remaining budget) */
	      (void)dr_check(e->nodes_left > 0);
	      b = e->budget_left * nodes_ch / e->nodes_left;
	      e->nodes_left -= nodes_ch;
	      e->last_child = ch;
	      dr_prune_nodes_stack_push(S, ch, b);
	    }
	  }
	}
	break;
      default:
	(void)dr_check(0);
      }

      if (pop) {
	/* we are done with this node. pop it. */
	if (DAG_RECORDER_VERBOSE_LEVEL>=3) {
	  long c = dr_cur_nodes_below(x);
	  spaces(S->n);
	  printf("--> %ld (%s)\n", c, pop);
	}
	(void)dr_check(dr_cur_nodes_below(x) == dr_check_node_counts(x));
	dr_prune_nodes_stack_pop(S, e);
      }
    }
    //dr_prune_nodes_stack_destroy(S);
    (void)dr_check(dr_cur_nodes_below(s) == dr_check_node_counts(s));
    return dr_cur_nodes_below(s);
  }

  /* try to collapse some nodes under S,
     so as to make the number of nodes for 
     S and its descendants under budget */
  static long
  dr_prune_nodes(dr_prune_nodes_stack * S,
		 dr_dag_node * s, long budget,
		 dr_dag_node_freelist * fl,
		 int indent) {
    dr_clock_t t0 = 0, t1 = 0;
    if (DAG_RECORDER_VERBOSE_LEVEL>=2) {
      spaces(indent);
      printf("dr_prune_nodes(%p (%ld/%ld nodes, kind=%s), budget=%ld)\n", 
	     s, 
	     dr_cur_nodes_below(s), 
	     dr_min_nodes_below(s), 
	     dr_kind_to_str(s->info.kind), budget);
      t0 = dr_get_tsc();
    }
    {
      long r = dr_prune_nodes_norec(S, s, budget, fl);
      if (DAG_RECORDER_VERBOSE_LEVEL>=2) {
	t1 = dr_get_tsc();
	printf("--> %ld (%llu clocks)\n", r, t1 - t0);
      }
      return r;
    }
  }

  /* we have just finished a section or task.
     see if we have to collapse it (or a part thereof) */
  static void 
  dr_summarize_section_or_task(dr_prune_nodes_stack * S,
			       dr_dag_node * s, 
			       dr_dag_node_freelist * fl) {
    if (DAG_RECORDER_VERBOSE_LEVEL>=2) {
      if (s->info.kind == dr_dag_node_kind_section) {
	printf("dr_summarize_section(section=%p)\n", s);
      } else if (s->info.kind == dr_dag_node_kind_task) {
	printf("dr_summarize_task(task=%p)\n", s);
      }
    }
    (void)dr_check(s->info.kind >= dr_dag_node_kind_section);
    (void)dr_check(!dr_dag_node_list_empty(s->subgraphs));
    /* accumulate t_1, t_inf, number of nodes, etc. */
    dr_accumulate_stats(s);
    (void)dr_check(dr_cur_nodes_below(s) == dr_check_node_counts(s));
    /* now check if we should collapse it */
    if (GS.opts.node_count_target) {
      /* a collapse method (I).
	 target a particular number of nodes.
	 when node_count_target is given,
	 make S withing the target.
	 (this is expensive. not recommended) */
      if (s->info.cur_node_count > GS.opts.prune_threshold) {
	dr_prune_nodes(S, s, GS.opts.node_count_target, fl, 0);
	(void)dr_check(dr_cur_nodes_below(s) == dr_check_node_counts(s));
      }
    } else if (GS.opts.collapse_max_count) { 
      /* collapse method (II)
	 node count based */
      if (dr_get_logical_node_counts(s) < GS.opts.collapse_max_count) {
	dr_collapse_subgraph(s, fl);
	(void)dr_check(dr_cur_nodes_below(s) == dr_check_node_counts(s));
      }
    } else if (s->info.end.t - s->info.start.t 
	       < GS.opts.uncollapse_min
	       || (s->info.worker != -1
		   && s->info.end.t - s->info.start.t 
		   < GS.opts.collapse_max)) {
      /* we collapse a subgraph when 
	 (i) its span is too short (< GS.opts.uncollapse_min), or
	 (ii) its span is not too long and it is worked on by a single worker
      */
      dr_collapse_subgraph(s, fl);
      (void)dr_check(dr_cur_nodes_below(s) == dr_check_node_counts(s));
    }
  }

  
  /* 
     called when we return from wait_tasks 
  */
  static_if_inline void 
  dr_return_from_wait_tasks__(dr_dag_node * t, 
			      const char * file, int line, int worker) {
    if (GS.generation % 2) {
      dr_worker_specific_state * wss 
	= dr_get_worker_specific_state(worker);
      /* get the section that has just finished */
      dr_dag_node * s = dr_task_last_node(t);
         dr_dag_node * p = dr_dag_node_list_last(s->subgraphs);
      /* calc EST and READY_T of the interval to start */
      /* the starting node's est is that of last one
	 + its critical path len */
      dr_clock_t est = p->info.est + p->info.t_inf;
      /* the starting node became ready when all its
	 predecessors ended. we later take max among
	 all child tasks that are waited */
      dr_clock_t ready_t = p->info.end.t;
      dr_dag_edge_kind_t edge_kind = dr_dag_edge_kind_wait_cont;
      dr_dag_node * head = s->subgraphs->head;
      dr_dag_node * ch;
      if (DAG_RECORDER_VERBOSE_LEVEL>=3) {
	printf("dr_return_from_wait_tasks(task=%p)"
	       " by %d section=%p, pred=%p\n", 
	       t, wss->worker, s, p);
      }
      (void)dr_check(s->info.kind == dr_dag_node_kind_section);
      (void)dr_check(p->info.kind == dr_dag_node_kind_wait_tasks);
      (void)dr_check(ready_t > 0);
      /* examine all child tasks that are waited by 
	 this wait_tasks */
      for (ch = head; ch; ch = ch->next) {
	dr_dag_node * sc = ch;
	(void)dr_check(sc->info.kind < dr_dag_node_kind_task); 
	if (sc->info.kind == dr_dag_node_kind_create_task) {
	  /* if the node is create_task, take the created
	     task (child) and see its end time */
	  dr_dag_node * ct = sc->child;
	  if (dr_check(ct)) {
	    dr_clock_t x = ct->info.est + ct->info.t_inf;
	    if (est < x) est = x;
	    if (ready_t < ct->info.end.t) {
	      /* ct finished after other predecessors */
	      ready_t = ct->info.end.t;
	      edge_kind = dr_dag_edge_kind_end;
	    }
	  }
	}
      }
      
      /* we may collapse it */
      dr_summarize_section_or_task(wss->prune_stack, s,
				   wss->freelist);
      /* set the current task of this worker */
      dr_set_cur_task_(wss, t);
      t->info.est = est;
      /* this node becomes ready when all of its predecessors finished */
      t->info.first_ready_t = ready_t;
      t->info.in_edge_kind = edge_kind;
      (void)dr_check(ready_t > 0);
      /* record start time etc. */
      dr_set_start_info(&t->info.start, wss->worker, file, line);
      
      /* call hook */
      if (GS.opts.hooks.return_from_wait_tasks) {
	GS.opts.hooks.return_from_wait_tasks(t);
      }
    }
  }
  
  /* enter a runtime for any reason. 
     the worker may change when we return from it */
  static_if_inline dr_dag_node * 
  dr_enter_other__(const char * file, int line, int worker) {
    if (GS.generation % 2) {
      dr_worker_specific_state * wss 
	= dr_get_worker_specific_state(worker);
      /* get time stamp */
      dr_clock_t end_t = dr_get_tsc();
      /* the current task of this worker */
      dr_dag_node * t = dr_get_cur_task_(wss);
      /* ensure t has a session */
      dr_dag_node * s = dr_task_active_node(t);
      /* add a new node as a child of s */
      dr_dag_node * i
	= dr_dag_node_list_push_back(s->subgraphs, wss->freelist);
      if (DAG_RECORDER_VERBOSE_LEVEL>=3) {
	printf("dr_enter_other() by %d task=%p, section=%p, new interval=%p\n", 
	       wss->worker, t, s, i);
      }
      //t->active_section = s->parent_section;
      dr_end_interval_(i, wss->worker, 
		       dr_dag_node_kind_other,
		       t->info.in_edge_kind, 
		       end_t, t->info.est, 
		       t->info.first_ready_t, 
		       file, line, t->info.start);
      /* call hook */
      if (GS.opts.hooks.enter_other) {
	GS.opts.hooks.enter_other(i);
      }
      return t;
    } else {
      return (dr_dag_node *)0;
    }
  }

  static_if_inline void 
  dr_return_from_other__(dr_dag_node * t, 
			 const char * file, int line, int worker) {
    if (GS.generation % 2) {
      dr_worker_specific_state * wss 
	= dr_get_worker_specific_state(worker);
      dr_dag_node * rt = dr_task_last_node(t);
      if (DAG_RECORDER_VERBOSE_LEVEL>=3) {
	printf("dr_return_from_other(task=%p) by %d interval=%p\n", 
	       t, wss->worker, rt);
      }
      (void)dr_check(rt->info.kind == dr_dag_node_kind_other);
      /* set this worker's current task */
      dr_set_cur_task_(wss, t);
      /* this node's est is the preceeding node's est
	 + its critical path len */
      t->info.est = rt->info.est + rt->info.t_inf;
      /* this node became ready when the preceeding
	 node finished */
      t->info.first_ready_t = rt->info.end.t;
      (void)dr_check(t->info.first_ready_t > 0);
      /* its incoming edge must be create_cont type */
      t->info.in_edge_kind = dr_dag_edge_kind_other_cont;
      /* call hook */
      if (GS.opts.hooks.return_from_other) {
	GS.opts.hooks.return_from_other(t);
      }
      /* record an interval just started */
      dr_set_start_info(&t->info.start, wss->worker, file, line);
    }
  }

  /* 
     called when a program ends a task
   */
  static_if_inline void 
  dr_end_task__(const char * file, int line,int worker) {
    if (GS.generation % 2) {
      dr_clock_t end_t = dr_get_tsc();
      dr_worker_specific_state * wss 
	= dr_get_worker_specific_state(worker);
      dr_dag_node * t = dr_get_cur_task_(wss);
      dr_dag_node * s = dr_task_active_node(t);
      /* create a new node */
      dr_dag_node * i 
	= dr_dag_node_list_push_back(s->subgraphs, wss->freelist);
      if (DAG_RECORDER_VERBOSE_LEVEL>=3) {
	printf("dr_end_task() by %d task=%p, section=%p, "
	       "new interval=%p\n", 
	       wss->worker, t, s, i);
      }
      dr_end_interval_(i, wss->worker, 
		       dr_dag_node_kind_end_task,
		       t->info.in_edge_kind,
		       end_t, t->info.est, t->info.first_ready_t, 
		       file, line, t->info.start);
      /* call hook (important to call it before summarize,
	 to get counters updated) */
      if (GS.opts.hooks.end_task) {
	GS.opts.hooks.end_task(i);
      }
      dr_summarize_section_or_task(wss->prune_stack, t, 
				   wss->freelist);
    }
  }

  /* dummy function to supress many
     "static function defined but not called" 
     errors */
#if ! __CILK__
  __attribute__((unused)) 
#endif
  static void dr_dummy_call_static_functions() {
    dr_dag_node * t;
    dr_dag_node * c;
    dr_start__(NULL, "", 1, 1, 1);

    dr_begin_section__(1);
    t = dr_enter_create_task__(&c, "", 1, 1);
    dr_enter_create_cilk_proc_task__("", 1, 1);
    dr_start_task__(c, "", 1, 1);
    dr_start_cilk_proc__("", 1, 1);
    dr_end_task__("", 1, 1);
    dr_return_from_create_task__(t, "", 1, 1);
    t = dr_enter_wait_tasks__("", 1, 1);
    dr_return_from_wait_tasks__(t, "", 1, 1);
    t = dr_enter_other__("", 1, 1);
    dr_return_from_other__(t, "", 1, 1);
    dr_stop__("", 1, 1);

    dr_free(t, sizeof(dr_dag_node));
    dr_dag_node_kind_to_str((dr_dag_node_kind_t)1);
    //dr_pi_dag_open_to_write("", "", NULL);
    dr_dag_node_list_size(0);
    dr_dummy_call_static_functions();

  }

#ifdef __cplusplus
}
#endif

