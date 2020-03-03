/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

#include "CallExpr.h"

#include "astutil.h"
#include "AstVisitor.h"
#include "passes.h"
#include "stringutil.h"
#include "wellknown.h"


static void callExprHelper(CallExpr* call, BaseAST* arg);

CallExpr::CallExpr(BaseAST* base,
                   BaseAST* arg1,
                   BaseAST* arg2,
                   BaseAST* arg3,
                   BaseAST* arg4,
                   BaseAST* arg5) : Expr(E_CallExpr) {
  primitive  = NULL;
  baseExpr   = NULL;
  partialTag = false;
  methodTag  = false;
  square     = false;
  tryTag     = TRY_TAG_NONE;

  if (Symbol* b = toSymbol(base)) {
    baseExpr = new SymExpr(b);
  } else if (Expr* b = toExpr(base)) {
    baseExpr = b;
  } else {
    INT_FATAL(this, "Bad baseExpr in CallExpr constructor");
  }

  callExprHelper(this, arg1);
  callExprHelper(this, arg2);
  callExprHelper(this, arg3);
  callExprHelper(this, arg4);
  callExprHelper(this, arg5);

  argList.parent = this;

  gCallExprs.add(this);
}


CallExpr::CallExpr(PrimitiveOp* prim,
                   BaseAST*     arg1,
                   BaseAST*     arg2,
                   BaseAST*     arg3,
                   BaseAST*     arg4,
                   BaseAST*     arg5) : Expr(E_CallExpr) {
  primitive  = prim;
  baseExpr   = NULL;
  partialTag = false;
  methodTag  = false;
  square     = false;
  tryTag     = TRY_TAG_NONE;

  callExprHelper(this, arg1);
  callExprHelper(this, arg2);
  callExprHelper(this, arg3);
  callExprHelper(this, arg4);
  callExprHelper(this, arg5);

  argList.parent = this;

  gCallExprs.add(this);
}

CallExpr::CallExpr(PrimitiveTag prim,
                   BaseAST*     arg1,
                   BaseAST*     arg2,
                   BaseAST*     arg3,
                   BaseAST*     arg4,
                   BaseAST*     arg5) : Expr(E_CallExpr) {
  primitive  = primitives[prim];
  baseExpr   = NULL;
  partialTag = false;
  methodTag  = false;
  square     = false;
  tryTag     = TRY_TAG_NONE;

  callExprHelper(this, arg1);
  callExprHelper(this, arg2);
  callExprHelper(this, arg3);
  callExprHelper(this, arg4);
  callExprHelper(this, arg5);

  argList.parent = this;

  gCallExprs.add(this);
}

CallExpr::CallExpr(const char* name,
                   BaseAST*    arg1,
                   BaseAST*    arg2,
                   BaseAST*    arg3,
                   BaseAST*    arg4,
                   BaseAST*    arg5) : Expr(E_CallExpr) {
  primitive  = NULL;
  baseExpr   = new UnresolvedSymExpr(name);
  partialTag = false;
  methodTag  = false;
  square     = false;
  tryTag     = TRY_TAG_NONE;

  callExprHelper(this, arg1);
  callExprHelper(this, arg2);
  callExprHelper(this, arg3);
  callExprHelper(this, arg4);
  callExprHelper(this, arg5);

  argList.parent = this;

  gCallExprs.add(this);
}

static void callExprHelper(CallExpr* call, BaseAST* arg) {
  if (arg != NULL) {
    if (isSymbol(arg) == true || isExpr(arg) == true) {
      call->insertAtTail(arg);
    } else {
      INT_FATAL(call, "Bad argList in CallExpr constructor");
    }
  }
}

CallExpr::~CallExpr() {
}


bool CallExpr::isEmpty() const {
  return primitive == NULL && baseExpr == NULL;
}

bool CallExpr::isPrimitive() const {
  return primitive != NULL;
}

bool CallExpr::isPrimitive(PrimitiveTag primitiveTag) const {
  return primitive && primitive->tag == primitiveTag;
}

bool CallExpr::isPrimitive(const char* primitiveName) const {
  return primitive && !strcmp(primitive->name, primitiveName);
}

Expr* CallExpr::getFirstExpr() {
  Expr* retval = NULL;

  if (baseExpr != NULL) {
    retval = baseExpr->getFirstExpr();

  } else if (argList.head != NULL) {
    retval = argList.head->getFirstExpr();

  } else {
    retval = this;
  }

  return retval;
}

Expr* CallExpr::getNextExpr(Expr* expr) {
  Expr* retval = this;

  if (expr == baseExpr && argList.head != NULL) {
    retval = argList.head->getFirstExpr();
  }

  return retval;
}

void CallExpr::verify() {
  int fieldIndex       = 1;

  // If the methodToken is present, it is usually in slot 1
  int methodTokenIndex = 1;

  Expr::verify(E_CallExpr);

  if (parentExpr == NULL) {
    INT_FATAL(this, "Every CallExpr is expected to have a parentExpr");
  }

  if (argList.parent != this) {
    INT_FATAL(this, "Bad AList::parent in CallExpr");
  }

  verifyParent(baseExpr);

  for_actuals(actual, this) {
    verifyParent(actual);

    if (SymExpr* se = toSymExpr(actual)) {
      Symbol* sym = se->symbol();

      if (sym == gModuleToken) {
        // If the moduleToken exists, the methodToken is moved to slot 3
        if (fieldIndex == 1) {
          methodTokenIndex = 3;

        } else {
          INT_FATAL(this, "The module token must be the 1st argument.");
        }

      } else if (sym->type == dtMethodToken) {
        INT_ASSERT(sym == gMethodToken); // current state of affairs
        if (fieldIndex != methodTokenIndex) {
          INT_FATAL(this, "The method token is in the wrong slot.");
        }
      }
    }

    fieldIndex = fieldIndex + 1;
  }

  if (primitive != NULL) {
    switch (primitive->tag) {
    case PRIM_BLOCK_PARAM_LOOP:
    case PRIM_BLOCK_WHILEDO_LOOP:
    case PRIM_BLOCK_DOWHILE_LOOP:
    case PRIM_BLOCK_FOR_LOOP:
    case PRIM_BLOCK_BEGIN:
    case PRIM_BLOCK_COBEGIN:
    case PRIM_BLOCK_COFORALL:
    case PRIM_BLOCK_ON:
    case PRIM_BLOCK_ELIDED_ON:
    case PRIM_BLOCK_BEGIN_ON:
    case PRIM_BLOCK_COBEGIN_ON:
    case PRIM_BLOCK_COFORALL_ON:
    case PRIM_BLOCK_LOCAL:
      if (isBlockStmt(parentExpr) == false) {
        INT_FATAL(this, "blockInfo-type CallExpr not in a BlockStmt");
      }
      break;

    case PRIM_BLOCK_UNLOCAL:
      INT_FATAL("PRIM_BLOCK_UNLOCAL between passes");
      break;

    default:
      break; // do nothing
    }
  }

  verifyNotOnList(baseExpr);
}

CallExpr* CallExpr::copyInner(SymbolMap* map) {
  CallExpr* _this = 0;

  if (primitive != NULL) {
    _this = new CallExpr(primitive);

  } else {
    _this = new CallExpr(COPY_INT(baseExpr));
  }

  for_actuals(expr, this) {
    _this->insertAtTail(COPY_INT(expr));
  }

  _this->primitive  = primitive;
  _this->partialTag = partialTag;
  _this->methodTag  = methodTag;
  _this->square     = square;
  _this->tryTag     = tryTag;

  return _this;
}


void CallExpr::replaceChild(Expr* oldAst, Expr* newAst) {
  if (oldAst == baseExpr) {
    baseExpr = newAst;

  } else {
    INT_FATAL(this, "Unexpected case in CallExpr::replaceChild");
  }
}


void CallExpr::insertAtHead(BaseAST* ast) {
  Expr* toInsert = NULL;

  if (Symbol* a = toSymbol(ast)) {
    toInsert = new SymExpr(a);

  } else {
    toInsert = toExpr(ast);
  }

  argList.insertAtHead(toInsert);

  parent_insert_help(this, toInsert);
}


void CallExpr::insertAtTail(BaseAST* ast) {
  Expr* toInsert = NULL;

  if (Symbol* a = toSymbol(ast)) {
    toInsert = new SymExpr(a);

  } else {
    toInsert = toExpr(ast);
  }

  argList.insertAtTail(toInsert);

  parent_insert_help(this, toInsert);
}

void CallExpr::setUnresolvedFunction(const char* name) {
  // Currently a PRIM_OP
  if (primitive != NULL) {
    primitive = NULL;
    baseExpr  = new UnresolvedSymExpr(astr(name));

    parent_insert_help(this, baseExpr);

  } else if (UnresolvedSymExpr* use = toUnresolvedSymExpr(baseExpr)) {
    use->unresolved = astr(name);

  } else if (SymExpr*           se  = toSymExpr(baseExpr)) {
    se->replace(new UnresolvedSymExpr(name));

  } else {
    INT_ASSERT(false);
  }
}

bool CallExpr::isResolved() const {
  return (resolvedFunction() != NULL) ? true : false;
}

FnSymbol* CallExpr::resolvedFunction() const {
  FnSymbol* retval = NULL;

  // A PRIM-OP
  if (primitive != NULL) {
    INT_ASSERT(baseExpr  == NULL);

  // A Chapel call
  } else if (baseExpr != NULL) {
    if (isUnresolvedSymExpr(baseExpr) == true) {

    } else if (SymExpr* base = toSymExpr(baseExpr)) {
      if (FnSymbol* fn = toFnSymbol(base->symbol())) {
        retval = fn;

      // Probably an array index
      } else if (isArgSymbol(base->symbol())  == true ||
                 isVarSymbol(base->symbol())  == true) {

      // A type specifier
      } else if (isTypeSymbol(base->symbol()) == true) {

      } else {
        INT_ASSERT(false);
      }

    } else if (CallExpr* subCall = toCallExpr(baseExpr)) {
      // Confirm that this is a partial call, but only if the call is not
      // within a DefExpr (indicated by not having a stmt-expr)
      if (subCall->getStmtExpr() != NULL) {
        INT_ASSERT(subCall->partialTag == true);
      }

    } else {
      INT_ASSERT(false);
    }

  // The CallExpr has been purged during resolve
  } else {
    INT_ASSERT(false);
  }

  return retval;
}

void CallExpr::setResolvedFunction(FnSymbol* fn) {
  // Currently a PRIM_OP
  if (primitive != NULL) {
    primitive = NULL;
    baseExpr  = new SymExpr(fn);

    parent_insert_help(this, baseExpr);

  } else if (isUnresolvedSymExpr(baseExpr) == true) {
    baseExpr->replace(new SymExpr(fn));

  } else if (SymExpr* se = toSymExpr(baseExpr)) {
    se->setSymbol(fn);

  } else {
    INT_ASSERT(false);
  }
}

// This function returns the resolved function, if it's totally resolved,
// or some resolved virtual function (probably a virtual parent that
// won't actually called at runtime) if it's a virtual method call.
// This function is useful for transformations on calls that work
// depending on the called function's signature (since the virtual
// parent and children will have the same signature).
FnSymbol* CallExpr::resolvedOrVirtualFunction() const {
  // The common case of a user-level call to a resolved function
  FnSymbol* retval = this->resolvedFunction();

  // Also handle the PRIMOP for a virtual method call
  if (retval == NULL) {
    if (isPrimitive(PRIM_VIRTUAL_METHOD_CALL) == true) {
      SymExpr* arg1 = toSymExpr(get(1));

     retval = toFnSymbol(arg1->symbol());
    }
  }

  return retval;
}

FnSymbol* CallExpr::theFnSymbol() const {
  FnSymbol* retval = NULL;

  if (SymExpr* base = toSymExpr(baseExpr)) {
    retval = toFnSymbol(base->symbol());
  }

  return retval;
}

bool CallExpr::isNamed(const char* name) const {
  bool retval = false;

  if (SymExpr* base = toSymExpr(baseExpr)) {
    retval = (strcmp(base->symbol()->name, name) == 0) ? true : false;

  } else if (UnresolvedSymExpr* base = toUnresolvedSymExpr(baseExpr)) {
    retval = (strcmp(base->unresolved,     name) == 0) ? true : false;

  } else {
    retval = false;
  }

  return retval;
}

// 'name' must be canonicalized
bool CallExpr::isNamedAstr(const char* name) const {
  bool retval = false;

  if (SymExpr* base = toSymExpr(baseExpr)) {
    retval = (base->symbol()->name == name) ? true : false;

  } else if (UnresolvedSymExpr* base = toUnresolvedSymExpr(baseExpr)) {
    retval = (base->unresolved     == name) ? true : false;

  } else {
    retval = false;
  }

  return retval;
}

int CallExpr::numActuals() const {
  return argList.length;
}


Expr* CallExpr::get(int index) const {
  return argList.get(index);
}


FnSymbol* CallExpr::findFnSymbol() {
  FnSymbol* retval = NULL;

  if (SymExpr* variable = toSymExpr(baseExpr)) {
    retval = toFnSymbol(variable->symbol());
  }

  if (retval == NULL) {
    INT_FATAL(this, "Cannot find FnSymbol in CallExpr");
  }

  return retval;
}

bool CallExpr::isCast(void) {
  return isNamedAstr(astr_cast);
}

Expr* CallExpr::castFrom(void) {
  INT_ASSERT(isCast());

  return get(2);
}

Expr* CallExpr::castTo(void) {
  INT_ASSERT(isCast());

  return get(1);
}

CallExpr* createCast(BaseAST* src, BaseAST* toType) {
  return new CallExpr(astr_cast, toType, src);
}

QualifiedType CallExpr::qualType(void) {
  QualifiedType retval(NULL);

  if (primitive) {
    retval = primitive->returnInfo(this);

  } else if (isResolved()) {
    FnSymbol* fn = resolvedFunction();
    Qualifier q  = QUAL_UNKNOWN;

    if (fn->retType->isRef()) {
      q = QUAL_REF;

    } else if (fn->retType->isWideRef()) {
      q = QUAL_WIDE_REF;

    } else if (fn->retTag == RET_VALUE) {
      q = QUAL_VAL;

    } else {
      q = QUAL_UNKNOWN;
    }

    retval = QualifiedType(q, fn->retType);
  } else if (SymExpr* se = toSymExpr(baseExpr)) {
    // Handle type constructor calls
    Type* retType = dtUnknown;
    if (se->symbol()->hasFlag(FLAG_TYPE_VARIABLE)) {
      AggregateType* at = toAggregateType(se->typeInfo());
      if (at && at->isGeneric() == false) {
        retType = at;
      } else if (isPrimitiveType(se->typeInfo()) && numActuals() == 0) {
        // (call uint(64) 8) represents 'uint(8)', so we don't want to return
        // a ``uint(64)`` unless there are zero arguments
        retType = se->typeInfo();
      }
    }

    retval = QualifiedType(QUAL_UNKNOWN, retType);

  } else {
    retval = QualifiedType(dtUnknown);
  }

  return retval;
}

void CallExpr::prettyPrint(std::ostream* o) {
  bool array   = false;
  bool unusual = false;

  if (FnSymbol* fn = theFnSymbol()) {
    if      (fn->hasFlag(FLAG_BEGIN_BLOCK) == true) {
      *o << "begin";
    }

    else if (fn->hasFlag(FLAG_ON_BLOCK)    == true) {
      *o << "on";
    }
  }

  if (baseExpr != NULL) {
    if (UnresolvedSymExpr *expr = toUnresolvedSymExpr(baseExpr)) {
      if (strcmp(expr->unresolved, "*") == 0){
        unusual = true;
        argList.first()->prettyPrint(o);
        *o << "*(";
        argList.last()->prettyPrint(o);
        *o << ")";

      } else if (strcmp(expr->unresolved, "chpl_build_bounded_range") == 0) {
        // Note that this code path is only used by chpldoc to create function
        // return signatures and the only place a range will show up is in a
        // fully specified array, in which case the range must be fully bounded
        argList.first()->prettyPrint(o);
        *o << "..";
        argList.last()->prettyPrint(o);
        unusual = true;

      } else if (strcmp(expr->unresolved, "chpl__ensureDomainExpr") == 0) {
        unusual = true;

        for_alist(expr, argList) {
          if (expr != argList.first()) {
            *o << ", ";
          }

          expr->prettyPrint(o);
        }

      } else if (strcmp(expr->unresolved,
                        "chpl__buildArrayRuntimeType") == 0) {
        *o << "[";
        array = true;

      } else if (strcmp(expr->unresolved,
                        "chpl__buildDomainRuntimeType") == 0) {
        *o << "domain(";
        argList.last()->prettyPrint(o);
        *o << ")";
        unusual = true;

      } else if (strcmp(expr->unresolved,
                        "_build_tuple") != 0) {
        baseExpr->prettyPrint(o);
      }

    } else {
      baseExpr->prettyPrint(o);
    }

  }

  if (!array && !unusual) {
    *o << "(";
  }

  if (!unusual) {
    for_alist(expr, argList) {
      if (expr != argList.first()) {
        if (array && expr == argList.last()) {
          *o << "] ";
        } else {
          *o << ", ";
        }
      }
      expr->prettyPrint(o);
    }

    if (array && argList.first() == argList.last()) {
      *o << "]";
    }
  }

  if (!array && !unusual) {
    *o << ")";
  }
}

void CallExpr::accept(AstVisitor* visitor) {
  if (visitor->enterCallExpr(this) == true) {
    if (baseExpr) {
      baseExpr->accept(visitor);
    }

    for_alist(next_ast, argList) {
      next_ast->accept(visitor);
    }

    visitor->exitCallExpr(this);
  }
}

bool CallExpr::isRefExternStarTuple(Symbol* formal, Expr* actual) const {
  Symbol* formalSym  = formal->type->symbol;
  Symbol* formalVal  = formalSym->getValType()->symbol;

  bool  retval       = false;

  if (formal->isRef()                     == true &&
      formalVal->hasFlag(FLAG_STAR_TUPLE) == true &&
      actual->isRef()                     == true) {
    retval = true;
  }

  return retval;
}

//
// 2017/07/09: resolveBlockStmt() currently relies on
//
//    void for_exprs_postorder(expr, blockStmt);
//
// to traverse the sub-expressions within a given block-stmt.  This
// implementation choice complicates any transformation that would otherwise
// replace one statement with a different one; a simple use of
//
//    node->replace(other);
//
// is likely to interfere with the internal sequencing within this macro.
//
// The work-around is to insert the new, fully resolved, statement
// immediately before the "current" statement and then convert the current
// statement in to a NOOP.  This will ensure that statements will be
// sequenced correctly.
//

void CallExpr::convertToNoop() {
  if (baseExpr != NULL) {
    baseExpr->remove();
  }

  while (numActuals() > 0) {
    get(1)->remove();
  }

  primitive = primitives[PRIM_NOOP];
}

// This builds an allocation of enough space to hold a variable of the
// given type.
//
// This function should be used *before* resolution
CallExpr* callChplHereAlloc(Type* type, VarSymbol* md) {
  INT_ASSERT(resolved == false);

  // Since the type is not necessarily known, resolution will fix up
  // this sizeof() call to take the resolved type of s as an argument
  CallExpr*  sizeExpr  = new CallExpr(PRIM_SIZEOF_BUNDLE,
                                      new SymExpr(type->symbol));
  VarSymbol* mdExpr    = (md != NULL) ? md : newMemDesc(type);
  CallExpr*  allocExpr = new CallExpr("chpl_here_alloc", sizeExpr, mdExpr);

  // Again, as we don't know the type yet, we leave it to resolution
  // to put in the cast to the proper type
  return allocExpr;
}

// This insert normalized call expressions for allocation of enough
// space to hold a variable of the given type.
//
// This function should be used *after* resolution
void insertChplHereAlloc(Expr*      call,
                         bool       insertAfter,
                         Symbol*    sym,
                         Type*      t,
                         VarSymbol* md) {
  INT_ASSERT(resolved);

  AggregateType* ct        = toAggregateType(toTypeSymbol(t->symbol)->type);
  Symbol*        sizeTmp   = newTemp("chpl_here_alloc_size", SIZE_TYPE);
  CallExpr*      sizeExpr  = new CallExpr(PRIM_MOVE,
                                          sizeTmp,
                                          new CallExpr(PRIM_SIZEOF_BUNDLE,
                                                       (ct != NULL) ?
                                                       ct->symbol   :
                                                       t->symbol));
  VarSymbol*     mdExpr    = (md != NULL) ? md : newMemDesc(t);
  Symbol*        allocTmp  = newTemp("chpl_here_alloc_tmp", dtCVoidPtr);
  CallExpr*      allocExpr = new CallExpr(PRIM_MOVE,
                                          allocTmp,
                                          new CallExpr(gChplHereAlloc,
                                                       sizeTmp,
                                                       mdExpr));
  CallExpr*      castExpr  = new CallExpr(PRIM_MOVE,
                                          sym,
                                          new CallExpr(PRIM_CAST,
                                                       t->symbol,
                                                       allocTmp));
  if (insertAfter == true) {
    call->insertAfter(castExpr);
    call->insertAfter(allocExpr);
    call->insertAfter(sizeExpr);
    call->insertAfter(new DefExpr(allocTmp));
    call->insertAfter(new DefExpr(sizeTmp));

  } else {
    call->insertBefore(new DefExpr(sizeTmp));
    call->insertBefore(new DefExpr(allocTmp));
    call->insertBefore(sizeExpr);
    call->insertBefore(allocExpr);
    call->insertBefore(castExpr);
  }
}

CallExpr* callChplHereFree(BaseAST* p) {
  CallExpr* retval = NULL;

  if (p->typeInfo()->symbol->hasFlag(FLAG_DATA_CLASS) == false) {
    CallExpr* castExpr = new CallExpr(PRIM_CAST_TO_VOID_STAR, p);

    if (resolved == false) {
      retval = new CallExpr("chpl_here_free", castExpr);

    } else {
      retval = new CallExpr(gChplHereFree,    castExpr);
    }

  } else {
    INT_FATAL(p->typeInfo()->symbol, "cannot delete data class");
  }

  return retval;
}

FnSymbol* resolvedToTaskFun(CallExpr* call) {
  FnSymbol* retval = NULL;

  if (FnSymbol* cfn = call->resolvedFunction()) {
    if (isTaskFun(cfn) == true) {
      retval = cfn;
    }
  }

  return retval;
}

// For use during/after resolution. Returns 'true' if the call is to
// a function returning a record by value or an initializer.
// Handles both 'move lhs, someCall()' and 'someCall(retarg=lhs)' forms.
// lhsSe is the SymExpr indicating what is being set.
// initOrCtor is the user call (e.g. someCall in the examples above).
bool isRecordInitOrReturn(CallExpr* call, SymExpr*& lhsSe, CallExpr*& initOrCtor) {

  if (call->isPrimitive(PRIM_MOVE) ||
      call->isPrimitive(PRIM_ASSIGN)) {
    if (CallExpr* rhsCallExpr = toCallExpr(call->get(2))) {
      if (rhsCallExpr->resolvedOrVirtualFunction()) {
        Type* t = NULL;
        if (call->isPrimitive(PRIM_MOVE))
          t = rhsCallExpr->typeInfo();
        else
          t = rhsCallExpr->getValType();
        if (AggregateType* at = toAggregateType(t)) {
          if (isRecord(at)) {
            SymExpr* se = toSymExpr(call->get(1));
            INT_ASSERT(se);
            lhsSe = se;
            initOrCtor = rhsCallExpr;
            return true;
          }
        }
      }
    }
  }

  if (FnSymbol* calledFn = call->resolvedOrVirtualFunction()) {
    if (calledFn->isMethod() &&
        (calledFn->name == astrInit || calledFn->name == astrInitEquals)) {
      SymExpr* se = toSymExpr(call->get(1));
      INT_ASSERT(se);
      Symbol* sym = se->symbol();
      if (isRecord(sym->type)) {
        lhsSe = se;
        initOrCtor = call;
        return true;
      }
    } else if (calledFn->hasFlag(FLAG_FN_RETARG)) {
      for_formals_actuals(formal, actual, call) {
        if (formal->hasFlag(FLAG_RETARG)) {
          if (isRecord(formal->getValType())) {
            SymExpr* se = toSymExpr(actual);
            INT_ASSERT(se);
            lhsSe = se;
            initOrCtor = call;
            return true;
          }
        }
      }
    }
  }

  lhsSe = NULL;
  initOrCtor = NULL;
  return false;
}
