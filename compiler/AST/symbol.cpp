#include <typeinfo>
#include "misc.h"
#include "stmt.h"
#include "symbol.h"
#include "symtab.h"


Symbol::Symbol(astType_t astType, char* init_name, Type* init_type) :
  BaseAST(astType),
  name(init_name),
  type(init_type),
  level(Symboltable::getLevel())
{}


void Symbol::traverse(Traversal* traversal) {
  traversal->preProcessSymbol(this);
  if (traversal->exploreSymbols) {
    traverseSymbol(traversal);
  }
  traversal->postProcessSymbol(this);
  if (next && !next->isNull()) {
    next->traverse(traversal);
  }
}


void Symbol::traverseSymbol(Traversal* traversal) {
  type->traverse(traversal);
}


void Symbol::print(FILE* outfile) {
  fprintf(outfile, "%s", name);
}

void Symbol::codegen(FILE* outfile) {
  fprintf(outfile, "%s", name);
}


void Symbol::printDef(FILE* outfile) {
  print(outfile);
}


void Symbol::printDefList(FILE* outfile, char* separator) {
  Symbol* ptr;

  printDef(outfile);
  ptr = nextLink(Symbol, this);
  while (ptr != NULL) {
    fprintf(outfile, "%s", separator);
    ptr->printDef(outfile);
    ptr = nextLink(Symbol, ptr);
  }
}


void Symbol::codegenDef(FILE* outfile) {
  type->codegen(outfile);
  fprintf(outfile, " ");
  this->codegen(outfile);
}  


void Symbol::codegenDefList(FILE* outfile, char* separator) {
  Symbol* ptr;

  codegenDef(outfile);
  ptr = nextLink(Symbol, this);
  while (ptr != NULL) {
    fprintf(outfile, "%s", separator);
    ptr->codegenDef(outfile);
    ptr = nextLink(Symbol, ptr);
  }
}


NullSymbol::NullSymbol(void) :
  Symbol(SYMBOL_NULL, "NullSymbol", new NullType())
{}


bool NullSymbol::isNull(void) {
  return true;
}


UseBeforeDefSymbol::UseBeforeDefSymbol(char* init_name) :
  Symbol(SYMBOL_USEBEFOREDEF, init_name)
{}


VarSymbol::VarSymbol(char* init_name, Type* init_type, varType init_varClass, 
		     bool init_isConst) :
  Symbol(SYMBOL_VAR, init_name, init_type),
  varClass(init_varClass),
  isConst(init_isConst)
{}


void VarSymbol::printWithType(FILE* outfile) {
  print(outfile);
  if (type != NULL) {
    fprintf(outfile, ": ");
    type->print(outfile);
  }
}


static char* paramTypeNames[NUM_PARAM_TYPES] = {
  "in",
  "inout",
  "out",
  "const"
};


NullVarSymbol::NullVarSymbol() :
  VarSymbol("NullVarSymbol", new NullType())
{}


bool NullVarSymbol::isNull(void) {
  return true;
}


ParamSymbol::ParamSymbol(paramType init_usage, char* init_name, 
			 Type* init_type) :
  Symbol(SYMBOL_PARAM, init_name, init_type),
  usage(init_usage)
{}


void ParamSymbol::printDef(FILE* outfile) {
  fprintf(outfile, "%s ", paramTypeNames[usage]);
  Symbol::print(outfile);
  fprintf(outfile, ": ");
  type->print(outfile);
}


void ParamSymbol::codegenDef(FILE* outfile) {
  if (type == dtUnknown) {       // BLC: This is a temporary hack until
    dtInteger->codegen(outfile); // we get type inference hooked in; assume
  } else {                       // all params are integers...  Inherit
    type->codegen(outfile);      // from Symbol::codegenDef() once this
  }                              // hack is fixed
  fprintf(outfile, " ");
  this->codegen(outfile);
}


TypeSymbol::TypeSymbol(char* init_name, Type* init_definition) :
  Symbol(SYMBOL_TYPE, init_name, init_definition)
{}


ClassSymbol::ClassSymbol(char* init_name, ClassType* init_class) :
  TypeSymbol(init_name, init_class)
{
  astType = SYMBOL_CLASS;
}


NullClassSymbol::NullClassSymbol(void) :
  ClassSymbol("NullClass", NULL)
{}


bool NullClassSymbol::isNull(void) {
  return true;
}


ClassType* ClassSymbol::getType(void) {
  if (typeid(*type) != typeid(ClassType)) {
    INT_FATAL(this, "ClassSymbol has non-ClassType");
  }
  return (ClassType*)type;
}


ReduceSymbol::ReduceSymbol(char* init_name, ClassType* init_class) :
  ClassSymbol(init_name, init_class)
{
  astType = SYMBOL_REDUCE;
}



FnSymbol::FnSymbol(char* init_name, Symbol* init_formals, Type* init_retType,
		   Stmt* init_body, bool init_exportMe) :
  Symbol(SYMBOL_FN, init_name, init_retType),
  exportMe(init_exportMe),
  formals(init_formals),
  body(init_body)
{}


void FnSymbol::traverseSymbol(Traversal* traversal) {
  formals->traverse(traversal);
  body->traverse(traversal);
}


void FnSymbol::codegenDef(FILE* outfile) {
  if (!exportMe) {
    fprintf(outfile, "static ");
  }
  if (type->isNull()) {
    fprintf(outfile, "void");
  } else {
    type->codegen(outfile);
  }
  fprintf(outfile, " ");
  this->codegen(outfile);
  fprintf(outfile, "(");
  if (formals->isNull()) {
    fprintf(outfile, "void");
  } else {
    formals->codegenDefList(outfile, ", ");
  }
  fprintf(outfile, ")");
}


EnumSymbol::EnumSymbol(char* init_name, int init_val) :
  Symbol(SYMBOL_ENUM, init_name),
  val(init_val)
{}
