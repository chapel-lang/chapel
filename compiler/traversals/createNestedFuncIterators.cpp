#include "createNestedFuncIterators.h"
#include "stringutil.h"
#include "stmt.h"
#include "expr.h"

//
// changes yields to copy of body of for loop, changes returns to
// return nothing
//
class UpdateIteratorYield : public Traversal {
public:
  FnSymbol* fn;

  UpdateIteratorYield(FnSymbol* iFn) : fn(iFn) { }

  // Changes yields to calls to function
  void postProcessExpr(Expr* expr) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(expr)) {
      if (SymExpr* symExpr = dynamic_cast<SymExpr*>(call->baseExpr)) {
        if (!strcmp(symExpr->var->name, "_yield")) {
          call->argList->first()->remove();
          call->replace(new CallExpr(fn, call->argList));
        }
      }
    }
  }

  // Changes returns to return nothing
  void postProcessStmt(Stmt* stmt) {
    if (ReturnStmt* ret = dynamic_cast<ReturnStmt*>(stmt))
      if (ret->expr)
        ret->expr->remove();  
  }
};


static void
addVarToFormals(FnSymbol* fn, Symbol* sym) {
  Map<BaseAST*,BaseAST*> update_map;
  ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, sym->name, sym->type);
  fn->formals->insertAtTail(new DefExpr(arg));
  update_map.put(sym, arg);
  fn->body->traverse(new UpdateSymbols(&update_map));
}


void CreateNestedFuncIterators::postProcessStmt(Stmt* stmt) {
  if (ForLoopStmt* fls = dynamic_cast<ForLoopStmt*>(stmt)) {
    if (fls->iterators->length() > 1) // no zippered iterators
      return;
    CallExpr* iteratorCall = dynamic_cast<CallExpr*>(fls->iterators->only());
    if (CallExpr* t = dynamic_cast<CallExpr*>(iteratorCall->argList->only())) {
      if (t->findFnSymbol()->fnClass == FN_ITERATOR) {
        iteratorCall = t;
      }
    }
    FnSymbol* iterator = iteratorCall->findFnSymbol()->copy(true);
    iterator->retType = dtVoid;
    iterator->addPragma("inline");
    FnSymbol* body = new FnSymbol("loop_body");
    body->addPragma("inline");
    body->formals = new AList<DefExpr>();
    body->body = fls->innerStmt->copy();
    body->retType = dtVoid;
    fls->insertBefore(new ExprStmt(new DefExpr(body)));
    fls->insertBefore(new ExprStmt(new DefExpr(iterator)));
    addVarToFormals(body, fls->indices->only()->sym);
    TRAVERSE(iterator, new UpdateIteratorYield(body), true);
    fls->replace(new ExprStmt(new CallExpr(iterator, iteratorCall->argList)));
  }
}
