#ifndef _SYMTAB_H_
#define _SYMTAB_H_

#include "chplenum.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "symscope.h"

extern SymScope* rootScope;
extern SymScope* preludeScope;

extern Vec<ModuleSymbol*> allModules;     // Contains all modules
extern Vec<ModuleSymbol*> codegenModules; // Contains codegened modules
extern Vec<ModuleSymbol*> userModules;    // Contains user modules

class Symboltable {
 public:
  static void init(void);
  static void parsePrelude(void);
  static void doneParsingPreludes(void);
  static void doneParsingUserFiles(void);
  static bool parsingUserCode(void);

  static void removeScope(SymScope* scope);
  static void pushScope(scopeType type);
  static SymScope* popScope(void);
  static SymScope* getCurrentScope(void);
  static SymScope* setCurrentScope(SymScope* newScope);
  static Vec<ModuleSymbol*>* getModules(moduleSet modules);
  static FnSymbol* getCurrentFn(void);

  /** SJD: These are the lookup's to use--
      lookupInScope only checks the passed scope
      lookupFromScope checks the passed scope and then moves up the scopes
  **/
  static Symbol* lookupInScope(char* name, SymScope* scope, Vec<SymScope*>* scopesAlreadyVisited = NULL);
  static Symbol* lookupInFileModuleScope(char* name);
  static Symbol* lookupFromScope(char* name, SymScope* scope);
  static Symbol* lookupInCurrentScope(char* name);
  static Symbol* lookup(char* name);

  static Symbol* lookupInternal(char* name, 
                                scopeType scope = SCOPE_PRELUDE);
  static TypeSymbol* lookupInternalType(char* name);

  static ModuleSymbol* startModuleDef(char* name, modType modtype = MOD_USER);
  static DefExpr* finishModuleDef(ModuleSymbol* mod, AList<Stmt>* def);
  static DefExpr* Symboltable::defineParam(intentTag tag, char* ident,
                                           Expr* type, Expr* init);
  static PrimitiveType* Symboltable::definePrimitiveType(char* name, char* cname, Symbol* initSymbol = NULL);
  static Type* Symboltable::defineBuiltinType(char* name, char* cname, Type* newType);
  static DefExpr* defineStructType(char* name, Type* type, AList<Stmt>* def);
};

bool ModuleDefContainsOnlyNestedModules(AList<Stmt>* def);

void setVarSymbolAttributes(AList<Stmt>* stmts,
                            varType vartag,
                            consType constag);

#endif
