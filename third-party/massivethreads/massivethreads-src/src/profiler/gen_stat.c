/* 
 * gen_basic.c
 */

#include <errno.h>
#define DAG_RECORDER 2
#include "dag_recorder_impl.h"

typedef struct {
  void (*process_event)(chronological_traverser * ct, dr_event evt);
  dr_pi_dag * G;
  dr_clock_t cum_running;
  dr_clock_t cum_delay;
  dr_clock_t cum_no_work;
  dr_clock_t t;
  long n_running;
  long n_ready;
  long n_workers;
  dr_clock_t total_elapsed;
  dr_clock_t total_t_1;
  long * edge_counts;		/* kind,u,v */
  int warnings_n_running;
} dr_basic_stat;

static void 
dr_basic_stat_process_event(chronological_traverser * ct, 
			    dr_event evt);

static void
dr_calc_inner_delay(dr_basic_stat * bs, dr_pi_dag * G) {
  long n = G->n;
  long i;
  dr_clock_t total_elapsed = 0;
  dr_clock_t total_t_1 = 0;
  dr_pi_dag_node * T = G->T;
  long n_negative_inner_delays = 0;
  for (i = 0; i < n; i++) {
    dr_pi_dag_node * t = &T[i];
    dr_clock_t t_1 = t->info.t_1;
    dr_clock_t elapsed = t->info.end.t - t->info.start.t;
    if (t->info.kind < dr_dag_node_kind_section
	|| t->subgraphs_begin_offset == t->subgraphs_end_offset) {
      total_elapsed += elapsed;
      total_t_1 += t_1;
      if (elapsed < t_1 && t->info.worker != -1) {
	if (1 || (n_negative_inner_delays == 0)) {
	  fprintf(stderr,
		  "warning: node %ld has negative"
		  " inner delay (worker=%d, start=%llu, end=%llu,"
		  " t_1=%llu, end - start - t_1 = -%llu\n",
		  i, t->info.worker,
		  t->info.start.t, t->info.end.t, t->info.t_1,
		  t_1 - elapsed);
	}
	n_negative_inner_delays++;
      }
    }
  }
  if (n_negative_inner_delays > 0) {
    fprintf(stderr,
	    "warning: there are %ld nodes that have negative delays",
	    n_negative_inner_delays);
  }
  bs->total_elapsed = total_elapsed;
  bs->total_t_1 = total_t_1;
}

static void
dr_calc_edges(dr_basic_stat * bs, dr_pi_dag * G) {
  long n = G->n;
  long m = G->m;
  long nw = G->num_workers;
  /* C : a three dimensional array
     C(kind,i,j) is the number of type k edges from 
     worker i to worker j.
     we may counter nodes with worker id = -1
     (executed by more than one workers);
     we use worker id = nw for such entries
  */
  long * C_ = (long *)dr_malloc(sizeof(long) * dr_dag_edge_kind_max * (nw + 1) * (nw + 1));
#define EDGE_COUNTS(k,i,j) C_[k*(nw+1)*(nw+1)+i*(nw+1)+j]
  dr_dag_edge_kind_t k;
  long i, j;
  for (k = 0; k < dr_dag_edge_kind_max; k++) {
    for (i = 0; i < nw + 1; i++) {
      for (j = 0; j < nw + 1; j++) {
	EDGE_COUNTS(k,i,j) = 0;
      }
    }
  }
  for (i = 0; i < n; i++) {
    dr_pi_dag_node * t = &G->T[i];
    if (t->info.kind >= dr_dag_node_kind_section
	&& t->subgraphs_begin_offset == t->subgraphs_end_offset) {
      for (k = 0; k < dr_dag_edge_kind_max; k++) {
	int w = t->info.worker;
	if (w == -1) {
#if 0
	  fprintf(stderr, 
		  "warning: node %ld (kind=%s) has worker = %d)\n",
		  i, dr_dag_node_kind_to_str(t->info.kind), w);
#endif
	  EDGE_COUNTS(k, nw, nw) += t->info.logical_edge_counts[k];
	} else {
	  (void)dr_check(w >= 0);
	  (void)dr_check(w < nw);
	  EDGE_COUNTS(k, w, w) += t->info.logical_edge_counts[k];
	}
      }
    }    
  }
  for (i = 0; i < m; i++) {
    dr_pi_dag_edge * e = &G->E[i];
    int uw = G->T[e->u].info.worker;
    int vw = G->T[e->v].info.worker;
    if (uw == -1) {
#if 0
      fprintf(stderr, "warning: source node (%ld) of edge %ld %ld (kind=%s) -> %ld (kind=%s) has worker = %d\n",
	      e->u,
	      i, 
	      e->u, dr_dag_node_kind_to_str(G->T[e->u].info.kind), 
	      e->v, dr_dag_node_kind_to_str(G->T[e->v].info.kind), uw);
#endif
      uw = nw;
    }
    if (vw == -1) {
#if 0
      fprintf(stderr, "warning: dest node (%ld) of edge %ld %ld (kind=%s) -> %ld (kind=%s) has worker = %d\n",
	      e->v,
	      i, 
	      e->u, dr_dag_node_kind_to_str(G->T[e->u].info.kind), 
	      e->v, dr_dag_node_kind_to_str(G->T[e->v].info.kind), vw);
#endif
      vw = nw;
    }
    (void)dr_check(uw >= 0);
    (void)dr_check(uw <= nw);
    (void)dr_check(vw >= 0);
    (void)dr_check(vw <= nw);
    EDGE_COUNTS(e->kind, uw, vw)++;
  }
#undef EDGE_COUNTS
  bs->edge_counts = C_;
}


static void 
dr_basic_stat_init(dr_basic_stat * bs, dr_pi_dag * G) {
  bs->process_event = dr_basic_stat_process_event;
  bs->G = G;
  bs->n_running = 0;
  bs->n_ready = 0;
  bs->n_workers = G->num_workers;
  bs->cum_running = 0;		/* cumulative running cpu time */
  bs->cum_delay = 0;		/* cumulative delay cpu time */
  bs->cum_no_work = 0;		/* cumulative no_work cpu time */
  bs->t = 0;			/* time of the last event */
  bs->warnings_n_running = 0;	/* we have not warned warnings_n_running > n_workers */
}

static void
dr_basic_stat_destroy(dr_basic_stat * bs, dr_pi_dag * G) {
  long nw = G->num_workers;
  dr_free(bs->edge_counts, 
	  sizeof(long) * dr_dag_edge_kind_max * (nw + 1) * (nw + 1));
}

static void 
dr_basic_stat_process_event(chronological_traverser * ct, 
			    dr_event evt) {
  dr_basic_stat * bs = (dr_basic_stat *)ct;
  dr_clock_t dt = evt.t - bs->t;

  int n_running = bs->n_running;
  int n_delay, n_no_work;
  if (bs->n_running >= bs->n_workers) {
    /* great, all workers are running */
    n_delay = 0;
    n_no_work = 0;
    if (bs->n_running > bs->n_workers) {
      if (bs->warnings_n_running < 2) {
	fprintf(stderr, 
		"warning: n_running = %ld"
		" > n_workers = %ld (clock skew?)\n",
		bs->n_running, bs->n_workers);
	if (bs->warnings_n_running > 0) {
	  fprintf(stderr, 
		  "note: further occurrences of this warning will be suppressed\n");
	}
	bs->warnings_n_running++;
      }
    }
    n_delay = 0;
    n_no_work = 0;
  } else if (bs->n_running + bs->n_ready <= bs->n_workers) {
    /* there were enough workers to run ALL ready tasks */
    n_delay = bs->n_ready;
    n_no_work = bs->n_workers - (bs->n_running + bs->n_ready);
  } else {
    n_delay = bs->n_workers - bs->n_running;
    n_no_work = 0;
  }
  bs->cum_running += n_running * dt;
  bs->cum_delay   += n_delay * dt;
  bs->cum_no_work += n_no_work * dt;

  switch (evt.kind) {
  case dr_event_kind_ready: {
    bs->n_ready++;
    break;
  }
  case dr_event_kind_start: {
    bs->n_running++;
    break;
  }
  case dr_event_kind_last_start: {
    bs->n_ready--;
    break;
  }
  case dr_event_kind_end: {
    bs->n_running--;
    break;
  }
  default:
    assert(0);
    break;
  }

  bs->t = evt.t;
}

static void
dr_write_edge_counts(dr_basic_stat * bs, FILE * wp) {
  dr_dag_edge_kind_t k;
  int i, j;
  int nw = bs->n_workers;
  long * C_ = bs->edge_counts;
#define EDGE_COUNTS(k,i,j) C_[k*(nw+1)*(nw+1)+i*(nw+1)+j]
  for (k = 0; k < dr_dag_edge_kind_max; k++) {
    switch (k) {
    case dr_dag_edge_kind_end:
      fprintf(wp, "end-parent edges:\n");
      break;
    case dr_dag_edge_kind_create:
      fprintf(wp, "create-child edges:\n");
      break;
    case dr_dag_edge_kind_create_cont:
      fprintf(wp, "create-cont edges:\n");
      break;
    case dr_dag_edge_kind_wait_cont:
      fprintf(wp, "wait-cont edges:\n");
      break;
    case dr_dag_edge_kind_other_cont:
      fprintf(wp, "other-cont edges:\n");
      break;
    default:
      (void)dr_check(0);
      break;
    }
    for (i = 0; i < nw + 1; i++) {
      for (j = 0; j < nw + 1; j++) {
	long c = EDGE_COUNTS(k,i,j);
	fprintf(wp, " %ld", c);
      }
      fprintf(wp, "\n");
    }
  }
}


static int
dr_basic_stat_write_to_file(dr_basic_stat * bs, FILE * wp) {
  dr_pi_dag * G = bs->G;
  long * nc = G->T[0].info.logical_node_counts;
  long n_creates = nc[dr_dag_node_kind_create_task];
  long n_waits = nc[dr_dag_node_kind_wait_tasks];
  long n_others = nc[dr_dag_node_kind_other];
  long n_ends = nc[dr_dag_node_kind_end_task];
  //long n_sections = nc[dr_dag_node_kind_section];
  //long n_tasks = nc[dr_dag_node_kind_task];
  long n_ints = n_creates + n_waits + n_others + n_ends;
  //long n_nodes = n_ints + n_sections + n_tasks;
  long n_nodes = n_ints + n_waits + n_creates + 1;
  long n_mat_nodes = G->T[0].info.cur_node_count;
  dr_clock_t t_inf = G->T[0].info.t_inf;
  dr_clock_t work = bs->total_t_1;
  dr_clock_t delay = bs->cum_delay + (bs->total_elapsed - bs->total_t_1);
  dr_clock_t no_work = bs->cum_no_work;
  double nw = bs->n_workers;

  (void)dr_check(work == G->T[0].info.t_1);
  /* +1 because root task is a task, yet not counted as a creation */
  //(void)dr_check(n_creates + 1 == n_tasks);
  //(void)dr_check(n_ends == n_tasks);
  //(void)dr_check(n_waits == n_sections);

  fprintf(wp, "create_task           = %ld\n", n_creates);
  fprintf(wp, "wait_tasks            = %ld\n", n_waits);
  fprintf(wp, "end_task              = %ld\n", n_ends);
  fprintf(wp, "work (T1)             = %llu\n", work);
  fprintf(wp, "delay                 = %llu\n", delay);
  fprintf(wp, "no_work               = %llu\n", no_work);
  fprintf(wp, "critical_path (T_inf) = %llu\n", t_inf);
  fprintf(wp, "n_workers (P)         = %ld\n", bs->n_workers);
  fprintf(wp, "elapsed               = %llu\n", bs->t);

  if (work + delay + no_work != bs->n_workers * bs->t) {
    fprintf(wp, 
	    "*** warning (running + delay + no_work)"
	    " / n_workers = %llu != elapsed_time\n", 
	    (work + delay + no_work) / bs->n_workers);
  }
  fprintf(wp, "T1/P                  = %.3f\n", 
	  work/nw);
  fprintf(wp, "T1/P+T_inf            = %.3f\n", 
	  work/nw + t_inf);
  fprintf(wp, "T1/T_inf              = %.3f\n", 
	  work/(double)t_inf);
  fprintf(wp, "greedy speedup        = %.3f\n", 
	  work/(work/nw + t_inf));
  fprintf(wp, "observed speedup      = %.3f\n", 
	  work/(double)bs->t);
  fprintf(wp, "observed/greedy       = %.3f\n", 
	  (work/nw + t_inf)/bs->t);
  fprintf(wp, "task granularity      = %.3f\n", 
	  bs->cum_running/(double)n_creates);
  fprintf(wp, "interval granularity  = %.3f\n", 
	  bs->cum_running/(double)n_ints);
  fprintf(wp, "dag nodes             = %ld\n", n_nodes);
  fprintf(wp, "materialized nodes    = %ld\n", n_mat_nodes);
  fprintf(wp, "compression ratio     = %f\n", 
	  n_mat_nodes / (double)n_nodes);
  dr_write_edge_counts(bs, wp);

  if (ferror(wp)) {
    fprintf(stderr, "%s:%d:error: fprintf %s\n", 
	    __FILE__, __LINE__, strerror(errno)); 
    return 0;
  } else {
    return 1;
  }
}

int 
dr_gen_basic_stat(dr_pi_dag * G) {
  if (!GS.opts.stat_file_yes) {
    return 1;			/* not requested, OK */
  } else {
    FILE * wp = dr_pi_dag_open_to_write(GS.opts.dag_file_prefix, ".stat", 
					"stat", 
					GS.opts.verbose_level >= 1);
    if (!wp) {
      return 0;			/* failed */
    } else {
      dr_basic_stat bs[1];
      dr_basic_stat_init(bs, G);
      dr_calc_inner_delay(bs, G);
      dr_calc_edges(bs, G);
      dr_pi_dag_chronological_traverse(G, (chronological_traverser *)bs);
      int r = dr_basic_stat_write_to_file(bs, wp);
      dr_basic_stat_destroy(bs, G);
      fclose(wp);
      return r;
    }
  }
}


