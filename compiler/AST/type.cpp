#include "expr.h"
#include "symbol.h"
#include "type.h"

Type* dtBoolean;
Type* dtInteger;
Type* dtFloat;

Type* dtTimer;
Type* dtLocale;

Symbol* pstBoolean;
Symbol* pstInteger;
Symbol* pstFloat;

Symbol* pstTimer;
Symbol* pstLocale;


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
  //  fprintf(outfile, "[");
  domain->print(outfile);
  //  fprintf(outfile, "] ");
  fprintf(outfile, " ");
  elementType->print(outfile);
}


static void newType(char* name, Type** dtHandle, Symbol** symHandle) {
  *dtHandle = new Type();
  *symHandle = new Symbol(name, *dtHandle);
  (*dtHandle)->addName(*symHandle);
}


void setupTypes(void) {
  newType("boolean", &dtBoolean, &pstBoolean);
  newType("integer", &dtInteger, &pstInteger);
  newType("float", &dtFloat, &pstFloat);
  newType("locale", &dtLocale, &pstLocale);
  newType("timer", &dtTimer, &pstTimer);
}
