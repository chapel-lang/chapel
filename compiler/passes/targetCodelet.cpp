#include "stmt.h"
#include "bb.h"
#include "expr.h"
#include "interval.h"
#include "passes.h"
#include "symbol.h"
#include "astutil.h"
#include "optimizations.h"

#define END_NODE 0
#define START_NODE 1
#define TAG_ID(id) (id)

/* Routine to remove empty if or while blocks.
 * These typically occur due to previous transformations */
static void removeEmptyBlocks(FnSymbol *fn) {
  Vec<BaseAST*> asts;
  collect_asts_postorder(fn, asts);
  forv_Vec(BaseAST, ast, asts) {
    /* if stmt is an if of some kind, and if its body is empty, then remove it */
    if (CondStmt *cond = toCondStmt(ast)) {
      bool thenEmpty = false;
      bool elseEmpty = false;
      if ((cond->thenStmt && cond->thenStmt->length() == 0) || cond->thenStmt == NULL)
        thenEmpty = true;
      if ((cond->elseStmt && cond->elseStmt->length() == 0) || cond->elseStmt == NULL)
        elseEmpty = true;
      if (thenEmpty && elseEmpty) {
        printf("removing empty if\n");
        cond->remove();
      }
    }
    else if (BlockStmt *block = toBlockStmt(ast)) {
      /* if block is a loop of some kind, and if its body is empty, then remove it */
      if (block->isLoop() && block->length() == 0) {
        printf("removing empty while\n");
        block->remove();
      }
    }
  }
}

static void
innerCodelets(FnSymbol* originalFn, BlockStmt* codeletBlock, BasicBlock* curBB, 
              Vec<BasicBlock*>& innerBB, Interval* interval, Map<BasicBlock*, 
              Interval*>& intervalMaps) {

  bool FLAG_MOVE_DEF = (interval->intervalTag != FORALL_LOOP);
  forv_Vec(Expr, expr, curBB->exprs) {
    /* more than likely if we are in variable declarations */
    if (CallExpr *call = toCallExpr(expr)) {
      if (call->isPrimitive(PRIM_RETURN)) {
        call->get(1)->remove();
        call->remove();
      } else if (call->isPrimitive(PRIM_ON_IL)) {
        call->remove();
      } else {
        codeletBlock->insertAtTail(call->remove());
      }
    }
    else if (DefExpr *def = toDefExpr(expr)) {
      if (FLAG_MOVE_DEF && !def->sym->hasFlag(FLAG_WHILE_INDEX)) {
        // Make variable global
        ModuleSymbol *mod = originalFn->getModule();
        mod->block->insertAtHead(def->remove());
      }
      else if (!def->sym->hasFlag(FLAG_WHILE_INDEX))
        codeletBlock->insertAtTail(def->remove());
      /* Instead, we should search the other intervals to see if this definition is
       * used elsewhere. If not, no need to move it out of the interval */
    }
    else if (SymExpr *sym = toSymExpr(expr)) {
      /* figure out where it is */
      if (sym->parentExpr) {
        if (BlockStmt *stmt = toBlockStmt(sym->parentExpr)) {
          if (stmt != originalFn->body) {
            ;
          }
        }
        else if (toCondStmt(sym->parentExpr)) {
          CallExpr* ci = new CallExpr(PRIM_IDEN, sym->copy());
          BasicBlock* thenbb = curBB->thenBB;
          BasicBlock* elsebb = curBB->elseBB;
          BlockStmt* thenStmt = new BlockStmt();
          BlockStmt* elseStmt = new BlockStmt();
          if (intervalMaps.get(thenbb) == interval){
            innerBB.remove(innerBB.index(thenbb));
            innerCodelets(originalFn, thenStmt, thenbb, innerBB, interval, intervalMaps);
          }
          if (intervalMaps.get(elsebb) == interval){
            innerBB.remove(innerBB.index(elsebb));
            innerCodelets(originalFn, elseStmt, elsebb, innerBB, interval, intervalMaps);
          }
          codeletBlock->insertAtTail(new CondStmt(ci,thenStmt,elseStmt));
        }
      }
    }
    else if (BlockStmt *block = toBlockStmt(expr)) {
      codeletBlock->insertAtTail(block->remove());
    }
    else if (CondStmt* cond = toCondStmt(expr)){
      codeletBlock->insertAtTail(cond->remove());
    } else {
      INT_FATAL(expr, "NOT SUPPOSED TO BE HERE...DEBUG ASAP!\n");
    }
  }
}

static void
createCodelet(FnSymbol *originalFn, FnSymbol *codeletFn, Interval *interval, Map<BasicBlock*, Interval*>& intervalMaps) {
  Vec<BasicBlock*> innerBB;
  innerBB.copy(interval->bb);

  Vec<Expr*> *short_condExprs;
  Vec<bool> *short_condBools;

  while (innerBB.n > 0) {
    BasicBlock* bb = innerBB.v[0];
    innerBB.remove(innerBB.index(bb));
    /* Need some way to preserve control flow between BBs inside of an interval node
     * This seems to only be needed when dealing with a do-while loop that contains parallel
     * nested. */
    /* If BB contains condExpr, surround the BB with the NOT version of it */
    if (bb->condExprs.n > 0) {
      short_condExprs = &bb->condExprs;
      short_condBools = &bb->condBools;
      Expr *expr = bb->condExprs.v[0];
      if (SymExpr *sym = toSymExpr(expr)) {
        if (sym->parentExpr) {
          if (CallExpr *call = toCallExpr(sym->parentExpr)) {
            if (call->isPrimitive(PRIM_BLOCK_DOWHILE_LOOP)) {
              CallExpr* condNumID;
              if (bb->condBools.v[0] == true)
                condNumID = new CallExpr(PRIM_IDEN, bb->condExprs.v[0]->copy());
              else if (bb->condBools.v[0] == false)
                condNumID = new CallExpr(PRIM_UNARY_LNOT, bb->condExprs.v[0]->copy());
              for (int i = 1; i < bb->condExprs.n; i++){
                if (bb->condBools.v[i] == true)
                  condNumID = new CallExpr(PRIM_AND, condNumID, bb->condExprs.v[i]->copy());
                else if (bb->condBools.v[i] == false)
                  condNumID = new CallExpr(PRIM_AND, condNumID, new CallExpr(PRIM_UNARY_LNOT, bb->condExprs.v[i]->copy()));
              }
              BlockStmt *elseStmt = new BlockStmt();
              innerCodelets(originalFn, elseStmt, bb, innerBB, interval, intervalMaps);

              /* If BB is at the end, add signal to mark the end of the program
               * TODO: There is probably a better place to add this call */
              if (bb->outs.n == 0)
                elseStmt->insertAtTail(new CallExpr(PRIM_CODELET_NOTIFY, new_IntSymbol(END_NODE)));
              codeletFn->body->insertAtTail(new CondStmt(condNumID, elseStmt));
            }
            else {
              INT_FATAL(expr, "FIX ASAP!!! We are not inside of a DO-WHILE\n");
            }
          }
          else {
            INT_FATAL(expr, "OH OH...something bad is happening\n");
          }
        }
      }
    }
    /* Situation where there is no BB's inside an interval that are chanined together
     * via a loop (e.g. do-while) */
    else
      innerCodelets(originalFn, codeletFn->body, bb, innerBB, interval, intervalMaps);
  }

  Vec<Expr*> tmpExpr;
  Vec<bool> tmpBools;
  
  /* Generate conditional statements if there are > 1 outgoing interval edges */
  if (interval->intervalTag != FORALL_LOOP) {
    if (interval->outs.n > 1 || (interval->outs.n == 1 && interval->isEnd)) {
      for (int i = 0; i < interval->outs.n; i++){
        Interval* targetInt = interval->outs.v[i];
        BasicBlock* sourceBB = interval->sourceEdges.v[i];
        tmpExpr.copy(sourceBB->condExprs);
        tmpBools.copy(sourceBB->condBools);
        if (sourceBB->outs.n > 1){
          tmpExpr.add(sourceBB->branch);
          if (targetInt->bb.v[0] == sourceBB->thenBB)
            tmpBools.add(true);
          else if (targetInt->bb.v[0] == sourceBB->elseBB)
            tmpBools.add(false);
        }
        if (tmpExpr.n >= 1){
          CallExpr* condNumID;
          BlockStmt* thenStmt = new BlockStmt();
          if (tmpBools.v[0] == true)
            condNumID = new CallExpr(PRIM_IDEN, tmpExpr.v[0]->copy());
          else if (tmpBools.v[0] == false)
            condNumID = new CallExpr(PRIM_UNARY_LNOT, tmpExpr.v[0]->copy());
          for (int j = 1; j < tmpExpr.n; j++){
            if (tmpBools.v[j] == true)
              condNumID = new CallExpr(PRIM_AND, condNumID, tmpExpr.v[j]->copy());
            else if (tmpBools.v[j] == false)
              condNumID = new CallExpr(PRIM_AND, condNumID, new CallExpr(PRIM_UNARY_LNOT, tmpExpr.v[j]->copy()));
          }
          thenStmt->insertAtTail(new CallExpr(PRIM_CODELET_NOTIFY, new_IntSymbol(targetInt->id)));
          codeletFn->insertAtTail(new CondStmt(condNumID,thenStmt));
        }
        tmpExpr.clear();
        tmpBools.clear();
      }
    }
    /* If there is only one outgoing edge, but we are not at the end of the program yet */
    else if ((interval->outs.n == 1) && (!interval->isEnd)) {
      Interval* targetInt = interval->outs.v[0];
      codeletFn->insertAtTail(new CallExpr(PRIM_CODELET_NOTIFY, new_IntSymbol(targetInt->id)));
    }
    /* If we are the last interval and there are no other outgoing edges */
    else if (interval->outs.n == 0 || interval->isEnd) {
      codeletFn->insertAtTail(new CallExpr(PRIM_CODELET_NOTIFY, new_IntSymbol(END_NODE)));
    }
  }
  codeletFn->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
  codeletFn->retType = dtVoid;
}


static void
createFn(FnSymbol *fn, Map<BasicBlock*, Interval*>& intervalMaps) {
  if (fn->intervals) {
    Vec<FnSymbol*> nestedFunctions;
    FnSymbol *intervalFn;
    Map<Symbol*,Vec<SymExpr*>*> defMap;
    Map<Symbol*,Vec<SymExpr*>*> useMap;


    buildDefUseMaps(fn, defMap, useMap);

    /* here we declare the branching conditional needed for MAP */
    ModuleSymbol *mod = fn->getModule();
    VarSymbol *numID= new VarSymbol("numID", dtInt[INT_SIZE_32]);
    mod->block->insertAtHead(new DefExpr(numID));

    forv_Vec(Interval, interval, *fn->intervals) {

      intervalFn = new FnSymbol(astr("intervalFn", istr(interval->id)));
      intervalFn->addFlag(FLAG_CODELET);
      intervalFn->intervals = new Vec<Interval*>();
      intervalFn->intervals->add(interval);
      nestedFunctions.add(intervalFn);
      createCodelet(fn, intervalFn, interval, intervalMaps);
      interval = NULL;
      insert_help(intervalFn, NULL, intervalFn);

      CallExpr *intervalCall = new CallExpr(intervalFn);

      fn->insertAtTail(new DefExpr(intervalFn));
      fn->insertAtTail(intervalCall);
    }
    fn->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
    fn->retType = dtVoid;
    flattenNestedFunctions(nestedFunctions, true);


    compute_call_sites();
    /* Create wrapper function for each codelet, so that the runtime makes the 
     * actual codelet invocation */
    forv_Vec(FnSymbol, nestedfn, nestedFunctions) {
      forv_Vec(CallExpr, call, *nestedfn->calledBy) {
        bundleArgs(call);
      }
    }

    compute_call_sites();
    forv_Vec(FnSymbol, nestedfn, nestedFunctions) {
      forv_Vec(CallExpr, call, *nestedfn->calledBy) {

        if (call->parentExpr == NULL) continue;

        FnSymbol *wrapperFn = toFnSymbol(call->parentSymbol);

        ArgSymbol *bufferArgs = new ArgSymbol(INTENT_REF, "buffer", dtVoid);
        wrapperFn->insertFormalAtHead(bufferArgs);

        forv_Vec(CallExpr, wrapperCall, *wrapperFn->calledBy) {

          /* Construct bitmap of incoming dependent edges */
          uint64_t incoming = 0;
          forv_Vec(Interval, intIn, nestedfn->intervals->v[0]->ins) {
            incoming |= TAG_ID(intIn->id);
          }
          uint64_t outgoing = 0;
          forv_Vec(Interval, intOut, nestedfn->intervals->v[0]->outs) {
            outgoing |= TAG_ID(intOut->id);
          }
          /* place call right before memfrees */
          if (nestedfn->intervals && (nestedfn->intervals->v[0]->intervalTag == BLOCK || nestedfn->intervals->v[0]->intervalTag == FOR_LOOP)) {
            wrapperCall->insertBefore(new CallExpr(PRIM_CODELET_SEQ, new_IntSymbol(TAG_ID(nestedfn->intervals->v[0]->id)), 
                  new_IntSymbol(incoming), new_IntSymbol(outgoing), numID, new_StringSymbol(wrapperFn->name), wrapperCall->get(1)->copy()));
            wrapperCall->remove();
          }
          /* place call right before memfrees */
          else if (nestedfn->intervals && (nestedfn->intervals->v[0]->intervalTag == FORALL_LOOP)) {
            wrapperCall->insertBefore(new CallExpr(PRIM_CODELET_PAR, new_IntSymbol(TAG_ID(nestedfn->intervals->v[0]->id)),
                  new_IntSymbol(incoming), new_IntSymbol(outgoing), nestedfn->intervals->v[0]->low,
                  nestedfn->intervals->v[0]->stride, nestedfn->intervals->v[0]->high, nestedfn->intervals->v[0]->ILIndex,
                  numID, new_StringSymbol(wrapperFn->name), wrapperCall->get(1)->copy()));
            wrapperCall->remove();
          }
        }
      }
    }
  }
  collapseBlocks(fn->body);
}

static void inlineMainFunction(void) {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (!strcmp(fn->name, "main")) {
      fn->addFlag(FLAG_INLINE);
      break;
    }
  }
  inlineFunctions();
}

void 
targetCodelet() {

  if (fTargetCodelet) {

    /* First thing we need to do is inline the main function */
    inlineMainFunction();

    /* For every other function besides chpl_main(), we build the intervals */
    forv_Vec(FnSymbol, fn, gFnSymbols) {
      if (!strcmp(fn->name, "chpl_gen_main")) {

        Map<BasicBlock*, Interval*> intervalMaps;

        singleAssignmentRefPropagation(fn);
        localCopyPropagation(fn);
        deadCodeElimination(fn);
        globalCopyPropagation(fn);
        deadCodeElimination(fn);
        removeUnnecessaryGotos(fn);

        /* 1. Coursen the basic blocks for the given function */
        buildCoarseBasicBlocks(fn);
        
        /* 2. Build intervals */
        buildIntervals(fn, intervalMaps);
      
        /* 3. Convert Basic Blocks to separate function */
        createFn(fn, intervalMaps);
        removeEmptyBlocks(fn);
        //inlineFunction(fn);

        if (fTargetCodelet) {
          /* Insert codelet initialization call at the head of main */
          fn->insertAtHead(new CallExpr(PRIM_CODELET_INIT));

          /* Insert "start" trigger of execution */
          fn->body->body.tail->insertBefore(new CallExpr(PRIM_CODELET_NOTIFY, new_IntSymbol(START_NODE)));

          /* Insert "barrier"  just before end of program */
          fn->body->body.tail->insertBefore(new CallExpr(PRIM_CODELET_WAIT_FOR_ID, new_IntSymbol(END_NODE)));

          /* Insert codelet shutdown code at end of program */
          fn->body->body.tail->insertBefore(new CallExpr(PRIM_CODELET_SHUTDOWN));
        }

        return;
      }
    }
  }
}
