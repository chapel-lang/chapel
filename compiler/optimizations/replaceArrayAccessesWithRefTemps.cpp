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

#include "optimizations.h"

#include "astutil.h"
#include "driver.h"
#include "expr.h"
#include "ForLoop.h"
#include "passes.h"
#include "stlUtil.h"
#include "stmt.h"

#define DEBUG_RAAWRT 0

static bool anyAssignmentsToArray(std::vector<ContextCallExpr*> allContextCalls,
                                  SymExpr* matchArray);
static bool isWrite(SymExpr* lhs);

// Return true if lhs is a write or any aliases of lhs are writes
static bool isWrite(SymExpr* lhs) {
  for_SymbolDefs(def, lhs->symbol()) {
    if (CallExpr* call = toCallExpr(def->parentExpr)) {
      if (!call->isPrimitive(PRIM_MOVE)) {
        return true;
      }
    }
  }
  for_SymbolUses(use, lhs->symbol()) {
    if (CallExpr* call = toCallExpr(use->parentExpr)) {
      if (call->isPrimitive(PRIM_MOVE)) {
        if (use == call->get(2)) {
          SymExpr* lhs = toSymExpr(call->get(1));
          if (isWrite(lhs)) {
            return true;
          }
        }
      }
    }
  }
  return false;
}

// Return true if any of the context calls in the allContextCalls vector
// are used to write to the array 'matchArray'.
static bool anyAssignmentsToArray(std::vector<ContextCallExpr*> allContextCalls,
                                  SymExpr* matchArray) {
  for_vector(ContextCallExpr, contextCall, allContextCalls) {
    CallExpr* call = toCallExpr(contextCall);
    SymExpr* callArray = toSymExpr(call->get(1));
    if (callArray->symbol() == matchArray->symbol()) {
      // figure out if this is a write
      if (CallExpr* parentCall = toCallExpr(contextCall->parentExpr)) {
        if (parentCall->isPrimitive(PRIM_MOVE)) {
          SymExpr* lhs = toSymExpr(parentCall->get(1));
          if (isWrite(lhs)) {
            return true;
          }
        }
      }
    }
  }
  return false;
}

// Translate multiple array accesses using an index variable in a loop into
// a single array access stored into a 'ref' temporary, then reuse the temp.
// The array access ('this') function can be expensive, so computing it only
// once and reusing the result can save time.
//
// TODO: I think this only works with serial 'for' loops right now.  Extend
//       it to apply to forall loops.
// TODO: See if the "removable array access" pragma can go away.
// TODO: Make sure this is OK to do with all array types and disable it for
//       any types where it is not OK.
// TODO: This optimization currently needs to run before the lowerIterators
//       pass.  It would be good to fix it up to allow it to run later,
//       as most other optimization passes do.  The challenges are that it
//       would need to handle CForLoops instead of just ForLoops, and it
//       needs to be able to find the index variable of the loop, which is
//       not as obvious with a CForLoop.
//
// For example:
//   var A, B: [1..n] real;
//   for i in 1..n {
//     A[i] = A[i] * A[i];
//     B[i] = A[i] + f(A[i]);
//   }
//
// Becomes:
//   var A, B: [1..n] real;
//   for i in 1..n {
//     ref tmp = A[i];
//     tmp = tmp * tmp;
//     B[i] = tmp + f(tmp);
//   }
//
void replaceArrayAccessesWithRefTemps() {
  if (!fReplaceArrayAccessesWithRefTemps)
    return;

  forv_Vec(BlockStmt, block, gBlockStmts) {
    if (ForLoop* forLoop = toForLoop(block)) {
      std::vector<ContextCallExpr*>                     allContextCalls;
      std::vector<BaseAST*>                             asts;
      std::map<Symbol*, std::vector<ContextCallExpr*> > arrayAccessMap;

      SymExpr*  loopIdx   = forLoop->indexGet();
      CallExpr* indexMove = NULL;
      Symbol*   indexVar  = NULL;

      collect_asts(forLoop, asts);

      for_vector(BaseAST, ast, asts) {
        if (CallExpr* call = toCallExpr(ast)) {
          // find the move that stores the for loop's index variable into
          // the user-level index variable.  A ref var pointing at array
          // elements will be inserted and initialized after this move.
          if (call->isPrimitive(PRIM_MOVE)) {
            if (SymExpr* rhs = toSymExpr(call->get(2))) {
              if (rhs->symbol() == loopIdx->symbol()) {
                if (toSymExpr(call->get(1))->symbol()->hasFlag(FLAG_TEMP)) {
                  loopIdx = toSymExpr(call->get(1));
                } else {
                  assert(indexMove == NULL && indexVar == NULL);
                  indexMove = call;
                  indexVar = toSymExpr(call->get(1))->symbol();
                }
              }
            }
          }
        }
      }

      if (!indexMove) {
        // If we couldn't find an expected index move, skip the optimization
        if (DEBUG_RAAWRT) {
          printf("%s:%d: Couldn't find index move.  "
                 "Not replacing accesses in loop\n",
                 forLoop->fname(), forLoop->linenum());
        }

        continue;
      }

      for_vector(BaseAST, astNode, asts) {
        if (ContextCallExpr* contextCall = toContextCallExpr(astNode)) {
          allContextCalls.push_back(contextCall);

          CallExpr* call = toCallExpr(contextCall);

          if (contextCall->parentSymbol != forLoop->parentSymbol ||
              call->numActuals() != 2) {
            // TODO: Multidimensional not handled yet.
            // TODO: Nested functions not handled yet.
            continue;
          }

          if (FnSymbol* fn = call->resolvedFunction()) {
            if (fn->hasFlag(FLAG_REMOVABLE_ARRAY_ACCESS)) {
              assert(isSymExpr(call->get(1)));

              Symbol* arraySym = toSymExpr(call->get(1))->symbol();

              if (SymExpr* arrayIdx = toSymExpr(call->get(2))) {
                if (arrayIdx->symbol()->defPoint->parentExpr == forLoop &&
                    /*indexVar == arrayIdx->var &&*/
                    arrayIdx->symbol()->hasFlag(FLAG_INDEX_VAR)) {
                  // build map from array symbol to vector of context calls
                  // where the context calls are all of the form:
                  // ContextCallExpr(CallExpr('this', 'array', 'loopIdx'),
                  //                 CallExpr('this', 'array', 'loopIdx'))
                  arrayAccessMap[arraySym].push_back(contextCall);

                  if (DEBUG_RAAWRT) {
                    CallExpr* call  = toCallExpr(contextCall);
                    SymExpr*  array = toSymExpr(call->get(1));
                    SymExpr*  idx   = toSymExpr(call->get(2));

                    printf("%s:%d: found removable array access %s[%s] (%d)\n",
                           contextCall->fname(),
                           contextCall->linenum(),
                           array->symbol()->name,
                           idx->symbol()->name,
                           contextCall->id);
                  }
                }
              }
            }
          }
        }
      }

      for (std::map<Symbol*, std::vector<ContextCallExpr*> >::iterator it = arrayAccessMap.begin(); it != arrayAccessMap.end(); ++it) {
        int              vecSize   = it->second.size();
        ContextCallExpr* firstCall = it->second.front();

        if (vecSize <= 2) {
          if (DEBUG_RAAWRT) {
            CallExpr* call  = toCallExpr(firstCall);
            SymExpr*  array = toSymExpr(call->get(1));
            SymExpr*  idx   = toSymExpr(call->get(2));

            printf("%s:%d: not replacing array access %s[%s] (%d), "
                   "number of accesses %d is under threshold\n",
                   firstCall->fname(),
                   firstCall->linenum(),
                   array->symbol()->name,
                   idx->symbol()->name,
                   firstCall->id, vecSize);
          }

        } else /*if (vecSize > 2) */ { // TODO: tune this threshold
          SET_LINENO(indexMove);

          CallExpr*  accessCall = toCallExpr(firstCall);
          SymExpr*   array      = toSymExpr(accessCall->get(1));

          // assign an array indexing context call in the vector to a 'ref'
          // variable at the top of the loop
          VarSymbol* ref        = newTemp("arrayAccessTmp",
                                          firstCall->typeInfo());

          if (anyAssignmentsToArray(allContextCalls, array)) {
            // If any assignment to the array happens in the loop, mark the
            // temp as a user-level reference var.  This will prevent it
            // from being changed to by-value during cullOverReferences.
            if (DEBUG_RAAWRT) {
              printf("found an assignment to %s, forcing refs\n",
                     array->symbol()->name);
            }

            ref->addFlag(FLAG_REF_VAR);
          }

          indexMove->insertAfter(new CallExpr(PRIM_MOVE,
                                              ref,
                                              firstCall->copy()));

          indexMove->insertAfter(new DefExpr(ref));

          // then replace all of the indexing context calls in the vector
          // with uses of that 'ref'
          for (std::vector<ContextCallExpr*>::iterator calls = it->second.begin();
               calls != it->second.end();
               ++calls) {

            ContextCallExpr* call = *calls;

            if (DEBUG_RAAWRT) {
              CallExpr*   accessCall = toCallExpr(call);
              SymExpr*    array      = toSymExpr(accessCall->get(1));
              SymExpr*    idx        = toSymExpr(accessCall->get(2));
              const char* sayref     = NULL;

              sayref = ref->hasFlag(FLAG_REF_VAR) ? " ref " : " ";

              printf("%s:%d: replacing array access %s[%s] (%d)"
                     " with%stemp\n",
                     call->fname(),
                     call->linenum(),
                     array->symbol()->name,
                     idx->symbol()->name,
                     call->id,
                     sayref);
            }

            call->replace(new SymExpr(ref));
          }
        }
      }
    }
  }
}

