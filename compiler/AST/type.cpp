#include "codegen.h"
#include "expr.h"
#include "misc.h"
#include "symbol.h"
#include "type.h"

Type* dtVoid;
Type* dtInteger;
Type* dtFloat;
Type* dtString;

Type* dtTimer;
Type* dtLocale;

Type* dtUnknown;

Symbol* pstVoid;
Symbol* pstInteger;
Symbol* pstFloat;
Symbol* pstString;

Symbol* pstTimer;
Symbol* pstLocale;

Symbol* pstUnknown;


Type::Type(astType_t astType) :
  BaseAST(astType),
  name(NULL),
  asymbol(NULL)
{}


void Type::addName(Symbol* newname) {
  name = newname;
}


void Type::traverse(Traversal* traversal) {
  traversal->preProcessType(this);
  if (traversal->exploreTypes) {
    traverseType(traversal);
  }
  traversal->postProcessType(this);
  if (next && !next-isNull()) {
    next->traverse(traversal);
  }
}


void Type::traverseType(Traversal* traversal) {
}


int Type::rank(void) {
  return 0;
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
  if (this == dtUnknown) {  // BLC: Hack until we get type inference working
    dtInteger->codegen(outfile);
  } else {
    this->codegen(outfile);
  }
}


int
Type::getSymbols(Vec<BaseAST *> &asts) {
  if (name)
    asts.add(name);
  return asts.n;
} 


NullType::NullType(void) :
  Type(TYPE_NULL)
{}

bool NullType::isNull(void) {
  return true;
}


EnumType::EnumType(EnumSymbol* init_valList) :
  Type(TYPE_ENUM),
  valList(init_valList)
{
  Symbol* val = valList;
  while (val != NULL) {
    val->type = this;
    val = nextLink(Symbol, val);
  }
}


void EnumType::traverseType(Traversal* traversal) {
  bool saveTraverseSymbols = traversal->exploreSymbols;
  traversal->exploreSymbols = false;
  valList->traverse(traversal);
  traversal->exploreSymbols = saveTraverseSymbols;
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


int
EnumType::getSymbols(Vec<BaseAST *> &asts) {
  BaseAST *v = valList;
  while (v) {
    asts.add(v);
    v = dynamic_cast<BaseAST *>(v->next);
  }
  return asts.n;
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


DomainType::DomainType(int init_numdims) :
  Type(TYPE_DOMAIN),
  numdims(init_numdims)
{}


int DomainType::rank(void) {
  return numdims;
}


void DomainType::print(FILE* outfile) {
  fprintf(outfile, "domain(");
  if (numdims != 0) {
    fprintf(outfile, "%d", numdims);
  } else {
    fprintf(outfile, "???");
  }
  fprintf(outfile, ")");
}


void DomainType::codegen(FILE* outfile) {
  fprintf(outfile, "_domain%d", numdims ? numdims : 1);  // BLC: hack!
}


SubDomainType::SubDomainType(Symbol* init_parent) :
  DomainType(),
  parent(init_parent)
{
  astType = TYPE_SUBDOMAIN;
  numdims = 777; // BLC -- fill in correctly!
}


void SubDomainType::print(FILE* outfile) {
  fprintf(outfile, "domain(");
  parent->print(outfile);
  fprintf(outfile, ")");
}


int
SubDomainType::getSymbols(Vec<BaseAST *> &asts) {
  asts.add(parent);
  return asts.n;
}


IndexType::IndexType(int init_numdims) :
  DomainType(init_numdims)
{
  astType = TYPE_INDEX;
}


void IndexType::print(FILE* outfile) {
  fprintf(outfile, "index(");
  if (numdims != 0) {
    fprintf(outfile, "%d", numdims);
  } else {
    fprintf(outfile, "???");
  }
  fprintf(outfile, ")");
}


SubIndexType::SubIndexType(Symbol* init_parent) :
  SubDomainType(init_parent)
{
  astType = TYPE_SUBINDEX;
  numdims = 777; // BLC -- fill in correctly!
}


void SubIndexType::print(FILE* outfile) {
  fprintf(outfile, "index(");
  parent->print(outfile);
  fprintf(outfile, ")");
}


ArrayType::ArrayType(Expr* init_domain, Type* init_elementType):
  Type(TYPE_ARRAY),
  domain(init_domain),
  elementType(init_elementType)
{}


void ArrayType::traverseType(Traversal* traversal) {
  domain->traverse(traversal);
  elementType->traverse(traversal);
}


int ArrayType::rank(void) {
  return domain->rank();
}


void ArrayType::print(FILE* outfile) {
  //  fprintf(outfile, "[");
  domain->print(outfile);
  //  fprintf(outfile, "] ");
  fprintf(outfile, " ");
  elementType->print(outfile);
}


void ArrayType::codegen(FILE* outfile) {
  fprintf(outfile, "_array%d", domain->rank());
  elementType->codegen(outfile);
}


void ArrayType::codegenDefaultFormat(FILE* outfile) {
  elementType->codegenDefaultFormat(outfile);
}


int
ArrayType::getExprs(Vec<BaseAST *> &asts) {
  asts.add(domain);
  return asts.n;
}


int
ArrayType::getTypes(Vec<BaseAST *> &asts) {
  asts.add(elementType);
  return asts.n;
}


UserType::UserType(Type* init_definition) :
  Type(TYPE_USER),
  definition(init_definition)
{}


void UserType::traverse(Traversal* traversal) {
  definition->traverse(traversal);
}


void UserType::printDef(FILE* outfile) {
  fprintf(outfile, "type ");
  name->print(outfile);
  fprintf(outfile, " = ");
  definition->print(outfile);
}


int
UserType::getTypes(Vec<BaseAST *> &asts) {
  asts.add(definition);
  return asts.n;
}


ClassType::ClassType(ClassType* init_parentClass) :
  Type(TYPE_CLASS),
  parentClass(init_parentClass)
{}


void ClassType::print(FILE* outfile) {
  fprintf(outfile, "/* Classes not implemented yet */\n");
}


int
ClassType::getTypes(Vec<BaseAST *> &asts) {
  asts.add(parentClass);
  return asts.n;
}


static void newType(char* name, Type** dtHandle, Symbol** symHandle) {
  *dtHandle = new Type(TYPE_BUILTIN);
  *symHandle = new TypeSymbol(name, *dtHandle);
  (*dtHandle)->addName(*symHandle);
}


void initType(void) {
  newType("void", &dtVoid, &pstVoid);
  newType("integer", &dtInteger, &pstInteger);
  newType("float", &dtFloat, &pstFloat);
  newType("string", &dtString, &pstString);
  newType("locale", &dtLocale, &pstLocale);
  newType("timer", &dtTimer, &pstTimer);
  newType("???", &dtUnknown, &pstUnknown);
}
