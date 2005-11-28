/*** normalize
 ***
 *** This pass and function normalizes parsed and scope-resolved AST.
 ***/

#include "astutil.h"
#include "expr.h"
#include "passes.h"
#include "runtime.h"
#include "runtime.h"
#include "stmt.h"
#include "symbol.h"
#include "symtab.h"
#include "stringutil.h"

static void reconstruct_iterator(FnSymbol* fn);
static void build_lvalue_function(FnSymbol* fn);

void normalize(void) {
  Vec<FnSymbol*> fns;
  collect_functions(&fns);
  forv_Vec(FnSymbol, fn, fns) {
    if (fn->fnClass == FN_ITERATOR)
      reconstruct_iterator(fn);
    if (0 && fn->retRef)
      build_lvalue_function(fn);
  }
}

static void reconstruct_iterator(FnSymbol* fn) {
  Expr* seqType = NULL;
  if (fn->retType != dtUnknown) {
    seqType = new SymExpr(fn->retType->symbol);
  } else if (fn->defPoint->exprType) {
    seqType = fn->defPoint->exprType->copy();
  } else {
    Vec<BaseAST*> asts;
    collect_asts(&asts, fn->body);
    Type* type = dtUnknown;
    forv_Vec(BaseAST, ast, asts) {
      if (ReturnStmt* returnStmt = dynamic_cast<ReturnStmt*>(ast))
        if (returnStmt->expr)
          type = returnStmt->expr->typeInfo();
    }
    if (type != dtUnknown) {
      seqType = new SymExpr(type->symbol);
    } else {
      USR_FATAL(fn, "Unable to infer type of iterator");
    }
  }

  Symbol* seq = new VarSymbol("_seq_result");
  DefExpr* def = new DefExpr(seq, NULL, new CallExpr(chpl_seq, seqType));

  fn->insertAtHead(new ExprStmt(def));

  Vec<BaseAST*> asts;
  collect_asts_postorder(&asts, fn->body);
  forv_Vec(BaseAST, ast, asts) {
    if (ReturnStmt* returnStmt = dynamic_cast<ReturnStmt*>(ast)) {
      Expr* expr = returnStmt->expr;
      returnStmt->expr->replace(new SymExpr(seq));
      returnStmt->insertBefore(
        new ExprStmt(new CallExpr(new MemberAccess(seq, "_yield"), expr)));
      if (returnStmt->yield)
        returnStmt->remove();
    }
  }
  fn->insertAtTail(new ReturnStmt(seq));
  fn->retType = dtUnknown;
  if (fn->defPoint->exprType)
    fn->defPoint->exprType->replace(def->exprType->copy());
  else if (no_infer) {
    DefExpr* tmp = fn->defPoint;
    tmp->replace(new DefExpr(fn, NULL, def->exprType->copy()));
  }
}

static AList<Stmt>* handle_return_expr(Expr* e, Symbol* lvalue) {
  Stmt* newStmt = NULL;
  if (e)
    if (CondExpr *ce = dynamic_cast<CondExpr *>(e))
      newStmt = new CondStmt(ce->condExpr, 
                             handle_return_expr(ce->thenExpr, lvalue),
                             handle_return_expr(ce->elseExpr, lvalue));
    else
      newStmt = new ExprStmt(new CallExpr(OP_GETSNORM, e, lvalue));
  return new AList<Stmt>(newStmt);
}

static void build_lvalue_function(FnSymbol* fn) {
  FnSymbol* new_fn = fn->copy();
  fn->defPoint->parentStmt->insertAfter(new ExprStmt(new DefExpr(new_fn)));
  if (fn->typeBinding)
    fn->typeBinding->definition->methods.add(new_fn);
  new_fn->retRef = false;
  new_fn->retType = dtVoid;
  new_fn->cname = stringcat("_setter_", fn->cname);
  ArgSymbol* setterToken = new ArgSymbol(INTENT_REF, "_setterTokenDummy",
                                         dtSetterToken);
  ArgSymbol* lvalue = new ArgSymbol(INTENT_BLANK, "_lvalue", fn->retType);
  new_fn->formals->insertAtTail(new DefExpr(setterToken));
  new_fn->formals->insertAtTail(new DefExpr(lvalue));
  Vec<BaseAST*> asts;
  collect_asts_postorder(&asts, new_fn->body);
  forv_Vec(BaseAST, ast, asts) {
    if (ReturnStmt* returnStmt = dynamic_cast<ReturnStmt*>(ast)) {
      Expr* expr = returnStmt->expr;
      returnStmt->expr->remove();
      returnStmt->insertBefore(handle_return_expr(expr, lvalue));
    }
  }
}
