#include <stdio.h>
#include <typeinfo>
#include "inlineFunctions.h"
#include "stringutil.h"
#include "symtab.h"


class ReplaceReturns : public Traversal {
 public:
  FnSymbol* fn;
  Symbol* sym;

  ReplaceReturns(FnSymbol* iFn, Symbol* iSym = NULL) : fn(iFn), sym(iSym) { }

  void ReplaceReturns::postProcessStmt(Stmt* stmt) {
    if (ReturnStmt* s = dynamic_cast<ReturnStmt*>(stmt)) {
      if (s->getFunction() == fn) {
        if (sym) {
          Expr* ret = s->expr;
          ret->remove();
          s->replace(new ExprStmt(new CallExpr(OP_MOVE, sym, ret)));
        } else
          s->remove();
      }
    }
  }
};


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
      if (no_infer || use_alloc)
        call->parentStmt->insertBefore
          (new CallExpr(OP_MOVE, temp, actual->copy()));
      call->parentStmt->insertBefore
        (new DefExpr(temp, actual->copy()));
      if (formal->isCopyOut())
        call->parentStmt->insertAfter
          (new CallExpr(OP_MOVE, actual->copy(), temp));
      map->put(formal, temp);
    }
    actual = call->argList->next();
  }
}


void InlineFunctions::postProcessExpr(Expr* expr) {
  if (no_inline)
    return;

  CallExpr* call = dynamic_cast<CallExpr*>(expr);

  if (!call || call->isPrimitive() || call->opTag != OP_NONE)
    return;

  if (no_infer && dynamic_cast<DefExpr*>(call->parentExpr))
    return;

  FnSymbol* fn = call->findFnSymbol();

  if (!fn || !fn->hasPragma("inline") || fn->hasPragma("no codegen"))
    return;

  Stmt* stmt = inline_call(call);
  TRAVERSE(stmt, this, true);
  if (report_inlining)
    printf("chapel compiler: reporting inlining"
           ", %s function was inlined\n", fn->cname);
}


Stmt* inline_call(CallExpr* call) {
  FnSymbol* fn = call->findFnSymbol();

  ASTMap map;
  mapFormalsToActuals(call, &map);

  Stmt* inlined_body = fn->body->copy(&map);
  if (fn->retType != dtVoid) {
    char* temp_name = stringcat("_inline_", fn->cname);
    VarSymbol* temp = new VarSymbol(temp_name, fn->retType);
    temp->noDefaultInit = true;
    call->parentStmt->insertBefore(new DefExpr(temp));
    call->parentStmt->insertBefore(inlined_body);
    TRAVERSE(inlined_body, new ReplaceReturns(call->getFunction(), temp), true);
    call->replace(new SymExpr(temp));
  } else {
    call->parentStmt->insertBefore(inlined_body);
    TRAVERSE(inlined_body, new ReplaceReturns(call->getFunction()), true);
    call->parentStmt->remove();
  }
  return inlined_body;
}


void InlineFunctions::run(Vec<ModuleSymbol*>* modules) {
  Traversal::run(modules);
}


void inlineFunctions(void) {
  Pass* pass = new InlineFunctions();
  pass->run(Symboltable::getModules(pass->whichModules));
}
