#include "geysa.h"
#include "driver.h"
#include "fa.h"
#include "dom.h"
#include "loop.h"
#include "graph.h"
#include "pattern.h"
#include "pdb.h"
#include "fun.h"
#include "pnode.h"
#include "if1.h"
#include "ast.h"
#include "var.h"

#define G_BOX		(1<<0)
#define G_BLUE		(1<<1)
#define G_GREEN 	(1<<2)
#define G_RED 		(1<<3)

// options to pnode print
#define G_DOM		(1<<10)
#define G_LOOP		(1<<11)

char graph_fun[80];
char graph_var[80];

static int graph_type = VCG;

static FILE *
graph_start(char *fn, char *tag, char *name) {
  char hfn[512];
  strcpy(hfn, log_dir);
  char *ffn = strrchr(fn, '/');
  if (ffn) fn = ffn;
  strcat(hfn, fn); 
  strcat(hfn, ".");
  strcat(hfn, tag);
  switch (graph_type) {
    case VCG: strcat(hfn, ".vcg"); break;
    case GraphViz: strcat(hfn, ".dot"); break;
    default: assert(!"bad case");
  }
  FILE *fp = fopen(hfn, "w");
  switch (graph_type) {
    case VCG:
      fprintf(fp, "graph: {\n");
      fprintf(fp, "\ttitle: \"%s\"\n", name);
      fprintf(fp, 
	      "\tedge.arrowsize: 15\n"
	      "\tedge.thickness: 3\n"
	);
      break;
    case GraphViz:
      fprintf(fp, "digraph G {\n");
      fprintf(fp, "edge [arraowhead=vee];\n");
      break;
    default: assert(!"bad case");
  }
  return fp;
}

static void
graph_end(FILE *fp) {
  fprintf(fp, "}\n");
  fclose(fp);
}

void
graph_node(FILE *fp, void *id, char *label, int options) {
  switch (graph_type) {
    case VCG:
      fprintf(fp, "node: {title:\"%p\" label:\"%s\"", id, label);
      if (options & G_BLUE)
	fprintf(fp, " color: blue");
      if (options & G_GREEN)
	fprintf(fp, " color: green");
      fprintf(fp, "}\n");
      break;
    case GraphViz:
      fprintf(fp, "n%p [label=\"%s\"", id, label);
      if (options & G_BOX)
	fprintf(fp, " shape=box");
      if (options & G_BLUE)
	fprintf(fp, " color=blue");
      if (options & G_GREEN)
	fprintf(fp, " color=green");
      fprintf(fp, "];\n");
      break;
    default: assert(!"bad case");
  }
}

void
graph_edge(FILE *fp, void *a, void *b, int options) {
  switch (graph_type) {
    case VCG:
      fprintf(fp, "edge: {sourcename:\"%p\" targetname:\"%p\"", a, b);
      if (options & G_BLUE)
	fprintf(fp, " color: blue");
      if (options & G_GREEN)
	fprintf(fp, " color: green");
      if (options & G_RED)
	fprintf(fp, " color: red");
      fprintf(fp, "}\n");
      break;
    case GraphViz:
      fprintf(fp, "n%p -> n%p [", a, b);
      if (options & G_BLUE)
	fprintf(fp, " color=blue");
      if (options & G_GREEN)
	fprintf(fp, " color=green");
      if (options & G_RED)
	fprintf(fp, " color=red");
      fprintf(fp, "];\n");
      break;
    default: assert(!"bad case");
  }
}

static int
compar_fun_ids(const void *ai, const void *aj) {
  uint32 i = (*(Fun**)ai)->id;
  uint32 j = (*(Fun**)aj)->id;
  return (i > j) ? 1 : ((i < j) ? -1 : 0);
}

static void 
graph_ast(Vec<Fun *> &funs, char *fn) {
  FILE *fp = graph_start(fn, "ast", "Abstract Syntax Tree");
  forv_Fun(f, funs)
    f->ast->graph(fp);
  graph_end(fp);
}

static void
strcat_sym_node(char *s, Sym *sy) {
  if (sy->name)
    strcat(s, sy->name);
  else {
    char id[80];
    sprintf(id, "_%d", sy->id);
    strcat(s, id);
  }
}

static void
graph_pnode_node(FILE *fp, PNode *pn, int options = 0) {
  char title[256] = "";
  if (pn->lvals.n) {
    forv_Var(v, pn->lvals) {
      strcat_sym_node(title, v->sym);
      strcat(title, " ");
    }
    strcat(title, "= ");
  }
  int kind = pn->code ? pn->code->kind : Code_MOVE; 
  strcat(title, code_string[kind]);
  forv_Var(v, pn->rvals) {
    strcat(title, " ");
    strcat_sym_node(title, v->sym);
  }
  if (options & G_DOM) {
    for (int i = 0; i < pn->dom->intervals.n; i += 2) {
      sprintf(title + strlen(title), "[%d %d]", 
	      pn->dom->intervals.v[i], pn->dom->intervals.v[i + 1]);
    }
  }
  if (options & G_LOOP) {
    sprintf(title + strlen(title), "C(%d %d)", 
	    pn->loop_node->pre_dfs, pn->loop_node->post_dfs);
    sprintf(title + strlen(title), "D(%d %d)", 
	    pn->loop_node->pre_dom, pn->loop_node->post_dom);
  }
  if (fgraph_frequencies)
    sprintf(title + strlen(title), "freq(%f)", pn->execution_frequency);
  graph_node(fp, pn, title);
}

static void
graph_loop_node(FILE *fp, LoopNode *n) {
  char title[256] = "";
  sprintf(title, "%d-%d", n->pre_dom, n->post_dom);
  graph_node(fp, n, title);
}

static void
graph_pnode_cfg_edges(FILE *fp, PNode *pn) {
  forv_PNode(ppn, pn->cfg_succ)
    graph_edge(fp, pn, ppn);
}

static void
graph_loop_edges(FILE *fp, LoopNode *n) {
  forv_LoopNode(nn, n->children)
    graph_edge(fp, 
	       n->node ? n->node : n, 
	       nn->node ? nn->node : nn, G_BLUE);
  forv_LoopNode(nn, n->loops)
    graph_edge(fp, 
	       n->node ? n->node : n, 
	       nn->node ? nn->node : nn, G_RED);
}

static void
graph_pnode_dom_edges(FILE *fp, PNode *pn) {
  for (int i = 0; i < pn->dom->children.n; i++)
    graph_edge(fp, pn, pn->dom->children.v[i]->node, G_BLUE);
}

static void
graph_phi_phy_edges(FILE *fp, PNode *pn) {
  forv_PNode(ppn, pn->phi)
    graph_edge(fp, pn, ppn, G_RED);
  forv_PNode(ppn, pn->phy)
    graph_edge(fp, pn, ppn, G_RED);
}

static void 
graph_cfg(Vec<Fun *> &funs, char *fn) {
  FILE *fp = graph_start(fn, "cfg", "Control Flow Graph");
  forv_Fun(f, funs) {
    Vec<PNode *> pnodes;
    f->collect_PNodes(pnodes);
    forv_PNode(p, pnodes)
      graph_pnode_node(fp, p);
    forv_PNode(p, pnodes)
      graph_pnode_cfg_edges(fp, p);
  }
  graph_end(fp);
}

static void 
graph_dom(Vec<Fun *> &funs, char *fn) {
  FILE *fp = graph_start(fn, "dom", "Dominators Graph");
  forv_Fun(f, funs) {
    Vec<PNode *> pnodes;
    f->collect_PNodes(pnodes);
    forv_PNode(p, pnodes)
      graph_pnode_node(fp, p, G_DOM);
    forv_PNode(p, pnodes) {
      graph_pnode_cfg_edges(fp, p);
      graph_pnode_dom_edges(fp, p);
    }
  }
  graph_end(fp);
}

static void 
graph_loops(Vec<Fun *> &funs, char *fn) {
  FILE *fp = graph_start(fn, "loops", "Loop Nests Graph");
  forv_Fun(f, funs) if (f->loops->loops) {
    forv_LoopNode(p, f->loops->nodes)
      if (p->node)
	graph_pnode_node(fp, (PNode*)p->node, G_LOOP);
      else
	graph_loop_node(fp, p);
    forv_LoopNode(p, f->loops->nodes) {
      if (p->node)
        graph_pnode_cfg_edges(fp, (PNode*)p->node);
      graph_loop_edges(fp, p);
    }
  }
  graph_end(fp);
}

static void
graph_var_node(FILE *fp, Var *v, int options = 0) {
  char id[80] = "";
  strcat_sym_node(id, v->sym);
  if (fgraph_constants) {
    Vec<Sym *> consts;
    if (constant_info(v, consts)) {
      strcat(id, " {");
      forv_Sym(s, consts) {
	strcat(id, " ");
	sprint(id + strlen(id), s->imm, s->type);
      }
      strcat(id, " }");
    }
  }
  graph_node(fp, v, id, options | G_BLUE);
}

static int 
graph_it(Var *v) {
  if (graph_var[0]) 
    if (!v->sym->name || strcmp(v->sym->name, graph_var))
      return 0;
  if (!fgraph_constants && (v->sym->is_constant || v->sym->is_symbol))
    return 0;	
  return 1;
}

static void
graph_pnode_var_edges(FILE *fp, PNode *pn) {
  forv_Var(v, pn->lvals)
    if (graph_it(v))
      graph_edge(fp, pn, v, G_BLUE);
  forv_Var(v, pn->rvals)
    if (graph_it(v))
      graph_edge(fp, v, pn, G_BLUE);
  if (!pn->code || pn->code->kind == Code_MOVE) {
    forv_Var(a, pn->rvals) forv_Var(b, pn->lvals)      
      if (graph_it(a) && graph_it(b))
	graph_edge(fp, a, b, G_GREEN);
  }
}

static void 
graph_ssu(Vec<Fun *> &funs, char *fn) {
  FILE *fp = graph_start(fn, "ssu", "Single-Static Use");
  Vec<Var *> vdone;
  forv_Fun(f, funs) {
    Vec<PNode *> pnodes;
    Vec<Var *> vars;
    f->collect_Vars(vars, &pnodes);
    forv_PNode(p, pnodes) {
      pnodes.append(p->phi);
      pnodes.append(p->phy);
    }
    forv_PNode(p, pnodes)
      graph_pnode_node(fp, p);
    forv_Var(v, vars) {
      if (graph_it(v))
	if (vdone.set_add(v))
	  graph_var_node(fp, v);
    }
    forv_PNode(p, pnodes) {
      graph_pnode_var_edges(fp, p);
      graph_pnode_cfg_edges(fp, p);
      graph_phi_phy_edges(fp, p);
    }
  }
  graph_end(fp);
}

static void
graph_avar_node(FILE *fp, AVar *av) {
  char label[80];
  sprintf(label, "%s_%d", av->var->sym->name ? av->var->sym->name : "",
	  av->var->sym->id);
  Vec<Sym *> consts;
  forv_CreationSet(cs, *av->out) if (cs) {
    if (cs->sym->constant)
      consts.set_add(cs->sym);
    else {
      consts.clear();
      break;
    }
  }
  consts.set_to_vec();
  if (consts.n) {
    strcat(label, " {");
    forv_Sym(s, consts) {
      strcat(label, " ");
      sprint(label + strlen(label), s->imm, s->type);
    }
    strcat(label, " }");
  }
  graph_node(fp, av, label, av->var->sym->constant ? G_BOX : 0);
}

static void 
graph_avars(FA *fa, char *fn) {
  FILE *fp = graph_start(fn, "avars", "Analysis Variables");
  Vec<AVar *> todo, todo_set;
  forv_EntrySet(es, fa->ess) {
    forv_Var(v, es->fun->fa_all_Vars) {
      AVar *av = make_AVar(v, es);
      todo_set.set_add(av);
    }
  }
  todo.copy(todo_set);
  todo.set_to_vec();
  forv_AVar(av, todo) {
    forv_AVar(avv, av->forward) if (avv)
      if (todo_set.set_add(avv))
	todo.add(avv);
    forv_AVar(avv, av->backward) if (avv)
      if (todo_set.set_add(avv))
	todo.add(avv);
  }
  forv_AVar(av, todo) {
    if (!av->forward.n && !av->backward.n)
      continue;
    graph_avar_node(fp, av);
  }
  forv_AVar(av, todo) if (av) {
    forv_AVar(avv, av->forward)
      graph_edge(fp, av, avv);
  }
  graph_end(fp);
}

static void
strcat_pattern(char *title, Sym *s) {
  strcat(title, "(");
  forv_Sym(a, s->has) {
    strcat(title, " ");
    strcat_sym_node(title, a);
  }
  strcat(title, " )");
}

static void
graph_fun_node(FILE *fp, Fun *f) {
  char title[256] = "";
  strcat_sym_node(title, f->sym);
  forv_MPosition(p, f->arg_positions) {
    Var *a = f->filtered_args.get(p);
    strcat(title, " ");
    if (!a->sym->is_pattern)
      strcat_sym_node(title, a->sym);
    else
      strcat_pattern(title, a->sym);
  }
  if (fgraph_frequencies)
    sprintf(title + strlen(title), "freq(%f)", f->execution_frequency);
  graph_node(fp, f, title);
}

static void
graph_call(FILE *fp, Fun *f, Fun *ff) {
  graph_edge(fp, f, ff);
}

static void
graph_calls(FA *fa, char *fn) {
  FILE *fp = graph_start(fn, "calls", "Call Graph");
  forv_Fun(f, fa->funs)
    graph_fun_node(fp, f);
  forv_Fun(f, fa->funs) {
    Vec<Fun *> calls;
    f->calls_funs(calls);
    forv_Fun(ff, calls)
      graph_call(fp, f, ff);
  }
  graph_end(fp);
}

static void
graph_rec(FA *fa, char *fn) {
  FILE *fp = graph_start(fn, "rec", "Recursion (Interprocedural Loops)");
  forv_LoopNode(n, fa->pdb->loops->nodes)
    if (n->node)
      graph_fun_node(fp, (Fun*)n->node);
    else
      graph_loop_node(fp, n);
  forv_LoopNode(n, fa->pdb->loops->nodes)
    if (n->node) {
      Fun *f = (Fun*)n->node;
      Vec<Fun *> calls;
      f->calls_funs(calls);
      forv_Fun(ff, calls)
	graph_call(fp, f, ff);
    } else
      graph_loop_edges(fp, n);
  graph_end(fp);
}

static void
graph_abstract_types(FA *fa, char *fn) {
  FILE *fp = graph_start(fn, "at", "Abstract Types");
  Vec<Sym *> syms;
  syms.set_union(fa->patterns->types);
  forv_Sym(s, fa->patterns->types) if (s)
    syms.set_union(s->implementors);
  forv_Sym(s, syms) if (s && s->live && !s->constant) {
    char name[256], *pname = name;
    strcpy(pname, type_kind_string[s->type_kind]);
    pname += strlen(pname);
    *pname++ = ' ';
    *pname = 0;
    if (s->is_symbol) {
      *pname++ = '#';
      strcpy(pname, s->name);
      pname += strlen(pname);
    } else if (s->is_pattern) {
      strcpy(pname, "pattern ");
      pname += strlen(pname);
      if (s->name)
	strcpy(pname, s->name);
      else
	sprintf(pname, "%d", s->id);
      pname += strlen(pname);
    } else if (s->fun) {
      pname += strlen(pname);
      if (s->name)
	strcpy(pname, s->name);
      else
	strcpy(pname, "<anonymous>");
      pname += strlen(pname);
      sprintf(pname, "%s:%d ", s->fun->filename(), s->fun->line());
      pname += strlen(pname);
    } else if (s->name) {
      strcpy(pname, s->name);
      pname += strlen(pname);
    } else {
      strcpy(pname, "<anonymous>");
      pname += strlen(pname);
    }
    graph_node(fp, s, name);
  }
  forv_Sym(s, syms) if (s && s->live && !s->constant)
    forv_Sym(ss, s->implements) if (ss && ss->live && !ss->constant)
      graph_edge(fp, s, ss);
  graph_end(fp);
}

void 
graph(FA *fa, char *fn, int agraph_type) {
  graph_type = agraph_type;
  Vec<Fun *> tfuns, funs;
  tfuns.copy(fa->funs);
  qsort(tfuns.v, tfuns.n, sizeof(tfuns.v[0]), compar_fun_ids);
  if (!graph_fun[0])
    funs.move(tfuns);
  else {
    forv_Fun(f, tfuns)
      if (f->sym->name && !strcmp(f->sym->name, graph_fun))
	funs.add(f);
  }
  graph_ast(funs, fn);
  graph_cfg(funs, fn);
  graph_ssu(funs, fn);
  graph_dom(funs, fn);
  graph_loops(funs, fn);
  graph_avars(fa, fn);
  graph_calls(fa, fn);
  graph_rec(fa, fn);
  graph_abstract_types(fa, fn);
}
