#include "astutil.h"
#include "bb.h"
#include "expr.h"
#include "optimizations.h"
#include "stmt.h"
#include "view.h"


void
reachingDefinitionsAnalysis(FnSymbol* fn,
                            Vec<SymExpr*>& defs,
                            Map<SymExpr*,int>& defMap,
                            Vec<SymExpr*>& useSet,
                            Vec<SymExpr*>& defSet,
                            Vec<Vec<bool>*>& IN) {
  Vec<Symbol*> locals;
  Map<Symbol*,int> localMap;
  buildLocalsVectorMap(fn, locals, localMap);
  buildDefUseSets(locals, fn, defSet, useSet);

  //
  // compute defs and defMap in an order such that defs of the same
  // variable are adjacent (this is important!); use localDefsMap to
  // make this linear; localDefsMap is like the defMap computed by
  // buildDefUseMaps, but is computed from the defSet in this more
  // efficient manner
  //
  Map<Symbol*,Vec<SymExpr*>*> localDefsMap;
  forv_Vec(Symbol, sym, locals) {
    localDefsMap.put(sym, new Vec<SymExpr*>());
  }
  forv_Vec(SymExpr, se, defSet) {
    if (se)
      localDefsMap.get(se->var)->add(se);
  }
  int i = 0;
  forv_Vec(Symbol, sym, locals) {
    Vec<SymExpr*>* symDefs = localDefsMap.get(sym);
    forv_Vec(SymExpr, se, *symDefs) {
      defs.add(se);
      defMap.put(se, i++);
    }
  }
  forv_Vec(Symbol, sym, locals) {
    delete localDefsMap.get(sym);
  }

  Vec<Vec<bool>*> KILL;
  Vec<Vec<bool>*> GEN;
  Vec<Vec<bool>*> OUT;

  forv_Vec(BasicBlock, bb, *fn->basicBlocks) {
    Vec<Symbol*> bbDefSet;
    Vec<bool>* kill = new Vec<bool>();
    Vec<bool>* gen = new Vec<bool>();
    Vec<bool>* in = new Vec<bool>();
    Vec<bool>* out = new Vec<bool>();
    forv_Vec(SymExpr, def, defs) {
      gen->add(false);
      in->add(false);
      out->add(false);
    }
    for (int i = bb->exprs.n-1; i >= 0; i--) {
      Expr* expr = bb->exprs.v[i];
      Vec<BaseAST*> asts;
      collect_asts(expr, asts);
      forv_Vec(BaseAST, ast, asts) {
        if (SymExpr* se = toSymExpr(ast)) {
          if (defSet.set_in(se)) {
            if (!bbDefSet.set_in(se->var))
              gen->v[defMap.get(se)] = true;
            bbDefSet.set_add(se->var);
          }
        }
      }
    }
    forv_Vec(SymExpr, def, defs) {
      kill->add((bbDefSet.set_in(def->var)) ? true : false);
    }
    KILL.add(kill);
    GEN.add(gen);
    IN.add(in);
    OUT.add(out);
  }

#ifdef DEBUG_REACHING
  list_view(fn);
  printBasicBlocks(fn);
  printDefsVector(defs, defMap);
  printf("KILL:\n"); printBitVectorSets(KILL);
  printf("GEN:\n"); printBitVectorSets(GEN);
  printf("IN:\n"); printBitVectorSets(IN);
  printf("OUT:\n"); printBitVectorSets(OUT);
#endif

  forwardFlowAnalysis(fn, GEN, KILL, IN, OUT, false);

#ifdef DEBUG_REACHING
  printf("IN:\n"); printBitVectorSets(IN);
  printf("OUT:\n"); printBitVectorSets(OUT);
#endif

  forv_Vec(Vec<bool>, gen, GEN)
    delete gen;

  forv_Vec(Vec<bool>, kill, KILL)
    delete kill;

  forv_Vec(Vec<bool>, out, OUT)
    delete out;
}


void
buildDefUseChains(FnSymbol* fn,
                  Map<SymExpr*,Vec<SymExpr*>*>& DU,
                  Map<SymExpr*,Vec<SymExpr*>*>& UD) {
  buildBasicBlocks(fn);

  Vec<SymExpr*> defs;
  Map<SymExpr*,int> defMap;
  Vec<SymExpr*> useSet;
  Vec<SymExpr*> defSet;
  Vec<Vec<bool>*> IN;
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
    DU.put(def, new Vec<SymExpr*>());
  }

  for (int i = 0; i < fn->basicBlocks->n; i++) {
    BasicBlock* bb = fn->basicBlocks->v[i];
    Vec<bool>* in = IN.v[i];
    forv_Vec(Expr, expr, bb->exprs) {
      Vec<BaseAST*> asts;
      collect_asts(expr, asts);
      forv_Vec(BaseAST, ast, asts) {
        if (SymExpr* se = toSymExpr(ast)) {
          if (useSet.set_in(se)) {
            UD.put(se, new Vec<SymExpr*>());
            for (int j = defsIndexMap.get(se->var); j < defs.n; j++) {
              if (defs.v[j]->var != se->var)
                break;
              if (in->v[j]) {
                DU.get(defs.v[j])->add(se);
                UD.get(se)->add(defs.v[j]);
              }
            }
          }
        }
      }
      forv_Vec(BaseAST, ast, asts) {
        if (SymExpr* se = toSymExpr(ast)) {
          if (defSet.set_in(se)) {
            for (int j = defsIndexMap.get(se->var); j < defs.n; j++) {
              if (defs.v[j]->var != se->var)
                break;
              if (defs.v[j] == se)
                in->v[j] = true;
              else
                in->v[j] = false;
            }
          }
        }
      }
    }
  }

  forv_Vec(Vec<bool>, in, IN)
    delete in;
}

void
freeDefUseChains(Map<SymExpr*,Vec<SymExpr*>*>& DU,
                 Map<SymExpr*,Vec<SymExpr*>*>& UD) {
  Vec<SymExpr*> keys;
  DU.get_keys(keys);
  forv_Vec(SymExpr, key, keys) {
    delete DU.get(key);
  }
  keys.clear();
  UD.get_keys(keys);
  forv_Vec(SymExpr, key, keys) {
    delete UD.get(key);
  }
}
