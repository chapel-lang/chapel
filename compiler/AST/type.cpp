#include <typeinfo>
#define TYPE_EXTERN
#include "expr.h"
#include "files.h"
#include "misc.h"
#include "stringutil.h"
#include "symbol.h"
#include "symtab.h"
#include "type.h"
#include "sym.h"


Type::Type(astType_t astType, Expr* init_defaultVal) :
  BaseAST(astType),
  symbol(NULL),
  defaultVal(init_defaultVal),
  asymbol(NULL),
  parentType(NULL)
{
  SET_BACK(defaultVal);
}


void Type::addSymbol(Symbol* newsymbol) {
  symbol = newsymbol;
}


bool Type::isComplex(void) {
  return (this == dtComplex);
}


Type* Type::copy(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone) {
  Type* new_type = copyType(clone, map, analysis_clone);

  new_type->lineno = lineno;
  new_type->filename = filename;
  if (analysis_clone) {
    analysis_clone->clone(this, new_type);
  }
  if (map) {
    map->put(this, new_type);
  }
  return new_type;
}


Type* Type::copyType(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone) {
  INT_FATAL(this, "Unanticipated call to Type::copyType");
  return NULL;
}

Type *Type::instantiate_generic(Map<Type *, Type *> &substitutions) {
  if (astType == TYPE_VARIABLE) {
    Type *t = substitutions.get(this);
    if (t)
      return t;
  }
  return 0;
}

void Type::traverse(Traversal* traversal, bool atTop) {
  if (traversal->processTop || !atTop) {
    traversal->preProcessType(this);
  }
  if (atTop || traversal->exploreChildTypes) {
    if (atTop || symbol == NULL) {
      traverseDefType(traversal);
    }
    else {
      traverseType(traversal);
    }
  }
  if (traversal->processTop || !atTop) {
    traversal->postProcessType(this);
  }
}


void Type::traverseDef(Traversal* traversal, bool atTop) {
  if (traversal->processTop || !atTop) {
    traversal->preProcessType(this);
  }
  TRAVERSE(symbol, traversal, false);
  traverseDefType(traversal);
  if (traversal->processTop || !atTop) {
    traversal->postProcessType(this);
  }
}


void Type::traverseType(Traversal* traversal) {
}


void Type::traverseDefType(Traversal* traversal) {
  TRAVERSE(defaultVal, traversal, false);
}


int Type::rank(void) {
  return 0;
}


void Type::print(FILE* outfile) {
  symbol->print(outfile);
}


void Type::printDef(FILE* outfile) {
  print(outfile);
}

void Type::codegen(FILE* outfile) {
  if (this == dtUnknown) {
    INT_FATAL(this, "Cannot generate unknown type");
  }
  symbol->codegen(outfile);
}


void Type::codegenDef(FILE* outfile) {
  INT_FATAL(this, "Don't know how to codegenDef() for all types yet");
}


void Type::codegenSafeInit(FILE* outfile) {
  if (this == dtString) {
    fprintf(outfile, " = NULL");
  } else {
    // Most types won't need an initializer to be safe, only correct
  }
}


void Type::codegenStringToType(FILE* outfile) {
}


void Type::codegenIORoutines(FILE* outfile) {
}


void Type::codegenConfigVarRoutines(FILE* outfile) {
}


void Type::codegenDefaultFormat(FILE* outfile, bool isRead) {
  fprintf(outfile, "_default_format");
  if (isRead) {
    fprintf(outfile, "_read");
  } else {
    fprintf(outfile, "_write");
  }
  this->codegen(outfile);
}


void Type::codegenConstructors(FILE* outfile) {
}


bool Type::outParamNeedsPtr(void) {
  return true;
}


bool Type::requiresCParamTmp(paramType intent) {
  switch (intent) {
  case PARAM_BLANK:
  case PARAM_IN:
  case PARAM_CONST:
  case PARAM_INOUT:
  case PARAM_OUT:
    return true;
  case PARAM_REF:
    return false;
  default:
    INT_FATAL(this, "case not handled in requiresCParamTmp");
    return false;
  }
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


Type* EnumType::copyType(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone) {
  Type* copy = new EnumType(valList);
  copy->addSymbol(symbol);
  return copy;

    /*
  Symbol* newSyms = valList->copyList(clone, map, analysis_clone);

  if (typeid(*newSyms) != typeid(EnumSymbol)) {
    INT_FATAL(this, "valList is not EnumSymbol in EnumType::copyType()");
    return NULL;
  } else {
    EnumSymbol* newEnums = (EnumSymbol*)newSyms;
    Type* copy = new EnumType(newEnums);
    copy->addSymbol(symbol);
    return copy;
  }
    */
}


void EnumType::traverseDefType(Traversal* traversal) {
  TRAVERSE_LS(valList, traversal, false);
  TRAVERSE(defaultVal, traversal, false);
}


void EnumType::printDef(FILE* outfile) {
  printf("enum ");
  symbol->print(outfile);
  printf(" = ");
  valList->printList(outfile, " | ");
}


void EnumType::codegen(FILE* outfile) {
  symbol->codegen(outfile);
}


void EnumType::codegenDef(FILE* outfile) {
  EnumSymbol* enumSym;
  int last = -1;

  fprintf(outfile, "typedef enum {\n");
  enumSym = valList;
  while (enumSym) {
    enumSym->printDef(outfile);

    if (enumSym->val != last + 1) {
      fprintf(outfile, " = %d", enumSym->val);
    }
    last = enumSym->val;

    enumSym = nextLink(EnumSymbol, enumSym);

    if (enumSym) {
      fprintf(outfile, ",");
    }
    fprintf(outfile, "\n");
  }
  fprintf(outfile, "} ");
  symbol->codegen(outfile);
  fprintf(outfile, ";\n\n");
}


static void codegenIOPrototype(FILE* outfile, Symbol* symbol, bool isRead) {
  fprintf(outfile, "void ");
  if (isRead) {
    fprintf(outfile, "_read");
  } else {
    fprintf(outfile, "_write");
  }
  symbol->codegen(outfile);
  fprintf(outfile, "(FILE* ");
  if (isRead) {
    fprintf(outfile, "infile");
  } else {
    fprintf(outfile, "outfile");
  }
  fprintf(outfile, ", char* format, ");
  symbol->codegen(outfile);
  if (isRead) {
    fprintf(outfile, "*");
  }
  fprintf(outfile, " val)");
}


void EnumType::codegenStringToType(FILE* outfile) {
  EnumSymbol* enumSym = valList;

  fprintf(outfile, "int _convert_string_to_enum");
  symbol->codegen(outfile);
  fprintf(outfile, "(char* inputString, ");
  symbol->codegen(outfile);
  fprintf(outfile, "* val) {\n");
  
  while (enumSym) {
    fprintf(outfile, "if (strcmp(inputString, \"");
    enumSym->codegen(outfile);
    fprintf(outfile, "\") == 0) {\n");
    fprintf(outfile, "*val = ");
    enumSym->codegen(outfile);
    fprintf(outfile, ";\n");
    fprintf(outfile, "} else ");
    enumSym = nextLink(EnumSymbol, enumSym);
  }
  fprintf(outfile, "{ \n");
  fprintf(outfile, "return 0;\n");
  fprintf(outfile, "}\n");
  fprintf(outfile, "return 1;\n}\n\n");
}

void EnumType::codegenIORoutines(FILE* outfile) {
  EnumSymbol* enumSym = valList;
  bool isRead;

  isRead = true;
  codegenIOPrototype(intheadfile, symbol, isRead);
  fprintf(intheadfile, ";\n");
  
  isRead = false;
  codegenIOPrototype(intheadfile, symbol, isRead);
  fprintf(intheadfile, ";\n\n");

  isRead = true;
  codegenIOPrototype(outfile, symbol, isRead);
  fprintf(outfile, " {\n");
  fprintf(outfile, "char* inputString = NULL;\n");
  fprintf(outfile, "_read_string(stdin, format, &inputString);\n");
  fprintf(outfile, "if (!(_convert_string_to_enum");
  symbol->codegen(outfile);
  fprintf(outfile, "(inputString, val))) {\n");
  fprintf(outfile, "fflush(stdout);\n");
  fprintf(outfile, "fprintf (stderr, \"***ERROR:  Not of ");
  symbol->codegen(outfile);
  fprintf(outfile, " type***\\n\");\n");
  fprintf(outfile, "exit(0);\n");
  fprintf(outfile, "}\n");
  fprintf(outfile, "}\n\n");

  isRead = false;
  codegenIOPrototype(outfile, symbol, isRead);
  fprintf(outfile, " {\n");
  fprintf(outfile, "switch (val) {\n");
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


void EnumType::codegenConfigVarRoutines(FILE* outfile) {
  fprintf(outfile, "int setInCommandLine");
  symbol->codegen(outfile);
  fprintf(outfile, "(char* varName, ");
  symbol->codegen(outfile);
  fprintf(outfile, "* value, char* moduleName) {\n");
  fprintf(outfile, "int isDefaultValue = 0;\n");
  fprintf(outfile, "int varSet = 0;\n");
  fprintf(outfile, "char* setValue = lookupSetValue(varName, moduleName);\n");
  fprintf(outfile, "if (setValue) {\n");
  fprintf(outfile, "int validEnum = _convert_string_to_enum");
  symbol->codegen(outfile);
  fprintf(outfile, "(setValue, value);\n");
  fprintf(outfile, "if (validEnum) {\n");
  fprintf(outfile, "varSet = 1;\n");
  fprintf(outfile, "} else {\n");
  fprintf(outfile, "fprintf(stderr, \"***Error: \\\"%%s\\\" is not a valid "
	  "value for a config var \\\"%%s\\\" of type ");
  symbol->codegen(outfile);
  fprintf(outfile, "***\\n\", setValue, varName);\n");
  fprintf(outfile, "exit(0);\n");
  fprintf(outfile, "}\n");
  fprintf(outfile, "}\n");
  fprintf(outfile, "return varSet;\n");
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
  Type(TYPE_DOMAIN, NULL),
  numdims(0),
  parent(NULL)
{
  if (init_expr) {
    if (typeid(*init_expr) == typeid(IntLiteral)) {
      numdims = init_expr->intVal();
    } else {
      numdims = init_expr->rank();
      parent = init_expr;
      SET_BACK(parent);
    }
  }
}


DomainType::DomainType(int init_numdims) :
  Type(TYPE_DOMAIN, NULL),
  numdims(init_numdims),
  parent(NULL)
{}


Type* DomainType::copyType(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone) {
  Type* copy;
  if (!parent) {
    copy = new DomainType(numdims);
  } else {
    copy = new DomainType(parent->copy(clone, map, analysis_clone));
  }
  copy->addSymbol(symbol);
  return copy;
}


int DomainType::rank(void) {
  return numdims;
}


void DomainType::print(FILE* outfile) {
  fprintf(outfile, "domain(");
  if (!parent) {
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


void DomainType::codegenDef(FILE* outfile) {
  fprintf(outfile, "typedef struct _");
  symbol->codegen(outfile);
  fprintf(outfile, " {\n");
  fprintf(outfile, "  _dom_perdim dim_info[%d];\n", numdims);
  fprintf(outfile, "} ");
  symbol->codegen(outfile);
  fprintf(outfile, ";\n\n");
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


Type* IndexType::copyType(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone) {
  Type* copy;
  if (!parent) {
    copy = new IndexType(numdims);
  } else {
    copy = new IndexType(parent->copy(clone, map, analysis_clone));
  }
  copy->addSymbol(symbol);
  return copy;
}


void IndexType::print(FILE* outfile) {
  fprintf(outfile, "index(");
  if (!parent) {
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
{
  SET_BACK(domain);
}


Type* ArrayType::copyType(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone) {
  Type* copy = new ArrayType(domain->copy(clone, map, analysis_clone),
			     elementType->copy(clone, map, analysis_clone));
  copy->addSymbol(symbol);
  return copy;
}


void ArrayType::traverseDefType(Traversal* traversal) {
  TRAVERSE(domain, traversal, false);
  TRAVERSE(elementType, traversal, false);
  TRAVERSE(defaultVal, traversal, false);
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
  symbol->codegen(outfile);
}


void ArrayType::codegenDef(FILE* outfile) {
  fprintf(outfile, "typedef struct _");
  symbol->codegen(outfile);
  fprintf(outfile, " {\n");
  fprintf(outfile, "  int elemsize;\n");
  fprintf(outfile, "  int size;\n");
  fprintf(outfile, "  ");
  elementType->codegen(outfile);
  fprintf(outfile, "* base;\n");
  fprintf(outfile, "  ");
  elementType->codegen(outfile);
  fprintf(outfile, "* origin;\n");
  fprintf(outfile, "  ");
  domainType->codegen(outfile);
  fprintf(outfile, "* domain;\n");
  fprintf(outfile, "  _arr_perdim dim_info[%d];\n", domainType->numdims);
  fprintf(outfile, "} ");
  symbol->codegen(outfile);
  fprintf(outfile, ";\n");

  fprintf(outfile, "void _write");
  symbol->codegen(outfile);
  fprintf(outfile, "(FILE* F, char* format, ");
  symbol->codegen(outfile);
  fprintf(outfile, " arr);\n\n");

  fprintf(codefile, "void _write");
  symbol->codegen(codefile);
  fprintf(codefile, "(FILE* F, char* format, ");
  symbol->codegen(codefile);
  fprintf(codefile, " arr) {\n");
  for (int dim = 0; dim < domainType->numdims; dim++) {
    fprintf(codefile, "  int i%d;\n", dim);
  }
  domainType->codegen(codefile);
  fprintf(codefile, "* const dom = arr.domain;\n\n");
  for (int dim = 0; dim < domainType->numdims; dim++) {
    fprintf(codefile, "for (i%d=dom->dim_info[%d].lo; i%d<=dom"
	    "->dim_info[%d].hi; i%d+=dom->dim_info[%d].str) {\n",
	    dim, dim, dim, dim, dim, dim);
  }
  fprintf(codefile, "fprintf(F, format, _ACC%d(arr, i0", domainType->numdims);
  for (int dim = 1; dim < domainType->numdims; dim++) {
    fprintf(codefile, ", i%d", dim);
  }
  fprintf(codefile, "));\n");
  fprintf(codefile, "if (i%d<dom->dim_info[%d].hi) {\n",
	  domainType->numdims-1, domainType->numdims-1);
  fprintf(codefile, "fprintf(F, \" \");\n");
  fprintf(codefile, "}\n");
  fprintf(codefile, "}\n");
  for (int dim = 1; dim < domainType->numdims; dim++) {
    fprintf(codefile, "fprintf(F, \"\\n\");\n");
    fprintf(codefile, "}\n");
  }
  fprintf(codefile, "}\n");
}


void ArrayType::codegenDefaultFormat(FILE* outfile, bool isRead) {
  elementType->codegenDefaultFormat(outfile, isRead);
}


UserType::UserType(Type* init_definition, Expr* init_defaultVal) :
  Type(TYPE_USER, init_defaultVal),
  definition(init_definition)
{}


Type* UserType::copyType(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone) {
  Type* copy = new UserType(definition,
			    defaultVal->copy(clone, map, analysis_clone));
  copy->addSymbol(symbol);
  return copy;
}


bool UserType::isComplex(void) {
  return definition->isComplex();
}


void UserType::traverseDefType(Traversal* traversal) {
  TRAVERSE(definition, traversal, false);
  TRAVERSE(defaultVal, traversal, false);
}


void UserType::printDef(FILE* outfile) {
  fprintf(outfile, "type ");
  symbol->print(outfile);
  fprintf(outfile, " = ");
  definition->print(outfile);
}


void UserType::codegen(FILE* outfile) {
  symbol->codegen(outfile);
}


void UserType::codegenDef(FILE* outfile) {
  fprintf(outfile, "typedef ");
  definition->codegen(outfile);
  fprintf(outfile, " ");
  symbol->codegen(outfile);
  fprintf(outfile, ";\n");
}


// TODO: We should probably instead have types print out
// their own write routines and have UserType print its
// definition's write routine

static void codegenIOPrototypeBody(FILE* outfile, Symbol* symbol, Type* definition, bool isRead) {
  codegenIOPrototype(outfile, symbol, isRead);
  fprintf(outfile, " {\n");
  if (isRead) {
    fprintf(outfile, " _read");
  } else {
  fprintf(outfile, "  _write");
  }
  definition->codegen(outfile);  
  if (isRead) {
    fprintf(outfile, "(infile, format, val);\n");
  } else {
    fprintf(outfile, "(outfile, format, val);\n");
  }    
  fprintf(outfile, "}\n");
}    


void UserType::codegenIORoutines(FILE* outfile) {
  bool isRead;

  isRead = true;
  codegenIOPrototype(intheadfile, symbol, isRead);
  fprintf(intheadfile, ";\n");

  isRead = false;
  codegenIOPrototype(intheadfile, symbol, isRead);
  fprintf(intheadfile, ";\n\n");

  isRead = true;
  codegenIOPrototypeBody(outfile, symbol, definition, isRead);
  fprintf(outfile, "\n\n");

  isRead = false;
  codegenIOPrototypeBody(outfile, symbol, definition, isRead);
}


void UserType::codegenDefaultFormat(FILE* outfile, bool isRead) {
  definition->codegenDefaultFormat(outfile, isRead);
}


ClassType::ClassType(bool isValueClass, bool isUnion,
		     ClassType* init_parentClass,
		     Stmt* init_constructor,
		     SymScope* init_classScope) :
  Type(TYPE_CLASS, NULL),
  value(isValueClass),
  union_value(isUnion),
  parentClass(init_parentClass),
  constructor(init_constructor),
  classScope(init_classScope)
{
  declarationList = NULL;
  fields.clear();
  methods.clear();
  types.clear();
  SET_BACK(constructor);
}


Type* ClassType::copyType(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone) {
  TypeDefStmt* def_stmt = dynamic_cast<TypeDefStmt*>(symbol->defPoint);

  if (!def_stmt) {
    INT_FATAL(this, "Attempt to copy ClassType not defined in TypeDefStmt");
  }

  //  static int uid = 0;
  ClassType* copy_type = new ClassType(value, union_value);
  //  TypeSymbol* copy_symbol = new TypeSymbol(glomstrings(3, symbol->name, "_copy_", intstring(uid++)), copy_type);
  //  copy_type->addSymbol(copy_symbol);
  Symboltable::pushScope(SCOPE_CLASS);

  Stmt* new_decls = NULL;
  Stmt* old_decls = declarationList;
  while (old_decls) {
    if (FnDefStmt* def = dynamic_cast<FnDefStmt*>(old_decls)) {
      copy_type->methods.add(def->fn);
      //Symboltable::define(def->fn);
    }
    else {
      new_decls = appendLink(new_decls, old_decls->copy(true, map, analysis_clone));
    }
    old_decls = nextLink(Stmt, old_decls);
  }
  copy_type->addDeclarations(new_decls);
  SymScope* copy_scope = Symboltable::popScope();
  copy_type->setClassScope(copy_scope);
  copy_type->buildConstructor();

  //  TypeDefStmt* copy_def = new TypeDefStmt(copy_symbol);
  //  copy_symbol->setDefPoint(copy_def);
  //  copy_scope->setContext(copy_def, copy_symbol);
  //  def_stmt->insertBefore(copy_def);
  return copy_type;
}


void ClassType::addDeclarations(Stmt* newDeclarations, Stmt* beforeStmt) {
  Stmt* tmp = newDeclarations;
  while (tmp) {
    if (FnDefStmt* fn_def_stmt = dynamic_cast<FnDefStmt*>(tmp)) {
      fn_def_stmt->fn->classBinding = this->symbol;
      fn_def_stmt->fn->method_type = PRIMARY_METHOD;
      methods.add(fn_def_stmt->fn);
    }
    else if (VarDefStmt* var_def_stmt = dynamic_cast<VarDefStmt*>(tmp)) {
      fields.add(var_def_stmt->var);
    }
    else if (TypeDefStmt* type_def_stmt = dynamic_cast<TypeDefStmt*>(tmp)) {
      types.add(type_def_stmt->type_sym);
    }
    tmp = nextLink(Stmt, tmp);
  }
  if (!declarationList) {
    declarationList = appendLink(declarationList, newDeclarations);
    SET_BACK(declarationList);
  }
  else if (beforeStmt) {
    beforeStmt->insertBefore(newDeclarations);
  }
  else {
    Stmt* last = dynamic_cast<Stmt*>(declarationList->tail());
    last->insertAfter(newDeclarations);
  }
}


void ClassType::buildConstructor(void) {
  if (symbol && !symbol->parentScope->isInternal()) {
    /* create default constructor */

    char* constructorName = glomstrings(2, "_construct_", symbol->name);
    FnSymbol* newFunSym = Symboltable::startFnDef(new FnSymbol(constructorName));
    newFunSym->cname = glomstrings(2, "_construct_", symbol->cname);
    if (value || union_value) {
      BlockStmt* body = Symboltable::startCompoundStmt();
      VarSymbol* this_insert = new VarSymbol("this", this);
      VarDefStmt* body1 = new VarDefStmt(this_insert);
      this_insert->setDefPoint(body1);
      ReturnStmt* body2 =  new ReturnStmt(new Variable(this_insert));
      body1->append(body2);
      Symboltable::finishCompoundStmt(body, body1);
      constructor = Symboltable::finishFnDef(newFunSym, NULL, this, body);
    }
    else {
      Expr* argList = new IntLiteral("1", 1);
      argList = appendLink(argList, new SizeofExpr(this));
      argList = appendLink(argList, new StringLiteral("string"));

      BlockStmt* body = new BlockStmt(
			  new ReturnStmt(
			    new CastExpr(this, 
			      new FnCall(
			        new Variable(
				  Symboltable::lookupInternal("_chpl_malloc")), 
				argList))
			    )
			  );
      constructor = Symboltable::finishFnDef(newFunSym, NULL, this, body);
    }
    /** Add test tags for unions: This is a little ugly, it should
	insert the enum statement that we generate in codegen before
	the union def stmt.
    **/
    if (union_value) {
      forv_Vec(VarSymbol, tmp, fields) {
	EnumSymbol* idtag = new EnumSymbol(glomstrings(4, "_", symbol->name, "_union_id_", tmp->name), NULL);
	idtag->setDefPoint(NULL); // SHOULD BE REAL statement for declaring this enum, UGH...short-term
      }
    }
  }
  SET_BACK(constructor);
}


void ClassType::setClassScope(SymScope* init_classScope) {
  classScope = init_classScope;
}


void ClassType::traverseDefType(Traversal* traversal) {
  SymScope* prevScope;
  if (classScope) {
    prevScope = Symboltable::setCurrentScope(classScope);
  }
  TRAVERSE_LS(declarationList, traversal, false);
  TRAVERSE_LS(constructor, traversal, false);
  TRAVERSE(defaultVal, traversal, false);
  if (classScope) {
    Symboltable::setCurrentScope(prevScope);
  }
}


void ClassType::codegen(FILE* outfile) {
  symbol->codegen(outfile);
}


void ClassType::codegenDef(FILE* outfile) {
  if (union_value) {
    fprintf(outfile, "typedef enum _");
    symbol->codegen(outfile);
    fprintf(outfile, "_union_id_def {\n");
    bool first = true;
    forv_Vec(VarSymbol, tmp, fields) {
      if (!first) {
	if (tmp) {
	  fprintf(outfile, ",");
	}
	fprintf(outfile, "\n");
      }
      else {
	first = false;
      }
      fprintf(outfile, "_%s_union_id_", symbol->name);
      tmp->codegen(outfile);
    }
    fprintf(outfile, "} _");
    symbol->codegen(outfile);
    fprintf(outfile, "_union_id;\n\n");
  }
  fprintf(outfile, "typedef struct _");
  symbol->codegen(outfile);
  fprintf(outfile, "_def {\n");
  if (union_value) {
    fprintf(outfile, "_");
    symbol->codegen(outfile);
    fprintf(outfile, "_union_id _chpl_union_tag;\n");
    fprintf(outfile, "union _chpl_union_def {\n");
  }
  for (Stmt* tmp = declarationList; tmp; tmp = nextLink(Stmt, tmp)) {
    if (VarDefStmt* def_stmt = dynamic_cast<VarDefStmt*>(tmp)) {
      def_stmt->var->codegenDef(outfile);
    }
  }
  /*
  forv_Vec(VarSymbol, tmp, fields) {
    tmp->codegenDef(outfile);
    fprintf(outfile, "\n");
  }
  */
  if (union_value) {
    fprintf(outfile, "} _chpl_union;\n");
  }
  fprintf(outfile, "} ");
  if (value || union_value) {
    symbol->codegen(outfile);
    fprintf(outfile, ";\n\n");
  }
  else {
    fprintf(outfile, "_");
    symbol->codegen(outfile);
    fprintf(outfile,", *");
    symbol->codegen(outfile);
    fprintf(outfile, ";\n\n");
  }
  forv_Vec(FnSymbol, fn, methods) {
    // Check to see if this is where it is defined
    if (fn->parentScope->symContext->type == this) {
      fn->codegenDef(codefile);
    }
    fprintf(codefile, "\n");
  }
}


void ClassType::codegenConstructors(FILE* outfile) {
  constructor->codegenList(outfile, "\n");
}


void ClassType::codegenIORoutines(FILE* outfile) {
  bool isRead;

  isRead = true;
  codegenIOPrototype(intheadfile, symbol, isRead);
  fprintf(intheadfile, ";\n");

  isRead = false;
  codegenIOPrototype(intheadfile, symbol, isRead);
  fprintf(intheadfile, ";\n\n");

  isRead = true;
  codegenIOPrototype(outfile, symbol, isRead);
  fprintf(outfile, "{\n");
  fprintf(outfile, "}\n");

  isRead = false;
  codegenIOPrototype(outfile, symbol, isRead);
  fprintf(outfile, "{\n");
  fprintf(outfile, "}\n");
}


TupleType::TupleType(Type* firstType) :
  Type(TYPE_TUPLE, NULL)
{
  components.add(firstType);
  defaultVal = new Tuple(firstType->defaultVal->copy());
  SET_BACK(defaultVal);
}


void TupleType::addType(Type* additionalType) {
  components.add(additionalType);
  if (Tuple* tuple = dynamic_cast<Tuple*>(defaultVal)) {
    tuple->exprs = appendLink(tuple->exprs, additionalType->defaultVal->copy());
  }
}


void TupleType::rebuildDefaultVal(void) {
  Tuple* tuple = new Tuple(NULL);
  forv_Vec(Type, component, components) {
    tuple->exprs = appendLink(tuple->exprs, component->defaultVal->copy());
  }
  defaultVal = tuple;
  SET_BACK(defaultVal);
}


Type* TupleType::copyType(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone) {
  TupleType* newTupleType =
    new TupleType(components.v[0]->copy(clone, map, analysis_clone));
  for (int i=1; i<components.n; i++) {
    newTupleType->addType(components.v[i]->copy(clone, map, analysis_clone));
  }
  newTupleType->addSymbol(symbol);
  return newTupleType;
}


void TupleType::traverseDefType(Traversal* traversal) {
  for (int i=0; i<components.n; i++) {
    TRAVERSE(components.v[i], traversal, false);
  }
  TRAVERSE(defaultVal, traversal, false);
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
  symbol->codegen(outfile);
}


void TupleType::codegenDef(FILE* outfile) {
  fprintf(outfile, "typedef struct _");
  symbol->codegen(outfile);
  fprintf(outfile, " {\n");
  int i = 0;
  forv_Vec(Type, component, components) {
    component->codegen(outfile);
    fprintf(outfile, " _field%d;\n", ++i);
  }
  fprintf(outfile, "} ");
  symbol->codegen(outfile);
  fprintf(outfile, ";\n\n");
}


SumType::SumType(Type* firstType) :
  Type(TYPE_SUM, NULL)
{
  components.add(firstType);
}


void SumType::addType(Type* additionalType) {
  components.add(additionalType);
}


VariableType::VariableType(Type *init_type) :
  Type(TYPE_VARIABLE, NULL), 
  type(init_type)
{}


Type* VariableType::copyType(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone) {
  return new VariableType();
}


void VariableType::codegen(FILE* outfile) {
  INT_FATAL(this, "ERROR:  Cannot codegen a variable type.");
}


UnresolvedType::UnresolvedType(char* init_symbol) :
  Type(TYPE_UNRESOLVED, NULL) {
  symbol = new UnresolvedSymbol(init_symbol);
}


Type* UnresolvedType::copyType(bool clone, Map<BaseAST*,BaseAST*>* map, CloneCallback* analysis_clone) {
  return new UnresolvedType(copystring(symbol->name));
}


void UnresolvedType::codegen(FILE* outfile) {
  INT_FATAL(this, "ERROR:  Cannot codegen an unresolved type.");
}


void initTypes(void) {
  // define built-in types
  dtUnknown = Symboltable::defineBuiltinType("???", "???", NULL);
  dtVoid = Symboltable::defineBuiltinType("void", "void", NULL);

  dtBoolean = Symboltable::defineBuiltinType("boolean", "_boolean",
					     new BoolLiteral("false", false));
  dtInteger = Symboltable::defineBuiltinType("integer", "_integer64",
					     new IntLiteral("0", 0));
  dtFloat = Symboltable::defineBuiltinType("float", "_float64",
					   new FloatLiteral("0.0", 0.0));
  dtComplex = Symboltable::defineBuiltinType("complex", "_complex128",
					     new FloatLiteral("0.0", 0.0));
  dtString = Symboltable::defineBuiltinType("string", "_string", 
					    new StringLiteral(""));
  dtNumeric = Symboltable::defineBuiltinType("numeric", "_numeric", NULL);

  dtLocale = Symboltable::defineBuiltinType("locale", "_locale", NULL);
}


void findInternalTypes(void) {
  dtTuple = Symboltable::lookupInternalType("Tuple")->type;
  dtIndex = Symboltable::lookupInternalType("Index")->type;
  dtDomain = Symboltable::lookupInternalType("Domain")->type;
  dtArray = Symboltable::lookupInternalType("Array")->type;
}

// you can use something like the following cache to 
// find an existing SumType.

// eventually this sort of a cache will have to be
// implemented over all 'structural' types, e.g.
// records

class LUBCacheHashFns : public gc {
 public:
  static unsigned int hash(Symbol *a) {
    unsigned int h = 0;
    Sym *s = a->asymbol->sym;
    for (int i = 0; i < s->has.n; i++)
      h += open_hash_multipliers[i % 256] * (uintptr_t)s->has.v[i];
    return h;
  }
  static int equal(Symbol *aa, Symbol *ab) { 
    Sym *a = aa->asymbol->sym, *b = ab->asymbol->sym;
    if (a->has.n != b->has.n)
      return 0;
    for (int i = 0; i < a->has.n; i++)
      if (a->has.v[i] != b->has.v[i])
	return 0;
    return 1;
  }
};

static class BlockHash<Symbol *, LUBCacheHashFns> lub_cache;


Type *find_or_make_sum_type(Vec<Type *> *types) {
  if (types->n < 2) {
    INT_FATAL("Trying to create sum_type of less than 2 types");
  }
  qsort(types->v, types->n, sizeof(types->v[0]), compar_baseast);
  SumType* new_sum_type = new SumType(types->v[0]);
  for (int i = 1; i <= types->n; i++) {
    new_sum_type->addType(types->v[i]);
  }
  return new_sum_type;
}
