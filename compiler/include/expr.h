#ifndef _EXPR_H_
#define _EXPR_H_

#include "baseAST.h"

#include "primitive.h"
#include "symbol.h"

#include <ostream>

class PrimitiveOp;


class Expr : public BaseAST {
public:
                  Expr(AstTag astTag);
  virtual        ~Expr();

  virtual Expr*   copy(SymbolMap* map = NULL, bool internal = false)   = 0;
  virtual void    replaceChild(Expr* old_ast, Expr* new_ast)           = 0;

  // Interface for BaseAST
  virtual bool    inTree();
  virtual bool    isStmt() const { return false; }
  virtual Type*   typeInfo();
  virtual void    verify();

  virtual bool    isNoInitExpr()                                     const;
  virtual void    prettyPrint(std::ostream* o);

  bool            isModuleDefinition();

  void            insertBefore(Expr* new_ast);
  void            insertAfter(Expr* new_ast);
  void            replace(Expr* new_ast);

  Expr*           remove();
  Expr*           getStmtExpr();

  Symbol*         parentSymbol;
  Expr*           parentExpr;

  AList*          list;           // alist pointer
  Expr*           prev;           // alist previous pointer
  Expr*           next;           // alist next     pointer

private:
  virtual Expr*   copyInner(SymbolMap* map) = 0;
};


class DefExpr : public Expr {
 public:
  Symbol* sym;
  Expr* init;
  Expr* exprType;

  DefExpr(Symbol* initSym = NULL,
          BaseAST* initInit = NULL,
          BaseAST* initExprType = NULL);
  virtual void verify(); 
  DECLARE_COPY(DefExpr);

  virtual void replaceChild(Expr* old_ast, Expr* new_ast);
  virtual void    accept(AstVisitor* visitor);

  Type* typeInfo(void);
  void prettyPrint(std::ostream *o);
  
  GenRet codegen();
};


class SymExpr : public Expr {
 public:
  Symbol* var;
  SymExpr(Symbol* init_var);
  DECLARE_COPY(SymExpr);
  virtual void replaceChild(Expr* old_ast, Expr* new_ast);
  virtual void verify(); 
  virtual void    accept(AstVisitor* visitor);

  Type* typeInfo(void);
  bool isNoInitExpr() const;
  GenRet codegen();
  void prettyPrint(std::ostream *o);
};


class UnresolvedSymExpr : public Expr {
 public:
  const char* unresolved;

  UnresolvedSymExpr(const char* init_var);
  DECLARE_COPY(UnresolvedSymExpr);
  virtual void replaceChild(Expr* old_ast, Expr* new_ast);
  virtual void verify(); 
  virtual void    accept(AstVisitor* visitor);
  Type* typeInfo(void);
  GenRet codegen();
  void prettyPrint(std::ostream *o);
};



class CallExpr : public Expr {
 public:
  Expr* baseExpr;         // function expression
  AList argList;          // function actuals
  PrimitiveOp* primitive; // primitive expression (baseExpr == NULL)
  bool partialTag;
  bool methodTag; ///< Set to true if the call is a method call.
  // It is used in gatherCandidates to filter out method field extraction
  // (partials).
  // TODO: Maybe use a new primitive to represent partials, and get rid of this tag.
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

  virtual void    accept(AstVisitor* visitor);

  void replaceChild(Expr* old_ast, Expr* new_ast);

  GenRet codegen();
  void prettyPrint(std::ostream *o);

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
  virtual void    accept(AstVisitor* visitor);
  Type* typeInfo(void);
  GenRet codegen();
  void prettyPrint(std::ostream *o);
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

static inline bool isGlobal(Symbol* symbol) {
  return isModuleSymbol(symbol->defPoint->parentSymbol);
}

static inline bool isTaskFun(FnSymbol* fn) {
  INT_ASSERT(fn);
  // Testing individual flags is more efficient than ops on entire FlagSet?
  return fn->hasFlag(FLAG_BEGIN) ||
         fn->hasFlag(FLAG_COBEGIN_OR_COFORALL) ||
         fn->hasFlag(FLAG_ON);
}

static inline FnSymbol* resolvedToTaskFun(CallExpr* call) {
  INT_ASSERT(call);
  if (FnSymbol* cfn = call->isResolved()) {
    if (isTaskFun(cfn))
      return cfn;
  }
  return NULL;
}

// Does this function require "capture for parallelism"?
// Yes, if it comes from a begin/cobegin/coforall block in Chapel source.
static inline bool needsCapture(FnSymbol* taskFn) {
  return taskFn->hasFlag(FLAG_BEGIN) ||
         taskFn->hasFlag(FLAG_COBEGIN_OR_COFORALL) ||
         taskFn->hasFlag(FLAG_NON_BLOCKING);
}


bool get_int(Expr* e, int64_t* i); // false is failure
bool get_uint(Expr *e, uint64_t *i); // false is failure
bool get_string(Expr *e, const char **s); // false is failure
const char* get_string(Expr* e); // fatal on failure

CallExpr* callChplHereAlloc(Symbol *s, VarSymbol* md = NULL);
void insertChplHereAlloc(Expr *call, bool insertAfter, Symbol *sym,
                         Type* t, VarSymbol* md = NULL);
CallExpr* callChplHereFree(BaseAST* p);

#define for_exprs_postorder(e, expr)                                         \
  for (Expr *last = (expr), *e = getFirstExpr(last); e; e = (e != last) ? getNextExpr(e) : NULL)

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

GenRet codegenDeref(GenRet toDeref);
GenRet codegenLocalDeref(GenRet toDeref);

#endif
