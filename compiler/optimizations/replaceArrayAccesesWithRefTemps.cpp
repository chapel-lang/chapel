#include "optimizations.h"

#include "astutil.h"
#include "expr.h"
#include "passes.h"
#include "stlUtil.h"
#include "stmt.h"
#include "ForLoop.h"

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
      SymExpr* loopIdx = forLoop->indexGet();
      CallExpr* indexMove = NULL;
      Symbol* indexVar = NULL;
      std::map<Symbol*, std::vector<ContextCallExpr*> > arrayAccessMap;
      std::vector<BaseAST*> asts;
      collect_asts(forLoop, asts);
      for_vector(BaseAST, ast, asts) {
        if (CallExpr* call = toCallExpr(ast)) {
          // find the move that stores the for loop's index variable into
          // the user-level index variable.  A ref var pointing at array
          // elements will be inserted and initialized after this move.
          if (call->isPrimitive(PRIM_MOVE)) {
            if (SymExpr* rhs = toSymExpr(call->get(2))) {
              if (rhs->var == loopIdx->var) {
                assert(indexMove == NULL && indexVar == NULL);
                indexMove = call;
                indexVar = toSymExpr(call->get(1))->var;
              }
            }
          }
        }
        if (ContextCallExpr* contextCall = toContextCallExpr(ast)) {
          CallExpr* call = toCallExpr(contextCall);
          if (FnSymbol* fn = call->isResolved()) {
            if (fn->hasFlag(FLAG_REMOVABLE_ARRAY_ACCESS)) {
              if (SymExpr* arrayIdx = toSymExpr(call->get(2))) {
                if (indexVar == arrayIdx->var && arrayIdx->var->hasFlag(FLAG_INDEX_VAR)) {
                  // build map from array symbol to vector of context calls
                  // where the context calls are all of the form:
                  // ContextCallExpr(CallExpr('this', 'array', 'loopIdx'),
                  //                 CallExpr('this', 'array', 'loopIdx'))
                  assert(isSymExpr(call->get(1)));
                  Symbol* arraySym = toSymExpr(call->get(1))->var;
                  arrayAccessMap[arraySym].push_back(contextCall);
                  //printf("Found removable array access: %s: %d\n", call->astloc.filename, call->astloc.lineno);
                }
              }
            }
          }
        }
      }
      for (std::map<Symbol*, std::vector<ContextCallExpr*> >::iterator it = arrayAccessMap.begin(); it != arrayAccessMap.end(); ++it) {
        int vecSize = it->second.size();
        if (vecSize > 2) { // TODO: tune this threshold
          SET_LINENO(indexMove);
          // assign an array indexing context call in the vector to a 'ref'
          // variable at the top of the loop
          VarSymbol* ref = newTemp("arrayAccessRef", it->second.front()->typeInfo());
          ref->addFlag(FLAG_REF_VAR);
          indexMove->insertAfter(new CallExpr(PRIM_MOVE, ref, it->second.front()->copy()));
          indexMove->insertAfter(new DefExpr(ref));

          // then replace all of the indexing context calls in the vector
          // with uses of that 'ref'
          for (std::vector<ContextCallExpr*>::iterator calls = it->second.begin(); calls != it->second.end(); ++calls) {
            ContextCallExpr* call = *calls;
            call->replace(new SymExpr(ref));
          }
        }
      }
    }
  }
}

