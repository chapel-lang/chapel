#ifndef _GETSTUFF_H_
#define _GETSTUFF_H_

#include "traversal.h"
#include "vec.h"
class Stmt;
class Expr;
class Symbol;
class Type;

enum GetStuffFlags {
  GET_STMTS = 0x1,
  GET_EXPRS = 0x2,
  GET_SYMBOLS  = 0x4,
  GET_TYPES = 0x8,
  GET_ALL = 0xf
};

class GetStuff : public Traversal {
 public:
  bool getStmts;
  bool getExprs;
  bool getSymbols;
  bool getTypes;
  Vec<Stmt*> stmts;
  Vec<Expr*> exprs;
  Vec<Symbol*> symbols;
  Vec<Type*> types;

  GetStuff(int flags = GET_ALL);

  void preProcessStmt(Stmt* stmt);
  void preProcessExpr(Expr* expr);
  void preProcessSymbol(Symbol* sym);
  void preProcessType(Type* type);

  void clear(void);
};


class GetStmts : public GetStuff {
  GetStmts(void);
};


class GetExprs : public GetStuff {
  GetExprs(void);
};


class GetSymbols : public GetStuff {
  GetSymbols(void);
};


class GetTypes : public GetStuff {
  GetTypes(void);
};


#endif
