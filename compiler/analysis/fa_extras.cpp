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

static void
show_sym(Sym *s, FILE *fp) {
  if (s->is_pattern) {
    fprintf(fp, "( ");
    forv_Sym(ss, s->has) {
      if (ss != s->has.v[0])
	fprintf(fp, ", ");
      show_sym(ss, fp);
    }
    fprintf(fp, ") ");
  } else if (s->name)
    fprintf(fp, "%s ", s->name);
  else if (s->constant)
    fprintf(fp, "\"%s\" ", s->constant);
  if (s->type && s->type->name)
    fprintf(fp, "= %s", s->type->name);
  else if (s->must_implement && 
	   s->must_implement == s->must_specialize)
    fprintf(fp, ": %s", s->must_implement->name);
  else if (s->must_implement)
    fprintf(fp, "< %s", s->must_implement->name);
  else if (s->must_specialize)
    fprintf(fp, "@ %s", s->must_specialize->name);
}

static void
show_fun(Fun *f, FILE *fp) {
  fprintf(fp, "%s:%d: ", f->filename(), f->line());
  forv_Sym(s, f->sym->has)
    show_sym(s, fp);
}

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
	fprintf(fp, "VAR %p\n", v->var);
    } else
      fprintf(fp, "AVAR %p\n", v);
    show_type(*v->out, fp); fprintf(fp, "\n");
    forv_AVar(vv, v->backward) if (vv) {
      if (!done.set_in(vv)) {
	todo.add(vv);
	done.set_add(vv);
      }
    }
  }
}

void
fa_dump_var_types(AVar *av, FILE *fp, int verbose = verbose_level) {
  Var *v = av->var;
  if (verbose < 2 && (!v->sym->name || v->sym->is_symbol))
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
	if (v->sym->global_scope) {
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
    if (!v->sym->is_constant && !v->sym->is_symbol)
      fa_dump_var_types(unique_AVar(v, GLOBAL_CONTOUR), fp);
}

static void
show_illegal_type(FILE *fp, ATypeViolation *v) {
  AVar *av = v->av;
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
  fprintf(stderr, "illegal: ");
  show_type(*v->type, fp);
  fprintf(stderr, "\n");
}

static void
show_call_tree(FILE *fp, PNode *p, EntrySet *es, int depth = 0) {
  depth++;
  if (depth > print_call_depth || !p->code)
    return;
  if (depth > 1 && p->code->filename()) {
    for (int x = 0; x < depth; x++)
      fprintf(stderr, " ");
    fprintf(stderr, "called from %s:%d\n", p->code->filename(), p->code->line());
  }
  AEdge **last = es->edges.last();
  for (AEdge **x = es->edges.first(); x < last; x++) if (*x)
    show_call_tree(fp, (*x)->pnode, (*x)->from, depth);
}

static void
show_call_tree(FILE *fp, AVar *av) {
  EntrySet *es = (EntrySet*)av->contour;
  AEdge **last = es->edges.last();
  for (AEdge **x = es->edges.first(); x < last; x++) if (*x)
    show_call_tree(fp, (*x)->pnode, (*x)->from, 1);
}

static int
compar_tv_pos(const void *aa, const void *bb) {
  ATypeViolation *a = (*(ATypeViolation**)aa);
  ATypeViolation *b = (*(ATypeViolation**)bb);
  AST *aast = a->send ? a->send->var->def->code->ast : 0;
  if (!aast) aast = a->av->var->sym->ast;
  AST *bast = b->send ? b->send->var->def->code->ast : 0;
  if (!bast) bast = b->av->var->sym->ast;
  if (!aast || !bast) {
    if (bast) return -1;
    if (aast) return 1;
    return 0;
  }
  if (!aast->pathname() || !bast->pathname()) {
    if (bast->pathname()) return -1;
    if (aast->pathname()) return 1;
  } else {
    int x = strcmp(aast->pathname(), bast->pathname());
    if (x) return x;
  }
  int i = aast->line();
  int j = bast->line();
  return (i > j) ? 1 : ((i < j) ? -1 : 0);
}

static void
show_violations(FA *fa, FILE *fp) {
  Vec<ATypeViolation *> vv;
  forv_ATypeViolation(v, type_violations) if (v)
    vv.add(v);
  qsort(vv.v, vv.n, sizeof(vv.v[0]), compar_tv_pos);
  forv_ATypeViolation(v, vv) if (v) {
    if (!verbose_level && !v->av->var->sym->name)
      continue;
    if (v->send)
      fprintf(stderr, "%s:%d: ", v->send->var->def->code->filename(), 
	      v->send->var->def->code->line());
    else if (v->av->var->sym->ast)
      fprintf(stderr, "%s:%d: ", v->av->var->sym->filename(), 
	      v->av->var->sym->line());
    else
      fprintf(stderr, "error: ");
    switch (v->kind) {
      default: assert(0);
      case ATypeViolation_PRIMITIVE_ARGUMENT:
	fprintf(stderr, "illegal primitive argument type ");
	show_illegal_type(fp, v);
	break;
      case ATypeViolation_SEND_ARGUMENT:
	if (v->av->var->sym->is_symbol &&
	    v->send->var->def->rvals.v[0] == v->av->var)
	  fprintf(stderr, "unresolved call '%s'\n", v->av->var->sym->name);
	else {
	  fprintf(stderr, "illegal call argument type ");
	  show_illegal_type(fp, v);
	}
	break;
      case ATypeViolation_DISPATCH_AMBIGUITY:
	fprintf(stderr, "ambiguous call '%s'\n", v->av->var->sym->name);
	fprintf(stderr, "  candidate functions:\n");
	forv_Fun(f, *v->funs) {
	  fprintf(stderr, "    ");
	  show_fun(f, stderr);
	  fprintf(stderr, "\n");
	}
	break;
      case ATypeViolation_MEMBER:
	if (v->av->out->n == 1)
	  fprintf(stderr, "unresolved member '%s'", v->av->out->v[0]->sym->name);
	else {
	  fprintf(stderr, "unresolved member\n");
	  forv_CreationSet(selector, *v->av->out)
	    fprintf(stderr, "  selector '%s'\n", selector->sym->name);
	}
	if (v->type->n == 1)
	  fprintf(stderr, "  class '%s'\n", v->type->v[0]->sym->name ? v->type->v[0]->sym->name : 
		  "<anonymous>");
	else {
	  fprintf(stderr, "  classes\n");
	  forv_CreationSet(cs, *v->type)
	    fprintf(stderr, "  class '%s'\n", cs->sym->name);
	}
	break;
      case ATypeViolation_MATCH:
	if (v->type->n == 1)
	  fprintf(stderr, "near '%s' unmatched type '%s'\n", 
		  v->av->var->sym->name ? v->av->var->sym->name : "<anonymous>", 
		  v->type->v[0]->sym->name);
	else {
	  fprintf(stderr, "near '%s' unmatched type\n");
	  forv_CreationSet(cs, *v->type)
	    fprintf(stderr, "  type '%s'\n", cs->sym->name);
	}
	break;
      case ATypeViolation_NOTYPE:
	if (v->av->var->sym->name)
	  fprintf(stderr, "'%s' ", v->av->var->sym->name);
	else
	  fprintf(stderr, "expression ");
	fprintf(stderr, "has no type\n");
	break;
    }
    if (v->send)
      show_call_tree(fp, v->send->var->def, (EntrySet*)v->send->contour);
    else if (v->av->contour_is_entry_set)
      show_call_tree(fp, v->av);
  }
}

static char *fn(char *s) {
  if (!s)
    return "<none>";
  char *filename = strrchr(s, '/');
  if (filename)
    return filename + 1;	
  return s;
}

void
log_var_types(Var *v, Fun *f) {
  if (!v->sym->name || v->sym->is_symbol)
    return;
  if (!v->sym->in)
    log(LOG_TEST_FA, "::");
  else if (v->sym->in->name)
    log(LOG_TEST_FA, "%s::", v->sym->in->name);
  else
    log(LOG_TEST_FA, "%d::", v->sym->in->id);
  if (v->sym->name)
    log(LOG_TEST_FA, "%s(%s:%d) ", v->sym->name, fn(v->sym->filename()), v->sym->line());
  else
    log(LOG_TEST_FA, "(%s:%d) ", fn(v->sym->filename()), v->sym->line());
  Vec<CreationSet *> css;
  for (int i = 0; i < v->avars.n; i++) if (v->avars.v[i].key) {
    AVar *av = v->avars.v[i].value;
    if (!f || f->ess.in(((EntrySet*)av->contour)))
      css.set_union(*av->out);
  }
  assert(css.n);
  log(LOG_TEST_FA, "( ");
  Vec<Sym *> syms;
  forv_CreationSet(cs, css) if (cs)
    syms.set_add(cs->sym);
  syms.set_to_vec();
  qsort(syms.v, syms.n, sizeof(syms.v[0]), compar_syms);
  forv_Sym(s, syms) {
    if (s->name)
      log(LOG_TEST_FA, "%s ", s->name);
    else if (s->constant)
      log(LOG_TEST_FA, "\"%s\" ", s->constant);
    log(LOG_TEST_FA, "(%s:%d) ", fn(s->filename()), s->line());
  }
  log(LOG_TEST_FA, ")\n");
}


