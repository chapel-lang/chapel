#include <typeinfo>
#include "resolveOverloadedOperators.h"
#include "symtab.h"
#include "symscope.h"
#include "symbol.h"
#include "type.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"


static void mangle_overloaded_operator_function_names(FnSymbol* fn) {
  static int uid = 0;

  Pragma* pr = fn->defPoint->parentStmt->pragmas;
  while (pr) {
    if (!strcmp(pr->str, "builtin")) {
      return;
    }
    pr = dynamic_cast<Pragma *>(pr->next);
  }

  if (!strcmp(fn->name, "=")) {
    fn->cname = glomstrings(2, "_assign", intstring(uid++));
  }
  else if (!strcmp(fn->name, "+")) {
    fn->cname = glomstrings(2, "_plus", intstring(uid++));
  }
  else if (!strcmp(fn->name, "-")) {
    fn->cname = glomstrings(2, "_minus", intstring(uid++));
  }
  else if (!strcmp(fn->name, "*")) {
    fn->cname = glomstrings(2, "_times", intstring(uid++));
  }
  else if (!strcmp(fn->name, "/")) {
    fn->cname = glomstrings(2, "_div", intstring(uid++));
  }
  else if (!strcmp(fn->name, "mod")) {
    fn->cname = glomstrings(2, "_mod", intstring(uid++));
  }
  else if (!strcmp(fn->name, "==")) {
    fn->cname = glomstrings(2, "_eq", intstring(uid++));
  }
  else if (!strcmp(fn->name, "!=")) {
    fn->cname = glomstrings(2, "_ne", intstring(uid++));
  }
  else if (!strcmp(fn->name, "<=")) {
    fn->cname = glomstrings(2, "_le", intstring(uid++));
  }
  else if (!strcmp(fn->name, ">=")) {
    fn->cname = glomstrings(2, "_ge", intstring(uid++));
  }
  else if (!strcmp(fn->name, "<")) {
    fn->cname = glomstrings(2, "_lt", intstring(uid++));
  }
  else if (!strcmp(fn->name, ">")) {
    fn->cname = glomstrings(2, "_gt", intstring(uid++));
  }
  else if (!strcmp(fn->name, "&")) {
    fn->cname = glomstrings(2, "_bitand", intstring(uid++));
  }
  else if (!strcmp(fn->name, "|")) {
    fn->cname = glomstrings(2, "_bitor", intstring(uid++));
  }
  else if (!strcmp(fn->name, "^")) {
    fn->cname = glomstrings(2, "_xor", intstring(uid++));
  }
  else if (!strcmp(fn->name, "and")) {
    fn->cname = glomstrings(2, "_and", intstring(uid++));
  }
  else if (!strcmp(fn->name, "or")) {
    fn->cname = glomstrings(2, "_or", intstring(uid++));
  }
  else if (!strcmp(fn->name, "**")) {
    fn->cname = glomstrings(2, "_exponent", intstring(uid++));
  }
  else if (!strcmp(fn->name, "by")) {
    fn->cname = glomstrings(2, "_by", intstring(uid++));
  }
}


void ResolveOverloadedOperators::postProcessExpr(Expr* expr) {
  if (DefExpr* def_expr = dynamic_cast<DefExpr*>(expr)) {
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(def_expr->sym)) {
      if (fn->parentScope->type != SCOPE_INTERNAL_PRELUDE) {
        mangle_overloaded_operator_function_names(fn);
      }
    }
  }

  if (!analyzeAST) { // Cannot resolve overloaded operators without analysis
    return;
  }

  if (typeid(*expr) != typeid(BinOp)) {
    return;
  }

  BinOp* op = dynamic_cast<BinOp*>(expr);

  if (!op) {
    INT_FATAL(expr, "Unable to dynamically cast BinOp");
  }

  Vec<FnSymbol*> fns;
  if (op->resolved)
    fns.add(op->resolved);
  else
    call_info(op, fns);
  if (fns.n != 1) {
    if (fns.n == 0) {
      INT_FATAL(expr, "Operator has no function");
    } else {
      INT_FATAL(expr, "Trouble resolving operator");
    }
  } else {
    Pragma* pr = fns.e[0]->defPoint->parentStmt->pragmas;
    while (pr) {
      if (!strcmp(pr->str, "builtin")) {
        return;
      }
      pr = dynamic_cast<Pragma *>(pr->next);
    }
    
    Expr* args = op->left->copy();
    args->append(op->right->copy());
    FnCall* fn = new FnCall(new Variable(fns.e[0]), args);
    expr->replace(fn);
  }
}
