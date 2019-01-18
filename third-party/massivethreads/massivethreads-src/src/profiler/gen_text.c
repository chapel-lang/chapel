/* 
 * gen_dot.c
 */
#include <errno.h>
#include <string.h>
#define DAG_RECORDER 2
#include "dag_recorder_impl.h"

/* write a single element x into wp. if failed, show err msg and return 0 */
static int
dr_pi_dag_gen_text_node(long idx,
			dr_pi_dag_node * g, 
			dr_pi_dag * G, FILE * wp, const char * sep) {
  (void)G;
  int i;
  fprintf(wp, "%ld",   idx);
  fprintf(wp, "%s%s",   sep, dr_dag_node_kind_to_str(g->info.kind));
  fprintf(wp, "%s%s",   sep, dr_dag_edge_kind_to_str(g->info.in_edge_kind));
  fprintf(wp, "%s%llu", sep, g->info.start.t);
  /* ********************8 */
  for (i = 0; i < dr_max_counters; i++) {
    fprintf(wp, "%s%llu", sep, g->info.start.counters[i]);
  }
  fprintf(wp, "%s%llu", sep, g->info.end.t);
  /* ******************** */
  for (i = 0; i < dr_max_counters; i++) {
    fprintf(wp, "%s%llu", sep, g->info.end.counters[i]);
  }
  fprintf(wp, "%s%llu", sep, g->info.est);
  fprintf(wp, "%s%llu", sep, g->info.t_1);
  fprintf(wp, "%s%llu", sep, g->info.t_inf);
  fprintf(wp, "%s%llu", sep, g->info.first_ready_t);
  fprintf(wp, "%s%llu", sep, g->info.last_start_t);
  fprintf(wp, "%s%llu", sep, g->info.t_ready[dr_dag_edge_kind_end]);
  fprintf(wp, "%s%llu", sep, g->info.t_ready[dr_dag_edge_kind_create]);
  fprintf(wp, "%s%llu", sep, g->info.t_ready[dr_dag_edge_kind_create_cont]);
  fprintf(wp, "%s%llu", sep, g->info.t_ready[dr_dag_edge_kind_wait_cont]);
  fprintf(wp, "%s%llu", sep, g->info.t_ready[dr_dag_edge_kind_other_cont]);
  fprintf(wp, "%s%ld",  sep, g->info.logical_node_counts[dr_dag_node_kind_create_task]);
  fprintf(wp, "%s%ld",  sep, g->info.logical_node_counts[dr_dag_node_kind_wait_tasks]);
  fprintf(wp, "%s%ld",  sep, g->info.logical_node_counts[dr_dag_node_kind_other]);
  fprintf(wp, "%s%ld",  sep, g->info.logical_node_counts[dr_dag_node_kind_end_task]);
  fprintf(wp, "%s%ld",  sep, g->info.logical_edge_counts[dr_dag_edge_kind_end]);
  fprintf(wp, "%s%ld",  sep, g->info.logical_edge_counts[dr_dag_edge_kind_create]);
  fprintf(wp, "%s%ld",  sep, g->info.logical_edge_counts[dr_dag_edge_kind_create_cont]);
  fprintf(wp, "%s%ld",  sep, g->info.logical_edge_counts[dr_dag_edge_kind_wait_cont]);
  fprintf(wp, "%s%ld",  sep, g->info.logical_edge_counts[dr_dag_edge_kind_other_cont]);
  fprintf(wp, "%s%ld",  sep, g->info.cur_node_count);
  fprintf(wp, "%s%ld",  sep, g->info.min_node_count);
  fprintf(wp, "%s%ld",  sep, g->info.n_child_create_tasks);
  fprintf(wp, "%s%d",   sep, g->info.worker);
  fprintf(wp, "%s%d",   sep, g->info.cpu);
  fprintf(wp, "%s%ld",  sep, g->info.start.pos.file_idx);
  fprintf(wp, "%s%ld",  sep, g->info.start.pos.line);
  fprintf(wp, "%s%ld",  sep, g->info.end.pos.file_idx);
  fprintf(wp, "%s%ld",  sep, g->info.end.pos.line);
  fprintf(wp, "\n");
  if (ferror(wp)) {
    fprintf(stderr, "%s:%d:error: fprintf %s\n", 
	    __FILE__, __LINE__, strerror(errno)); 
    return 0;
  } else {
    return 1;
  }
}

static int
dr_pi_dag_gen_text_edge(long idx, dr_pi_dag_edge * e, 
			FILE * wp, const char * sep) {
  fprintf(wp, "%ld", idx);
  fprintf(wp, "%s%lu", sep, e->u);
  fprintf(wp, "%s%lu", sep, e->v);
  fprintf(wp, "%s%s\n", sep, dr_dag_edge_kind_to_str(e->kind));
  return 1;
}

static int
dr_pi_dag_gen_text(dr_pi_dag * G, FILE * wp, const char * sep) {
  long n = G->n;
  long m = G->m;
  dr_pi_dag_node * T = G->T;
  dr_pi_dag_edge * E = G->E;
  dr_pi_string_table * S = G->S;
  const char * C = S->C;
  long * I = S->I;
  long i;
  fprintf(wp, "%ld%s%ld%s%ld\n", n, sep, m, sep, S->n);
  for (i = 0; i < n; i++) {
    if (!dr_pi_dag_gen_text_node(i, &T[i], G, wp, sep)) return 0;
  }
  for (i = 0; i < m; i++) {
    if (!dr_pi_dag_gen_text_edge(i, &E[i], wp, sep)) return 0;
  }
  for (i = 0; i < S->n; i++) {
    if (!fprintf(wp, "%ld%s%s\n", i, sep, C + I[i])) return 0;
  }
  return 1;
}

int dr_gen_text(dr_pi_dag * G) {
  if (!GS.opts.text_file_yes) {
    return 1;
  } else {
    const char * sep = GS.opts.text_file_sep;
    FILE * wp = dr_pi_dag_open_to_write(GS.opts.dag_file_prefix, ".txt",
					"text", GS.opts.verbose_level >= 1);
    if (!wp) {
      return 0;
    } else {
      int r = dr_pi_dag_gen_text(G, wp, sep);
      fclose(wp);
      return r;
    }
  }
}

#if 0
int dr_gen_nodes_edges_strings(dr_pi_dag * G) {
  FILE * wp = NULL;
  const char * nodes_file = GS.opts.nodes_file;
  const char * edges_file = GS.opts.edges_file;
  const char * strings_file = GS.opts.strings_file;
  int must_close_nodes = 0;
  int must_close_edges = 0;
  int must_close_strings = 0;
  FILE * nodes_wp = open_to_write(text_file, "nodes", &must_close_nodes);
  FILE * edges_wp = open_to_write(text_file, "edges", &must_close_edges);
  FILE * strings_wp = open_to_write(text_file, "strings", &must_close_strings);
  dr_pi_dag_gen_nodes_edges_strings(G, nodes_wp, edges_wp, strings_wp);
  if (must_close_nodes) fclose(nodes_wp);
  if (must_close_edges) fclose(edges_wp);
  if (must_close_strings) fclose(strings_wp);
  return 1;
}
#endif

