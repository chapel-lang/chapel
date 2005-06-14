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
  if (fn && (fn->_setter || fn->_getter || fn->isConstructor)) {
    return;
  }
  if (dynamic_cast<VarInitExpr*>(expr->parentExpr)) {
    return;
  }
  if (MemberAccess* memberAccess = dynamic_cast<MemberAccess*>(expr)) {
    if (dynamic_cast<ParenOpExpr*>(memberAccess->parentExpr)) {
      return;
    }
    AList<Expr>* arguments = new AList<Expr>();
    arguments->insertAtTail(new Variable(Symboltable::lookupInternal("_methodToken")));
    arguments->insertAtTail(memberAccess->base->copy());
    Expr* replacement =
      new ParenOpExpr(
        new Variable(
          new UnresolvedSymbol(memberAccess->member->name)),
        arguments);
    expr->replace(replacement);
  }
  if (ParenOpExpr* parenOpExpr = dynamic_cast<ParenOpExpr*>(expr)) {
    if (MemberAccess* memberAccess = dynamic_cast<MemberAccess*>(parenOpExpr->baseExpr)) {
      AList<Expr>* arguments = parenOpExpr->argList->copy();
      arguments->insertAtHead(memberAccess->base->copy());
      arguments->insertAtHead(new Variable(Symboltable::lookupInternal("_methodToken")));
      Expr* replacement =
        new ParenOpExpr(
          new Variable(
            new UnresolvedSymbol(memberAccess->member->name)),
          arguments);
      expr->replace(replacement);
    }
  }
}
