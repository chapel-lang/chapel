/* 
 * chronological.c
 */
#define DAG_RECORDER 2
#include "dag_recorder_impl.h"

dr_event 
dr_mk_event(dr_clock_t t, 
	    dr_event_kind_t kind, 
	    dr_pi_dag_node * u,
	    dr_pi_dag_node * pred,
	    dr_dag_edge_kind_t edge_kind) {
  dr_event e = { t, kind, u, pred, edge_kind };
  return e;
}

static dr_pi_dag_node * 
dr_pi_dag_first_leaf(dr_pi_dag * G) {
  dr_pi_dag_node * T = G->T;
  dr_pi_dag_node * T_lim = G->T + G->n;
  dr_pi_dag_node * g = T;
  assert(g < T_lim);
  while (g->info.kind >= dr_dag_node_kind_section
	 && g->subgraphs_begin_offset < g->subgraphs_end_offset) {
    g = g + g->subgraphs_begin_offset;
    assert(g < T_lim);
  }
  return g;
}

/* priority queue of events (sorted by its time stamp (t) field) */
typedef struct {
  long sz;
  long n;
  dr_event * events;
} dr_event_queue;

static dr_event_queue * 
dr_mk_event_queue() {
  dr_event_queue * q = (dr_event_queue *)dr_malloc(sizeof(dr_event_queue)); /* leaking */
  long sz = 100;
  q->events = (dr_event *)dr_malloc(sizeof(dr_event) * sz); /* leaking ? */
  q->sz = sz;
  q->n = 0;
  return q;
}

static void
dr_destroy_event_queue(dr_event_queue * q) {
  dr_free(q->events, sizeof(dr_event) * q->sz);
  dr_free(q, sizeof(dr_event_queue));
}

/* check if parent <= child */
static void 
dr_event_queue_heap_check(dr_event_queue * q) {
  dr_event * evts = q->events;
  long n = q->n;
  long i;
  for (i = 0; i < n; i++) {
    if (2 * i + 1 < n) {
      assert(evts[i].t <= evts[2*i+1].t);
    }
    if (2 * i + 2 < n) {
      assert(evts[i].t <= evts[2*i+2].t);
    }
  }
}

/* ensure q has sz elements */
static void 
dr_event_queue_ensure(dr_event_queue * q, long sz) {
  if (q->sz < sz) {
    long new_sz = 2 * sz + 1;
    dr_event * evts = (dr_event *)dr_malloc(sizeof(dr_event) * new_sz);
    memcpy(evts, q->events, sizeof(dr_event) * q->sz);
    dr_free(q->events, sizeof(dr_event) * q->sz);
    q->events = evts;
    q->sz = new_sz;
  }
  dr_event_queue_heap_check(q);
}

/* 
   i's children -> 2i+1 and 2i+2
   i's parent = (i - 1)/2
 */

/* evts constitute a heap
   except for the last element evts[n-1].
   fix up the last element so that 
   it again becomes a heap */
static void 
dr_event_queue_heapify_up(dr_event_queue * q) {
  dr_event * evts = q->events;
  long n = q->n;
  long x = n - 1;
  while (x > 0) {
    long p = (x-1)/2;		/* parent */
    if (evts[x].t >= evts[p].t) break;
    /* sway x <-> y */
    dr_event e = evts[x];
    evts[x] = evts[p];
    evts[p] = e;
    x = p;
  }
  dr_event_queue_heap_check(q);
}

/* evts constitute a heap, 
   except for the first element evts[0].
   fix up the last element so that 
   it again becomes a heap */
static void 
dr_event_queue_heapify_down(dr_event_queue * q) {
  dr_event * evts = q->events;
  long n = q->n;
  long x = 0;
  while (2 * x + 2 < n) {
    long p = 2 * x + 1;
    long q = 2 * x + 2;
    if (evts[x].t <= evts[p].t
	&& evts[x].t <= evts[q].t) break;
    if (evts[p].t < evts[q].t) {
      /* swap x <-> p */
      dr_event e = evts[x];
      evts[x] = evts[p];
      evts[p] = e;
      x = p;
    } else {
      /* swap x <-> q */
      dr_event e = evts[x];
      evts[x] = evts[q];
      evts[q] = e;
      x = q;
    }
  }
  //assert(2 * x + 2 >= n);
  if (2 * x + 1 < n) {
    long p = 2 * x + 1;
    if (evts[x].t > evts[p].t) {
      /* swap x <-> p */
      dr_event e = evts[x];
      evts[x] = evts[p];
      evts[p] = e;
      x = p;
    }
  }
  dr_event_queue_heap_check(q);
}

/* enqueue evt */
static void 
dr_event_queue_enq(dr_event_queue * q, dr_event evt) {
  dr_event_queue_ensure(q, q->n + 1);
  assert(q->n < q->sz);
  q->events[q->n] = evt;
  q->n++;
  dr_event_queue_heapify_up(q);
}

/* dequeue evt */
static dr_event
dr_event_queue_deq(dr_event_queue * q) {
  assert(q->n > 0);
  dr_event ev = q->events[0];
  q->events[0] = q->events[q->n - 1];
  q->n--;
  dr_event_queue_heapify_down(q);
  return ev;
}

/* chronogically generate events into ct  */
void 
dr_pi_dag_chronological_traverse(dr_pi_dag * G,
				 chronological_traverser * ct) {
  dr_pi_dag_node * s = dr_pi_dag_first_leaf(G);
  dr_event_queue * F = dr_mk_event_queue();
  long i;
  int * ready_count = (int *)dr_malloc(sizeof(int) * G->n);
  for (i = 0; i < G->n; i++) {
    ready_count[i] = 0;
  }
  for (i = 0; i < G->m; i++) {
    assert(G->E[i].v < G->n);
    ready_count[G->E[i].v]++;
  }
  dr_event_queue_enq(F, dr_mk_event(0, dr_event_kind_ready, s, 0, 
				    dr_dag_edge_kind_create));
  while (F->n) {
    dr_event ev = dr_event_queue_deq(F);
    dr_pi_dag_node * u = ev.u;
    //ct->process_event(ct, ev);
    switch (ev.kind) {
    case dr_event_kind_ready: {
      if (GS.opts.dbg_level>=2) {
	printf("%llu : node %ld ready\n", ev.t, u - G->T);
      }
      dr_event_queue_enq(F, dr_mk_event(u->info.start.t, 
					dr_event_kind_start, u, 
					ev.pred, ev.edge_kind));
      break;
    }
    case dr_event_kind_start: {
      if (GS.opts.dbg_level>=2) {
	printf("%llu : node %ld start\n", ev.t, u - G->T);
      }
      dr_event_queue_enq(F, dr_mk_event(u->info.last_start_t, 
					dr_event_kind_last_start, u, 
					ev.pred, ev.edge_kind));
      break;
    }
    case dr_event_kind_last_start: {
      if (GS.opts.dbg_level>=2) {
	printf("%llu : node %ld last start\n", ev.t, u - G->T);
      }
      dr_event_queue_enq(F, dr_mk_event(u->info.end.t, 
					dr_event_kind_end, u, 
					ev.pred, ev.edge_kind));
      break;
    }
    case dr_event_kind_end: {
      if (GS.opts.dbg_level>=2) {
	printf("%llu : node %ld end\n", ev.t, u - G->T);
      }
      dr_pi_dag_edge * e_begin = G->E + u->edges_begin;
      dr_pi_dag_edge * e_end = G->E + u->edges_end;
      dr_pi_dag_edge * e;
      for (e = e_begin; e < e_end; e++) {
	assert(G->T + e->u == u);
	if (GS.opts.dbg_level>=2) {
	  printf(" node %ld ready counter %d -> %d\n", 
		 e->v, ready_count[e->v], ready_count[e->v] - 1);
	}
	ready_count[e->v]--;
	if (ready_count[e->v] == 0) {
	  dr_pi_dag_node * v = G->T + e->v;
	  dr_event_queue_enq(F, dr_mk_event(u->info.end.t, 
					    dr_event_kind_ready, v, u, 
					    e->kind));
	}
      }
      break;
    }
    default:
      break;
    }
    ct->process_event(ct, ev);
  }
  dr_free(ready_count, sizeof(int) * G->n);
  dr_destroy_event_queue(F);
}

