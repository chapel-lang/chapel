#include <typeinfo>
#include "misc.h"
#include "symbol.h"


Symbol::Symbol(char* init_name) :
  name(init_name)
{}


void Symbol::print(FILE* outfile) {
  fprintf(outfile, "%s", name);
}


void Symbol::printDef(FILE* outfile) {
  print(outfile);
}


void Symbol::printDefList(FILE* outfile, char* separator) {
  Symbol* ptr;

  printDef(outfile);
  ptr = (Symbol*)next;  // BLC: switch to dynamic cast
  while (ptr != NULL) {
    fprintf(outfile, "%s", separator);
    ptr->printDef(outfile);
    ptr = (Symbol*)(ptr->next);  // BLC: switch to dynamic cast
  }
}


NullSymbol::NullSymbol(void) :
  Symbol("NullSymbol")
{}


bool NullSymbol::isNull(void) {
  return true;
}


UseBeforeDefSymbol::UseBeforeDefSymbol(char* init_name) :
  Symbol(init_name)
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


NullVarSymbol::NullVarSymbol() :
  VarSymbol("NullVarSymbol")
{}


bool NullVarSymbol::isNull(void) {
  return true;
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


void ParamSymbol::printDef(FILE* outfile) {
  fprintf(outfile, "%s ", paramTypeNames[usage]);
  Symbol::print(outfile);
  fprintf(outfile, ": ");
  type->print(outfile);
}


TypeSymbol::TypeSymbol(char* init_name, Type* init_definition) :
  Symbol(init_name),
  definition(init_definition)
{}


ClassSymbol::ClassSymbol(char* init_name, ClassType* init_class) :
  TypeSymbol(init_name, init_class)
{}


NullClassSymbol::NullClassSymbol(void) :
  ClassSymbol("NullClass", NULL)
{}


bool NullClassSymbol::isNull(void) {
  return true;
}


ClassType* ClassSymbol::getType(void) {
  if (typeid(*definition) != typeid(ClassType)) {
    INT_FATAL(NULL, "ClassSymbol has non-ClassType");
  }
  return (ClassType*)definition;
}


ReduceSymbol::ReduceSymbol(char* init_name, ClassType* init_class) :
  ClassSymbol(init_name, init_class)
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
