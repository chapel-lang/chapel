#include "applyGettersSetters.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"


void ApplyGettersSetters::postProcessExpr(Expr* expr) {
  if (!applyGettersSetters) {
    return;
  }
  FnSymbol* fn = NULL;
  if (expr->parentStmt) {
    fn = expr->parentStmt->parentFunction();
  }
  if (fn && (fn->_setter || fn->_getter || fn->fnClass == FN_CONSTRUCTOR)) {
    return;
  }
  if (dynamic_cast<VarInitExpr*>(expr->parentExpr)) {
    return;
  }
  if (MemberAccess* memberAccess = dynamic_cast<MemberAccess*>(expr)) {
    if (CallExpr* parenOpExpr = dynamic_cast<CallExpr*>(memberAccess->parentExpr)) {
      if (parenOpExpr->baseExpr == memberAccess) {
        return;
      }
    }
    AList<Expr>* arguments = new AList<Expr>();
    arguments->insertAtTail(new Variable(Symboltable::lookupInternal("_methodToken")));
    arguments->insertAtTail(memberAccess->base->copy());
    Expr* replacement =
      new CallExpr(
        new Variable(
          new UnresolvedSymbol(memberAccess->member->name)),
        arguments);
    expr->replace(replacement);
  }
  if (CallExpr* parenOpExpr = dynamic_cast<CallExpr*>(expr)) {
    if (MemberAccess* memberAccess = dynamic_cast<MemberAccess*>(parenOpExpr->baseExpr)) {
      AList<Expr>* arguments = parenOpExpr->argList->copy();
      arguments->insertAtHead(memberAccess->base->copy());
      arguments->insertAtHead(new Variable(Symboltable::lookupInternal("_methodToken")));
      Expr* replacement =
        new CallExpr(
          new Variable(
            new UnresolvedSymbol(memberAccess->member->name)),
          arguments);
      expr->replace(replacement);
    }
  }
}
