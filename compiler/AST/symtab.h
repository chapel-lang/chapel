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

  static void pushScope(scopeType type);
  static SymScope* popScope(void);
  static SymScope* getCurrentScope(void);
  static FnSymbol* getCurrentFn(void);

  static void define(Symbol* sym);
  static Symbol* lookupInScope(char* name, SymScope* scope);
  static Symbol* lookupInternal(char* name);
  static Symbol* lookup(char* name, bool inLexer = false);
  static ClassSymbol* lookupClass(char* name);

  static void startCompoundStmt(void);
  static BlockStmt* finishCompoundStmt(Stmt* body);
  static ParamSymbol* defineParams(paramType formaltag, Symbol* idents,
				   Type* type);
  static VarSymbol* Symboltable::defineVars(Symbol* idents, Type* type, 
					    Expr* init = nilExpr, 
					    varType vartag = VAR_NORMAL, 
					    bool isConst = false);
  static EnumSymbol* Symboltable::defineEnumList(Symbol* symList);
  static Type* Symboltable::defineBuiltinType(char* name, 
					      bool placeholder = false);
  static FnSymbol* startFnDef(char* name);
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

  static DomainExpr* defineQueryDomain(char* name);

  static void print(FILE* outfile);
  static void dump(FILE* outfile);
};

#endif
