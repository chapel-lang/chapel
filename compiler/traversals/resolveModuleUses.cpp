#include "resolveModuleUses.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"
#include "symscope.h"
#include "../symtab/symlink.h"

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

    for (SymLink* symLink = module->modScope->firstSym;
         symLink;
         symLink = nextLink(SymLink, symLink)) {
      for (Symbol* sym = symLink->pSym; sym; sym = nextLink(Symbol, sym)) {
        if (!dynamic_cast<ForwardingSymbol*>(sym)) {
          new ForwardingSymbol(sym);
        }
      }
    }

    FnCall* callInitFn = new FnCall(new Variable(module->initFn), NULL);
    useStmt->insertBefore(new ExprStmt(callInitFn));
    module->usedBy.add(Symboltable::getCurrentScope());
    Symboltable::getCurrentModule()->uses.add(module);

    if (saveScope) {
      Symboltable::setCurrentScope(saveScope);
    }
  }
}

void ResolveModuleUses::run(ModuleSymbol* moduleList) {
  for (ModuleSymbol* mod = moduleList; mod; mod = nextLink(ModuleSymbol, mod)) {
    if (mod->modtype == MOD_USER) {
      mod->initFn->body->body->insertBefore(
        new UseStmt(
          new Variable(
            new UnresolvedSymbol("_chpl_complex"))));
    }
  }
  Traversal::run(moduleList);
}
