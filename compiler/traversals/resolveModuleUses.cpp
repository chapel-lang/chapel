#include "expr.h"
#include "moduleList.h"
#include "resolveModuleUses.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"
#include "symscope.h"

void ResolveModuleUses::preProcessStmt(Stmt* stmt) {
  SymScope* saveScope = NULL;

  if (UseStmt* useStmt = dynamic_cast<UseStmt*>(stmt)) {
    ModuleSymbol* module = useStmt->getModule();

    if (!module) {
      INT_FATAL(stmt, "UseStmt has no module");
    }

    if (Symboltable::getCurrentModule()->initFn == useStmt->parentSymbol) {
      saveScope = 
        Symboltable::setCurrentScope(Symboltable::getCurrentModule()->modScope);
    }

    forv_Vec(Symbol, sym, module->modScope->symbols) {
      if (!dynamic_cast<ForwardingSymbol*>(sym)) {
        new ForwardingSymbol(sym);
      }
    }

    FnCall* callInitFn = new FnCall(new Variable(module->initFn));
    useStmt->insertBefore(new ExprStmt(callInitFn));
    module->usedBy.add(Symboltable::getCurrentScope());
    Symboltable::getCurrentModule()->uses.add(module);

    if (saveScope) {
      Symboltable::setCurrentScope(saveScope);
    }
  }
}

void ResolveModuleUses::run(ModuleList* moduleList) {
  for (ModuleSymbol* mod = moduleList->first(); mod; mod = moduleList->next()) {
    if (mod->modtype == MOD_USER) {
      mod->initFn->body->body->insertAtHead(
        new UseStmt(
          new Variable(
            new UnresolvedSymbol("_chpl_complex"))));
      if (analyzeAST) {
        if (!_dtinteger_IndexType_switch)
          mod->initFn->body->body->insertAtHead(
            new UseStmt(
              new Variable(
                new UnresolvedSymbol("_chpl_htuple"))));
        mod->initFn->body->body->insertAtHead(
          new UseStmt(
            new Variable(
              new UnresolvedSymbol("_chpl_seq"))));
      }
    }
  }
  Traversal::run(moduleList);
}
