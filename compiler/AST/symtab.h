#ifndef _SYMTAB_H_
#define _SYMTAB_H_

#include "expr.h"
#include "symbol.h"
#include "symscope.h"

class Symboltable {
 public:
  static void pushScope(scopeType type);
  static void popScope(void);
  static SymScope* getCurrentScope(void);

  static void define(Symbol* sym);
  static Symbol* lookup(char* name, bool inLexer = false);
  static ClassSymbol* lookupClass(char* name);

  static ParamSymbol* defineParams(paramType formaltag, Symbol* idents,
				   Type* type);
  static VarSymbol* Symboltable::defineVars(Symbol* idents, Type* type, 
					    Expr* init = NULL, 
					    varType vartag = VAR_NORMAL, 
					    bool isConst = false);
  static EnumSymbol* Symboltable::defineEnumList(Symbol* symList);
  static FnSymbol* defineFunction(char* name, Symbol* formals, Type* retType, 
				  Stmt* body, bool isExtern = false);
  static ClassType* defineClass(char* name, ClassSymbol* parent);

  static VarSymbol* enterForLoop(Symbol* indices);
  static void exitForLoop(void);

  static DomainExpr* defineQueryDomain(char* name);

  static void print(FILE* outfile);
};

#endif
