#include "codegen.h"
#include "expr.h"
#include "misc.h"
#include "symbol.h"
#include "type.h"

Type* dtInteger;
Type* dtFloat;
Type* dtString;

Type* dtTimer;
Type* dtLocale;

Type* dtUnknown;

Symbol* pstInteger;
Symbol* pstFloat;
Symbol* pstString;

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

void Type::codegen(FILE* outfile) {
  if (this == dtInteger) {
    fprintf(outfile, "_integer64");
  } else if (this == dtFloat) {
    fprintf(outfile, "_float64");
  } else if (this == dtString) {
    fprintf(outfile, "_string");
  } else {
    fprintf(outfile, "???");
  }
}


void Type::codegenDef(FILE* outfile) {
  INT_FATAL(this, "Don't know how to codegenDef() for all types yet");
}


void Type::codegenIORoutines(FILE* outfile) {
  INT_FATAL(this, "Don't know how to codegenIORoutines() for all types yet");
}


void Type::codegenDefaultFormat(FILE* outfile) {
  fprintf(outfile, "_default_format");
  this->codegen(outfile);
}


NullType::NullType(void) {
}

bool NullType::isNull(void) {
  return true;
}


EnumType::EnumType(EnumSymbol* init_valList) :
  valList(init_valList)
{
  Symbol* val = valList;
  while (val != NULL) {
    val->type = this;
    val = nextLink(Symbol, val);
  }
}


void EnumType::printDef(FILE* outfile) {
  printf("enum ");
  name->print(outfile);
  printf(" = ");
  valList->printList(outfile, " | ");
}


void EnumType::codegen(FILE* outfile) {
  name->codegen(outfile);
}


void EnumType::codegenDef(FILE* outfile) {
  EnumSymbol* enumSym;

  fprintf(outfile, "typedef enum {\n");
  enumSym = valList;
  while (enumSym != NULL) {
    enumSym->printDef(outfile);

    enumSym = nextLink(EnumSymbol, enumSym);

    if (enumSym) {
      fprintf(outfile, ",");
    }
    fprintf(outfile, "\n");
  }
  fprintf(outfile, "} ");
  name->codegen(outfile);
  fprintf(outfile, ";\n\n");
}


static void codegenIOPrototype(FILE* outfile, Symbol* name) {
  fprintf(outfile, "void _write");
  name->codegen(outfile);
  fprintf(outfile, "(FILE* outfile, char* format, ");
  name->codegen(outfile);
  fprintf(outfile, " val)");
}


void EnumType::codegenIORoutines(FILE* outfile) {
  EnumSymbol* enumSym;

  codegenIOPrototype(intheadfile, name);
  fprintf(intheadfile, ";\n\n");

  codegenIOPrototype(outfile, name);
  fprintf(outfile, " {\n");

  fprintf(outfile, "switch (val) {\n");
  enumSym = valList;
  while (enumSym != NULL) {
    fprintf(outfile, "case ");
    enumSym->codegen(outfile);
    fprintf(outfile, ":\n");
    fprintf(outfile, "fprintf(outfile, format, \"");
    enumSym->codegen(outfile);
    fprintf(outfile, "\");\n");
    fprintf(outfile, "break;\n");

    enumSym = nextLink(EnumSymbol, enumSym);
  }
  fprintf(outfile, "}\n");
  fprintf(outfile, "}\n\n");
}


void EnumType::codegenDefaultFormat(FILE* outfile) {
  fprintf(outfile, "_default_format_enum");
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


void DomainType::codegen(FILE* outfile) {
  fprintf(outfile, "_domain%d", rank ? rank : 1);  // BLC: hack!
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


IndexType::IndexType(int init_rank) :
  DomainType(init_rank)
{}


void IndexType::print(FILE* outfile) {
  fprintf(outfile, "index(");
  if (rank != 0) {
    fprintf(outfile, "%d", rank);
  } else {
    fprintf(outfile, "???");
  }
  fprintf(outfile, ")");
}


SubIndexType::SubIndexType(Symbol* init_parent) :
  SubDomainType(init_parent)
{
  rank = 777; // BLC -- fill in correctly!
}


void SubIndexType::print(FILE* outfile) {
  fprintf(outfile, "index(");
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


ClassType::ClassType(ClassType* init_parentClass) :
  parentClass(init_parentClass)
{}


void ClassType::print(FILE* outfile) {
  fprintf(outfile, "/* Classes not implemented yet */\n");
}


static void newType(char* name, Type** dtHandle, Symbol** symHandle) {
  *dtHandle = new Type();
  *symHandle = new TypeSymbol(name, *dtHandle);
  (*dtHandle)->addName(*symHandle);
}


void initType(void) {
  newType("integer", &dtInteger, &pstInteger);
  newType("float", &dtFloat, &pstFloat);
  newType("string", &dtString, &pstString);
  newType("locale", &dtLocale, &pstLocale);
  newType("timer", &dtTimer, &pstTimer);
  newType("???", &dtUnknown, &pstUnknown);
}
