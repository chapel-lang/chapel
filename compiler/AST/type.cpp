#include <typeinfo>
#define TYPE_EXTERN
#include "expr.h"
#include "misc.h"
#include "module.h"
#include "stringutil.h"
#include "symbol.h"
#include "symtab.h"
#include "type.h"


Type::Type(astType_t astType, Expr* init_defaultVal) :
  BaseAST(astType),
  name(nilSymbol),
  defaultVal(init_defaultVal),
  asymbol(NULL)
{}


void Type::addName(Symbol* newname) {
  name = newname;
}


Type* Type::copy(void) {
  return this;
}


bool Type::isNull(void) {
  return (this == nilType);
}


bool Type::isComplex(void) {
  return (this == dtComplex);
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
    if (atTop || name == nilSymbol) {
      // if the user has asked to traverse the type, always traverse
      // its definition
      // OR if the type is anonymous, always traverse its definition
      traverseDefType(traversal);
    } else {
      traverseType(traversal);
    }
  }
  if (traversal->processTop || !atTop) {
    traversal->postProcessType(this);
  }
}


void Type::traverseDef(Traversal* traversal, bool atTop) {
  if (isNull()) {
    return;
  }

  // expore Type and components
  if (traversal->processTop || !atTop) {
    traversal->preProcessType(this);
  }
  if (atTop || traversal->exploreChildTypes) {
    name->traverse(traversal, false);
    traverseDefType(traversal);
  }
  if (traversal->processTop || !atTop) {
    traversal->postProcessType(this);
  }
}


void Type::traverseType(Traversal* traversal) {
}


void Type::traverseDefType(Traversal* traversal) {
  defaultVal->traverse(traversal, false);
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
  } else if (this == dtComplex) {
    fprintf(outfile, "_complex128");
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


void Type::codegenSafeInit(FILE* outfile) {
  if (this == dtString) {
    fprintf(outfile, " = NULL");
  } else {
    // Most types won't need an initializer to be safe
  }
}


void Type::codegenIORoutines(FILE* outfile) {
}


void Type::codegenDefaultFormat(FILE* outfile, bool isRead) {
  fprintf(outfile, "_default_format");
  if (isRead) {
    fprintf(outfile, "_read");
  } else {
    fprintf(outfile, "_write");
  }
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


EnumType::EnumType(EnumSymbol* init_valList) :
  Type(TYPE_ENUM, new Variable(init_valList)),
  valList(init_valList)
{
  Symbol* val = valList;
  while (val) {
    val->type = this;
    val = nextLink(Symbol, val);
  }
}


Type* EnumType::copy(void) {
  Symbol* newSyms = valList->copyList();

  if (typeid(*newSyms) != typeid(EnumSymbol)) {
    INT_FATAL(this, "valList is not EnumSymbol in EnumType::copy()");
    return nilType;
  } else {
    EnumSymbol* newEnums = (EnumSymbol*)newSyms;
    return new EnumType(newEnums);
  }
}


void EnumType::traverseDefType(Traversal* traversal) {
  valList->traverseList(traversal, false);
  defaultVal->traverse(traversal, false);
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


static void codegenIOPrototype(FILE* outfile, Symbol* name) {
  fprintf(outfile, "void _write");
  name->codegen(outfile);
  fprintf(outfile, "(FILE* outfile, char* format, ");
  name->codegen(outfile);
  fprintf(outfile, " val)");
}


void EnumType::codegenIORoutines(FILE* outfile) {
  EnumSymbol* enumSym;

  codegenIOPrototype(currentModule->intheadfile, name);
  fprintf(currentModule->intheadfile, ";\n\n");

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


void EnumType::codegenDefaultFormat(FILE* outfile, bool isRead) {
  fprintf(outfile, "_default_format");
  if (isRead) {
    fprintf(outfile, "_read");
  } else {
    fprintf(outfile, "_write");
  }
  fprintf(outfile, "_enum");
}


DomainType::DomainType(Expr* init_expr) :
  Type(TYPE_DOMAIN, nilExpr),
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
  Type(TYPE_DOMAIN, nilExpr),
  numdims(init_numdims),
  parent(nilExpr)
{}


Type* DomainType::copy(void) {
  if (parent->isNull()) {
    return new DomainType(numdims);
  } else {
    return new DomainType(parent->copy());
  }
}


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


IndexType::IndexType(int init_numdims) :
  DomainType(init_numdims)
{
  astType = TYPE_INDEX;
}


Type* IndexType::copy(void) {
  if (parent->isNull()) {
    return new IndexType(numdims);
  } else {
    return new IndexType(parent->copy());
  }
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
  Type(TYPE_ARRAY, init_elementType->defaultVal),
  domain(init_domain),
  elementType(init_elementType)
{}


Type* ArrayType::copy(void) {
  return new ArrayType(domain->copy(), elementType->copy());
}


void ArrayType::traverseDefType(Traversal* traversal) {
  domain->traverse(traversal, false);
  elementType->traverse(traversal, false);
  defaultVal->traverse(traversal, false);
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


void ArrayType::codegenDefaultFormat(FILE* outfile, bool isRead) {
  elementType->codegenDefaultFormat(outfile, isRead);
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


UserType::UserType(Type* init_definition, Expr* init_defaultVal) :
  Type(TYPE_USER, init_defaultVal),
  definition(init_definition)
{}


Type* UserType::copy(void) {
  return new UserType(definition->copy(), defaultVal->copy());
}


bool UserType::isComplex(void) {
  return definition->isComplex();
}


void UserType::traverseDefType(Traversal* traversal) {
  definition->traverse(traversal, false);
  defaultVal->traverse(traversal, false);
}


void UserType::printDef(FILE* outfile) {
  fprintf(outfile, "type ");
  name->print(outfile);
  fprintf(outfile, " = ");
  definition->print(outfile);
}


void UserType::codegen(FILE* outfile) {
  name->codegen(outfile);
}


void UserType::codegenDef(FILE* outfile) {
  fprintf(outfile, "typedef ");
  definition->codegen(outfile);
  fprintf(outfile, " ");
  name->codegen(outfile);
  fprintf(outfile, ";\n");
}


// TODO: We should probably instead have types print out
// their own write routines and have UserType print its
// definition's write routine
void UserType::codegenIORoutines(FILE* outfile) {
  codegenIOPrototype(currentModule->intheadfile, name);
  fprintf(currentModule->intheadfile, ";\n\n");
  
  codegenIOPrototype(outfile, name);
  fprintf(outfile, " {\n");
  fprintf(outfile, "  _write");
  definition->codegen(outfile);
  fprintf(outfile, "(outfile, format, val);\n");
  fprintf(outfile, "}\n");
}


void UserType::codegenDefaultFormat(FILE* outfile, bool isRead) {
  definition->codegenDefaultFormat(outfile, isRead);
}


ClassType::ClassType(ClassType* init_parentClass, Stmt* init_definition,
		     FnDefStmt* init_constructor, SymScope* init_scope) :
  Type(TYPE_CLASS, nilExpr),
  parentClass(init_parentClass),
  definition(init_definition),
  constructor(init_constructor),
  scope(init_scope)
{}


Type* ClassType::copy(void) {
  FnDefStmt* newConstructor = dynamic_cast<FnDefStmt*>(constructor->copy());
  ClassType* newParent = dynamic_cast<ClassType*>(parentClass->copy());
  return new ClassType(newParent, definition->copyList(), newConstructor);
}


void ClassType::addDefinition(Stmt* init_definition) {
  definition = init_definition;

  if (!isNull() && Symboltable::parsingUserCode()) {
    /* create default constructor */
    char* constructorName = glomstrings(2, "_construct_", name->name);
    FnSymbol* newFunSym = Symboltable::startFnDef(constructorName, false);
    BlockStmt* body = new BlockStmt(
			new ReturnStmt(
			  new CastExpr(this, 
			    new FnCall(
			      new Variable(
				Symboltable::lookupInternal("malloc")), 
			      new SizeofExpr(this)))
			  )
			);
    constructor = Symboltable::finishFnDef(newFunSym, nilSymbol, this, body);
  }
}


void ClassType::addScope(SymScope* init_scope) {
  scope = init_scope;
}


void ClassType::traverseDefType(Traversal* traversal) {
  definition->traverseList(traversal, false);
  constructor->traverseList(traversal, false);
  defaultVal->traverse(traversal, false);
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
  codegenIOPrototype(currentModule->intheadfile, name);
  fprintf(currentModule->intheadfile, ";\n\n");

  codegenIOPrototype(outfile, name);
  fprintf(outfile, "{\n");
  fprintf(outfile, "}\n");
}


TupleType::TupleType(Type* firstType) :
  Type(TYPE_TUPLE, nilExpr)
{
  components.add(firstType);
}


void TupleType::addType(Type* additionalType) {
  components.add(additionalType);
}


Type* TupleType::copy(void) {
  TupleType* newTupleType = new TupleType(components.v[0]->copy());
  for (int i=1; i<components.n; i++) {
    newTupleType->addType(components.v[i]->copy());
  }
  
  return newTupleType;
}


void TupleType::traverseDefType(Traversal* traversal) {
  for (int i=0; i<components.n; i++) {
    components.v[i]->traverse(traversal, false);
  }
  defaultVal->traverse(traversal, false);
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
  dtUnknown = Symboltable::defineBuiltinType("???", nilExpr);
  dtVoid = Symboltable::defineBuiltinType("void", nilExpr);

  dtBoolean = Symboltable::defineBuiltinType("boolean", 
					     new BoolLiteral("false", false));
  dtInteger = Symboltable::defineBuiltinType("integer", 
					     new IntLiteral("0", 0));
  dtFloat = Symboltable::defineBuiltinType("float", 
					   new FloatLiteral("0.0", 0.0));
  dtComplex = Symboltable::defineBuiltinType("complex", 
					     new FloatLiteral("0.0", 0.0));
  dtString = Symboltable::defineBuiltinType("string", new StringLiteral(""));
  

  dtLocale = Symboltable::defineBuiltinType("locale", nilExpr);
}


void findInternalTypes(void) {
  dtTuple = Symboltable::lookupInternalType("Tuple")->type;
  dtIndex = Symboltable::lookupInternalType("Index")->type;
  dtDomain = Symboltable::lookupInternalType("Domain")->type;
  dtArray = Symboltable::lookupInternalType("Array")->type;
}
