/* 
 * dag_recorder_impl.h
 */

#pragma once

#include <dag_recorder.h>

/* these are used to make sure the library version
   that dumps a dag file and the version that 
   reads it match.
   you ought not change the length of this string.
   they are the first bytes of dag recorder files */
#define DAG_RECORDER_HEADER     "DAG_RECORDER FORMAT VERSION  1.24 2014/09/18\n"
#define DAG_RECORDER_HEADER_LEN 45

/* a node of the dag, position independent */
struct dr_pi_dag_node {
  /* misc. information about this node */
  dr_dag_node_info info;
  /* two indexes in the edges array, pointing to 
     the begining and the end of edges from this node */
  long edges_begin;	 
  long edges_end;
  union {
    /* valid when this node is a create node.
       index of its child */
    long child_offset;
    /* valid when this node is a section or task node
       begin/end indexes of its subgraphs */
    struct {
      long subgraphs_begin_offset;
      long subgraphs_end_offset;
    };
  };
};

typedef struct dr_pi_dag_edge {
  dr_dag_edge_kind_t kind;
  long u;
  long v;
} dr_pi_dag_edge;

/* 
   n
   sz
   I ------+
   S ------+----+
   I[0] <--+    |
   I[1]         |
   ...          |
   I[n-1]       |
   ...  <-------+
   
 */

typedef struct {
  long n;			/* number of strings */
  long sz;			/* total bytes including headers */
  long * I;			/* index I[0] .. I[n-1] */
  const char * C;		/* char array */
} dr_pi_string_table;

/* the toplevel structure of a position-independent dag.
   when dumped into a file, we make sure each integer 
   fields occupies 8 bytes */
typedef struct dr_pi_dag {
  long n;			/* length of T */
  long m;			/* length of E */
  long start_clock;		/* absolute clock time of start */
  long num_workers;		/* number of workers */
  dr_pi_dag_node * T;		/* all nodes in a contiguous array */
  dr_pi_dag_edge * E;		/* all edges in a contiguous array */
  dr_pi_string_table * S;
} dr_pi_dag;

typedef enum {
  dr_event_kind_ready,
  dr_event_kind_start,
  dr_event_kind_last_start,
  dr_event_kind_end,
} dr_event_kind_t;

typedef struct {
  dr_clock_t t;			/* time at which it happened */
  dr_event_kind_t kind;		/* ready, start, or end */
  dr_pi_dag_node * u;		/* the node of the event */
  dr_pi_dag_node * pred;	/* the last predecessor */
  dr_dag_edge_kind_t edge_kind;	/* kind of edges from pred -> this node */
} dr_event;

dr_event 
dr_mk_event(dr_clock_t t, 
	    dr_event_kind_t kind, 
	    dr_pi_dag_node * u,
	    dr_pi_dag_node * pred,
	    dr_dag_edge_kind_t edge_kind);

typedef struct chronological_traverser chronological_traverser;
struct chronological_traverser {
  void (*process_event)(chronological_traverser * pp, dr_event evt);
};

void 
dr_pi_dag_chronological_traverse(dr_pi_dag * G,
				 chronological_traverser * ct);

FILE * dr_pi_dag_open_to_write(const char * prefix, const char * ext,
			       const char * file_kind, 
			       int show_message);

void dr_opts_init(dr_options * opts);
void dr_opts_print(dr_options * opts);

dr_pi_dag * 
dr_read_dag(const char * filename);

int dr_gen_pi_dag(dr_pi_dag * G);
int dr_gen_gpl(dr_pi_dag * G);
int dr_gen_basic_stat(dr_pi_dag * G);
int dr_gen_dot(dr_pi_dag * G);
int dr_gen_text(dr_pi_dag * G);

int interpolate_counters(dr_pi_dag * G);

void dr_copy_pi_dag(dr_pi_dag * G_, dr_pi_dag * G);

/* 
 *
 */

typedef struct dr_dag_node_stack_cell {
  /* next cell in the stack or a free list */
  struct dr_dag_node_stack_cell * next;
  dr_dag_node * node;		/* this is a node */
} dr_dag_node_stack_cell;

/* TODO:
   probably we should make it larger, so that
   we don't call malloc too many times.
   to do so, we explictly need to maintain
   the set of addresses obtained from malloc.
   currently there are no track of them.
   in the end we simply call free for individual
   cells (see dr_dag_node_stack_fini), and
   it is wrong when dr_dag_node_stack_cell_sz > 1
 */
enum { dr_dag_node_stack_cell_sz = 1 }; /* it must be 1 */

typedef struct dr_dag_node_stack {
  /* free list (recycle popped cells) */
  dr_dag_node_stack_cell * freelist;
  /* the stack (linear list) */
  dr_dag_node_stack_cell * top;
} dr_dag_node_stack;


dr_pi_dag_node *
dr_pi_dag_node_first(dr_pi_dag_node * g, dr_pi_dag * G);
dr_pi_dag_node *
dr_pi_dag_node_last(dr_pi_dag_node * g, dr_pi_dag * G);


/* initialize the stack to be empty */
static void 
dr_dag_node_stack_init(dr_dag_node_stack * s) {
  s->freelist = 0;
  s->top = 0;
}

static void 
dr_dag_node_stack_fini(dr_dag_node_stack * s) {
  (void)dr_check(!s->top);
  dr_dag_node_stack_cell * cell;
  dr_dag_node_stack_cell * next;
  for (cell = s->freelist; cell; cell = next) {
    next = cell->next;
    dr_free(cell, sizeof(dr_dag_node_stack_cell) * dr_dag_node_stack_cell_sz);
  }
}

/* ensure the free list is not empty.
   get memory via malloc and fill the free list.
   return a pointer to a cell */
static dr_dag_node_stack_cell * 
ensure_freelist(dr_dag_node_stack * s) {
  dr_dag_node_stack_cell * f = s->freelist;
  if (!f) {
    int n = dr_dag_node_stack_cell_sz;
    f = (dr_dag_node_stack_cell *)
      dr_malloc(sizeof(dr_dag_node_stack_cell) * n);
    int i;
    for (i = 0; i < n - 1; i++) {
      f[i].next = &f[i + 1];
    }
    f[n - 1].next = 0;
    s->freelist = f;
  }
  return f;
}

/* push a dag node to the stack */
static void 
dr_dag_node_stack_push(dr_dag_node_stack * s, 
		       dr_dag_node * node) {
  dr_dag_node_stack_cell * f = ensure_freelist(s);
  f->node = node;
  s->freelist = f->next;
  f->next = s->top;
  s->top = f;
}

/* push the children of g in the reverse order,
   so that we later handle the children in the
   right order */
static void
dr_dag_node_stack_push_children(dr_dag_node_stack * s, 
				dr_dag_node * g) {
  if (g->info.kind < dr_dag_node_kind_section) {
    if (g->info.kind == dr_dag_node_kind_create_task
	&& g->child) {
      dr_dag_node_stack_push(s, g->child);
    }
  } else {
    dr_dag_node * head = g->subgraphs->head;
    dr_dag_node * tail = g->subgraphs->tail;
    dr_dag_node * ch;
    /* a bit complicated to reverse the children */

    /* 1. count the number of children */
    int n_children = 0;
    for (ch = head; ch; ch = ch->next) {
      n_children++;
    }
    /* 2. make the array of the right size and
       fill the array with children */
    dr_dag_node ** children
      = (dr_dag_node **)dr_malloc(sizeof(dr_dag_node *) * n_children);
    int idx = 0;
    dr_dag_node_kind_t K = g->info.kind;
    for (ch = head; ch; ch = ch->next) {
      dr_dag_node_kind_t k = ch->info.kind;
      if (DAG_RECORDER_CHK_LEVEL>=1) {
	if (K == dr_dag_node_kind_section) {
	  (void)dr_check(k == dr_dag_node_kind_create_task 
			 || k == dr_dag_node_kind_wait_tasks
			 || k == dr_dag_node_kind_other
			 || k == dr_dag_node_kind_section);
	  if (k == dr_dag_node_kind_wait_tasks) {
	    (void)dr_check(ch == tail);
	  }
	} else {
	  (void)dr_check(K == dr_dag_node_kind_task);
	  (void)dr_check(k == dr_dag_node_kind_section
			 || k == dr_dag_node_kind_other
			 || k == dr_dag_node_kind_end_task);
	  if (k == dr_dag_node_kind_end_task) {
	    (void)dr_check(ch == tail);
	  }
	}
      }
      children[idx++] = ch;
    }
    assert(idx == n_children);
    /* 3. finally push them in the reverse order */
    for (idx = n_children - 1; idx >= 0; idx--) {
      dr_dag_node_stack_push(s, children[idx]);
    }
    dr_free(children, sizeof(dr_dag_node *) * n_children);
  }
}

/* pop an element from the stack s.
   it is either a string or a dag node.
   the result is returned to one of 
   *xp (when it is a node) and *strp
   (when it is a string). the other one
   is filled with a null.
*/
static dr_dag_node * 
dr_dag_node_stack_pop(dr_dag_node_stack * s) {
  dr_dag_node_stack_cell * top = s->top;
  (void)dr_check(top);
  s->top = top->next;
  top->next = s->freelist;
  s->freelist = top;
  return top->node;
}

static void use_unused() __attribute__ ((unused));
static void use_unused() {
  dr_dag_node_stack_init(0);
  dr_dag_node_stack_fini(0);
  dr_dag_node_stack_push(0, 0);
  dr_dag_node_stack_push_children(0, 0);
  dr_dag_node_stack_pop(0);
}
