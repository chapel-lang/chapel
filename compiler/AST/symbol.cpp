#include "symbol.h"

Symbol* pstSumReduce = new Symbol("sum");

Symbol::Symbol(char* init_name) :
  name(init_name)
{}


void Symbol::print(FILE* outfile) {
  fprintf(outfile, "%s", name);
}


NullSymbol::NullSymbol(void) :
  Symbol("")
{}


VarSymbol::VarSymbol(char* init_name, Type* init_type) :
  Symbol(init_name),
  type(init_type)
{}


void VarSymbol::setType(Type* newType) {
  type = newType;
}


void VarSymbol::setIsVar(bool init_isVar) {
  isVar = init_isVar;
}


void VarSymbol::setVarClass(varType init_varClass) {
  varClass = init_varClass;
}


void VarSymbol::printWithType(FILE* outfile) {
  print(outfile);
  if (type != NULL) {
    fprintf(outfile, ": ");
    type->print(outfile);
  }
}


TypeSymbol::TypeSymbol(char* init_name, Type* init_definition) :
  Symbol(init_name),
  definition(init_definition)
{}


FunSymbol::FunSymbol(char* init_name, Symbol* init_formals, Type* init_retType,
		     Stmt* init_body) :
  Symbol(init_name),
  formals(init_formals),
  retType(init_retType),
  body(init_body)
{}
