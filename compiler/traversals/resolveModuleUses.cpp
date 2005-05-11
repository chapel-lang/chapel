#include "resolveModuleUses.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"
#include "symscope.h"
#include "../symtab/symlink.h"

void ResolveModuleUses::preProcessStmt(Stmt* stmt) {
  if (UseStmt* useStmt = dynamic_cast<UseStmt*>(stmt)) {
    ModuleSymbol* module = useStmt->getModule();

    if (!module) {
      INT_FATAL(stmt, "UseStmt has no module");
    }

    for (SymLink* symLink = module->modScope->firstSym;
         symLink;
         symLink = nextLink(SymLink, symLink)) {
      new ForwardingSymbol(symLink->pSym);
    }

    ModuleSymbol* currentModule = Symboltable::getCurrentModule();
    currentModule->initFn->body->body->insertBefore(new ExprStmt(new FnCall(new Variable(module->initFn), NULL)));
    currentModule->uses.add(module);
  }
}
