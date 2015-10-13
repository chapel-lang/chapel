/*
 * Copyright 2004-2015 Cray Inc.
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
#include "bb.h"
#include "bitVec.h"
#include "expr.h"
#include "stlUtil.h"
#include "stmt.h"

//#define DEBUG_LIVE


//
// Given a function fn, computes:
//
//   locals: a vector of local variables
//   localMap: a map of local variables to indices
//   useSet: a set of SymExprs that are uses of local variables
//   defSet: a set of SymExprs that are defs of local variables
//   OUT: set of live variables at exit of each basic block
//
// if OUT(i)(j) is true then the jth local variable is live at the
// exit of basic block i
//
#if HILDE_MM
static void
addUsesForNamedLocals(Vec<Symbol*>& locals, BasicBlock* bb, BitVec* use)
{
  if (bb->exprs.size() > 0)
  {
    Expr* last = bb->exprs.back();
    for (int j = 0; j < locals.n; ++j)
    {
      Symbol* sym = locals.v[j];

      // Skip unnamed temporaries.
      if (sym->hasFlag(FLAG_TEMP))
        continue;

      if (BlockStmt* scope = toBlockStmt(sym->defPoint->parentExpr))
        if (scope->contains(last))
          use->set(j);
    }
  }
}


void
liveVariableAnalysis(FnSymbol* fn,
                     Vec<Symbol*>& locals,
                     Map<Symbol*,int>& localMap,
                     Vec<SymExpr*>& useSet,
                     Vec<SymExpr*>& defSet,
                     std::vector<BitVec*>& OUT)
{
  BasicBlock::buildLocalsVectorMap(fn, locals, localMap);

#ifdef DEBUG_LIVE
  BasicBlock::printLocalsVector(locals, localMap);
#endif

  // This is only done because the useSet is used in the special case for
  // variables in the initializer clause of a C-style for loop.
  buildDefUseSets(locals, fn, defSet, useSet);

  // Under AMM, a symbol is potentially deleted at the end of its scope.
  // Therefore, it is live to the end of its scope, regardless whether its last
  // use comes somewhere before that.
  // As a heuristic, we assume that only named variables have this
  // property.  That is, an unnamed temporary is effectively dead after its
  // last use.

  std::vector<BitVec*> USE;
  std::vector<BitVec*> DEF;
  std::vector<BitVec*> IN;
  for_vector(BasicBlock, bb, *fn->basicBlocks)
  {
    BitVec* use = new BitVec(locals.n);
    BitVec* def = new BitVec(locals.n);
    BitVec* lvin = new BitVec(locals.n);
    BitVec* lvout = new BitVec(locals.n);

    for_vector(Expr, expr, bb->exprs)
    {
      std::vector<BaseAST*> asts;
      collect_asts(expr, asts);

      // Compute the use set for this block: Every symbol that is used in this
      // block.
      for_vector(BaseAST, ast, asts)
      {
        if (SymExpr* se = toSymExpr(ast))
        {
          if (useSet.set_in(se))
          {
            int id = localMap.get(se->var);
            if (!def->get(id)) // TODO: Can this condition be removed?
              use->set(id);
          }
        }
      }

      // Compute the def set for this block: Every symbol the is defined in
      // this block.
      for_vector(BaseAST, ast1, asts)
      {
        if (SymExpr* se = toSymExpr(ast1))
        {
          if (defSet.set_in(se))
          {
            int id = localMap.get(se->var);
            if (!use->get(id)) // TODO: Can this condition be removed?
              def->set(id);
          }
        }
      }
    }

    addUsesForNamedLocals(locals, bb, use);

    USE.push_back(use);
    DEF.push_back(def);
    IN.push_back(lvin);
    OUT.push_back(lvout);
  }

  BasicBlock::backwardFlowAnalysis(fn, USE, DEF, IN, OUT);

  for_vector(BitVec, use, USE)
    delete use, use = 0;

  for_vector(BitVec, def, DEF)
    delete def, def = 0;

  for_vector(BitVec, in, IN)
    delete in, in = 0;
}
#else
void
liveVariableAnalysis(FnSymbol* fn,
                     Vec<Symbol*>& locals,
                     Map<Symbol*,int>& localMap,
                     Vec<SymExpr*>& useSet,
                     Vec<SymExpr*>& defSet,
                     std::vector<BitVec*>& OUT) {
  BasicBlock::buildLocalsVectorMap(fn, locals, localMap);

#ifdef DEBUG_LIVE
  BasicBlock::printLocalsVector(locals, localMap);
#endif

  buildDefUseSets(locals, fn, defSet, useSet);

  //
  // USE(i): the set of variables that are used in basic block i
  // before they are defined (if at all) in the block
  // DEF(i): the set of variables that are defined in basic block i
  // before they are used (if at all) in the block
  // IN(i): the set of variables that are live at entry to basic
  // block i
  //
  std::vector<BitVec*> USE;
  std::vector<BitVec*> DEF;
  std::vector<BitVec*> IN;

  for_vector(BasicBlock, bb, *fn->basicBlocks) {
    BitVec* use = new BitVec(locals.n);
    BitVec* def = new BitVec(locals.n);
    BitVec* lvin = new BitVec(locals.n);
    BitVec* lvout = new BitVec(locals.n);
    for_vector(Expr, expr, bb->exprs) {
      std::vector<BaseAST*> asts;
      collect_asts(expr, asts);
      for_vector(BaseAST, ast, asts) {
        if (SymExpr* se = toSymExpr(ast)) {
          if (useSet.set_in(se)) {
            int id = localMap.get(se->var);
            if (!def->get(id))
              use->set(id);
          }
        }
      }
      for_vector(BaseAST, ast1, asts) {
        if (SymExpr* se = toSymExpr(ast1)) {
          if (defSet.set_in(se)) {
            int id = localMap.get(se->var);
            if (!use->get(id))
              def->set(id);
          }
        }
      }
    }
    USE.push_back(use);
    DEF.push_back(def);
    IN.push_back(lvin);
    OUT.push_back(lvout);
  }

#ifdef DEBUG_LIVE
  printf("DEF\n"); BasicBlock::printLocalsVectorSets(DEF, locals);
  printf("USE\n"); BasicBlock::printLocalsVectorSets(USE, locals);
#endif

  BasicBlock::backwardFlowAnalysis(fn, USE, DEF, IN, OUT);

  for_vector(BitVec, use, USE)
    delete use, use = 0;

  for_vector(BitVec, def, DEF)
    delete def, def = 0;

  for_vector(BitVec, in, IN)
    delete in, in = 0;
}
#endif

// TODO: liveVariableAnalysis is used only in iterator.cpp, and the
// backwardFlowAnalysis in bb.cpp is only used here.  Both should be moved to
// iterator.cpp.

