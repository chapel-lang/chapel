#include "expr.h"
#include "moduleList.h"
#include "resolveModuleUses.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"
#include "symscope.h"


void ResolveModuleUses::preProcessExpr(Expr* expr) {
 SymScope* saveScope = NULL;

 if (UseExpr* useExpr = dynamic_cast<UseExpr*>(expr)) {
    ModuleSymbol* module = useExpr->getModule();

    if (!module) {
      INT_FATAL(expr, "UseExpr has no module");
    }

    if (Symboltable::getCurrentModule()->initFn == useExpr->parentSymbol) {
      saveScope = 
        Symboltable::setCurrentScope(Symboltable::getCurrentModule()->modScope);
    }

    FnCall* callInitFn = new FnCall(new Variable(module->initFn));
    useExpr->parentStmt->insertBefore(new ExprStmt(callInitFn));
    Symboltable::getCurrentScope()->uses.add(module);

    if (saveScope) {
      Symboltable::setCurrentScope(saveScope);
    }

  }
}


void ResolveModuleUses::run(ModuleList* moduleList) {
  for (ModuleSymbol* mod = moduleList->first(); mod; mod = moduleList->next()) {
    if (mod->modtype == MOD_USER) {
      mod->initFn->body->body->insertAtHead(
        new ExprStmt(
          new UseExpr(
            new Variable(
              new UnresolvedSymbol("_chpl_complex")))));

      mod->initFn->body->body->insertAtHead(
        new ExprStmt(
          new UseExpr(
            new Variable(
              new UnresolvedSymbol("_chpl_file")))));

      if (analyzeAST) {
        mod->initFn->body->body->insertAtHead(
          new ExprStmt(
            new UseExpr(
              new Variable(
                new UnresolvedSymbol("_chpl_htuple")))));
        mod->initFn->body->body->insertAtHead(
          new ExprStmt(
            new UseExpr(
              new Variable(
                new UnresolvedSymbol("_chpl_seq")))));
      }
    }
  }
  Traversal::run(moduleList);
}
