#ifndef _TRAVERSAL_H_
#define _TRAVERSAL_H_

#include "../passes/pass.h"

class Stmt;
class Expr;
class Symbol;
class Type;

class Traversal : public Pass {
 public:
  bool processInternalModules;
  bool processTop;
  bool exploreChildStmts;
  bool exploreChildExprs;
  bool exploreChildSymbols;
  bool exploreChildTypes;

  Traversal(bool processThis = true, bool exploreByDefault = true);

  virtual void preProcessStmt(Stmt* &stmt);
  virtual void postProcessStmt(Stmt* &stmt);
  virtual void preProcessExpr(Expr* &expr);
  virtual void postProcessExpr(Expr* &expr);
  virtual void preProcessSymbol(Symbol* &sym);
  virtual void postProcessSymbol(Symbol* &sym);
  virtual void preProcessType(Type* &type);
  virtual void postProcessType(Type* &type);

  virtual void run(ModuleSymbol* moduleList);
};


#define TRAVERSE(link, traversal, atTop) \
  link->traverse(link, traversal, atTop)

#define TRAVERSE_LS(link, traversal, atTop) \
  link->traverseList(link, traversal, atTop)

#define CAST_TRAVERSE_LS(cast, link, traversal, atTop) \
  ((cast*)link)->traverseList((cast*&)(link), traversal, atTop)

// BLC: should consider removing the recursion here, although
// perhaps that would make it harder to debug?

#define DEFINE_TRAVERSE_LIST(name) \
  void traverseList(name* &_this, Traversal* traversal, bool atTop = true) { \
    if (isNull()) return; \
    TRAVERSE(_this, traversal, atTop); \
    if (_this->isNull() || _this->next->isNull()) return; \
    if (dynamic_cast<name*>(_this->next)) { \
      CAST_TRAVERSE_LS(name, _this->next, traversal, atTop); \
    } else { \
      INT_FATAL(this, "Non-homogeneous list encountered"); \
    } \
  }

#define TRAVERSABLE_ILINK(name) \
 public: \
  void traverse(name* &_this, Traversal* traversal, bool atTop = true); \
  DEFINE_TRAVERSE_LIST(name)

#define DEFINE_TRAVERSABLE_AST(base, name) \
  public: \
  void traverse(name* &_this, Traversal* traversal, bool atTop = true) { \
    if (isNull()) return; \
    if (traversal->processTop || !atTop) \
      traversal->preProcess##base((base*&)_this); \
    if (atTop || traversal->exploreChild##base##s) \
      _this->traverse##base(traversal); \
    if (traversal->processTop || !atTop) \
      traversal->postProcess##base((base*&)_this); \
  } \
  DEFINE_TRAVERSE_LIST(name)

#define TRAVERSABLE_STMT(name) DEFINE_TRAVERSABLE_AST(Stmt, name)
#define TRAVERSABLE_EXPR(name) DEFINE_TRAVERSABLE_AST(Expr, name)
#define TRAVERSABLE_SYMBOL(name) DEFINE_TRAVERSABLE_AST(Symbol, name)
#define TRAVERSABLE_TYPE(_name) \
 public: \
  void traverse(_name* &_this, Traversal* traversal, bool atTop = true) { \
    if (isNull()) return; \
    if (traversal->processTop || !atTop) \
      traversal->preProcessType((Type*&)_this); \
    if (atTop || traversal->exploreChildTypes) { \
      if (atTop || _this->name == nilSymbol) { \
	_this->traverseDefType(traversal); \
      } else { \
	_this->traverseType(traversal); \
      } \
    } \
    if (traversal->processTop || !atTop) \
      traversal->postProcessType((Type*&)_this); \
  }; \
  DEFINE_TRAVERSE_LIST(_name)


#endif
