#ifndef _SYMTAB_H_
#define _SYMTAB_H_

#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "symscope.h"

extern ModuleSymbol* commonModule;

class Symboltable {
 public:
  static void init(void);
  static void parseInternalPrelude(void);
  static void parsePrelude(void);
  static void doneParsingPreludes(void);
  static void doneParsingUserFiles(void);
  static bool parsingUserCode(void);

  static void traverse(SymtabTraversal* traversal);
  static void traverseFromScope(SymtabTraversal* traversal,
				SymScope* scope);

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
  static VarSymbol* Symboltable::defineVars(Symbol* idents, Type* type, 
					    Expr* init = nilExpr, 
					    varType vartag = VAR_NORMAL, 
					    bool isConst = false);
  static ParamSymbol* Symboltable::defineParams(paramType tag, Symbol* syms,
						Type* type);
  static VarDefStmt* Symboltable::defineVarDefStmt(Symbol* idents, Type* type, 
						   Expr* init, varType vartag, 
						   bool isConst);
  static VarDefStmt* Symboltable::defineVarDefStmt1(Symbol* idents,
						    Type* type, 
						    Expr* init);
  static VarDefStmt* Symboltable::defineVarDefStmt2(VarDefStmt* stmts,
						    varType vartag, 
						    bool isConst);
  static VarDefStmt* Symboltable::defineSingleVarDefStmt(char* name, 
							 Type* type,
							 Expr* init,
							 varType vartag,
							 bool isConst,
							 Symbol** newVar);
  static ForallExpr* startForallExpr(Expr* domainExpr, 
				     Expr* indexExpr = nilExpr);
  static ForallExpr* finishForallExpr(ForallExpr* indexExpr, 
				      Expr* argExpr = nilExpr);
  // REPLACED  static EnumSymbol* Symboltable::defineEnumList(Symbol* symList);
  static Type* Symboltable::defineBuiltinType(char* name, char* cname, Expr* init);
  static FnSymbol* startFnDef(FnSymbol* fnsym);
  static FnDefStmt* finishFnDef(FnSymbol* fnsym, Symbol* formals, 
				Type* retType, Stmt* body, 
				bool isExtern = false);
  static FnDefStmt* defineFunction(char* name, Symbol* formals, Type* retType, 
				   Stmt* body, bool isExtern = false);

  static TypeSymbol* startClassDef(char* name, bool isValueClass, bool isUnion);
  static TypeDefStmt* finishClassDef(TypeSymbol* classSym, Stmt* definition);

  static ForLoopStmt* startForLoop(bool forall, Symbol* indices, Expr* domain);
  static ForLoopStmt* finishForLoop(ForLoopStmt* forstmt, Stmt* body);

  static ForallExpr* defineQueryDomain(char* name);

  static void print(FILE* outfile = stderr);
  static void dump(FILE* outfile = stderr);
};

#endif
