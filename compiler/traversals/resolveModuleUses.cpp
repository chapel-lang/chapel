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


static ExprStmt* genModuleUse(char* moduleName) {
  Expr* moduleUse = new UseExpr(new Variable(new UnresolvedSymbol(moduleName)));
  return new ExprStmt(moduleUse);

}


void ResolveModuleUses::run(ModuleList* moduleList) {
  for (ModuleSymbol* mod = moduleList->first(); mod; mod = moduleList->next()) {
    ExprStmt* moduleUse;
    if (mod->modtype == MOD_USER) {
      moduleUse = genModuleUse("_chpl_complex");
      mod->initFn->body->body->insertAtHead(moduleUse);

      moduleUse = genModuleUse("_chpl_file");
      mod->initFn->body->body->insertAtHead(moduleUse);
      
      if (analyzeAST) {
        moduleUse = genModuleUse("_chpl_htuple");
        mod->initFn->body->body->insertAtHead(moduleUse);

        moduleUse = genModuleUse("_chpl_seq");
        mod->initFn->body->body->insertAtHead(moduleUse);
      }
    }
  }
  Traversal::run(moduleList);
}
