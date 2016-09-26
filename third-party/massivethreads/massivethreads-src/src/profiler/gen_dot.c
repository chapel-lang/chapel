/* 
 * gen_dot.c
 */

#include <errno.h>
#include <string.h>
#define DAG_RECORDER 2
#include "dag_recorder_impl.h"

static const char * 
dr_node_attr(dr_pi_dag_node * g) {
  switch (g->info.kind) {
  case dr_dag_node_kind_create_task:
    return "shape=\"box\",color=\"blue\"";
  case dr_dag_node_kind_wait_tasks:
    return "shape=\"box\",color=\"red\"";
  case dr_dag_node_kind_other:
    return "shape=\"box\",color=\"yellow\"";
  case dr_dag_node_kind_end_task:
    return "shape=\"box\"";
  case dr_dag_node_kind_section:
    return "shape=\"hexagon\"";
  case dr_dag_node_kind_task:
    return "shape=\"octagon\"";
  default:
    (void)dr_check(0);
  }
  return 0;
}

static const char * 
dr_edge_color(dr_dag_edge_kind_t k) {
  switch (k) {
  case dr_dag_edge_kind_end:
    return "green";
  case dr_dag_edge_kind_create:
    return "blue";
  case dr_dag_edge_kind_create_cont:
    return "pink";
  case dr_dag_edge_kind_wait_cont:
    return "black";
  case dr_dag_edge_kind_other_cont:
    return "yellow";
  default:
    (void)dr_check(0);
  }
  return 0;
}

static int
dr_pi_dag_node_gen_dot(dr_pi_dag_node * g, 
		       dr_pi_dag * G, FILE * wp) {
  dr_pi_string_table * S = G->S;
  const char * C = S->C;
  long * I = S->I;
  double running_dt = g->info.end.t - g->info.start.t;
  double ready_dt = g->info.last_start_t - g->info.first_ready_t;

  fprintf(wp, 
	  "/* node %ld : edges: %ld-%ld */\n"
	  "T%lu [%s, label=\"[%ld] %s\\n"
	  "running %llu-%llu (%llu)\\n"
	  "ready %llu-%llu (%llu)\\n"
	  "t_running %llu (%f)\\n"
	  "t_ready %llu/%llu/%llu/%llu/%llu (%f/%f/%f/%f/%f)\\n"
	  "est=%llu T=%llu/%llu,\\n"
	  "nodes=%ld/%ld/%ld/%ld,edges=%ld/%ld/%ld/%ld/%ld\\n"
	  "by %d on %d %s:%ld-%s:%ld\"];\n",
	  g - G->T, 		/* node: .. */
	  g->edges_begin, 	/* edges: .. */
	  g->edges_end,
	  
	  g - G->T,		/* T%lu */
	  dr_node_attr(g),	/* [%s, .. */
	  g - G->T,		/* label=\"...  */
	  dr_dag_node_kind_to_str(g->info.kind),
	  
	  /* "running %llu-%llu (%llu)\\n" */
	  g->info.start.t, g->info.end.t, 
	  g->info.end.t - g->info.start.t, 
	  
	  /* "ready %llu-%llu (%llu)\\n" */
	  g->info.first_ready_t, g->info.last_start_t, 
	  g->info.last_start_t - g->info.first_ready_t,
	  
	  /* "t_running %llu (%f)\\n" */
	  g->info.t_1, 
	  g->info.t_1 / running_dt,
	  
	  /* "t_ready %llu/%llu/%llu/%llu/%llu (%f/%f/%f/%f/%f)\\n" */
	  g->info.t_ready[dr_dag_edge_kind_end], 
	  g->info.t_ready[dr_dag_edge_kind_create], 
	  g->info.t_ready[dr_dag_edge_kind_create_cont], 
	  g->info.t_ready[dr_dag_edge_kind_wait_cont], 
	  g->info.t_ready[dr_dag_edge_kind_other_cont], 
	  g->info.t_ready[dr_dag_edge_kind_end]         / ready_dt,
	  g->info.t_ready[dr_dag_edge_kind_create]      / ready_dt,
	  g->info.t_ready[dr_dag_edge_kind_create_cont] / ready_dt,
	  g->info.t_ready[dr_dag_edge_kind_wait_cont]   / ready_dt,
	  g->info.t_ready[dr_dag_edge_kind_other_cont]  / ready_dt,
	  
	  /* "est=%llu T=%llu/%llu,\\n" */
	  g->info.est, 
	  g->info.t_1, 
	  g->info.t_inf,
	  
	  /* "nodes=%ld/%ld/%ld/%ld,edges=%ld/%ld/%ld/%ld/%ld\\n" */
	  g->info.logical_node_counts[dr_dag_node_kind_create_task],
	  g->info.logical_node_counts[dr_dag_node_kind_wait_tasks],
	  g->info.logical_node_counts[dr_dag_node_kind_other],
	  g->info.logical_node_counts[dr_dag_node_kind_end_task],
	  g->info.logical_edge_counts[dr_dag_edge_kind_end],
	  g->info.logical_edge_counts[dr_dag_edge_kind_create],
	  g->info.logical_edge_counts[dr_dag_edge_kind_create_cont],
	  g->info.logical_edge_counts[dr_dag_edge_kind_wait_cont],
	  g->info.logical_edge_counts[dr_dag_edge_kind_other_cont],
	  
	  /* "by %d on %d */
	  g->info.worker,
	  g->info.cpu,
	  
	  /* %s:%ld-%s:%ld\"];\n" */
	  C + I[g->info.start.pos.file_idx], 
	  g->info.start.pos.line,
	  C + I[g->info.end.pos.file_idx],   
	  g->info.end.pos.line
	  );
  if (ferror(wp)) {
    fprintf(stderr, "%s:%d:error: fprintf %s\n", 
	    __FILE__, __LINE__, strerror(errno)); 
    return 0;
  } else {
    return 1;
  }
}

static int
dr_pi_dag_edge_gen_dot(dr_pi_dag_edge * e, FILE * wp) {
  if (!fprintf(wp, "T%lu -> T%lu [color=\"%s\"];\n", 
	       e->u, e->v, dr_edge_color(e->kind))) return 0;
  return 1;
}

static int
dr_pi_dag_gen_dot(dr_pi_dag * G, FILE * wp) {
  long n = G->n;
  long m = G->m;
  dr_pi_dag_node * T = G->T;
  dr_pi_dag_edge * E = G->E;
  long i;
  fprintf(wp, "digraph G {\n");
  for (i = 0; i < n; i++) {
    dr_pi_dag_node * g = &T[i];
    if (g->info.kind < dr_dag_node_kind_section
	|| g->subgraphs_begin_offset == g->subgraphs_end_offset) {
      dr_pi_dag_node_gen_dot(g, G, wp);
    }
  }
  for (i = 0; i < m; i++) {
    if (!dr_pi_dag_edge_gen_dot(&E[i], wp)) return 0;
  }
  if (!fprintf(wp, "}\n")) return 0;
  return 1;
}

int dr_gen_dot(dr_pi_dag * G) {
  if (!GS.opts.dot_file_yes) {
    return 1;
  } else {
    FILE * wp = dr_pi_dag_open_to_write(GS.opts.dag_file_prefix, ".dot",
					"dot", GS.opts.verbose_level >= 1);
    if (!wp) {
      return 0;
    } else {
      int r = dr_pi_dag_gen_dot(G, wp);
      fclose(wp);
      return r;
    }
  }
}

