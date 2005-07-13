#include "insertDefaultInitVariables.h"
#include "symtab.h"
#include "symscope.h"
#include "stmt.h"
#include "expr.h"
#include "type.h"
#include "stringutil.h"


InsertDefaultInitVariables::InsertDefaultInitVariables() {
  whichModules = MODULES_CODEGEN;
}


void InsertDefaultInitVariables::processSymbol(Symbol* sym) {
  static int uid = 1;


  // No type, no default init
  if (VarSymbol* var = dynamic_cast<VarSymbol*>(sym)) {
    if (var->type == dtUnknown) {
      var->noDefaultInit = true;
    }
  }


  if (dynamic_cast<TypeSymbol*>(sym)) {
    if (dynamic_cast<ArrayType*>(sym->type)) { // bail on array types
      if (sym->type->defaultVal) {
        sym->type->defaultVal->remove();
        sym->type->defaultVal = NULL;
      }
      return; // they don't get default initialized yet
      // the problem with not bailing is that the domain may not be
      // declared where the default initial value is setup like in the
      // common module.
    }
    if (dynamic_cast<IndexType*>(sym->type)) { // RED: bail on index types
      if (sym->type->defaultVal) {
        sym->type->defaultVal->remove();
        sym->type->defaultVal = NULL;
      }
      return; // they don't get default initialized yet
      // the problem is that index variables have a type symbol,
      //but not the tuples when the IndexType happens to have a index type tuple
    }
    if (sym->type->defaultVal) {
      char* temp_name = glomstrings(3, "_init_", sym->name, intstring(uid++));
      //RED -- initialization for index types.
      //Type* temp_type = sym->type;
      Type* temp_type = sym->type->getType();
      Expr* temp_init = sym->type->defaultVal->copy();

      Symbol* parent_symbol = sym->defPoint->parentStmt->parentSymbol;
      Symbol* outer_symbol = sym;
      while (dynamic_cast<TypeSymbol*>(parent_symbol)) {
        parent_symbol = parent_symbol->defPoint->parentStmt->parentSymbol;
        outer_symbol = outer_symbol->defPoint->parentStmt->parentSymbol;
      }

      SymScope* saveScope =
        Symboltable::setCurrentScope(outer_symbol->parentScope);
      DefStmt* def_stmt = Symboltable::defineSingleVarDefStmt(temp_name,
                                                              temp_type,
                                                              temp_init,
                                                              VAR_NORMAL,
                                                              VAR_VAR);
      Symboltable::setCurrentScope(saveScope);
      if (ModuleSymbol* mod = dynamic_cast<ModuleSymbol*>(parent_symbol)) {
        mod->initFn->body->body->insertAtHead(def_stmt);
      } else {
        Stmt* insert_point = outer_symbol->defPoint->parentStmt;
        insert_point->insertBefore(def_stmt);
      }
      VarSymbol* var = dynamic_cast<VarSymbol*>(def_stmt->defExpr->sym);
      sym->type->defaultVal->replace(new Variable(var));
      var->noDefaultInit = true;
    }
  }
}
