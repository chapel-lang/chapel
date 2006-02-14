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


Stmt* inline_call(CallExpr* call) {
  FnSymbol* fn = call->findFnSymbol();
  ASTMap map;
  mapFormalsToActuals(call, &map);
  BlockStmt* inlined_body = fn->body->copy(&map);
  if (fn->retType == dtVoid) {
    call->parentStmt->insertBefore(inlined_body);
    ReturnStmt* retStmt = dynamic_cast<ReturnStmt*>(inlined_body->body->last());
    retStmt->remove();
    call->parentStmt->remove();
  } else {
    char* temp_name = stringcat("_inline_", fn->cname);
    VarSymbol* temp = new VarSymbol(temp_name, fn->retType);
    temp->noDefaultInit = true;
    call->parentStmt->insertBefore(new DefExpr(temp));
    call->parentStmt->insertBefore(inlined_body);
    ReturnStmt* retStmt = dynamic_cast<ReturnStmt*>(inlined_body->body->last());
    Expr* retVal = retStmt->expr;
    retVal->remove();
    retStmt->replace(new ExprStmt(new CallExpr(PRIMITIVE_MOVE, temp, retVal)));
    call->replace(new SymExpr(temp));
  }
  return inlined_body;
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
      Stmt* stmt = inline_call(call);
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
