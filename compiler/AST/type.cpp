#include "expr.h"
#include "symbol.h"
#include "type.h"

Type* dtInteger;
Symbol* pstInteger;

Type* dtFloat;
Symbol* pstFloat;

Type::Type(void) {
}

/*
Type::Type(Symbol* init_name) :
  name(init_name) {}
*/


void Type::addName(Symbol* newname) {
  name = newname;
}


void Type::print(FILE* outfile) {
  if (name != NULL) {
    name->print(outfile);
  }
}


DomainType::DomainType(int init_rank) :
  rank(init_rank)
{}


void DomainType::print(FILE* outfile) {
  fprintf(outfile, "domain(");
  if (rank != 0) {
    fprintf(outfile, "%d", rank);
  } else {
    fprintf(outfile, "?");
  }
  fprintf(outfile, ")");
}


ArrayType::ArrayType(Expr* init_domain, Type* init_elementType):
  domain(init_domain),
  elementType(init_elementType)
{}


void ArrayType::print(FILE* outfile) {
  fprintf(outfile, "[");
  domain->print(outfile);
  fprintf(outfile, "] ");
  elementType->print(outfile);
}


void setupTypes(void) {
  dtInteger = new Type();
  pstInteger = new Symbol("integer", dtInteger);
  dtInteger->addName(pstInteger);

  dtFloat = new Type();
  pstFloat = new Symbol("float", dtFloat);
  dtFloat->addName(pstFloat);
}
