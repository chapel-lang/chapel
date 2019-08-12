/*
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

#include "preFold.h"

#include "astutil.h"
#include "buildDefaultFunctions.h"
#include "DecoratedClassType.h"
#include "driver.h"
#include "ForallStmt.h"
#include "iterator.h"
#include "ParamForLoop.h"
#include "passes.h"
#include "resolution.h"
#include "resolveFunction.h"
#include "resolveIntents.h"
#include "scopeResolve.h"
#include "stlUtil.h"
#include "stringutil.h"
#include "symbol.h"
#include "typeSpecifier.h"
#include "visibleFunctions.h"

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include <inttypes.h>

#include <iostream>
#include <sstream>

// lookup table/cache for function types and their representative parents
static std::map<std::string,
                std::pair<AggregateType*, FnSymbol*> >     functionTypeMap;


//lookup table/cache for function captures
static std::map<FnSymbol*, FnSymbol*>                      functionCaptureMap;

static Expr*          preFoldPrimOp(CallExpr* call);

static Expr*          preFoldNamed (CallExpr* call);

static Expr*          resolveTupleIndexing(CallExpr* call,
                                           Symbol*   baseVar);

static Symbol*        determineQueriedField(CallExpr* call);

static bool           isInstantiatedField(Symbol* field);

static Expr*          createFunctionAsValue(CallExpr* call);

static AggregateType* createOrFindFunTypeFromAnnotation(AList&    argList,
                                                        CallExpr* call);

static Expr*          dropUnnecessaryCast(CallExpr* call);

static bool           isNormalField(Symbol* field);

static std::string    buildParentName(AList& argList,
                                      bool   isFormal,
                                      Type*  retType,
                                      bool   throws);

static AggregateType* createAndInsertFunParentClass(CallExpr*   call,
                                                    const char* name);

static FnSymbol*      createAndInsertFunParentMethod(CallExpr*      call,
                                                     AggregateType* parent,
                                                     AList&         argList,
                                                     bool           isFormal,
                                                     Type*          retType,
                                                     bool           throws);

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

Expr* preFold(CallExpr* call) {
  Expr* baseExpr = call->baseExpr;
  Expr* retval   = call;

  if (call->isPrimitive() == true) {
    if (Expr* tmp = preFoldPrimOp(call)) {
      retval = tmp;
    }

  } else if (isUnresolvedSymExpr(baseExpr) == true) {
    if (Expr* tmp = preFoldNamed(call)) {
      retval = tmp;
    }

  } else if (SymExpr* symExpr = toSymExpr(baseExpr)) {
    // Primitive typeSpecifier -> SymExpr
    if (Type* type = typeForTypeSpecifier(call, true)) {
      retval = new SymExpr(type->symbol);

      call->replace(retval);

    } else {
      if (symExpr->symbol()->hasFlag(FLAG_TYPE_VARIABLE) &&
          symExpr->getValType()->symbol->hasFlag(FLAG_TUPLE) == false) {
        // Type constructor calls OK
      } else if (isLcnSymbol(symExpr->symbol()) == true) {
        baseExpr->replace(new UnresolvedSymExpr("this"));

        call->insertAtHead(baseExpr);
        call->insertAtHead(gMethodToken);
      }

      if (Expr* tmp = preFoldNamed(call)) {
        retval = tmp;
      }
    }

  } else if (CallExpr* callExpr = toCallExpr(baseExpr)) {
    if (callExpr->partialTag == true) {
      for_actuals_backward(actual, callExpr) {
        actual->remove();
        call->insertAtHead(actual);
      }

      callExpr->replace(callExpr->baseExpr->remove());

      if (Expr* tmp = preFoldNamed(callExpr)) {
        retval = tmp;
      }

    } else {
      VarSymbol* thisTemp = newTemp("_this_tmp_");

      thisTemp->addFlag(FLAG_EXPR_TEMP);

      callExpr->replace(new UnresolvedSymExpr("this"));

      retval = new CallExpr(PRIM_MOVE, thisTemp, callExpr);

      call->insertAtHead(new SymExpr(thisTemp));
      call->insertAtHead(gMethodToken);

      call->getStmtExpr()->insertBefore(new DefExpr(thisTemp));
      call->getStmtExpr()->insertBefore(retval);
    }

  } else {
    INT_ASSERT(false);
  }

  // ensure result of pre-folding is in the AST
  INT_ASSERT(retval->parentSymbol);

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static bool isFollowerITer(FnSymbol* iter) {
  // Follower iterator is not resolved yet - can't use isFollowerIterator().
  // Instead, heuristically just look for a "followThis" formal.
  // This is OK because we are in compiler-generated loopexpr function.
  for_formals(formal, iter)
    if (!strcmp(formal->name, iterFollowthisArgname))
      return true;
  return false;
}

static FnSymbol* findForallexprFollower(FnSymbol* serialIter) {
  if (!isLoopExprFun(serialIter))
    // Not a forall-expression.
    return NULL;

  // All iterators are defined in the same block - loopexpr function's body.
  BlockStmt* parent = toBlockStmt(serialIter->defPoint->parentExpr);
  for (Expr* curr = parent->body.head; curr != NULL; curr = curr->next) {
    if (DefExpr* def = toDefExpr(curr))
      if (FnSymbol* fn = toFnSymbol(def->sym))
        if (fn->name == serialIter->name)
          if (isFollowerITer(fn))
            return fn;
  }

  // The loopexpr function does not define parallel iterators
  // when it implements a (serial) for-expression.
  return NULL;
}

static Expr* preFoldPrimOp(CallExpr* call) {
  Expr* retval = NULL;

  switch (call->primitive->tag) {

  case PRIM_ADDR_OF: {
    // remove set ref if already a reference
    if (call->get(1)->typeInfo()->symbol->hasFlag(FLAG_REF) ||
        isTupleContainingOnlyReferences(call->get(1)->typeInfo())) {
      retval = call->get(1)->remove();

      call->replace(retval);
    }
    break;
  }

  case PRIM_BLOCK_PARAM_LOOP: {
    ParamForLoop* paramLoop = toParamForLoop(call->parentExpr);

    retval = paramLoop->foldForResolve();

    break;
  }

  case PRIM_GATHER_TESTS: {
    int  totalTest = 0;
    forv_Vec(FnSymbol, fn, gFnSymbols) {
      if (fn->throwsError()) {
        ModuleSymbol *mod = fn->getModule();
        if (mod->modTag == MOD_USER) {
          if (fn->numFormals() == 1) {
            if (!fn->hasFlag(FLAG_GENERIC) && fn->instantiatedFrom == NULL) {
              const char* name = astr(fn->name);

              // temporarily add a call to try resolving.
              CallExpr* tryCall = new CallExpr(name);
              // Add our new call to the AST temporarily.
              call->getStmtExpr()->insertAfter(tryCall);

              // copy actual args into tryCall.
              for_actuals(actual, call) {
                tryCall->insertAtTail(actual->copy());
              }

              // Try to resolve it.
              if (tryResolveCall(tryCall)) {
                totalTest++;
              }

              // remove the call from the AST
              tryCall->remove();
            }
          }
        }
        
      }
    }
    retval=new SymExpr(new_IntSymbol(totalTest));
    call->replace(retval);
    break;
  }

  case PRIM_CALL_RESOLVES:
  case PRIM_METHOD_CALL_RESOLVES: {
    Expr* fnName   = NULL;
    Expr* callThis = NULL;
    int   firstArg = 0;

    // this would be easier if we had a non-normalized AST!
    // That is, if this call could contain a whole expression subtree.
    if (call->isPrimitive(PRIM_METHOD_CALL_RESOLVES) ) {
      // get(1) should be a receiver
      // get(2) should be a string function name.
      callThis = call->get(1);
      fnName   = call->get(2);
      firstArg = 3;

    } else {
      // get(1) should be a string function name.
      fnName   = call->get(1);
      firstArg = 2;
    }

    VarSymbol* var = toVarSymbol(toSymExpr(fnName)->symbol());
    Immediate* imm = var->immediate;

    if (imm == NULL) {
      INT_FATAL(call, "proc name required");
    }

    if (imm->const_kind != CONST_KIND_STRING) {
      INT_FATAL(call, "proc name must be a string");
    }

    const char* name = imm->v_string;

    // temporarily add a call to try resolving.
    CallExpr* tryCall = NULL;

    if (call->isPrimitive(PRIM_METHOD_CALL_RESOLVES)) {
      tryCall = new CallExpr(new UnresolvedSymExpr(name),
                             gMethodToken,
                             callThis->copy());

    } else {
      tryCall = new CallExpr(name);
    }

    // Add our new call to the AST temporarily.
    call->getStmtExpr()->insertAfter(tryCall);

    // copy actual args into tryCall.
    int i = 1;

    for_actuals(actual, call) {
      if (i >= firstArg ) { // skip fn name, maybe method receiver
        tryCall->insertAtTail(actual->copy());
      }

      i++;
    }

    // Try to resolve it.
    if (tryResolveCall(tryCall)) {
      retval = new SymExpr(gTrue);

    } else {
      retval = new SymExpr(gFalse);
    }

    // remove the call from the AST
    tryCall->remove();

    call->replace(retval);

    break;
  } // PRIM_CALL_RESOLVES, PRIM_METHOD_CALL_RESOLVES

  case PRIM_CAPTURE_FN_FOR_CHPL:
  case PRIM_CAPTURE_FN_FOR_C: {
    retval = createFunctionAsValue(call);
    call->replace(retval);
    break;
  }

  case PRIM_CREATE_FN_TYPE: {
    AggregateType* parent = createOrFindFunTypeFromAnnotation(call->argList,
                                                              call);
    retval = new SymExpr(parent->symbol);
    call->replace(retval);
    break;
  }

  case PRIM_DEREF: {
    // remove deref if arg is already a value
    if (!call->get(1)->typeInfo()->symbol->hasFlag(FLAG_REF)) {
      retval = call->get(1)->remove();
      call->replace(retval);
    }
    break;
  }

  case PRIM_FIELD_BY_NUM: {
    // if call->get(1) is a reference type, dereference it
    Type*          t          = canonicalDecoratedClassType(call->get(1)->getValType());
    AggregateType* classType  = toAggregateType(t);

    VarSymbol*     var        = toVarSymbol(toSymExpr(call->get(2))->symbol());

    int            fieldNum   = var->immediate->int_value();
    int            fieldCount = 0;
    const char*    name       = NULL;

    for_fields(field, classType) {
      if (isNormalField(field) == true) {
        fieldCount++;

        if (fieldCount == fieldNum) {
          name = field->name;
        }
      }
    }

    if (name == NULL) {
      USR_FATAL(call,
                "'%d' is not a valid field number for %s",
                fieldNum,
                toString(classType));
    }

    retval = new CallExpr(PRIM_GET_MEMBER,
                          call->get(1)->copy(),
                          new_CStringSymbol(name));

    call->replace(retval);

    break;
  }

  case PRIM_FIELD_NAME_TO_NUM: {
    Type*          t          = canonicalDecoratedClassType(call->get(1)->getValType());
    AggregateType* classType  = toAggregateType(t);

    VarSymbol*     var        = toVarSymbol(toSymExpr(call->get(2))->symbol());
    Immediate*     imm        = var->immediate;

    INT_ASSERT(imm->const_kind == CONST_KIND_STRING);

    const char*    fieldName  = imm->v_string;
    int            fieldCount = 0;
    int            num        = 0;

    for_fields(field, classType) {
      if (isNormalField(field) == true) {
        fieldCount++;

        if (strcmp(field->name, fieldName) == 0) {
          num = fieldCount;
        }
      }
    }

    retval = new SymExpr(new_IntSymbol(num));

    call->replace(retval);

    break;
  }

  case PRIM_FIELD_NUM_TO_NAME: {
    Type*          t          = canonicalDecoratedClassType(call->get(1)->getValType());
    AggregateType* classType  = toAggregateType(t);

    classType = toAggregateType(classType->getValType());

    VarSymbol*     var        = toVarSymbol(toSymExpr(call->get(2))->symbol());

    INT_ASSERT(var != NULL);

    int            fieldNum   = var->immediate->int_value();
    int            fieldCount = 0;
    const char*    name       = NULL;

    for_fields(field, classType) {
      if (isNormalField(field) == true) {
        fieldCount++;

        if (fieldCount == fieldNum) {
          name = field->name;
        }
      }
    }

    if (name == NULL) {
      // In this case, we ran out of fields without finding the number
      // specified.  This is the user's error.
      USR_FATAL(call,
                "'%d' is not a valid field number for %s",
                fieldNum,
                toString(classType));
    }

    retval = new SymExpr(new_StringSymbol(name));

    call->replace(retval);

    break;
  }

  case PRIM_GET_COMPILER_VAR: {
    // Resolving Primitive PRIM_GET_COMPILER_VAR
    SymExpr*    se1     = toSymExpr(call->get(1));
    Immediate*  chplEnv = toVarSymbol(se1->symbol())->immediate;
    std::string envKey;

    // Check if this immediate is a string
    if (chplEnv->const_kind == CONST_KIND_STRING) {
      envKey = chplEnv->v_string;

    } else {
      USR_FATAL(call, "expected immediate of type string");
    }

    // Check if string is in envMap, and replace retval with mapped value
    if (envMap.find(envKey) != envMap.end()) {
      retval = new SymExpr(new_StringSymbol(envMap[envKey]));

      call->replace(retval);

    } else {
      USR_FATAL(call,
                "primitive string does not match any environment variable");
    }

    break;
  }

  case PRIM_GET_SVEC_MEMBER: {
    // Convert these to PRIM_GET_SVEC_MEMBER_VALUE if the
    // field in question is a reference.
    // An alternative to this transformation here would be
    // to build homog. tuple accessor entirely in tuples.cpp.
    AggregateType* ct = toAggregateType(call->get(1)->getValType());

    if (ct && ct->symbol->hasFlag(FLAG_STAR_TUPLE)) {
      FnSymbol* inFn = toFnSymbol(call->parentSymbol);

      if (inFn && inFn->hasFlag(FLAG_STAR_TUPLE_ACCESSOR)) {
        QualifiedType fieldType = ct->getFieldType(call->get(2));

        if (fieldType.type() && fieldType.isRef()) {
          if (call->isPrimitive(PRIM_GET_SVEC_MEMBER)) {
            Expr* base  = call->get(1);
            Expr* field = call->get(2);

            retval = new CallExpr(PRIM_GET_SVEC_MEMBER_VALUE,
                                  base->remove(),
                                  field->remove());

            call->replace(retval);
          }
        }
      }
    }

    break;
  }

  case PRIM_IS_ATOMIC_TYPE: {
    if (isAtomicType(call->get(1)->typeInfo())) {
      retval = new SymExpr(gTrue);
    } else {
      retval = new SymExpr(gFalse);
    }

    call->replace(retval);

    break;
  }

  case PRIM_IS_EXTERN_TYPE: {
    if (call->get(1)->typeInfo()->symbol->hasFlag(FLAG_EXTERN)) {
      retval = new SymExpr(gTrue);
    } else {
      retval = new SymExpr(gFalse);
    }

    call->replace(retval);

    break;
  }

  case PRIM_IS_ABS_ENUM_TYPE: {
    EnumType* et = toEnumType(call->get(1)->typeInfo());
    if (et && et->isAbstract()) {
      retval = new SymExpr(gTrue);
    } else {
      retval = new SymExpr(gFalse);
    }

    call->replace(retval);

    break;
  }

  case PRIM_IS_GENERIC_TYPE: {
    Type* t = call->get(1)->typeInfo();

    if (t->symbol->hasFlag(FLAG_GENERIC)) {
      retval = new SymExpr(gTrue);
    } else {
      retval = new SymExpr(gFalse);
    }

    call->replace(retval);

    break;
  }


  case PRIM_IS_CLASS_TYPE: {
    Type* t = call->get(1)->typeInfo();

    if (isClassLike(t) &&
        !t->symbol->hasFlag(FLAG_EXTERN)) {
      retval = new SymExpr(gTrue);
    } else if (isManagedPtrType(t)) {
      retval = new SymExpr(gTrue);
    } else {
      retval = new SymExpr(gFalse);
    }

    call->replace(retval);

    break;
  }

  case PRIM_IS_NILABLE_CLASS_TYPE:
  case PRIM_IS_NON_NILABLE_CLASS_TYPE: {
    Type* t = call->get(1)->typeInfo();

    if (isManagedPtrType(t)) {
      t = getManagedPtrBorrowType(t);
    }

    bool value = false;
    if (isClassLike(t) &&
        !t->symbol->hasFlag(FLAG_EXTERN)) {
      ClassTypeDecorator d = classTypeDecorator(t);
      if (call->isPrimitive(PRIM_IS_NILABLE_CLASS_TYPE))
        value = isDecoratorNilable(d);
      else if (call->isPrimitive(PRIM_IS_NON_NILABLE_CLASS_TYPE))
        value = isDecoratorNonNilable(d);
    }

    if (value)
      retval = new SymExpr(gTrue);
    else
      retval = new SymExpr(gFalse);

    call->replace(retval);

    break;
  }


  case PRIM_TO_UNMANAGED_CLASS:
  case PRIM_TO_BORROWED_CLASS:
  case PRIM_TO_NILABLE_CLASS:
  case PRIM_TO_NON_NILABLE_CLASS: {
    Type* totype = call->typeInfo();
    Expr* e = call->get(1);

    if (isTypeExpr(e)) {
      retval = new SymExpr(totype->symbol);
      call->replace(retval);
    } else {
      if (isManagedPtrType(e->typeInfo())) {
        VarSymbol* tmp = newTemp("borrow_tmp");
        call->getStmtExpr()->insertBefore(new DefExpr(tmp));
        CallExpr* c = new CallExpr("borrow", gMethodToken, e->remove());
        CallExpr* move = new CallExpr(PRIM_MOVE, tmp, c);
        call->getStmtExpr()->insertBefore(move);
        // so it is resolved
        resolveExpr(c);
        resolveExpr(move);
        retval = new CallExpr(PRIM_CAST, totype->symbol, tmp);
        call->replace(retval);
      } else {
        retval = new CallExpr(PRIM_CAST, totype->symbol, e->remove());
        call->replace(retval);
      }
    }


    break;
  }

  case PRIM_IS_POD: {
    Type* t = call->get(1)->typeInfo();

    // call propagateNotPOD to set FLAG_POD/FLAG_NOT_POD
    propagateNotPOD(t);

    if (isPOD(t)) {
      retval = new SymExpr(gTrue);
    } else {
      retval = new SymExpr(gFalse);
    }

    call->replace(retval);

    break;
  }

  case PRIM_NEEDS_AUTO_DESTROY: {
    Type* t = call->get(1)->typeInfo();

    // call propagateNotPOD to set FLAG_POD/FLAG_NOT_POD
    propagateNotPOD(t);

    bool needsDestroy = isUserDefinedRecord(t) && !isPOD(t);

    if (needsDestroy) {
      retval = new SymExpr(gTrue);
    } else {
      retval = new SymExpr(gFalse);
    }

    call->replace(retval);

    break;
  }

  case PRIM_IS_REF_ITER_TYPE: {
    if (isRefIterType(call->get(1)->typeInfo())) {
      retval = new SymExpr(gTrue);
    } else {
      retval = new SymExpr(gFalse);
    }

    call->replace(retval);

    break;
  }

  case PRIM_IS_STAR_TUPLE_TYPE: {
    Type* tupleType = call->get(1)->typeInfo();

    // If the type isn't a tuple, it definitely isn't a homogeneous tuple!
    if (tupleType->symbol->hasFlag(FLAG_TUPLE) &&
        tupleType->symbol->hasFlag(FLAG_STAR_TUPLE)) {
      retval = new SymExpr(gTrue);
    } else {
      retval = new SymExpr(gFalse);
    }

    call->replace(retval);

    break;
  }

  case PRIM_IS_TUPLE_TYPE: {
    Type* tupleType = call->get(1)->typeInfo();

    if (tupleType->symbol->hasFlag(FLAG_TUPLE)) {
      retval = new SymExpr(gTrue);
    } else {
      retval = new SymExpr(gFalse);
    }

    call->replace(retval);

    break;
  }

  case PRIM_IS_RECORD_TYPE: {
    AggregateType* at = toAggregateType(call->get(1)->typeInfo());

    if (isRecord(at) == true) {
      retval = new SymExpr(gTrue);

    } else {
      retval = new SymExpr(gFalse);
    }

    call->replace(retval);

    break;
  }

  case PRIM_IS_UNION_TYPE: {
    AggregateType* classType = toAggregateType(call->get(1)->typeInfo());

    if (isUnion(classType) == true) {
      retval = new SymExpr(gTrue);
    } else {
      retval = new SymExpr(gFalse);
    }

    call->replace(retval);

    break;
  }

  case PRIM_LOGICAL_FOLDER: {
    SymExpr*   sym1 = toSymExpr(call->get(1));
    VarSymbol* lhs  = NULL;

    if (VarSymbol* sym = toVarSymbol(sym1->symbol())) {
      if (sym->immediate || paramMap.get(sym)) {
        CallExpr* mvCall = toCallExpr(call->parentExpr);
        SymExpr*  sym    = toSymExpr(mvCall->get(1));

        lhs = toVarSymbol(sym->symbol());

        lhs->addFlag(FLAG_MAYBE_PARAM);
      }
    }

    retval = call->get(2)->remove();

    if (SymExpr* se = toSymExpr(retval)) {
      Symbol*    sym2   = se->symbol();
      VarSymbol* varSym = toVarSymbol(se->symbol());
      ArgSymbol* argSym = toArgSymbol(se->symbol());

      if (varSym != NULL || argSym != NULL) {
        bool       isRef       = isReferenceType(sym2->type);
        bool       isImmediate = false;
        IntentTag  intent      = INTENT_BLANK;

        if (varSym != NULL) {
          if (varSym->immediate) {
            isImmediate = true;
          }

          intent = concreteIntent(INTENT_BLANK, varSym->type);
        }

        if (argSym != NULL) {
          intent = concreteIntent(argSym->intent, argSym->type);
        }

        if (isRef == true) {
          // can't take address of something already a ref

        } else if (sym2->type == dtNil) {
          // can't take address of nil

        } else if (sym2->hasFlag(FLAG_EXPR_TEMP) == true ||
                   sym2->hasFlag(FLAG_TEMP)      == true ||
                   isImmediate                   == true ||
                   paramMap.get(sym2)) {

          // can't take address of call temps, param values
          if (lhs != NULL) {
            lhs->removeFlag(FLAG_MAYBE_REF);
          }

        } else if (argSym && (intent & INTENT_FLAG_IN)) {
          // don't take the address of arguments passed with in intent
          // (it doesn't help and causes problems with inlining)

        } else if (varSym                            != NULL &&
                   (intent & INTENT_FLAG_IN)         !=    0 &&
                   (varSym->isConstValWillNotChange() == true ||
                    varSym->hasFlag(FLAG_TYPE_VARIABLE))) {
          // don't take address of outer variables declared to be const
          // (otherwise, after flattenFunctions, we will take the
          //  address of a by-value argument). An outer variable might be a
          //  type if the symbol represents a field, and taking the address of
          //  a type does not make sense.

        } else {
          Expr* stmt = call->getStmtExpr();
          Type* t    = sym2->type;

          makeRefType(t);

          VarSymbol* tmp = newTemp("_fold_tmp", t->refType);

          stmt->insertBefore(new DefExpr(tmp));

          stmt->insertBefore(new CallExpr(PRIM_MOVE,
                                          tmp,
                                          new CallExpr(PRIM_ADDR_OF, retval)));

          retval = new SymExpr(tmp);
        }
      }
    }

    call->replace(retval);

    break;
  }

  case PRIM_NUM_FIELDS: {
    Type*          t          = canonicalDecoratedClassType(call->get(1)->getValType());
    AggregateType* classType  = toAggregateType(t);
    int            fieldCount = 0;

    for_fields(field, classType) {
      if (isNormalField(field) == true) {
        fieldCount++;
      }
    }

    retval = new SymExpr(new_IntSymbol(fieldCount));

    call->replace(retval);

    break;
  }

  case PRIM_HAS_LEADER: {
    FnSymbol* iterator   = getTheIteratorFn(call->get(1)->typeInfo());
    Symbol* result = gFalse;
    if (IteratorGroup* igroup = iterator->iteratorGroup)
      if (igroup->leader != NULL)
        result = gTrue;

    retval = new SymExpr(result);
    call->replace(retval);

    break;
  }

  case PRIM_TO_FOLLOWER: {
    FnSymbol* iterator     = getTheIteratorFn(call->get(1)->typeInfo());
    CallExpr* followerCall = NULL;

    if (FnSymbol* f2 = findForallexprFollower(iterator)) {
      followerCall = new CallExpr(f2);
    } else {
      followerCall = new CallExpr(iterator->name);
    }

    for_formals(formal, iterator) {
      if (formal->name  == astrTag && formal->type == gFollowerTag->type) {
        INT_FATAL("tag already present in PRIM_TO_FOLLOWER");
        // Could remove it, but would have to figure out what's
        // happening with followThis and fast too.
      }
      // Note: this can add a use formal outside of its function
      // This is cleaned up in cleanupLeaderFollowerIteratorCalls
      followerCall->insertAtTail(new NamedExpr(formal->name,
                                   createSymExprPropagatingParam(formal)));
    }

    // "tag", "followThis" and optionally "fast" should be placed at the end
    // of the formals in the source code as well, to avoid insertion of an
    // order wrapper.
    followerCall->insertAtTail(new NamedExpr("tag",
                                             new SymExpr(gFollowerTag)));

    followerCall->insertAtTail(new NamedExpr(iterFollowthisArgname,
                                             call->get(2)->remove()));

    if (call->numActuals() > 1) {
      followerCall->insertAtTail(new NamedExpr("fast",
                                               call->get(2)->remove()));
    }

    call->replace(followerCall);

    retval = followerCall;

    break;
  }

  case PRIM_TO_LEADER: {
    FnSymbol* iterator   = getTheIteratorFn(call->get(1)->typeInfo());
    CallExpr* leaderCall = new CallExpr(iterator->name);

    for_formals(formal, iterator) {
      // Note: this can add a use formal outside of its function
      // This is cleaned up in cleanupLeaderFollowerIteratorCalls
      if (formal->name  == astrTag && formal->type == gLeaderTag->type) {
        // Leave out the tag since we add it in again below
      } else {
        leaderCall->insertAtTail(new NamedExpr(formal->name,
                                   createSymExprPropagatingParam(formal)));
      }
    }

    // "tag" should be placed at the end of the formals in the source code as
    // well, to avoid insertion of an order wrapper.
    leaderCall->insertAtTail(new NamedExpr("tag", new SymExpr(gLeaderTag)));

    call->replace(leaderCall);

    retval = leaderCall;

    break;
  }

  case PRIM_TO_STANDALONE: {
    FnSymbol* iterator       = getTheIteratorFn(call->get(1)->typeInfo());
    CallExpr* standaloneCall = new CallExpr(iterator->name);

    for_formals(formal, iterator) {
      // Note: this can add a use formal outside of its function
      // This is cleaned up in cleanupLeaderFollowerIteratorCalls
      if (formal->name  == astrTag && formal->type == gStandaloneTag->type) {
        // Leave out the tag since we add it in again below
      } else {
        standaloneCall->insertAtTail(new NamedExpr(formal->name,
                                       createSymExprPropagatingParam(formal)));
      }
    }

    // "tag" should be placed at the end of the formals in the source code as
    // well, to avoid insertion of an order wrapper.
    standaloneCall->insertAtTail(new NamedExpr("tag",
                                               new SymExpr(gStandaloneTag)));

    call->replace(standaloneCall);

    retval = standaloneCall;

    break;
  }

  case PRIM_ITERATOR_RECORD_SET_SHAPE: {
    Symbol* ir = toSymExpr(call->get(1))->symbol();
    if (ir->hasFlag(FLAG_TYPE_VARIABLE)) {
      // This is a type. Do not do shape.
      retval = new CallExpr(PRIM_NOOP);
      call->replace(retval);
    } else if (ir->type == dtUnknown) {
      // Ex. test/arrays/return/returnArbitraryArray and siblings.
      INT_ASSERT(ir->hasFlag(FLAG_RVV));
      // Delay the lowering - skip 'call' for now.
      retval = new CallExpr(PRIM_NOOP);
      call->insertAfter(retval);
    } else {
      // Keep in sync with setIteratorRecordShape(CallExpr* call).
      INT_ASSERT(ir->type->symbol->hasFlag(FLAG_ITERATOR_RECORD));
      Symbol* shapeSpec = toSymExpr(call->get(2))->symbol();
      Symbol* fromForLoop = toSymExpr(call->get(3))->symbol();
      retval = setIteratorRecordShape(call, ir, shapeSpec,
                 getSymbolImmediate(fromForLoop)->bool_value());
      call->replace(retval);
    }

    break;
  }

  case PRIM_TYPE_TO_STRING: {
    SymExpr* se = toSymExpr(call->get(1));

    INT_ASSERT(se && se->symbol()->hasFlag(FLAG_TYPE_VARIABLE));

    Type* t = se->symbol()->type;
    const char* typeName = toString(t);

    retval = new SymExpr(new_StringSymbol(typeName));

    call->replace(retval);

    break;
  }

  case PRIM_TYPEOF: {
    SymExpr* se = toSymExpr(call->get(1));
    Type* type = se->getValType();

    if (se->symbol()->hasFlag(FLAG_TYPE_VARIABLE)) {
      USR_FATAL_CONT(call, "can't apply '.type' to a type (%s)",
                     toString(se->typeInfo()));
    } else if (type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE)) {
      retval = new CallExpr("chpl__convertValueToRuntimeType",
                            call->get(1)->remove());

      call->replace(retval);

      // If this call is inside a BLOCK_TYPE_ONLY, it will be removed and the
      // runtime type will not be initialized. Unset this bit to fix.
      //
      // Assumption: The block we need to modify is either the parent or
      // grandparent expression of the call.
      BlockStmt* blk = NULL;

      if ((blk = toBlockStmt(retval->parentExpr))) {
        // If the call's parent expression is a block, we assume it to
        // be a scopeless type_only block.
        INT_ASSERT(blk->blockTag & BLOCK_TYPE);

      } else {
        // The grandparent block doesn't necessarily have the BLOCK_TYPE_ONLY
        // flag.
        blk = toBlockStmt(retval->parentExpr->parentExpr);
      }

      if (blk) {
        (unsigned&)(blk->blockTag) &= ~(unsigned)BLOCK_TYPE_ONLY;
      }
    } else if (type->symbol->hasFlag(FLAG_TUPLE)) {
      Type* newt = computeNonRefTuple(toAggregateType(type));
      retval = new SymExpr(newt->symbol);
      call->replace(retval);
    } else {
      // Check whether the type's def is within the .type block itself.
      if (BlockStmt* blk = toBlockStmt(call->getStmtExpr()->parentExpr))
        if (type->symbol->defPoint->parentExpr == blk)
          USR_FATAL_CONT(call, ".type is not supported for this kind of expression");
    }

    break;
  }

  case PRIM_STATIC_TYPEOF:
  case PRIM_STATIC_FIELD_TYPE:
  case PRIM_SCALAR_PROMOTION_TYPE: {

    // Replace the type query call with a SymExpr of the type symbol
    // call->typeInfo() will request the type from the primitive
    Type* type = call->typeInfo();
    retval = new SymExpr(type->symbol);
    call->replace(retval);

    break;
  }

  case PRIM_QUERY: {
    Symbol* field = determineQueriedField(call);

    if (field &&
        (field->hasFlag(FLAG_PARAM) ||
         field->hasFlag(FLAG_TYPE_VARIABLE))) {
      retval = new CallExpr(field->name, gMethodToken, call->get(1)->remove());
      call->replace(retval);

    } else if (field && isInstantiatedField(field)) {
      VarSymbol* tmp = newTemp("_instantiated_field_tmp_");

      call->getStmtExpr()->insertBefore(new DefExpr(tmp));

      if (call->get(1)->getValType()->symbol->hasFlag(FLAG_TUPLE) &&
          field->name[0] == 'x') {
        retval = new CallExpr(PRIM_GET_MEMBER_VALUE,
                              call->get(1)->remove(),
                              new_CStringSymbol(field->name));
      } else {
        retval = new CallExpr(field->name,
                              gMethodToken,
                              call->get(1)->remove());
      }

      call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, retval));

      // Wrap it in PRIM_TYPEOF unless it's just used in another
      // PRIM_QUERY.
      bool wrapInTypeOf = true;
      if (CallExpr* parentCall = toCallExpr(call->parentExpr))
        if (parentCall->isPrimitive(PRIM_MOVE))
          if (SymExpr* lhsSe = toSymExpr(parentCall->get(1)))
            if (SymExpr* useSe = lhsSe->symbol()->getSingleUse())
              if (CallExpr* useParent = toCallExpr(useSe->parentExpr))
                if (useParent->isPrimitive(PRIM_QUERY))
                  wrapInTypeOf = false;

      if (wrapInTypeOf)
        call->replace(new CallExpr(PRIM_TYPEOF, tmp));
      else
        call->replace(new SymExpr(tmp));

    } else {
      // Possibly indicated by 'determineQueriedField' returning NULL
      USR_FATAL(call,
                "invalid query -- queried field must be a type or parameter");
    }

    break;
  }

  case PRIM_REDUCE_ASSIGN: {
    // Convert this 'call' into a call to accumulateOntoState().
    INT_ASSERT(call->numActuals() == 2);
    Expr*         rhs       = call->get(2)->remove();
    Expr*         lhs       = call->get(1)->remove();
    ShadowVarSymbol*   svar = toShadowVarSymbol(toSymExpr(lhs)->symbol());
    Symbol*       globalOp  = svar->ReduceOpForAccumState();

    INT_ASSERT(!strcmp(toSymExpr(lhs)->symbol()->name, svar->name));
    INT_ASSERT(svar->isReduce());
    INT_ASSERT(isReduceOp(globalOp->type));

    retval = new_Expr("accumulateOntoState(%S,%S,%E,%E)",
                      gMethodToken, globalOp, lhs, rhs);
    call->replace(retval);

    break;
  }

  case PRIM_WIDE_GET_LOCALE:
  case PRIM_WIDE_GET_NODE: {
    Type* type = call->get(1)->getValType();

    //
    // ensure .locale (and on) are applied to lvalues or classes
    // (locale type is a class)
    //
    SymExpr* se = toSymExpr(call->get(1));

    if (se->symbol()->hasFlag(FLAG_EXPR_TEMP) &&
        !(isClassLikeOrPtr(type) || isReferenceType(type))) {
      USR_WARN(se, "accessing the locale of a local expression");
    }

    //
    // if .locale is applied to an expression of array, domain,
    // or distribution wrapper type, apply .locale to the _value
    // field of the wrapper
    //
    if (isRecordWrappedType(type)) {
      VarSymbol* tmp = newTemp("_locale_tmp_");

      call->getStmtExpr()->insertBefore(new DefExpr(tmp));

      retval = new CallExpr("_value", gMethodToken, call->get(1)->remove());

      call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, retval));

      call->insertAtTail(tmp);
    }

    //
    // if .locale applies to a value x of owned or shared type,
    // return x.borrow().locale.
    //
    if (isManagedPtrType(type)) {
      VarSymbol* tmp = newTemp("_locale_tmp_");

      call->getStmtExpr()->insertBefore(new DefExpr(tmp));

      retval = new CallExpr("borrow", gMethodToken, call->get(1)->remove());

      call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, retval));

      call->insertAtTail(tmp);
    }

    break;
  }

  case PRIM_SIZEOF_BUNDLE: {
    // Fix up arg to sizeof_bundle(), as we may not have known the
    // type earlier
    SymExpr* sizeSym  = toSymExpr(call->get(1));
    Type*    sizeType = sizeSym->symbol()->typeInfo();

    retval = new CallExpr(PRIM_SIZEOF_BUNDLE, sizeType->symbol);
    call->replace(retval);

    break;
  }

  case PRIM_SIZEOF_DDATA_ELEMENT: {
    // Fix up arg to sizeof_ddata_element(), as we may not have known
    // the type earlier
    SymExpr* sizeSym  = toSymExpr(call->get(1));
    Type*    sizeType = sizeSym->symbol()->typeInfo();

    retval = new CallExpr(PRIM_SIZEOF_DDATA_ELEMENT, sizeType->symbol);
    call->replace(retval);

    break;
  }

  case PRIM_REDUCE: {
    // Need to do this ahead of resolveCall().
    retval = lowerPrimReduce(call);
    break;
  }

  default:
    break;

  } // switch (call->primitive->tag)

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

// This function finds the enum constant that matches imm.
// imm stores a compile-time constant integer or string.
// It's used when casting from an param integer or a param string to the
// enum type.
static Symbol* findMatchingEnumSymbol(Immediate* imm, EnumType* typeEnum) {
  uint64_t fromUint = 0;
  int64_t  fromInt  = 0;
  const char* fromString = NULL;
  bool haveUint = false;
  bool haveInt  = false;
  bool haveString = false;

  ensureEnumTypeResolved(typeEnum);

  if (imm->const_kind == NUM_KIND_INT) {
    haveInt = true;
    fromInt = imm->int_value();
  } else if (imm->const_kind == NUM_KIND_UINT) {
    haveUint = true;
    fromUint = imm->uint_value();
  } else if (imm->const_kind == CONST_KIND_STRING) {
    haveString = true;
    fromString = imm->string_value();
  }

  INT_ASSERT(haveInt || haveUint || haveString);

  for_enums(constant, typeEnum) {

    uint64_t cUint = 0;
    int64_t   cInt = 0;
    bool   gotUint = false;
    bool   gotInt  = false;
    const char* extendedName = NULL;

    gotInt  = get_int(constant->init, &cInt);
    gotUint = get_uint(constant->init, &cUint);

    if (haveString)
      extendedName = astr(typeEnum->symbol->name, ".", constant->sym->name);

    bool match = false;
    // string matches name
    if (haveString &&
        (fromString == constant->sym->name ||
         fromString == extendedName))
      match = true;
    // both int
    else if (gotInt && haveInt && cInt == fromInt)
      match = true;
    // both uint
    else if (gotUint && haveUint && cUint == fromUint)
      match = true;
    // int/uint and int >= 0
    else if (gotInt && haveUint && cInt >= 0 &&
             (uint64_t)cInt == fromUint)
      match = true;
    // uint/int and int >= 0
    else if (gotUint && haveInt && fromInt >= 0 &&
             cUint == (uint64_t)fromInt)
      match = true;

    if (match) {
      return constant->sym;
    }
  }

  return NULL;
}


static Expr* preFoldNamed(CallExpr* call) {
  Expr* retval = NULL;

  if (call->isNamedAstr(astrThis)) {
    SymExpr* base = toSymExpr(call->get(2));

    if (base == NULL) {
      if (NamedExpr* nb = toNamedExpr(call->get(2))) {
        base = toSymExpr(nb->actual);
      }
    }

    INT_ASSERT(base);

    Symbol* sym = base->symbol();

    if (isVarSymbol(sym)                 == true &&
        sym->hasFlag(FLAG_TYPE_VARIABLE) == true) {
      int64_t index    =        0;
      char    field[8] = { '\0' };

      if (call->numActuals() == 2) {
        USR_FATAL(call, "illegal call of type");
      }

      if (call->numActuals() > 3) {
        USR_FATAL(call, "too many arguments to type index expression");
      }

      if (!get_int(call->get(3), &index)) {
        USR_FATAL(call, "illegal type index expression");
      }

      AggregateType* at = toAggregateType(sym->type);

      if (!at) {
        USR_FATAL(call, "illegal type index expression");
      }

      if (index <= 0 || index > at->fields.length-1) {
        USR_FATAL(call, "type index expression '%i' out of bounds", index);
      }

      sprintf(field, "x%" PRId64, index);

      retval = new SymExpr(sym->type->getField(field)->type->symbol);
      call->replace(retval);

    } else if (isLcnSymbol(sym) == true) {
      //
      // resolve tuple indexing by an integral parameter
      //
      Type* t = sym->getValType();

      if (t->symbol->hasFlag(FLAG_TUPLE)) {
        if (Expr* expr = resolveTupleIndexing(call, base->symbol())) {
          retval = expr;  // call was replaced by expr
        }
      }
    }

  } else if (call->isNamed("chpl__initCopy") ||
             call->isNamed("chpl__autoCopy")) {
    if (call->numActuals() == 1) {
      if (SymExpr* symExpr = toSymExpr(call->get(1))) {
        if (VarSymbol* var = toVarSymbol(symExpr->symbol())) {
          if (var->immediate != NULL) {
            retval = new SymExpr(var);

            call->replace(retval);
          }

        } else {
          if (EnumSymbol* var = toEnumSymbol(symExpr->symbol())) {
            // Treat enum values as immediates
            retval = new SymExpr(var);

            call->replace(retval);
          }
        }
      }
    }

  } else if (call->isCast()) {
    SymExpr* toSE = toSymExpr(call->castTo());

    // TODO: we could just bail out aka 'return (retval=call)' if !toSE.
    if (toSE && !toSE->symbol()->hasFlag(FLAG_TYPE_VARIABLE)) {
      // TODO: here, also replace 'call' with toSE so it does not trigger
      // the same USR_FATAL_CONT in printResolutionErrorUnresolved().
      USR_FATAL_CONT(call, "illegal cast to non-type");
    }

    retval = dropUnnecessaryCast(call);

    if (retval == call) {
      // The cast was not dropped.
      // Handle (fold) casts on param values
      if (SymExpr* se = toSymExpr(call->castFrom())) {
        Symbol* sym = se->symbol();

        if (EnumType* enumType = toEnumType(sym->type))
          ensureEnumTypeResolved(enumType);

        Immediate* imm = getSymbolImmediate(sym);

        if (toSE != NULL) {
          Type* oldType = sym->type;
          Type* newType = toSE->symbol()->type;

          bool fromEnum = is_enum_type(oldType);
          bool fromString = (oldType == dtString || oldType == dtStringC);
          bool fromIntUint = is_int_type(oldType) ||
                             is_uint_type(oldType);
          bool fromRealEtc = is_real_type(oldType) ||
                             is_imag_type(oldType) ||
                             is_complex_type(oldType);
          bool fromIntEtc = fromIntUint || fromRealEtc || is_bool_type(oldType);

          bool toEnum = is_enum_type(newType);
          bool toString = (newType == dtString || newType == dtStringC);
          bool toIntUint = is_int_type(newType) ||
                           is_uint_type(newType);
          bool toRealEtc = is_real_type(newType) ||
                           is_imag_type(newType) ||
                           is_complex_type(newType);
          bool toIntEtc = toIntUint || toRealEtc || is_bool_type(newType);


          // Handle casting between numeric types
          if (imm != NULL && (fromEnum || fromIntEtc) && toIntEtc) {
            Immediate coerce = getDefaultImmediate(newType);

            coerce_immediate(imm, &coerce);

            retval = new SymExpr(new_ImmediateSymbol(&coerce));

            call->replace(retval);

          // Handle casting to enum
          } else if (imm != NULL && toEnum && (fromString || fromIntUint)) {

            EnumType* typeEnum = toEnumType(newType);
            Symbol* constant = findMatchingEnumSymbol(imm, typeEnum);

            if (constant == NULL) {
              if (typeEnum->isAbstract()) {
                // skip this case, as functionResolution.cpp will print a
                // better error message.
              } else {
                USR_FATAL_CONT(call->castFrom(), "enum cast out of bounds");
              }
              retval = call;
            } else {
              retval = new SymExpr(constant);
              call->replace(retval);
            }

          // Handle enumsym:string casts
          } else if (fromEnum && toString) {
            EnumSymbol* enumSym = toEnumSymbol(sym);

            if (enumSym) {
              if (newType == dtStringC)
                retval = new SymExpr(new_CStringSymbol(enumSym->name));
              else
                retval = new SymExpr(new_StringSymbol(enumSym->name));

              call->replace(retval);
            } else {
              retval = call;
            }

          // Handle string:c_string and c_string:string casts
          } else if (imm != NULL && fromString && toString) {

            if (newType == dtStringC)
              retval = new SymExpr(new_CStringSymbol(imm->v_string));
            else
              retval = new SymExpr(new_StringSymbol(imm->v_string));

            call->replace(retval);

          // Handle other casts to string
          } else if (imm != NULL && fromIntEtc && toString) {
            // special case because newType->defaultValue will
            // be null for dtString

            IF1_string_kind skind = STRING_KIND_STRING;
            if (newType == dtStringC)
              skind = STRING_KIND_C_STRING;

            Immediate coerce = Immediate("", skind);

            coerce_immediate(imm, &coerce);

            if (newType == dtStringC)
              retval = new SymExpr(new_CStringSymbol(coerce.v_string));
            else
              retval = new SymExpr(new_StringSymbol(coerce.v_string));

            call->replace(retval);

          }
        }
      }
    }

  } else if (call->isNamed("chpl__staticFastFollowCheck")  ||
             call->isNamed("chpl__dynamicFastFollowCheck")  ) {
    if (! call->isResolved())
      buildFastFollowerChecksIfNeeded(call);
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

// Returns NULL if no substitution was made.
// Otherwise, returns the expression that replaced 'call'.
static Expr* resolveTupleIndexing(CallExpr* call, Symbol* baseVar) {
  if (call->numActuals() != 3)
    USR_FATAL(call, "illegal tuple indexing expression");

  Type* indexType = call->get(3)->getValType();

  if (!is_int_type(indexType) && !is_uint_type(indexType))
    USR_FATAL(call, "tuple indexing expression is not of integral type");

  AggregateType* baseType = toAggregateType(baseVar->getValType());
  int64_t index;
  uint64_t uindex;
  char field[8];
  bool zero_error = false;
  bool error = false;

  if (get_int(call->get(3), &index)) {
    sprintf(field, "x%" PRId64, index);
    if (index <= 0 || index >= baseType->fields.length) {
      USR_FATAL_CONT(call, "tuple index %ld is out of bounds", index);
      if (index == 0) zero_error = true;
      error = true;
    }
  } else if (get_uint(call->get(3), &uindex)) {
    sprintf(field, "x%" PRIu64, uindex);
    if (uindex <= 0 || uindex >= (unsigned long)baseType->fields.length) {
      USR_FATAL_CONT(call, "tuple index %lu is out of bounds", uindex);
      if (uindex == 0) zero_error = true;
      error = true;
    }
  } else {
    return NULL; // not a tuple indexing expression
  }

  if (error) {
    if (zero_error)
      USR_PRINT(call, "tuple elements start at index 1");
    else
      USR_PRINT(call, "this tuple contains elements %i..%i (inclusive)",
                1, baseType->fields.length-1);
    USR_STOP();
  }

  Type* fieldType = baseType->getField(field)->type;

  // Decomposing into a loop index variable from a non-var iterator?
  // In some cases, extract the value and mark constant.
  // See e.g. test/statements/vass/index-variable-const-errors.chpl
  bool intoIndexVarByVal = false;

  // If decomposing this special variable
  // or another tuple that we just decomposed.
  if (baseVar->hasFlag(FLAG_INDEX_OF_INTEREST)) {
    // Find the destination.
    CallExpr* move = toCallExpr(call->parentExpr);
    INT_ASSERT(move && move->isPrimitive(PRIM_MOVE));
    SymExpr* destSE = toSymExpr(move->get(1));
    INT_ASSERT(destSE);

    if (!isReferenceType(baseVar->type) &&
        !isReferenceType(fieldType)) {
      if (destSE->symbol()->hasFlag(FLAG_INDEX_VAR)) {
        // The destination is constant only if both the tuple
        // and the current component are non-references.
        // And it's not an array (arrays are always yielded by reference)
        // - see boundaries() in release/examples/benchmarks/miniMD/miniMD.
        if (!fieldType->symbol->hasFlag(FLAG_ARRAY) &&
            !fieldType->symbol->hasFlag(FLAG_COPY_MUTATES)) {
          destSE->symbol()->addFlag(FLAG_CONST);
        }
      } else {
        INT_ASSERT(destSE->symbol()->hasFlag(FLAG_TEMP));
        // We are detupling into another tuple,
        // which will be detupled later.
        destSE->symbol()->addFlag(FLAG_INDEX_OF_INTEREST);
      }
    }

    if (!isReferenceType(baseVar->type))
      // If either a non-var iterator or zippered,
      // extract with PRIM_GET_MEMBER_VALUE.
      intoIndexVarByVal = true;
  }

  Expr* result;
  if (isReferenceType(fieldType) || intoIndexVarByVal)
    result = new CallExpr(PRIM_GET_MEMBER_VALUE, baseVar, new_CStringSymbol(field));
  else
    result = new CallExpr(PRIM_GET_MEMBER, baseVar, new_CStringSymbol(field));

  call->replace(result);
  return result;
}


//
// determine field associated with query expression
//
static Symbol* determineQueriedField(CallExpr* call) {
  AggregateType* at     =
    toAggregateType(canonicalDecoratedClassType(call->get(1)->getValType()));
  SymExpr*       last   = toSymExpr(call->get(call->numActuals()));
  VarSymbol*     var    = toVarSymbol(last->symbol());
  Symbol*        retval = NULL;

  if (var->immediate->const_kind == CONST_KIND_STRING) {
    retval = at->getField(var->immediate->v_string, false);

  } else {
    Vec<Symbol*> args;
    int             position      = var->immediate->int_value();

    if (at->symbol->hasFlag(FLAG_TUPLE)) {
      return at->getField(position);
    }

    for_vector(Symbol, field, at->getRootInstantiation()->genericFields) {
      args.add(field);
    }

    // A PRIM_QUERY supports three forms:
    // The first case occurs when the user exclusively used positional queries:
    //     proc foo(arg : R(?A, ?B));
    //     ===>
    //     (query <integer>) for each query
    //
    // The second case occurs when the user wrote a named-query:
    //     proc foo(arg : R(A=?A, B=?B));
    //     ===>
    //     (query <string>) for each query
    //
    // The third case occurs when the user wrote a mix of positional and named
    // queries:
    //     proc foo(arg : R(A=?A, B=?B, ?C, ?D));
    //     ===>
    //     (query "A")       // ?A
    //     (query "B")       // ?B
    //     (query "A" "B" 1) // ?C
    //     (query "A" "B" 2) // ?D
    // The positional arguments in this case contain a list of the named fields
    // which are used to find the other unnamed fields.
    for (int i = 2; i < call->numActuals(); i++) {
      SymExpr*   actual = toSymExpr(call->get(i));
      VarSymbol* var    = toVarSymbol(actual->symbol());

      INT_ASSERT(var->immediate->const_kind == CONST_KIND_STRING);

      for (int j = 0; j < args.n; j++) {
        if (args.v[j]                                         != NULL &&
            strcmp(args.v[j]->name, var->immediate->v_string) ==    0) {
          args.v[j] = NULL;
        }
      }
    }

    forv_Vec(Symbol, arg, args) {
      if (arg != NULL) {
        if (position == 1) {
          retval = at->getField(arg->name, false);
          break;

        } else {
          position--;
        }
      }
    }
  }

  return retval;
}


// returns true if the field was instantiated
static bool isInstantiatedField(Symbol* field) {
  TypeSymbol*    ts     = toTypeSymbol(field->defPoint->parentSymbol);
  AggregateType* at     = toAggregateType(ts->type);
  bool           retval = false;

  // BHARSH INIT TODO: Sometimes the type constructor is not resolved when
  // initializers are used. Why?
  if (at->hasInitializers()) {
    Symbol* origField = at->getRootInstantiation()->getField(field->name);
    DefExpr* def = origField->defPoint;

    if (field->hasFlag(FLAG_TYPE_VARIABLE)) {
      retval = true;
    } else if (def->exprType == NULL && def->init == NULL) {
      // Fully-generic types are apparently OK?
      retval = true;
    }
  } else if (at->symbol->hasFlag(FLAG_TUPLE)) {
    retval = true;
  } else {
    INT_ASSERT(false);
  }

  return retval;
}

static VarSymbol* dummyFcfError = NULL;

/*
  Captures a function as a first-class value by creating an object that will
  represent the function.  The class is created at the same scope as the
  function being referenced.  Each class is unique and shared among all
  uses of that function as a value.  Once built, the class will override
  the .this method of the parent and wrap the call to the function being
  captured as a value.  Then, an instance of the class is instantiated and
  returned.
*/
static Expr* createFunctionAsValue(CallExpr *call) {
  static int unique_fcf_id = 0;

  UnresolvedSymExpr* use    = toUnresolvedSymExpr(call->get(1));
  const char*        flname = use->unresolved;

  Vec<FnSymbol*>     visibleFns;

  getVisibleFunctions(flname, call, visibleFns);

  if (visibleFns.n > 1) {
    USR_FATAL(call, "%s: can not capture overloaded functions as values",
                    visibleFns.v[0]->name);
  }

  INT_ASSERT(visibleFns.n == 1);

  FnSymbol* captured_fn = visibleFns.head();

  if (call->isPrimitive(PRIM_CAPTURE_FN_FOR_CHPL)) {
    //
    // If we're doing a Chapel first-class function, we can re-use the
    // cached first-class function information from an earlier call if
    // there was one, so check to see if we've already cached the
    // capture somewhere.
    //
    if (functionCaptureMap.find(captured_fn) != functionCaptureMap.end()) {
      return new CallExpr(functionCaptureMap[captured_fn]);
    }
  }

  resolveSignature(captured_fn);

  for_formals(formal, captured_fn) {
    if (formal->type->symbol->hasFlag(FLAG_GENERIC)) {
      USR_FATAL_CONT(call, "'%s' cannot be captured as a value because it is a generic function", captured_fn->name);
      if (dummyFcfError == NULL) {
        AggregateType* parent = createAndInsertFunParentClass(call, "_fcf_error");
        dummyFcfError = newTemp(parent);
        theProgram->block->body.insertAtTail(new DefExpr(dummyFcfError));
      }
      return new SymExpr(dummyFcfError);
    }
  }

  resolveFnForCall(captured_fn, call);

  //
  // When all we need is a C pointer, we can cut out here, returning
  // a reference to the function symbol.
  //
  if (call->isPrimitive(PRIM_CAPTURE_FN_FOR_C)) {
    return new SymExpr(captured_fn);
  }

  //
  // Otherwise, we need to create a Chapel first-class function (fcf)...
  //

  AggregateType* parent;
  FnSymbol*      thisParentMethod;

  std::string parent_name = buildParentName(captured_fn->formals, true,
      captured_fn->retType, captured_fn->throwsError());

  if (functionTypeMap.find(parent_name) != functionTypeMap.end()) {
    std::pair<AggregateType*, FnSymbol*> ctfs = functionTypeMap[parent_name];

    parent           = ctfs.first;
    thisParentMethod = ctfs.second;

  } else {
    parent = createAndInsertFunParentClass(call, parent_name.c_str());
    thisParentMethod = createAndInsertFunParentMethod(call, parent,
        captured_fn->formals, true, captured_fn->retType,
        captured_fn->throwsError());
    functionTypeMap[parent_name] = std::pair<AggregateType*, FnSymbol*>(parent, thisParentMethod);
  }

  AggregateType *ct = new AggregateType(AGGREGATE_CLASS);
  std::ostringstream fcf_name;

  fcf_name << "_chpl_fcf_" << unique_fcf_id++ << "_" << flname;

  TypeSymbol *ts = new TypeSymbol(astr(fcf_name.str().c_str()), ct);

  // Allow a use of a FCF to appear at the statement level i.e.
  //    nameOfFunc;
  //
  // In the longer term it might be good to generate a warning for this
  if (isBlockStmt(call->parentExpr) == true) {
    call->insertBefore(new DefExpr(ts));

  // The common case in which the reference is within a move/assign/call
  } else {
    call->parentExpr->insertBefore(new DefExpr(ts));
  }

  ct->dispatchParents.add(parent);

  bool inserted = parent->dispatchChildren.add_exclusive(ct);

  INT_ASSERT(inserted);

  VarSymbol* super = new VarSymbol("super", parent);

  super->addFlag(FLAG_SUPER_CLASS);

  ct->fields.insertAtHead(new DefExpr(super));

  ct->processGenericFields();

  ct->buildDefaultInitializer();

  buildDefaultDestructor(ct);

  FnSymbol*  thisMethod = new FnSymbol("this");
  ArgSymbol* thisSymbol = new ArgSymbol(INTENT_BLANK, "this", ct);

  thisMethod->addFlag(FLAG_FIRST_CLASS_FUNCTION_INVOCATION);
  thisMethod->addFlag(FLAG_COMPILER_GENERATED);
  thisMethod->addFlag(FLAG_OVERRIDE);

  thisMethod->insertFormalAtTail(new ArgSymbol(INTENT_BLANK,
                                               "_mt",
                                               dtMethodToken));

  thisMethod->setMethod(true);

  thisMethod->insertFormalAtTail(thisSymbol);

  thisMethod->_this = thisSymbol;

  thisSymbol->addFlag(FLAG_ARG_THIS);

  CallExpr* innerCall = new CallExpr(captured_fn);
  int       skip      = 2;

  for_alist(formalExpr, thisParentMethod->formals) {
    //Skip the first two arguments from the parent, which are _mt and this
    if (skip) {
      --skip;
      continue;
    }

    DefExpr*   dExp = toDefExpr(formalExpr);
    ArgSymbol* fArg = toArgSymbol(dExp->sym);

    ArgSymbol* newFormal = new ArgSymbol(INTENT_BLANK, fArg->name, fArg->type);

    if (fArg->typeExpr) {
      newFormal->typeExpr = fArg->typeExpr->copy();
    }

    SymExpr* argSym = new SymExpr(newFormal);

    innerCall->insertAtTail(argSym);

    thisMethod->insertFormalAtTail(newFormal);
  }

  std::vector<CallExpr*> calls;

  collectCallExprs(captured_fn, calls);

  for_vector(CallExpr, cl, calls) {
    if (cl->isPrimitive(PRIM_YIELD)) {
      USR_FATAL_CONT(cl, "Iterators not allowed in first class functions");
    }
  }

  if (captured_fn->retType == dtVoid) {
    thisMethod->insertAtTail(innerCall);

  } else {
    VarSymbol* tmp = newTemp("_return_tmp_");

    thisMethod->insertAtTail(new DefExpr(tmp));
    thisMethod->insertAtTail(new CallExpr(PRIM_MOVE, tmp, innerCall));

    thisMethod->insertAtTail(new CallExpr(PRIM_RETURN, tmp));
  }

  if (captured_fn->throwsError()) {
    thisMethod->throwsErrorInit();
  }

  // (Seen note above)
  if (isBlockStmt(call->parentExpr) == true) {
    call->insertBefore(new DefExpr(thisMethod));

  } else {
    call->parentExpr->insertBefore(new DefExpr(thisMethod));
  }

  normalize(thisMethod);

  ct->methods.add(thisMethod);

  FnSymbol* wrapper = new FnSymbol("wrapper");

  wrapper->addFlag(FLAG_INLINE);

  wrapper->insertAtTail(new CallExpr(PRIM_RETURN,
                                     new CallExpr(PRIM_CAST,
                                                  parent->symbol,
                                                  new CallExpr(PRIM_NEW,
                                                               new NamedExpr(astr_chpl_manager, new SymExpr(dtUnmanaged->symbol)),
                                                               new SymExpr(ct->symbol)))));

  call->getStmtExpr()->insertBefore(new DefExpr(wrapper));

  normalize(wrapper);

  CallExpr* callWrapper = new CallExpr(wrapper);

  functionCaptureMap[captured_fn] = wrapper;


  /* make writeThis for FCFs */
  {
    ArgSymbol* fileArg = NULL;
    FnSymbol* fn = buildWriteThisFnSymbol(ct, &fileArg);
    // when printing out a FCF, print out the function's name
    fn->insertAtTail(new CallExpr(new CallExpr(".", fileArg,
                                               new_StringSymbol("writeIt")),
                                  new_StringSymbol(astr(flname, "()"))));
    normalize(fn);
  }

  return callWrapper;
}

/*
  Helper function for creating or finding the parent class for a given
  function type specified by the type signature.  The last type given
  in the signature is the return type, the remainder represent arguments
  to the function.
*/
static AggregateType* createOrFindFunTypeFromAnnotation(AList&     argList,
                                                        CallExpr*  call) {
  AggregateType* parent      = NULL;
  SymExpr*       retTail     = toSymExpr(argList.tail);
  Type*          retType     = retTail->symbol()->type;
  bool           throws      = false; // TODO: how to distinguish?
  std::string    parent_name = buildParentName(argList, false, retType, throws);

  if (functionTypeMap.find(parent_name) != functionTypeMap.end()) {
    parent = functionTypeMap[parent_name].first;

  } else {
    FnSymbol* parentMethod = NULL;

    parent       = createAndInsertFunParentClass(call,
                                                 parent_name.c_str());
    parentMethod = createAndInsertFunParentMethod(call,
                                                  parent,
                                                  argList,
                                                  false,
                                                  retType,
                                                  throws);

    functionTypeMap[parent_name] = std::pair<AggregateType*,
                                             FnSymbol*>(parent, parentMethod);
  }

  return parent;
}

//
//
//

static Expr* dropUnnecessaryCast(CallExpr* call) {
  // Check for and remove casts to the original type and size
  Expr* result = call;
  if (!call->isCast())
    INT_FATAL("dropUnnecessaryCasts called on non cast call");

  if (SymExpr* fromSe = toSymExpr(call->castFrom())) {
    if (LcnSymbol* var = toLcnSymbol(fromSe->symbol())) {
      // Casts of type variables are always required
      // eg. foo.type:string
      if (!var->hasFlag(FLAG_TYPE_VARIABLE)) {
        if (SymExpr* toSe = toSymExpr(call->castTo())) {
          Type* oldType = var->type->getValType();
          Type* newType = toSe->symbol()->type->getValType();

          if (newType == oldType) {
            if (isUserDefinedRecord(newType) && !getSymbolImmediate(var)) {
              result = new CallExpr("_removed_cast", var);
              call->replace(result);
            } else {
              result = new SymExpr(var);
              call->replace(result);
            }
          }
        }
      }
    } else if (EnumSymbol* e = toEnumSymbol(fromSe->symbol())) {
      if (SymExpr* toSe = toSymExpr(call->castTo())) {
        EnumType* oldType = toEnumType(e->type);
        EnumType* newType = toEnumType(toSe->symbol()->type);
        if (newType && oldType == newType) {
          result = new SymExpr(e);
          call->replace(result);
        }
      }
    }
  }
  return result;
}

SymExpr* createSymExprPropagatingParam(Symbol* arg) {
  Symbol* result = arg;
  if (Symbol* paramVal = paramMap.get(arg))
    result = paramVal;
  return new SymExpr(result);
}

// This function returns true for user fields, including
// const, param, and type fields.
// It returns false for compiler-introduced fields like
// super and outer.
static bool isNormalField(Symbol* field)
{
  if( field->hasFlag(FLAG_IMPLICIT_ALIAS_FIELD) ) return false;
  if( field->hasFlag(FLAG_SUPER_CLASS) ) return false;

  return true;
}

/*
  Builds up the name of the parent for lookup by looking through the types
  of the arguments, either formal or actual
*/
static std::string buildParentName(AList& arg_list,
                                   bool   isFormal,
                                   Type*  retType,
                                   bool throws) {
  std::ostringstream oss;
  bool               isFirst = true;

  oss << "chpl__fcf_type_";

  if (isFormal) {
    if (arg_list.length == 0) {
      oss << "void";

    } else {
      for_alist(formalExpr, arg_list) {
        DefExpr* dExp = toDefExpr(formalExpr);
        ArgSymbol* fArg = toArgSymbol(dExp->sym);

        if (!isFirst)
          oss << "_";

        oss << fArg->type->symbol->cname;

        isFirst = false;
      }
    }
    oss << "_";
    oss << retType->symbol->cname;

  } else {
    int i = 0, alength = arg_list.length;

    if (alength == 1) {
      oss << "void_";
    }

    for_alist(actualExpr, arg_list) {
      if (!isFirst)
        oss << "_";

      SymExpr* sExpr = toSymExpr(actualExpr);

      ++i;

      oss << sExpr->symbol()->type->symbol->cname;

      isFirst = false;
    }
  }

  if (throws)
    oss << "_throws";

  return oss.str();
}

/*
  Creates the parent class which will represent the function's type.
  Children of the parent class will capture different functions which
  happen to share the same function type.  By using the parent class
  we can assign new values onto variable that match the function type
  but may currently be pointing at a different function.
*/
static AggregateType* createAndInsertFunParentClass(CallExpr*   call,
                                                    const char* name) {
  AggregateType* parent   = new AggregateType(AGGREGATE_CLASS);
  TypeSymbol*    parentTs = new TypeSymbol(name, parent);

  parentTs->addFlag(FLAG_FUNCTION_CLASS);

  // Because this function type needs to be globally visible (because
  // we don't know the modules it will be passed to), we put it at the
  // highest scope
  theProgram->block->body.insertAtTail(new DefExpr(parentTs));

  parent->dispatchParents.add(dtObject);

  dtObject->dispatchChildren.add(parent);

  VarSymbol* parentSuper = new VarSymbol("super", dtObject);

  parentSuper->addFlag(FLAG_SUPER_CLASS);

  parent->fields.insertAtHead(new DefExpr(parentSuper));

  parent->processGenericFields();

  parent->buildDefaultInitializer();

  buildDefaultDestructor(parent);

  return parent;
}

/*
  To mimic a function call, we create a .this method for the parent class.
  This will allow the object to look and feel like a first-class function,
  by both being an object and being invoked using parentheses syntax.

  Children of the parent class will override this method and wrap the
  function that is being used as a first-class value.

  To focus on just the types of the arguments and not their names or
  default values, we use the parent method's names and types as the basis
  for all children which override it.

  The function is put at the highest scope so that all functions of a given
  type will share the same parent class.
*/
static FnSymbol* createAndInsertFunParentMethod(CallExpr*      call,
                                                AggregateType* parent,
                                                AList&         arg_list,
                                                bool           isFormal,
                                                Type*          retType,
                                                bool           throws) {

  // Add a "getter" method that returns the return type of the function
  //
  //   * The return type itself is not actually stored as a member variable
  //
  //   * A function that returns void will similarly cause a call to retType
  //     to return void, which is to say it doesn't return anything.  This is
  //     maybe not terribly intuitive to the user, but should be resolved
  //     when "void" becomes a fully-featured type.
  {
    FnSymbol* rtGetter = new FnSymbol("retType");

    rtGetter->addFlag(FLAG_NO_IMPLICIT_COPY);
    rtGetter->addFlag(FLAG_INLINE);
    rtGetter->addFlag(FLAG_COMPILER_GENERATED);
    rtGetter->retTag = RET_TYPE;
    rtGetter->insertFormalAtTail(new ArgSymbol(INTENT_BLANK,
                                               "_mt",
                                               dtMethodToken));

    ArgSymbol* _this = new ArgSymbol(INTENT_BLANK, "this", parent);

    _this->addFlag(FLAG_ARG_THIS);
    rtGetter->insertFormalAtTail(_this);
    rtGetter->insertAtTail(new CallExpr(PRIM_RETURN, retType->symbol));

    DefExpr* def = new DefExpr(rtGetter);

    parent->symbol->defPoint->insertBefore(def);

    normalize(rtGetter);

    parent->methods.add(rtGetter);

    rtGetter->setMethod(true);
    rtGetter->addFlag(FLAG_METHOD_PRIMARY);

    rtGetter->cname = astr("chpl_get_",
                           parent->symbol->cname,
                           "_",
                           rtGetter->cname);

    rtGetter->addFlag(FLAG_NO_PARENS);
    rtGetter->_this = _this;
  }

  // Add a "getter" method that returns the tuple of argument types
  // for the function
  {
    FnSymbol* atGetter = new FnSymbol("argTypes");

    atGetter->addFlag(FLAG_NO_IMPLICIT_COPY);
    atGetter->addFlag(FLAG_INLINE);
    atGetter->addFlag(FLAG_COMPILER_GENERATED);
    atGetter->retTag = RET_TYPE;
    atGetter->insertFormalAtTail(new ArgSymbol(INTENT_BLANK,
                                               "_mt",
                                               dtMethodToken));

    CallExpr* expr = new CallExpr(PRIM_ACTUALS_LIST);

    if (isFormal) {
      for_alist(formalExpr, arg_list) {
        DefExpr*   dExp = toDefExpr(formalExpr);
        ArgSymbol* fArg = toArgSymbol(dExp->sym);

        expr->insertAtTail(fArg->type->symbol);
      }
    }
    else {
      for_alist(actualExpr, arg_list) {
        if (actualExpr != arg_list.tail) {
          SymExpr* sExpr = toSymExpr(actualExpr);

          expr->insertAtTail(sExpr->symbol()->type->symbol);
        }
      }
    }

    ArgSymbol* _this = new ArgSymbol(INTENT_BLANK, "this", parent);

    _this->addFlag(FLAG_ARG_THIS);

    atGetter->insertFormalAtTail(_this);
    atGetter->insertAtTail(new CallExpr(PRIM_RETURN,
                                        new CallExpr("_build_tuple", expr)));

    DefExpr* def = new DefExpr(atGetter);

    parent->symbol->defPoint->insertBefore(def);
    normalize(atGetter);
    parent->methods.add(atGetter);

    atGetter->setMethod(true);
    atGetter->addFlag(FLAG_METHOD_PRIMARY);

    atGetter->cname = astr("chpl_get_",
                           parent->symbol->cname, "_",
                           atGetter->cname);
    atGetter->addFlag(FLAG_NO_PARENS);
    atGetter->_this = _this;
  }

  FnSymbol* parent_method = new FnSymbol("this");

  parent_method->addFlag(FLAG_FIRST_CLASS_FUNCTION_INVOCATION);
  parent_method->addFlag(FLAG_COMPILER_GENERATED);

  parent_method->insertFormalAtTail(new ArgSymbol(INTENT_BLANK,
                                                  "_mt",
                                                  dtMethodToken));
  parent_method->setMethod(true);

  ArgSymbol* thisParentSymbol = new ArgSymbol(INTENT_BLANK, "this", parent);

  thisParentSymbol->addFlag(FLAG_ARG_THIS);

  parent_method->insertFormalAtTail(thisParentSymbol);

  parent_method->_this = thisParentSymbol;

  int i       = 0;
  int alength = arg_list.length;

  // We handle the arg list differently depending on if it's a list of
  // formal args or actual args
  if (isFormal) {

    for_alist(formalExpr, arg_list) {
      DefExpr* dExp = toDefExpr(formalExpr);
      ArgSymbol* fArg = toArgSymbol(dExp->sym);

      if (fArg->type != dtNothing) {
        ArgSymbol* newFormal = new ArgSymbol(INTENT_BLANK,
                                             fArg->name,
                                             fArg->type);

        if (fArg->typeExpr)
          newFormal->typeExpr = fArg->typeExpr->copy();

        parent_method->insertFormalAtTail(newFormal);
      }
    }

  } else {
    char name_buffer[100];
    int  name_index = 0;

    for_alist(actualExpr, arg_list) {
      sprintf(name_buffer, "name%i", name_index++);

      if (i != (alength-1)) {
        SymExpr* sExpr = toSymExpr(actualExpr);

        if (sExpr->symbol()->type != dtNothing) {
          ArgSymbol* newFormal = new ArgSymbol(INTENT_BLANK,
                                               name_buffer,
                                               sExpr->symbol()->type);

          parent_method->insertFormalAtTail(newFormal);
        }
      }

      ++i;
    }
  }

  if (retType != dtVoid) {
    VarSymbol *tmp = newTemp("_return_tmp_", retType);

    parent_method->insertAtTail(new DefExpr(tmp));
    parent_method->insertAtTail(new CallExpr(PRIM_RETURN, tmp));
  }
  if (throws)
    parent_method->throwsErrorInit();

  // Because this function type needs to be globally visible
  // (because we don't know the modules it will be passed to), we put
  // it at the highest scope
  theProgram->block->body.insertAtTail(new DefExpr(parent_method));

  normalize(parent_method);

  parent->methods.add(parent_method);

  return parent_method;
}
