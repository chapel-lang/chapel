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

  static void define(Symbol* sym);
  static Symbol* lookupInScope(char* name, SymScope* scope);
  static Symbol* lookupInternal(char* name, bool publicSym = false);
  static TypeSymbol* lookupInternalType(char* name, bool publicSym = false);
  static Symbol* lookup(char* name, bool genError = false, 
			bool inLexer = false);
  static ClassSymbol* lookupClass(char* name);

  static void startCompoundStmt(void);
  static BlockStmt* finishCompoundStmt(Stmt* body);
  static ModuleSymbol* defineModule(char* name, bool internal = false);
  static TypeDefStmt* defineUserType(char* name, Type* definition, 
				     Expr* initializer = nilExpr);
  static ParamSymbol* defineParams(paramType formaltag, Symbol* idents,
				   Type* type);
  static ParamSymbol* copyParams(ParamSymbol* formals);
  static Symbol* Symboltable::exprToIndexSymbols(Expr* expr, Symbol* indices=nilSymbol);
  static VarSymbol* Symboltable::defineVars(Symbol* idents, Type* type, 
					    Expr* init = nilExpr, 
					    varType vartag = VAR_NORMAL, 
					    bool isConst = false);
  static VarDefStmt* Symboltable::defineVarDefStmt(Symbol* idents, Type* type, 
						   Expr* init, varType vartag, 
						   bool isConst);
  static EnumSymbol* Symboltable::defineEnumList(Symbol* symList);
  static Type* Symboltable::defineBuiltinType(char* name, Expr* init,
					      bool placeholder = false);
  static FnSymbol* startFnDef(char* name, bool insert = true);
  static FnDefStmt* finishFnDef(FnSymbol* fnsym, Symbol* formals, 
				Type* retType, Stmt* body, 
				bool isExtern = false);
  static FnDefStmt* defineFunction(char* name, Symbol* formals, Type* retType, 
				   Stmt* body, bool isExtern = false);

  static ClassSymbol* startClassDef(char* name, ClassSymbol* parent);
  static TypeDefStmt* finishClassDef(ClassSymbol* classSym, Stmt* definition);

  static VarSymbol* startForLoop(Symbol* indices);
  static ForLoopStmt* finishForLoop(bool forall, VarSymbol* index,
				    Expr* domain, Stmt* body);

  static MemberAccess* defineMemberAccess(Expr* base, char* member);

  static DomainExpr* defineQueryDomain(char* name);

  static void print(FILE* outfile = stderr);
  static void dump(FILE* outfile = stderr);
};

#endif
