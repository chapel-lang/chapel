#include <typeinfo>
#define TYPE_EXTERN
#include "codegen.h"
#include "expr.h"
#include "misc.h"
#include "stringutil.h"
#include "symbol.h"
#include "symtab.h"
#include "type.h"


Type::Type(astType_t astType) :
  BaseAST(astType),
  name(nilSymbol),
  asymbol(NULL)
{}


void Type::addName(Symbol* newname) {
  name = newname;
}


bool Type::isNull(void) {
  return (this == nilType);
}


void Type::traverse(Traversal* traversal, bool atTop) {
  if (isNull()) {
    return;
  }

  // expore Type and components
  if (traversal->processTop || !atTop) {
    traversal->preProcessType(this);
  }
  if (atTop || traversal->exploreChildTypes) {
    traverseType(traversal);
  }
  if (traversal->processTop || !atTop) {
    traversal->postProcessType(this);
  }
}


void Type::traverseType(Traversal* traversal) {
}


int Type::rank(void) {
  return 0;
}


void Type::print(FILE* outfile) {
  name->print(outfile);
}


void Type::printDef(FILE* outfile) {
  print(outfile);
}

void Type::codegen(FILE* outfile) {
  // BLC: Perhaps we should just rename the built-in types between
  // parsing and codegen?
  if (this == dtBoolean) {
    fprintf(outfile, "_boolean");
  } else if (this == dtInteger) {
    fprintf(outfile, "_integer64");
  } else if (this == dtFloat) {
    fprintf(outfile, "_float64");
  } else if (this == dtString) {
    fprintf(outfile, "_string");
  } else if (this == dtVoid) {
    fprintf(outfile, "void");
  } else if (this == dtUnknown) {
    fprintf(outfile, "???");
  } else {
    INT_FATAL(this, "Don't know how to codegen() for all types yet");
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


void Type::codegenConstructors(FILE* outfile) {
}


bool Type::needsInit(void) {
  return false;
}


void Type::generateInit(FILE* outfile, VarSymbol* var) {
  INT_FATAL(this, "Don't know how to generateInit() for all types yet");
}


int
Type::getSymbols(Vec<BaseAST *> &asts) {
  if (name)
    asts.add(name);
  return asts.n;
} 


EnumType::EnumType(EnumSymbol* init_valList) :
  Type(TYPE_ENUM),
  valList(init_valList)
{
  Symbol* val = valList;
  while (val) {
    val->type = this;
    val = nextLink(Symbol, val);
  }
}


void EnumType::traverseType(Traversal* traversal) {
  valList->traverseList(traversal, false);
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
  while (enumSym) {
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
  while (enumSym) {
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


DomainType::DomainType(Expr* init_expr) :
  Type(TYPE_DOMAIN),
  numdims(0),
  parent(nilExpr)
{
  if (!init_expr->isNull()) {
    if (typeid(*init_expr) == typeid(IntLiteral)) {
      numdims = init_expr->intVal();
    } else {
      numdims = init_expr->rank();
      parent = init_expr;
    }
  }
}

  DomainType::DomainType(int init_numdims) :
    Type(TYPE_DOMAIN),
    numdims(init_numdims),
    parent(nilExpr)
{}


int DomainType::rank(void) {
  return numdims;
}


void DomainType::print(FILE* outfile) {
  fprintf(outfile, "domain(");
  if (parent->isNull()) {
    if (numdims != 0) {
      fprintf(outfile, "%d", numdims);
    } else {
      fprintf(outfile, "???");
    }
  } else {
    parent->print(outfile);
  }
  fprintf(outfile, ")");
}


void DomainType::codegen(FILE* outfile) {
  fprintf(outfile, "_domain%d", numdims ? numdims : 1);  // BLC: hack!
}


IndexType::IndexType(Expr* init_expr) :
  DomainType(init_expr)
{
  astType = TYPE_INDEX;
}


void IndexType::print(FILE* outfile) {
  fprintf(outfile, "index(");
  if (parent->isNull()) {
    if (numdims != 0) {
      fprintf(outfile, "%d", numdims);
    } else {
      fprintf(outfile, "???");
    }
  } else {
    parent->print(outfile);
  }
  fprintf(outfile, ")");
}


ArrayType::ArrayType(Expr* init_domain, Type* init_elementType):
  Type(TYPE_ARRAY),
  domain(init_domain),
  elementType(init_elementType)
{}


void ArrayType::traverseType(Traversal* traversal) {
  domain->traverse(traversal, false);
  elementType->traverse(traversal, false);
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


bool ArrayType::needsInit(void) {
  return true;
}


void ArrayType::generateInit(FILE* outfile, VarSymbol* sym) {
  fprintf(outfile, "_init");
  this->codegen(outfile);
  fprintf(outfile, "(&(");
  sym->codegen(outfile);
  fprintf(outfile, "), &(");
  domain->codegen(outfile);
  fprintf(outfile, "));\n");
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
  definition->traverse(traversal, false);
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
  parentClass(init_parentClass),
  definition(nilStmt),
  scope(NULL)
{}


void ClassType::addDefinition(Stmt* init_definition) {
  definition = init_definition;

  if (!isNull()) {
    /* create default constructor */
    char* constructorName = glomstrings(2, "_construct_", name->name);
    constructor = new FnDefStmt(
                    new FnSymbol(constructorName, nilSymbol, this, 
		      new BlockStmt(
                        new ReturnStmt(
                          new CastExpr(this, 
			    new FnCall(
			      new Variable(
				Symboltable::lookupInternal("malloc")), 
			      new SizeofExpr(this)
			      ))
			  )
			))
		    );
  }
}


void ClassType::addScope(SymScope* init_scope) {
  scope = init_scope;
}


void ClassType::traverseType(Traversal* traversal) {
  definition->traverseList(traversal, false);
  //  constructor->traverseList(traversal, false);
}


bool ClassType::isNull(void) {
  return (this == nilClassType);
}


void ClassType::codegen(FILE* outfile) {
  name->codegen(outfile);
}


void ClassType::codegenDef(FILE* outfile) {
  fprintf(outfile, "typedef struct _");
  name->codegen(outfile);
  fprintf(outfile, "_def {\n");
  definition->codegenVarDefs(outfile);
  fprintf(outfile, "} _");
  name->codegen(outfile);
  fprintf(outfile,", *");
  name->codegen(outfile);
  fprintf(outfile, ";\n\n");
}


void ClassType::codegenConstructors(FILE* outfile) {
  constructor->codegenList(outfile, "\n");
}


void ClassType::codegenIORoutines(FILE* outfile) {
  codegenIOPrototype(intheadfile, name);
  fprintf(intheadfile, ";\n\n");

  codegenIOPrototype(outfile, name);
  fprintf(outfile, "{\n");
  fprintf(outfile, "}\n");
}


int
ClassType::getTypes(Vec<BaseAST *> &asts) {
  asts.add(parentClass);
  return asts.n;
}


int
ClassType::getStmts(Vec<BaseAST *> &asts) {
  Vec<BaseAST *> elements;
  getLinkElements(elements, definition);
  asts.append(elements);
  return asts.n;
}


TupleType::TupleType(Type* firstType) :
  Type(TYPE_TUPLE)
{
  components.add(firstType);
}


void TupleType::addType(Type* additionalType) {
  components.add(additionalType);
}


void TupleType::traverseType(Traversal* traversal) {
  for (int i=0; i<components.n; i++) {
    components.v[i]->traverse(traversal, false);
  }
}


void TupleType::print(FILE* outfile) {
  fprintf(outfile, "(");
  for (int i=0; i<components.n; i++) {
    if (i) {
      fprintf(outfile, ", ");
    }
    components.v[i]->print(outfile);
  }
  fprintf(outfile, ")");
}


void TupleType::codegen(FILE* outfile) {
  INT_FATAL(this, "Cannot codegen Tuple types yet");
}


void initTypes(void) {
  // define built-in types
  dtUnknown = Symboltable::defineBuiltinType("???");
  dtVoid = Symboltable::defineBuiltinType("void");

  dtBoolean = Symboltable::defineBuiltinType("boolean");
  dtInteger = Symboltable::defineBuiltinType("integer");
  dtFloat = Symboltable::defineBuiltinType("float");
  dtComplex = Symboltable::defineBuiltinType("complex");
  dtString = Symboltable::defineBuiltinType("string");


  dtLocale = Symboltable::defineBuiltinType("locale");

  // this needs to be moved into the standard prelude
  dtTimer = Symboltable::defineBuiltinType("timer", false);
}


void findInternalTypes(void) {
  dtTuple = dynamic_cast<TypeSymbol *>(Symboltable::lookupInternal("Tuple"))->type;
  dtIndex = dynamic_cast<TypeSymbol *>(Symboltable::lookupInternal("Index"))->type;
  dtDomain = dynamic_cast<TypeSymbol *>(Symboltable::lookupInternal("Domain"))->type;
  dtArray = dynamic_cast<TypeSymbol *>(Symboltable::lookupInternal("Array"))->type;
}
