#include "insertDefaultInitVariables.h"
#include "symtab.h"
#include "symscope.h"
#include "stmt.h"
#include "expr.h"
#include "type.h"
#include "stringutil.h"


InsertDefaultInitVariables::InsertDefaultInitVariables() {
  whichModules = MODULES_USER;
}


void InsertDefaultInitVariables::processSymbol(Symbol* sym) {
  static int uid = 1;
  if (dynamic_cast<TypeSymbol*>(sym)) {
    if (sym->type->defaultVal) {
      //      SymScope* saveScope;
      DefStmt* def_stmt;
      char* temp_name = glomstrings(3, "_init_", sym->name, intstring(uid++));
      Type* temp_type = sym->type;
      Expr* temp_init = sym->type->defaultVal->copy();

      /*
      if (ModuleSymbol* mod =
          dynamic_cast<ModuleSymbol*>(sym->defPoint->stmt->parentSymbol)) {
        BlockStmt* block_stmt = dynamic_cast<BlockStmt*>(mod->initFn->body);
        if (block_stmt->blkScope) {
          saveScope = Symboltable::setCurrentScope(block_stmt->blkScope);
        } else {
          Symboltable::pushScope(SCOPE_FUNCTION);
        }
        def_stmt = Symboltable::defineSingleVarDefStmt(temp_name,
                                                       temp_type,
                                                       temp_init,
                                                       VAR_NORMAL,
                                                       VAR_VAR);
        if (block_stmt->blkScope) {
          Symboltable::setCurrentScope(saveScope);
        } else {
          block_stmt->blkScope = Symboltable::popScope();
        }
      } else {
      */
        def_stmt = Symboltable::defineSingleVarDefStmt(temp_name,
                                                       temp_type,
                                                       temp_init,
                                                       VAR_NORMAL,
                                                       VAR_VAR);
        //}
      Stmt* insert_point;
      if (ModuleSymbol* mod =
          dynamic_cast<ModuleSymbol*>(sym->defPoint->stmt->parentSymbol)) {
        insert_point = dynamic_cast<BlockStmt*>(mod->initFn->body)->body;
      } else {
        insert_point = sym->defPoint->stmt;
      }
      insert_point->insertBefore(def_stmt);
      sym->type->defaultVal->replace(new Variable(def_stmt->varDef()));
    }
  }
}
