#include <typeinfo>
#include "driver.h"
#include "expr.h"
#include "misc.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"
#include "yy.h"
#include "runtime.h"


SymScope* rootScope = NULL;

Vec<ModuleSymbol*> allModules;  // Contains all modules
Vec<ModuleSymbol*> userModules; // Contains user modules

void registerModule(ModuleSymbol* mod) {
  switch (mod->modtype) {
  case MOD_USER:
    userModules.add(mod);
  case MOD_STANDARD:
    allModules.add(mod);
    break;
  default:
    INT_FATAL(mod, "Unable to register module");
  }
}


DefExpr*
Symboltable::defineParam(intentTag tag, char* ident, Expr* type, Expr* init, Expr* variable) {
  ArgSymbol* argSymbol = new ArgSymbol(tag, ident, dtUnknown, init, variable);
  if (tag == INTENT_TYPE) {
    char *name = stringcat("__type_variable_", argSymbol->name);
    VariableType* new_type = new VariableType(getMetaType(NULL));
    TypeSymbol* new_type_symbol = new TypeSymbol(name, new_type);
    argSymbol->type = getMetaType(NULL);
    argSymbol->genericSymbol = new_type_symbol;
  } 
#if 0
  else if (tag == INTENT_PARAM) {
    char *name = stringcat("__parameter_", argSymbol->name);
    VarSymbol *varSymbol = new VarSymbol(name, dtUnknown, VAR_NORMAL, VAR_PARAM);
    argSymbol->genericSymbol = varSymbol;
  }
#endif
  return new DefExpr(argSymbol, NULL, type);
}


DefExpr* Symboltable::defineStructType(char* name, // NULL = anonymous
                                       Type* type,
                                       AList<Stmt>* def) {
  ClassType* structType = dynamic_cast<ClassType*>(type);

  if (!structType) {
    INT_FATAL(type, "defineStructType called on non ClassType");
  }

  TypeSymbol* sym = new TypeSymbol(name, structType);
  DefExpr* defExpr = new DefExpr(sym);
  structType->addDeclarations(def);
  return defExpr;
}
