#include "processImportExprs.h"
#include "expr.h"
#include "symtab.h"
#include "runtime.h"


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
    if (module != compilerModule)
      importExpr->parentStmt->insertBefore(new CallExpr(module->initFn));
    importExpr->parentScope->uses.add(module);
    importExpr->parentStmt->remove();
  }
}


void ProcessImportExprs::run(Vec<ModuleSymbol*>* modules) {
  Traversal::run(modules);
}


void processImportExprs(void) {
  Pass* pass = new ProcessImportExprs();
  pass->run(Symboltable::getModules(pass->whichModules));
}
