#include "insertDefaultInitVariables.h"
#include "symtab.h"
#include "symscope.h"
#include "stmt.h"
#include "expr.h"
#include "type.h"
#include "stringutil.h"


InsertDefaultInitVariables::InsertDefaultInitVariables() {
  whichModules = MODULES_COMMON_AND_USER;
}


void InsertDefaultInitVariables::processSymbol(Symbol* sym) {
  static int uid = 1;
  if (dynamic_cast<TypeSymbol*>(sym)) {
    if (dynamic_cast<ArrayType*>(sym->type)) { // bail on array types
      if (sym->type->defaultVal) {
        sym->type->defaultVal->extract();
        sym->type->defaultVal = NULL;
      }
      return; // they don't get default initialized yet
      // the problem with not bailing is that the domain may not be
      // declared where the default initial value is setup like in the
      // common module.
    }
    if (sym->type->defaultVal) {
      char* temp_name = glomstrings(3, "_init_", sym->name, intstring(uid++));
      Type* temp_type = sym->type;
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
      Stmt* insert_point;
      if (ModuleSymbol* mod = dynamic_cast<ModuleSymbol*>(parent_symbol)) {
        insert_point = dynamic_cast<BlockStmt*>(mod->initFn->body)->body;
      } else {
        insert_point = outer_symbol->defPoint->parentStmt;
      }
      insert_point->insertBefore(def_stmt);
      sym->type->defaultVal->replace(new Variable(def_stmt->varDef()));
      def_stmt->varDef()->noDefaultInit = true;
    }
  }
}
