#ifndef _SYMTAB_H_
#define _SYMTAB_H_

#include "link.h"
#include "symbol.h"

enum scopeType {
  SCOPE_INTRINSIC,
  SCOPE_FILE,
  SCOPE_PARAM,
  SCOPE_FUNCTION,
  SCOPE_LOCAL
};
  

class Symboltable : public Link {
 public:
  Symboltable(scopeType init_type);

  static void pushScope(scopeType type);
  static void popScope();

  static void define(Symbol* sym);
  static Symbol* lookup(char* name);

  static ParamSymbol* defineParams(paramType formaltag, Symbol* idents,
				   Type* type);
  static Stmt* Symboltable::defineVars(varType vartag, bool isConst, 
				       Symbol* idents, Type* type, 
				       Expr* init);
};

#endif
