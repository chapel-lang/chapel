/*
  Copyright 2003 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/
#include "geysa.h"

static char *int_strings[] = {
  "char", "short", "int", "long long"
};

static char *uint_strings[] = {
  "unsigned char", "unsigned short", "unsigned int", "unsigned long long"
};

static char *float_strings[] = {
  "float", "double", "long double", "long long double"
};

static void
write_c_fun_proto(FILE *fp, Fun *f, int type = 0) {
  assert(f->rets.n == 1);
  fputs(f->rets.v[0]->type->cg_string, fp);
  if (type)
    fputs(" (*", fp);
  else
    fputs(" ", fp);
  if (type)
    fputs(f->cg_structural_string, fp);
  else
    fputs(f->cg_string, fp);
  if (type)
    fputs(")(", fp);
  else
    fputs("(", fp);
  for (int i = 0; i < f->args.n; i++) {
    Var *v = f->args.v[i];
    if (i)
      fputs(", ", fp);
    fputs(v->type->cg_string, fp);
    fprintf(fp, " a%d", i);
  }
  fputs(")", fp);
}

#if 0
static inline char *
c_type(Var *v) {
  if (v->type->var)
    return v->type->var->cg_string;
  else
    return v->type->cg_string;
}

static int
application_depth(PNode *n) {
  Var *v = n->rvals.v[0]->type->var;
  if (v->sym->fun)
    return 1;
  else
    return 1 + application_depth(v->def);
}

static void
write_c_apply_arg(FILE *fp, char *base, int n, int i) {
  fputs(base, fp);
  for (int j = i; j < n - 2; j++)
    fputs("->e0", fp);
  if (!i)
    fputs("->e0", fp);
  else
    fputs("->e1", fp);
}

static int
write_c_prim(FILE *fp, FA *fa, Fun *f, PNode *n) {
  switch (n->primitive->index) {
    default: return 0;
    case P_prim_tuple: {
      char *t = c_type(n->lvals.v[0]);
      fprintf(fp, "%s = (%s)GC_malloc(sizeof(*(%s)0));\n", n->lvals.v[0]->cg_string, t, t);
      for (int i = 1; i < n->rvals.n; i++)
	fprintf(fp, "%s->e%d = %s;\n", n->lvals.v[0]->cg_string, i-1, n->rvals.v[i]->cg_string);
      break;
    }
    case P_prim_vector: {
      char *t = c_type(n->rvals.v[1]);
      fprintf(fp, "%s = (%s*)GC_malloc(sizeof(%s) * %d);\n", n->lvals.v[0]->cg_string, t, t,
	      n->rvals.n - 1);
      for (int i = 1; i < n->rvals.n; i++)
	fprintf(fp, "((%s*)%s)[%d] = %s;\n", 
		t, n->lvals.v[0]->cg_string, i - 1, n->rvals.v[i]->cg_string);
      break;
    }
    case P_prim_period: {
      Sym *selector = n->rvals.v[2]->type;
      char *symbol = selector->name; assert(symbol);
      Sym *obj = n->rvals.v[0]->type;
      if (obj == sym_tuple) {
	forv_Sym(s, obj->has) {
	  PNode *pp = s->var->def;
	  for (int i = 1; i < pp->rvals.n; i++) {
	    if (symbol == pp->rvals.v[i]->sym->name) {
	      fprintf(fp, "%s = %s->e%d;\n", n->lvals.v[0]->cg_string,
		      n->rvals.v[0]->cg_string, i - 1);
	      break;
	    }
	  }
	} 
      } else {
	for (int i = 0; i < obj->has.n; i++) {
	  if (symbol == obj->has.v[i]->name) {
	    fprintf(fp, "%s = %s->e%d;\n", n->lvals.v[0]->cg_string,
		    n->rvals.v[0]->cg_string, i);
	    break;
	  }
	}
      }
      break;
    }
    case P_prim_apply: {
      int incomplete = n->lvals.n && n->lvals.v[0]->type->var &&
	n->lvals.v[0]->type->var->def == n;
      if (incomplete) {
	fprintf(fp, "Fmake_apply(%s, ", n->lvals.v[0]->cg_string);
	//write_c_type(fp, n->lvals.v[0]);
	fprintf(fp, ", %s, %s);\n", n->rvals.v[0]->cg_string, n->rvals.v[2]->cg_string);
      } else {
	if (n->lvals.n) {
	  assert(n->lvals.n == 1);
	  fprintf(fp, "%s = ", n->lvals.v[0]->cg_string);
	}
	int depth = application_depth(n);
	write_c_apply_arg(fp, n->rvals.v[0]->cg_string, depth, 0);
	fputs("((T__symbol) 0", fp);
	for (int i = 1; i < depth; i++) {
	  fputs(", ", fp);
	  write_c_apply_arg(fp, n->rvals.v[0]->cg_string, depth, i);
	}
	fputs(", ", fp);
	fputs(n->rvals.v[2]->cg_string, fp);
	fputs(");\n", fp);
      }
      break;
    }
#if 0
    case P_prim_bracket: {
      if (n->lvals.n) {
	assert(n->lvals.n == 1);
	fprintf(fp, "%s = ", n->lvals.v[0]->cg_string);
      }
      fputs("((", fp);
      write_c_type(fp, n->lvals.v[0]);
      fprintf(fp, "*)%s)", n->rvals.v[0]->cg_string);
      Sym *obj = n->rvals.v[2]->type;
      int ni;
      if (obj == sym_tuple)
	ni = obj->var->def->rvals.n -1;
      else
	ni = obj->has.n;
      for (int i = 0; i < ni; i++)
	fprintf(fp, "[%s->e%d]", n->rvals.v[2]->cg_string, i);
      fputs(";\n", fp);
    }
#endif
  }
  return 1;
}

static Fun *
get_target_fun(PNode *n, Fun *f) {
  Fun *res = 0;
  forv_EntrySet(es, f->called_ess) if (es) {
    forv_AEdge(e, es->edges) if (e) {
      if (e->send == n) {
	if (res && res != e->fun)
	  fail("unable to resolve to a single function at call site");
	res = e->fun;
      }
    }
  }
  return res;
}

static void
write_c_pnode(FILE *fp, FA *fa, Fun *f, PNode *n, Vec<PNode *> &done) {
  switch (n->code->kind) {
    case Code_MOVE:
      for (int i = 0; i < n->lvals.n; i++)
	fprintf(fp, "%s = %s;\n", n->lvals.v[i]->cg_string, n->rvals.v[i]->cg_string);
      break;
    case Code_SEND:
      if (n->primitive)
	if (write_c_prim(fp, fa, f, n))
	  break;
      if (n->lvals.n) {
	assert(n->lvals.n == 1);
	fprintf(fp, "%s = ", n->lvals.v[0]->cg_string);
      }
      if (n->primitive) {
	fprintf(fp, "_CG_%s(", n->primitive->name);
	for (int i = 0; i < n->rvals.n; i++)
	  if (i != n->primitive->pos) {
	    if (i && n->primitive->pos || i > 1)
	      fprintf(fp, ", ");
	    fputs(n->rvals.v[i]->cg_string, fp);
	  }
      } else {
	Fun *target = get_target_fun(n, f);
	fputs(target->cg_string, fp);
	fputs("(", fp);
	for (int i = 0; i < n->rvals.n; i++) {
	  if (i)
	    fprintf(fp, ", ");
	  fputs(n->rvals.v[i]->cg_string, fp);
	}
      }
      fputs(");\n", fp);
      break;
    case Code_IF:
      fprintf(fp, "if (%s) goto L%d; else goto L%d;\n", 
	      n->rvals.v[0]->cg_string,
	      n->code->label[0]->id,
	      n->code->label[1]->id);
      break;
    case Code_LABEL:
      fprintf(fp, "L%d:;\n", n->code->label[0]->id);
      break;
    case Code_GOTO:
      break;
    default: assert(!"case");
  }
  int extra_goto = n->cfg_succ.n == 1 && n->code->kind != Code_GOTO;
  forv_PNode(p, n->cfg_succ)
    if (done.set_add(p)) {
      write_c_pnode(fp, fa, f, p, done);
      extra_goto = 0;
    }
  if (extra_goto) {
    assert(n->cfg_succ.v[0]->code->kind == Code_LABEL);
    fprintf(fp, "goto L%d;\n", n->cfg_succ.v[0]->code->label[0]->id);
  }
}

static void
write_c(FILE *fp, FA *fa, Fun *f, Vec<Var *> *globals = 0) {
  fputs("\n", fp);
  write_c_fun_proto(fp, f);
  if (!f->entry) {
    fputs(" { }\n", fp);
    return;
  }
  fputs(" {\n", fp);
  int index = 0;
  Vec<Var *> vars, defs;
  f->collect_Vars(vars);
  forv_Var(v, vars) {
    if (Var_is_local(v->sym->var, f)) {
      if (v->sym->var->cg_string) 
	v->cg_string = v->sym->var->cg_string;
      else {
	char s[100];
	sprintf(s, "t%d", index++);
	if (v->sym->var->cg_string)
	  v->cg_string = v->sym->var->cg_string;
	else {
	  v->cg_string = dupstr(s);
	  v->sym->var->cg_string = v->cg_string;
	}
	if (defs.set_add(v->sym->var)) {
	  //write_c_type(fp, v);
	  fprintf(fp, " %s;\n", v->cg_string);
	}
      }
    }
  }
  if (defs.n)
    fputs("\n", fp);
  if (globals)
    forv_Var(v, *globals)
      if (v->sym->fun)
	fprintf(fp, "%s = %s;\n", v->cg_string, v->sym->fun->cg_string);
  for (int i = 0; i < f->sym->args.n; i++) {
    Sym *a = f->sym->args.v[i];
    for (int j = 0; j < a->has.n; j++)
      if (a->has.v[j]->var && !a->has.v[j]->symbol && a->has.v[j]->var->cg_string)
	fprintf(fp, "%s = a%d->e%d;\n", a->has.v[j]->var->cg_string, i, j);
  }
  Vec<PNode *> done;
  write_c_pnode(fp, fa, f, f->entry, done);
  fputs("}\n", fp);
}
#endif

static int
build_type_strings(FILE *fp, FA *fa, Vec<Var *> &globals) {
  // build builtin map
#define S(_n) if1_get_builtin(fa->pdb->if1, #_n)->cg_string = "_CG_" #_n;
#include "builtin_symbols.h"
#undef S
  // assign functions a C type string
  int f_index = 0;
  forv_Fun(f, fa->funs) {
    char s[100];
    if (f->sym->name)
      sprintf(s, "f%d/* %s(%d) */", f_index, f->sym->name, f->sym->id);
    else
      sprintf(s, "f%d", f_index);
    f->cg_string = dupstr(s);
    sprintf(s, "pf%d", f_index);
    f->cg_structural_string = dupstr(s);
    f->sym->cg_string = f->cg_structural_string;
    f_index++;
  }
  // collect all syms
  Vec<Sym *> allsyms;
  forv_Fun(f, fa->funs) {
    Vec<Var *> vars;
    f->collect_Vars(vars);
    forv_Var(v, vars) {
      if (!Var_is_local(v, f))
	globals.set_add(v);
      allsyms.set_add(v->type);
    }
  }
  // collect type has syms
  int again = 1;
  while (again) {
    again = 0;
    Vec<Sym *> loopsyms;
    loopsyms.copy(allsyms);
    for (int i = 0; i < loopsyms.n; i++) 
      if (loopsyms.v[i]) {
	forv_Sym(s, loopsyms.v[i]->has)
	  again = allsyms.set_add(s) || again;
      }
  }
  globals.set_to_vec();
  // assign creation sets C type strings
  forv_Sym(s, allsyms) if (s) {
    if (s->num_type) {
      switch (s->num_type) {
	default: assert(!"case");
	case IF1_NUM_TYPE_UINT:
	  s->cg_string = uint_strings[s->num_index];
	  break;
	case IF1_NUM_TYPE_INT:
	  s->cg_string = int_strings[s->num_index];
	  break;
	case IF1_NUM_TYPE_FLOAT:
	  s->cg_string = float_strings[s->num_index];
	  break;
      }
    } else {
      if (s->cg_string) {
	// skip
      } else {
	switch (s->type_kind) {
	  default: 
	    s->cg_string = strdup("_GC_any");
	    break;
	  case Type_RECORD: {
	    if (s->has.n) {
	      char ss[100];
	      fprintf(fp, "struct s%d;\n", s->id);
	      fprintf(fp, "typedef struct s%d *ps%d;\n", s->id, s->id);
	      sprintf(ss, "ps%d", s->id);
	      s->cg_string = dupstr(ss);
	    } else
	      s->cg_string = "_CG_null";
	    break;
	  }
	}
      }
    }
  }
  // resolve types
  forv_Sym(s, allsyms) if (s) {
    if (s->var) {
      if (!s->var->def) {
	if (s->var->sym->fun) {
	  if (!s->cg_string)
	    s->cg_string = s->var->sym->fun->cg_structural_string;
	} else if (s->type == sym_symbol)
	  s->cg_string = s->type->cg_string;
      }
    }
  }
  // define function types and prototypes
  forv_Fun(f, fa->funs) {
    write_c_fun_proto(fp, f);
    fputs(";\ntypedef ", fp);
    write_c_fun_proto(fp, f, 1);
    fputs(";\n", fp);
  }
  // define structs
  forv_Sym(s, allsyms) if (s) {
    switch (s->type_kind) {
      default: 
	break;
      case Type_RECORD: {
	if (s->has.n) {
	  fprintf(fp, "struct s%d {\n", s->id);
	  for (int i = 0; i <  s->has.n; i++) {
	    fputs("  ", fp);
	    fputs(s->has.v[i]->cg_string, fp);
	    fprintf(fp, " e%d;\n", i);
	  }
	  fprintf(fp, "};\n");
	}
      }
    }
  }
  return 0;
}

#if 0
void
cg_print_c(FILE *fp, FA *fa, Fun *init) {
  Vec<Var *> globals;
  int index = 0;
  fprintf(fp, "#include \"c_runtime.h\"\n\n");
  if (build_type_strings(fp, fa, globals) < 0)
    fail("unable to generate C code: no unique typing");
  if (globals.n)
    fputs("\n", fp);
  forv_Var(v, globals) {
    if (v->sym->constant) 
      v->cg_string = v->sym->constant;
    else if (v->sym->symbol) {
      char s[100];
      sprintf(s, "_CG_Symbol(%d, \"%s\")", v->sym->id, v->sym->name);
      v->cg_string = dupstr(s);
    } else {
      char s[100];
      sprintf(s, "g%d", index++);
      v->cg_string = dupstr(s);
      //write_c_type(fp, v);
      fputs(" ", fp);
      fputs(v->cg_string, fp);
      fputs(";\n", fp);
    }
  }
  forv_Fun(f, fa->funs)
    if (f != init)
      write_c(fp, fa, f);
  write_c(fp, fa, init, &globals);
  fprintf(fp, "\nint main(int argc, char *argv[]) {"
	  "  (void)argc; (void) argv;\n"
	  "  %s();\n"
	  "  return 0;\n"
	  "}\n", init->cg_string);
}
#endif

void
cg_write_c(FA *fa, Fun *main, char *filename) {
  FILE *fp = stdout;
  Vec<Var *> globals;
  if (build_type_strings(fp, fa, globals) < 0)
    fail("unable to generate C code: no unique typing");
/*
  FILE *fp = fopen(filename, "w");
  cg_print_c(fp, fa, main);
  fclose(fp);
*/
}
