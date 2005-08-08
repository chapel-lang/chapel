#include "processImportExprs.h"
#include "expr.h"
#include "moduleList.h"
#include "symtab.h"


void ProcessImportExprs::postProcessExpr(Expr* expr) {
  ImportExpr* importExpr = dynamic_cast<ImportExpr*>(expr);

  if (!importExpr) {
    return;
  }

  if (importExpr->importTag == IMPORT_WITH) {
    if (TypeSymbol* symType = dynamic_cast<TypeSymbol*>(importExpr->parentSymbol)) {
      if (ClassType* class_type = dynamic_cast<ClassType*>(symType->definition)) {
        AList<Stmt>* with_declarations = importExpr->getStruct()->declarationList->copy(true);
        class_type->addDeclarations(with_declarations, importExpr->parentStmt);
        return;
      }
    }
    USR_FATAL(expr, "Cannot find ClassType in ImportExpr");
  }

  if (importExpr->importTag == IMPORT_USE) {
    SymScope* saveScope = NULL;
    ModuleSymbol* module = importExpr->getModule();
    if (!module) {
      INT_FATAL(expr, "ImportExpr has no module");
    }
    if (Symboltable::getCurrentModule()->initFn == importExpr->parentSymbol) {
      saveScope = 
        Symboltable::setCurrentScope(Symboltable::getCurrentModule()->modScope);
    }
    CallExpr* callInitFn = new CallExpr(new Variable(module->initFn));
    importExpr->parentStmt->insertBefore(new ExprStmt(callInitFn));
    Symboltable::getCurrentScope()->uses.add(module);
    if (saveScope) {
      Symboltable::setCurrentScope(saveScope);
    }
  }
}


static ExprStmt* genModuleUse(char* moduleName) {
  Expr* moduleUse = new ImportExpr(IMPORT_USE, new Variable(new UnresolvedSymbol(moduleName)));
  return new ExprStmt(moduleUse);

}


void ProcessImportExprs::run(ModuleList* moduleList) {
  for_alist(ModuleSymbol, mod, moduleList) {
    ExprStmt* moduleUse;
    if (mod->modtype == MOD_USER) {
      moduleUse = genModuleUse("_chpl_complex");
      mod->initFn->body->body->insertAtHead(moduleUse);
      moduleUse = genModuleUse("_chpl_file");
      mod->initFn->body->body->insertAtHead(moduleUse);
      moduleUse = genModuleUse("_chpl_htuple");
      mod->initFn->body->body->insertAtHead(moduleUse);
      moduleUse = genModuleUse("_chpl_seq");
      mod->initFn->body->body->insertAtHead(moduleUse);
    }
  }
  Traversal::run(moduleList);
}
