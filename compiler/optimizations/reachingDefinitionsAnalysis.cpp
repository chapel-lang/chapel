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

#include <vector>

#include "optimizations.h"

#include "astutil.h"
#include "bb.h"
#include "bitVec.h"
#include "expr.h"
#include "stlUtil.h"
#include "stmt.h"
#include "view.h"

static void
reachingDefinitionsAnalysis(FnSymbol* fn,
                            Vec<SymExpr*>& defs,
                            Map<SymExpr*,int>& defMap,
                            Vec<SymExpr*>& useSet,
                            Vec<SymExpr*>& defSet,
                            std::vector<BitVec*>& IN) {
  Vec<Symbol*> locals;
  Map<Symbol*,int> localMap;

  BasicBlock::buildLocalsVectorMap(fn, locals, localMap);

  buildDefUseSets(locals, fn, defSet, useSet);

  //
  // compute defs and defMap in an order such that defs of the same
  // variable are adjacent (this is important!); use localDefsMap to
  // make this linear; localDefsMap is like the defMap computed by
  // buildDefUseMaps, but is computed from the defSet in this more
  // efficient manner
  //
  std::vector<int> localDefs(locals.n);
  forv_Vec(SymExpr, se, defSet) {
    if (se) {
      localDefs[localMap.get(se->var)]++;
      defs.add(NULL);
    }
  }
  int sum = 0;
  for (int i = 0; i < locals.n; i++) {
    int nextSum = localDefs[i];
    localDefs[i] = sum;
    sum += nextSum;
  }
  forv_Vec(SymExpr, se, defSet) {
    if (se) {
      int i = localDefs[localMap.get(se->var)]++;
      defs.v[i] = se;
      defMap.put(se, i);
    }
  }

  std::vector<BitVec*> KILL;
  std::vector<BitVec*> GEN;
  std::vector<BitVec*> OUT;

  for_vector(BasicBlock, bb, *fn->basicBlocks) {
    Vec<Symbol*> bbDefSet;
    BitVec* kill = new BitVec(defs.n);
    BitVec* gen = new BitVec(defs.n);
    BitVec* in = new BitVec(defs.n);
    BitVec* out = new BitVec(defs.n);
    for (int i = bb->exprs.size()-1; i >= 0; i--) {
      Expr* expr = bb->exprs[i];
      Vec<SymExpr*> symExprs;
      collectSymExprs(expr, symExprs);
      forv_Vec(SymExpr, se, symExprs) {
        if (defSet.set_in(se)) {
          if (!bbDefSet.set_in(se->var)) {
            gen->set(defMap.get(se));
          }
          bbDefSet.set_add(se->var);
        }
      }
    }
    for (int i = 0; i < defs.n; i++) {
      if (bbDefSet.set_in(defs.v[i]->var))
        kill->set(i);
    }
    KILL.push_back(kill);
    GEN.push_back(gen);
    IN.push_back(in);
    OUT.push_back(out);
  }

#ifdef DEBUG_REACHING
  list_view(fn);
  BasicBlock::printBasicBlocks(fn);
  BasicBlock::printDefsVector(defs, defMap);
  printf("KILL:\n"); BasicBlock::printBitVectorSets(KILL);
  printf("GEN:\n");  BasicBlock::printBitVectorSets(GEN);
  printf("IN:\n");   BasicBlock::printBitVectorSets(IN);
  printf("OUT:\n");  BasicBlock::printBitVectorSets(OUT);
#endif

  BasicBlock::forwardFlowAnalysis(fn, GEN, KILL, IN, OUT, false);

#ifdef DEBUG_REACHING
  printf("IN:\n");  BasicBlock::printBitVectorSets(IN);
  printf("OUT:\n"); BasicBlock::printBitVectorSets(OUT);
#endif

  for_vector(BitVec, gen, GEN)
    delete gen, gen = 0;

  for_vector(BitVec, kill, KILL)
    delete kill, kill = 0;

  for_vector(BitVec, out, OUT)
    delete out, out = 0;
}


void
buildDefUseChains(FnSymbol* fn,
                  std::map<SymExpr*,Vec<SymExpr*>*>& DU,
                  std::map<SymExpr*,Vec<SymExpr*>*>& UD) {
  Vec<SymExpr*> defs;
  Map<SymExpr*,int> defMap;
  Vec<SymExpr*> useSet;
  Vec<SymExpr*> defSet;
  std::vector<BitVec*> IN;
  reachingDefinitionsAnalysis(fn, defs, defMap, useSet, defSet, IN);

  //
  // map from symbol to index into defs vector where defs of symbol
  // begin
  //
  Map<Symbol*,int> defsIndexMap;
  Symbol* last = 0;
  int i = 0;
  forv_Vec(SymExpr, se, defs) {
    if (se->var != last)
      defsIndexMap.put(se->var, i);
    last = se->var;
    i++;
  }

  forv_Vec(SymExpr, def, defs) {
    DU[def] = new Vec<SymExpr*>();
  }

  for (size_t i = 0; i < fn->basicBlocks->size(); i++) {
    BasicBlock* bb = (*fn->basicBlocks)[i];
    BitVec* in = IN[i];
    for_vector(Expr, expr, bb->exprs) {
      Vec<SymExpr*> symExprs;
      collectSymExprs(expr, symExprs);
      forv_Vec(SymExpr, se, symExprs) {
        if (useSet.set_in(se)) {
          UD[se] = new Vec<SymExpr*>();
          for (int j = defsIndexMap.get(se->var); j < defs.n; j++) {
            if (defs.v[j]->var != se->var)
              break;
            if (in->get(j)) {
              DU[defs.v[j]]->add(se);
              UD[se]->add(defs.v[j]);
            }
          }
        }
      }
      forv_Vec(SymExpr, se, symExprs) {
        if (defSet.set_in(se)) {
          for (int j = defsIndexMap.get(se->var); j < defs.n; j++) {
            if (defs.v[j]->var != se->var)
              break;
            if (defs.v[j] == se)
              in->set(j);
            else
              in->unset(j);
          }
        }
      }
    }
  }

  for_vector(BitVec, in, IN)
    delete in, in = 0;
}

typedef MapElem<SymExpr*,Vec<SymExpr*>*> SymExprToVecSymExprMapElem;

void
freeDefUseChains(std::map<SymExpr*,Vec<SymExpr*>*>& DU,
                 std::map<SymExpr*,Vec<SymExpr*>*>& UD) {
  std::map<SymExpr*,Vec<SymExpr*>*>::iterator vec;
  for(vec = DU.begin(); vec != DU.end(); vec++)
    delete vec->second;
  for(vec = UD.begin(); vec != UD.end(); vec++)
    delete vec->second;
}
