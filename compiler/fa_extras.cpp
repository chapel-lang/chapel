/* -*-Mode: c++;-*-
 Copyright 2003-4 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

void
fa_print_backward(AVar *v, FILE *fp) {
  Vec<AVar *> done, todo;
  todo.add(v);
  done.set_add(v);
  for (int i = 0; i < todo.n; i++) {
    v = todo.v[i];
    if (v->var) {
      if (v->var->sym) {
	if (v->var->sym->name)
	  fprintf(fp, "%s %d\n", v->var->sym->name, v->var->sym->id);
	else
	  fprintf(fp, "%d\n", v->var->sym->id);
      } else
	fprintf(fp, "VAR %X\n", (int)v->var);
    } else
      fprintf(fp, "AVAR %X\n", (int)v);
    forv_AVar(vv, v->backward) {
      if (!done.set_in(vv)) {
	todo.add(vv);
	done.set_add(vv);
      }
    }
  }
}

static void
show_type(Vec<CreationSet *> &t, FILE *fp) {
  fprintf(fp, "( ");
  forv_CreationSet(cs, t) if (cs) {
    if (cs->sym->name)
      fprintf(fp, "%s ", cs->sym->name);
    else if (cs->sym->constant)
      fprintf(fp, "\"%s\" ", cs->sym->constant);
    else
      fprintf(fp, "%d ", cs->sym->id);
  }
  fprintf(fp, ") ");
}

void
fa_dump_var_types(AVar *av, FILE *fp, int verbose = verbose_level) {
  Var *v = av->var;
  if (verbose < 2 && (!v->sym->name || v->sym->type == sym_symbol))
    return;
  if (!v->sym->in)
    fprintf(fp, "::");
  else if (v->sym->in->name)
    fprintf(fp, "%s::", v->sym->in->name);
  else
    fprintf(fp, "%d::", v->sym->in->id);
  if (v->sym->name)
    fprintf(fp, "%s(%d) ", v->sym->name, v->sym->id);
  else
    fprintf(fp, "(%d) ", v->sym->id);
  if (v->sym->constant) {
    if (v->sym->constant[0] != '<')
      fprintf(fp, "\"%s\" ", v->sym->constant);
    else {
      fprintf(fp, "\"");
      print(fp, v->sym->imm, v->sym->type);
      fprintf(fp, "\" ");
    }
  }
  show_type(*av->out, fp);
  fprintf(fp, "\n");
}

void
fa_dump_types(FA *fa, FILE *fp) {
  Vec<Var *> gvars;
  forv_Fun(f, fa->funs) {
    forv_EntrySet(es, f->ess) {
      if (f->sym->name)
	fprintf(fp, "function %s (%d) ", f->sym->name, f->sym->id);
      else
	fprintf(fp, "function %d ", f->sym->id);
      fprintf(fp, "entry set with %d edges\n", es->edges.count());
      Vec<Var *> vars;
      f->collect_Vars(vars);
      forv_Var(v, vars) {
	assert(v);
	assert(v->sym);
	if (v->sym->in != f->sym) {
	  gvars.set_add(v);
	  continue;
	}
	fa_dump_var_types(make_AVar(v, es), fp);
      }
    }
  }
  gvars.set_to_vec();
  fprintf(fp, "globals\n");
  forv_Var(v, gvars)
    fa_dump_var_types(unique_AVar(v, GLOBAL_CONTOUR), fp);
}

static void
show_violations(FA *fa, FILE *fp) {
  forv_ATypeViolation(v, type_violations) if (v) {
    AVar *av = v->av;
    assert(v->pnode->code->ast);
    fprintf(stderr, "%s:%d: ", v->pnode->code->ast->pathname, v->pnode->code->ast->line);
    if (av->var->sym->name)
      fprintf(stderr, "'%s' ", av->var->sym->name);
    else if (verbose_level)
      fprintf(stderr, "expr:%d ", av->var->sym->id);
    else
      fprintf(stderr, "expression ");
    if (verbose_level) {
      fprintf(stderr, "id:%d ", av->var->sym->id);
      if (av->out->n) {
	fprintf(stderr, ": ");
	show_type(*av->out, fp);
      }
    }
    fprintf(stderr, "has illegal type: ");
    show_type(*v->type, fp);
    fprintf(fp, "\n");
  }
}

static int
show_untyped(FA *fa) {
  int res = 0;
  forv_EntrySet(es, fa->ess) {
    forv_Var(v, es->fun->fa_all_Vars) {
      if (make_AVar(v, es)->out == bottom_type) {
	res++;
	if (v->def && v->def->code && v->def->code->ast && v->def->code->ast->pathname)
	  fprintf(stderr, "%s:%d: ", v->def->code->ast->pathname, 
		  v->def->code->ast->line);
	else
	  fprintf(stderr, "error: ");
	if (v->sym->name)
	  fprintf(stderr, "'%s' ", v->sym->name);
	else
	  fprintf(stderr, "expression ");
	if (verbose_level) {
	  if (!v->sym->name)
	    fprintf(stderr, "expr:%d ", v->sym->id);
	  else
	    fprintf(stderr, "id:%d ", v->sym->id);
	}
	fprintf(stderr, "has no type\n");
      }
    }
  }
  return res;
}

