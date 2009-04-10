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
#include <cstring>


bool fEnableDestructorCalls = true;

void fixupDestructors(void) {

  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (ts->type->destructor) {
      ClassType* ct = toClassType(ts->type);
      INT_ASSERT(ct);

      //
      // insert calls to destructors for all 'value' fields
      //
      int count = 0;  // count of how many fields need to be destroyed
      for_fields_backward(field, ct) {
        if (field->type->destructor) {
          ClassType* fct = toClassType(field->type);
          INT_ASSERT(fct);
          if (!isClass(fct)) {
            bool useRefType = !fct->symbol->hasFlag(FLAG_ARRAY) && !fct->symbol->hasFlag(FLAG_DOMAIN);
            VarSymbol* tmp = useRefType ? newTemp(fct->refType) : newTemp(fct);
            ct->destructor->insertBeforeReturnAfterLabel(new DefExpr(tmp));
            ct->destructor->insertBeforeReturnAfterLabel(new CallExpr(PRIM_MOVE, tmp,
              new CallExpr(useRefType ? PRIM_GET_MEMBER : PRIM_GET_MEMBER_VALUE, ct->destructor->_this, field)));
            ct->destructor->insertBeforeReturnAfterLabel(new CallExpr(field->type->destructor, tmp));
          }
          count++;
        } else if (field->type == dtString && !ct->symbol->hasFlag(FLAG_TUPLE)) {
          VarSymbol* tmp = newTemp(dtString);
          ct->destructor->insertBeforeReturnAfterLabel(new DefExpr(tmp));
          ct->destructor->insertBeforeReturnAfterLabel(new CallExpr(PRIM_MOVE, tmp,
            new CallExpr(PRIM_GET_MEMBER_VALUE, ct->destructor->_this, field)));
          ct->destructor->insertBeforeReturnAfterLabel(new CallExpr(PRIM_CHPL_FREE, tmp));
        }
      }
      if (count <= 1 ||
           (count <= 3 && !strncmp(ts->type->destructor->cname, "chpl__auto_destroy", 18)))
        ts->type->destructor->addFlag(FLAG_INLINE);

      //
      // insert call to parent destructor
      //
      INT_ASSERT(ct->dispatchParents.n <= 1);
      if (ct->dispatchParents.n >= 1) {
        if (FnSymbol* parentDestructor = ct->dispatchParents.v[0]->destructor) {
          VarSymbol* tmp;
          if (isClass(ct) || ct->symbol->hasFlag(FLAG_ARRAY) ||
              ct->symbol->hasFlag(FLAG_DOMAIN))
            tmp = newTemp(ct->dispatchParents.v[0]);
          else
            tmp = newTemp(ct->dispatchParents.v[0]->refType);
          ct->destructor->insertBeforeReturnAfterLabel(new DefExpr(tmp));
          ct->destructor->insertBeforeReturnAfterLabel(new CallExpr(PRIM_MOVE, tmp,
            new CallExpr(PRIM_CAST,
                         isClass(ct) || ct->symbol->hasFlag(FLAG_ARRAY) || ct->symbol->hasFlag(FLAG_DOMAIN) ?
                ct->dispatchParents.v[0]->symbol : ct->dispatchParents.v[0]->refType->symbol,
              ct->destructor->_this)));
          ct->destructor->insertBeforeReturnAfterLabel(new CallExpr(parentDestructor, tmp));
        }
      }
    }
  }
}

static bool tupleContainsArrayOrDomain(ClassType* t) {
  for (int i = 1; i <= t->fields.length; i++) {
    Type* fieldType = t->getField(i)->type;
    ClassType* fieldClassType = toClassType(fieldType);
    if (fieldType->symbol->hasFlag(FLAG_ARRAY) || fieldType->symbol->hasFlag(FLAG_DOMAIN) ||
        (fieldType->symbol->hasFlag(FLAG_TUPLE) && tupleContainsArrayOrDomain(toClassType(fieldType))) ||
        (fieldClassType && !isClass(fieldClassType) && tupleContainsArrayOrDomain(toClassType(fieldType))))
      return true;
  }
  return false;
}

static void insertDestructorCalls(bool onlyMarkConstructors) {

  if (!fEnableDestructorCalls) return;

  //
  // insert destructors for values when they go out of scope
  //
  compute_call_sites();

  Map<Symbol*,Vec<SymExpr*>*> defMap, useMap;
  buildDefUseMaps(defMap, useMap);

  if (onlyMarkConstructors) {
    // as a sanity check, look for variable uses outside of the blocks
    // in which they are declared
    forv_Vec(VarSymbol, var, gVarSymbols) {
      // some vars may be bogus at this point because they have been pruned
      if (var->defPoint->parentSymbol &&
          // no need to check global symbols
          !isModuleSymbol(var->defPoint->parentSymbol)) {
        BlockStmt* parentBlock = toBlockStmt(var->defPoint->parentExpr);
        if (!parentBlock) {
          FnSymbol *fn = toFnSymbol(var->defPoint->parentSymbol);
          if (!fn)
            fn = toFnSymbol(var->defPoint->parentSymbol->defPoint->parentSymbol);
          if (fn)
            parentBlock = fn->body;
        }
        if (parentBlock) {
          for_uses(se, useMap, var) {
            Expr* block = se->parentExpr;
            while (block && toBlockStmt(block) != parentBlock)
              block = block->parentExpr;
            if (!block)
              INT_FATAL(se, "var used outside of block in which it is declared");
          }
        }
      }
    }
  }

  Vec<FnSymbol*> constructors, functionsContainingGotos;

  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    ClassType* ct = toClassType(ts->type);
    if (ct && !isClass(ct) && ct->defaultConstructor && ct->destructor &&
        (!ct->symbol->hasFlag(FLAG_TUPLE) || !tupleContainsArrayOrDomain(ct)))
      constructors.add(ct->defaultConstructor);
  }

  if (!onlyMarkConstructors) {
    forv_Vec(FnSymbol, fn, gFnSymbols) {
      if (fn->hasFlag(FLAG_CALLS_CONSTRUCTOR) && fn->calledBy)
        constructors.add(fn);
    }
    forv_Vec(GotoStmt, gs, gGotoStmts) {
      FnSymbol* fn = toFnSymbol(gs->parentSymbol);
      INT_ASSERT(fn);
      functionsContainingGotos.set_add(fn);
    }
  }

  Vec<CallExpr*> constructorCalls;

  forv_Vec(FnSymbol, constructor, constructors) {
    forv_Vec(CallExpr, call, *constructor->calledBy)
      constructorCalls.add(call);
  }
  forv_Vec(CallExpr, call, gCallExprs) {
    // look for casts from a numeric type to string,
    // since they will require allocating memory for the resulting string
    if (call->isPrimitive(PRIM_CAST) && toSymExpr(call->get(1))->var->type == dtString &&
        is_arithmetic_type(toSymExpr(call->get(2))->var->type) ||
        call->isPrimitive(PRIM_STRING_COPY))
      constructorCalls.add(call);
  }

  forv_Vec(CallExpr, call, constructorCalls) {
    if (call->parentSymbol) {
      CallExpr* move = toCallExpr(call->parentExpr);
      if (!move) continue;
      INT_ASSERT(move->isPrimitive(PRIM_MOVE));
      SymExpr* lhs = toSymExpr(move->get(1));
      INT_ASSERT(lhs);
      if (!lhs->var->type->destructor &&
          !call->isPrimitive(PRIM_CAST) && !call->isPrimitive(PRIM_STRING_COPY) &&
          (lhs->var->type != dtString || !isSymExpr(call->baseExpr) ||
            strcmp(toSymExpr(call->baseExpr)->var->name, "_cast") &&
            strcmp(toSymExpr(call->baseExpr)->var->name, "_copy")) ||
          // don't destroy global variables
          isModuleSymbol(lhs->var->defPoint->parentSymbol)) {
        continue;
      }
      FnSymbol* fn = toFnSymbol(move->parentSymbol);
      INT_ASSERT(fn);
      // functions containing gotos may skip over initializations,
      // potentially causing uninitialized variables to be freed;
      // functions with "_loopexpr" in their name are most likely related to reduce
      // expressions; they may embed domains, for example, into iterator classes
      if (functionsContainingGotos.set_in(fn) || strstr(fn->name, "_loopexpr"))
        continue;
      if (fn->getReturnSymbol() == lhs->var) {
        if (defMap.get(lhs->var)->length() == 1) {
          if (!constructors.in(fn)) {
            forv_Vec(CallExpr, call, *fn->calledBy)
              constructorCalls.add(call);
          }
          if (onlyMarkConstructors)
            fn->addFlag(FLAG_CALLS_CONSTRUCTOR);
        }
      } else {
        Vec<Symbol*> varsToTrack;
        varsToTrack.add(lhs->var);
        Expr *onlyOneUse = NULL;
        if (useMap.get(lhs->var) && useMap.get(lhs->var)->length() == 1)
          onlyOneUse = useMap.get(lhs->var)->first();
        bool maybeCallDestructor = true;
        INT_ASSERT(lhs->var->defPoint);
        BlockStmt* parentBlock = toBlockStmt(lhs->var->defPoint->parentExpr);
        INT_ASSERT(parentBlock);
        forv_Vec(Symbol, var, varsToTrack) {
          // may not be OK if there is more than one definition of var
          //INT_ASSERT(defMap.get(var)->length() == 1);
          if (maybeCallDestructor)
            for_uses(se, useMap, var) {
              // The following conditional may not seem necessary, particularly since
              // a similar check appears above, but there may be new cases in which
              // a variable is used outside of the block in which it is defined!
              // (The scalar replace and copy propagation passes could introduce
              // such uses; other uses like this are most likely bugs.)
              if (var == lhs->var) {
                Expr* block = se->parentExpr;
                while (block && toBlockStmt(block) != parentBlock)
                  block = block->parentExpr;
                if (!block)
                  // it is probably possible to work around this situation, but this
                  // situation is very unlikely to occur, so make it a fatal error
                  INT_FATAL(var->defPoint, "variable used outside of block in which it's declared");
              }
              CallExpr* call = toCallExpr(se->parentExpr);
              if (call && (call->isPrimitive(PRIM_SET_REF) || call->isPrimitive(PRIM_GET_REF) /*||
                  call->isPrimitive(PRIM_GET_MEMBER) || call->isPrimitive(PRIM_GET_MEMBER_VALUE)*/) &&
                  lhs->var->type == dtString)
                call = toCallExpr(call->parentExpr);
              if (call) {
                if (call->isPrimitive(PRIM_MOVE)) {
                  if (fn->getReturnSymbol() == toSymExpr(call->get(1))->var) {
                    maybeCallDestructor = false;
                    if (defMap.get(toSymExpr(call->get(1))->var)->n == 1) {
                      if (!constructors.in(fn)) {
                        forv_Vec(CallExpr, call, *fn->calledBy)
                          constructorCalls.add(call);
                      }
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
                  } else if (toSymExpr(call->get(1))->var->type == var->type->refType &&
                             (lhs->var->type != dtString || !isCallExpr(call->get(2)))) {
                    maybeCallDestructor = false;
                    break;
                  } else if (isArgSymbol(toSymExpr(call->get(1))->var)) {
                    // if lhs is being assigned directly or indirectly to an argument,
                    // then the caller could still have access to the piece of memory
                    // in question
                    maybeCallDestructor = false;
                    break;
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
                  Symbol *moveDest = toSymExpr(call->get(1))->var;
                  varsToTrack.add_exclusive(moveDest);
                  if (onlyOneUse &&
                      useMap.get(moveDest) && useMap.get(moveDest)->length() == 1 &&
                      useMap.get(moveDest)->first()->parentSymbol == fn)
                    onlyOneUse = useMap.get(moveDest)->first();
                  // if there are no uses of moveDest, don't change onlyOneUse
                  else if (useMap.get(moveDest) && useMap.get(moveDest)->length() > 1)
                    onlyOneUse = NULL;
#if 1  // can cause some arrays to leak memory
                } else if (call->isPrimitive(PRIM_SET_MEMBER)) {
                  if (toSymExpr(call->get(1))->var->type->symbol->hasFlag(FLAG_RUNTIME_TYPE_VALUE))
                    // free at the end of the block, not right after this use
                    onlyOneUse = NULL;
                  else {
                    maybeCallDestructor = false;
                    break;
                  }
#endif
                } else if (call->isPrimitive(PRIM_ARRAY_SET_FIRST)) {
                  // used (only) in init_elts in ChapelBase.chpl
                  maybeCallDestructor = false;
                  break;
                } else if (!call->primitive) {
                  varsToTrack.add_exclusive(actual_to_formal(se));
                }
              } else
                INT_FATAL(se, "unexpected use");
            }
        }
        if (maybeCallDestructor && !onlyMarkConstructors) {
          // lhs does not "escape" its scope, so go ahead and insert a call to its destructor
          if (ClassType* ct = toClassType(lhs->var->type)) {
            bool useRefType = !isClass(ct) &&
              !ct->symbol->hasFlag(FLAG_ARRAY) && !ct->symbol->hasFlag(FLAG_DOMAIN);
            if (parentBlock == fn->body) {
              VarSymbol* tmp = newTemp(useRefType ? ct->refType : ct);
              fn->insertBeforeReturnAfterLabel(new DefExpr(tmp));
              if (useRefType)
                fn->insertBeforeReturnAfterLabel(new CallExpr(PRIM_MOVE, tmp,
                  new CallExpr(PRIM_SET_REF, lhs->var)));
              else
                fn->insertBeforeReturnAfterLabel(new CallExpr(PRIM_MOVE, tmp, lhs->var));
              fn->insertBeforeReturnAfterLabel(new CallExpr(ct->destructor, tmp));
            } else {
              INT_ASSERT(parentBlock);
              VarSymbol* tmp = newTemp(useRefType ? ct->refType : ct);
              parentBlock->insertAtTailBeforeGoto(new DefExpr(tmp));
              if (useRefType)
                parentBlock->insertAtTailBeforeGoto(new CallExpr(PRIM_MOVE, tmp,
                  new CallExpr(PRIM_SET_REF, lhs->var)));
              else
                parentBlock->insertAtTailBeforeGoto(new CallExpr(PRIM_MOVE, tmp, lhs->var));
              parentBlock->insertAtTailBeforeGoto(new CallExpr(ct->destructor, tmp));
            }
          } else {
            INT_ASSERT(lhs->var->type == dtString);
            if (onlyOneUse) {
              // find the statement that contains the use of lhs;
              // lhs will be freed after this statement
              Expr* parentExpr = onlyOneUse;
              while (parentExpr && !toBlockStmt(parentExpr->parentExpr))
                parentExpr = parentExpr->parentExpr;
              // find out if the only use is inside a loop;
              // if so, free lhs after the outermost loop
              for (Expr* block = parentExpr->parentExpr;
                   block && block != parentBlock;
                   block = block->parentExpr) {
                if (BlockStmt* b = toBlockStmt(block)) {
                  if (b->blockInfo &&
                      (b->blockInfo->isPrimitive(PRIM_BLOCK_PARAM_LOOP) ||
                       b->blockInfo->isPrimitive(PRIM_BLOCK_WHILEDO_LOOP) ||
                       b->blockInfo->isPrimitive(PRIM_BLOCK_DOWHILE_LOOP) ||
                       b->blockInfo->isPrimitive(PRIM_BLOCK_FOR_LOOP)))
                    parentExpr = b;
                }
              }
              // find out if the only use is inside a conditional statement;
              // if so, free lhs after the outermost conditional statement
              for (Expr* condStmt = parentExpr;
                   condStmt->parentExpr && condStmt->parentExpr != parentBlock &&
                     isCondStmt(condStmt->parentExpr->parentExpr); /* do nothing */) {
                condStmt = condStmt->parentExpr->parentExpr;
                parentExpr = condStmt;
              }
              parentExpr->insertAfter(new CallExpr(PRIM_CHPL_FREE, lhs->var));
            } else if (parentBlock == fn->body) {
              fn->insertBeforeReturnAfterLabel(new CallExpr(PRIM_CHPL_FREE, lhs->var));
            } else {
              parentBlock->insertAtTailBeforeGoto(new CallExpr(PRIM_CHPL_FREE, lhs->var));
            }
          }
        }
      }
    }
  }
  freeDefUseMaps(defMap, useMap);
}

void markConstructors() {
  insertDestructorCalls(true);
}

void insertDestructors() {
  insertDestructorCalls(false);
}
