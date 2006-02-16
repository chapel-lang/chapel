#include <stdio.h>
#include <typeinfo>
#include "inlineFunctions.h"
#include "stringutil.h"
#include "symtab.h"


static void mapFormalsToActuals(CallExpr* call, ASTMap* map) {
  FnSymbol* fn = call->findFnSymbol();
  Expr* actual = call->argList->first();
  for_alist(DefExpr, formalDef, fn->formals) {
    ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
    if (formal->isRef()) {
      if (SymExpr* symExpr = dynamic_cast<SymExpr*>(actual)) {
        map->put(formal, symExpr->var);
      } else {
        INT_FATAL("Illegal reference actual encountered in inlining");
      }
    } else {
      char* temp_name =  stringcat("_inline_temp_", formal->cname);
      VarSymbol* temp = new VarSymbol(temp_name, actual->typeInfo());
      temp->noDefaultInit = true;
      call->parentStmt->insertBefore(new DefExpr(temp));
      call->parentStmt->insertBefore
        (new CallExpr(PRIMITIVE_MOVE, temp, actual->copy()));
      map->put(formal, temp);
    }
    actual = call->argList->next();
  }
}


void inline_call(CallExpr* call, Vec<Stmt*>* stmts) {
  FnSymbol* fn = call->findFnSymbol();
  ASTMap map;
  mapFormalsToActuals(call, &map);
  AList<Stmt>* fn_body = fn->body->body->copy(&map);
  ReturnStmt* return_stmt = dynamic_cast<ReturnStmt*>(fn_body->last());
  if (!return_stmt)
    INT_FATAL(call, "Cannot inline function, function is not normalized");
  Expr* return_value = return_stmt->expr;
  return_stmt->remove();
  for_alist(Stmt, stmt, fn_body)
    stmts->add(stmt);
  call->parentStmt->insertBefore(fn_body);
  if (fn->retType == dtVoid)
    call->parentStmt->remove();
  else
    call->replace(return_value);
}

void inline_calls(BaseAST* base, Vec<FnSymbol*>* inline_stack = NULL) {
  Vec<BaseAST*> asts;
  collect_asts_postorder(&asts, base);
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
      if (call->isPrimitive() || !call->parentStmt)
        continue;
      FnSymbol* fn = call->findFnSymbol();
      if (!fn || !fn->hasPragma("inline") || fn->hasPragma("no codegen"))
        continue;
      if (!inline_stack)
        inline_stack = new Vec<FnSymbol*>();
      if (inline_stack->in(fn))
        INT_FATAL(fn, "Recursive inlining detected");
      inline_stack->add(fn);
      Vec<Stmt*> stmts;
      inline_call(call, &stmts);
      forv_Vec(Stmt, stmt, stmts)
        inline_calls(stmt, inline_stack);
      inline_stack->pop();
      if (report_inlining)
        printf("chapel compiler: reporting inlining"
               ", %s function was inlined\n", fn->cname);
    }
  }
}

void inlineFunctions(void) {
  if (no_inline)
    return;
  forv_Vec(ModuleSymbol, mod, allModules)
    inline_calls(mod);
}
