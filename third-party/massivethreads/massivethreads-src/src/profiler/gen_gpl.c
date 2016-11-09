/* 
 * gen_gpl.c
 */

#include <errno.h>
#include <string.h>
#define DAG_RECORDER 2
#include "dag_recorder_impl.h"

/* 
   this file calculates a time series
   of the number of running and ready
   tasks and generates a gnuplot file.
 */

typedef enum {
  dr_node_state_running,
  dr_node_state_ready_end,
  dr_node_state_ready_create,
  dr_node_state_ready_create_cont,
  dr_node_state_ready_wait_cont,
  dr_node_state_ready_other_cont,
  dr_node_state_max,
} dr_node_state_t;

/* an entry in the parallelism profile. */
typedef struct {
  dr_clock_t t;			/* time */
  /* count of nodes in each state */
  int hist[dr_node_state_max];
  /* count of nodes in each state in float */
  double hist_float[dr_node_state_max];
} dr_para_prof_history_entry;

typedef struct {
  /* this must be the first element */
  void (*process_event)(chronological_traverser * ct, dr_event evt);
  int counts[dr_node_state_max];
  double counts_float[dr_node_state_max];
  int last_counts[dr_node_state_max];
  double last_counts_float[dr_node_state_max];
  dr_clock_t last_record_time;
  dr_clock_t next_check_time;
  dr_clock_t finish_time;
  dr_para_prof_history_entry * history;
  long hist_sz;
  long n_hists;
} dr_para_prof;

static void 
dr_para_prof_process_event(chronological_traverser * pp, dr_event evt);

static void 
dr_para_prof_init(dr_para_prof * pp, long hist_sz, dr_clock_t finish_time) {
  int i;
  for (i = 0; i < dr_node_state_max; i++) {
    pp->counts[i] = 0;
    pp->counts_float[i] = 0.0;
    pp->last_counts[i] = 0;
    pp->last_counts_float[i] = 0.0;
  }
  pp->process_event = dr_para_prof_process_event;
  pp->finish_time = finish_time;
  pp->last_record_time = 0;
  pp->next_check_time = 0;
  pp->hist_sz = hist_sz;
  pp->n_hists = 0;
  pp->history = (dr_para_prof_history_entry *)
    dr_malloc(sizeof(dr_para_prof_history_entry) * hist_sz);
}

static void 
dr_para_prof_destroy(dr_para_prof * pp) {
  dr_free(pp->history, 
	  sizeof(dr_para_prof_history_entry) * pp->hist_sz);
}


static void 
dr_para_prof_add_hist(dr_para_prof * pp, dr_clock_t t) {
  long sz = pp->hist_sz;
  long n = pp->n_hists;
  dr_para_prof_history_entry * hist = pp->history;
  /* 

     +-----+-----+-----+-----+
     0     1       ...       sz-1

     we try to plot the time series by 
     sampling values at the above points.
     when we have recorded n points,

     +-----+-----+-----+-----+
     0    ...   <n>          sz-1

     the next point should come at or after
     the point <n>. we track the newest
     value, and when we encounter a point
     at or after <n+1>, 

   */

  int k;
  if (t >= pp->next_check_time) {
    dr_para_prof_history_entry * h = &hist[n];
    assert(n < sz);
    /* 
                         +
          ++
          |++
         ++
        ++
	----x------x-----x------------
          last    next   t

     */

    /* make check point at the last point */
    h->t = pp->last_record_time;
    for (k = 0; k < dr_node_state_max; k++) {
      h->hist[k] = pp->last_counts[k];
      h->hist_float[k] = pp->last_counts_float[k];
    }
    if (GS.opts.dbg_level>=2) {
      fprintf(stderr, "hist[%ld] @ %llu :", n, h->t);
      for (k = 0; k < dr_node_state_max; k++) {
	fprintf(stderr, " %f", h->hist_float[k]);
      }
      fprintf(stderr, "\n");
    }
    pp->n_hists = n + 1;
    /* 
       divide the remaining time by
       the remaining slots and calc
       next check time

         -----------+--x-------------+ 
                    t  next      finish_time
                   [n]             [sz-1]
       
     */
    if (n == sz - 1) {
      pp->next_check_time = pp->finish_time + 1;
    } else {
      pp->next_check_time = t + (pp->finish_time - t) / (sz - 1 - n);
    }
  }

  for (k = 0; k < dr_node_state_max; k++) {
    pp->last_counts[k] = pp->counts[k];
    pp->last_counts_float[k] = pp->counts_float[k];
    pp->last_record_time = t;
  }
}

static void
dr_para_prof_check(dr_para_prof * pp) {
  int i;
  for (i = 0; i < dr_node_state_max; i++) {
    (void)dr_check(pp->counts[i] == 0);
    (void)dr_check(pp->counts_float[i] < 1.0e-5);
    (void)dr_check(pp->counts_float[i] > -1.0e-5);
  }
}

static int
dr_para_prof_write_to_file(dr_para_prof * pp, FILE * wp) {
  long n = pp->n_hists;
  dr_para_prof_history_entry * h = pp->history;
  long i;
  int k;
  /* 
     dr_node_state_running,
     dr_node_state_ready_end,
     dr_node_state_ready_create,
     dr_node_state_ready_create_cont,
     dr_node_state_ready_wait_cont,
     dr_node_state_ready_other_cont,

     the following must match the definition
     of dr_node_state_t
  */

  fprintf(wp,
	  "plot"
	  "  \"-\" u 1:2:3 w filledcurves title \"running\""
	  ", \"-\" u 1:2:3 w filledcurves title \"end\""
	  ", \"-\" u 1:2:3 w filledcurves title \"create\""
	  ", \"-\" u 1:2:3 w filledcurves title \"create cont\""
	  ", \"-\" u 1:2:3 w filledcurves title \"wait cont\""
	  ", \"-\" u 1:2:3 w filledcurves title \"other cont\""
	  "\n"
	  );
  for (k = 0; k < dr_node_state_max; k++) {
    for (i = 0; i < n; i++) {
      /* we transit from
	 (t_prev, y_prev) -> (t, y), we draw
	 the following *******

                    y        *-----+
		             *
               y_prev +*******
                      |      
                      |
                     t_prev  t
      */
      int j;
      int y_prev = 0;
      int y = 0;
      double y_prev_float = 0;
      double y_float = 0;

      if (i == 0) {
	//fprintf(wp, "%d %f %f\n", 0, 0.0, 0.0);
	//fprintf(wp, "%llu %f %f\n", h[0].t, 0.0, 0.0);
      } else {
	for (j = 0; j < k; j++) {
	  y_prev       += h[i-1].hist[j];
	  y_prev_float += h[i-1].hist_float[j];
	}
	fprintf(wp, "%llu %f %f\n", 
		h[i].t, y_prev_float, 
		y_prev_float + h[i-1].hist_float[k]);
      }
      for (j = 0; j < k; j++) {
	y += h[i].hist[j];
	y_float += h[i].hist_float[j];
      }
      fprintf(wp, "%llu %f %f\n", 
	      h[i].t, y_float, y_float + h[i].hist_float[k]);
    }
    fprintf(wp, "e\n");
  }
  fprintf(wp, "pause -1\n");

  if (ferror(wp)) {
    fprintf(stderr, "%s:%d:error: fprintf %s\n", 
	    __FILE__, __LINE__, strerror(errno)); 
    return 0;
  } else {
    return 1;
  }
}


static void 
dr_para_prof_show_counts(dr_para_prof * pp) {
  int k;
  printf(" node states:");
  for (k = 0; k < dr_node_state_max; k++) {
    //printf(" %d", pp->counts[k]);
    printf(" %f", pp->counts_float[k]);
  }  
  printf("\n");
}

static dr_node_state_t 
dr_dag_edge_kind_to_node_state(dr_dag_edge_kind_t ek) {
  switch (ek) {
  case dr_dag_edge_kind_end:
    return dr_node_state_ready_end;
  case dr_dag_edge_kind_create:
    return dr_node_state_ready_create;
  case dr_dag_edge_kind_create_cont:
    return dr_node_state_ready_create_cont;
  case dr_dag_edge_kind_wait_cont:
    return dr_node_state_ready_wait_cont;
  case dr_dag_edge_kind_other_cont:
    return dr_node_state_ready_other_cont;
  default:
    (void)dr_check(0);
  }
  return 0;
}

static const char *
dr_node_state_to_str(dr_node_state_t s) {
  switch(s) {
  case dr_node_state_running:
    return "running";
  case dr_node_state_ready_end:
    return "ready_end";
  case dr_node_state_ready_create:
    return "ready_create";
  case dr_node_state_ready_create_cont:
    return "ready_create_cont";
  case dr_node_state_ready_wait_cont:
    return "ready_wait_cont";
  case dr_node_state_ready_other_cont:
    return "ready_other_cont";
  default:
    (void)dr_check(0);
  }
  return 0;
}

static void 
dr_para_prof_process_event(chronological_traverser * pp_, 
			   dr_event ev) {
  dr_para_prof * pp = (dr_para_prof *)pp_;
  switch (ev.kind) {
  case dr_event_kind_ready: {
    /* dt = time between 
       the point when a leaf in u first became ready and 
       the point when a leaf in u last became started */
    double dt = ev.u->info.last_start_t - ev.u->info.first_ready_t;
    /* t_ready = cumulative time in which tasks are ready */
    dr_clock_t * t_ready = ev.u->info.t_ready;
    /* on average we have t_read/dt tasks ready in 
       the interval mentioned above */
    dr_dag_edge_kind_t k;
    for (k = 0; k < dr_dag_edge_kind_max; k++) {
      dr_node_state_t s = dr_dag_edge_kind_to_node_state(k);
      if (GS.opts.dbg_level>=2) {
	printf(" %p ready: count[%s] %f += %f -> %f\n",
	       ev.u, dr_node_state_to_str(s), 
	       pp->counts_float[s],  t_ready[k] / dt, 
	       pp->counts_float[s] + t_ready[k] / dt);
      }
      pp->counts[s]++;
      pp->counts_float[s] += t_ready[k] / dt;
    }
    break;
  }
  case dr_event_kind_start: {
    double dt = ev.u->info.end.t - ev.u->info.start.t;
    dr_clock_t w = ev.u->info.t_1;
    if (GS.opts.dbg_level>=2) {
      printf("           count[running] %f += %f -> %f\n",
	     pp->counts_float[dr_node_state_running], w / dt, 
	     pp->counts_float[dr_node_state_running] + w / dt);
    }
    pp->counts[dr_node_state_running]++;
    pp->counts_float[dr_node_state_running] += w / dt;
    break;
  }
  case dr_event_kind_last_start: {
    double dt = ev.u->info.last_start_t - ev.u->info.first_ready_t;
    dr_clock_t * t_ready = ev.u->info.t_ready;
    dr_dag_edge_kind_t k;
    for (k = 0; k < dr_dag_edge_kind_max; k++) {
      dr_node_state_t s = dr_dag_edge_kind_to_node_state(k);
      if (GS.opts.dbg_level>=2) {
	printf(" %p start: count[%s] %f -= %f -> %f\n",
	       ev.u, dr_node_state_to_str(s), 
	       pp->counts_float[s],  t_ready[k] / dt, 
	       pp->counts_float[s] - t_ready[k] / dt);
      }
      pp->counts[s]--;
      pp->counts_float[s] -= t_ready[k] / dt;
    }
    break;
  }
  case dr_event_kind_end: {
    double dt = ev.u->info.end.t - ev.u->info.start.t;
    dr_clock_t w = ev.u->info.t_1;
    if (GS.opts.dbg_level>=2) {
      printf(" %p end: count[running] %f -= %f -> %f\n",
	     ev.u, pp->counts_float[dr_node_state_running], w / dt, 
	     pp->counts_float[dr_node_state_running] - w / dt);
    }
    pp->counts[dr_node_state_running]--;
    pp->counts_float[dr_node_state_running] -= w / dt;
    break;
  }
  default:
    assert(0);
    break;
  }
  if (GS.opts.dbg_level>=2) {
    dr_para_prof_show_counts(pp);
  }
  dr_para_prof_add_hist(pp, ev.t);
}

int 
dr_gen_gpl(dr_pi_dag * G) {
  if (!GS.opts.gpl_file_yes) {
    return 1;
  } else {
    FILE * wp = dr_pi_dag_open_to_write(GS.opts.dag_file_prefix, ".gpl",
					"parallelism", GS.opts.verbose_level >= 1);
    if (!wp) {
      return 0;
    } else {
      dr_pi_dag_node * last = dr_pi_dag_node_last(G->T, G);
      dr_para_prof pp[1];
      dr_para_prof_init(pp, GS.opts.gpl_sz, last->info.end.t);
      dr_pi_dag_chronological_traverse(G, (chronological_traverser *)pp);
      dr_para_prof_check(pp);
      int r = dr_para_prof_write_to_file(pp, wp);
      dr_para_prof_destroy(pp);
      fclose(wp);
      return r;
    }
  }
}

