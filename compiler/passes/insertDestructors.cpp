#include "passes.h"
#include "symbol.h"
#include "expr.h"
#include "type.h"
#include "stmt.h"
#include "flags.h"
#include "baseAST.h"
#include "astutil.h"
#include "alist.h"
#include "vec.h"
#include "map.h"
#include "misc.h"

// This file is under construction!  Please pardon the dust and noise!


bool fEnableDestructorCalls = false;

void fixupDestructors(void) {

  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (ts->type->destructor) {
      ClassType* ct = toClassType(ts->type);
      INT_ASSERT(ct);

      //
      // insert calls to destructors for all 'value' fields
      //
      for_fields_backward(field, ct) {
        if (field->type->destructor) {
          ClassType* fct = toClassType(field->type);
          INT_ASSERT(fct);
          if (fct->classTag != CLASS_CLASS) {
            VarSymbol* tmp = newTemp(fct->refType);
            ct->destructor->insertBeforeReturnAfterLabel(new DefExpr(tmp));
            ct->destructor->insertBeforeReturnAfterLabel(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_GET_MEMBER, ct->destructor->_this, field)));
            ct->destructor->insertBeforeReturnAfterLabel(new CallExpr(field->type->destructor, tmp));
          }
        }
      }

      //
      // insert call to parent destructor
      //
      INT_ASSERT(ct->dispatchParents.n <= 1);
      if (ct->dispatchParents.n >= 1) {
        if (FnSymbol* parentDestructor = ct->dispatchParents.v[0]->destructor) {
          VarSymbol* tmp;
          if (ct->classTag == CLASS_CLASS || ct->symbol->hasFlag(FLAG_ARRAY))
            tmp = newTemp(ct->dispatchParents.v[0]);
          else
            tmp = newTemp(ct->dispatchParents.v[0]->refType);
          ct->destructor->insertBeforeReturnAfterLabel(new DefExpr(tmp));
          ct->destructor->insertBeforeReturnAfterLabel(new CallExpr(PRIMITIVE_MOVE, tmp,
            new CallExpr(PRIMITIVE_CAST,
              ct->classTag == CLASS_CLASS || ct->symbol->hasFlag(FLAG_ARRAY) ?
                ct->dispatchParents.v[0]->symbol : ct->dispatchParents.v[0]->refType->symbol,
              ct->destructor->_this)));
          ct->destructor->insertBeforeReturnAfterLabel(new CallExpr(parentDestructor, tmp));
        }
      }
    }
  }
}

#if 0
static bool arrayPassedAsArgument(FnSymbol *fn) {
  for (int i = 1; i <= fn->numFormals(); i++)
    if (fn->getFormal(i)->type->symbol->hasFlag(FLAG_ARRAY))
      return true;
  return false;
}
#endif

static bool tupleContainsArrayOrDomain(ClassType* t) {
  for (int i = 1; i <= t->fields.length; i++) {
    Type* fieldType = t->getField(i)->type;
    ClassType* fieldClassType = toClassType(fieldType);
    if (fieldType->symbol->hasFlag(FLAG_ARRAY) || fieldType->symbol->hasFlag(FLAG_DOMAIN))
      return true;
    else if (fieldType->symbol->hasFlag(FLAG_TUPLE) && tupleContainsArrayOrDomain(toClassType(fieldType)))
      return true;
    else if (fieldClassType && fieldClassType->classTag != CLASS_CLASS && tupleContainsArrayOrDomain(toClassType(fieldType)))
      return true;
  }
  return false;
}

static void insertDestructorsCalls(bool onlyMarkConstructors) {

  if (!fEnableDestructorCalls) return;

  //
  // insert destructors for values when they go out of scope
  //
  compute_call_sites();

  Map<Symbol*,Vec<SymExpr*>*> defMap;
  Map<Symbol*,Vec<SymExpr*>*> useMap;
  buildDefUseMaps(defMap, useMap);

  Vec<FnSymbol*> constructors, functionsContainingGotos;

  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    ClassType* ct = toClassType(ts->type);
    if (ct && ct->classTag != CLASS_CLASS && ct->defaultConstructor && ct->destructor &&
        //(!ct->symbol->hasFlag(FLAG_ARRAY) || !arrayPassedAsArgument(fn)) &&
        (!ct->symbol->hasFlag(FLAG_TUPLE) || !tupleContainsArrayOrDomain(ct))) {
      constructors.add(ct->defaultConstructor);
    }
  }

  if (!onlyMarkConstructors) {
    forv_Vec(FnSymbol, fn, gFnSymbols) {
      //ClassType* ct = toClassType(fn->retType);
      if (fn->hasFlag(FLAG_CALLS_CONSTRUCTOR) && fn->calledBy /*&&
          ct && ct->classTag != CLASS_CLASS && ct->destructor &&
          //(!ct->symbol->hasFlag(FLAG_ARRAY) || !arrayPassedAsArgument(fn)) &&
          (!ct->symbol->hasFlag(FLAG_TUPLE) || !tupleContainsArrayOrDomain(ct))*/) {
        constructors.add(fn);
      }
    }
    forv_Vec(GotoStmt, gs, gGotoStmts) {
      FnSymbol* fn = toFnSymbol(gs->parentSymbol);
      INT_ASSERT(fn);
      functionsContainingGotos.set_add(fn);
    }
  }

  forv_Vec(FnSymbol, constructor, constructors) {
    forv_Vec(CallExpr, call, *constructor->calledBy) {
      if (call->parentSymbol) {
        CallExpr* move = toCallExpr(call->parentExpr);
        if (!move) continue;
        INT_ASSERT(move->isPrimitive(PRIMITIVE_MOVE));
        SymExpr* lhs = toSymExpr(move->get(1));
        INT_ASSERT(lhs);
        if (!lhs->var->type->destructor || !gFnSymbols.in(lhs->var->type->destructor) ||
            // don't destroy global variables
            isModuleSymbol(lhs->var->defPoint->parentSymbol)) {
          continue;
        }
        //if (lhs->var->type->symbol->hasFlag(FLAG_DOMAIN)) continue;
        FnSymbol* fn = toFnSymbol(move->parentSymbol);
        INT_ASSERT(fn);
        // functions containing gotos may skip over initializations,
        // potentially causing uninitialized variables to be freed;
        // functions with "_loopexpr" in their name are most likely related to reduce
        // expressions; they may embed domains, for example, into iterator classes
        if (functionsContainingGotos.set_in(fn) || strstr(fn->name, "_loopexpr"))
          continue;
        if (fn->getReturnSymbol() == lhs->var) {
          if (defMap.get(lhs->var)->n == 1) {
            constructors.add_exclusive(fn);
            if (onlyMarkConstructors)
              fn->addFlag(FLAG_CALLS_CONSTRUCTOR);
          }
        } else {
          Vec<Symbol*> varsToTrack;
          varsToTrack.add(lhs->var);
          bool maybeCallDestructor = fEnableDestructorCalls;
          INT_ASSERT(lhs->var->defPoint && lhs->var->defPoint->parentExpr);
          BlockStmt* parentBlock = toBlockStmt(lhs->var->defPoint->parentExpr);
          forv_Vec(Symbol, var, varsToTrack) {
            // may not be OK if there is more than one definition of var
            //INT_ASSERT(defMap.get(var)->length() == 1);
            if (maybeCallDestructor && useMap.get(var))
              forv_Vec(SymExpr, se, *useMap.get(var)) {
                // the following conditional should not be necessary, but there may be cases
                // in which a variable is used outside of the block in which it is defined!
                if (var == lhs->var) {
                  Expr* block = se->parentExpr;
                  while (block && toBlockStmt(block) != parentBlock)
                    block = block->parentExpr;
                  if (!toBlockStmt(block))
                  {
                    // changing the parentBlock here could cause a variable's destructor
                    // to be called outside the conditional block in which it is defined,
                    // which could cause the destructor to be called on an unitialized
                    // variable
//                    parentBlock = fn->body;
                    maybeCallDestructor = false;
                    break;
                  }
                }
                CallExpr* call = toCallExpr(se->parentExpr);
                if (call && (//call->isPrimitive(PRIMITIVE_SET_REF) || call->isPrimitive(PRIMITIVE_GET_MEMBER) ||
                    call->isPrimitive(PRIMITIVE_GET_MEMBER_VALUE)))
                  call = toCallExpr(call->parentExpr);
                if (call) {
                  if (call->isPrimitive(PRIMITIVE_MOVE)) {
                    if (fn->getReturnSymbol() == toSymExpr(call->get(1))->var) {
                      maybeCallDestructor = false;
                      if (defMap.get(toSymExpr(call->get(1))->var)->n == 1 /*&&
                          strcmp(fn->name, "reindex") && !strstr(fn->name, "buildDomainExpr")*/) {
                        constructors.add_exclusive(fn);
                        if (onlyMarkConstructors)
                          fn->addFlag(FLAG_CALLS_CONSTRUCTOR);
                      }
                      break;
#if 0
                    } else if (toFnSymbol(call->parentSymbol)->getReturnSymbol() == toSymExpr(call->get(1))->var) {
                      // if call->parentSymbol is a different function than fn,
                      // it is nontrivial to track down how the caller uses the return value
                      maybeCallDestructor = false;
                      break;
#endif
                    } else if (isModuleSymbol(toSymExpr(call->get(1))->var->defPoint->parentSymbol)) {
                      // lhs is directly or indirectly being assigned to a global variable
                      maybeCallDestructor = false;
                      break;
                    } else if (toSymExpr(call->get(1))->var->defPoint->parentSymbol == fn) {
                      if (toSymExpr(call->get(1))->var->defPoint->parentExpr != parentBlock) {
                        Expr* block = parentBlock->parentExpr;
                        while (block && block != toSymExpr(call->get(1))->var->defPoint->parentExpr)
                          block = block->parentExpr;
                        if (toBlockStmt(block)) {
                          // changing the parentBlock here could cause a variable's destructor
                          // to be called outside the conditional block in which it is defined,
                          // which could cause the destructor to be called on an unitialized
                          // variable
//                          parentBlock = toBlockStmt(block);
                          maybeCallDestructor = false;
                          break;
                        }
                      }
                    }
                    varsToTrack.add_exclusive(toSymExpr(call->get(1))->var);
#if 0
                  } else if (var->type->symbol->hasFlag(FLAG_ARRAY) &&
                             call->isResolved() && call->isResolved()->hasFlag(FLAG_DEFAULT_CONSTRUCTOR)) {
//                printf("Probably shouldn't call destructor on %s\n", lhs->var->cname);
                    maybeCallDestructor = false;
                    break;
#endif
                  } else if (call->isPrimitive(PRIMITIVE_SET_MEMBER) &&
                             !toSymExpr(call->get(1))->var->type->symbol->hasFlag(FLAG_RUNTIME_TYPE_VALUE)) {
                    maybeCallDestructor = false;
                    break;
                  } else if (call->isPrimitive(PRIMITIVE_ARRAY_SET_FIRST)) {
                    // used (only) in init_elts in ChapelBase.chpl
                    maybeCallDestructor = false;
                    break;
                  } else if (!call->primitive) {
                    varsToTrack.add_exclusive(actual_to_formal(se));
                  }
                } else
                  printf("found a use of %s\n", var->cname);
              }
          }
          if (maybeCallDestructor && !onlyMarkConstructors) {
            // lhs does not "escape" its scope, so go ahead and insert a call to its destructor
            ClassType* ct = toClassType(lhs->var->type);
            bool useRefType = ct->classTag != CLASS_CLASS &&
              !ct->symbol->hasFlag(FLAG_ARRAY) && !ct->symbol->hasFlag(FLAG_DOMAIN);
            if (parentBlock == fn->body) {
              if (useRefType) {
                VarSymbol* tmp = newTemp(ct->refType);
                fn->insertBeforeReturnAfterLabel(new DefExpr(tmp));
                fn->insertBeforeReturnAfterLabel(new CallExpr(PRIMITIVE_MOVE, tmp,
                  new CallExpr(PRIMITIVE_SET_REF, lhs->var)));
                fn->insertBeforeReturnAfterLabel(new CallExpr(ct->destructor, tmp));
              } else {
                VarSymbol* tmp = newTemp(ct);
                fn->insertBeforeReturnAfterLabel(new DefExpr(tmp));
                fn->insertBeforeReturnAfterLabel(new CallExpr(PRIMITIVE_MOVE, tmp, lhs->var));
                fn->insertBeforeReturnAfterLabel(new CallExpr(ct->destructor, tmp));
              }
            } else {
              INT_ASSERT(parentBlock);
              if (useRefType) {
                VarSymbol* tmp = newTemp(ct->refType);
                parentBlock->insertAtTailBeforeGoto(new DefExpr(tmp));
                parentBlock->insertAtTailBeforeGoto(new CallExpr(PRIMITIVE_MOVE, tmp,
                  new CallExpr(PRIMITIVE_SET_REF, lhs->var)));
                parentBlock->insertAtTailBeforeGoto(new CallExpr(ct->destructor, tmp));
              } else {
                VarSymbol* tmp = newTemp(ct);
                parentBlock->insertAtTailBeforeGoto(new DefExpr(tmp));
                parentBlock->insertAtTailBeforeGoto(new CallExpr(PRIMITIVE_MOVE, tmp, lhs->var));
                parentBlock->insertAtTailBeforeGoto(new CallExpr(ct->destructor, tmp));
              }
            }
          }
        }
      }
    }
  }

  inlineFunctions();
}

void markConstructors() {
  insertDestructorsCalls(true);
}

void insertDestructors() {
  insertDestructorsCalls(false);
}
