#ifndef _TRAVERSAL_H_
#define _TRAVERSAL_H_

class Stmt;
class Expr;
class Symbol;
class Type;

class Traversal {
 public:
  bool exploreStmts;
  bool exploreExprs;
  bool exploreSymbols;
  bool exploreTypes;

  Traversal(void);

  virtual void preProcessStmt(Stmt* stmt);
  virtual void postProcessStmt(Stmt* stmt);
  virtual void preProcessExpr(Expr* expr);
  virtual void postProcessExpr(Expr* expr);
  virtual void preProcessSymbol(Symbol* sym);
  virtual void postProcessSymbol(Symbol* sym);
  virtual void preProcessType(Type* type);
  virtual void postProcessType(Type* type);
};

#endif
