#include "bb.h"
#include "expr.h"
#include "stmt.h"
#include "../traversals/view.h"
#include "map.h"

BasicBlock::BasicBlock(int init_id) : id(init_id) {}

#define BB_START()                              \
  basicBlock = new BasicBlock(++id)

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

void buildBasicBlocks(FnSymbol* fn, Stmt* stmt) {
  static BasicBlock* basicBlock;
  static Map<LabelSymbol*,Vec<BasicBlock*>*> gotoMaps;
  static Map<LabelSymbol*,BasicBlock*> labelMaps;
  static int id;
  if (!stmt) {
    fn->basicBlocks = new Vec<BasicBlock*>();
    gotoMaps.clear();
    labelMaps.clear();
    id = 0;
    BB_START();
    BBB(fn->body);
    BB_STOP();
  } else {
    if (ExprStmt* s = dynamic_cast<ExprStmt*>(stmt)) {
      BB_ADD(s->expr);
    } else if (ReturnStmt* s = dynamic_cast<ReturnStmt*>(stmt)) {
      BB_ADD(s->expr);
    } else if (BlockStmt* s = dynamic_cast<BlockStmt*>(stmt)) {
      for_alist(Stmt, stmt, s->body) {
        BBB(stmt);
      }
    } else if (WhileLoopStmt* s = dynamic_cast<WhileLoopStmt*>(stmt)) {
      BB_ADD(s->condition);
      BasicBlock* top = basicBlock;
      BB_RESTART();
      BasicBlock* bodyTop = basicBlock;
      BBB(s->block);
      BasicBlock* bodyBottom = basicBlock;
      BB_RESTART();
      BasicBlock* bottom = basicBlock;
      BB_THREAD(top, bottom);
      BB_THREAD(top, bodyTop);
      BB_THREAD(bodyBottom, bottom);
    } else if (ForLoopStmt* s = dynamic_cast<ForLoopStmt*>(stmt)) {
      BB_ADD_LS(s->indices);
      BB_ADD_LS(s->iterators);
      BasicBlock* top = basicBlock;
      BB_RESTART();
      BasicBlock* bodyTop = basicBlock;
      BBB(s->innerStmt);
      BasicBlock* bodyBottom = basicBlock;
      BB_RESTART();
      BasicBlock* bottom = basicBlock;
      BB_THREAD(top, bottom);
      BB_THREAD(top, bodyTop);
      BB_THREAD(bodyBottom, bottom);
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
    } else if (SelectStmt* s = dynamic_cast<SelectStmt*>(stmt)) {
      BB_ADD(s->caseExpr);
      for_alist(WhenStmt, whenStmt, s->whenStmts) {
        BB_ADD_LS(whenStmt->caseExprs);
      }
      BasicBlock* top = basicBlock;
      BB_RESTART();
      Vec<BasicBlock*> selectTops;
      Vec<BasicBlock*> selectBottoms;
      for_alist(WhenStmt, whenStmt, s->whenStmts) {
        selectTops.add(basicBlock);
        BBB(whenStmt->doStmt);
        selectBottoms.add(basicBlock);
        BB_RESTART();
      }
      BasicBlock* bottom = basicBlock;
      forv_Vec(BasicBlock, bb, selectTops) {
        BB_THREAD(top, bb);
      }
      forv_Vec(BasicBlock, bb, selectBottoms) {
        BB_THREAD(bb, bottom);
      }
      // Can you bypass every when clause?  If so, uncomment
      // BB_THREAD(top, bottom);
    } else if (LabelStmt* s = dynamic_cast<LabelStmt*>(stmt)) {
      BasicBlock* top = basicBlock;
      BB_RESTART();
      BasicBlock* bottom = basicBlock;
      BB_THREAD(top, bottom);
      LabelSymbol* label = dynamic_cast<LabelSymbol*>(s->defLabel->sym);
      if (Vec<BasicBlock*>* vbb = gotoMaps.get(label)) {
        forv_Vec(BasicBlock, basicBlock, *vbb) {
          BB_THREAD(basicBlock, bottom);
        }
      }
      labelMaps.put(label, bottom);
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
      INT_FATAL(stmt, "Unexpected stmt in buildBasicBlocks");
    }
  }
}

void printBasicBlocks(FnSymbol* fn) {
  forv_Vec(BasicBlock, b, *fn->basicBlocks) {
    printf("%d:  ", b->id);
    forv_Vec(BasicBlock, bb, b->ins) {
      printf("%d ", bb->id);
    }
    printf(" >  ");
    forv_Vec(BasicBlock, bb, b->outs) {
      printf("%d ", bb->id);
    }
    forv_Vec(Expr, expr, b->exprs) {
      print_view_noline(expr);
    }
    printf("\n\n");
  }
}
