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


void Symbol::setIsVar(bool init_isVar) {
  isVar = init_isVar;
}


void Symbol::setVarClass(varType init_varClass) {
  varClass = init_varClass;
}


void Symbol::print(FILE* outfile) {
  fprintf(outfile, "%s", name);
}


void Symbol::printList(FILE* outfile) {
  Link* ptr;

  print(outfile);
  ptr = next;
  while (ptr != NULL) {
    fprintf(outfile, " | ");
    ptr->print(outfile);
    ptr = ptr->next;
  }
}


void Symbol::printWithType(FILE* outfile) {
  print(outfile);
  if (type != NULL) {
    fprintf(outfile, ": ");
    type->print(outfile);
  }
}
