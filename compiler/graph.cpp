/* -*-Mode: c++;-*-
 Copyright 2004 John Plevyak, All Rights Reserved, see COPYRIGHT file
nnn*/

#include "geysa.h"

#define G_BOX		(1<<0)
#define G_BLUE		(1<<2)
#define G_GREEN 	(1<<3)
#define G_RED 		(1<<4)

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
	      "\torientation: left_to_right\n"
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

static void
graph_node(FILE *fp, int id, char *label, int options = 0) {
  switch (graph_type) {
    case VCG:
      fprintf(fp, "node: {title:\"%x\" label:\"%s\"", id, label);
      if (options & G_BLUE)
	fprintf(fp, " color: blue");
      if (options & G_GREEN)
	fprintf(fp, " color: green");
      fprintf(fp, "}\n");
      break;
    case GraphViz:
      fprintf(fp, "n%x [label=\"%s\"", id, label);
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

static void
graph_edge(FILE *fp, int a, int b, int options = 0) {
  switch (graph_type) {
    case VCG:
      fprintf(fp, "edge: {sourcename:\"%x\" targetname:\"%x\"", a, b);
      if (options & G_BLUE)
	fprintf(fp, " color: blue");
      if (options & G_GREEN)
	fprintf(fp, " color: green");
      if (options & G_RED)
	fprintf(fp, " color: red");
      fprintf(fp, "}\n");
      break;
    case GraphViz:
      fprintf(fp, "n%x -> n%x [", a, b);
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
graph_it(AST *a) {
  switch (a->kind) {
    case AST_def_fun: return 0;
    case AST_const: 
    case AST_def_ident: 
      if (!a->sym->var || !a->sym->var->avars.n)
	return 0;
      break;
    case AST_def_type:
      if (!unalias_type(a->sym)->creators.n)
	return 0;
      break;
    default: break;
  }
  return 1;
}

static void
graph_ast_nodes(FILE *fp, Fun *f, AST *a) {
  graph_node(fp, (int)a, AST_name[a->kind]);
  forv_AST(aa, *a)
    if (graph_it(aa))
      graph_ast_nodes(fp, f, aa);
}

static void
graph_ast_edges(FILE *fp, Fun *f, AST *a) {
  forv_AST(aa, *a)
    if (graph_it(aa)) {
      graph_edge(fp, (int)a, (int)aa);
      graph_ast_edges(fp, f, aa);
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
  forv_Fun(f, funs) {
    AST *ast;
    if (f->ast->kind == AST_def_fun)
      ast = f->ast->last();
    else
      ast = f->ast;
    graph_ast_nodes(fp, f, ast);
    graph_ast_edges(fp, f, ast);
  }
  graph_end(fp);
}

static void
strcat_var_node(char *s, Var *v) {
  if (v->sym->name)
    strcat(s, v->sym->name);
  else {
    char id[80];
    sprintf(id, "_%d", v->sym->id);
    strcat(s, id);
  }
}

static void
graph_pnode_node(FILE *fp, PNode *pn) {
  char title[80] = "";
  if (pn->lvals.n) {
    forv_Var(v, pn->lvals) {
      strcat_var_node(title, v);
      strcat(title, " ");
    }
    strcat(title, "= ");
  }
  int kind = pn->code ? pn->code->kind : Code_MOVE; 
  strcat(title, code_string[kind]);
  forv_Var(v, pn->rvals) {
    strcat(title, " ");
    strcat_var_node(title, v);
  }
  graph_node(fp, (int)pn, title);
}

static void
graph_pnode_edges(FILE *fp, PNode *pn) {
  forv_PNode(ppn, pn->cfg_succ)
    graph_edge(fp, (int)pn, (int)ppn);
}

static void
graph_phi_phy_edges(FILE *fp, PNode *pn) {
  forv_PNode(ppn, pn->phi)
    graph_edge(fp, (int)pn, (int)ppn, G_RED);
  forv_PNode(ppn, pn->phy)
    graph_edge(fp, (int)pn, (int)ppn, G_RED);
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
      graph_pnode_edges(fp, p);
  }
  graph_end(fp);
}

static void
graph_var_node(FILE *fp, Var *v, int options = 0) {
  char id[80] = "";
  strcat_var_node(id, v);
  Vec<Sym *> consts;
  if (constant_info(v, consts)) {
    strcat(id, " {");
    forv_Sym(s, consts) {
      strcat(id, " ");
      sprint(id + strlen(id), s->imm, s->type);
    }
    strcat(id, " }");
  }
  graph_node(fp, (int)v, id, options | G_BLUE);
}

static int 
graph_it(Var *v) {
  if (graph_var[0]) 
    if (!v->sym->name || strcmp(v->sym->name, graph_var))
      return 0;
  if (!fgraph_constants && (v->sym->constant || v->sym->symbol))
    return 0;	
  return 1;
}

static void
graph_pnode_var_edges(FILE *fp, PNode *pn) {
  forv_Var(v, pn->lvals)
    if (graph_it(v))
      graph_edge(fp, (int)pn, (int)v, G_BLUE);
  forv_Var(v, pn->rvals)
    if (graph_it(v))
      graph_edge(fp, (int)v, (int)pn, G_BLUE);
  if (!pn->code || pn->code->kind == Code_MOVE) {
    forv_Var(a, pn->rvals) forv_Var(b, pn->lvals)      
      if (graph_it(a) && graph_it(b))
	graph_edge(fp, (int)a, (int)b, G_GREEN);
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
      graph_pnode_edges(fp, p);
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
  forv_CreationSet(cs, *av->out) {
    if (cs->sym->constant)
      consts.set_add(cs->sym);
    else {
      consts.clear();
      break;
    }
  }
  if (consts.n) {
    strcat(label, " {");
    forv_Sym(s, consts) {
      strcat(label, " ");
      sprint(label + strlen(label), s->imm, s->type);
    }
    strcat(label, " }");
  }
  graph_node(fp, (int)av, label, av->var->sym->constant ? G_BOX : 0);
}

static void 
graph_avars(FA *fa, char *fn) {
  FILE *fp = graph_start(fn, "avars", "Analysis Variables");
  Vec<AVar *> todo;
  int again = 1;
  forv_EntrySet(es, fa->ess) {
    forv_Var(v, es->fun->fa_all_Vars) {
      AVar *av = make_AVar(v, es);
      todo.set_add(av);
    }
  }
  while (again) {
    again = 0;
    for (int i = 0; i < todo.n; i++) {
      AVar *av = todo.v[i];
      if (!av) 
	continue;
      forv_AVar(avv, av->forward)  
	if (todo.set_add(avv))
	  again = 1;
      forv_AVar(avv, av->backward) 
	if (todo.set_add(avv))
	  again = 1;
    }
  }
  forv_AVar(av, todo) if (av) {
    if (!av->forward.n && !av->backward.n)
      continue;
    graph_avar_node(fp, av);
  }
  forv_AVar(av, todo) if (av) {
    forv_AVar(avv, av->forward)
      graph_edge(fp, (int)av, (int)avv);
  }
  graph_end(fp);
}

void 
graph(FA *fa, Fun *top, char *fn, int agraph_type) {
  (void) top;
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
  graph_avars(fa, fn);
}
