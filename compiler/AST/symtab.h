#ifndef _SYMTAB_H_
#define _SYMTAB_H_

#include "symbol.h"

enum scopeType {
  SCOPE_INTRINSIC,
  SCOPE_FILE,
  SCOPE_PARAM,
  SCOPE_FUNCTION,
  SCOPE_LOCAL
};
  

class Symboltable {
 public:
  Symboltable(scopeType init_type);

  static void pushScope(scopeType type);
  static void popScope();

  static void define(Symbol* sym);
  static Symbol* lookup(char* name, bool genError = true);

  static ParamSymbol* defineParams(paramType formaltag, Symbol* idents,
				   Type* type);
  static Stmt* Symboltable::defineVars(varType vartag, bool isConst, 
				       Symbol* idents, Type* type, 
				       Expr* init);
  static EnumSymbol* Symboltable::defineEnumList(Symbol* symList);

  static void print(FILE* outfile);
};

#endif
