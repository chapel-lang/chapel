#include <stdlib.h>
#include "astutil.h"
#include "bb.h"
#include "bitVec.h"
#include "expr.h"
#include "stmt.h"
#include "view.h"

BasicBlock::BasicBlock(int init_id) : id(init_id) {}

#define BB_START()                              \
  basicBlock = new BasicBlock(id++)

#define BB_ADD(expr)                            \
  basicBlock->exprs.add(expr)

#define BB_ADD_LS(exprls)                       \
  for_alist(expr, exprls) {               \
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
    if (BlockStmt* s = toBlockStmt(stmt)) {
      if (s->loopInfo) {
        BasicBlock* top = basicBlock;
        BB_RESTART();
        BB_ADD(s->loopInfo);
        BasicBlock* loopTop = basicBlock;
        for_alist(stmt, s->body) {
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
        for_alist(stmt, s->body) {
          BBB(stmt);
        }
      }
    } else if (CondStmt* s = toCondStmt(stmt)) {
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
    } else if (GotoStmt* s = toGotoStmt(stmt)) {
      LabelSymbol* label = toLabelSymbol(s->label->var);
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
      DefExpr* def = toDefExpr(stmt);
      if (def && toLabelSymbol(def->sym)) {
        BasicBlock* top = basicBlock;
        BB_RESTART();
        BasicBlock* bottom = basicBlock;
        BB_THREAD(top, bottom);
        LabelSymbol* label = toLabelSymbol(def->sym);
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
      if (DefExpr* def = toDefExpr(expr)) {
        if (toVarSymbol(def->sym)) {
          locals.add(def->sym);
          localMap.put(def->sym, i++);
        }
      }
    }
  }
}


//#define DEBUG_FLOW
void backwardFlowAnalysis(FnSymbol* fn,
                          Vec<BitVec*>& GEN,
                          Vec<BitVec*>& KILL,
                          Vec<BitVec*>& IN,
                          Vec<BitVec*>& OUT) {
  bool iterate = true;
  while (iterate) {
    iterate = false;
    int i = 0;
    forv_Vec(BasicBlock, bb, *fn->basicBlocks) {
      for (int j = 0; j < IN.v[i]->ndata; j++) {
        unsigned new_in = (OUT.v[i]->data[j] & ~KILL.v[i]->data[j]) | GEN.v[i]->data[j];
        if (new_in != IN.v[i]->data[j]) {
          IN.v[i]->data[j] = new_in;
          iterate = true;
        }
        unsigned new_out = 0;
        forv_Vec(BasicBlock, bbout, bb->outs) {
          new_out = new_out | IN.v[bbout->id]->data[j];
        }
        if (new_out != OUT.v[i]->data[j]) {
          OUT.v[i]->data[j] = new_out;
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


void forwardFlowAnalysis(FnSymbol* fn,
                         Vec<BitVec*>& GEN,
                         Vec<BitVec*>& KILL,
                         Vec<BitVec*>& IN,
                         Vec<BitVec*>& OUT,
                         bool intersect) {
  int nbbq = fn->basicBlocks->n; // size of bb queue
  Vec<int> bbq;
  BitVec bbs(nbbq);
  int iq = -1, nq = nbbq-1;      // index to first and last bb in bbq
  for (int i = 0; i < fn->basicBlocks->n; i++) {
    bbq.add(i);
    bbs.set(i);
  }
  while (iq != nq) {
    iq = (iq + 1) % nbbq;
    int i = bbq.v[iq];
    bbs.unset(i);
#ifdef DEBUG_FLOW
    if (iq == 0) {
      printf("IN\n"); printBitVectorSets(IN);
      printf("OUT\n"); printBitVectorSets(OUT);
    }
#endif
    BasicBlock* bb = fn->basicBlocks->v[i];
    bool change = false;
    for (int j = 0; j < IN.v[i]->ndata; j++) {
      if (bb->ins.n > 0) {
        unsigned new_in = (intersect) ? (unsigned)(-1) : 0;
        forv_Vec(BasicBlock, bbin, bb->ins) {
          if (intersect)
            new_in &= OUT.v[bbin->id]->data[j];
          else
            new_in |= OUT.v[bbin->id]->data[j];
        }
        if (new_in != IN.v[i]->data[j]) {
          IN.v[i]->data[j] = new_in;
          change = true;
        }
      }
      unsigned new_out = (IN.v[i]->data[j] & ~KILL.v[i]->data[j]) | GEN.v[i]->data[j];
      if (new_out != OUT.v[i]->data[j]) {
        OUT.v[i]->data[j] = new_out;
        change = true;
      }
    }
    if (change) {
      forv_Vec(BasicBlock, bbout, bb->outs) {
        if (!bbs.get(bbout->id)) {
          nq = (nq + 1) % nbbq;
          bbs.set(bbout->id);
          bbq.v[nq] = bbout->id;
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

void printLocalsVectorSets(Vec<BitVec*>& sets, Vec<Symbol*> locals) {
  int i = 0;
  forv_Vec(BitVec, set, sets) {
    printf("%2d: ", i);
    for (int j = 0; j < set->size; j++) {
      if (set->get(j))
        printf("%s[%d] ", locals.v[j]->name, locals.v[j]->id);
    }
    printf("\n");
    i++;
  }
  printf("\n");
}

void printBitVectorSets(Vec<BitVec*>& sets) {
  int i = 0;
  forv_Vec(BitVec, set, sets) {
    printf("%2d: ", i);
    for (int j = 0; j < set->size; j++) {
      printf("%d", (set->get(j)) ? 1 : 0);
      if ((j+1) % 10 == 0) printf(" ");
    }
    printf("\n");
    i++;
  }
  printf("\n");
}
