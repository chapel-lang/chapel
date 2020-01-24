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

// refPropagation.cpp
//
#include "optimizations.h"

#include "astutil.h"
#include "driver.h"
#include "expr.h"
#include "passes.h"
#include "stlUtil.h"
#include "stmt.h"
#include "view.h"

static size_t s_ref_repl_count; ///< The number of references replaced this pass.

// If there is exactly one definition of var by something of reference type, 
// then return the call that defines it.
// Otherwise, return NULL.
static CallExpr*
findRefDef(Map<Symbol*,Vec<SymExpr*>*>& defMap, Symbol* var) {
  CallExpr* ret = NULL;
  for_defs(def, defMap, var) {
    if (CallExpr* call = toCallExpr(def->parentExpr)) {
      if (call->isPrimitive(PRIM_MOVE) || call->isPrimitive(PRIM_ASSIGN))
        if (call->get(2)->isRef()) {
          if (ret)
            return NULL;
          else
            ret = call;
        }
    }
  }
  return ret;
}

//
// Attempts to replace references with the variables the references point to,
// provided the references have a single definition.
//
// For example:
// var foo : int;
// ref A : int;
// (move A (addr-of foo))
//
// (move B (deref A))     --->    (move B foo)
//
void
eliminateSingleAssignmentReference(Map<Symbol*,Vec<SymExpr*>*>& defMap,
                                   Map<Symbol*,Vec<SymExpr*>*>& useMap,
                                   Symbol* var) {
  if (CallExpr* move = findRefDef(defMap, var)) {
    if (CallExpr* rhs = toCallExpr(move->get(2))) {
      if (rhs->isPrimitive(PRIM_ADDR_OF) || rhs->isPrimitive(PRIM_SET_REFERENCE)) {
        bool stillAlive = false;
        for_uses(se, useMap, var) {
          CallExpr* parent = toCallExpr(se->parentExpr);
          SET_LINENO(se);
          if (parent && (parent->isPrimitive(PRIM_DEREF) || isDerefMove(parent))) {
            SymExpr* se = toSymExpr(rhs->get(1)->copy());
            INT_ASSERT(se);
            Expr* toReplace = parent;
            if (isMoveOrAssign(parent)) {
              toReplace = parent->get(2);
            }
            toReplace->replace(se);
            ++s_ref_repl_count;
            addUse(useMap, se);
          } else if (parent &&
                     (parent->isPrimitive(PRIM_GET_MEMBER_VALUE) ||
                      parent->isPrimitive(PRIM_GET_MEMBER) ||
                      parent->isPrimitive(PRIM_GET_MEMBER_VALUE) ||
                      parent->isPrimitive(PRIM_GET_MEMBER))) {
            SymExpr* se = toSymExpr(rhs->get(1)->copy());
            INT_ASSERT(se);
            parent->get(1)->replace(se);
            ++s_ref_repl_count;
            addUse(useMap, se);
          }
          else if (parent && (parent->isPrimitive(PRIM_MOVE) || parent->isPrimitive(PRIM_SET_REFERENCE))) {
            CallExpr* rhsCopy = rhs->copy();
            if (parent->isPrimitive(PRIM_SET_REFERENCE)) {
              // Essentially a pointer copy like a (move refA refB)
              parent = toCallExpr(parent->parentExpr);
              INT_ASSERT(parent && isMoveOrAssign(parent));
            }
            parent->get(2)->replace(rhsCopy);
            ++s_ref_repl_count;
            SymExpr* se = toSymExpr(rhsCopy->get(1));
            INT_ASSERT(se);
            addUse(useMap, se);
            // BHARSH TODO: Is it possible to handle the following case safely
            // for PRIM_ASSIGN?
            //
            // ref i_foo : T;
            // (move i_foo (set reference bar))
            // (= call_tmp i_foo)
            //
            // Should that turn into (= call_tmp bar)?
          } else if (parent && parent->isPrimitive(PRIM_ASSIGN) && parent->get(1) == se) {
            // for_defs should handle this case
          } else if (parent && parent->isResolved()) {
            stillAlive = true;
            // TODO -- a reference argument can be passed directly
          } else {
            stillAlive = true;
          }
        }
        for_defs(se, defMap, var) {
          CallExpr* parent = toCallExpr(se->parentExpr);
          SET_LINENO(se);
          if (parent == move)
            continue;
          if (parent && isMoveOrAssign(parent)) {
            SymExpr* se = toSymExpr(rhs->get(1)->copy());
            INT_ASSERT(se);
            parent->get(1)->replace(se);
            ++s_ref_repl_count;
            addDef(defMap, se);
          } else
            stillAlive = true;
        }
        if (!stillAlive) {
          var->defPoint->remove();
          Vec<SymExpr*>* defs = defMap.get(var);
          if (defs == NULL) {
            INT_FATAL(var, "Expected var to be defined");
          }
          // Remove the first definition from the AST.
          defs->v[0]->getStmtExpr()->remove();
        }
      } else if (rhs->isPrimitive(PRIM_GET_MEMBER) ||
                 rhs->isPrimitive(PRIM_GET_SVEC_MEMBER)) {
        bool stillAlive = false;
        bool isSvec = rhs->isPrimitive(PRIM_GET_SVEC_MEMBER);
        for_uses(se, useMap, var) {
          CallExpr* parent = toCallExpr(se->parentExpr);
          SET_LINENO(se);
          if (parent && (parent->isPrimitive(PRIM_DEREF) || isDerefMove(parent))) {
            Expr* toReplace = parent;
            if (isMoveOrAssign(parent)) {
              toReplace = parent->get(2);
            }
            SymExpr* se = toSymExpr(rhs->get(1)->copy());
            INT_ASSERT(se);
            if (!isSvec)
              toReplace->replace(new CallExpr(PRIM_GET_MEMBER_VALUE,
                                           se,
                                           rhs->get(2)->copy()));
            else
              toReplace->replace(new CallExpr(PRIM_GET_SVEC_MEMBER_VALUE,
                                           se,
                                           rhs->get(2)->copy()));
            ++s_ref_repl_count;
            addUse(useMap, se);
          }
          else if (parent && isMoveOrAssign(parent) && parent->get(2) == se) {
            CallExpr* rhsCopy = rhs->copy();
            parent->get(2)->replace(rhsCopy);
            ++s_ref_repl_count;
            SymExpr* se = toSymExpr(rhsCopy->get(1));
            INT_ASSERT(se);
            addUse(useMap, se);
          } else
            stillAlive = true;
        }
        for_defs(se, defMap, var) {
          CallExpr* parent = toCallExpr(se->parentExpr);
          SET_LINENO(se);
          if (parent == move)
            continue;
          if (parent && isMoveOrAssign(parent)) {
            if (SymExpr* rtmp = toSymExpr(parent->get(2))) {
              SymExpr* se = toSymExpr(rhs->get(1)->copy());
              INT_ASSERT(se);
              if (!isSvec)
                parent->replace(new CallExpr(PRIM_SET_MEMBER,
                                             se,
                                             rhs->get(2)->copy(),
                                             rtmp->remove()));
              else
                parent->replace(new CallExpr(PRIM_SET_SVEC_MEMBER,
                                             se,
                                             rhs->get(2)->copy(),
                                             rtmp->remove()));
              ++s_ref_repl_count;
              addUse(useMap, se);
            } else {
              QualifiedType qt = parent->get(2)->qualType();
              VarSymbol* tmp = newTemp(qt);
              parent->getStmtExpr()->insertBefore(new DefExpr(tmp));
              parent->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, parent->get(2)->remove()));
              SymExpr* se = toSymExpr(rhs->get(1)->copy());
              INT_ASSERT(se);
              if (!isSvec)
                parent->replace(new CallExpr(PRIM_SET_MEMBER,
                                             se,
                                             rhs->get(2)->copy(),
                                             tmp));
              else
                parent->replace(new CallExpr(PRIM_SET_MEMBER,
                                             se,
                                             rhs->get(2)->copy(),
                                             tmp));
              ++s_ref_repl_count;
              addUse(useMap, se);
            }
          } else
            stillAlive = true;
        }
        if (!stillAlive) {
          var->defPoint->remove();
          defMap.get(var)->v[0]->getStmtExpr()->remove();
        }
      }
    } else if (SymExpr* rhs = toSymExpr(move->get(2))) {
      SymExpr* LHS = toSymExpr(move->get(1));
      INT_ASSERT(LHS->symbol() == var);
      for_uses(se, useMap, var) {
        SET_LINENO(se);
        if (se != LHS && se->parentExpr) {
          SymExpr* rcp = rhs->copy();
          se->replace(rcp);
          ++s_ref_repl_count;
          addUse(useMap, rcp);
        }
      }
      for_defs(se, defMap, var) {
        SET_LINENO(se);
        if (se != LHS && se->parentExpr) {
          SymExpr* rcp = rhs->copy();
          se->replace(rcp);
          ++s_ref_repl_count;
          addDef(defMap, rcp);
        }
      }
    }
  }
}


size_t singleAssignmentRefPropagation(FnSymbol* fn) {
  std::vector<BaseAST*> asts;
  collect_asts(fn, asts);

  Vec<Symbol*> refSet;
  Vec<Symbol*> refVec;
  // Walk the asts in this function, and build lists of reference variables and sym exprs.
  for_vector(BaseAST, ast, asts) {
    if (VarSymbol* var = toVarSymbol(ast)) {
      if (var->isRef()) {
        refVec.add(var);
        refSet.set_add(var);
      }
    }
  }

  // Build def/use maps across all symexprs in the function, 
  // but restricted to only the ref variables therein.
  Map<Symbol*,Vec<SymExpr*>*> defMap;
  Map<Symbol*,Vec<SymExpr*>*> useMap;
  buildDefUseMaps(refSet, defMap, useMap);

  s_ref_repl_count = 0;
  // Walk the list of reference vars
  forv_Vec(Symbol, var, refVec) {
    if (var) {
      // Get the move that defines this reference
      if (CallExpr* move = findRefDef(defMap, var)) {
        if (SymExpr* rhs = toSymExpr(move->get(2))) {
          // If it is defined from another reference, these two are mutual aliases.
          if (rhs->isRef()) {
            // Replace each use of the new name with the old name.
            for_uses(se, useMap, var) {
              if (se->parentExpr && se->parentExpr != move) {
                SET_LINENO(se);
                SymExpr* rhsCopy = rhs->copy();
                se->replace(rhsCopy);
                ++s_ref_repl_count;
                addUse(useMap, rhsCopy);
                if (CallExpr* parent = toCallExpr(rhsCopy->parentExpr)) {
                  if (isMoveOrAssign(parent) && parent->get(1) == rhsCopy) {
                    addDef(defMap, rhsCopy);
                  }
                }
              }
            }
            // Other than the original definition, replace
            // definitions of the new name with defs of the old.
            //
            // TODO: 'findRefDef' only returns something if the var is defined
            // once. How is this loop useful?
            for_defs(se, defMap, var) {
              CallExpr* parent = toCallExpr(se->parentExpr);
              if (parent && parent != move) {
                SET_LINENO(se);
                SymExpr* rhsCopy = rhs->copy();
                se->replace(rhsCopy);
                ++s_ref_repl_count;
                addDef(defMap, rhsCopy);
              }
            }
          }
        }
      }
    }
  }

  forv_Vec(Symbol, var, refVec) { // ack! note: order matters
    if (var && !var->type->symbol->hasFlag(FLAG_COPY_MUTATES)) {
      eliminateSingleAssignmentReference(defMap, useMap, var);
    }
  }

  freeDefUseMaps(defMap, useMap);

  return s_ref_repl_count;
}

void refPropagation() {
  if (!fNoCopyPropagation) {
    forv_Vec(FnSymbol, fn, gFnSymbols) {
      singleAssignmentRefPropagation(fn);
      if (!fNoDeadCodeElimination)
        deadVariableElimination(fn);
    }
  }
}

