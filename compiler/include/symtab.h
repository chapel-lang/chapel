#ifndef _SYMTAB_H_
#define _SYMTAB_H_

#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "symscope.h"

class Symboltable {
 public:
  static void init(void);
  static void parseInternalPrelude(void);
  static void parsePrelude(void);
  static void doneParsingPreludes(void);
  static void doneParsingUserFiles(void);
  static bool parsingUserCode(void);

  static void pushScope(scopeType type);
  static SymScope* popScope(void);
  static SymScope* getCurrentScope(void);
  static SymScope* setCurrentScope(SymScope* newScope);
  static ModuleSymbol* getModuleList(void);
  static FnSymbol* getCurrentFn(void);

  static void defineInScope(Symbol* sym, SymScope* scope);
  static void define(Symbol* sym);

  /** SJD: These are the lookup's to use--
      lookupInScope only checks the passed scope
      lookupFromScope checks the passed scope and then moves up the scopes
  **/
  static Symbol* lookupInScope(char* name, SymScope* scope);
  static Symbol* lookupFromScope(char* name, SymScope* scope, 
				 bool genError = false);

  static Symbol* lookupInCurrentScope(char* name);
  static Symbol* lookup(char* name, bool genError = false);

  static Symbol* lookupInternal(char* name, bool publicSym = false);
  static TypeSymbol* lookupInternalType(char* name, bool publicSym = false);

  static BlockStmt* startCompoundStmt(void);
  static BlockStmt* finishCompoundStmt(BlockStmt* blkstmt, Stmt* body);
  static ModuleSymbol* startModuleDef(char* name, bool internal = false);
  static ModuleDefStmt* finishModuleDef(ModuleSymbol* mod, Stmt* definition);
  static TypeDefStmt* defineUserType(char* name, Type* definition, 
				     Expr* initializer = nilExpr);
  static ParamSymbol* defineParams(paramType formaltag, Symbol* idents,
				   Type* type);
  static ParamSymbol* copyParams(ParamSymbol* formals);
  static VarSymbol* Symboltable::defineVars(Symbol* idents, Type* type, 
					    Expr* init = nilExpr, 
					    varType vartag = VAR_NORMAL, 
					    bool isConst = false);
  static VarDefStmt* Symboltable::defineVarDefStmt(Symbol* idents, Type* type, 
						   Expr* init, varType vartag, 
						   bool isConst);
  static ForallExpr* startForallExpr(Expr* domainExpr, 
				     Expr* indexExpr = nilExpr);
  static ForallExpr* finishForallExpr(ForallExpr* indexExpr, 
				      Expr* argExpr = nilExpr);
  // REPLACED  static EnumSymbol* Symboltable::defineEnumList(Symbol* symList);
  static Type* Symboltable::defineBuiltinType(char* name, char* cname, Expr* init);
  static FnSymbol* startFnDef(char* name, bool insert = true);
  static FnDefStmt* finishFnDef(FnSymbol* fnsym, Symbol* formals, 
				Type* retType, Stmt* body, 
				bool isExtern = false);
  static FnDefStmt* defineFunction(char* name, Symbol* formals, Type* retType, 
				   Stmt* body, bool isExtern = false);

  static TypeSymbol* startClassDef(char* name, bool isValueClass);
  static TypeDefStmt* finishClassDef(TypeSymbol* classSym, Stmt* definition);

  static ForLoopStmt* startForLoop(bool forall, Symbol* indices, Expr* domain);
  static ForLoopStmt* finishForLoop(ForLoopStmt* forstmt, Stmt* body);

  static ForallExpr* defineQueryDomain(char* name);

  static void print(FILE* outfile = stderr);
  static void dump(FILE* outfile = stderr);
};

#endif
