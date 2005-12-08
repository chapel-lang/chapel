#include "processImportExprs.h"
#include "expr.h"
#include "symtab.h"


void ProcessImportExprs::postProcessExpr(Expr* expr) {
  ImportExpr* importExpr = dynamic_cast<ImportExpr*>(expr);

  if (!importExpr) {
    return;
  }

  if (importExpr->importTag == IMPORT_WITH) {
    if (TypeSymbol* symType = dynamic_cast<TypeSymbol*>(importExpr->parentSymbol)) {
      if (ClassType* class_type = dynamic_cast<ClassType*>(symType->definition)) {
        AList<Stmt>* with_declarations = importExpr->getStruct()->declarationList->copy();
        class_type->addDeclarations(with_declarations, importExpr->parentStmt);
        importExpr->parentStmt->remove();
        return;
      }
    }
    USR_FATAL(expr, "Cannot find ClassType in ImportExpr");
  }

  if (importExpr->importTag == IMPORT_USE) {
    ModuleSymbol* module = importExpr->getImportedModule();
    if (!module) {
      INT_FATAL(expr, "ImportExpr has no module");
    }
    CallExpr* callInitFn = new CallExpr(module->initFn);
    importExpr->parentStmt->insertBefore(new ExprStmt(callInitFn));
    importExpr->parentScope->uses.add(module);
    importExpr->parentStmt->remove();
  }
}


static ExprStmt* genModuleUse(char* moduleName) {
  Expr* moduleUse = new ImportExpr(IMPORT_USE, new SymExpr(new UnresolvedSymbol(moduleName)));
  return new ExprStmt(moduleUse);

}


void ProcessImportExprs::run(Vec<ModuleSymbol*>* modules) {
  forv_Vec(ModuleSymbol, mod, *modules) {
    ExprStmt* moduleUse;
    if ((mod->modtype == MOD_USER || (no_infer && mod->modtype == MOD_COMMON))) {
      if (!fnostdincs) {
        moduleUse = genModuleUse("_chpl_standard");
        mod->initFn->insertAtHead(moduleUse);
      } else {
        mod->initFn->insertAtHead(genModuleUse("_chpl_file"));
      }
    }
  }
  Traversal::run(modules);
}


void processImportExprs(void) {
  Pass* pass = new ProcessImportExprs();
  pass->run(Symboltable::getModules(pass->whichModules));
}
