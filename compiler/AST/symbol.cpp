#include "symbol.h"

Symbol* pstSumReduce = new Symbol("sum");

Symbol::Symbol(char* init_name, Type* init_type) :
  name(init_name),
  type(init_type)
{}


Symbol::Symbol(char* init_name) :
  name(init_name),
  type(NULL)
{}


void Symbol::setType(Type* newType) {
  type = newType;
}



void Symbol::print(FILE* outfile) {
  fprintf(outfile, "%s", name);
}


void Symbol::printWithType(FILE* outfile) {
  print(outfile);
  if (type != NULL) {
    fprintf(outfile, ": ");
    type->print(outfile);
  }
}
