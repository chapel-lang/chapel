#ifndef _EXPR_H_
#define _EXPR_H_

#include <cstdlib>
#include <cstdio>
#include <fstream>
#include "alist.h"
#include "baseAST.h"
#include "symbol.h"
#include "primitive.h"

#include "genret.h"

class FnSymbol;

#define IS_EXPR(e)                                          \
  ((e)->astTag == E_CallExpr || (e)->astTag == E_SymExpr || \
   (e)->astTag == E_DefExpr || (e)->astTag == E_NamedExpr)

#define IS_STMT(e)                                              \
  ((e)->astTag == E_BlockStmt ||                                \
   (e)->astTag == E_CondStmt || (e)->astTag == E_GotoStmt)

class Expr : public BaseAST {
 public:
  Expr* prev;           // alist previous pointer
  Expr* next;           // alist next pointer
  AList* list;          // alist pointer
  Expr* parentExpr;
  Symbol* parentSymbol;

  Expr(AstTag astTag);
  virtual ~Expr() { }
  virtual Expr* copy(SymbolMap* map = NULL, bool internal = false) = 0;
  virtual Expr* copyInner(SymbolMap* map) = 0;
  virtual void replaceChild(Expr* old_ast, Expr* new_ast) = 0;

  virtual void verify();
  virtual bool inTree();
  virtual Type* typeInfo(void);

  virtual void prettyPrint(std::ofstream *o);

  Expr* getStmtExpr();

  Expr* remove(void);
  void replace(Expr* new_ast);
  void insertBefore(Expr* new_ast);
  void insertAfter(Expr* new_ast);
};


class DefExpr : public Expr {
 public:
  Symbol* sym;
  Expr* init;
  Expr* exprType;

  DefExpr(Symbol* initSym = NULL,
          BaseAST* initInit = NULL,
          BaseAST* initExprType = NULL);
  void verify(); 
  DECLARE_COPY(DefExpr);
  void replaceChild(Expr* old_ast, Expr* new_ast);

  Type* typeInfo(void);
  void prettyPrint(std::ofstream *o);
  
  GenRet codegen();
};


class SymExpr : public Expr {
 public:
  Symbol* var;
  SymExpr(Symbol* init_var);
  DECLARE_COPY(SymExpr);
  void replaceChild(Expr* old_ast, Expr* new_ast);
  void verify(); 
  Type* typeInfo(void);
  GenRet codegen();
  void prettyPrint(std::ofstream *o);
};


class UnresolvedSymExpr : public Expr {
 public:
  const char* unresolved;
  UnresolvedSymExpr(const char* init_var);
  DECLARE_COPY(UnresolvedSymExpr);
  void replaceChild(Expr* old_ast, Expr* new_ast);
  void verify(); 
  Type* typeInfo(void);
  GenRet codegen();
  void prettyPrint(std::ofstream *o);
};



class CallExpr : public Expr {
 public:
  Expr* baseExpr;         // function expression
  AList argList;          // function actuals
  PrimitiveOp* primitive; // primitive expression (baseExpr == NULL)
  bool partialTag;
  bool methodTag;
  bool square; // true if call made with square brackets

  CallExpr(BaseAST* base, BaseAST* arg1 = NULL, BaseAST* arg2 = NULL,
           BaseAST* arg3 = NULL, BaseAST* arg4 = NULL);
  CallExpr(PrimitiveOp *prim, BaseAST* arg1 = NULL, BaseAST* arg2 = NULL,
           BaseAST* arg3 = NULL, BaseAST* arg4 = NULL);
  CallExpr(PrimitiveTag prim, BaseAST* arg1 = NULL, BaseAST* arg2 = NULL,
           BaseAST* arg3 = NULL, BaseAST* arg4 = NULL);
  CallExpr(const char* name, BaseAST* arg1 = NULL, BaseAST* arg2 = NULL,
           BaseAST* arg3 = NULL, BaseAST* arg4 = NULL);
  ~CallExpr();
  void verify(); 
  DECLARE_COPY(CallExpr);

  void replaceChild(Expr* old_ast, Expr* new_ast);

  GenRet codegen();
  void prettyPrint(std::ofstream *o);

  void insertAtHead(BaseAST* ast);
  void insertAtTail(BaseAST* ast);

  FnSymbol* isResolved(void);
  bool isNamed(const char*);

  int numActuals();
  Expr* get(int index);
  FnSymbol* findFnSymbol(void);
  Type* typeInfo(void);
  bool isPrimitive(PrimitiveTag primitiveTag);
  bool isPrimitive(const char* primitiveName);
};


class NamedExpr : public Expr {
 public:
  const char* name;
  Expr* actual;
  NamedExpr(const char* init_name, Expr* init_actual);
  void verify(); 
  DECLARE_COPY(NamedExpr);
  void replaceChild(Expr* old_ast, Expr* new_ast);
  Type* typeInfo(void);
  GenRet codegen();
  void prettyPrint(std::ofstream *o);
};


// Determines whether a node is in the AST (vs. has been removed
// from the AST). Used e.g. by cleanAst().
// Exception: 'n' is also live if isRootModule(n).

static inline bool isAlive(Expr* expr) {
  return expr->parentSymbol;
}

static inline bool isAliveQuick(Symbol* symbol) {
  return isAlive(symbol->defPoint);
}

static inline bool isAlive(Symbol* symbol) {
  if (symbol->hasFlag(FLAG_GLOBAL_TYPE_SYMBOL)) return true;
  if (! symbol->defPoint) return false;
  return isAliveQuick(symbol);
}

static inline bool isAlive(Type* type) {
  return isAliveQuick(type->symbol);
}

#define isRootModule(ast)  \
  ((ast) == rootModule)

#define isRootModuleWithType(ast, type)  \
  (E_##type == E_ModuleSymbol && ((ModuleSymbol*)(ast)) == rootModule)


bool get_int(Expr* e, int64_t* i); // false is failure
bool get_uint(Expr *e, uint64_t *i); // false is failure
bool get_string(Expr *e, const char **s); // false is failure
const char* get_string(Expr* e); // fatal on failure
VarSymbol *get_constant(Expr *e);

#define for_exprs_postorder(e, expr)                            \
  for (Expr* e = getFirstExpr(expr); e; e = getNextExpr(e))

Expr* getFirstExpr(Expr* expr);
Expr* getNextExpr(Expr* expr);

Expr* new_Expr(const char* format, ...);
Expr* new_Expr(const char* format, va_list vl);

GenRet codegenValue(GenRet r);
GenRet codegenValuePtr(GenRet r);
#ifdef HAVE_LLVM
llvm::Value* createTempVarLLVM(llvm::Type* type, const char* name);
llvm::Value* createTempVarLLVM(llvm::Type* type);
#endif
GenRet createTempVarWith(GenRet v);
GenRet createTempVar(Type *t);

GenRet codegenDeref(GenRet toDeref);
GenRet codegenLocalDeref(GenRet toDeref);

#endif
