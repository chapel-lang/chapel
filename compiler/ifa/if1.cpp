#include "geysa.h"
#include "driver.h"
#include "prim.h"
#include "if1.h"
#include "builtin.h"
#include "ast.h"
#include "sym.h"

char *builtin_strings[] = {
#define S(_x) #_x,
#include "builtin_symbols.h"
#undef S
};

char *code_string[] = { "SUB", "MOVE", "SEND", "IF", "LABEL", "GOTO", "SEQ", "CONC", "NOP" };

static int mark_sym_live(Sym *s);

IF1 *if1 = 0;

IF1::IF1() {
 memset(this, 0, sizeof *this); 
 primitives = new Primitives(this);
 pointer_size = sizeof(void *);
 if1 = this;
}

Sym *
if1_register_sym(IF1 *p, Sym *sy, char *name) {
  sy->id = p->allsyms.n;
  p->allsyms.add(sy);
  if (name)
    sy->name = if1_cannonicalize_string(p, name);
  return sy;
}

Sym *
if1_const(IF1 *p, Sym *type, char *constant, Immediate *imm) {
  assert(type);
  imm->const_kind = type->num_kind;
  imm->num_index = type->num_index;
  if (!imm->const_kind) {
    if (type == sym_string)
      imm->const_kind = IF1_CONST_KIND_STRING;
    else if (type == sym_symbol) 
      imm->const_kind = IF1_CONST_KIND_SYMBOL;  
    else
      assert(!"bad const");
  }
  char *c = if1_cannonicalize_string(p, constant);
  Sym *sym = p->constants.get(imm);
  if (type)
    type = unalias_type(type);
  if (sym) {
    assert(sym->type == type);
    return sym;
  }
  sym = new_Sym();
  sym->is_constant = 1;
  sym->constant = c;
  sym->type = type;
  sym->meta_type = sym;
  sym->imm = *imm;
  p->constants.put(&sym->imm, sym);
  return sym;
}

Sym *
if1_make_symbol(IF1 *p, char *name, char *end) {
  if (end)
    name = dupstr(name, end);
  name = if1_cannonicalize_string(p, name);
  Sym *s = p->symbols.get(name);
  if (s)
    return s;
  s = new_Sym(name);
  s->name = name;
  s->type_kind = Type_PRIMITIVE;
  s->type = s;
  s->meta_type = s;
  p->symbols.put(name, s);
  s->is_symbol = 1;
  return s;
}

void
if1_set_symbols_type(IF1 *p) {
  for (int i = 0; i < p->symbols.n; i++)
    if (p->symbols.v[i].value)
      p->symbols.v[i].value->inherits_add(sym_symbol);
}

void
if1_set_builtin(IF1 *p, Sym *s, char *name, char *end) {
  if (end)
    name = dupstr(name, end);
  name = if1_cannonicalize_string(p, name);
  Sym *ss = p->builtins.get(name);
  if (ss == s)
    return;
  if (ss)
    fail("duplicate builtin '%s'", name);
  s->is_builtin = 1;
  p->builtins.put(name, s);
  p->builtins_names.put(s, name);
}

Sym *
if1_get_builtin(IF1 *p, char *name, char *end) {
  if (end)
    name = dupstr(name, end);
  name = if1_cannonicalize_string(p, name);
  return p->builtins.get(name);
}

Code *
if1_nop(IF1 *p, Code **c) {
  Code *cc = new Code(Code_NOP);
  if (c) {
    if (!*c) *c = new Code;
    (*c)->sub.add(cc);
  }
  return cc;
}

#if 0
static void
traverse(Code *c) {
  if (c->is_group()) {
    forv_Code(cc, c->sub)
      traverse(cc);
  }
}
#endif

void
if1_gen(IF1 *p, Code **c, Code *cc) {
  if (!cc)
    return;
  // traverse(cc);
  if (c) {
    if (!*c) *c = new Code;
    (*c)->sub.add(cc);
    assert(*c != cc);
  }
}

void
if1_seq(IF1 *p, Code **c, Code *cc) {
  Code *ccc;
  if (!cc)
    return;
  ccc = new Code(Code_SEQ);
  ccc->sub.add(cc);
  if (c) {
    if (!*c) *c = new Code;
    (*c)->sub.add(ccc);
  }
}

void
if1_conc(IF1 *p, Code **c, Code *cc) {
  Code *ccc;
  if (!cc)
    return;
  ccc = new Code(Code_CONC);
  ccc->sub.add(cc);
  if (c) {
    if (!*c) *c = new Code;
    (*c)->sub.add(ccc);
  }
}

Code *
if1_move(IF1 *p, Code **c, Sym *a, Sym *b, IFAAST *ast) {
  Code *cc = new Code(Code_MOVE);
  assert(a && b);
  cc->rvals.add(a);
  cc->lvals.add(b);
  cc->ast = ast;
  if (c) {
    if (!*c) *c = new Code;
    (*c)->sub.add(cc);
  }
  return cc;
}

Code *
if1_goto(IF1 *p, Code **c, Label *label) {
  Code *cc = new Code(Code_GOTO);
  cc->label[0] = label;
  if (c) {
    if (!*c) *c = new Code;
    (*c)->sub.add(cc);
  }
  return cc;
}

Label *
if1_alloc_label(IF1 *p) {
  Label *l = (Label *)MALLOC(sizeof(struct Label));
  memset(l, 0, sizeof(struct Label));
  l->id = p->alllabels.n;
  p->alllabels.add(l);
  return l;
}

Label *
if1_label(IF1 *p, Code **c, IFAAST *ast, Label *l) {
  Code *cc = new Code(Code_LABEL);
  cc->ast = ast;
  if (!l)
    cc->label[0] = if1_alloc_label(p);
  else
    cc->label[0] = l;
  if (!*c) *c = new Code;
  (*c)->sub.add(cc);
  return cc->label[0];
}

Sym *
if1_operator(IF1 *p, Code **c, Sym *a1, Sym *a2, Sym *a3) {
  Code *cc = new Code(Code_SEND);
  Sym *res;

  res = new_Sym();
  if (a1) cc->rvals.add(a1);
  if (a2) cc->rvals.add(a2);
  if (a3) cc->rvals.add(a3);
  cc->lvals.add(res);
  if (c) {
    if (!*c) *c = new Code;
    (*c)->sub.add(cc);
  }
  assert(cc->rvals.n > 0);
  return res;
}

void testme(Code *c) {
  if (!c)
    printf("hi");
}

Code *
if1_send(IF1 *p, Code **c, int args, int results, ...) {
  va_list ap;
  Sym *v;
  Code *send = new Code(Code_SEND);

  va_start(ap, results);
  for (int i = 0; i < args; i++) {
    v = va_arg(ap, Sym *);
    assert(v);
    send->rvals.add(v);
  }
  for (int i = 0; i < results; i++) {
    v = va_arg(ap, Sym *);
    assert(v);
    send->lvals.add(v);
  }
  if (c) {
    if (!*c) *c = new Code;
    (*c)->sub.add(send);
  }
  assert(send->rvals.n > 0);
  return send;
}

Code *
if1_send1(IF1 *p, Code **c) {
  Code *send = new Code(Code_SEND);
  if (c) {
    if (!*c) *c = new Code;
    (*c)->sub.add(send);
  }
  return send;
}

void if1_add_send_arg(IF1 *p, Code *c, Sym *a) { 
  assert(a);
  c->rvals.add(a); 
}

void if1_add_send_result(IF1 *p, Code *c, Sym *r) { 
  c->lvals.add(r); 
}

Code *
if1_if_goto(IF1 *p, Code **c, Sym *ifcond, IFAAST *ast) {
  Code *cc = new Code(Code_IF);
  cc->ast = ast;
  assert(ifcond);
  cc->rvals.add(ifcond);
  if (c) {
    if (!*c) *c = new Code;
    (*c)->sub.add(cc);
  }
  return cc;
}

void
if1_set_goto(IF1 *p, Code *g, Label *label) {
  assert(g->kind == Code_GOTO);
  g->label[0] = label;
}

void
if1_if_label_true(IF1 *p, Code *ifcode, Label *label, IFAAST *ast) {
  assert(ifcode->kind == Code_IF);
  ifcode->label[0] = label;
  ifcode->ast = ast;
}

void
if1_if_label_false(IF1 *p, Code *ifcode, Label *label, IFAAST *ast) {
  assert(ifcode->kind == Code_IF);
  ifcode->label[1] = label;
  ifcode->ast = ast;
}

Code *
if1_if(IF1 *p, Code **c, Code *ifcond, Sym *ifcondvar, 
       Code *ifif, Sym *if_var, Code *ifelse, Sym *else_var, Sym *r, IFAAST *ast) 
{
  Code *if_code, *if_goto;
  if1_gen(p, c, ifcond);
  if_code = if1_if_goto(p, c, ifcondvar, ast);
  if_code->ast = ast;
  if1_if_label_true(p, if_code, if1_label(p, c, ast));
  if1_gen(p, c, ifif);
  if (if_var && r)
    if1_move(p, c, if_var, r, ast);
  if (ifelse || else_var) {
    if_goto = if1_goto(p, c);
    if_goto->ast = ast;
    if1_if_label_false(p, if_code, if1_label(p, c, ast), ast);
    if1_gen(p, c, ifelse);
    if (else_var && r)
      if1_move(p, c, else_var, r, ast);
    if1_set_goto(p, if_goto, if1_label(p, c, ast));
  } else
    if1_if_label_false(p, if_code, if1_label(p, c, ast), ast);
  return if_code;
}

Code *
if1_loop(IF1 *p, Code **t, Label *cont, Label *brk, Sym *cond_var, 
         Code *before, Code *cond, Code *after, Code *body, IFAAST *ast)
{
  Code *if_goto;
  int do_while = before == body;

  if (do_while) {
    if1_label(p, t, ast, cont);
    if1_gen(p, t, body);
    if1_gen(p, t, after);
    if1_gen(p, t, cond);
    if_goto = if1_if_goto(p, t, cond_var, ast);
    if_goto->ast = ast;
    if1_if_label_true(p, if_goto, cont);
  } else {
    if1_gen(p, t, before);
    if1_label(p, t, ast, cont);
    if1_gen(p, t, cond);
    if_goto = if1_if_goto(p, t, cond_var, ast);
    if_goto->ast = ast;
    if1_if_label_true(p, if_goto, if1_label(p, t, ast));
    if1_gen(p, t, body);
    if1_gen(p, t, after);
    if1_goto(p, t, cont);
  }
  if1_if_label_false(p, if_goto, brk);
  if1_label(p, t, ast, brk);
  return if_goto;
}

Sym *
if1_closure(IF1 *p, Sym *f, Code *c, int nargs, Sym **args) {
  for (int i = 0; i < nargs; i++)
    f->has.add(args[i]);
  f->code = c;
  f->is_fun = 1;
  f->type_kind = Type_FUN;
  f->type = f;
  f->meta_type = f;
  p->allclosures.add(f);
  return f;
}

void
if1_dump_sym(FILE *fp, Sym *s) {
  if (s->name)
    fprintf(fp, "(var \"%s\" %d)",
            s->name, s->id);
  else if (s->constant)
    fprintf(fp, "(const \"%s\" %d)", (char*)s->constant, s->id);
  else
    fprintf(fp, "(temp %d)", s->id);
}

static int
mark_sym_live(Sym *s) {
  if (!s->live) {
    s->live = 1;
    if (s->type)
      mark_sym_live(s->type);
    if (s->in)
      mark_sym_live(s->in);
    if (s->type_kind) {
      forv_Sym(ss, s->implements)
        mark_sym_live(ss);
      forv_Sym(ss, s->specializes)
        mark_sym_live(ss);
      forv_Sym(ss, s->includes)
        mark_sym_live(ss);
    }
    if (s->must_specialize)
      mark_sym_live(s->must_specialize);
    if (s->must_implement)
      mark_sym_live(s->must_implement);
    if (s->is_fun || s->is_pattern || s->type_kind)
      forv_Sym(ss, s->has)
        mark_sym_live(ss);
    return 1;
  }
  return 0;
}

int
is_functional(IF1 *p, Code *code) {
  if (code->prim && !code->prim->nonfunctional)
    return 1;
  return 0;
}

static int
mark_code_live(IF1 *p, Code *code, int &code_live) {
  int changed = 0;
  if (code_live)
    code->live = 1;

  switch (code->kind) {
    case Code_GOTO:
      if (code_live) {
        if (!code->label[0]->live) {
          code->label[0]->live = 1;
          changed = 1;
        }
        code_live = 0;
      }
      break;
    case Code_IF:
      if (code_live) {
        if (!code->label[0]->live || !code->label[1]->live) {
          code->label[0]->live = 1;
          code->label[1]->live = 1;
          changed = 1;
        }
        code_live = 0;
      }
      break;
    case Code_LABEL:
      if (code->label[0]->live) {
        code_live = 1;
        code->live = 1;
      }
      break;
    case Code_MOVE: break;
    case Code_SEND: break;
    default:
      for (int i = 0; i < code->sub.n; i++)
        changed = mark_code_live(p, code->sub.v[i], code_live) || changed;
      break;
  }
  return changed;
}

static int
mark_live(IF1 *p, Code *code) {
  int changed = 0;

  if (code->live) {
    switch (code->kind) {
      case Code_GOTO:
        break;
      case Code_IF:
        changed = mark_sym_live(code->rvals.v[0]) || changed;
        break;
      case Code_MOVE:
        if (code->lvals.v[0]->live)
          changed = mark_sym_live(code->rvals.v[0]) || changed;
        break;
      case Code_SEND:
        if (!code->lvals.n || code->lvals.v[0]->live || !is_functional(p, code)) {
          for (int i = 0; i < code->rvals.n; i++)
            changed = mark_sym_live(code->rvals.v[i]) || changed;
          for (int i = 0; i < code->lvals.n; i++)
            changed = mark_sym_live(code->lvals.v[i]) || changed;
        }
        break;
      default:
        for (int i = 0; i < code->sub.n; i++)
          changed = mark_live(p, code->sub.v[i]) || changed;
        break;
    }
  }
  return changed;
}

static void
mark_dead(IF1 *p, Code *code) {
  if (!code->live)
    code->dead = 1;
  switch (code->kind) {
    case Code_LABEL:
      if (!code->label[0]->live)
        code->dead = 1;
      break;
    case Code_MOVE:
      if (!code->lvals.v[0]->live)
        code->dead = 1;
      break;
    case Code_SEND:
      if (is_functional(p, code) && !code->lvals.v[0]->live)
        code->dead = 1;
      break;
    default:
      break;
  }
  for (int i = 0; i < code->sub.n; i++)
    mark_dead(p, code->sub.v[i]);
}

static char *SPACES = "                                        ";

#define SP(_fp, _n) fputs(&SPACES[40-(_n)], _fp)

static char*Code_kind_string[] = {
  "SUB", "MOVE", "SEND", "IF", "LABEL", "GOTO", "SEQ", "CONC", "NOP"
};

void
print_code(FILE *fp, Code *code, int indent, int lf) {
  if (indent > 40)
    indent = 40;
  switch (code->kind) {
    case Code_SUB:
      for (int i = 0; i < code->sub.n; i++)
        print_code(fp, code->sub.v[i], indent, i < code->sub.n-1 ? 1 : lf);
      break;
    case Code_MOVE:
      if (!code->dead) {
        SP(fp, indent);
        fputs("(MOVE ", fp);
        if1_dump_sym(fp, code->rvals.v[0]);
        fputs(" ", fp);
        if1_dump_sym(fp, code->lvals.v[0]);
        fputs(")", fp);
      }
      break;
    case Code_SEND:
      SP(fp, indent);
      fputs("(SEND", fp);
      if (code->lvals.n) {
        fputs(" (FUTURE ", fp);
        for (int i = 0; i < code->lvals.n; i++) {
          if1_dump_sym(fp, code->lvals.v[i]);
          if (i < code->lvals.n-1) fputs(" ", fp);
        }
        fputs(")", fp);
      }
      for (int i = 0; i < code->rvals.n; i++) {
        fputs(" ", fp);
        if1_dump_sym(fp, code->rvals.v[i]);
      }
      fputs(")", fp);
      break;
    case Code_IF:
      SP(fp, indent);
      fputs("(IF ", fp);
      if1_dump_sym(fp, code->rvals.v[0]);
      fprintf(fp, " %d %d)", code->label[0]->id, code->label[1]->id);
      break;
    case Code_LABEL:
      if (!code->dead) {
        SP(fp, indent);
        fprintf(fp, "(LABEL %d)", code->label[0]->id);
      }
      break;
    case Code_GOTO:
      SP(fp, indent);
      fprintf(fp, "(GOTO %d)", code->label[0]->id);
      break;
    case Code_SEQ:
    case Code_CONC:
      SP(fp, indent);
      fprintf(fp, "(%s\n", Code_kind_string[code->kind]);
      for (int i = 0; i < code->sub.n; i++)
        print_code(fp, code->sub.v[i], indent+1, i < code->sub.n-1 ? 1 : 0);
      fputs(")", fp);
      break;
    case Code_NOP:
      SP(fp, indent);
      fputs("(NOP)", fp);
      break;
  }
  if (code->kind != Code_SUB && lf)
    fputs("\n", fp);
}

void
print_syms(FILE *fp, Vec<Sym *> *syms, int start = 0) {
  Sym *s;

  for (int i = start; i < syms->n; i++) {
    s = syms->v[i];
    if (!s->live)
      continue;
    fputs("(SYMBOL ", fp);
    if1_dump_sym(fp, s);
    if (s->type_kind)
      fprintf(fp, " :TYPE_KIND %s", type_kind_string[s->type_kind]);
    if (s->type) {
      fputs(" :TYPE ", fp);
      if1_dump_sym(fp, s->type);
    }
    if (s->must_specialize) {
      fputs(" :MUST_SPECIALIZE ", fp);
      if1_dump_sym(fp, s->must_specialize);
    }
    if (s->must_implement) {
      fputs(" :MUST_IMPLEMENT ", fp);
      if1_dump_sym(fp, s->must_implement);
    }
    if (s->is_constant) {
      if (s->type && s->constant)
        fprintf(fp, " :CONSTANT %s", (char*)s->constant);
      else {
        fprintf(fp, " :CONSTANT ");
        fprint_imm(fp, s->imm);
      }
    }
    if (s->aspect) {
      fputs(" :ASPECT ", fp);
      if1_dump_sym(fp, s->aspect);
    }
    if (s->in) {
      fputs(" :IN ", fp);
      if1_dump_sym(fp, s->in);
    }
    if (s->has.n) {
      fputs(" :HAS (", fp);
      for (int j = 0; j < s->has.n; j++) {
        if (j)
          fprintf(fp, " ");
        if1_dump_sym(fp, s->has.v[j]);
      }
      fputs(")", fp);
    }
    if (s->implements.n) {
      fputs(" :IMPLEMENTS (", fp);
      for (int j = 0; j < s->implements.n; j++) {
        if (j)
          fprintf(fp, " ");
        if1_dump_sym(fp, s->implements.v[j]);
      }
      fputs(")", fp);
    }
    if (s->specializes.n) {
      fputs(" :SPECIALZES (", fp);
      for (int j = 0; j < s->specializes.n; j++) {
        if (j)
          fprintf(fp, " ");
        if1_dump_sym(fp, s->specializes.v[j]);
      }
      fputs(")", fp);
    }
    if (s->includes.n) {
      fputs(" :INCLUDES (", fp);
      for (int j = 0; j < s->includes.n; j++) {
        if (j)
          fprintf(fp, " ");
        if1_dump_sym(fp, s->includes.v[j]);
      }
      fputs(")", fp);
    }
    if (s->ret) {
      fputs(" :RET ", fp);
      if1_dump_sym(fp, s->ret);
    }
    if (s->cont) {
      fputs(" :CONT ", fp);
      if1_dump_sym(fp, s->cont);
    }
    if (s->is_var)
      fputs(" :VAR true", fp);
    if (s->is_constant)
      fputs(" :IS_CONSTANT true", fp);
    if (s->is_external)
      fputs(" :EXTERNAL true", fp);
    if (s->global_scope)
      fputs(" :GLOBAL true", fp);
    if (s->is_meta_type)
      fputs(" :META_TYPE true", fp);
    if (s->is_value_class)
      fputs(" :VALUE true", fp);
    if (s->code) {
      fputs(" :CODE\n", fp);
      print_code(fp, s->code, 1, 0);
    }
    fputs(")\n", fp);
  }
}

static void
if1_simple_dead_code_elimination(IF1 *p) {
  for (int i = 0; i < p->allsyms.n; i++) {
    Sym *s = p->allsyms.v[i];
    if (s->global_scope)
      mark_sym_live(s);
  }
  for (int i = 0; i < p->allclosures.n; i++) {
    mark_sym_live(p->allclosures.v[i]);
    if (p->allclosures.v[i]->ret) 
      mark_sym_live(p->allclosures.v[i]->ret);
    for (int j = 0; j < p->allclosures.v[i]->has.n; j++)
      mark_sym_live(p->allclosures.v[i]->has.v[j]);
  }
  for (int i = 0; i < p->allclosures.n; i++)
    if (p->allclosures.v[i]->code) {
      int code_live = 1;
      while (mark_code_live(p, p->allclosures.v[i]->code, code_live));
    }
  int again = 1;
  while (again) {
    again = 0;
    for (int i = 0; i < p->allclosures.n; i++)
      if (p->allclosures.v[i]->code)
        again = mark_live(p, p->allclosures.v[i]->code) || again;
  }
  for (int i = 0; i < p->allclosures.n; i++) {
    if (p->allclosures.v[i]->code)
      mark_dead(p, p->allclosures.v[i]->code);
  }
}

void
if1_flatten_code(Code *c, Code_kind k, Vec<Code *> *v) {
  assert(!c->flattened);
  c->flattened = 1;
  switch (c->kind) {
    case Code_MOVE:
    case Code_SEND:
    case Code_LABEL:
    case Code_GOTO:
    case Code_IF:
      if (!c->dead)
        v->add(c);
      break;
    case Code_SUB:
    case Code_CONC:
    case Code_SEQ: 
    {
      Vec<Code *> newsub, *nv = v;
      if (!nv) nv = &newsub;
      for (int i = 0; i < c->sub.n; i++) {
        Code *cc = c->sub.v[i];
        if (cc->is_group() && cc->kind != Code_SUB && cc->kind != k) {
          if1_flatten_code(cc, c->kind, NULL);
          if (cc->sub.n) {
            if (cc->sub.n == 1)
              nv->add(cc->sub.v[1]);
            else
              nv->add(cc);
          }
        } else
          if1_flatten_code(cc, k, nv);
      }
      if (!v)
        c->sub.move(newsub);
      if (c->kind == Code_SUB)
        c->kind = k;
      break;
    }
    case Code_NOP:
      break;
  }
}

static void
find_primitives(Primitives *prim, Code *c) {
  c->prim = prim->find(c);
  forv_Code(cc, c->sub)
    find_primitives(prim, cc);
}

static void
find_primitives(IF1 *p) {
  for (int i = 0; i < p->allclosures.n; i++)
    find_primitives(p->primitives, p->allclosures.v[i]->code);
}

void
if1_finalize(IF1 *p) {
  p->top = if1_get_builtin(p, "init");
  find_primitives(p);
  if (fdce_if1)
    if1_simple_dead_code_elimination(p);
  else 
    forv_Sym(s, p->allsyms) s->live = 1;
  for (int i = 0; i < p->allclosures.n; i++) {
    Sym *f = p->allclosures.v[i];
    if (f->code)
      if1_flatten_code(f->code, Code_CONC, NULL);
  }
}

void
if1_finalize_closure(IF1 *p, Sym *c) {
  mark_sym_live(c);
  if (c->ret) 
    mark_sym_live(c->ret);
  for (int j = 0; j < c->has.n; j++)
    mark_sym_live(c->has.v[j]);
  if (c->code) {
    int code_live = 1;
    while (mark_code_live(p,c->code, code_live));
    int again = 1;
    while (again) {
      again = 0;
      again = mark_live(p, c->code) || again;
    }
    mark_dead(p, c->code);
  }
  if1_flatten_code(c->code, Code_CONC, NULL);
}

void
if1_write(FILE *fp, IF1 *p, int start) {
  print_syms(fp, &p->allsyms, start);
  fflush(fp);
}

void
if1_write_log() {
  static int start = 0;
  if (logging(LOG_IF1))
    if1_write(log_fp(LOG_IF1), if1, start);
  start = if1->allsyms.n;
}

char *
if1_cannonicalize_string(IF1 *p, char *s, char *e) {
  char *x = p->strings.cannonicalize(s, e);
  return x;
}

void
if1_dump_code(FILE *fp, Code *code, int indent) {
  if (indent > 40)
    indent = 40;
  SP(fp, indent);
  switch (code->kind) {
    case Code_MOVE:
      fputs("(MOVE ", fp);
      if1_dump_sym(fp, code->rvals.v[0]);
      fputs(" ", fp);
      if1_dump_sym(fp, code->lvals.v[0]);
      if (code->dead) fputs(" :DEAD", fp);
      fputs(")\n", fp);
      break;
    case Code_SEND:
      fputs("(SEND", fp);
      if (code->lvals.n) {
        fputs(" (FUTURE ", fp);
        for (int i = 0; i < code->lvals.n; i++) {
          if1_dump_sym(fp, code->lvals.v[i]);
          if (i < code->lvals.n-1) fputs(" ", fp);
        }
        fputs(")", fp);
      }
      for (int i = 0; i < code->rvals.n; i++) {
        fputs(" ", fp);
        if1_dump_sym(fp, code->rvals.v[i]);
      }
      fputs(")\n", fp);
      break;
    case Code_IF:
      fputs("(IF ", fp);
      if1_dump_sym(fp, code->rvals.v[0]);
      fprintf(fp, " %d %d)\n", code->label[0]->id, code->label[1]->id);
      break;
    case Code_LABEL:
      fprintf(fp, "(LABEL %d%s)\n", code->label[0]->id,
              code->dead ? " :DEAD" : "");
      break;
    case Code_GOTO:
      fprintf(fp, "(GOTO %d)\n", code->label[0]->id);
      break;
    case Code_SUB:
    case Code_SEQ:
    case Code_CONC:
      fprintf(fp, "(%s\n", Code_kind_string[code->kind]);
      for (int i = 0; i < code->sub.n; i++)
        if1_dump_code(fp, code->sub.v[i], indent+1);
      SP(fp, indent);
      fputs(")\n", fp);
      break;
    case Code_NOP:
      fputs("(NOP)\n", fp);
      break;
  }
}

void
if1_dump(FILE *fp, Code *code) {
  if1_dump_code(fp, code, 0);
}

int pp(Immediate &imm) {
  return fprint_imm(stdout, imm);
}

char *
Code::pathname() {
  if (ast)
    return ast->pathname();
  return "<unknown>";
}

char *
Code::filename() {
  char *fn = pathname();
  char *r = strrchr(fn, '/');
  if (r) return r+1; else return fn;
}

int
Code::line() {
  if (ast)
    return ast->line();
  return 0;
}

int
Code::log_line() {
  if (strstr(filename(), "prelude"))
    return 0;
  return line();
}

static char *int_type_names[IF1_INT_TYPE_NUM][2] = {
  { "bool",   0 },
  { "uint8",   "int8" },
  { "uint16",  "int16" },
  { "uint32",  "int32" },
  { "uint64",  "int64" }
};
static char *float_type_names[IF1_FLOAT_TYPE_NUM] = {
  "float32",  "float64", "float128"
};
static char *complex_type_names[IF1_FLOAT_TYPE_NUM] = {
  "complex32",  "complex64", "complex128"
};
static int float_type_sizes[IF1_FLOAT_TYPE_NUM] = {
  32, 64, 128
};

void
if1_set_primitive_types(IF1 *if1) {
  for (int signd = 0; signd < 2; signd++)
    for (int s = 0; s < IF1_INT_TYPE_NUM; s++) {
      char *tt = int_type_names[s][signd];
      if (tt) {
        Sym *ss = if1_get_builtin(if1, tt, tt+strlen(tt));
        if (!ss) fail("unable to find builtin type '%s'", tt);
        if1_set_int_type(if1, ss, signd, 8 << (s-1));
      }
    }
  for (int s = 0; s < IF1_FLOAT_TYPE_NUM; s++) {
    char *tt = float_type_names[s];
    if (tt) {
      Sym *ss = if1_get_builtin(if1, tt, tt+strlen(tt));
      if (!ss) fail("unable to find builtin type '%s'", tt);
      if1_set_float_type(if1, ss, float_type_sizes[s]);
    }
    tt = complex_type_names[s];
    if (tt) {
      Sym *ss = if1_get_builtin(if1, tt, tt+strlen(tt));
      if (!ss) fail("unable to find builtin type '%s'", tt);
      if1_set_complex_type(if1, ss, float_type_sizes[s]);
    }
  }
}



