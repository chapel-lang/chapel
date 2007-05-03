#include "astutil.h"
#include "bb.h"
#include "expr.h"
#include "optimizations.h"
#include "stmt.h"

//#define DEBUG_LIVE


#ifdef DEBUG_LIVE
static void
debug_df_print_set(Vec<Vec<bool>*>& sets, Vec<Symbol*> locals) {
  int i = 0;
  forv_Vec(Vec<bool>, set, sets) {
    printf("%d: ", i);
    for (int j = 0; j < set->n; j++) {
      if (set->v[j])
        printf("%s[%d] ", locals.v[j]->name, locals.v[j]->id);
    }
    printf("\n");
    i++;
  }
  printf("\n");
}
#endif


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
void
liveVariableAnalysis(FnSymbol* fn,
                     Vec<Symbol*>& locals,
                     Map<Symbol*,int>& localMap,
                     Vec<SymExpr*>& useSet,
                     Vec<SymExpr*>& defSet,
                     Vec<Vec<bool>*>& OUT) {
  int i = 0;
  forv_Vec(BasicBlock, bb, *fn->basicBlocks) {
    forv_Vec(Expr, expr, bb->exprs) {
      if (DefExpr* def = dynamic_cast<DefExpr*>(expr)) {
        if (dynamic_cast<VarSymbol*>(def->sym)) {
          locals.add(def->sym);
          localMap.put(def->sym, i++);
        }
      }
    }
  }

#ifdef DEBUG_LIVE
  printf("Local Variables\n");
  forv_Vec(Symbol, local, locals) {
    printf("%d: %s[%d]\n", localMap.get(local), local->name, local->id);
  }
  printf("\n");
#endif

  compute_sym_uses(fn);
  forv_Vec(Symbol, local, locals) {
    forv_Vec(SymExpr, se, local->defs) {
      defSet.set_add(se);
    } 
    forv_Vec(SymExpr, se, local->uses) {
      useSet.set_add(se);
    }
  }

  //
  // USE(i): the set of variables that are used in basic block i
  // before they are defined (if at all) in the block
  // DEF(i): the set of variables that are defined in basic block i
  // before they are used (if at all) in the block
  // IN(i): the set of variables that are live at entry to basic
  // block i
  //
  Vec<Vec<bool>*> USE;
  Vec<Vec<bool>*> DEF;
  Vec<Vec<bool>*> IN;

  forv_Vec(BasicBlock, bb, *fn->basicBlocks) {
    Vec<bool>* use = new Vec<bool>();
    Vec<bool>* def = new Vec<bool>();
    Vec<bool>* lvin = new Vec<bool>();
    Vec<bool>* lvout = new Vec<bool>();
    forv_Vec(Symbol, local, locals) {
      use->add(false);
      def->add(false);
      lvin->add(false);
      lvout->add(false);
    }
    forv_Vec(Expr, expr, bb->exprs) {
      Vec<BaseAST*> asts;
      collect_asts(&asts, expr);
      forv_Vec(BaseAST, ast, asts) {
        if (SymExpr* se = dynamic_cast<SymExpr*>(ast)) {
          if (useSet.set_in(se)) {
            int id = localMap.get(se->var);
            if (!def->v[id])
              use->v[id] = true;
          }
          if (defSet.set_in(se)) {
            int id = localMap.get(se->var);
            if (!use->v[id])
              def->v[id] = true;
          }
        }
      }
    }
    USE.add(use);
    DEF.add(def);
    IN.add(lvin);
    OUT.add(lvout);
  }

#ifdef DEBUG_LIVE
  printf("DEF\n"); debug_df_print_set(DEF, locals);
  printf("USE\n"); debug_df_print_set(USE, locals);
#endif

  backwardFlowAnalysis(fn, USE, DEF, IN, OUT);

  forv_Vec(Vec<bool>, use, USE)
    delete use;

  forv_Vec(Vec<bool>, def, DEF)
    delete def;

  forv_Vec(Vec<bool>, in, IN)
    delete in;
}
