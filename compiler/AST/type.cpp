#include "expr.h"
#include "symbol.h"
#include "type.h"

Type* dtBoolean;
Type* dtInteger;
Type* dtFloat;

Type* dtTimer;
Type* dtLocale;

Type* dtUnknown;

Symbol* pstBoolean;
Symbol* pstInteger;
Symbol* pstFloat;

Symbol* pstTimer;
Symbol* pstLocale;

Symbol* pstUnknown;


Type::Type(void) :
  name(NULL)
{}

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


void Type::printDef(FILE* outfile) {
  print(outfile);
}


EnumType::EnumType(Symbol* init_valList) :
  valList(init_valList)
{}


void EnumType::printDef(FILE* outfile) {
  printf("enum ");
  name->print(outfile);
  printf(" = ");
  valList->printList(outfile);
}


DomainType::DomainType(int init_rank) :
  rank(init_rank)
{}


void DomainType::print(FILE* outfile) {
  fprintf(outfile, "domain(");
  if (rank != 0) {
    fprintf(outfile, "%d", rank);
  } else {
    fprintf(outfile, "???");
  }
  fprintf(outfile, ")");
}


SubDomainType::SubDomainType(Symbol* init_parent) :
  parent(init_parent)
{
  rank = 777; // BLC -- fill in correctly!
}


void SubDomainType::print(FILE* outfile) {
  fprintf(outfile, "domain(");
  parent->print(outfile);
  fprintf(outfile, ")");
}


ArrayType::ArrayType(Expr* init_domain, Type* init_elementType):
  domain(init_domain),
  elementType(init_elementType)
{}


UserType::UserType(Type* init_definition) :
  definition(init_definition)
{}


void UserType::printDef(FILE* outfile) {
  fprintf(outfile, "type ");
  name->print(outfile);
  fprintf(outfile, " = ");
  definition->print(outfile);
}




void ArrayType::print(FILE* outfile) {
  //  fprintf(outfile, "[");
  domain->print(outfile);
  //  fprintf(outfile, "] ");
  fprintf(outfile, " ");
  elementType->print(outfile);
}


static void newType(char* name, Type** dtHandle, Symbol** symHandle) {
  *dtHandle = new Type();
  *symHandle = new TypeSymbol(name, *dtHandle);
  (*dtHandle)->addName(*symHandle);
}


void setupTypes(void) {
  newType("boolean", &dtBoolean, &pstBoolean);
  newType("integer", &dtInteger, &pstInteger);
  newType("float", &dtFloat, &pstFloat);
  newType("locale", &dtLocale, &pstLocale);
  newType("timer", &dtTimer, &pstTimer);
  newType("???", &dtUnknown, &pstUnknown);
}
