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
  if (VarSymbol* vs = dynamic_cast<VarSymbol*>(sym)) {
    if (vs->type == dtUnknown && !vs->defPoint->exprType) {
      vs->noDefaultInit = true;
    }
  }

  if (TypeSymbol *ts = dynamic_cast<TypeSymbol*>(sym)) {
    if (UserType* userType = dynamic_cast<UserType*>(ts->definition)) {
      if (userType->underlyingType == dtUnknown &&
          userType->typeExpr &&
          userType->typeExpr->typeInfo() != dtUnknown) {
        userType->underlyingType = userType->typeExpr->typeInfo();
        userType->typeExpr = NULL;
        if (userType->defaultExpr) {
          char* temp_name = stringcat("_init_", sym->name, intstring(uid++));
          Type* temp_type = userType;
          Expr *temp_init = userType->defaultExpr->copy();
          Symbol* parent_symbol = sym->defPoint->parentStmt->parentSymbol;
          Symbol* outer_symbol = sym;
          while (dynamic_cast<TypeSymbol*>(parent_symbol)) {
            parent_symbol = parent_symbol->defPoint->parentStmt->parentSymbol;
            outer_symbol = outer_symbol->defPoint->parentStmt->parentSymbol;
          }
          VarSymbol* temp = new VarSymbol(temp_name, temp_type);
          DefExpr* def = new DefExpr(temp, temp_init);
          if (ModuleSymbol* mod = dynamic_cast<ModuleSymbol*>(parent_symbol)) {
            mod->initFn->insertAtHead(new ExprStmt(def));
          } else {
            Stmt* insert_point = outer_symbol->defPoint->parentStmt;
            insert_point->insertBefore(new ExprStmt(def));
          }
          userType->defaultValue = temp;
          userType->defaultExpr = NULL;
          temp->noDefaultInit = true;
        } else if (userType->underlyingType->defaultConstructor) 
          userType->defaultConstructor = userType->underlyingType->defaultConstructor;
        else if (userType->underlyingType->defaultValue)
          userType->defaultValue = userType->underlyingType->defaultValue;
      }
    }
  }
}

