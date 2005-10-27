#ifndef _SYMTAB_H_
#define _SYMTAB_H_

#include "chplenum.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "symscope.h"

extern SymScope* rootScope;
extern SymScope* preludeScope;
extern ModuleSymbol* commonModule;

class Symboltable {
 public:
  static void init(void);
  static void parsePrelude(void);
  static void doneParsingPreludes(void);
  static void doneParsingUserFiles(void);
  static bool parsingUserCode(void);

  static void traverse(SymtabTraversal* traversal);
  static void traverseFromScope(SymtabTraversal* traversal,
                                SymScope* scope);

  static void removeScope(SymScope* scope);
  static void pushScope(scopeType type);
  static SymScope* popScope(void);
  static SymScope* getCurrentScope(void);
  static SymScope* setCurrentScope(SymScope* newScope);
  static ModuleSymbol* getCurrentModule(void);
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
  static FnSymbol* startFnDef(FnSymbol* fnsym);
  static void continueFnDef(FnSymbol* fnsym, AList<DefExpr>* formals, 
                            Type* retType, bool isRef = false, Expr *whereExp = NULL);
  static FnSymbol* finishFnDef(FnSymbol* fnsym, BlockStmt* body);
  static DefExpr* defineFunction(char* name, AList<DefExpr>* formals, 
                                 Type* retType, BlockStmt* body);

  static DefExpr* defineStructType(char* name, Type* type, AList<Stmt>* def);

  static void print(FILE* outfile = stderr);
  static void dump(FILE* outfile = stderr);
};

bool ModuleDefContainsOnlyNestedModules(AList<Stmt>* def);

void setVarSymbolAttributes(AList<Stmt>* stmts,
                            varType vartag,
                            consType constag);

#endif
