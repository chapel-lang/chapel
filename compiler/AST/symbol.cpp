#include "symbol.h"

// BLC: Yuck
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


VarSymbol::VarSymbol(char* init_name, varType init_varClass, bool init_isConst,
		     Type* init_type) :
  Symbol(init_name),
  varClass(init_varClass),
  isConst(init_isConst),
  type(init_type)
{}


void VarSymbol::printWithType(FILE* outfile) {
  print(outfile);
  if (type != NULL) {
    fprintf(outfile, ": ");
    type->print(outfile);
  }
}


static char* paramTypeNames[NUM_PARAM_TYPES] = {
  "in",
  "inout",
  "out",
  "const"
};


ParamSymbol::ParamSymbol(paramType init_usage, char* init_name, 
			 Type* init_type) :
  Symbol(init_name),
  usage(init_usage),
  type(init_type)
{}


void ParamSymbol::print(FILE* outfile) {
  fprintf(outfile, "%s ", paramTypeNames[usage]);
  Symbol::print(outfile);
  fprintf(outfile, ": ");
  type->print(outfile);
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


EnumSymbol::EnumSymbol(char* init_name, int init_val) :
  Symbol(init_name),
  val(init_val)
{}
