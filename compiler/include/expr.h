/*
 * Copyright 2004-2020 Cray Inc.
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
#include "driver.h"

#include "primitive.h"
#include "symbol.h"

#include <ostream>

class PrimitiveOp;


class Expr : public BaseAST {
public:
                  Expr(AstTag astTag);
  virtual        ~Expr();

  // Interface for BaseAST
          bool    inTree();
  virtual bool    isStmt()                                           const;
  virtual QualifiedType qualType();
  virtual void    verify();

  void verify(AstTag expectedTag); // ensure tag is as expected, then verify()
  void verifyParent(const Expr* child); // verify proper child->parentExpr

  // New interface
  virtual Expr*   copy(SymbolMap* map = NULL, bool internal = false)   = 0;
  virtual void    replaceChild(Expr* old_ast, Expr* new_ast)           = 0;

  virtual Expr*   getFirstExpr()                                       = 0;
  virtual Expr*   getNextExpr(Expr* expr);

  virtual bool    isNoInitExpr()                                     const;

  virtual void    prettyPrint(std::ostream* o);


  bool            isRef();
  bool            isWideRef();
  bool            isRefOrWideRef();

  /* Returns true if the given expression is contained by this one. */
  bool            contains(const Expr* expr)                         const;

  bool            isModuleDefinition();

  void            insertBefore(Expr* new_ast);
  void            insertAfter(Expr* new_ast);
  void            replace(Expr* new_ast);

  // Insert multiple ASTs in the order of the arguments.
  // Todo: replace with a single varargs version.
  void            insertAfter(Expr* e1, Expr* e2);
  void            insertAfter(Expr* e1, Expr* e2, Expr* e3);
  void            insertAfter(Expr* e1, Expr* e2, Expr* e3, Expr* e4);
  void            insertAfter(Expr* e1, Expr* e2, Expr* e3, Expr* e4,
                              Expr* e5);
  void            insertAfter(Expr* e1, Expr* e2, Expr* e3, Expr* e4,
                              Expr* e5, Expr* e6);

  void            insertBefore(AList exprs);
  void            insertAfter(AList exprs);

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

  virtual QualifiedType qualType();
  virtual void    prettyPrint(std::ostream* o);

  virtual GenRet  codegen();

  virtual Expr*   getFirstExpr();

  const char*     name()                               const;

  Symbol*         sym;
  Expr*           init;
  Expr*           exprType;
};


class SymExpr : public Expr {
 private:
  Symbol* var;

 public:
  // List entries to support enumerating SymExprs in a Symbol
  // These are public because:
  //  * they are managed in Symbol (but could friend class Symbol)
  //  * they are used in for_SymbolSymExprs (but could create a real iterator)
  SymExpr* symbolSymExprsPrev;
  SymExpr* symbolSymExprsNext;

  SymExpr(Symbol* init_var);

  DECLARE_COPY(SymExpr);

  virtual void    replaceChild(Expr* old_ast, Expr* new_ast);
  virtual void    verify();
  virtual void    accept(AstVisitor* visitor);

  virtual QualifiedType qualType();
  virtual bool    isNoInitExpr() const;
  virtual GenRet  codegen();
  virtual void    prettyPrint(std::ostream* o);

  virtual Expr*   getFirstExpr();

  Symbol* symbol() {
    return var;
  }

  void setSymbol(Symbol* s);
};

class UnresolvedSymExpr : public Expr {
 public:
  const char* unresolved;

  UnresolvedSymExpr(const char* init_var);

  DECLARE_COPY(UnresolvedSymExpr);

  virtual void    replaceChild(Expr* old_ast, Expr* new_ast);
  virtual void    verify();
  virtual void    accept(AstVisitor* visitor);
  virtual QualifiedType qualType();
  virtual GenRet  codegen();
  virtual void    prettyPrint(std::ostream *o);

  virtual Expr*   getFirstExpr();
};


#include "CallExpr.h"


//
// A ContextCallExpr
//
// There are situations in which function resolution is unable to make a
// final selection for the function to be used for a particular call
// because of incomplete information about "ref-ness".
//
// When this occurs the CallExpr is replaced with a ContextCallExpr that
// contains either 2 or 3 CallExprs. These are stored in a consistent
// order in an AList :-
//
//     the best "value"     function
//     the best "const-ref" function
//     the best "ref"       function
//
// and flags are used to indicate which functions are present.  The final
// CallExpr is selected in cullReferences
//
// A ContextCall has a designated call.  The designated call will be returned
// if toCallExpr() is called on the context call.
//
// typeInfo/qualType on the context call will return the type info
// for the designated call.
//
// isCallExpr() will return true for a ContextCallExpr
//
class ContextCallExpr : public Expr {
public:
                         ContextCallExpr();

  DECLARE_COPY(ContextCallExpr);

  virtual void           replaceChild(Expr* oldAst, Expr* newAst);

  virtual void           verify();
  virtual void           accept(AstVisitor* visitor);
  virtual QualifiedType  qualType();
  virtual GenRet         codegen();
  virtual void           prettyPrint(std::ostream* o);

  virtual Expr*          getFirstExpr();

  void                   setRefValueConstRefOptions(CallExpr* refCall,
                                                    CallExpr* valueCall,
                                                    CallExpr* constRefCall);

  void                   getCalls(CallExpr*& refCall,
                                  CallExpr*& valueCall,
                                  CallExpr*& constRefCall)               const;

  CallExpr*              getValueCall()                                  const;
  CallExpr*              getConstRefCall()                               const;
  CallExpr*              getRefCall()                                    const;

  AList                  options;

private:
  bool                   hasValue;
  bool                   hasConstRef;
  bool                   hasRef;
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
  virtual QualifiedType qualType();
  virtual GenRet  codegen();
  virtual void    prettyPrint(std::ostream* o);

  virtual Expr*   getFirstExpr();
};


// Determines whether a node is in the AST (vs. has been removed
// from the AST). Used e.g. by cleanAst().
//
// We may want to replace isAlive() with {Expr,Symbol,Type}::inTree().
// Right now they are different:
//  - Symbol::inTree() performs an additional check for rootModule
//    whereas isAlive(Symbol) does not.
//  - isAlive(Symbol) is false vs. Symbol::inTree() is true on rootModule.
//    'rootModule' is the only module that is always alive/in tree
//    yet does not have a defPoint.
//  - Type::inTree() performs an additional check for Type::symbol != NULL,
//    whereas isAlive(Type) does not.
//
static inline bool isAlive(Expr* expr) {
  return expr->parentSymbol;
}

static inline bool isAliveQuick(Symbol* symbol) {
  return isAlive(symbol->defPoint);
}

static inline bool isAlive(Symbol* symbol) {
  return symbol->defPoint && isAlive(symbol->defPoint);
}

static inline bool isAlive(Type* type) {
  if (fMinimalModules) {
    if(type == dtBytes || type == dtString) {
      return false;
    }
  }
  return isAlive(type->symbol->defPoint);
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

static inline bool isLoopExprFun(FnSymbol* fn) {
  return 0 == strncmp(fn->name, astr_loopexpr_iter, strlen(astr_loopexpr_iter));
}

// Does this function require "capture for parallelism"?
// Yes, if it comes from a begin/cobegin/coforall block in Chapel source.
static inline bool needsCapture(FnSymbol* taskFn) {
  return taskFn->hasFlag(FLAG_BEGIN) ||
         taskFn->hasFlag(FLAG_COBEGIN_OR_COFORALL) ||
         taskFn->hasFlag(FLAG_NON_BLOCKING);
}

inline Symbol* ShadowVarSymbol::outerVarSym() const {
  if (SymExpr* ovse = this->outerVarSE)
    return ovse->symbol();
  else
    return NULL;
}

// E.g. NamedExpr::actual, DefExpr::init.
static inline void verifyNotOnList(Expr* expr) {
  if (expr && expr->list)
    INT_FATAL(expr, "Expr is in a list incorrectly");
}

// Strip NamedExpr, if present.
static inline Symbol* symbolForActual(Expr* actual) {
  if (NamedExpr* ne = toNamedExpr(actual))
    actual = ne->actual;
  return toSymExpr(actual)->symbol();
}


bool get_int(Expr* e, int64_t* i); // false is failure
bool get_uint(Expr *e, uint64_t *i); // false is failure
bool get_string(Expr *e, const char **s); // false is failure
const char* get_string(Expr* e); // fatal on failure


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

// This mechanism allows storing optimization/analysis results
// in case they need to be used by later passes.
// The optimization/analysis result is stored in a PRIM_OPTIMIZATION_INFO
// expression after insertAfter, or added to one if it already exists.
void addOptimizationFlag(Expr* insertAfter, Flag flag);
// Returns true if a nearby PRIM_OPTIMIZATION_INFO includes this flag
bool hasOptimizationFlag(Expr* anchor, Flag flag);


#ifdef HAVE_LLVM
llvm::Value* createVarLLVM(llvm::Type* type, const char* name);
llvm::Value* createVarLLVM(llvm::Type* type);
#endif

GenRet codegenValue(GenRet r);
GenRet codegenValuePtr(GenRet r);

GenRet createTempVarWith(GenRet v);

GenRet codegenDeref(GenRet toDeref);
GenRet codegenLocalDeref(GenRet toDeref);
GenRet codegenNullPointer();
GenRet codegenCast(const char* typeName, GenRet value, bool Cparens = true);

void codegenCallPrintf(const char* arg);

#endif
