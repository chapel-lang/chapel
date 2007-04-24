#include "alist.h"
#include "type.h"
#include "symbol.h"
#include "expr.h"
#include "stmt.h"

/* Return if type t would be allocated on the heap.
   Currently only true if t is a regular Class type */
bool isHeapType(Type* t) {
  if (ClassType* ct = dynamic_cast<ClassType*>(t)) {
    if (ct->classTag == CLASS_CLASS) {
      return true;
    }
  }
  return false;
}


/* Insert primitive calls to add and remove a symbol to the root set */
void addToRootSet(FnSymbol* fn, Symbol* sym) {
  if (isHeapType(sym->type)) {
    fn->insertAtHead(new CallExpr(PRIMITIVE_GC_ADD_ROOT, sym));
    fn->insertBeforeReturnAfterLabel(new CallExpr(PRIMITIVE_GC_DELETE_ROOT));
  }
}


/* Recursively walk through sub-blocks adding pointers to the root set */
void copyCollectionHelper(FnSymbol* fn, BlockStmt* block) {
  for_asts(expr, block->body) {
    if (DefExpr* def = dynamic_cast<DefExpr*>(expr)) {
      addToRootSet(fn, def->sym);
    } else if (BlockStmt* blk = dynamic_cast<BlockStmt*>(expr)) {
      copyCollectionHelper(fn, blk);
    }
  }
  
}


void copyCollection(void) {
  if (!copyCollect)
    return;
  forv_Vec(FnSymbol, fn, gFns) {
    for_formals(formal, fn) {
      addToRootSet(fn, formal);
    }
    copyCollectionHelper(fn, fn->body);
  }
}
