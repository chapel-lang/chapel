#include "symbol.h"

Symbol::Symbol(char* init_name, Type* init_type) :
  name(init_name),
  type(init_type) {}


Symbol::Symbol(char* init_name) :
  name(init_name),
  type(NULL) {}


void Symbol::print(FILE* outfile) {
  fprintf(outfile, "%s", name);
}


void Symbol::printWithType(FILE* outfile) {
  print(outfile);
  fprintf(outfile, ": ");
  type->print(outfile);
}
