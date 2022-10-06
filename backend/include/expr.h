/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#ifdef HAVE_LLVM
// Forward declare AllocaInst.
namespace llvm
{
  class AllocaInst;
}
#endif

class PrimitiveOp;

#include "expr-class-def.h"

class DefExpr final : public Expr {
public:
  DefExpr(Symbol*  initSym      = NULL,
          BaseAST* initInit     = NULL,
          BaseAST* initExprType = NULL);
 ~DefExpr() override = default;

  void    verify()                                   override;

  DECLARE_COPY(DefExpr);
  DefExpr* copyInner(SymbolMap* map)                 override;


  void    replaceChild(Expr* old_ast, Expr* new_ast) override;
  void    accept(AstVisitor* visitor)                override;

  QualifiedType qualType()                           override;
  void    prettyPrint(std::ostream* o)               override;

  GenRet  codegen()                                  override;

  Expr*   getFirstExpr()                             override;

  const char*     name()                               const;

  Symbol*         sym;
  Expr*           init;
  Expr*           exprType;
};


class SymExpr final : public Expr {
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
 ~SymExpr() override = default;

  DECLARE_COPY(SymExpr);
  SymExpr* copyInner(SymbolMap* map)                  override;

  void    replaceChild(Expr* old_ast, Expr* new_ast)  override;
  void    verify()                                    override;
  void    accept(AstVisitor* visitor)                 override;

  QualifiedType qualType()                            override;
  bool    isNoInitExpr()                        const override;
  GenRet  codegen()                                   override;
  void    prettyPrint(std::ostream* o)                override;

  Expr*   getFirstExpr()                              override;

  Symbol* symbol() const {
    return var;
  }

  void setSymbol(Symbol* s);
};

class UnresolvedSymExpr final : public Expr {
 public:
  const char* unresolved;

  UnresolvedSymExpr(const char* init_var);
 ~UnresolvedSymExpr() override = default;

  DECLARE_COPY(UnresolvedSymExpr);
  UnresolvedSymExpr* copyInner(SymbolMap* map)        override;

  void    replaceChild(Expr* old_ast, Expr* new_ast)  override;
  void    verify()                                    override;
  void    accept(AstVisitor* visitor)                 override;
  QualifiedType qualType()                            override;
  GenRet  codegen()                                   override;
  void    prettyPrint(std::ostream *o)                override;

  Expr*   getFirstExpr()                              override;
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
class ContextCallExpr final : public Expr {
public:
  ContextCallExpr();
 ~ContextCallExpr() override = default;

  DECLARE_COPY(ContextCallExpr);
  ContextCallExpr* copyInner(SymbolMap* map)              override;

  void           replaceChild(Expr* oldAst, Expr* newAst) override;

  void           verify()                                 override;
  void           accept(AstVisitor* visitor)              override;
  QualifiedType  qualType()                               override;
  GenRet         codegen()                                override;
  void           prettyPrint(std::ostream* o)             override;

  Expr*          getFirstExpr()                           override;

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


class NamedExpr final : public Expr {
 public:
  const char*     name;
  Expr*           actual;

  NamedExpr(const char* init_name, Expr* init_actual);
 ~NamedExpr() override = default;

  void    verify()                                    override;

  DECLARE_COPY(NamedExpr);
  NamedExpr* copyInner(SymbolMap* map)                override;

  void    replaceChild(Expr* old_ast, Expr* new_ast)  override;
  void    accept(AstVisitor* visitor)                 override;
  QualifiedType qualType()                            override;
  GenRet  codegen()                                   override;
  void    prettyPrint(std::ostream* o)                override;

  Expr*   getFirstExpr()                              override;
};

//
// Represents interface constraints written as:
//   implements InterfaceName(actualType...)
// or
//   actualType implements InterfaceName
//
class IfcConstraint final : public Expr {
public:
  static IfcConstraint* build(const char* name,
                              CallExpr* actuals);
  IfcConstraint(Expr* iifc);
 ~IfcConstraint() override = default;

  DECLARE_COPY(IfcConstraint);
  IfcConstraint* copyInner(SymbolMap* map)            override;
  GenRet codegen()                                    override;
  void   verify()                                     override;
  void   accept(AstVisitor* visitor)                  override;
  QualifiedType qualType()                            override;

  void   replaceChild(Expr* oldAst, Expr* newAst)     override;
  Expr*  getFirstExpr()                               override;
  Expr*  getNextExpr(Expr* expr)                      override;
  void   prettyPrint(std::ostream* o)                 override;

  InterfaceSymbol* ifcSymbol()  const;
  int              numActuals() const { return consActuals.length; }

  Expr* interfaceExpr;  // UnresolvedSymExpr -> SymExpr(InterfaceSymbol)
  AList consActuals;    // Exprs -> SymExprs of the constraint's actuals
};

// valid after scopeResolve
inline InterfaceSymbol* IfcConstraint::ifcSymbol() const {
  return toInterfaceSymbol(toSymExpr(interfaceExpr)->symbol());
}

inline bool Expr::inTree() {
  return parentSymbol != nullptr;
}

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


bool get_bool(Expr* e, uint64_t *i); // false is failure
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
llvm::AllocaInst* createVarLLVM(llvm::Type* type, const char* name);
llvm::AllocaInst* createVarLLVM(llvm::Type* type);

llvm::Value *convertValueToType(llvm::Value *value, llvm::Type *newType,
                                bool isSigned = false, bool force = false);
#endif

GenRet codegenValue(GenRet r);
GenRet codegenValuePtr(GenRet r);

GenRet createTempVar(const char* ctype);
GenRet createTempVar(Type* t);
GenRet createTempVarWith(GenRet v);

GenRet codegenDeref(GenRet toDeref);
GenRet codegenLocalDeref(GenRet toDeref);
GenRet codegenNullPointer();
GenRet codegenCast(const char* typeName, GenRet value, bool Cparens = true);

void codegenCallPrintf(const char* arg);

#endif
