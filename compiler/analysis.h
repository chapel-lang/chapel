/*
  Copyright 2004 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/
#ifndef _analysis_H_
#define _analysis_H_

#include "ast.h"
#include "sym.h"

class Symbol;
class BaseAST;
class Label;
class Code;

class ASymbol : public Sym {
 public:
  BaseAST *xsymbol;
  
  ASymbol();
};

class AInfo : public AST {
 public:
  BaseAST *xast;
  Code *code;
  Label *label[2];	// before and after for loops (continue,break)
  Sym *sym, *rval;

  char *pathname();
  int line();
  Sym *symbol();
  AST *copy(Map<PNode *, PNode*> *nmap = 0);

  AInfo();
};

int AST_to_IF1(BaseAST *s);
void print_AST_Expr_types(BaseAST *s);
void print_AST_types();

#endif
