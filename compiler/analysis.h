/*
  Copyright 2004 John Plevyak, All Rights Reserved, see COPYRIGHT file
*/
#ifndef _analysis_H_
#define _analysis_H_

class Symbol;
class Expr;
class Stmt;

class ASymbol {
  void *symbol;
  
  ASymbol();
};

class AInfo {
  void *code;

  AInfo();
};

void analyze_new_ast(Stmt *s);

#endif
