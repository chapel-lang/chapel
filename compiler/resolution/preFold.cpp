/*
 * Copyright 2004-2017 Cray Inc.
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
#include "expr.h"
#include "expr.h"
#include "ParamForLoop.h"
#include "passes.h"
#include "resolution.h"
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


static Expr*          resolveTupleIndexing(CallExpr* call,
                                           Symbol*   baseVar);

static Symbol*        determineQueriedField(CallExpr* call);

static bool           isInstantiatedField(Symbol* field);

static Expr*          createFunctionAsValue(CallExpr* call);

static AggregateType* createOrFindFunTypeFromAnnotation(AList&    argList,
                                                        CallExpr* call);

static Expr*          dropUnnecessaryCast(CallExpr* call);

static SymExpr*       symOrParamExpr(Symbol* arg);

static bool           isNormalField(Symbol* field);

static std::string    buildParentName(AList& argList,
                                      bool   isFormal,
                                      Type*  retType);

static AggregateType* createAndInsertFunParentClass(CallExpr*   call,
                                                    const char* name);

static FnSymbol*      createAndInsertFunParentMethod(CallExpr*      call,
                                                     AggregateType* parent,
                                                     AList&         argList,
                                                     bool           isFormal,
                                                     Type*          retType);


Expr* preFold(CallExpr* call) {
  Expr* result = call;

  // Match calls that look like:  (<type-symbol> <immediate-integer>)
  // and replace them with:       <new-type-symbol>
  // <type-symbol> is in {dtBools, dtInt, dtUint, dtReal, dtImag, dtComplex}.
  // This replaces, e.g.
  //   dtInt[INT_SIZE_DEFAULT] 32) with dtInt[INT_SIZE_32].
  if (Type* type = typeForTypeSpecifier(call)) {
    result = new SymExpr(type->symbol);

    call->replace(result);
  }

  if (SymExpr* sym = toSymExpr(call->baseExpr)) {
    if (isLcnSymbol(sym->symbol())) {
      Expr* base = call->baseExpr;
      base->replace(new UnresolvedSymExpr("this"));
      call->insertAtHead(base);
      call->insertAtHead(gMethodToken);
    }
  }

  if (CallExpr* base = toCallExpr(call->baseExpr)) {
    if (base->partialTag) {
      for_actuals_backward(actual, base) {
        actual->remove();
        call->insertAtHead(actual);
      }
      base->replace(base->baseExpr->remove());
    } else {
      VarSymbol* this_temp = newTemp("_this_tmp_");
      this_temp->addFlag(FLAG_EXPR_TEMP);
      base->replace(new UnresolvedSymExpr("this"));
      CallExpr* move = new CallExpr(PRIM_MOVE, this_temp, base);
      call->insertAtHead(new SymExpr(this_temp));
      call->insertAtHead(gMethodToken);
      call->getStmtExpr()->insertBefore(new DefExpr(this_temp));
      call->getStmtExpr()->insertBefore(move);
      result = move;
      return result;
    }
  }

  if (call->isNamed("this")) {
    SymExpr* base = toSymExpr(call->get(2));
    if (!base) {
      if (NamedExpr* nb = toNamedExpr(call->get(2)))
        base = toSymExpr(nb->actual);
    }
    INT_ASSERT(base);
    if (isVarSymbol(base->symbol()) && base->symbol()->hasFlag(FLAG_TYPE_VARIABLE)) {
      if (call->numActuals() == 2)
        USR_FATAL(call, "illegal call of type");
      int64_t index;
      if (!get_int(call->get(3), &index))
        USR_FATAL(call, "illegal type index expression");
      char field[8];
      sprintf(field, "x%" PRId64, index);
      result = new SymExpr(base->symbol()->type->getField(field)->type->symbol);
      call->replace(result);
    } else if (base && isLcnSymbol(base->symbol())) {
      //
      // resolve tuple indexing by an integral parameter
      //
      Type* t = base->symbol()->getValType();
      if (t->symbol->hasFlag(FLAG_TUPLE))
        if (Expr* expr = resolveTupleIndexing(call, base->symbol()))
          result = expr;  // call was replaced by expr
    }
  }
  else if (call->isPrimitive(PRIM_INIT))
  {
    if (Expr* expr = resolvePrimInit(call))
    {
      // call was replaced by expr.
      result = expr;
    }
    // No default value yet, so defer resolution of this init
    // primitive until record initializer resolution.
  } else if (call->isPrimitive(PRIM_NO_INIT)) {
    // Lydia note: fUseNoinit does not control this section.  This was
    // necessary because with the definition of type defaults in the module
    // code, return temporary variables would cause an infinite loop by
    // trying to default initialize within the default initialization
    // definition.  (It is safe for these temporaries to skip default
    // initialization, as they will always be assigned a value before they
    // are returned.)  Thus noinit must remain attached to these temporaries,
    // even if --no-use-noinit is thrown.  This is an implementation detail
    // that the user does not need to care about.

    // fUseNoinit controls the insertion of PRIM_NO_INIT statements in the
    // normalize pass.
    SymExpr* se = toSymExpr(call->get(1));
    INT_ASSERT(se);
    if (!se->symbol()->hasFlag(FLAG_TYPE_VARIABLE))
      USR_FATAL(call, "invalid type specification");
    Type* type = call->get(1)->getValType();
    if (isAggregateType(type) || type == dtStringC || type == dtStringCopy) {

      bool containsNotPOD = propagateNotPOD(type);

      if (containsNotPOD) {
        // These types deal with their uninitialized fields differently than
        // normal records/classes.  They may require special case
        // implementations, but were capable of being isolated from the new
        // cases that do work.

        bool nowarn = false;
        // In the case of temporary variables that use noinit (at this point
        // only return variables), it is not useful to warn the user we are
        // still default initializing the values as they weren't the ones to
        // tell us to use noinit in the first place.  So squash the warning
        // in this case.
        if (call->parentExpr) {
          CallExpr* parent = toCallExpr(call->parentExpr);
          if (parent && parent->isPrimitive(PRIM_MOVE)) {
            // Should always be true, but just in case...
            if (SymExpr* holdsDest = toSymExpr(parent->get(1))) {
              Symbol* dest = holdsDest->symbol();
              // TODO: Surely there's a better way to test for this pattern
              bool okName = strcmp(dest->name, "ret") == 0 ||
                strcmp(dest->name, arrayUnrefName) == 0;
              if (dest->hasFlag(FLAG_TEMP) && okName) {
                nowarn = true;
              }
            }
          }
        }
        if (!nowarn)
          USR_WARN(call, "type %s does not currently support noinit, using default initialization", type->symbol->name);
        result = new CallExpr(PRIM_INIT, call->get(1)->remove());
        call->replace(result);
        inits.add(toCallExpr(result));
      } else {
        result = call;
        inits.add(call);
      }
    }

  } else if (call->isPrimitive(PRIM_DELETE)) {
    result = new CallExpr("chpl__delete", call->get(1)->remove());
    call->replace(result);
  } else if (call->isPrimitive(PRIM_TYPEOF)) {
    Type* type = call->get(1)->getValType();
    if (type->symbol->hasFlag(FLAG_HAS_RUNTIME_TYPE)) {
      result = new CallExpr("chpl__convertValueToRuntimeType", call->get(1)->remove());
      call->replace(result);

      // If this call is inside a BLOCK_TYPE_ONLY, it will be removed and the
      // runtime type will not be initialized. Unset this bit to fix.
      //
      // Assumption: The block we need to modify is either the parent or
      // grandparent expression of the call.
      BlockStmt* blk = NULL;
      if ((blk = toBlockStmt(result->parentExpr))) {
        // If the call's parent expression is a block, we assume it to
        // be a scopeless type_only block.
        INT_ASSERT(blk->blockTag & BLOCK_TYPE);
      } else {
        // The grandparent block doesn't necessarily have the BLOCK_TYPE_ONLY
        // flag.
        blk = toBlockStmt(result->parentExpr->parentExpr);
      }
      if (blk) {
        (unsigned&)(blk->blockTag) &= ~(unsigned)BLOCK_TYPE_ONLY;
      }
    }
  } else if (call->isPrimitive(PRIM_QUERY)) {
    Symbol* field = determineQueriedField(call);
    if (field && (field->hasFlag(FLAG_PARAM) || field->hasFlag(FLAG_TYPE_VARIABLE))) {
      result = new CallExpr(field->name, gMethodToken, call->get(1)->remove());
      call->replace(result);
    } else if (field && isInstantiatedField(field)) {
      VarSymbol* tmp = newTemp("_instantiated_field_tmp_");
      call->getStmtExpr()->insertBefore(new DefExpr(tmp));
      if (call->get(1)->typeInfo()->symbol->hasFlag(FLAG_TUPLE) && field->name[0] == 'x')
        result = new CallExpr(PRIM_GET_MEMBER_VALUE, call->get(1)->remove(), new_CStringSymbol(field->name));
      else
        result = new CallExpr(field->name, gMethodToken, call->get(1)->remove());
      call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, result));
      call->replace(new CallExpr(PRIM_TYPEOF, tmp));
    } else {
      // Possibly indicated by 'determineQueriedField' returning NULL
      USR_FATAL(call, "invalid query -- queried field must be a type or parameter");
    }
  } else if (call->isPrimitive(PRIM_CAPTURE_FN_FOR_CHPL) ||
             call->isPrimitive(PRIM_CAPTURE_FN_FOR_C)) {
    result = createFunctionAsValue(call);
    call->replace(result);

  } else if (call->isPrimitive(PRIM_GET_COMPILER_VAR)) {

    // Resolving Primitive PRIM_GET_COMPILER_VAR
    Immediate* chplEnv = toVarSymbol(toSymExpr(call->get(1))->symbol())->immediate;
    std::string envKey;

    // Check if this immediate is a string
    if (chplEnv->const_kind == CONST_KIND_STRING) {
      envKey = chplEnv->v_string;
    } else {
      USR_FATAL(call, "expected immediate of type string");
    }

    // Check if string is in envMap, and replace result with mapped value
    if (envMap.find(envKey) != envMap.end()) {
      result = new SymExpr(new_StringSymbol(envMap[envKey]));
      call->replace(result);
    } else {
      USR_FATAL(call, "primitive string does not match any environment variable");
    }

  } else if (call->isPrimitive(PRIM_CREATE_FN_TYPE)) {
    AggregateType *parent = createOrFindFunTypeFromAnnotation(call->argList, call);

    result = new SymExpr(parent->symbol);
    call->replace(result);
  } else if (call->isNamed("chpl__initCopy") ||
             call->isNamed("chpl__autoCopy")) {
    if (call->numActuals() == 1) {
      if (SymExpr* symExpr = toSymExpr(call->get(1))) {
        if (VarSymbol* var = toVarSymbol(symExpr->symbol())) {
          if (var->immediate) {
            result = new SymExpr(var);
            call->replace(result);
          }
        } else {
          if (EnumSymbol* var = toEnumSymbol(symExpr->symbol())) {
            // Treat enum values as immediates
            result = new SymExpr(var);
            call->replace(result);
          }
        }
      }
    }
  } else if (call->isCast()) {
    SymExpr* toSE = toSymExpr(call->castTo());
    // TODO: we could just bail out aka 'return (result=call)' if !toSE.
    if (toSE && !toSE->symbol()->hasFlag(FLAG_TYPE_VARIABLE))
      // TODO: here, also replace 'call' with toSE so it does not trigger
      // the same USR_FATAL_CONT in printResolutionErrorUnresolved().
      USR_FATAL_CONT(call, "illegal cast to non-type");
    result = dropUnnecessaryCast(call);
    if (result == call) {
      // The cast was not dropped.  Remove integer casts on immediate values.
      if (SymExpr* sym = toSymExpr(call->castFrom())) {
        if (VarSymbol* var = toVarSymbol(sym->symbol())) {
          if (var->immediate) {
            if (toSE) {
              Type* oldType = var->type;
              Type* newType = toSE->symbol()->type;
              if ((is_int_type(oldType) || is_uint_type(oldType) ||
                   is_bool_type(oldType)) &&
                  (is_int_type(newType) || is_uint_type(newType) ||
                   is_bool_type(newType) || is_enum_type(newType) ||
                   newType == dtString || newType == dtStringC)) {
                VarSymbol* typevar = toVarSymbol(newType->defaultValue);
                EnumType* typeenum = toEnumType(newType);
                if (typevar) {
                  if (!typevar->immediate)
                    INT_FATAL("unexpected case in cast_fold");

                  Immediate coerce = *typevar->immediate;
                  coerce_immediate(var->immediate, &coerce);
                  result = new SymExpr(new_ImmediateSymbol(&coerce));
                  call->replace(result);
                } else if (newType == dtString) {
                  // typevar will be null for dtString so we need a special
                  // case.
                  Immediate coerce = Immediate("", STRING_KIND_STRING);
                  coerce_immediate(var->immediate, &coerce);
                  result = new SymExpr(new_StringSymbol(coerce.v_string));
                  call->replace(result);
                } else if (typeenum) {
                  int64_t value, count = 0;
                  bool replaced = false;
                  if (!get_int(call->castFrom(), &value)) {
                    INT_FATAL("unexpected case in cast_fold");
                  }
                  for_enums(constant, typeenum) {
                    if (!get_int(constant->init, &count)) {
                      count++;
                    }
                    if (count == value) {
                      result = new SymExpr(constant->sym);
                      call->replace(result);
                      replaced = true;
                      // could break here but might have issues with gcc 5.1
                    }
                  }
                  if (!replaced) {
                    USR_FATAL(call->castFrom(), "enum cast out of bounds");
                  }
                } else {
                  INT_FATAL("unexpected case in cast_fold");
                }
              } else if (oldType == dtString && newType == dtStringC) {
                result = new SymExpr(new_CStringSymbol(var->immediate->v_string));
                call->replace(result);
              } else if (oldType == dtStringC && newType == dtString) {
                result = new SymExpr(new_StringSymbol(var->immediate->v_string));
                call->replace(result);
              }

            }
          }
        } else if (EnumSymbol* enumSym = toEnumSymbol(sym->symbol())) {
          if (toSE) {
            Type* newType = toSE->symbol()->type;
            if (newType == dtString) {
              result = new SymExpr(new_StringSymbol(enumSym->name));
              call->replace(result);
            } else if (newType == dtStringC) {
              result = new SymExpr(new_CStringSymbol(enumSym->name));
              call->replace(result);
            }
          }
        }
      }
    }
  } else if (call->isNamed("==")) {
    if (isTypeExpr(call->get(1)) && isTypeExpr(call->get(2))) {
      Type* lt = call->get(1)->getValType();
      Type* rt = call->get(2)->getValType();
      if (lt != dtUnknown && rt != dtUnknown &&
          !lt->symbol->hasFlag(FLAG_GENERIC) &&
          !rt->symbol->hasFlag(FLAG_GENERIC)) {
        result = (lt == rt) ? new SymExpr(gTrue) : new SymExpr(gFalse);
        call->replace(result);
      }
    }
  } else if (call->isNamed("!=")) {
    if (isTypeExpr(call->get(1)) && isTypeExpr(call->get(2))) {
      Type* lt = call->get(1)->getValType();
      Type* rt = call->get(2)->getValType();
      if (lt != dtUnknown && rt != dtUnknown &&
          !lt->symbol->hasFlag(FLAG_GENERIC) &&
          !rt->symbol->hasFlag(FLAG_GENERIC)) {
        result = (lt != rt) ? new SymExpr(gTrue) : new SymExpr(gFalse);
        call->replace(result);
      }
    }
  } else if (call->isNamed("_type_construct__tuple") && !call->isResolved()) {
    if (SymExpr* sym = toSymExpr(call->get(1))) {
      if (VarSymbol* var = toVarSymbol(sym->symbol())) {
        if (var->immediate) {
          int rank = var->immediate->int_value();
          if (rank != call->numActuals() - 1) {
            if (call->numActuals() != 2)
              INT_FATAL(call, "bad homogeneous tuple");
            Expr* actual = call->get(2);
            for (int i = 1; i < rank; i++) {
              call->insertAtTail(actual->copy());
            }
          }
        }
      }
    }
  } else if (call->isPrimitive(PRIM_BLOCK_PARAM_LOOP)) {
    ParamForLoop* paramLoop = toParamForLoop(call->parentExpr);

    result = paramLoop->foldForResolve();
  } else if (call->isPrimitive(PRIM_LOGICAL_FOLDER)) {
    SymExpr* sym1 = toSymExpr(call->get(1));
    VarSymbol* lhs = NULL;
    if (VarSymbol* sym = toVarSymbol(sym1->symbol())) {
      if (sym->immediate || paramMap.get(sym)) {
        CallExpr* mvCall = toCallExpr(call->parentExpr);
        SymExpr* sym = toSymExpr(mvCall->get(1));
        lhs = toVarSymbol(sym->symbol());
        lhs->addFlag(FLAG_MAYBE_PARAM);
      }
    }

    result = call->get(2)->remove();
    if (SymExpr* se = toSymExpr(result)) {
      Symbol *sym2 = se->symbol();
      VarSymbol *v = toVarSymbol(se->symbol());
      ArgSymbol *a = toArgSymbol(se->symbol());
      bool isRef = isReferenceType(sym2->type);
      bool isImmediate = false;
      IntentTag intent = INTENT_BLANK;

      if (v) {
        if (v->immediate)
          isImmediate = true;
        intent = concreteIntent(INTENT_BLANK, v->type);
      }

      if (a) {
        intent = concreteIntent(a->intent, a->type);
      }

      if (v || a) {
        if (isRef) {
          // can't take address of something already a ref
        } else if (sym2->type == dtNil) {
          // can't take address of nil
        } else if (sym2->hasFlag(FLAG_EXPR_TEMP) ||
                   sym2->hasFlag(FLAG_TEMP) ||
                   isImmediate ||
                   paramMap.get(sym2)) {
          // can't take address of call temps, param values
          if (lhs)
            lhs->removeFlag(FLAG_MAYBE_REF);
        } else if (a && (intent & INTENT_FLAG_IN)) {
          // don't take the address of arguments passed with in intent
          // (it doesn't help and causes problems with inlining)
        } else if (v &&
                   (intent & INTENT_FLAG_IN) &&
                   v->isConstValWillNotChange()) {
          // don't take address of outer variables declared to be const
          // (otherwise, after flattenFunctions, we will take the
          //  address of a by-value argument).
        } else {
          Expr* stmt = call->getStmtExpr();
          Type* t = sym2->type;
          makeRefType(t);
          VarSymbol* tmp = newTemp("_fold_tmp", t->refType);
          stmt->insertBefore(new DefExpr(tmp));
          stmt->insertBefore(new CallExpr(PRIM_MOVE, tmp, new CallExpr(PRIM_ADDR_OF, result)));
          result = new SymExpr(tmp);
        }
      }
    }
    call->replace(result);
  } else if (call->isPrimitive(PRIM_ADDR_OF)) {
    // remove set ref if already a reference
    if (call->get(1)->typeInfo()->symbol->hasFlag(FLAG_REF) ||
        isTupleContainingOnlyReferences(call->get(1)->typeInfo())) {
      result = call->get(1)->remove();
      call->replace(result);
    } else {
      // Otherwise, we put off further checking until
      // checkResolved().
    }
  } else if (call->isPrimitive(PRIM_DEREF)) {
    // remove deref if arg is already a value
    if (!call->get(1)->typeInfo()->symbol->hasFlag(FLAG_REF)) {
      result = call->get(1)->remove();
      call->replace(result);
    }
  } else if (call->isPrimitive(PRIM_TYPE_TO_STRING)) {
    SymExpr* se = toSymExpr(call->get(1));
    INT_ASSERT(se && se->symbol()->hasFlag(FLAG_TYPE_VARIABLE));
    result = new SymExpr(new_StringSymbol(se->symbol()->type->symbol->name));
    call->replace(result);
  } else if (call->isPrimitive(PRIM_WIDE_GET_LOCALE) ||
             call->isPrimitive(PRIM_WIDE_GET_NODE)) {
    Type* type = call->get(1)->getValType();

    //
    // ensure .locale (and on) are applied to lvalues or classes
    // (locale type is a class)
    //
    SymExpr* se = toSymExpr(call->get(1));
    if (se->symbol()->hasFlag(FLAG_EXPR_TEMP) && !isClass(type))
      USR_WARN(se, "accessing the locale of a local expression");

    //
    // if .locale is applied to an expression of array, domain, or distribution
    // wrapper type, apply .locale to the _value field of the
    // wrapper
    //
    if (isRecordWrappedType(type)) {
      VarSymbol* tmp = newTemp("_locale_tmp_");
      call->getStmtExpr()->insertBefore(new DefExpr(tmp));
      result = new CallExpr("_value", gMethodToken, call->get(1)->remove());
      call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, result));
      call->insertAtTail(tmp);
    }
  } else if (call->isPrimitive(PRIM_TO_STANDALONE)) {
    FnSymbol* iterator = getTheIteratorFn(call);
    CallExpr* standaloneCall = new CallExpr(iterator->name);
    for_formals(formal, iterator) {
      // Note: this can add a use formal outside of its function
      // This is cleaned up in cleanupLeaderFollowerIteratorCalls
      standaloneCall->insertAtTail(new NamedExpr(formal->name,
                                                 symOrParamExpr(formal)));
    }
    // "tag" should be placed at the end of the formals in the source code as
    // well, to avoid insertion of an order wrapper.
    standaloneCall->insertAtTail(new NamedExpr("tag", new SymExpr(gStandaloneTag)));
    call->replace(standaloneCall);
    result = standaloneCall;
  } else if (call->isPrimitive(PRIM_TO_LEADER)) {
    FnSymbol* iterator = getTheIteratorFn(call);
    CallExpr* leaderCall;
    if (FnSymbol* leader = iteratorLeaderMap.get(iterator))
      leaderCall = new CallExpr(leader);
    else
      leaderCall = new CallExpr(iterator->name);
    for_formals(formal, iterator) {
      // Note: this can add a use formal outside of its function
      // This is cleaned up in cleanupLeaderFollowerIteratorCalls
      leaderCall->insertAtTail(new NamedExpr(formal->name,
                                             symOrParamExpr(formal)));
    }
    // "tag" should be placed at the end of the formals in the source code as
    // well, to avoid insertion of an order wrapper.
    leaderCall->insertAtTail(new NamedExpr("tag", new SymExpr(gLeaderTag)));
    call->replace(leaderCall);
    result = leaderCall;
  } else if (call->isPrimitive(PRIM_TO_FOLLOWER)) {
    FnSymbol* iterator = getTheIteratorFn(call);
    CallExpr* followerCall;
    if (FnSymbol* follower = iteratorFollowerMap.get(iterator))
      followerCall = new CallExpr(follower);
    else
      followerCall = new CallExpr(iterator->name);
    for_formals(formal, iterator) {
      // Note: this can add a use formal outside of its function
      // This is cleaned up in cleanupLeaderFollowerIteratorCalls
      followerCall->insertAtTail(new NamedExpr(formal->name,
                                               symOrParamExpr(formal)));
    }
    // "tag", "followThis" and optionally "fast" should be placed at the end
    // of the formals in the source code as well, to avoid insertion of an
    // order wrapper.
    followerCall->insertAtTail(new NamedExpr("tag", new SymExpr(gFollowerTag)));
    followerCall->insertAtTail(new NamedExpr(iterFollowthisArgname, call->get(2)->remove()));
    if (call->numActuals() > 1) {
      followerCall->insertAtTail(new NamedExpr("fast", call->get(2)->remove()));
    }
    call->replace(followerCall);
    result = followerCall;
  } else if (call->isPrimitive(PRIM_NUM_FIELDS)) {
    AggregateType* classtype =
      toAggregateType(toSymExpr(call->get(1))->symbol()->type);
    INT_ASSERT( classtype != NULL );
    classtype = toAggregateType(classtype->getValType());
    INT_ASSERT( classtype != NULL );

    int fieldcount = 0;

    for_fields(field, classtype) {
      if( ! isNormalField(field) ) continue;

      fieldcount++;
    }

    result = new SymExpr(new_IntSymbol(fieldcount));

    call->replace(result);
  } else if (call->isPrimitive(PRIM_FIELD_NUM_TO_NAME)) {
    AggregateType* classtype =
      toAggregateType(toSymExpr(call->get(1))->symbol()->type);
    INT_ASSERT( classtype != NULL );
    classtype = toAggregateType(classtype->getValType());
    INT_ASSERT( classtype != NULL );

    VarSymbol* var = toVarSymbol(toSymExpr(call->get(2))->symbol());

    INT_ASSERT( var != NULL );

    int fieldnum = var->immediate->int_value();
    int fieldcount = 0;
    const char* name = NULL;
    for_fields(field, classtype) {
      if( ! isNormalField(field) ) continue;

      fieldcount++;
      if (fieldcount == fieldnum) {
        name = field->name;
        // break could be here, but might have issues with GCC 5.10
      }
    }
    if (!name) {
      // In this case, we ran out of fields without finding the number
      // specified.  This is the user's error.
      USR_FATAL(call, "'%d' is not a valid field number for %s", fieldnum,
                toString(classtype));
    }
    result = new SymExpr(new_StringSymbol(name));
    call->replace(result);
  } else if (call->isPrimitive(PRIM_FIELD_BY_NUM)) {
    // if call->get(1) is a reference type, dereference it
    AggregateType* classtype = toAggregateType(call->get(1)->typeInfo());
    INT_ASSERT( classtype != NULL );
    classtype = toAggregateType(classtype->getValType());
    INT_ASSERT( classtype != NULL );

    VarSymbol* var = toVarSymbol(toSymExpr(call->get(2))->symbol());

    INT_ASSERT( var != NULL );

    int fieldnum = var->immediate->int_value();
    int fieldcount = 0;
    const char* name = NULL;

    for_fields(field, classtype) {
      if( ! isNormalField(field) ) continue;

      fieldcount++;
      if (fieldcount == fieldnum) {
        name = field->name;
        // break could be here, but seems to cause issues with GCC 5.10
      }
    }
    if (!name) {
      USR_FATAL(call, "'%d' is not a valid field number for %s", fieldnum,
                toString(classtype));
    }
    result = new CallExpr(PRIM_GET_MEMBER, call->get(1)->copy(),
                          new_CStringSymbol(name));
    call->replace(result);
  } else if (call->isPrimitive(PRIM_FIELD_NAME_TO_NUM)) {
    AggregateType* classtype =
      toAggregateType(toSymExpr(call->get(1))->symbol()->type);
    INT_ASSERT( classtype != NULL );
    classtype = toAggregateType(classtype->getValType());
    INT_ASSERT( classtype != NULL );

    VarSymbol* var = toVarSymbol(toSymExpr(call->get(2))->symbol());
    INT_ASSERT( var != NULL );

    Immediate* imm = var->immediate;

    INT_ASSERT( classtype != NULL );
    // fail horribly if immediate is not a string .
    INT_ASSERT(imm->const_kind == CONST_KIND_STRING);

    const char* fieldname = imm->v_string;
    int fieldcount = 0;
    int num = 0;  // return 0 if the field is not found.
    for_fields(field, classtype) {
      if( ! isNormalField(field) ) continue;

      fieldcount++;
      if ( 0 == strcmp(field->name,  fieldname) ) {
        num = fieldcount;
        // break could be here, but might have issues with GCC 5.10
      }
    }
    result = new SymExpr(new_IntSymbol(num));

    call->replace(result);
  } else if (call->isPrimitive(PRIM_CALL_RESOLVES) ||
             call->isPrimitive(PRIM_METHOD_CALL_RESOLVES)) {
    Expr* fnName = NULL;
    Expr* callThis = NULL;

    // this would be easier if we had a non-normalized AST!
    // That is, if this call could contain a whole expression subtree.
    int first_arg;
    if( call->isPrimitive(PRIM_METHOD_CALL_RESOLVES) ) {
      // get(1) should be a receiver
      // get(2) should be a string function name.
      callThis = call->get(1);
      fnName = call->get(2);
      first_arg = 3;
    } else {
      // get(1) should be a string function name.
      fnName = call->get(1);
      first_arg = 2;
    }
    VarSymbol* var = toVarSymbol(toSymExpr(fnName)->symbol());
    INT_ASSERT( var != NULL );
    // the rest are arguments.
    Immediate* imm = var->immediate;
    // fail horribly if immediate is not a string .
    if (!imm)
      INT_FATAL(call, "proc name required");
    if (imm->const_kind != CONST_KIND_STRING)
      INT_FATAL(call, "proc name must be a string");

    const char* name = imm->v_string;

    // temporarily add a call to try resolving.
    CallExpr* tryCall = NULL;
    if( call->isPrimitive(PRIM_METHOD_CALL_RESOLVES) ) {
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
      if( i >= first_arg ) { // skip fn name, maybe method receiver
        tryCall->insertAtTail(actual->copy());
      }
      i++;
    }

    // Try to resolve it.
    if( tryResolveCall(tryCall) ) {
      result = new SymExpr(gTrue);
    } else {
      result = new SymExpr(gFalse);
    }

    // remove the call from the AST
    tryCall->remove();

    call->replace(result);
  } else if (call->isPrimitive(PRIM_ENUM_MIN_BITS) || call->isPrimitive(PRIM_ENUM_IS_SIGNED)) {
    EnumType* et = toEnumType(toSymExpr(call->get(1))->symbol()->type);


    ensureEnumTypeResolved(et);

    result = NULL;
    if( call->isPrimitive(PRIM_ENUM_MIN_BITS) ) {
      result = new SymExpr(new_IntSymbol(get_width(et->integerType)));
    } else if( call->isPrimitive(PRIM_ENUM_IS_SIGNED) ) {
      if( is_int_type(et->integerType) )
        result = new SymExpr(gTrue);
      else
        result = new SymExpr(gFalse);
    }
    call->replace(result);
  } else if (call->isPrimitive(PRIM_IS_UNION_TYPE)) {
    AggregateType* classtype = toAggregateType(call->get(1)->typeInfo());

    if( isUnion(classtype) )
      result = new SymExpr(gTrue);
    else
      result = new SymExpr(gFalse);
    call->replace(result);

  } else if (call->isPrimitive(PRIM_IS_ATOMIC_TYPE)) {
    if (isAtomicType(call->get(1)->typeInfo()))
      result = new SymExpr(gTrue);
    else
      result = new SymExpr(gFalse);
    call->replace(result);

  } else if (call->isPrimitive(PRIM_IS_REF_ITER_TYPE)) {
    if (isRefIterType(call->get(1)->typeInfo()))
      result = new SymExpr(gTrue);
    else
      result = new SymExpr(gFalse);
    call->replace(result);
  } else if (call->isPrimitive(PRIM_IS_POD)) {
    Type* t = call->get(1)->typeInfo();
    // call propagateNotPOD to set FLAG_POD/FLAG_NOT_POD
    propagateNotPOD(t);
    if (isPOD(t))
      result = new SymExpr(gTrue);
    else
      result = new SymExpr(gFalse);
    call->replace(result);
  } else if (call->isPrimitive(PRIM_IS_TUPLE_TYPE)) {
    Type* tupleType = call->get(1)->typeInfo();
    if (tupleType->symbol->hasFlag(FLAG_TUPLE))
      result = new SymExpr(gTrue);
    else
      result = new SymExpr(gFalse);
    call->replace(result);
  } else if (call->isPrimitive(PRIM_IS_STAR_TUPLE_TYPE)) {
    Type* tupleType = call->get(1)->typeInfo();
    // If the type isn't a tuple, it definitely isn't a homogeneous tuple!
    if (tupleType->symbol->hasFlag(FLAG_TUPLE) &&
        tupleType->symbol->hasFlag(FLAG_STAR_TUPLE))
      result = new SymExpr(gTrue);
    else
      result = new SymExpr(gFalse);
    call->replace(result);
  } else if (call->isPrimitive(PRIM_GET_SVEC_MEMBER)) {
    // Convert these to PRIM_GET_SVEC_MEMBER_VALUE if the
    // field in question is a reference.
    // An alternative to this transformation here would be
    // to build homog. tuple accessor entirely in tuples.cpp.
    AggregateType *ct = toAggregateType(call->get(1)->getValType());
    if (ct && ct->symbol->hasFlag(FLAG_STAR_TUPLE)) {
      FnSymbol* inFn = toFnSymbol(call->parentSymbol);
      if (inFn && inFn->hasFlag(FLAG_STAR_TUPLE_ACCESSOR)) {
        QualifiedType fieldType = ct->getFieldType(call->get(2));
        if (fieldType.type() && fieldType.isRef()) {
          if (call->isPrimitive(PRIM_GET_SVEC_MEMBER)) {
            Expr* base = call->get(1);
            Expr* field = call->get(2);
            result = new CallExpr(PRIM_GET_SVEC_MEMBER_VALUE,
                                  base->remove(), field->remove());
            call->replace(result);
          }
        }
      }
    }
  }

  //
  // ensure result of pre-folding is in the AST
  //
  INT_ASSERT(result->parentSymbol);

  return result;
}




// Returns NULL if no substitution was made.  Otherwise, returns the expression
// that replaced 'call'.
static Expr* resolveTupleIndexing(CallExpr* call, Symbol* baseVar)
{
  if (call->numActuals() != 3)
    USR_FATAL(call, "illegal tuple indexing expression");
  Type* indexType = call->get(3)->getValType();
  if (!is_int_type(indexType) && !is_uint_type(indexType))
    USR_FATAL(call, "tuple indexing expression is not of integral type");

  AggregateType* baseType = toAggregateType(baseVar->getValType());
  int64_t index;
  uint64_t uindex;
  char field[8];

  if (get_int(call->get(3), &index)) {
    sprintf(field, "x%" PRId64, index);
    if (index <= 0 || index >= baseType->fields.length)
      USR_FATAL(call, "tuple index out-of-bounds error (%ld)", index);
  } else if (get_uint(call->get(3), &uindex)) {
    sprintf(field, "x%" PRIu64, uindex);
    if (uindex <= 0 || uindex >= (unsigned long)baseType->fields.length)
      USR_FATAL(call, "tuple index out-of-bounds error (%lu)", uindex);
  } else {
    return NULL; // not a tuple indexing expression
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
        if (!fieldType->symbol->hasFlag(FLAG_ARRAY)) {
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
  AggregateType* ct = toAggregateType(call->get(1)->getValType());
  INT_ASSERT(ct);
  SymExpr* last = toSymExpr(call->get(call->numActuals()));
  INT_ASSERT(last);
  VarSymbol* var = toVarSymbol(last->symbol());
  INT_ASSERT(var && var->immediate);
  if (var->immediate->const_kind == CONST_KIND_STRING) {
    // field queried by name
    return ct->getField(var->immediate->v_string, false);
  } else {
    // field queried by position
    int position = var->immediate->int_value();
    Vec<ArgSymbol*> args;
    for_formals(arg, ct->defaultTypeConstructor) {
      args.add(arg);
    }
    for (int i = 2; i < call->numActuals(); i++) {
      SymExpr* actual = toSymExpr(call->get(i));
      INT_ASSERT(actual);
      VarSymbol* var = toVarSymbol(actual->symbol());
      INT_ASSERT(var && var->immediate && var->immediate->const_kind == CONST_KIND_STRING);
      for (int j = 0; j < args.n; j++) {
        if (args.v[j] && !strcmp(args.v[j]->name, var->immediate->v_string))
          args.v[j] = NULL;
      }
    }
    forv_Vec(ArgSymbol, arg, args) {
      if (arg) {
        if (position == 1)
          return ct->getField(arg->name, false);
        position--;
      }
    }
  }
  return NULL;
}


//
// returns true if the field was instantiated
//
static bool isInstantiatedField(Symbol* field) {
  TypeSymbol* ts = toTypeSymbol(field->defPoint->parentSymbol);
  INT_ASSERT(ts);
  AggregateType* ct = toAggregateType(ts->type);
  INT_ASSERT(ct);
  for_formals(formal, ct->defaultTypeConstructor) {
    if (!strcmp(field->name, formal->name))
      if (formal->hasFlag(FLAG_TYPE_VARIABLE))
        return true;
  }
  return false;
}

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

  resolveFormals(captured_fn);
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

  AggregateType *parent;
  FnSymbol *thisParentMethod;

  std::string parent_name = buildParentName(captured_fn->formals, true, captured_fn->retType);

  if (functionTypeMap.find(parent_name) != functionTypeMap.end()) {
    std::pair<AggregateType*, FnSymbol*> ctfs = functionTypeMap[parent_name];
    parent = ctfs.first;
    thisParentMethod = ctfs.second;
  }
  else {
    parent = createAndInsertFunParentClass(call, parent_name.c_str());
    thisParentMethod = createAndInsertFunParentMethod(call, parent, captured_fn->formals, true, captured_fn->retType);
    functionTypeMap[parent_name] = std::pair<AggregateType*, FnSymbol*>(parent, thisParentMethod);
  }

  AggregateType *ct = new AggregateType(AGGREGATE_CLASS);
  std::ostringstream fcf_name;
  fcf_name << "_chpl_fcf_" << unique_fcf_id++ << "_" << flname;

  TypeSymbol *ts = new TypeSymbol(astr(fcf_name.str().c_str()), ct);

  call->parentExpr->insertBefore(new DefExpr(ts));

  ct->dispatchParents.add(parent);

  bool inserted = parent->dispatchChildren.add_exclusive(ct);
  INT_ASSERT(inserted);

  VarSymbol* super = new VarSymbol("super", parent);

  super->addFlag(FLAG_SUPER_CLASS);

  ct->fields.insertAtHead(new DefExpr(super));

  ct->buildConstructors();

  buildDefaultDestructor(ct);

  FnSymbol *thisMethod = new FnSymbol("this");

  thisMethod->addFlag(FLAG_FIRST_CLASS_FUNCTION_INVOCATION);

  thisMethod->insertFormalAtTail(new ArgSymbol(INTENT_BLANK,
                                               "_mt",
                                               dtMethodToken));

  thisMethod->addFlag(FLAG_METHOD);

  ArgSymbol *thisSymbol = new ArgSymbol(INTENT_BLANK, "this", ct);

  thisSymbol->addFlag(FLAG_ARG_THIS);

  thisMethod->insertFormalAtTail(thisSymbol);

  thisMethod->_this = thisSymbol;

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

    if (fArg->type->symbol->hasFlag(FLAG_GENERIC)) {
      USR_FATAL_CONT("Generic functions can't be captured as values");
    }

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

  call->parentExpr->insertBefore(new DefExpr(thisMethod));

  normalize(thisMethod);

  ct->methods.add(thisMethod);

  FnSymbol* wrapper = new FnSymbol("wrapper");

  wrapper->addFlag(FLAG_INLINE);

  wrapper->insertAtTail(new CallExpr(PRIM_RETURN,
                                     new CallExpr(PRIM_CAST,
                                                  parent->symbol,
                                                  new CallExpr(ct->defaultInitializer))));

  call->getStmtExpr()->insertBefore(new DefExpr(wrapper));

  normalize(wrapper);

  CallExpr* callWrapper = new CallExpr(wrapper);

  functionCaptureMap[captured_fn] = wrapper;

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
  std::string    parent_name = buildParentName(argList, false, retType);

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
                                                  retType);

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

  if (SymExpr* sym = toSymExpr(call->castFrom())) {
    if (LcnSymbol* var = toLcnSymbol(sym->symbol())) {
      // Casts of type variables are always required
      // eg. foo.type:string
      if (!var->hasFlag(FLAG_TYPE_VARIABLE)) {
        if (SymExpr* sym = toSymExpr(call->castTo())) {
          Type* oldType = var->type->getValType();
          Type* newType = sym->symbol()->type->getValType();

          if (newType == oldType) {
            result = new SymExpr(var);
            call->replace(result);
          }
        }
      }
    } else if (EnumSymbol* e = toEnumSymbol(sym->symbol())) {
      if (SymExpr* sym = toSymExpr(call->castTo())) {
        EnumType* oldType = toEnumType(e->type);
        EnumType* newType = toEnumType(sym->symbol()->type);
        if (newType && oldType == newType) {
          result = new SymExpr(e);
          call->replace(result);
        }
      }
    }
  }
  return result;
}

static SymExpr* symOrParamExpr(Symbol* arg) {
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
  // TODO -- this will break user fields named outer!
  if( 0 == strcmp("outer", field->name)) return false;

  return true;
}

/*
  Builds up the name of the parent for lookup by looking through the types
  of the arguments, either formal or actual
*/
static std::string buildParentName(AList& arg_list,
                                   bool   isFormal,
                                   Type*  retType) {
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

  parent->buildConstructors();

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
                                                Type*          retType) {
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

    rtGetter->addFlag(FLAG_METHOD);
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

    atGetter->addFlag(FLAG_METHOD);
    atGetter->addFlag(FLAG_METHOD_PRIMARY);
    atGetter->cname = astr("chpl_get_",
                           parent->symbol->cname, "_",
                           atGetter->cname);
    atGetter->addFlag(FLAG_NO_PARENS);
    atGetter->_this = _this;
  }

  FnSymbol* parent_method = new FnSymbol("this");

  parent_method->addFlag(FLAG_FIRST_CLASS_FUNCTION_INVOCATION);
  parent_method->insertFormalAtTail(new ArgSymbol(INTENT_BLANK,
                                                  "_mt",
                                                  dtMethodToken));
  parent_method->addFlag(FLAG_METHOD);

  ArgSymbol* thisParentSymbol = new ArgSymbol(INTENT_BLANK, "this", parent);

  thisParentSymbol->addFlag(FLAG_ARG_THIS);

  parent_method->insertFormalAtTail(thisParentSymbol);
  parent_method->_this = thisParentSymbol;

  int i = 0, alength = arg_list.length;

  // We handle the arg list differently depending on if it's a list of
  // formal args or actual args
  if (isFormal) {

    for_alist(formalExpr, arg_list) {
      DefExpr* dExp = toDefExpr(formalExpr);
      ArgSymbol* fArg = toArgSymbol(dExp->sym);

      if (fArg->type != dtVoid) {
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

        if (sExpr->symbol()->type != dtVoid) {
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

  // Because this function type needs to be globally visible
  // (because we don't know the modules it will be passed to), we put
  // it at the highest scope
  theProgram->block->body.insertAtTail(new DefExpr(parent_method));

  normalize(parent_method);

  parent->methods.add(parent_method);

  return parent_method;
}
