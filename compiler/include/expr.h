/*
 * Copyright 2004-2015 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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

  // Interface for BaseAST
  virtual bool    inTree();
  virtual bool    isStmt()                                           const;
  virtual Type*   typeInfo();
  virtual void    verify();

  // New interface
  virtual Expr*   copy(SymbolMap* map = NULL, bool internal = false)   = 0;
  virtual void    replaceChild(Expr* old_ast, Expr* new_ast)           = 0;

  virtual Expr*   getFirstExpr()                                       = 0;
  virtual Expr*   getFirstChild()                                      = 0;
  virtual Expr*   getNextExpr(Expr* expr);

  virtual bool    isNoInitExpr()                                     const;

  virtual void    prettyPrint(std::ostream* o);

  /* Returns true if the given expressions is contained by this one. */
  bool            contains(Expr const * const expr) const;
  bool            isModuleDefinition();

  void            insertBefore(Expr* new_ast);
  void            insertAfter(Expr* new_ast);
  void            replace(Expr* new_ast);

  void            insertBefore(const char* format, ...);
  void            insertAfter(const char* format, ...);
  void            replace(const char* format, ...);

  Expr*           remove();

  bool            isStmtExpr()                                       const;
  Expr*           getStmtExpr();
  BlockStmt*      getScopeBlock();

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
                  DefExpr(Symbol*  initSym      = NULL,
                          BaseAST* initInit     = NULL,
                          BaseAST* initExprType = NULL);

  virtual void    verify();

  DECLARE_COPY(DefExpr);

  virtual void    replaceChild(Expr* old_ast, Expr* new_ast);
  virtual void    accept(AstVisitor* visitor);

  virtual Type*   typeInfo();
  virtual void    prettyPrint(std::ostream* o);

  virtual GenRet  codegen();

  virtual Expr*   getFirstExpr();
  virtual Expr*   getFirstChild();

  const char*     name()                               const;

  Symbol*         sym;
  Expr*           init;
  Expr*           exprType;
};


class SymExpr : public Expr {
 public:
  Symbol* var;

  SymExpr(Symbol* init_var);

  DECLARE_COPY(SymExpr);

  virtual void    replaceChild(Expr* old_ast, Expr* new_ast);
  virtual void    verify();
  virtual void    accept(AstVisitor* visitor);

  virtual Type*   typeInfo();
  virtual bool    isNoInitExpr() const;
  virtual GenRet  codegen();
  virtual void    prettyPrint(std::ostream* o);

  virtual Expr*   getFirstExpr();
  virtual Expr*   getFirstChild();
};


class UnresolvedSymExpr : public Expr {
 public:
  const char* unresolved;

  UnresolvedSymExpr(const char* init_var);

  DECLARE_COPY(UnresolvedSymExpr);

  virtual void    replaceChild(Expr* old_ast, Expr* new_ast);
  virtual void    verify();
  virtual void    accept(AstVisitor* visitor);
  virtual Type*   typeInfo();
  virtual GenRet  codegen();
  virtual void    prettyPrint(std::ostream *o);

  virtual Expr*   getFirstExpr();
  virtual Expr*   getFirstChild();
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

  virtual void    verify();

  DECLARE_COPY(CallExpr);

  virtual void    accept(AstVisitor* visitor);

  virtual void    replaceChild(Expr* old_ast, Expr* new_ast);

  virtual GenRet  codegen();
  virtual void    prettyPrint(std::ostream* o);
  virtual Type*   typeInfo();

  virtual Expr*   getFirstExpr();
  virtual Expr*   getFirstChild();
  virtual Expr*   getNextExpr(Expr* expr);

  void            insertAtHead(BaseAST* ast);
  void            insertAtTail(BaseAST* ast);

  FnSymbol*       isResolved();
  bool            isNamed(const char*);

  int             numActuals();
  Expr*           get(int index);
  FnSymbol*       findFnSymbol();

  bool            isPrimitive(PrimitiveTag primitiveTag);
  bool            isPrimitive(const char*  primitiveName);
};

class NamedExpr : public Expr {
 public:
  const char*     name;
  Expr*           actual;

  NamedExpr(const char* init_name, Expr* init_actual);

  virtual void    verify();

  DECLARE_COPY(NamedExpr);

  virtual void    replaceChild(Expr* old_ast, Expr* new_ast);
  virtual void    accept(AstVisitor* visitor);
  virtual Type*   typeInfo();
  virtual GenRet  codegen();
  virtual void    prettyPrint(std::ostream* o);

  virtual Expr*   getFirstExpr();
  virtual Expr*   getFirstChild();
};


// Returns true if 'this' properly contains the given expr, false otherwise.
inline bool
Expr::contains(Expr const * const expr) const
{
  Expr const * parent = expr;
  while ((parent = parent->parentExpr))
    if (parent == this)
      return true;
  return false;
}


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


// Returns true if this statement (expression) causes a change in flow.
// When inserting cleanup code, it must be placed ahead of such flow
// statements, or it will be skipped (which means it's in the wrong place).
// TODO: This predicate could be turned into a method on exprs.
inline static bool isFlowStmt(Expr* stmt)
{
  // A goto is definitely a jump.
  if (isGotoStmt(stmt))
    return true;

  // A return primitive works like a jump. (Nothing should appear after it.)
  if (CallExpr* call = toCallExpr(stmt))
  {
    if (call->isPrimitive(PRIM_RETURN))
      return true;

    // _downEndCount is treated like a flow statement because we do not want to
    // insert autoDestroys after the task says "I'm done."  This can result in
    // false-positive memory allocation errors because the waiting (parent
    // task) can then proceed to test that the subtask has not leaked before
    // the subtask release locally-(dynamically-)allocated memory.
    if (FnSymbol* fn = call->isResolved())
      if (!strcmp(fn->name, "_downEndCount"))
        return true;
  }
  return false;
}


bool get_int(Expr* e, int64_t* i); // false is failure
bool get_uint(Expr *e, uint64_t *i); // false is failure
bool get_string(Expr *e, const char **s); // false is failure
const char* get_string(Expr* e); // fatal on failure

CallExpr* callChplHereAlloc(Symbol *s, VarSymbol* md = NULL);
void insertChplHereAlloc(Expr *call, bool insertAfter, Symbol *sym,
                         Type* t, VarSymbol* md = NULL);
void insertChplHereAlloc(Expr *call, bool insertAfter, Symbol *sym,
                         Type* t, VarSymbol* md,
                         Symbol *sizeTmp);

CallExpr* callChplHereFree(BaseAST* p);

// Walk the subtree of expressions rooted at "expr" in postorder, returning the
// current expression in "e", stopping after "expr" has been returned.
// Assignments to e in the calling context will change the path taken by the
// iterator, so should be avoided (unless you really know what you are doing).
#define for_exprs_postorder(e, expr)                            \
  for (Expr *last = (expr), *e = expr->getFirstExpr();          \
       e;                                                       \
       e = (e != last) ? getNextExpr(e) : NULL)

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
