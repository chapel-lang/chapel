#include "geysa.h"
#include "parse.h"
#include "parse_structs.h"
#include "parse_ast.h"
#include "if1.h"
#include "grammar.h"

char *
constant_type(D_ParseNode &pn, D_Symbol *d_symbols) {
  D_ParseNode *child = d_get_child(&pn, 0);
  while (d_symbols[child->symbol].kind != D_SYMBOL_REGEX && 
	 d_symbols[child->symbol].kind != D_SYMBOL_NTERM)
    child = d_get_child(child, 0);
  return d_symbols[child->symbol].name;
}

ParseAST *
loop_AST(D_ParseNode &loop, D_ParseNode &cond, D_ParseNode *before, 
	 D_ParseNode *after, D_ParseNode &body) 
{
  ParseAST *b = body.user.ast;
  if (after) {
    b = new_AST(AST_block);
    b->set_location_and_add(&body);
    b->add(after->user.ast);
  }
  ParseAST *l = new_AST(AST_loop), *a = l;
  l->set_location(&loop);
  if (before && before != &body) {
    a = new_AST(AST_block);
    a->set_location_and_add(before);
    a->add(l);
  }
  ParseAST *c = new_AST(AST_loop_cond);
  c->set_location_and_add(&cond);
  if (before != &body)
    l->add(c);
  l->add(b);
  if (before == &body)
    l->add(c);
  return a;
}

ParseAST *
symbol_AST(IF1 *if1, D_ParseNode *pn) {
  ParseAST *a = new_AST(AST_const, pn); 
  char *s;
  int l = pn->end - pn->start_loc.s;
  if (!l)
    s = "#^^";
  else {
    s = (char*)MALLOC(l + 2);
    memcpy(s+1, pn->start_loc.s, l);
    s[0] = '#';
    s[l+1] = 0;
  }
  a->string = if1_cannonicalize_string(if1, s);
  a->constant_type = "symbol";
  return a;
}

static
int dig_add_ast(ParseAST *op, D_ParseNode *pn) {
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

ParseAST *
op_AST(IF1 *if1, D_ParseNode &pn) {
  ParseAST *op = new_AST(AST_op);
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

