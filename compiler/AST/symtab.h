#ifndef _SYMTAB_H_
#define _SYMTAB_H_

#include "symbol.h"

enum scopeType {
  SCOPE_INTRINSIC,
  SCOPE_FILE,
  SCOPE_PARAM,
  SCOPE_FUNCTION,
  SCOPE_LOCAL,
  SCOPE_FORLOOP
};
  

class Symboltable {
 public:
  Symboltable(scopeType init_type);

  static void pushScope(scopeType type);
  static void popScope();

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
  static FunSymbol* defineFunction(char* name, Symbol* formals, Type* retType, 
				   Stmt* body);
  static ClassType* defineClass(char* name, ClassSymbol* parent);

  static VarSymbol* enterForLoop(Symbol* indices);
  static void exitForLoop(void);

  static DomainExpr* defineQueryDomain(char* name);

  static void print(FILE* outfile);
};

#endif
