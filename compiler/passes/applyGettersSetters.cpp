#include "applyGettersSetters.h"
#include "astutil.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"


#define REPLACE(_x) do { \
      BaseAST* replacement = _x; \
      ast->replace(replacement); \
      ast = replacement; \
      goto Ldone; \
} while (0)

void 
apply_getters_setters(BaseAST* ast) {
  // Most generally:
  //   x.f(1) += y ---> f(x, _mt, 1, _st, +(f(x,_mt, 1), y)
  // or
  //   CallExpr(+=, CallExpr(MemberAccess(x, "f"), 1), y) --->
  //     CallExpr("f", x, _mt, 1, _st, CallExpr("+", CallExpr("f", x, _mt, 1), y))
  // though, it could be a just = (vs +=) 
  //         or a simple MemberAccess without CallExpr
  //         or a CallExpr without a MemberAccess
  CallExpr *call = dynamic_cast<CallExpr*>(ast), *assign = 0;
  if (call && OP_ISASSIGNOP(call->opTag)) {
    SymExpr* symExpr = dynamic_cast<SymExpr*>(call->baseExpr);
    if (!symExpr || strcmp(symExpr->var->name, "_move")) {
      assign = call;
      call = dynamic_cast<CallExpr*>(assign->get(1));
    }
  }
  BaseAST *base = call ? call->baseExpr : (assign ? assign->get(1) : ast);
  if (MemberAccess* memberAccess = dynamic_cast<MemberAccess*>(base)) {
    Expr *rhs = assign ? assign->argList->get(2)->copy() : 0;
    // build the main accessor/setter
    if ((call && !call->argList->isEmpty())) {
      CallExpr *lhs = new CallExpr(memberAccess->member->name, 
                                   new SymExpr(Symboltable::lookupInternal("_methodToken")),
                                   memberAccess->base->copy());
      lhs->partialTag = PARTIAL_OK;
      AList<Expr>* arguments = call ? call->argList->copy() : new AList<Expr>;
      if (rhs) {
        arguments->insertAtTail(new SymExpr(Symboltable::lookupInternal("_setterToken")));
        arguments->insertAtTail(rhs);
      }
      lhs = new CallExpr(lhs, arguments);
      REPLACE(lhs);
    } else {
      AList<Expr>* arguments = call ? call->argList->copy() : new AList<Expr>;
      arguments->insertAtHead(memberAccess->base->copy());
      arguments->insertAtHead(new SymExpr(Symboltable::lookupInternal("_methodToken")));
      if (rhs) {
        arguments->insertAtTail(new SymExpr(Symboltable::lookupInternal("_setterToken")));
        arguments->insertAtTail(rhs);
      }
      REPLACE(new CallExpr(memberAccess->member->name, arguments));
    }
  } 
  if (assign) {
    Expr *rhs = assign->argList->get(2)->copy();
    if (call) {
      AList<Expr>* arguments = call->argList->copy();
      arguments->insertAtTail(new SymExpr(Symboltable::lookupInternal("_setterToken")));
      arguments->insertAtTail(rhs);
      REPLACE(new CallExpr(call->baseExpr->copy(), arguments));
    } else
      REPLACE(new CallExpr(OP_GETS, assign->argList->get(1)->copy(), rhs));
  } 
 Ldone:
  // top down, on the modified AST
  Vec<BaseAST *> asts;
  get_ast_children(ast, asts);
  forv_BaseAST(a, asts)
    apply_getters_setters(a);
}

void applyGettersSetters(void) {
  Vec<FnSymbol*> fns;
  collect_functions(&fns);
  forv_Vec(FnSymbol, fn, fns) {
    if (!(fn->_setter || fn->_getter || (!no_infer && fn->fnClass == FN_CONSTRUCTOR))) {
      apply_getters_setters(fn->body);
    }
  }
}
