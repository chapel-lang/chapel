/*
  Copyright 2003 John Plevyak, All Rights Reserved
*/
/*
TODO
  constant folding
  prepass to generate externals, dependencies, and obsolete forward declarations
*/

%<

#include "geysa.h"

extern D_Symbol d_symbols_chpl[];
extern D_ParserTables parser_tables_chpl;

Vec<Sym *> pattern_args;
D_Scope *pattern_scope;
int assign_ops[] = {
  ${string op assign}, ${string *=}, ${string /=}, ${string %=}, ${string +=},
  ${string -=}, ${string <<=}, ${string >>=}, ${string &=}, ${string |=}, ${string ^=},
  ${string pre++}, ${string pre--}, ${string post++}, ${string post--}
};
char *is_assign_op;
int ref_ops[] = {
  ${string op period}, ${string .*}, ${string op arrow}, ${string ->*}, ${string op deref}
};
char *is_ref_op = 0;
Sym *sym_deref = 0;
char *continue_label = "continue label";
char *continue_label_end;
char *break_label = "break label";
char *break_label_end;
char *return_label = "return label";
char *return_label_end;
char *scope_start = "current scope";
char *scope_end;
char *symbol_start = "__symbol";
char *symbol_end;
char *with_start = "with scope";
char *with_end;
Sym *null_var;

#define COPY_PN(_t, _s) do {				\
  (_t).sym = (_s).sym;					\
  (_t).d_sym_list.head = (_s).d_sym_list.head;		\
  (_t).sym_list.head = (_s).sym_list.head;		\
  (_t).rval = (_s).rval;				\
  (_t).lval = (_s).lval;				\
  (_t).type = (_s).type;				\
  (_t).saved_scope = (_s).saved_scope;			\
  (_t).in_tuple = (_s).in_tuple;			\
} while (0)

#define COPY_PN_IF1(_i, _t, _s) do {			\
  COPY_PN(_t, _s); 					\
  if1_gen(_i, &(_t).if1, (_s).if1);			\
} while (0)

static void
set_in(D_Scope *scope, Sym *s) {
  D_Sym *sscope = NEW_D_SYM(scope, scope_start, scope_end);
  sscope->user.var = s;
  sscope->user.internal = 1;
}

void
sym_set_in(Sym *a, Sym *in) {
  if (!a->constant && !a->symbol) {
    a->in = in;
    forv_Sym(s, a->has)
      sym_set_in(s, in);
  }
}

D_Sym *
new_sym(IF1 *i, char *s, char *e, D_Scope *scope, Code **c) {
  D_Sym *sym = NEW_D_SYM(scope, s, e);
  if (s)
    sym->user.var = if1_alloc_var(i, s, e);
  else
    sym->user.var = if1_alloc_sym(i);
  if (scope) {
    sym->user.label = if1_label(i, c);
    D_Sym *ss = find_D_Sym(scope, scope_start, scope_end);
    if (ss)
      sym->user.var->in = ss->user.var;
  }
  return sym;
}

Sym *
new_temp(IF1 *if1, D_Scope *scope) {
  D_Sym *ss;
  Sym *s = if1_alloc_sym(if1);
  ss = find_D_Sym(scope, scope_start, scope_end);
  s->in = ss->user.var == sym_system ? sym_init : ss->user.var;
  return s;
}

D_Sym *
new_fun(IF1 *i, char *s, char *e, D_Scope *in_scope, D_Scope **scope, D_Scope **saved_scope, Code **c) {
  D_Scope *original_scope = *scope;
  D_Sym *next_sym = find_D_Sym(original_scope, s, e);
  D_Sym *sym = new_sym(i, s, e, *scope, c), *sreturn;
  sym->user.var->next_sym = next_sym ? next_sym->user.var : 0;
  *saved_scope = *scope;
  *scope = new_D_Scope(*scope);
  set_in(*scope, sym->user.var);
  if (in_scope != original_scope)
    *scope = scope_D_Scope(*scope, in_scope);
  sreturn = NEW_D_SYM(*scope, return_label, return_label_end);
  sreturn->user.label = if1_alloc_label(i);
  sreturn->user.internal = 1;
  sreturn->user.var = new_temp(i, *scope);
  sym->user.var->ret = new_temp(i, *scope);
  return sym;
}

Sym *
constant_type(IF1 *i, D_ParseNode &pn) {
  D_ParseNode *child = d_get_child(&pn, 0);
  while (d_symbols_chpl[child->symbol].kind != D_SYMBOL_REGEX && 
	 d_symbols_chpl[child->symbol].kind != D_SYMBOL_NTERM)
    child = d_get_child(child, 0);
  return if1_get_builtin(i, d_symbols_chpl[child->symbol].name);
}

D_Sym *
new_constant(IF1 *i, D_ParseNode &pn) {
  D_Sym *sym = NEW_D_SYM(0, 0, 0);
  char *e = pn.end;
  while (e > pn.start_loc.s && isspace(e[-1])) e--;
  char *s = pn.start_loc.s;
  if (s + 1 < e && s[0] == '#') {
    if (s[1] == '"') {
      s += 2;
      e--;
    } else
      s++;
    sym->user.var = if1_make_symbol(i, s, e);
    return sym;
  }
  sym->user.var = if1_const(i, constant_type(i, pn), dupstr(s, e));
  return sym;
}

D_Scope *
scope_list(D_Scope *original_scope, List<D_Sym *> &l) {
  D_Scope *scope = original_scope;
  forc_List(D_Sym*, s, l) {
    if (!s->car)
      return scope;
    D_Scope *current_scope = current_D_Sym(original_scope, s->car)->user.sym_scope;
    if (current_scope)
      return scope;
    scope = scope_D_Scope(scope, current_scope);
  }
  return scope;
}

void
build_has_from_scope(Sym *s, D_Scope *scope) {
  D_Sym *sym = NULL;
  while (next_D_Sym_in_Scope(&scope, &sym))
    if (!sym->user.internal)
      s->has.add(sym->user.var);
}

void
build_args_from_list(Sym *s, List<D_Sym *> &l) {
  forc_List(D_Sym*, sym, l)
    s->has.add(sym->car->user.var);
}

Sym *
new_range_type(IF1 *i, Sym *index, Sym *range) {
  Sym *s = if1_alloc_sym(i);
  s->type_kind = Type_RANGE;
  if (index)
    s->has.add(index);
  else
    s->has.add(if1_const(i, range->type, "0"));
  s->has.add(range);
  return s;
}

Sym *
new_vector_type(IF1 *i) {
  Sym *s = if1_alloc_sym(i);
  s->type_kind = Type_VECTOR;
  s->has.add(0);		// element type
  return s;
}

Sym *
new_sum_type(IF1 *i) {
  Sym *s = if1_alloc_sym(i);
  s->type_kind = Type_SUM;
  return s;
}

Sym *
new_enum_element(IF1 *i, char *start, char *end) {
  Sym *s = if1_alloc_var(i, start, end);
  s->type_kind = Type_CONST;
  s->type = sym_enum_element;
  return s;
}

Sym *
new_ref_type(IF1 *i, Sym *s) {
  Sym *sym = if1_alloc_sym(i);
  sym->type_kind = Type_REF;
  sym->has.add(s);
  return sym;
}

Sym *
new_type(IF1 *i, Type_kind kind, Sym *s = 0, D_ParseNode *pn = 0) {
  Sym *sym = if1_alloc_sym(i);
  sym->type_kind = kind;
  if (s)
    sym->has.add(s);
  if (pn) {
    pn = d_get_child(pn, 0);
    for (int i = 0; i < d_get_number_of_children(pn); i+= 2)
      sym->has.add(d_get_child(pn, i + 1)->user.type);
  }
  return sym;
}

Sym *
new_tagged_type(IF1 *i, char *s, char *e, Sym *t) {
  Sym *sym = if1_alloc_sym(i);
  sym->type_kind = Type_PRODUCT;
  sym->has.add(if1_make_symbol(i, s, e));
  sym->has.add(t);
  return sym;
}

Sym *
new_parametized_type(IF1 *i, Sym *t, List<Sym*>) {
  return t;
}

void
type_check(Globals *g, D_ParseNode &pn, Sym *v, Sym *t) {
#if 0
  g->errors += show_error("type check failure '%s'", 
			  pn.start_loc, v->name ? v->name "<anonymous>");
#endif
  if (!v->type)
    v->type = t;
}

void 
initialize_parser() {
  continue_label_end = continue_label + strlen(continue_label);
  break_label_end = break_label + strlen(break_label);
  return_label_end = return_label + strlen(return_label);
  scope_end = scope_start + strlen(scope_start);
  symbol_end = symbol_start + strlen(symbol_start);
  with_end = with_start + strlen(with_start);
  is_assign_op = (char*)MALLOC(parser_tables_chpl.nsymbols);
  memset(is_assign_op, 0, parser_tables_chpl.nsymbols);
  for (int j = 0; j < (int)(sizeof(assign_ops)/sizeof(assign_ops[0])); j++)
    is_assign_op[assign_ops[j]] = 1;
  is_ref_op = (char*)MALLOC(parser_tables_chpl.nsymbols);
  memset(is_ref_op, 0, parser_tables_chpl.nsymbols);
  for (int j = 0; j < (int)(sizeof(ref_ops)/sizeof(ref_ops[0])); j++)
    is_ref_op[ref_ops[j]] = 1;
}

void
initialize_IF1(IF1 *i) {
  sym_deref = if1_make_symbol(i, "*", 0);
}

void
build_function(Globals *g, D_ParseNode &pn) {
  D_ParseNode *fn_pn = d_get_child(&pn, 0);
  D_ParseNode *expr_pn = d_get_child(&pn, 1);
  Sym *fn = fn_pn->user.sym->user.var;
  D_Scope *fn_scope = fn_pn->scope;
  D_Sym *return_sym = find_D_Sym(fn_scope, return_label, return_label_end);
  Sym *ret = fn->ret;
  Code *body = NULL;
  if1_gen(g->i, &body, expr_pn->user.if1);
  if (expr_pn->user.rval)
    if1_move(g->i, &body, expr_pn->user.rval, return_sym->user.var);
  if1_label1(g->i, &body, return_sym->user.label);
  if1_send(g->i, &body, 3, 0, sym_reply, ret, return_sym->user.var);
  D_ParseNode *args = d_get_child(fn_pn, 1);
  int n = d_get_number_of_children(args);
  Sym **as = (Sym **)alloca((n + 1) * sizeof(Sym *));
  as[0] = if1_make_symbol(g->i, fn->name, 0);
  for (int i = 0; i < n; i++) {
    as[i + 1] = d_get_child(args, i)->user.rval;
    sym_set_in(as[i + 1], fn);
  }
  if1_closure(g->i, fn, body, n + 1, as, ret);
  fn->type = sym_function;
  pn.user.rval = new_temp(g->i, pn.scope);
  if1_move(g->i, &pn.user.if1, fn, pn.user.rval);
}

int
get_tuple_args(IF1 *i, Code *send, D_ParseNode *pn) {
  D_ParseNode *ppn;
  if (pn->symbol != ${nterm expression} || d_get_number_of_children(pn) != 3)
    goto Lreturn;
  if (d_get_number_of_children(pn) != 3)
    goto Lreturn;
  ppn = d_get_child(pn, 1);
  if (ppn->symbol == ${nterm binary_operator} && d_get_child(ppn, 0)->symbol == ${string op comma}) {
    int r = get_tuple_args(i, send, d_get_child(pn, 0));
    if1_add_send_arg(i, send, d_get_child(pn, 2)->user.rval);
    return 1 + r;
  }
 Lreturn:
  if1_add_send_arg(i, send, pn->user.rval);
  return 1;
}

int
is_apply(D_ParseNode *pn) {
  if (pn->symbol == ${nterm binary_operator}) {
    D_ParseNode *child = d_get_child(pn, 0);
    if (!child)
      return 1;
    return child->symbol == ${string op apply} || child->start_loc.s == child->end;
  }
  return 0;
}

int
get_apply_args(IF1 *i, Code *send, D_ParseNode *pn) {
  D_ParseNode *ppn;
  assert(pn->symbol == ${nterm expression});
  if (d_get_number_of_children(pn) != 3)
    goto Lreturn;
  ppn = d_get_child(pn, 1);
  if (is_apply(ppn)) {
    int r = get_apply_args(i, send, d_get_child(pn, 0));
    if1_add_send_arg(i, send, d_get_child(pn, 2)->user.rval);
    return 1 + r;
  }
 Lreturn:
  if1_add_send_arg(i, send, pn->user.rval);
  return 1;
}

void
build_operator(IF1 *i, D_ParseNode &pn, D_ParseNode *a0, D_ParseNode &op, D_ParseNode *a1) {
  D_ParseNode *pop = d_get_child(&op, 0);
  while (pop && d_symbols_chpl[pop->symbol].kind != D_SYMBOL_STRING)
    pop = d_get_child(pop, 0);
  int assign = pop ? is_assign_op[pop->symbol] : 0;
  int lval = pop ? is_ref_op[pop->symbol] : 0;
  Sym *op_sym = if1_make_symbol(i, pop ? pop->start_loc.s : (char*)"^^", pop ? pop->end : 0);
  Code **c = &pn.user.if1;
  int binary = a0 && a1;
  pn.user.rval = new_temp(i, pn.scope);
  if (a0) if1_gen(i, c, a0->user.if1);
  if (a1) if1_gen(i, c, a1->user.if1);
  assert(sym_make_tuple && sym_primitive);
  if (*op_sym->name == ',') {
    if (pn.user.in_tuple)
      return;
    Code *send = if1_send1(i, c);
    Sym *constructor = sym_make_tuple;
    switch (pn.scope->kind) {
      default: break;
      case SCOPE_PARALLEL: constructor = sym_make_vector; break;
      case SCOPE_RECURSIVE: constructor = sym_make_set; break;
    }
    if1_add_send_arg(i, send, constructor);
    get_tuple_args(i, send, a0);
    if1_add_send_arg(i, send, a1->user.rval);
    if1_add_send_result(i, send, pn.user.rval);
  } else if (*op_sym->name == '^' && op_sym->name[1] == '^') {
    assert(binary);
    if (pn.user.in_apply)
      return;
    Code *send = if1_send1(i, c);
    get_apply_args(i, send, a0);
    if1_add_send_arg(i, send, a1->user.rval);
    if1_add_send_result(i, send, pn.user.rval);
  } else if (a0->user.rval != sym_primitive) {
    Sym *args = new_temp(i, pn.scope);
    Sym *res = lval ? new_temp(i, pn.scope) : pn.user.rval;
    if (binary)
      if1_send(i, c, 4, 1, sym_make_tuple, a0->user.rval, op_sym, a1->user.rval, args);
    else if (a0)
      if1_send(i, c, 3, 1, sym_make_tuple, a0->user.rval, op_sym, args);
    else
      if1_send(i, c, 3, 1, sym_make_tuple, op_sym, a1->user.rval, args);
    if1_send(i, c, 2, 1, sym_operator, args, res);
    if (lval)
      if1_send(i, c, 3, 1, sym_primitive, sym_deref, res, pn.user.rval);
    if (assign) {
      if (a0)
	if1_move(i, c, pn.user.rval, a0->user.lval);
      else
	if1_move(i, c, pn.user.rval, a1->user.lval);
    }
  } else {
    assert(binary && *op_sym->name == '^');
    if1_send(i, c, 2, 1, sym_primitive, a1->user.rval, pn.user.rval);
  }
}

D_Scope *
new_loop_scope(IF1 *i, D_Scope *scope) {
  int kind = scope->kind;
  scope = new_D_Scope(scope);
  scope->kind = kind;
  D_Sym *s = NEW_D_SYM(scope, continue_label, continue_label_end);
  s->user.label = if1_alloc_label(i);
  s = NEW_D_SYM(scope, break_label, break_label_end);
  s->user.label = if1_alloc_label(i);
  return scope;
}

Sym *
build_loop(IF1 *i, D_Scope *scope, D_ParseNode &cond, D_ParseNode *before, D_ParseNode *after, 
	   D_ParseNode &body, Code **c) 
{
  Label *lc = find_D_Sym(scope, continue_label, continue_label_end)->user.label;
  Label *lb = find_D_Sym(scope, break_label, break_label_end)->user.label;
  if1_loop(i, c, lc, lb, 
	   cond.user.rval, before ? before->user.if1 : 0, 
	   cond.user.if1, after ? after->user.if1 : 0,
	   body.user.if1);
  return body.user.rval;
}

void 
scope_type(Globals *g, D_Scope *scope, D_ParseNode *pn) {
  char kind = *d_get_child(pn, 0)->start_loc.s;
  D_ParseNode *expr = d_get_child(pn, 3);
  D_ParseNode *stmts = d_get_child(pn, 2);
  if (!d_get_number_of_children(expr)) {
    if (d_get_number_of_children(stmts)) {
      pn->user.type = if1_alloc_sym(g->i);
      switch (kind) {
	default: assert(!"case");
	case '{': pn->user.type->type_kind = Type_RECORD; break;
	case '[': pn->user.type->type_kind = Type_VECTOR; break;
	case '(': pn->user.type->type_kind = Type_PRODUCT; break;
      }
      if (kind == '{') {
	D_Sym *sym = NULL;
	while (next_D_Sym_in_Scope(&scope, &sym))
	  if (!sym->user.internal)
	    pn->user.type->has.add(sym->user.var);
      } else {
	Sym *vector_type = 0;
	for (int i = 0; i < d_get_number_of_children(stmts); i++) {
	  D_ParseNode *n = d_get_child(stmts, i);
	  if (kind == '(')
	    pn->user.type->has.add(n->user.type);
	  else {
	    if (!vector_type) {
	      vector_type = n->user.type;
	      assert(vector_type);
	    } else {
	      if ((vector_type->num_type && !n->user.type->num_type) ||
		  (!vector_type->num_type && n->user.type->num_type))
		show_error("mixed numeric non-numeric vector", pn->start_loc);
	      vector_type = coerce_type(g->i, vector_type, n->user.type);
	    }
	  }
	}
	if (vector_type)
	  pn->user.type->has.add(vector_type);
      }
    } else {
      switch (kind) {
	default: assert(!"case");
	case '{': pn->user.type = sym_set; break;
	case '[': pn->user.type = sym_vector; break;
	case '(': pn->user.type = sym_void; break;
      }
    }
  } else {
    // single element
    switch (kind) {
      default: assert(!"case");
      case '{': pn->user.type = sym_set; break;
      case '[': pn->user.type = sym_vector; break;
      case '(': pn->user.type = expr->user.type; break;
    }
  }
}

void 
build_scope(Globals *g, D_Scope *current_scope, D_Scope *scope, D_ParseNode *pn) {
  char kind = *d_get_child(pn, 0)->start_loc.s;
  Sym *constructor = 0;
  D_ParseNode *stmts = d_get_child(pn, 2);
  D_ParseNode *expr = d_get_child(pn, 3);
  if1_gen(g->i, &pn->user.if1, stmts->user.if1);
  if1_gen(g->i, &pn->user.if1, expr->user.if1);
  if (d_get_number_of_children(expr)) {
    switch (kind) {
      default: assert(!"case");
      case '{': constructor = sym_make_set; break;
      case '[': constructor = sym_make_vector; break;
      case '(': pn->user.rval = expr->user.rval; break;
    }
    if (constructor) {
      pn->user.rval = new_temp(g->i, scope);
      Code *send = if1_send1(g->i, &pn->user.if1);
      if1_add_send_arg(g->i, send, constructor);
      get_tuple_args(g->i, send, expr);
      if1_add_send_result(g->i, send, pn->user.rval);
    }
  } else {
    Code *send = if1_send1(g->i, &pn->user.if1);
    switch (kind) {
      default: assert(!"case");
      case '{': constructor = sym_make_tuple; break;
      case '[': constructor = sym_make_vector;; break;
      case '(': constructor = sym_make_list;; break;
    }
    assert(constructor);
    pn->user.rval = new_temp(g->i, scope);
    if1_add_send_result(g->i, send, pn->user.rval);
    if1_add_send_arg(g->i, send, constructor);
    for (int i = 0; i < d_get_number_of_children(stmts); i++)
      if1_add_send_arg(g->i, send, d_get_child(stmts, i)->user.rval);
  }
}

void
set_builtin(IF1 *i, Sym *sym, char *start, char *end) {
  if1_set_builtin(i, sym, start, end);
  int x = 0;
  for (; builtin_strings[x]; x++)
    if ((int)strlen(builtin_strings[x]) == (int)(end-start) && !strncmp(builtin_strings[x], start, end-start))
      goto Lfound;
  fail("builtin not found '%s'", dupstr(start, end));
 Lfound:
  switch (x) {
#define S(_n) case Builtin_##_n: sym_##_n = sym; break;
#include "builtin_symbols.h"
#undef S
    default: assert(!"bad case");
  }
  switch (x) {
    default: break;
    case Builtin_void: sym->type_kind = Type_PRODUCT; break;
    case Builtin_symbol: if1_set_symbols_type(i); break;
  }
}

static D_Scope *
enter_module(IF1 *i, D_Scope *scope, char *s, char *e, D_ParseNode &name) {
  D_Sym *sym = NEW_D_SYM(scope, s, e);
  sym->user.var = if1_alloc_var(i, s, e);
  sym->user.var->in = 0;
  scope = enter_D_Scope(scope, scope);
  set_in(scope, sym->user.var);
  if (d_get_number_of_children(&name)) {
    D_ParseNode *nn = d_get_child(&name, 0);
    nn = d_get_child(nn, 1);
    set_builtin(i, sym->user.var, nn->start_loc.s+1, nn->end-1);
  }
  return scope;
}

%>

${declare tokenize}
${pass if1 for_all postorder}
${pass pre for_all postorder}
${pass pattern for_all postorder}

program : [ ${scope}->kind = SCOPE_RECURSIVE; ] top_level_statement* {
  int n = d_get_number_of_children(&$n1);
  for (int i = 0; i < n; i++)
    if1_gen($g->i, &$$.if1, d_get_child(&$n1, i)->user.if1);
  if (n)
    $$.rval = d_get_child(&$n1, n-1)->user.rval;
};

top_level_statement: some_top_level_statement
  [ ${scope} = commit_D_Scope(${scope}); ]
{ 
  d_pass(${parser}, &$n, ${pass pre}); 
  if (!$g->errors)
    d_pass(${parser}, &$n, ${pass if1}); 
};

some_top_level_statement
  : module_statement 
//  | c_extern_statement
  | statement
  ;

// c_extern_statement: 'extern' c_declarator ';' ; // see c.g
  
module_statement 
  : 'in' identifier ('__name' string)? ';'
    [ ${scope} = enter_module($g->i, ${scope}, $n1.start_loc.s, $n1.end, $n2); ]
  | 'use' identifier ';'
    [ assert(!"supported"); ]
  | 'export' module_expression (',' module_expression)* ';'
    [ assert(!"supported"); ] 
  | 'import' module_expression (',' module_expression)* ';'
    [ assert(!"supported"); ]
 ;
module_expression: module_identifier ('as' module_identifier)?;
module_identifier: qualified_identifier ('.' identifier)*;

statement : () some_statement ';'
  [
    if ($n0.scope->kind == SCOPE_PARALLEL)
      ${scope} = enter_D_Scope(${scope}, $g->parallel_scope);
    COPY_PN($$, $1);
  ]
pre: { COPY_PN($$, $1); }
if1: { COPY_PN_IF1($g->i, $$, $1); }
  | ';'
  ;

some_statement
  : type_statement where_statement?  [ COPY_PN($$, $0); ] if1: { COPY_PN_IF1($g->i, $$, $0); }
  | var_statement
  | function_statement 
  | where_statement
  | expression [ COPY_PN($$, $0); ] if1: { COPY_PN_IF1($g->i, $$, $0); }
  | control_flow [ COPY_PN($$, $0); ] if1: { COPY_PN_IF1($g->i, $$, $0); }
  ;

type_statement
  : 'type' type_definition (',' type_definition)* 
  | ('class' | 'struct') def_type def_type_parameter_list? class_definition? 
[ 
  ${scope} = enter_D_Scope(${scope}, $1.saved_scope); 
  build_has_from_scope($1.sym->user.var, $3.saved_scope);
  $1.sym->user.sym_scope = $3.saved_scope;
  $1.sym->user.var->type_kind = Type_RECORD;
  if (${child 0, 0}->symbol == ${string struct})
    $1.sym->user.var->structure = 1;
]
pre: {
  if ($#2)
    build_args_from_list($1.sym->user.var, ${child 2, 0}->user.d_sym_list);
}
  | 'enum' def_type enum_definition? 
[ 
  ${scope} = enter_D_Scope(${scope}, $1.saved_scope); 
  build_has_from_scope($1.sym->user.var, $2.saved_scope);
  $1.sym->user.sym_scope = $2.saved_scope;
];

type_definition : def_type def_type_parameter_list? ('__name' string)? (':' super_types)? ('=' type)? 
[ 
  ${scope} = enter_D_Scope(${scope}, $0.saved_scope);
  if ($#4)
    $0.sym->user.sym_scope = ${child 4, 0, 1}->user.saved_scope;
]
pre: {
  printf("type '%s' %d\n", dupstr($n0.start_loc.s, $n0.end), $n0.start_loc.line);
  $$.type = $0.sym->user.var;
  if ($#1)
    build_args_from_list($0.sym->user.var, ${child 1, 0}->user.d_sym_list);
  if ($#2) {
    D_ParseNode *c = ${child 2, 0, 1};
    set_builtin($g->i, $0.sym->user.var, c->start_loc.s+1, c->end-1);
  }
  if ($#3) {
    $$.type->implements.add(${child 3, 0, 1, 0}->user.sym->user.var);
    for (int i = 0; i < d_get_number_of_children(${child 3, 0, 1, 1})/2; i++)
      $$.type->implements.add(${child 3, 0, 1, 1, i * 2 + 1}->user.sym->user.var);
  }
  if ($#4)
    $$.type->type = ${child 4, 0, 1}->user.type;
};

super_types: super_type (',' super_type)*;
super_type: type [
  $$.sym = $0.sym;
  if ($0.sym->user.sym_scope)
    ${scope} = scope_D_Scope(${scope}, $0.sym->user.sym_scope);
];

def_type : identifier [ 
  $$.sym = new_sym($g->i, $n0.start_loc.s, $n0.end, ${scope}, &$$.if1); 
  printf("type_identifier %X '%s' %d\n", (int)$$.sym, dupstr($n0.start_loc.s, $n0.end), $n0.start_loc.line);
  $$.sym->user.type_identifier = 1;
  $$.saved_scope = ${scope};
  ${scope} = new_D_Scope(${scope});
  set_in(${scope}, $$.sym->user.var);
];

def_type_parameter_list : '(' (def_type_parameter (',' def_type_parameter)*)? ')'
pre: { 
  if ($#1) {
    D_ParseNode *pn = ${child 1, 0, 1};
    if (pn)
      for (int i = d_get_number_of_children(pn) - 1; i >= 0; i -= 2) {
	if (${child 1, 0, 1, i}->user.sym)
	  $0.d_sym_list.push(${child 1, 0, 1, i}->user.sym);
      }
    if (${child 1, 0, 0}->user.sym)
      $0.d_sym_list.push(${child 1, 0, 0}->user.sym);
  }
};

def_type_parameter
  : def_type
  | def_type ':' type $binary_left 700 
  | constant
  ;

type
  : type vector_type		$unary_left 900
pre: { $$.type = $1.type; $$.type->has.v[0] = $0.type; }
  | type '&'			$unary_left 800
pre: { $$.type = new_ref_type($g->i, $0.type); }
  | type ':' type		$binary_left 700  // subtype constraint
  | type ('*' type)+		$binary_left 600
pre: { $$.type = new_type($g->i, Type_PRODUCT, $0.type, &$n1); }
  | type ('|' type)+		$binary_left 500
pre: { $$.type = new_type($g->i, Type_SUM, $0.type, &$n1); }
  | type ('->' type)+		$binary_left 400
pre: { $$.type = new_type($g->i, Type_FUN, $0.type, &$n1); }
  | identifier 'of' type	$binary_left 300
pre: { $$.type = new_tagged_type($g->i, $n0.start_loc.s, $n0.end, $2.type); }
  | type type_parameter_list	$unary_left 200
pre: { $$.type = new_parametized_type($g->i, $0.type, $1.sym_list); }
  | '(' type  ')'
  | 'class' class_definition
[ COPY_PN($$, $1); ]
pre: {
  $$.type = if1_alloc_sym($g->i);
  $$.type->type_kind = Type_RECORD;
  build_has_from_scope($$.type, $1.saved_scope); 
}
  | type_identifier
  ;

type_parameter_list : '(' (type | constant)  (',' (type | constant) )* ')'
pre: { 
  if ($#2)
    for (int i = d_get_number_of_children(${child 2, 0}) - 1; i >= 0; i -= 2)
      $$.sym_list.push(${child 2, 0, i, 0}->user.type);
  $$.sym_list.push(${child 1, 0, 0}->user.type);
}
;

class_definition : class_modifiers '{' new_class_scope statement* '}' 
[  
  $$.saved_scope = ${scope};
  ${scope} = enter_D_Scope(${scope}, $n0.scope);
];
new_class_scope: [ ${scope} = new_D_Scope(${scope}); ${scope}->kind = SCOPE_RECURSIVE; ];

class_modifiers : (class_modifier (',' class_modifiers)*)?;
class_modifier  
  : (':'  | 'inherits') qualified_identifier_list
    [ ${scope} = scope_list(${scope}, $1.d_sym_list); ]
  | (':>' | 'implements') qualified_identifier_list
  | (':+' | 'includes') qualified_identifier_list
    [ ${scope} = scope_list(${scope}, $1.d_sym_list); ]
  ;
qualified_identifier_list
  : qualified_identifier 
[ $$.d_sym_list.push($$.sym); ]
  | qualified_identifier ',' qualified_identifier_list
[  
  $$.d_sym_list.head = $2.d_sym_list.head;
  $$.d_sym_list.push($0.sym);
];

enum_definition : '{' enumerator (',' enumerator)* '}'
pre: {
  $$.type = new_sum_type($g->i);
  $$.type->has.add($1.type);
  if ($#2) {
    D_ParseNode *pn = ${child 2, 0};
    for (int i = 0; i < d_get_number_of_children(pn); i += 2)
      $$.type->has.add(${child 1, 0, 1, i + 1}->user.type);
  }
};
enumerator: identifier ('=' expression)? 
pre: {
  $$.type = new_enum_element($g->i, $n0.start_loc.s, $n0.end);
}
if1: {
  if ($#1) {
    $$.type->constant = ${child 1, 0, 1}->user.rval->constant;
    if (!$$.type->constant)
      show_error("non-constant enum initializer '%s'", $n.start_loc, dupstr($n0.start_loc.s, $n0.end));
  }
};

vector_type : '[' (vector_index (',' vector_index)*)? ']' 
pre: {
  $$.type = new_vector_type($g->i);
  if ($#1) {
    $$.type->has.add(${child 1, 0, 0}->user.type);
    D_ParseNode *pn = ${child 1, 0, 1};
    for (int i = 0; i < d_get_number_of_children(pn); i += 2)
      $$.type->has.add(${child 1, 0, 1, i + 1}->user.type);
  }
};

vector_index 
  : anyint 
pre: { $$.type = new_range_type($g->i, 0, $0.rval); }
  | anyint '..' anyint
pre: { $$.type = new_range_type($g->i, $0.rval, $2.rval); }
  ;

type_identifier: qualified_identifier
[ 
  $$.sym = $0.sym;
  if (!$$.sym || !$$.sym->user.type_identifier)
    ${reject};
  $$.type = $0.sym->user.var;
]
if1: {}
;

var_statement 
  : 'let' var_definitions ('in' expression)? 
pre: { $$.type = ($#2) ? ${child 2, 0, 1}->user.type : $1.type; }
  | type var_definitions 
pre: { $$.type = $1.type; }
  | def_identifier type
pre: { $0.sym->user.var->type = $$.type = $1.type; }
if1: { $$.rval = $0.sym->user.var; }
  ;

var_definitions : var_definition (',' var_definition)* ;
var_definition : qualified_identifier '=' expression ;
function_statement : def_function type
[ ${scope} = enter_D_Scope(${scope}, $0.saved_scope); ];

where_statement : 'where' where_type (',' where_type)* ;
where_type : type_identifier def_type_parameter_list? (':' type)? ('=' type)?;

expression
  : constant ("__name" string)?
pre: { 
  if ($#1) {
    D_ParseNode *c = ${child 1, 0, 1};
    set_builtin($g->i, $0.sym->user.var, c->start_loc.s+1, c->end-1);
  }
  $$.type = $0.type; 
}
if1: { COPY_PN_IF1($g->i, $$, $0); }
  | non_type_identifier
pre: {
  if (!$$.sym && (${scope}->kind != SCOPE_RECURSIVE ||
                  !($$.sym = find_D_Sym(${scope}, $n0.start_loc.s, $n0.end))))
    $g->errors += show_error("undefined identifier '%s'", 
			     $n0.start_loc, dupstr($n0.start_loc.s, $n0.end));
  if ($$.sym)
    $$.type = $$.sym->user.var->type;
}
if1: {
  $$.rval = $$.sym->user.var;
  $$.lval = $$.sym->user.var;
}
  | def_identifier expression $right 5100
[ COPY_PN($$, $0); ]
pre: { type_check($g, $n, $0.sym->user.var, $1.type); }
if1: {
    //$$.rval = new_temp($g->i, ${scope});
    if1_gen($g->i, &$$.if1, $1.if1);
    if1_move($g->i, &$$.if1, $1.rval, $0.sym->user.var);
    $$.rval = $0.sym->user.var;
    $$.lval = $0.sym->user.var;
    //if1_move($g->i, &$$.if1, $0.sym->user.var, $$.rval);
}
  | def_function expression $right 5100
[ ${scope} = enter_D_Scope(${scope}, $0.saved_scope); ]
pre: { type_check($g, $n, $0.sym->user.var, $1.type); }
if1: { build_function($g, $n); }
  | pre_operator expression 
if1: { build_operator($g->i, $n, 0, $n0, &$n1); }
  | expression post_operator 
if1: { build_operator($g->i, $n, &$n0, $n1, 0); }
  | expression binary_operator expression
pre: { 
  if (${child 1, 0} && ${child 1, 0}->symbol == ${string op comma}) $0.in_tuple = 1;
  if (is_apply(${child 1})) $0.in_apply = 1; 
}
if1: { build_operator($g->i, $n, &$n0, $n1, &$n2); }
  | expression ('.' $name "op period" | '->' $name "op arrow") symbol_identifier $left 9900
if1: { build_operator($g->i, $n, &$n0, $n1, &$n2); }
  | curly_block
  | paren_block
  | curly_block
  | expression '?' expression ':' expression $right 8600
if1: { 
    $$.rval = new_temp($g->i, ${scope});
    if1_if($g->i, &$$.if1, $0.if1, $0.rval, $2.if1, $2.rval, $4.if1, $4.rval, $$.rval); 
}
  | 'if' '(' expression ')' expression $right 6000
if1: { 
    $$.rval = new_temp($g->i, ${scope});
    if1_if($g->i, &$$.if1, $2.if1, $2.rval, $4.if1, $4.rval, NULL, NULL, $$.rval);
}
  | 'if' '(' expression ')' expression 'else' expression $right 6100
if1: { 
    $$.rval = new_temp($g->i, ${scope});
    if1_if($g->i, &$$.if1, $2.if1, $2.rval, $4.if1, $4.rval, $6.if1, $6.rval, $$.rval); 
}
  | 'while' loop_scope '(' expression ')' expression $right 6200
[ ${scope} = enter_D_Scope(${scope}, $n0.scope); ]
if1: { $$.rval = build_loop($g->i, $n1.scope, $n3, 0, 0, $n5, &$$.if1); }
  | 'do' loop_scope expression 'while' expression $right 6300
[ ${scope} = enter_D_Scope(${scope}, $n0.scope); ]
if1: { $$.rval = build_loop($g->i, $n1.scope, $n4, &$n2, 0, $n2, &$$.if1); }
  | 'for' loop_scope '(' expression ';' expression ';' expression ')' expression
[ ${scope} = enter_D_Scope(${scope}, $n0.scope); ]
if1: { $$.rval = build_loop($g->i, $n1.scope, $n3, &$n5, &$n7, $n9, &$$.if1); }
  | 'with' with_scope expression $right 5100
[ ${scope} = enter_D_Scope(${scope}, $n0.scope); ]
if1: { COPY_PN_IF1($g->i, $$, $2); }
  | 'conc' expression $right 5000
if1: { 
  COPY_PN($$, $1);
  if1_conc($g->i, &$$.if1, $1.if1); 
}
  | 'seq' expression $right 5000
if1: { 
  COPY_PN($$, $1);
  if1_seq($g->i, &$$.if1, $1.if1); 
};

loop_scope: [ ${scope} = new_loop_scope($g->i, ${scope}); ]; 

symbol_identifier: identifier
pre: { 
  $$.rval = if1_make_symbol($g->i, $n0.start_loc.s, $n0.end); 
  $$.type = $$.rval->type;
}
if1: {};

non_type_identifier: qualified_identifier [ 
  $$.sym = $0.sym;
  if (!$$.sym || $$.sym->user.type_identifier)
    ${reject};
];

def_identifier: qualified_identifier ('__name' string)? ':' 
[ $$.sym = new_sym($g->i, $n0.start_loc.s, $n0.end, $0.saved_scope, &$$.if1); ]
pre: {
  if ($#1) {
    D_ParseNode *c = ${child 1, 0, 1};
    set_builtin($g->i, $$.sym->user.var, c->start_loc.s+1, c->end-1);
  }
};

def_function: qualified_identifier pattern+ ('__name' string)? ':' 
[
  int n = $#0;
  D_ParseNode *id = ${child 0, n-1};
  $$.sym = new_fun($g->i, id->start_loc.s, id->end, $0.saved_scope, &${scope}, &$$.saved_scope, &$$.if1);
  pattern_scope = ${scope};
  pattern_args.clear();
  d_pass(${parser}, &$n, ${pass pattern});
]
pre: {
  printf("fun '%s' %d\n", dupstr($n0.start_loc.s, $n0.end), $n0.start_loc.line);
  if ($#2) {
    D_ParseNode *c = ${child 2, 0, 1};
    set_builtin($g->i, $$.sym->user.var, c->start_loc.s+1, c->end-1);
  }
};

pattern 
  : identifier (':' type)?
pattern: { 
  //printf("pattern '%s' %d\n", dupstr($n0.start_loc.s, $n0.end), $n0.start_loc.line);
  $$.sym = new_sym($g->i, $n0.start_loc.s, $n0.end, pattern_scope, &$$.if1);
  pattern_args.add($$.sym->user.var);
}
pre: {
  if ($#1)
    $$.type = $$.sym->user.var->declared_type = ${child 1, 0, 1}->user.type;
}
if1: {
  $$.rval = $$.sym->user.var;
}
  | constant
pattern: { 
  $$.sym = new_constant($g->i, $n0);
  $$.rval = $$.sym->user.var;
  pattern_args.add($$.rval);
}
pre: { $$.type = $$.rval->type; }
if1: {} // default copies from child
  | type
pattern: { 
  $$.sym = new_sym($g->i, 0, 0, 0, &$$.if1);
  pattern_args.add($$.sym->user.var);
}
pre: { $$.type = $$.sym->user.var->type = $0.type; }
if1: { $$.rval = $$.sym->user.var; }
 | '(' (sub_pattern (',' sub_pattern)*)? ')'
pattern: {
  $$.rval = new_temp($g->i, pattern_scope);
  $$.rval->type = sym_tuple;
  $$.rval->pattern = 1;
  $$.rval->has.move(pattern_args);
  pattern_args.add($$.rval);
}
pre: { $$.type = $$.rval->type; }
  ;

sub_pattern
  : pattern
pre: { $$.type = $0.type; }
  | '...' (identifier (':' type)?)?
pattern: {
  if (!$#1) {
    $$.sym = new_sym($g->i, 0, 0, 0, &$$.if1);
    pattern_args.add($$.sym->user.var);
  } else {
    D_ParseNode *c = ${child 1, 0, 0};
    $$.sym = new_sym($g->i, c->start_loc.s, c->end, pattern_scope, &$$.if1);
    $$.sym->user.var->vararg = 1;
  }
  $$.rval = $$.sym->user.var;
  pattern_args.add($$.sym->user.var);
}
pre: {
  if ($#1 && d_get_number_of_children(${child 1, 0, 1})) {
    $$.type = ${child 1, 0, 1, 0, 1}->user.type;
    $$.sym->user.var->type = $$.type;
  } else
    $$.type = $$.sym->user.var;
}
  ;

qualified_identifier
  : '::' identifier
[
  $$.saved_scope = global_D_Scope(${scope});
  $$.sym = find_D_Sym_in_Scope($$.saved_scope, $n1.start_loc.s, $n1.end);
]
  | identifier
[  
  $$.saved_scope = ${scope};
  $$.sym = find_D_Sym(${scope}, $n0.start_loc.s, $n0.end); 
]
  | qualified_identifier '::' identifier
[  
  $$.saved_scope = $0.sym->user.sym_scope;
  $$.sym = find_D_Sym_in_Scope($$.saved_scope, $n2.start_loc.s, $n2.end); 
]
  ;

with_scope : expression (',' expression)* ':' ;

control_flow
  : 'goto' identifier 
  | 'continue' identifier? 
  | 'break' identifier? 
  | 'return' expression? 
  ;

binary_operator
  : '.*'        $binary_op_left 9900 
  | '*'         $binary_op_left 9600
  | '/'         $binary_op_left 9600
  | '%'         $binary_op_left 9600
  | '+'         $binary_op_left 9500
  | '-'         $binary_op_left 9500
  | '<<'        $binary_op_left 9400
  | '>>'        $binary_op_left 9400
  | '<'         $binary_op_left 9300
  | '<='        $binary_op_left 9300
  | '>'         $binary_op_left 9300
  | '>='        $binary_op_left 9300
  | '=='        $binary_op_left 9200
  | '!='        $binary_op_left 9200
  | '&'         $binary_op_left 9100
  | '^'         $binary_op_left 9000
  | '|'		$binary_op_left 8900
  | '&&'        $binary_op_left 8800
  | '||'        $binary_op_left 8700
  | '='	$name "op assign" $binary_op_left 8500
  | '*='        $binary_op_left 8500
  | '/='        $binary_op_left 8500
  | '%='        $binary_op_left 8500
  | '+='        $binary_op_left 8500
  | '-='        $binary_op_left 8500
  | '<<='       $binary_op_left 8500
  | '>>='       $binary_op_left 8500
  | '&='        $binary_op_left 8500
  | '|='        $binary_op_left 8500
  | '^='        $binary_op_left 8500
  | ',' $name "op comma" $binary_op_left 8400 
  | '->*'       $binary_op_left 9900
  | '^^' $name "op apply" $binary_op_left 7000
  |      $binary_op_left 7000
  ;

pre_operator
  : '++' $name "pre++" $unary_op_right 9800
  | '--' $name "pre--" $unary_op_right 9800
  | '+'          $unary_op_right 9800
  | '-'          $unary_op_right 9800
  | '~'          $unary_op_right 9800
  | '!'          $unary_op_right 9800
  | '*' $name "op deref" $unary_op_right 9800
  | '&'          $unary_op_right 9800
  | '(' type ')' $unary_op_right 9800
  | 'sizeof'     $unary_op_right 9900
  ;

post_operator
  : '--'  $name "post--" $unary_op_left 9800
  | '++'  $name "post++" $unary_op_left 9800
  ;

curly_block: '{' [ ${scope} = new_D_Scope(${scope}); ${scope}->kind = SCOPE_RECURSIVE; ] 
                 statement* expression? '}' 
[ ${scope} = enter_D_Scope(${scope}, $n0.scope); ]
pre: { scope_type($g, $n1.scope, &$n); }
if1: { build_scope($g, ${scope}, $n1.scope, &$n); };

paren_block: '(' [ ${scope} = new_D_Scope(${scope}); ${scope}->kind = SCOPE_SEQUENTIAL; ]
                 statement* expression? ')' 
[ ${scope} = enter_D_Scope(${scope}, $n0.scope); ]
pre: { scope_type($g, $n1.scope, &$n); }
if1: { build_scope($g, ${scope}, $n1.scope, &$n); };

square_block: '[' [ ${scope} = new_D_Scope(${scope}); ${scope}->kind = SCOPE_PARALLEL; ]
                 statement* expression? ']' 
[ ${scope} = enter_D_Scope(${scope}, $n0.scope); ]
pre: { scope_type($g, $n1.scope, &$n); }
if1: { build_scope($g, ${scope}, $n1.scope, &$n); };


constant : (character | int8 | uint8 | int16 | uint16 | 
	    int32 | uint32 | int64 | uint64 | int | uint |
	    float32 | float64 | float128 | float | 
	    string | symbol)
pre: {
  $$.sym = new_constant($g->i, $n0);
  $$.rval = $$.sym->user.var;
  $$.type = $$.rval->type;
}
if1: {}
;

symbol 
  : '#' identifier
  | '#' string
  ;

string ::= "\"([^\"\\]|\\[^])*\"";
character ::= $name "character" "\'([^\'\\]|\\[^])*\'";

base_int ::= "(0[0-7]*|(0x|0X)[0-9a-fA-F]+|[1-9][0-9]*)";
int8	 ::= $name "int8" base_int "(b|B)";
uint8	 ::= $name "uint8" base_int "((u|U)(b|B)|(b|B)(u|U)";
int16	 ::= $name "int16" base_int "(s|S)";
uint16	 ::= $name "uint16" base_int "(u|U)(s|S)|(s|S)(u|U)";
int32	 ::= $name "int32" base_int "(w|W)";
uint32	 ::= $name "uint32" base_int "(u|U)(w|W)|(w|W)(u|U)";
int64	 ::= $name "int64" base_int "(l|L)";
uint64	 ::= $name "uint64" base_int "(u|U)(l|L)|(u|U)(l|L)";
int	 ::= $name "int" base_int;
uint	 ::= $name "uint" base_int "(u|U)";
anyint	 : character | int8 | uint8 | int16 | uint16 | 
           int32 | uint32 | int64 | uint64 | int | uint;

base_float ::= "(([0-9]+.[0-9]*|[0-9]*.[0-9]+)([eE][\-\+]?[0-9]+)?|[0-9]+[eE][\-\+]?[0-9]+)";
float32	   ::= $name "float32" base_float "(f|F)";
float64	   ::= $name "float64" base_float "(d|D)";
float80	   ::= $name "float80" base_float "(t|T)";
float128   ::= $name "float128" base_float "(l|L)?";
float	   ::= $name "float" base_float;
anyfloat   : float32 | float64 | float128 | float;

anynum ::= character | int8 | uint8 | int16 | uint16 | 
           int32 | uint32 | int64 | uint64 | int | uint |
           float32 | float64 | float128 | float;

complex	   ::= anynum "i" ;

identifier : "[a-zA-Z_][a-zA-Z0-9_]*" $term -1;

_: // default actions
[ 
  if ($# == 1)
    COPY_PN($$, $0);
]
if1: { // propogate generate code up
  if ($# == 1)
    COPY_PN_IF1($g->i, $$, $0);
  else {
    for (int i = 0; i < $#; i++)
      if1_gen($g->i, &$$.if1, d_get_child(&$n,i)->user.if1);
  }
};
