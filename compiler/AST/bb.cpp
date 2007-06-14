#include "astutil.h"
#include "bb.h"
#include "expr.h"
#include "stmt.h"
#include "view.h"

BasicBlock::BasicBlock(int init_id) : id(init_id) {}

#define BB_START()                              \
  basicBlock = new BasicBlock(id++)

#define BB_ADD(expr)                            \
  basicBlock->exprs.add(expr)

#define BB_ADD_LS(exprls)                       \
  for_alist(Expr, expr, exprls) {               \
    BB_ADD(expr);                               \
  }

#define BB_STOP()                               \
  fn->basicBlocks->add(basicBlock);             \
  basicBlock = NULL

#define BB_RESTART()                            \
  BB_STOP();                                    \
  BB_START()

#define BBB(stmt)                               \
  if (stmt) {                                   \
    buildBasicBlocks(fn, stmt);                 \
  }

#define BB_THREAD(in, out) \
  out->ins.add(in); \
  in->outs.add(out)

void buildBasicBlocks(FnSymbol* fn, Expr* stmt) {
  static BasicBlock* basicBlock;
  static Map<LabelSymbol*,Vec<BasicBlock*>*> gotoMaps;
  static Map<LabelSymbol*,BasicBlock*> labelMaps;
  static int id;
  if (!stmt) {
    if (fn->basicBlocks) {
      forv_Vec(BasicBlock, bb, *fn->basicBlocks) {
        delete bb;
      }
      delete fn->basicBlocks;
    }
    fn->basicBlocks = new Vec<BasicBlock*>();
    gotoMaps.clear();
    labelMaps.clear();
    id = 0;
    BB_START();
    BBB(fn->body);
    BB_STOP();
  } else {
    if (BlockStmt* s = dynamic_cast<BlockStmt*>(stmt)) {
      if (s->loopInfo) {
        BasicBlock* top = basicBlock;
        BB_RESTART();
        BB_ADD(s->loopInfo);
        BasicBlock* loopTop = basicBlock;
        for_alist(Expr, stmt, s->body) {
          BBB(stmt);
        }
        BasicBlock* loopBottom = basicBlock;
        BB_RESTART();
        BasicBlock* bottom = basicBlock;
        BB_THREAD(top, loopTop);
        BB_THREAD(loopBottom, bottom);
        BB_THREAD(loopBottom, loopTop);
        BB_THREAD(top, bottom);
      } else {
        for_alist(Expr, stmt, s->body) {
          BBB(stmt);
        }
      }
    } else if (CondStmt* s = dynamic_cast<CondStmt*>(stmt)) {
      BB_ADD(s->condExpr);
      BasicBlock* top = basicBlock;
      BB_RESTART();
      BasicBlock* thenTop = basicBlock;
      BBB(s->thenStmt);
      BasicBlock* thenBottom = basicBlock;
      BB_RESTART();
      BasicBlock* elseTop = basicBlock;
      BBB(s->elseStmt);
      BasicBlock* elseBottom = basicBlock;
      BB_RESTART();
      BasicBlock* bottom = basicBlock;
      BB_THREAD(top, thenTop);
      BB_THREAD(top, elseTop);
      BB_THREAD(thenBottom, bottom);
      BB_THREAD(elseBottom, bottom);
    } else if (GotoStmt* s = dynamic_cast<GotoStmt*>(stmt)) {
      LabelSymbol* label = dynamic_cast<LabelSymbol*>(s->label);
      if (BasicBlock* bb = labelMaps.get(label)) {
        BB_THREAD(basicBlock, bb);
      } else {
        Vec<BasicBlock*>* vbb = gotoMaps.get(label);
        if (!vbb)
          vbb = new Vec<BasicBlock*>();
        vbb->add(basicBlock);
        gotoMaps.put(label, vbb);
      }
      BB_RESTART();
    } else {
      DefExpr* def = dynamic_cast<DefExpr*>(stmt);
      if (def && dynamic_cast<LabelSymbol*>(def->sym)) {
        BasicBlock* top = basicBlock;
        BB_RESTART();
        BasicBlock* bottom = basicBlock;
        BB_THREAD(top, bottom);
        LabelSymbol* label = dynamic_cast<LabelSymbol*>(def->sym);
        if (Vec<BasicBlock*>* vbb = gotoMaps.get(label)) {
          forv_Vec(BasicBlock, basicBlock, *vbb) {
            BB_THREAD(basicBlock, bottom);
          }
        }
        labelMaps.put(label, bottom);
      } else
        BB_ADD(stmt);
    }
  }
}

void buildLocalsVectorMap(FnSymbol* fn,
                          Vec<Symbol*>& locals,
                          Map<Symbol*,int>& localMap) {
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
}

void buildDefsVectorMap(FnSymbol* fn,
                        Vec<Symbol*>& locals,
                        Vec<SymExpr*>& defs,
                        Map<SymExpr*,int>& defMap) {
  int i = 0;
  forv_Vec(Symbol, local, locals) {
    forv_Vec(SymExpr, se, local->defs) {
      defs.add(se);
      defMap.put(se, i++);
    }
  }
}

void buildDefUseSets(FnSymbol* fn,
                     Vec<Symbol*>& locals,
                     Vec<SymExpr*>& useSet,
                     Vec<SymExpr*>& defSet) {
  compute_sym_uses(fn);
  forv_Vec(Symbol, local, locals) {
    forv_Vec(SymExpr, se, local->defs) {
      defSet.set_add(se);
    } 
    forv_Vec(SymExpr, se, local->uses) {
      useSet.set_add(se);
    }
  }
}

//#define DEBUG_FLOW

void backwardFlowAnalysis(FnSymbol* fn,
                          Vec<Vec<bool>*>& GEN,
                          Vec<Vec<bool>*>& KILL,
                          Vec<Vec<bool>*>& IN,
                          Vec<Vec<bool>*>& OUT) {
  bool iterate = true;
  while (iterate) {
    iterate = false;
    int i = 0;
    forv_Vec(BasicBlock, bb, *fn->basicBlocks) {
      for (int j = 0; j < IN.v[i]->n; j++) {
        bool new_in = (OUT.v[i]->v[j] & !KILL.v[i]->v[j]) | GEN.v[i]->v[j];
        if (new_in != IN.v[i]->v[j]) {
          IN.v[i]->v[j] = new_in;
          iterate = true;
        }
        bool new_out = false;
        forv_Vec(BasicBlock, outbb, bb->outs) {
          new_out = new_out | IN.v[outbb->id]->v[j];
        }
        if (new_out != OUT.v[i]->v[j]) {
          OUT.v[i]->v[j] = new_out;
          iterate = true;
        }
      }
      i++;
    }
#ifdef DEBUG_FLOW
    printf("IN\n"); printBitVectorSets(IN);
    printf("OUT\n"); printBitVectorSets(OUT);
#endif
  }
}


//
// compute OUT set for ith basic block for forward flow analysis
//
static bool
forwardComputeOUT(FnSymbol* fn,
                  Vec<Vec<bool>*>& GEN,
                  Vec<Vec<bool>*>& KILL,
                  Vec<Vec<bool>*>& IN,
                  Vec<Vec<bool>*>& OUT,
                  bool intersection,
                  int i) {
  bool iterate = false;
  for (int j = 0; j < IN.v[i]->n; j++) {
    bool new_out = (IN.v[i]->v[j] & !KILL.v[i]->v[j]) | GEN.v[i]->v[j];
    if (new_out != OUT.v[i]->v[j]) {
      OUT.v[i]->v[j] = new_out;
      iterate = true;
    }
  }
  return iterate;
}


void forwardFlowAnalysis(FnSymbol* fn,
                         Vec<Vec<bool>*>& GEN,
                         Vec<Vec<bool>*>& KILL,
                         Vec<Vec<bool>*>& IN,
                         Vec<Vec<bool>*>& OUT,
                         bool intersection) {
  forwardComputeOUT(fn, GEN, KILL, IN, OUT, intersection, 0);
  bool iterate = true;
  while (iterate) {
    iterate = false;
#ifdef DEBUG_FLOW
    printf("IN\n"); debug_flow_print_set(IN);
    printf("OUT\n"); debug_flow_print_set(OUT);
#endif
    for (int i = 1; i < fn->basicBlocks->n; i++) {
      iterate |= forwardComputeOUT(fn, GEN, KILL, IN, OUT, intersection, i);
    }
    for (int i = 1; i < fn->basicBlocks->n; i++) {
      BasicBlock* bb = fn->basicBlocks->v[i];
      if (intersection) {
        for (int j = 0; j < IN.v[i]->n; j++) {
          bool new_in = true;
          forv_Vec(BasicBlock, inbb, bb->ins) {
            new_in = new_in & OUT.v[inbb->id]->v[j];
          }
          if (new_in != IN.v[i]->v[j]) {
            IN.v[i]->v[j] = new_in;
            iterate = true;
          }
        }
      } else {
        for (int j = 0; j < IN.v[i]->n; j++) {
          bool new_in = false;
          forv_Vec(BasicBlock, inbb, bb->ins) {
            new_in = new_in | OUT.v[inbb->id]->v[j];
          }
          if (new_in != IN.v[i]->v[j]) {
            IN.v[i]->v[j] = new_in;
            iterate = true;
          }
        }
      }
    }
  }
}


void printBasicBlocks(FnSymbol* fn) {
  forv_Vec(BasicBlock, b, *fn->basicBlocks) {
    printf("%2d:  ", b->id);
    forv_Vec(BasicBlock, bb, b->ins) {
      printf("%d ", bb->id);
    }
    printf(" >  ");
    forv_Vec(BasicBlock, bb, b->outs) {
      printf("%d ", bb->id);
    }
    printf("\n");
    forv_Vec(Expr, expr, b->exprs) {
      list_view_noline(expr);
    }
    printf("\n");
  }
}

void printLocalsVector(Vec<Symbol*> locals, Map<Symbol*,int>& localMap) {
  printf("Local Variables\n");
  forv_Vec(Symbol, local, locals) {
    printf("%2d: %s[%d]\n", localMap.get(local), local->name, local->id);
  }
  printf("\n");
}

void printDefsVector(Vec<SymExpr*> defs, Map<SymExpr*,int>& defMap) {
  printf("Variable Definitions\n");
  forv_Vec(SymExpr, def, defs) {
    printf("%2d: %s[%d] in %d\n", defMap.get(def), def->var->name,
           def->var->id, def->getStmtExpr()->id);
  }
  printf("\n");
}

void printLocalsVectorSets(Vec<Vec<bool>*>& sets, Vec<Symbol*> locals) {
  int i = 0;
  forv_Vec(Vec<bool>, set, sets) {
    printf("%2d: ", i);
    for (int j = 0; j < set->n; j++) {
      if (set->v[j])
        printf("%s[%d] ", locals.v[j]->name, locals.v[j]->id);
    }
    printf("\n");
    i++;
  }
  printf("\n");
}

void printBitVectorSets(Vec<Vec<bool>*>& sets) {
  int i = 0;
  forv_Vec(Vec<bool>, set, sets) {
    printf("%2d: ", i);
    for (int j = 0; j < set->n; j++) {
      printf("%d", (set->v[j]) ? 1 : 0);
      if ((j+1) % 10 == 0) printf(" ");
    }
    printf("\n");
    i++;
  }
  printf("\n");
}
