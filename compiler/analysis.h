/*
  Copyright 2004 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/
#ifndef _analysis_H_
#define _analysis_H_

#include "ast.h"
#include "sym.h"

class Symbol;
class BaseAST;

class ASymbol : public Sym {
 public:
  BaseAST *xsymbol;
  
  ASymbol();
};

class AInfo : public AST {
 public:
  BaseAST *xast;

  char *pathname();
  int line();
  Sym *symbol();
  AST *copy(Map<PNode *, PNode*> *nmap = 0);

  AInfo();
};

void analyze_new_ast(BaseAST *s);

#endif
