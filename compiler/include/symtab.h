#ifndef _SYMTAB_H_
#define _SYMTAB_H_

#include "chplenum.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "symscope.h"

extern SymScope* rootScope;

extern Vec<ModuleSymbol*> allModules;     // Contains all modules
extern Vec<ModuleSymbol*> codegenModules; // Contains codegened modules
extern Vec<ModuleSymbol*> userModules;    // Contains user modules

void registerModule(ModuleSymbol* mod);

class Symboltable {
 public:
  static void      init(void);

  static void      removeScope(SymScope* scope);
  static SymScope* pushScope(scopeType type, SymScope* parent = NULL);
  static FnSymbol* getCurrentFn(void);

  /** SJD: These are the lookup's to use--
      lookupInScope only checks the passed scope
      lookupFromScope checks the passed scope and then moves up the scopes
  **/
  static Symbol* lookupInScope(char* name, SymScope* scope, Vec<SymScope*>* scopesAlreadyVisited = NULL);
  static Symbol* lookupInFileModuleScope(char* name);
  static Symbol* lookupFromScope(char* name, SymScope* scope);

  static DefExpr* Symboltable::defineParam(intentTag tag, char* ident,
                                           Expr* type, Expr* init, Expr* variable = NULL);
  static PrimitiveType* Symboltable::definePrimitiveType(char* name, char* cname, Symbol* initSymbol = NULL);
  static Type* Symboltable::defineBuiltinType(char* name, char* cname, Type* newType);
  static DefExpr* defineStructType(char* name, Type* type, AList<Stmt>* def);

  static PrimitiveType* createPrimitiveType (char      *name,
                                             char      *cname,
                                             char      *ltype_name = NULL,
                                             char      *ltype_cname = NULL);
};

void setVarSymbolAttributes(AList<Stmt>* stmts,
                            varType vartag,
                            consType constag);

#endif
