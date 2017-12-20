#include "AstVisitorTraverse.h"
#include "ForallStmt.h"

// some forwards
class ExpandForForall;
static void expandYield(ExpandForForall* EV, CallExpr* yield) {} //vass
static void expandTaskFnCall(ExpandForForall* EV, CallExpr* call, FnSymbol* taskFn) {} //vass
static void expandForall(ExpandForForall* EV, ForallStmt* fs) {} //vass

class ExpandForForall : public AstVisitorTraverse {
public:

  // 'ibody' is a clone that is replacing 'fs'.
  ExpandForForall(ForallStmt* fs, FnSymbol* parIterFn,
                  BlockStmt* iwrap, BlockStmt* ibody);

  virtual bool enterCallExpr(CallExpr* node) {
    if (node->isPrimitive(PRIM_YIELD)) {
      expandYield(this, node);
    } else if (FnSymbol* taskFn = resolvedToTaskFun(node)) {
      expandTaskFnCall(this, node, taskFn);
    }
    // There shouldn't be anything interesting inside the call.
    return false;
  }

  virtual bool enterForallStmt(ForallStmt* node) {
    expandForall(this, node);
    // expandForall() takes care of descending into 'node'
    return false;
  }
};

ExpandForForall::ExpandForForall(ForallStmt* fs, FnSymbol* parIterFn,
                                 BlockStmt* iwrap, BlockStmt* ibody)
{
  INT_ASSERT(ibody->inTree()); //fyi

/* vass - need these?
  // Place initialization and finalization code before these anchors.
  CallExpr* aInit = new CallExpr("anchorInit");
  CallExpr* aFini = new CallExpr("anchorFini");
  iwrap->insertAtHead(aInit);
  iwrap->insertAtTail(aFini);
*/

  // Place initialization code before aInit, finalization code after aFini.
  Expr* aInit = ibody;
  Expr* aFini = ibody;

  // When cloning the loop body of 'fs', replace each shadow variable
  // with the variable given by 'map'.
  SymbolMap map;
  for_shadow_vars(svar, temp1, fs) {
    switch (svar->intent) {
      case TFI_DEFAULT:
      case TFI_CONST:
        INT_ASSERT(false);   // don't give me an abstract intent
        break;

      case TFI_IN:
      case TFI_CONST_IN:
        {
          bool isconst = svar->intent == TFI_CONST_IN;
          VarSymbol* bodyvar = new VarSymbol(svar->name);
          bodyvar->type = svar->type->getValType();
          bodyvar->qual = isconst ? QUAL_CONST_VAL : QUAL_VAL;
          aInit->insertBefore(new DefExpr(bodyvar));

          // Initialize it from the outer var.
          FnSymbol* autoCopyFn = getAutoCopy(bodyvar->type); // NB val type
          CallExpr* autoCopyCE = new CallExpr(autoCopyFn, svar->outerVarSym());
          aInit->insertBefore(new CallExpr(PRIM_MOVE, bodyvar, autoCopyCE));

          // Deinitialize it at the end.
          if (FnSymbol* autoDestroy = getAutoDestroy(bodyvar->type))
            aFini->insertAfter(new CallExpr(autoDestroy, bodyvar));

          map.put(svar, bodyvar);
        }
        break;

      case TFI_REF:
      case TFI_CONST_REF:
        // Let us reference the outer variable directly, for simplicity.
        map.put(svar, svar->outerVarSym());
        break;

      case TFI_REDUCE:
        break;
    }
  }
}

static void lowerForallStmtsInline() {
  forv_Vec(ForallStmt, fs, gForallStmts)
  {
    if (fs->id == breakOnResolveID) gdbShouldBreakHere(); //vass

    // If this fails, need to filter out those FSes.
    INT_ASSERT(fs->inTree() && fs->getFunction()->isResolved());

    // We have converted zippered, if any, to the follower loop.
    INT_ASSERT(fs->numIteratedExprs() == 1);
    CallExpr* parIterCall = toCallExpr(fs->firstIteratedExpr());
    
    FnSymbol* parIterFn = parIterCall->resolvedFunction();
    // Make sure it is a parallel iterator.
    INT_ASSERT(parIterFn->hasFlag(FLAG_INLINE_ITERATOR));
    // We don't know yet what to do with these.
    INT_ASSERT(!parIterFn->hasFlag(FLAG_RECURSIVE_ITERATOR));

    // Clone the iterator body. See expandIteratorInline().
    BlockStmt* ibody = parIterFn->body->copy();
    // Place to put pre- and post- code.
    BlockStmt* iwrap = new BlockStmt(ibody);
    // Let us remove 'fs' later, for debugging convenience.
    fs->insertAfter(iwrap);

    ExpandForForall expandV(fs, parIterFn, iwrap ,ibody);
    ibody->accept(&expandV);

    fs->remove();
    // We could also do {iwrap,ibody}->flattenAndRemove().
  }
}
