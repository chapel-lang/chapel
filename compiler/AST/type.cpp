#include <typeinfo>
#define TYPE_EXTERN
#include "codegen.h"
#include "expr.h"
#include "misc.h"
#include "stringutil.h"
#include "symbol.h"
#include "symtab.h"
#include "type.h"


Type::Type(astType_t astType, Expr* init_initDefault) :
  BaseAST(astType),
  name(nilSymbol),
  initDefault(init_initDefault),
  asymbol(NULL)
{}


void Type::addName(Symbol* newname) {
  name = newname;
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
  initDefault->traverse(traversal, false);
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


int
Type::getSymbols(Vec<BaseAST *> &asts) {
  if (name)
    asts.add(name);
  return asts.n;
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


void EnumType::traverseDefType(Traversal* traversal) {
  valList->traverseList(traversal, false);
  initDefault->traverse(traversal, false);
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
  Type(TYPE_ARRAY, init_elementType->initDefault),
  domain(init_domain),
  elementType(init_elementType)
{}


void ArrayType::traverseDefType(Traversal* traversal) {
  domain->traverse(traversal, false);
  elementType->traverse(traversal, false);
  initDefault->traverse(traversal, false);
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


UserType::UserType(Type* init_definition, Expr* init_initDefault) :
  Type(TYPE_USER, init_initDefault),
  definition(init_definition)
{}


bool UserType::isComplex(void) {
  return definition->isComplex();
}


void UserType::traverseDefType(Traversal* traversal) {
  definition->traverse(traversal, false);
  initDefault->traverse(traversal, false);
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
  codegenIOPrototype(intheadfile, name);
  fprintf(intheadfile, ";\n\n");
  
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


int
UserType::getTypes(Vec<BaseAST *> &asts) {
  asts.add(definition);
  return asts.n;
}


ClassType::ClassType(ClassType* init_parentClass) :
  Type(TYPE_CLASS, nilExpr),
  parentClass(init_parentClass),
  definition(nilStmt),
  scope(NULL),
  constructor(nilFnDefStmt)
{}


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
  initDefault->traverse(traversal, false);
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
  Type(TYPE_TUPLE, nilExpr)
{
  components.add(firstType);
}


void TupleType::addType(Type* additionalType) {
  components.add(additionalType);
}


void TupleType::traverseDefType(Traversal* traversal) {
  for (int i=0; i<components.n; i++) {
    components.v[i]->traverse(traversal, false);
  }
  initDefault->traverse(traversal, false);
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
