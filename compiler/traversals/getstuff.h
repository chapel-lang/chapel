#ifndef _GETSTUFF_H_
#define _GETSTUFF_H_

#include "traversal.h"
#include "vec.h"
class BaseAST;
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
  Vec<BaseAST*> asts;
  Vec<Stmt*> stmts;
  Vec<Expr*> exprs;
  Vec<Symbol*> symbols;
  Vec<Type*> types;

  GetStuff(int flags = GET_ALL);

  void preProcessStmt(Stmt* &stmt);
  void preProcessExpr(Expr* &expr);
  void preProcessSymbol(Symbol* &sym);
  void preProcessType(Type* &type);

  void clear(void);
};


class GetStmts : public GetStuff {
 public:
  GetStmts(void);
};


class GetExprs : public GetStuff {
 public:
  GetExprs(void);
};


class GetSymbols : public GetStuff {
 public:
  GetSymbols(void);
};


class GetTypes : public GetStuff {
 public:
  GetTypes(void);
};


#endif
