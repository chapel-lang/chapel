/* -*-Mode: c++;-*-
  Copyright 2003-4 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/

#include "geysa.h"
#include "grammar.h"

char *
constant_type(D_ParseNode &pn, D_Symbol *d_symbols) {
  D_ParseNode *child = d_get_child(&pn, 0);
  while (d_symbols[child->symbol].kind != D_SYMBOL_REGEX && 
	 d_symbols[child->symbol].kind != D_SYMBOL_NTERM)
    child = d_get_child(child, 0);
  return d_symbols[child->symbol].name;
}

void
in_module(Globals *g, char *s, char *e, D_Scope **scope) {
  D_Scope *global = global_D_Scope(*scope);
  D_Sym *sym = find_D_Sym(global, s, e);
  if (!sym)
    sym = NEW_D_SYM(global, s, e);
  if (sym->user.scope)
    *scope = enter_D_Scope(*scope, sym->user.scope);
  else {
    *scope = sym->user.scope = new_D_Scope(global);
    D_Sym *s = new_internal(*scope, __module);
    s->user.sym = sym;
  }
}

AST *
loop_AST(D_ParseNode &loop, D_ParseNode &cond, D_ParseNode *before, 
	 D_ParseNode *after, D_ParseNode &body) 
{
  AST *b = body.user.ast;
  if (after) {
    b = new AST(AST_block);
    b->set_location(&body);
    b->add(body.user.ast);
    b->add(after->user.ast);
  }
  AST *l = new AST(AST_loop), *a = l;
  l->set_location(&loop);
  if (before && before != &body) {
    a = new AST(AST_block);
    a->add(before->user.ast);
    a->add(l);
  }
  if (before != &body)
    l->add(new AST(AST_loop_cond, &cond));
  l->add(b);
  if (before == &body)
    l->add(new AST(AST_loop_cond, &cond));
  return a;
}

AST *
symbol_AST(IF1 *if1, D_ParseNode *pn) {
  AST *a = new AST(AST_const, pn); 
  char *s;
  int l = pn->end - pn->start_loc.s;
  if (!l)
    s = "#^^";
  else {
    s =  (char*)MALLOC(l + 1);
    memcpy(s+1, pn->start_loc.s, l);
    s[0] = '#';
  }
  a->string = if1_cannonicalize_string(if1, s);
  a->constant_type = "symbol";
  return a;
}

static
int dig_add_ast(AST *op, D_ParseNode *pn) {
  int n = 0;
  for (int i = 0; i < d_get_number_of_children(pn); i++) {
    D_ParseNode *c = d_get_child(pn, i);
    if (c->user.ast) {
      op->add(c->user.ast);
      n++;
    } else
      n += dig_add_ast(op, c);
  }
  return n;
}

AST *
op_AST(IF1 *if1, D_ParseNode &pn) {
  AST *op = new AST(AST_op);
  op->set_location(&pn);
  int n = d_get_number_of_children(&pn);
  for (int i = 0; i < n; i++) {
    D_ParseNode *c = d_get_child(&pn, i);
    if (c->user.ast)
      op->add(c->user.ast);
    else {
      op->op_index = i;
      if (!dig_add_ast(op, c))
	op->add(symbol_AST(if1, c));
    }
  }
  return op;
}

