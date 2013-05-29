#include "astutil.h"
#include "passes.h"
#include "stmt.h"

void createTaskFunctions(void) {
  // Process task-creating constructs. We include 'on' blocks, too.
  // This code used to be in parallel().
  forv_Vec(BlockStmt, block, gBlockStmts) {
    if (CallExpr* info = block->blockInfo) {
      SET_LINENO(block);
      FnSymbol* fn = NULL;
      VarSymbol* oldSubLoc = NULL;
      if (info->isPrimitive(PRIM_BLOCK_BEGIN)) {
        fn = new FnSymbol("begin_fn");
        fn->addFlag(FLAG_BEGIN);
      } else if (info->isPrimitive(PRIM_BLOCK_COBEGIN)) {
        fn = new FnSymbol("cobegin_fn");
        fn->addFlag(FLAG_COBEGIN_OR_COFORALL);
      } else if (info->isPrimitive(PRIM_BLOCK_COFORALL)) {
        fn = new FnSymbol("coforall_fn");
        fn->addFlag(FLAG_COBEGIN_OR_COFORALL);
      } else if (info->isPrimitive(PRIM_BLOCK_ON) ||
                 info->isPrimitive(PRIM_BLOCK_ON_NB)) {
        fn = new FnSymbol("on_fn");
        fn->addFlag(FLAG_ON);
        if (block->blockInfo->isPrimitive(PRIM_BLOCK_ON_NB))
          fn->addFlag(FLAG_NON_BLOCKING);
        // This is now a real locale arg.
        ArgSymbol* arg = new ArgSymbol(INTENT_CONST_IN, "_locale_arg", dtLocaleID);
        fn->insertFormalAtTail(arg);
        // Special case for the first argument of an on_fn, which carries the destination locale ID.
        // We set the sublocale field in task-private data before executing the body of the task,
        // saving off a copy for restoration at the end of the on block.
        oldSubLoc = newTemp(dtInt[INT_SIZE_32]);
        fn->insertAtTail(new DefExpr(oldSubLoc));
        fn->insertAtTail(new CallExpr(PRIM_MOVE, oldSubLoc, new CallExpr(PRIM_GET_SUBLOC_ID)));
        fn->insertAtTail(new CallExpr(PRIM_SET_SUBLOC_ID, new CallExpr(PRIM_LOC_GET_SUBLOC, arg)));
      }
      else if (info->isPrimitive(PRIM_BLOCK_PARAM_LOOP) || // resolution will remove this case.
               info->isPrimitive(PRIM_BLOCK_WHILEDO_LOOP) ||
               info->isPrimitive(PRIM_BLOCK_DOWHILE_LOOP) ||
               info->isPrimitive(PRIM_BLOCK_FOR_LOOP) ||
               info->isPrimitive(PRIM_BLOCK_XMT_PRAGMA_FORALL_I_IN_N) ||
               info->isPrimitive(PRIM_BLOCK_XMT_PRAGMA_NOALIAS) ||
               info->isPrimitive(PRIM_BLOCK_LOCAL) ||
               info->isPrimitive(PRIM_BLOCK_UNLOCAL))
        ; // Not a parallel block construct, so do nothing special.
      else
        INT_FATAL(block, "Unhandled blockInfo case.");

      if (fn) {
        INT_ASSERT(isTaskFun(fn));
        CallExpr* call = new CallExpr(fn);
        if (block->blockInfo->isPrimitive(PRIM_BLOCK_ON) ||
            block->blockInfo->isPrimitive(PRIM_BLOCK_ON_NB))
          // This puts the target locale expression "onExpr" at the start of the call.
          call->insertAtTail(block->blockInfo->get(1)->remove());

        block->insertBefore(new DefExpr(fn));
        block->insertBefore(call);
        block->blockInfo->remove();
        // This block becomes the body of the new function.
        fn->insertAtTail(block->remove());
        if (oldSubLoc) // only true for ON blocks
          fn->insertAtTail(new CallExpr(PRIM_SET_SUBLOC_ID, oldSubLoc));
        fn->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
        fn->retType = dtVoid;

      } // if fn
    } // if blockInfo
  } // for block

}  // createTaskFunctions()
