#ifndef _SYMTAB_H_
#define _SYMTAB_H_

#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "symscope.h"

extern ModuleSymbol* commonModule;
extern SymScope* internalScope;

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

  static Expr* startLetExpr(void);
  static Expr* finishLetExpr(Expr* let_expr, DefExpr* exprs, Expr* inner_expr);
  static BlockStmt* startCompoundStmt(void);
  static BlockStmt* finishCompoundStmt(BlockStmt* blkstmt, Stmt* body);
  static ModuleSymbol* startModuleDef(char* name, bool internal = false);
  static DefExpr* finishModuleDef(ModuleSymbol* mod, Stmt* definition);
  static VarSymbol* Symboltable::defineVars(Symbol* idents, Type* type, 
					    Expr* init = NULL, 
					    varType vartag = VAR_NORMAL, 
					    consType constag = VAR_VAR);
  static ParamSymbol* Symboltable::defineParams(paramType tag, Symbol* syms,
						Type* type, Expr* init);
	
  static DefExpr* Symboltable::defineVarDef(Symbol* idents, Type* type, 
						Expr* init, varType vartag, 
						consType consTag);
  static DefExpr* Symboltable::defineVarDef1(Symbol* idents,
						 Type* type, 
						 Expr* init);
  static DefExpr* Symboltable::defineVarDef2(DefExpr* stmts,
						 varType vartag, 
						 consType constag);

  static DefStmt* Symboltable::defineSingleVarDefStmt(char* name, 
						      Type* type,
						      Expr* init,
						      varType vartag,
						      consType constag);
  static ForallExpr* startForallExpr(Expr* domainExpr, 
				     Expr* indexExpr = NULL);
  static ForallExpr* finishForallExpr(ForallExpr* indexExpr, 
				      Expr* argExpr = NULL);
  // REPLACED  static EnumSymbol* Symboltable::defineEnumList(Symbol* symList);
  static Type* Symboltable::defineBuiltinType(char* name, char* cname, Expr* init);
  static FnSymbol* startFnDef(FnSymbol* fnsym);
  static DefExpr* finishFnDef(FnSymbol* fnsym, Symbol* formals, 
			      Type* retType, Stmt* body, 
			      bool isExtern = false);
  static DefStmt* defineFunction(char* name, Symbol* formals, Type* retType, 
				 Stmt* body, bool isExtern = false);

  static TypeSymbol* startClassDef(char* name, bool isValueClass, bool isUnion);
  static DefExpr* finishClassDef(TypeSymbol* classSym, Stmt* definition);

  static ForLoopStmt* startForLoop(bool forall, Symbol* indices, Expr* domain);
  static ForLoopStmt* finishForLoop(ForLoopStmt* forstmt, Stmt* body);

  static ForallExpr* defineQueryDomain(char* name);

  static void print(FILE* outfile = stderr);
  static void dump(FILE* outfile = stderr);
};

#endif
