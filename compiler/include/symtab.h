#ifndef _SYMTAB_H_
#define _SYMTAB_H_

#include "chplenum.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "symscope.h"

extern SymScope* rootScope;

extern Vec<ModuleSymbol*> allModules;     // Contains all modules
extern Vec<ModuleSymbol*> userModules;    // Contains user modules

void registerModule(ModuleSymbol* mod);

class Symboltable {
 public:
  static DefExpr* Symboltable::defineParam(intentTag tag, char* ident,
                                           Expr* type, Expr* init, Expr* variable = NULL);
  static DefExpr* defineStructType(char* name, Type* type, AList<Stmt>* def);
};

void setVarSymbolAttributes(AList<Stmt>* stmts,
                            varType vartag,
                            consType constag);

#endif
