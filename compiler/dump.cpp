/*
  Copyright 2004 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#include "geysa.h"
#include "driver.h"
#include "pattern.h"
#include "internal.h"
#include "prim.h"
#include "if1.h"

#define ANON "*anon*"

static void
dump_header(FILE *fp, char *fn) {
  int relative = system_dir[0] != '/';
  fprintf(fp, "<HTML>\n");
  fprintf(fp, "<HEAD>\n");
  fprintf(fp, "<TITLE> Program Dump for %s </TITLE>\n", fn);
  fprintf(fp, "<SCRIPT SRC=\"%s%smktree.js\" LANGUAGE=\"JavaScript\"></SCRIPT>", 
	  relative ? "../" : "", system_dir);
  fprintf(fp, "<LINK REL=\"stylesheet\" HREF=\"%s%smktree.css\">", 
	  relative ? "../" : "", system_dir);
  fprintf(fp, "</HEAD>\n");
  fprintf(fp, "<div style=\"text-align: center;\"><big><big><span style=\"font-weight: bold;\">");
  fprintf(fp, "Program Dump for %s <br></span></big></big>\n", fn);
  fprintf(fp, "<div style=\"text-align: left;\">\n\n");
  fprintf(fp, "<H1>Contents</H1>\n\n");
  fprintf(fp, "<ul>\n");
  fprintf(fp, "<li><a href=\"#CONCRETE_TYPES\">Concrete Types</a>\n");
  fprintf(fp, "<li><a href=\"#CONCRETE_FUNCTIONS\">Concrete Functions</a>\n");
  fprintf(fp, "<li>Symbols</a>\n");
  fprintf(fp, "<ul>\n");
  fprintf(fp, "<li><a href=\"#FUNCTION_SYMBOLS\">Function Symbols</a>\n");
  fprintf(fp, "<li><a href=\"#GLOBALS\">Global/Module Symbols</a>\n");
  fprintf(fp, "<li><a href=\"#SYMBOLS\">Local Symbols</a>\n");
  fprintf(fp, "</ul>\n");
  fprintf(fp, "</ul>\n");
}

static void
dump_footer(FILE *fp) {
  fprintf(fp, "</HTML>\n");
}

typedef int (*sym_pred_fn)(Sym *s);

static int
is_internal_type(Sym *s) {
  return s->type_kind == Type_SUM && !s->name;
}

static void
dump_sub_sym(FILE *fp, Sym *ss, char *title) {
  if (ss) {
    fprintf(fp, "<TR><TD><TD>%s<TD><a href=\"#SYM_%d\">%s (%d)</a>",
	    title,
	    ss->id,
	    ss->name ? ss->name : ANON,
	    ss->id);
  }
}

static void
dump_sym_list(FILE *fp, Sym *s, Vec<Sym *> &l, char *title, sym_pred_fn fn = 0) {
  if (l.n) {
    Vec<Sym *> v;
    forv_Sym(ss, l) if (ss && ss != s && (!fn || !fn(ss)))
      v.add(ss);
    if (v.n) {
      dump_sub_sym(fp, v.v[0], title);
      for (int i = 1; i < v.n; i++)
	fprintf(fp, ", <a href=\"#SYM_%d\">%s (%d)</a>", 
		v.v[i]->id,
		v.v[i]->name ? v.v[i]->name : ANON,
		v.v[i]->id);
      fprintf(fp, "\n");
    }
  }
}

static void
dump_sym(FILE *fp, Sym *t) {
  fprintf(fp, "<b><A NAME=\"SYM_%d\">%s (%d)</A></b>\n", t->id, 
	  t->name ? t->name : ANON,
	  t->id);
  fprintf(fp, "<TABLE BORDER=0, CELLSPACING=0, CELLPADDING=0>\n");
#if 0
  fprintf(fp, "<TR><TD WIDTH=30><TD WIDTH=100>ID<TD>%d\n",  t->id);
  if (t->name)
    fprintf(fp, "<TR><TD><TD>Name<TD>%s\n",  t->name);
#endif
  if (t->in && t->in->name)
    fprintf(fp, "<TR><TD WIDTH=30><TD WIDTH=100>In<TD>%s %s\n", 
	    t->in->module ? "module" : (t->type_kind != Type_NONE ? "type" : "function"), 
	    t->in->name);
  else
    fprintf(fp, "<TR><TD WIDTH=30><TD WIDTH=100>In<TD>*global*\n");
  if (t->ast && t->ast->pathname)
    fprintf(fp, "<TR><TD><TD>Location<TD>%s:%d\n", t->ast->pathname, t->ast->line);
  if (t->internal)
    fprintf(fp, "<TR><TD><TD>internal<TD>%s\n", internal_string[t->internal]);
  if (t->builtin) {
    char *name = if1->builtins_names.get(t);
    fprintf(fp, "<TR><TD><TD>Builtin<TD>%s\n", name);
  }
  dump_sub_sym(fp, t->aspect, "Aspect");
  if (t->type_kind != Type_NONE)
    fprintf(fp, "<TR><TD><TD>Type Kind<TD>%s\n", type_kind_string[t->type_kind]);
  if (t->type) {
    Sym *tt = t->type;
    if (t->type == t && t->symbol)
      tt = sym_symbol;
    if (t != tt)
      dump_sub_sym(fp, tt->type, "Type");
  }
  dump_sym_list(fp, t, t->implements, "Implements", is_internal_type);
  dump_sym_list(fp, t, t->includes, "Includes");
  dump_sym_list(fp, t, t->constraints, "Constraints");
  dump_sym_list(fp, t, t->subtypes, "Subtypes");
  dump_sym_list(fp, t, t->has, "Has");
  dump_sub_sym(fp, t->self, "Self");
  dump_sub_sym(fp, t->ret, "Ret");
  dump_sub_sym(fp, t->cont, "Cont");
  dump_sub_sym(fp, t->init, "Init");
  fprintf(fp, "</TABLE><BR>\n");
}

static int
compar_sym_ids(const void *ai, const void *aj) {
  uint32 i = (*(Sym**)ai)->id;
  uint32 j = (*(Sym**)aj)->id;
  return (i > j) ? 1 : ((i < j) ? -1 : 0);
}

static int
compar_fun_ids(const void *ai, const void *aj) {
  uint32 i = (*(Fun**)ai)->id;
  uint32 j = (*(Fun**)aj)->id;
  return (i > j) ? 1 : ((i < j) ? -1 : 0);
}

static void
dump_sym_name(FILE *fp, Sym *s) {
  fprintf(fp, "<a href=\"#SYM_%d\">%s (%d)</a>", s->id, s->name ? s->name : ANON, s->id);
}

static void
dump_var_type(FILE *fp, Var *v, int i) {
  if (i)
    fprintf(fp, ", ");
  dump_sym_name(fp, v->sym);
  fprintf(fp, " : ");
  dump_sym_name(fp, v->type);
  Vec<Sym *> consts;
  if (constant_info(v, consts)) {
    fprintf(fp, " constants {");
    forv_Sym(s, consts) {
      fprintf(fp, " ");
      print(fp, s->imm, s->type);
    }
    fprintf(fp, " }");
  }
}

static void
dump_var_type_list(FILE *fp, Vec<Var *> &vars) {
  for (int i = 0; i < vars.n; i++)
    dump_var_type(fp, vars.v[i], i);
}

static void
dump_var_type_marg_positions(FILE *fp, Vec<MPosition *> &arg_positions, Map<MPosition *, Var *> &vars) {
  int i = 0;
  forv_MPosition(p, arg_positions) {
    dump_var_type(fp, vars.get(p), i);
    i++;
  }
}

static void
dump_fun_name(FILE *fp, Fun *f) {
  const char *name = f->sym->name ? f->sym->name : ANON;
  const char *sname = f->sym->in ? f->sym->in->name : "";
  if (!sname) sname = ANON;
  fprintf(fp, "<a href=\"#FUN_%d\">%s::%s (%d)</a>", f->id, sname, name, f->id);
}

static void
dump_fun_list(FILE *fp, Vec<Fun *> &funs) {
  for (int i = 0; i < funs.n; i++) {
    if (i)
      fprintf(fp, ", ");
    dump_fun_name(fp, funs.v[i]);
  }
}

static void
dump_ast_tree(FILE *fp, Fun *f, AST *a, int indent = 0) {
  switch (a->kind) {
    case AST_def_fun: return;
    case AST_const: 
    case AST_def_ident: 
      if (!a->sym->var || !a->sym->var->avars.n)
	return;
      break;
    case AST_def_type:
      if (!unalias_type(a->sym)->creators.n)
	return;
      break;
    default: break;
  }
  for (int i = 0; i < indent; i++) putc(' ', fp);
  fprintf(fp, "<LI>%s ", AST_name[a->kind]);
  if (a->sym) {
    if (a->sym->constant) {
      if (!a->sym->type->num_type)
	fprintf(fp, " constant %s", a->sym->constant);
      else {
	fprintf(fp, " constant ");
	print(fp, a->sym->imm, a->sym->type);
      }
    } else if (a->sym->symbol)
      fprintf(fp, " symbol %s", a->sym->name);
    else if (a->sym->name)
      fprintf(fp, " sym %s", a->sym->name);
    else 
      fprintf(fp, " id(%d)", a->sym->id);
  }
  Sym *s = a->sym;
  if (!s)
    s = a->rval;
  Sym *t = type_info(a, s);
  if (t) {
    fprintf(fp, " : ");
    dump_sym_name(fp, t);
  }
  if (a->string)
    fprintf(fp, " %s", a->string);
  if (a->builtin)
    fprintf(fp, " builtin %s", a->builtin);
  if (!a->sym || !a->sym->constant) {
    Vec<Sym *> consts;
    if (constant_info(a, consts, s)) {
      fprintf(fp, " constants {");
      forv_Sym(s, consts) {
	fprintf(fp, " ");
	print(fp, s->imm, s->type);
      }
      fprintf(fp, " }");
    }
  }
  Vec<Fun *> funs;
  call_info(f, a, funs);
  if (funs.n) {
    fprintf(fp, " calls ");
    dump_fun_list(fp, funs);
  }
  if (a->prim)
    fprintf(fp, " primitive %s", a->prim->name);
  fputs("\n", fp);
  if (a->n) {
    for (int i = 0; i < indent; i++) putc(' ', fp);
    fprintf(fp, "<UL>\n");
    forv_AST(aa, *a)
      dump_ast_tree(fp, f, aa, indent + 1);
    for (int i = 0; i < indent; i++) putc(' ', fp);
    fprintf(fp, "</UL>\n");
  }
}

static void
dump_functions(FILE *fp, FA *fa) {
  fprintf(fp, "<H1><A NAME=\"CONCRETE_FUNCTIONS\">Concrete Functions</A></H1>\n\n");
  Vec<Fun *> funs;
  funs.copy(fa->funs);
  qsort(funs.v, funs.n, sizeof(funs.v[0]), compar_fun_ids);
  forv_Fun(f, funs) {
    const char *name = f->sym->name ? f->sym->name : ANON;
    const char *sname = f->sym->in ? f->sym->in->name : "";
    if (!sname) sname = ANON;
    fprintf(fp, "<b><A NAME=\"FUN_%d\">%s::%s (%d)</A></b>\n", f->id, sname, name, f->id);
    fprintf(fp, "<TABLE BORDER=0, CELLSPACING=0, CELLPADDING=0>\n");
    if (f->ast && f->ast->pathname)
      fprintf(fp, "<TR><TD><TD>Location<TD>%s:%d\n", f->ast->pathname, f->ast->line);
    fprintf(fp, "<TR><TD WIDTH=30><TD WIDTH=100>Args<TD>\n");
    dump_var_type_marg_positions(fp, f->arg_positions, f->args);
    fprintf(fp, "<TR><TD><TD>Rets<TD>\n");
    dump_var_type_list(fp, f->rets);
    fprintf(fp, "<TR><TD><TD>Calls<TD>\n");
    Vec<Fun *> funs;
    for (int i = 0; i < f->calls.n; i++)
      if (f->calls.v[i].key)
	funs.set_union(*f->calls.v[i].value);
    funs.set_to_vec();
    dump_fun_list(fp, funs);
    fprintf(fp, "<TR><TD><TD>Called by<TD>\n");
    funs.clear();
    forv_CallPoint(cp, f->called)
      funs.set_add(cp->fun);
    funs.set_to_vec();
    dump_fun_list(fp, funs);
    fprintf(fp, "<TR><TD><TD>AST<TD>\n");
    fprintf(fp, "<a href=\"#\" onClick=\"expandTree('funtree%d'); return false;\">[Expand AST]</a>&nbsp;&nbsp;\n", f->id);
    fprintf(fp, "<a href=\"#\" onClick=\"collapseTree('funtree%d'); return false;\">[Collapse AST]</a>\n", f->id);
    fprintf(fp, "</TABLE>\n");
    fprintf(fp, "<UL CLASS =\"mktree\" ID=\"funtree%d\">\n", f->id);
    if (f->ast->kind == AST_def_fun)
      dump_ast_tree(fp, f, f->ast->last());
    else
      dump_ast_tree(fp, f, f->ast);
    fprintf(fp, "</UL><BR>\n");
  }
}

static void
dump_symbols(FILE *fp, FA *fa) {
  int again = 1;
  Vec<Sym *> syms, concrete_types, funs, globals, other, tmp;
  forv_CreationSet(cs, fa->css)
    concrete_types.set_add(cs->type);
  syms.copy(concrete_types);
  while (again) {
    again = 0;
    forv_Sym(s, syms) if (s) {
      if (s->type)
	again = syms.set_add(s->type) || again;
      forv_Sym(ss, s->implements)
	again = syms.set_add(ss) || again;
      forv_Sym(ss, s->includes)
	again = syms.set_add(ss) || again;
      forv_Sym(ss, s->constraints)
	again = syms.set_add(ss) || again;
      forv_Sym(ss, s->subtypes) if (ss)
	again = syms.set_add(ss) || again;
      forv_Sym(ss, s->dispatch_order) if (ss)
	again = syms.set_add(ss) || again;
      forv_Sym(ss, s->has) if (ss)
	again = syms.set_add(ss) || again;
      if (s->self)
	again = syms.set_add(s->self) || again;
      if (s->ret)
	again = syms.set_add(s->ret) || again;
      if (s->cont)
	again = syms.set_add(s->cont) || again;
      if (s->init)
	again = syms.set_add(s->init) || again;
    }
  }

  tmp.move(concrete_types);
  forv_Sym(s, tmp) if (s)
    if (!s->symbol && !s->fun)
      concrete_types.set_add(s);
  
  syms.set_difference(concrete_types, other);
  concrete_types.set_to_vec();

  syms.move(other);
  forv_EntrySet(es, fa->ess) 
    funs.set_add(es->fun->sym);
  syms.set_difference(funs, other);
  funs.set_to_vec();

  tmp.clear();
  forv_Fun(f, fa->funs) {
    forv_Var(v, f->fa_all_Vars) {
      tmp.set_add(v->sym);
    }
  }
  syms.move(other);
  syms.set_intersection(tmp);

  forv_Sym(s, syms)
    if (s && (!s->in || s->in->module))
      globals.set_add(s);
  syms.set_difference(globals, other);
  globals.set_to_vec();

  other.set_to_vec();
  qsort(concrete_types.v, concrete_types.n, sizeof(concrete_types.v[0]), compar_sym_ids);
  qsort(funs.v, funs.n, sizeof(funs.v[0]), compar_sym_ids);
  qsort(other.v, other.n, sizeof(other.v[0]), compar_sym_ids);
  qsort(globals.v, globals.n, sizeof(globals.v[0]), compar_sym_ids);
  // Concrete Types
  fprintf(fp, "<H1><A NAME=\"CONCRETE_TYPES\">Concrete Types</A></H1>\n\n");
  forv_Sym(t, concrete_types)
    if (!is_internal_type(t))
      dump_sym(fp, t);
  fprintf(fp, "\n");

  // Functions
  dump_functions(fp, fa);

  // Function Symbols
  fprintf(fp, "<H1><A NAME=\"FUNCTION_SYMBOLS\">Function Symbols</A></H1>\n\n");
  forv_Sym(t, funs)
    dump_sym(fp, t);
  fprintf(fp, "\n");
  // Globals Symbols
  fprintf(fp, "<H1><A NAME=\"GLOBALS\">Global/Module Symbols</A></H1>\n\n");
  forv_Sym(t, globals)
    dump_sym(fp, t);
  fprintf(fp, "\n");
  // Other Symbols
  fprintf(fp, "<H1><A NAME=\"SYMBOLS\">Local Symbols</A></H1>\n\n");
  forv_Sym(t, other)
    if (!is_internal_type(t))
      dump_sym(fp, t);
  fprintf(fp, "\n");
}

void 
dump_html(FA *fa, char *fn) {
  char hfn[512];
  strcpy(hfn, log_dir);
  strcat(hfn, "dump.html");
  FILE *fp = fopen(hfn, "w");
  dump_header(fp, fn);
  dump_symbols(fp, fa);
  dump_footer(fp);
  fclose(fp);
}
