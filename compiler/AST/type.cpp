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
#include "../traversals/fixup.h"
#include "../traversals/updateSymbols.h"
#include "../traversals/collectASTS.h"


// Utilities for building write functions
static void addWriteStmt(AList<Stmt>* body, Expr* arg) {
  Expr* write = new Variable(new UnresolvedSymbol("write"));
  body->insertAtTail(new ExprStmt(new ParenOpExpr(write, new AList<Expr>(arg))));
}


Type::Type(astType_t astType, Expr* init_defaultVal) :
  BaseAST(astType),
  symbol(NULL),
  defaultVal(init_defaultVal),
  defaultConstructor(NULL),
  asymbol(NULL),
  parentType(NULL),
  metaType(NULL)
{ }


void Type::addSymbol(Symbol* newsymbol) {
  symbol = newsymbol;
}


Type* Type::copy(bool clone, Map<BaseAST*,BaseAST*>* map) {
  if (!this) {
    return this;
  }

  if (map == NULL) {
    map = new Map<BaseAST*,BaseAST*>();
  }
  Type* new_type = copyInternal(clone, map);
  TRAVERSE(new_type, new UpdateSymbols(map), true);
  return new_type;
}


Type* Type::copyInternal(bool clone, Map<BaseAST*,BaseAST*>* map) {
  Type* new_type = copyType(clone, map);

  new_type->lineno = lineno;
  new_type->filename = filename;
  if (map) {
    map->put(this, new_type);
  }
  return new_type;
}


Type* Type::copyType(bool clone, Map<BaseAST*,BaseAST*>* map) {
  INT_FATAL(this, "Unanticipated call to Type::copyType");
  return NULL;
}


Type *Type::instantiate_generic(Map<BaseAST *, BaseAST *> &substitutions) {
  if (astType == TYPE_VARIABLE) {
    if (Type *t = dynamic_cast<Type*>(substitutions.get(this)))
      return t;
  }
  return 0;
}


Type *Type::getMetaType() {
  if (astType == TYPE_META) {
    INT_FATAL(this, "Attempt to get MetaMeta Type");
  }
  if (metaType)
    return metaType;
  return metaType = new MetaType(this);
}


void Type::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == defaultVal) {
    defaultVal = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in Type::replaceChild(old, new)");
  }
}


void Type::traverse(Traversal* traversal, bool atTop) {
  if (traversal->processTop || !atTop) {
    currentLineno = lineno;
    currentFilename = filename;
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
    currentLineno = lineno;
    currentFilename = filename;
    traversal->postProcessType(this);
  }
}


void Type::traverseDef(Traversal* traversal, bool atTop) {
  if (traversal->processTop || !atTop) {
    currentLineno = lineno;
    currentFilename = filename;
    traversal->preProcessType(this);
  }
  TRAVERSE(symbol, traversal, false);
  traverseDefType(traversal);
  if (traversal->processTop || !atTop) {
    currentLineno = lineno;
    currentFilename = filename;
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


void Type::codegenPrototype(FILE* outfile) { }


void Type::codegenSafeInit(FILE* outfile) {
  // SJD: Is this ever going to be used?
  // Used to be used to initialize strings to NULL
}


void Type::codegenStringToType(FILE* outfile) {
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


bool Type::outParamNeedsPtr(void) {
  return true;
}


bool Type::requiresCParamTmp(paramType intent) {
  if (intent == PARAM_BLANK) {
    if (blankIntentImpliesRef()) {
      intent = PARAM_REF;
    } else {
      intent = PARAM_CONST;
    }
  }
  switch (intent) {
  case PARAM_BLANK:
    INT_FATAL(this, "should never have reached PARAM_BLANK case");
  case PARAM_CONST:
  case PARAM_IN:
    // if these are implemented using C's pass-by-value, then C
    // effectively puts in the temp for us
    if (implementedUsingCVals()) {
      return false;
    } else {
      return true;
    }
  case PARAM_INOUT:
    // here a temp is probably always needed in order to avoid
    // affecting the original value
  case PARAM_OUT:
    // and here it's needed to set up the default value of the type
    return true;
  case PARAM_REF:
    // here, a temp should never be needed
    return false;
  case PARAM_PARAMETER:
  case PARAM_TYPE:
    return false;  // Should never be encountered EVENTUALLY
  default:
    INT_FATAL(this, "case not handled in requiresCParamTmp");
    return false;
  }
}


bool Type::blankIntentImpliesRef(void) {
  return false;
}


bool Type::implementedUsingCVals(void) {
  if (this == dtBoolean ||
      this == dtInteger ||
      this == dtFloat) {
    return true;
  } else {
    return false;
 }
}

Type* Type::getType(){
  return this;
}


bool Type::hasDefaultWriteFunction(void) {
  return false;
}


AList<Stmt>* Type::buildDefaultWriteFunctionBody(ParamSymbol* arg) {
  return new AList<Stmt>();
}


bool Type::hasDefaultReadFunction(void) {
  return false;
}


AList<Stmt>* Type::buildDefaultReadFunctionBody(ParamSymbol* arg) {
  return new AList<Stmt>();
}


FnType::FnType(void) :
  Type(TYPE_FN, NULL)
{}


void FnType::codegen(FILE* outfile) {
  INT_FATAL(this, "FnType is not yet implemented!\n");
}


void FnType::codegenDef(FILE* outfile) {
  INT_FATAL(this, "FnType is not yet implemented\n");
}


EnumType::EnumType(AList<EnumSymbol>* init_valList) :
  Type(TYPE_ENUM, new Variable(init_valList->first())),
  valList(init_valList)
{
  Symbol* val = valList->first();
  while (val) {
    val->type = this;
    val = valList->next();
  }
}


Type* EnumType::copyType(bool clone, Map<BaseAST*,BaseAST*>* map) {
  Type* copy = new EnumType(valList);
  copy->addSymbol(symbol);
  return copy;
}


void EnumType::traverseDefType(Traversal* traversal) {
  valList->traverse(traversal, false);
  TRAVERSE(defaultVal, traversal, false);
}


void EnumType::printDef(FILE* outfile) {
  printf("enum ");
  symbol->print(outfile);
  printf(" = ");
  valList->print(outfile, " | ");
}


void EnumType::codegenDef(FILE* outfile) {
  EnumSymbol* enumSym;
  int last = -1;

  fprintf(outfile, "typedef enum {\n");
  enumSym = valList->first();
  while (enumSym) {
    enumSym->printDef(outfile);

    if (enumSym->val != last + 1) {
      fprintf(outfile, " = %d", enumSym->val);
    }
    last = enumSym->val;

    enumSym = valList->next();

    if (enumSym) {
      fprintf(outfile, ",");
    }
    fprintf(outfile, "\n");
  }
  fprintf(outfile, "} ");
  symbol->codegen(outfile);
  fprintf(outfile, ";\n\n");
}


void EnumType::codegenStringToType(FILE* outfile) {
  EnumSymbol* enumSym = valList->first();

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
    enumSym = valList->next();
  }
  fprintf(outfile, "{ \n");
  fprintf(outfile, "return 0;\n");
  fprintf(outfile, "}\n");
  fprintf(outfile, "return 1;\n}\n\n");
}


void EnumType::codegenConfigVarRoutines(FILE* outfile) {
  fprintf(outfile, "int setInCommandLine");
  symbol->codegen(outfile);
  fprintf(outfile, "(char* varName, ");
  symbol->codegen(outfile);
  fprintf(outfile, "* value, char* moduleName) {\n");
  fprintf(outfile, "int varSet = 0;\n");
  fprintf(outfile, "char* setValue = lookupSetValue(varName, moduleName);\n");
  fprintf(outfile, "if (setValue) {\n");
  fprintf(outfile, "int validEnum = _convert_string_to_enum");
  symbol->codegen(outfile);
  fprintf(outfile, "(setValue, value);\n");
  fprintf(outfile, "if (validEnum) {\n");
  fprintf(outfile, "varSet = 1;\n");
  fprintf(outfile, "} else {\n");
  fprintf(outfile, "char* message = _glom_strings(5, \"\\\"\", setValue, ");
  fprintf(outfile, "\"\\\" is not a valid \"\n");
  fprintf(outfile, "\"value for a config var \\\"\"");
  fprintf(outfile, ", varName,\n");
  fprintf(outfile, "\"\\\" of type ");
  symbol->codegen(outfile);
  fprintf(outfile, "\");\n");
  fprintf(outfile, "printError(message);\n");
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


bool EnumType::implementedUsingCVals(void) {
  return true;
}


bool EnumType::hasDefaultWriteFunction(void) {
  return true;
}


AList<Stmt>* EnumType::buildDefaultWriteFunctionBody(ParamSymbol* arg) {
  return new AList<Stmt>(new ExprStmt(new ParenOpExpr(new Variable(Symboltable::lookupInternal("_EnumWriteStopgap")), new AList<Expr>(new Variable(arg)))));
}


bool EnumType::hasDefaultReadFunction(void) {
  return true;
}


AList<Stmt>* EnumType::buildDefaultReadFunctionBody(ParamSymbol* arg) {
  return new AList<Stmt>(new ExprStmt(new ParenOpExpr(new Variable(Symboltable::lookupInternal("_EnumReadStopgap")), new AList<Expr>(new Variable(arg)))));
}


DomainType::DomainType(Expr* init_expr) :
  Type(TYPE_DOMAIN, NULL),
  numdims(0),
  parent(NULL),
  initExpr(init_expr),
  idxType(NULL)
{}


DomainType::DomainType(int init_numdims) :
  Type(TYPE_DOMAIN, NULL),
  numdims(init_numdims),
  parent(NULL),
  idxType(NULL)
{
  //RED -- keep track of the initialization expression for domains
  //can help with creating index types
  initExpr = new IntLiteral(intstring(init_numdims), init_numdims);
}


void DomainType::computeRank(void) {
  if (initExpr) {
    if ((typeid(*initExpr) == typeid(IntLiteral)) ||
        (initExpr->isParam() && initExpr->typeInfo() == dtInteger)) {
      numdims = initExpr->intVal();
      parent = NULL;
    } else {
      numdims = initExpr->rank();
      parent = initExpr;
    }
    idxType = new IndexType(initExpr->copy());
    idxType->domainType = this;
  }
}


Type* DomainType::copyType(bool clone, Map<BaseAST*,BaseAST*>* map) {
  Type* copy;
  if (!parent) {
    copy = new DomainType(numdims);
  } else {
    copy = new DomainType(parent->copyInternal(clone, map));
  }
  copy->addSymbol(symbol);
  return copy;
}


int DomainType::rank(void) {
  return numdims;
}


void DomainType::traverseDefType(Traversal* traversal) {
  TRAVERSE(initExpr, traversal, false);
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


bool DomainType::blankIntentImpliesRef(void) {
  return true;
}


bool DomainType::hasDefaultWriteFunction(void) {
  return true;
}


AList<Stmt>* DomainType::buildDefaultWriteFunctionBody(ParamSymbol* arg) {
  return new AList<Stmt>(new ExprStmt(new ParenOpExpr(new Variable(Symboltable::lookupInternal("_DomainWriteStopgap")), new AList<Expr>(new Variable(arg)))));
}


IndexType::IndexType(Type* init_idxType):
  Type(TYPE_INDEX, init_idxType->defaultVal),
  idxType(init_idxType) 
{
  domainType = NULL;
}


IndexType::IndexType(Expr* init_expr) :
  Type(TYPE_INDEX, NULL),
  idxExpr(init_expr),
  domainType(NULL)
{
  if (typeid(*init_expr) == typeid(IntLiteral)) {
    //RED: ugly hack -- apparently Tuple(1) does not work...
    //I am not sure I understand why...
    if (init_expr->intVal() == 1) {
      idxType = dtInteger;
    }
    else {
      TupleType* newTType = new TupleType();
      for (int i = 0; i < init_expr->intVal(); i++){
        newTType->addType(init_expr->typeInfo());
      }
      //idxType->defaultVal = newTType->defaultVal->copy();
      //TypeSymbol* tupleSymbol = TypeSymbol::lookupOrDefineTupleTypeSymbol(&newTType->components);
      idxType = newTType;
      //idxType = tupleSymbol->type;
    } 
  } else {
    //RED: init_expr in this case is a user-defined type
    idxType = init_expr->typeInfo(); 
  }
}


Type* IndexType::copyType(bool clone, Map<BaseAST*,BaseAST*>* map) {
  Type* copy = new IndexType(idxType->copyInternal(clone, map));
  copy->addSymbol(symbol);
  return copy;
}


void IndexType::print(FILE* outfile) {
  fprintf(outfile, "index(");
  idxExpr->print(outfile);
  fprintf(outfile, ")");
}

void IndexType::codegenDef(FILE* outfile) {
  fprintf(outfile, "typedef ");
  if (typeid(*idxType) == typeid(*dtInteger)) {
    fprintf(outfile, "struct {\n");
    int i = 0;
    idxType->codegen(outfile);
     fprintf(outfile, " _field%d;\n", ++i);
    fprintf(outfile, "} ");
    fprintf(outfile, " ");
    symbol->codegen(outfile);
    fprintf(outfile, ";\n\n");
    return;
  }
  //fprintf(outfile, "typedef ");
  TupleType* tt = dynamic_cast<TupleType*>(idxType);
  if (tt){
    fprintf(outfile, "struct {\n");
    int i = 0;
    forv_Vec(Type, component, tt->components) {
      component->codegen(outfile);
      fprintf(outfile, " _field%d;\n", ++i);
    }
    fprintf(outfile, "} ");
    symbol->codegen(outfile);
    fprintf(outfile, ";\n\n");
  } else {
    idxType->codegenDef(outfile);
    symbol->codegen(outfile);
    fprintf(outfile, ";\n\n");
  }   
}


void IndexType::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == defaultVal) {
    defaultVal = dynamic_cast<Expr*>(new_ast);
  } else if (old_ast == idxExpr) {
    idxExpr = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in Type::replaceChild(old, new)");
  }
}


void IndexType::traverseDefType(Traversal* traversal) {
  if (!(typeid(*idxExpr) == typeid(IntLiteral))) {
    TRAVERSE(idxExpr, traversal, false);
  }
  TRAVERSE(idxType, traversal, false);
  TRAVERSE(defaultVal, traversal, false);
}


Type* IndexType::getType(){
  return idxType; 
}


SeqType::SeqType(Type* init_elementType):
  ClassType(TYPE_SEQ),
  elementType(init_elementType)
{
  defaultVal = NULL; // override class defaultVal of nil
}


Type* SeqType::copyType(bool clone, Map<BaseAST*,BaseAST*>* map) {
  Type* new_type = new SeqType(elementType->copyInternal(clone, map));
  new_type->addSymbol(symbol);
  return new_type;
}


void SeqType::traverseDefType(Traversal* traversal) {
  TRAVERSE(elementType, traversal, false);
  ClassType::traverseDefType(traversal);
}


void SeqType::print(FILE* outfile) {
  fprintf(outfile, "seq(");
  elementType->print(outfile);
  fprintf(outfile, ")");
}


void SeqType::codegenDef(FILE* outfile) {
  ClassType::codegenDef(outfile);

  fprintf(outfile, "void _write");
  symbol->codegen(outfile);
  fprintf(outfile, "(FILE* F, char* format, ");
  symbol->codegen(outfile);
  fprintf(outfile, " seq);\n\n");

  fprintf(codefile, "void _write");
  symbol->codegen(codefile);
  fprintf(codefile, "(FILE* F, char* format, ");
  symbol->codegen(codefile);
  fprintf(codefile, " seq) {\n"); 
  fprintf(codefile, "%s tmp = seq->first;\n", types.v[0]->cname);
  fprintf(codefile, "if (tmp != nil) {");
  fprintf(codefile, "  fprintf(F, \"(/\");\n");
  fprintf(codefile, "  while (tmp != nil) {\n");
  fprintf(codefile, "    fprintf(F, format, tmp->element);\n");
  fprintf(codefile, "    tmp = tmp->next;\n");
  fprintf(codefile, "    if (tmp != nil) {\n");
  fprintf(codefile, "      fprintf(F, \", \");\n");
  fprintf(codefile, "    }\n");
  fprintf(codefile, "  }\n");
  fprintf(codefile, "  fprintf(F, \"/)\");\n");
  fprintf(codefile, "} else {\n");
  fprintf(codefile, "  fprintf(F, \"nil\");\n");
  fprintf(codefile, "}\n");
  fprintf(codefile, "}\n");
}


void SeqType::codegenDefaultFormat(FILE* outfile, bool isRead) {
  elementType->codegenDefaultFormat(outfile, isRead);
}


bool SeqType::implementedUsingCVals(void) {
  return false;
}


SeqType* SeqType::createSeqType(char* new_seq_name, Type* init_elementType) {
  SeqType* new_seq_type = new SeqType(init_elementType);
  Symbol* _seq = Symboltable::lookupInternal("_seq");
  ClassType* _seq_type = dynamic_cast<ClassType*>(_seq->type);
  Symboltable::pushScope(SCOPE_CLASS);
  // we need to copy the sequence's definition except for its element
  // type declaration.  While it's tempting to copy all and then pop
  // the type declaration, this causes problems downstream.  So
  // instead we remove the type declaration before copying and then
  // re-insert it
  Stmt* typeDecl = _seq_type->declarationList->popHead();
  AList<Stmt>* new_decls = _seq_type->declarationList->copy(true);
  _seq_type->declarationList->insertAtHead(typeDecl);

  new_seq_type->addDeclarations(new_decls);
  SymScope* new_seq_scope = Symboltable::popScope();
  new_seq_type->setScope(new_seq_scope);

  TypeSymbol* new_seq_sym = new TypeSymbol(new_seq_name, new_seq_type);
  new_seq_type->addSymbol(new_seq_sym);

  Symbol* _append = Symboltable::lookupInternal("append");
  _append->keepLive = true;
  DefStmt* appendDefStmt = new DefStmt(dynamic_cast<DefExpr*>(_append->defPoint->copy(true)));
  new_seq_type->addDeclarations(new AList<Stmt>(appendDefStmt));

  Symbol* _prepend = Symboltable::lookupInternal("prepend");
  _prepend->keepLive = true;
  DefStmt* prependDefStmt = new DefStmt(dynamic_cast<DefExpr*>(_prepend->defPoint->copy(true)));
  new_seq_type->addDeclarations(new AList<Stmt>(prependDefStmt));

  Symbol* _concat = Symboltable::lookupInternal("concat");
  _concat->keepLive = true;
  DefStmt* concatDefStmt = new DefStmt(dynamic_cast<DefExpr*>(_concat->defPoint->copy(true)));
  new_seq_type->addDeclarations(new AList<Stmt>(concatDefStmt));

  Symbol* _copy = Symboltable::lookupInternal("copy");
  _copy->keepLive = true;
  DefStmt* copyDefStmt = new DefStmt(dynamic_cast<DefExpr*>(_copy->defPoint->copy(true)));
  new_seq_type->addDeclarations(new AList<Stmt>(copyDefStmt));

  /*** set class bindings and this ***/
  forv_Vec(FnSymbol, method, new_seq_type->methods) {
    method->typeBinding = new_seq_sym;
    method->_this = method->formals->first();
    method->cname = glomstrings(3, new_seq_sym->name, "_", method->cname);
  }

  /*** update _seq type to new type ***/
  Map<BaseAST*,BaseAST*>* map = new Map<BaseAST*,BaseAST*>();
  map->put(_seq, new_seq_sym);
  map->put(_seq_type, new_seq_type);
  map->put(_seq_type->types.v[0]->type, new_seq_type->elementType);
  map->put(_seq_type->types.v[1]->type, new_seq_type->types.v[0]->type);
  map->put(dynamic_cast<StructuralType*>(_seq_type->types.v[1]->type)->fields.v[0],
           dynamic_cast<StructuralType*>(new_seq_type->types.v[0]->type)->fields.v[0]);
  map->put(dynamic_cast<StructuralType*>(_seq_type->types.v[1]->type)->fields.v[1],
           dynamic_cast<StructuralType*>(new_seq_type->types.v[0]->type)->fields.v[1]);
  map->put(_seq_type->fields.v[0], new_seq_type->fields.v[0]);
  map->put(_seq_type->fields.v[1], new_seq_type->fields.v[1]);
  map->put(_seq_type->fields.v[2], new_seq_type->fields.v[2]);
  new_decls->traverse(new UpdateSymbols(map), true);

  Symbol* _node = new_seq_type->types.v[0];
  _node->cname = glomstrings(2, new_seq_name, _node->cname);

  return new_seq_type;
}


bool SeqType::hasDefaultWriteFunction(void) {
  return true;
}


AList<Stmt>* SeqType::buildDefaultWriteFunctionBody(ParamSymbol* arg) {
  return new AList<Stmt>(new ExprStmt(new ParenOpExpr(new Variable(Symboltable::lookupInternal("_SeqWriteStopgap")), new AList<Expr>(new Variable(arg)))));
}


ArrayType::ArrayType(Expr* init_domain, Type* init_elementType):
  Type(TYPE_ARRAY, init_elementType->defaultVal),
  domain(init_domain),
  elementType(init_elementType)
{ }


Type* ArrayType::copyType(bool clone, Map<BaseAST*,BaseAST*>* map) {
  Type* copy = new ArrayType(domain->copyInternal(clone, map),
                             elementType);
  copy->addSymbol(symbol);
  return copy;
}


void ArrayType::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == defaultVal) {
    defaultVal = dynamic_cast<Expr*>(new_ast);
  } else if (old_ast == domain) {
    domain = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in Type::replaceChild(old, new)");
  }
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


void ArrayType::codegenDef(FILE* outfile) {
  fprintf(outfile, "struct _");
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
  fprintf(outfile, "};\n");

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


void ArrayType::codegenPrototype(FILE* outfile) {
  fprintf(outfile, "typedef struct _");
  symbol->codegen(outfile);
  fprintf(outfile, " ");
  symbol->codegen(outfile);
  fprintf(outfile, ";\n");
}


void ArrayType::codegenDefaultFormat(FILE* outfile, bool isRead) {
  elementType->codegenDefaultFormat(outfile, isRead);
}


bool ArrayType::blankIntentImpliesRef(void) {
  return true;
}


bool ArrayType::hasDefaultWriteFunction(void) {
  return true;
}


AList<Stmt>* ArrayType::buildDefaultWriteFunctionBody(ParamSymbol* arg) {
  AList<Expr>* zero_inds = new AList<Expr>(new IntLiteral("0", 0));
  AList<Stmt>* body = new AList<Stmt>(new ExprStmt(new ParenOpExpr(new Variable(Symboltable::lookupInternal("_ArrayWriteStopgap")), new AList<Expr>(new Variable(arg)))));
  // The addition of this statement is somewhat of a kludge.  See the notes
  // related to it in expr.cpp
  body->insertAtTail(new ExprStmt(new ParenOpExpr(new Variable(new UnresolvedSymbol("write")), new AList<Expr>(new ArrayRef(new Variable(arg), zero_inds)))));
  return body;
}


UserType::UserType(Type* init_definition, Expr* init_defaultVal) :
  Type(TYPE_USER, init_defaultVal),
  definition(init_definition)
{}


Type* UserType::copyType(bool clone, Map<BaseAST*,BaseAST*>* map) {
  Type* copy = new UserType(definition,
                            defaultVal->copyInternal(clone, map));
  copy->addSymbol(symbol);
  return copy;
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


void UserType::codegenDef(FILE* outfile) {
  fprintf(outfile, "typedef ");
  definition->codegen(outfile);
  fprintf(outfile, " ");
  symbol->codegen(outfile);
  fprintf(outfile, ";\n");
}


void UserType::codegenDefaultFormat(FILE* outfile, bool isRead) {
  definition->codegenDefaultFormat(outfile, isRead);
}


LikeType::LikeType(Expr* init_expr) :
  Type(TYPE_LIKE, NULL),
  expr(init_expr)
{ }


Type* LikeType::copyType(bool clone, Map<BaseAST*,BaseAST*>* map) {
  Type* copy = new LikeType(expr->copyInternal(clone, map));
  copy->addSymbol(symbol);
  return copy;
}


void LikeType::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == defaultVal) {
    defaultVal = dynamic_cast<Expr*>(new_ast);
  } else if (old_ast == expr) {
    expr = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in Type::replaceChild(old, new)");
  }
}


void LikeType::traverseDefType(Traversal* traversal) {
  TRAVERSE(expr, traversal, false);
}


void LikeType::printDef(FILE* outfile) {
  fprintf(outfile, "like type ");
  symbol->print(outfile);
  fprintf(outfile, " = ");
  expr->print(outfile);
}


void LikeType::codegen(FILE* outfile) {
  INT_FATAL(this, "Unanticipated call to LikeType::codegen");
}


void LikeType::codegenDef(FILE* outfile) {
  INT_FATAL(this, "Unanticipated call to LikeType::codegenDef");
}


StructuralType::StructuralType(astType_t astType, Expr* init_defaultVal) :
  Type(astType, init_defaultVal),
  structScope(NULL),
  declarationList(new AList<Stmt>()),
  parentStruct(NULL)
{
  fields.clear();
  methods.clear();
  types.clear();
}


void StructuralType::copyGuts(StructuralType* copy_type, bool clone, 
                              Map<BaseAST*,BaseAST*>* map) {
  Symboltable::pushScope(SCOPE_CLASS);
  AList<Stmt>* new_decls = new AList<Stmt>();
  for (Stmt* old_decls = declarationList->first();
       old_decls;
       old_decls = declarationList->next()) {
    DefStmt* def = dynamic_cast<DefStmt*>(old_decls);
    if (def && def->definesFunctions()) {
      for (DefExpr* defExpr = def->defExprls->first();
           defExpr;
           defExpr = def->defExprls->next()) {
        if (FnSymbol* fn = dynamic_cast<FnSymbol*>(defExpr->sym)) {
          copy_type->methods.add(fn);
        }
      }
    } else {
      new_decls->insertAtTail(old_decls->copyInternal(true, map));
    }
  }
  copy_type->addDeclarations(new_decls);
  SymScope* copy_scope = Symboltable::popScope();
  copy_type->setScope(copy_scope);
}


void StructuralType::addDeclarations(AList<Stmt>* newDeclarations, 
                                     Stmt* beforeStmt) {
  Stmt* tmp = newDeclarations->first();
  while (tmp) {
    DefStmt* def_stmt = dynamic_cast<DefStmt*>(tmp);
    if (def_stmt) {
      if (def_stmt->definesFunctions()) {
        for (DefExpr* defExpr = def_stmt->defExprls->first();
             defExpr;
             defExpr = def_stmt->defExprls->next()) {
          FnSymbol* fn = dynamic_cast<FnSymbol*>(defExpr->sym);
          fn->typeBinding = this->symbol;
          if (!fn->isConstructor) {
            fn->method_type = PRIMARY_METHOD;
          }
          methods.add(fn);
        }
      } else if (def_stmt->definesTypes()) {
        for (DefExpr* defExpr = def_stmt->defExprls->first();
             defExpr;
             defExpr = def_stmt->defExprls->next()) {
          types.add(dynamic_cast<TypeSymbol*>(defExpr->sym));
        }
      } else if (def_stmt->definesVariables()) {
        for (DefExpr* defExpr = def_stmt->defExprls->first();
             defExpr;
             defExpr = def_stmt->defExprls->next()) {
          VarSymbol* var = dynamic_cast<VarSymbol*>(defExpr->sym);
          fields.add(var);
        }
      }
    }
    tmp = newDeclarations->next();
  }
  if (beforeStmt) {
    while (Stmt* tmp = newDeclarations->popHead()) {
      beforeStmt->insertBefore(tmp);
    }
  } else {
    declarationList->add(newDeclarations);
  }
}


void StructuralType::setScope(SymScope* init_structScope) {
  structScope = init_structScope;
}


void StructuralType::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == defaultVal) {
    defaultVal = dynamic_cast<Expr*>(new_ast);
  } else if (old_ast == declarationList) {
    declarationList = dynamic_cast<AList<Stmt>*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in Type::replaceChild(old, new)");
  }
}


void StructuralType::traverseDefType(Traversal* traversal) {
  SymScope* prevScope = NULL;
  if (structScope) {
    prevScope = Symboltable::setCurrentScope(structScope);
  }
  declarationList->traverse(traversal, false);
  TRAVERSE(defaultVal, traversal, false);
  if (structScope) {
    Symboltable::setCurrentScope(prevScope);
  }
}


int
is_Scalar_Type(Type *t) {
  return t && t != dtUnknown && t != dtString && 
    (t->astType == TYPE_BUILTIN || t->astType == TYPE_ENUM);
}


int
is_Value_Type(Type *t) {
  return t->astType == TYPE_RECORD || t->astType == TYPE_UNION;
}


int
is_Reference_Type(Type *t) {
  return t && (t->astType == TYPE_CLASS || t->astType == TYPE_SUM);
}


void StructuralType::buildConstructorBody(AList<Stmt>* stmts, Symbol* _this, 
                                          AList<ParamSymbol>* arguments) {
  forv_Vec(VarSymbol, tmp, fields) {
    if (is_Scalar_Type(tmp->type))
      continue;
    Expr* lhs = new MemberAccess(new Variable(_this), tmp);
    Expr* varInitExpr = new VarInitExpr(new MemberAccess(new Variable(_this), tmp));
    Expr* assign_expr = new AssignOp(GETS_NORM, lhs, varInitExpr);
    Stmt* assign_stmt = new ExprStmt(assign_expr);
    stmts->insertAtTail(assign_stmt);
  }

  ParamSymbol* ptmp = arguments->first();
  forv_Vec(TypeSymbol, tmp, types) {
    if (dynamic_cast<VariableType*>(tmp->type)) {
      if (analyzeAST) {
        // Have type variable in class and type variable in parameter
        // Should I do anything with these?
        ptmp = arguments->next();
      }
    }
  }
  forv_Vec(VarSymbol, tmp, fields) {
    Expr* lhs = new MemberAccess(new Variable(_this), tmp);
    Expr* rhs = NULL;
    if (analyzeAST) {
      rhs = new Variable(ptmp);
    } else {
      Expr* varInitExpr = new VarInitExpr(new MemberAccess(new Variable(_this), tmp));
      rhs = tmp->defPoint->init ? tmp->defPoint->init->expr->copy() : varInitExpr;
      if (tmp->defPoint->init) {
        tmp->defPoint->init->remove();
      }
    }
    if (rhs) {
      Expr* assign_expr = new AssignOp(GETS_NORM, lhs, rhs);
      Stmt* assign_stmt = new ExprStmt(assign_expr);
      stmts->insertAtTail(assign_stmt);
    }
    if (analyzeAST) {
      ptmp = arguments->next();
    }
  }
}


void StructuralType::codegenStartDefFields(FILE* outfile) {}

void StructuralType::codegenStopDefFields(FILE* outfile) {}


void StructuralType::codegenDef(FILE* outfile) {
  forv_Vec(TypeSymbol, type, types) {
    if (type) {
      type->codegenDef(outfile);
      fprintf(outfile, "\n");
    }
  }
  fprintf(outfile, "struct __");
  symbol->codegen(outfile);
  fprintf(outfile, " {\n");
  codegenStartDefFields(outfile);
  bool printedSomething = false; // BLC: this is to avoid empty structs, illegal in C
  for (Stmt* tmp = declarationList->first(); tmp; tmp = declarationList->next()) {
    if (DefStmt* def_stmt = dynamic_cast<DefStmt*>(tmp)) {
      if (def_stmt->definesVariables()) {
        for (DefExpr* defExpr = def_stmt->defExprls->first();
             defExpr;
             defExpr = def_stmt->defExprls->next()) {
          VarSymbol* var = dynamic_cast<VarSymbol*>(defExpr->sym);
          var->codegenDef(outfile);
          printedSomething = true;
        }
      }
    }
  }
  if (!printedSomething) {
    fprintf(outfile, "int _emptyStructPlaceholder;\n");
  }
  codegenStopDefFields(outfile);
  fprintf(outfile, "};\n\n");
}


void StructuralType::codegenStructName(FILE* outfile) {
  symbol->codegen(outfile);
  
}


void StructuralType::codegenPrototype(FILE* outfile) {
  forv_Vec(TypeSymbol, type, types) {
    if (type) {
      type->codegenPrototype(outfile);
      fprintf(outfile, "\n");
    }
  }
  fprintf(outfile, "typedef struct __");
  symbol->codegen(outfile);
  fprintf(outfile, " ");
  codegenStructName(outfile);
  fprintf(outfile, ";\n");
}


void StructuralType::codegenMemberAccessOp(FILE* outfile) {
  fprintf(outfile, ".");
}


bool StructuralType::blankIntentImpliesRef(void) {
  return false;
}


bool StructuralType::implementedUsingCVals(void) {
  return true;
}


bool StructuralType::hasDefaultWriteFunction(void) {
  return true;
}


AList<Stmt>* StructuralType::buildDefaultWriteFunctionBody(ParamSymbol* arg) {
  AList<Stmt>* body = new AList<Stmt>(new NoOpStmt());
  if (dynamic_cast<ClassType*>(this)) {
    Expr* write = new Variable(new UnresolvedSymbol("write"));
    AList<Stmt>* writeNil = new AList<Stmt>(new ExprStmt(new ParenOpExpr(write, new AList<Expr>(new StringLiteral("nil")))));
    writeNil->insertAtTail(new ReturnStmt(NULL));
    BlockStmt* blockStmt = new BlockStmt(writeNil);
    Symbol* nil = Symboltable::lookupInternal("nil", SCOPE_INTRINSIC);
    Expr* argIsNil = new BinOp(BINOP_EQUAL, new Variable(arg), new Variable(nil));
    body->insertAtTail(new CondStmt(argIsNil, blockStmt));
  }

  if (dynamic_cast<ClassType*>(this)) {
    addWriteStmt(body, new StringLiteral("{"));
  } else {
    addWriteStmt(body, new StringLiteral("("));
  }

  bool first = true;
  forv_Vec(VarSymbol, tmp, fields) {
    if (!first) {
      addWriteStmt(body, new StringLiteral(", "));
    }
    addWriteStmt(body, new StringLiteral(tmp->name));
    addWriteStmt(body, new StringLiteral(" = "));
    addWriteStmt(body, new MemberAccess(new Variable(arg), tmp));
    first = false;
  }

  if (dynamic_cast<ClassType*>(this)) {
    addWriteStmt(body, new StringLiteral("}"));
  } else {
    addWriteStmt(body, new StringLiteral(")"));
  }

  return body;
}


ClassType::ClassType(astType_t astType) :
  StructuralType(astType, 
                 new Variable(Symboltable::lookupInternal("nil", 
                                                          SCOPE_INTRINSIC)))
{
  parentClasses.clear();
}


Type* ClassType::copyType(bool clone, Map<BaseAST*,BaseAST*>* map) {
  ClassType* copy_type = new ClassType(astType);
  copyGuts(copy_type, clone, map);
  return copy_type;
}


void ClassType::codegenStructName(FILE* outfile) {
  fprintf(outfile, "_");
  symbol->codegen(outfile);
  fprintf(outfile,", *");
  symbol->codegen(outfile);
}


void ClassType::codegenMemberAccessOp(FILE* outfile) {
  fprintf(outfile, "->");
}


bool ClassType::blankIntentImpliesRef(void) {
  return false;  // BLC: it does, but not for the pointer that we're passing in
}


bool ClassType::implementedUsingCVals(void) {
  return false;
}


RecordType::RecordType(void) :
  StructuralType(TYPE_RECORD)
{}


Type* RecordType::copyType(bool clone, Map<BaseAST*,BaseAST*>* map) {
  RecordType* copy_type = new RecordType();
  copyGuts(copy_type, clone, map);
  return copy_type;
}


UnionType::UnionType(void) :
  StructuralType(TYPE_UNION),
  fieldSelector(NULL)
{}


Type* UnionType::copyType(bool clone, Map<BaseAST*,BaseAST*>* map) {
  UnionType* copy_type = new UnionType();
  copyGuts(copy_type, clone, map);
  return copy_type;
}


static char* buildFieldSelectorName(UnionType* unionType, Symbol* field, 
                                    bool enumName = false) {
  char* fieldName;
  if (field) {
    fieldName = field->name;
  } else {
    if (enumName) {
      fieldName = "";
    } else {
      fieldName = "_uninitialized";
    }
  }
  return glomstrings(4, "_", unionType->symbol->name, "_union_id_", fieldName);
}


void UnionType::buildFieldSelector(void) {
  AList<EnumSymbol>* id_list = new AList<EnumSymbol>();

  /* build list of enum symbols */
  char* id_name = buildFieldSelectorName(this, NULL);
  EnumSymbol* id_symbol = new EnumSymbol(id_name, NULL);
  id_list->insertAtTail(id_symbol);
  forv_Vec(VarSymbol, tmp, fields) {
    id_name = buildFieldSelectorName(this, tmp);
    id_symbol = new EnumSymbol(id_name, NULL);
    id_list->insertAtTail(id_symbol);
  }
  EnumSymbol::setValues(id_list);

  /* build enum type */
  EnumType* enum_type = new EnumType(id_list);
  char* enum_name = buildFieldSelectorName(this, NULL, true);
  TypeSymbol* enum_symbol = new TypeSymbol(enum_name, enum_type);
  enum_type->addSymbol(enum_symbol);

  /* build definition of enum */
  DefExpr* def_expr = new DefExpr(enum_symbol);
  enum_symbol->setDefPoint(def_expr);
  EnumSymbol::setDefPoints(id_list, def_expr);
  DefStmt* def_stmt = new DefStmt(def_expr);
  symbol->defPoint->parentStmt->insertBefore(def_stmt);

  fieldSelector = enum_type;
}


static char* unionCallName[NUM_UNION_CALLS] = {
  "_UNION_SET",
  "_UNION_CHECK",
  "_UNION_CHECK_QUIET"
};


FnCall* UnionType::buildSafeUnionAccessCall(unionCall type, Expr* base, 
                                            Symbol* field) {
  AList<Expr>* args = new AList<Expr>(base->copy());
  char* id_tag = buildFieldSelectorName(this, field);
  args->insertAtTail(new Variable(Symboltable::lookupFromScope(id_tag, structScope)));
  if (type == UNION_CHECK) {
    args->insertAtTail(new StringLiteral(base->filename));
    args->insertAtTail(new IntLiteral(intstring(base->lineno), base->lineno));
  }
  
  char* fnName = unionCallName[type];
  return new FnCall(new Variable(Symboltable::lookupInternal(fnName)), args);
}


void UnionType::buildConstructorBody(AList<Stmt>* stmts, Symbol* _this, 
                                     AList<ParamSymbol>* arguments) {
  AList<Expr>* args = new AList<Expr>(new Variable(_this));
  Expr* arg2 = new Variable(fieldSelector->valList->first());
  args->insertAtTail(arg2);
  FnCall* init_function = 
    new FnCall(new Variable(Symboltable::lookupInternal("_UNION_SET")), args);
  ExprStmt* init_stmt = new ExprStmt(init_function);
  stmts->insertAtTail(init_stmt);
}


void UnionType::codegenStartDefFields(FILE* outfile) {
  fieldSelector->codegen(outfile);
  fprintf(outfile, " _chpl_union_tag;\n");
  fprintf(outfile, "union {\n");
}


void UnionType::codegenStopDefFields(FILE* outfile) {
  fprintf(outfile, "} _chpl_union;\n");
}


void UnionType::codegenMemberAccessOp(FILE* outfile) {
  fprintf(outfile, "._chpl_union.");
}


bool UnionType::hasDefaultWriteFunction(void) {
  return true;
}


AList<Stmt>* UnionType::buildDefaultWriteFunctionBody(ParamSymbol* arg) {
  return new AList<Stmt>(new ExprStmt(new ParenOpExpr(new Variable(Symboltable::lookupInternal("_UnionWriteStopgap")), new AList<Expr>(new Variable(arg)))));
}


TupleType::TupleType() :
  StructuralType(TYPE_TUPLE)
{
  Symboltable::pushScope(SCOPE_CLASS);
  setScope(Symboltable::popScope());
}


void TupleType::addType(Type* additionalType) {
  SymScope* saveScope = Symboltable::setCurrentScope(structScope);
  components.add(additionalType);
  char* field_name = glomstrings(2, "_field", intstring(components.n));
  addDeclarations(new AList<Stmt>(new DefStmt(new DefExpr(new VarSymbol(field_name, additionalType)))));
  Symboltable::setCurrentScope(saveScope);
}


void TupleType::rebuildDefaultVal(void) {
  AList<Expr>* exprs = new AList<Expr>();
  forv_Vec(Type, component, components) {
    exprs->insertAtTail(component->defaultVal->copy());
  }
  Tuple* tuple = new Tuple(exprs);
  if (symbol) {
    defaultVal->replace(tuple);
  }
}


Type* TupleType::copyType(bool clone, Map<BaseAST*,BaseAST*>* map) {
  TupleType* newTupleType = new TupleType();
  forv_Vec(Type, component, components) {
    newTupleType->components.add(component);
  }
  copyGuts(newTupleType, clone, map);
  return newTupleType;
}


void TupleType::traverseDefType(Traversal* traversal) {
  for (int i=0; i<components.n; i++) {
    TRAVERSE(components.v[i], traversal, false);
  }
  StructuralType::traverseDefType(traversal);
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


MetaType::MetaType(Type* init_base) :
  Type(TYPE_META, NULL),
  base(init_base)
{
}

void MetaType::traverseDefType(Traversal* traversal) {
  TRAVERSE(base, traversal, false);
}

SumType::SumType(Type* firstType) :
  Type(TYPE_SUM, new Variable(Symboltable::lookupInternal("nil",
                                                          SCOPE_INTRINSIC)))
{
  components.add(firstType);
}

void SumType::addType(Type* additionalType) {
  components.add(additionalType);
}

void SumType::codegenDef(FILE* outfile) {
  fprintf(outfile, "typedef void *");
  symbol->codegen(outfile);
  fprintf(outfile, ";");
}

void SumType::codegenStructName(FILE* outfile) {
  symbol->codegen(outfile);
}

VariableType::VariableType(Type *init_type) :
  Type(TYPE_VARIABLE, NULL), 
  type(init_type)
{}


Type* VariableType::copyType(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new VariableType(type);
}


void VariableType::codegen(FILE* outfile) {
  INT_FATAL(this, "Unanticipated call to VariableType::codegen");
}

void VariableType::traverseDefType(Traversal* traversal) {
  TRAVERSE(type, traversal, false);
}


UnresolvedType::UnresolvedType(Vec<char*>* init_names) :
  Type(TYPE_UNRESOLVED, NULL),
  names(init_names) {
}


Type* UnresolvedType::copyType(bool clone, Map<BaseAST*,BaseAST*>* map) {
  Vec<char*>* new_names = new Vec<char*>();
  forv_Vec(char, str, *names) {
    new_names->add(str);
  }
  return new UnresolvedType(new_names);
}


void UnresolvedType::print(FILE* outfile) {
  fprintf(outfile, "%s", names->v[0]);
  for (int i = 1; i < names->n; i++) {
    fprintf(outfile, ".%s", names->v[i]);
  }
}


void UnresolvedType::codegen(FILE* outfile) {
  INT_FATAL(this, "Unanticipated call to UnresolvedType::codegen");
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
  // We should eventually point this to the new complex
  dtComplex =
    Symboltable::defineBuiltinType("complex", "_complex128",
                                   new ComplexLiteral("0.0i", 0.0, 0.0, "0.0"));
  dtString = Symboltable::defineBuiltinType("string", "_string", NULL);
  dtNumeric = Symboltable::defineBuiltinType("numeric", "_numeric", NULL);
  dtAny = Symboltable::defineBuiltinType("any", "_any", NULL);
  dtObject = Symboltable::defineBuiltinType("object", "_object", NULL);
  dtLocale = Symboltable::defineBuiltinType("locale", "_locale", NULL);

  // define dtNil.  This doesn't reuse the above because it's a
  // different subclass of Type.  Could come up with a more clever
  // way to generalize
  dtNil = new NilType();
  TypeSymbol* sym = new TypeSymbol("_nilType", dtNil);
  sym->cname = copystring("_nilType");
  dtNil->addSymbol(sym);
  builtinTypes.add(dtNil);
}


void findInternalTypes(void) {
  dtSequence = Symboltable::lookupInternalType("_seq")->type;
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
  static unsigned int hash(SumType *a) {
    unsigned int h = 0;
    for (int i = 0; i < a->components.n; i++)
      h += open_hash_multipliers[i % 256] * (uintptr_t)a->components.v[i];
    return h;
  }
  static int equal(SumType *a, SumType *b) { 
    if (a->components.n != b->components.n)
      return 0;
    for (int i = 0; i < a->components.n; i++)
      if (a->components.v[i] != b->components.v[i])
        return 0;
    return 1;
  }
};

static class BlockHash<SumType *, LUBCacheHashFns> lub_cache;

Type *find_or_make_sum_type(Vec<Type *> *types) {
  static int uid = 1;
  if (types->n < 2) {
    INT_FATAL("Trying to create sum_type of less than 2 types");
  }
  qsort(types->v, types->n, sizeof(types->v[0]), compar_baseast);
  SumType* new_sum_type = new SumType(types->v[0]);
  for (int i = 1; i < types->n; i++)
    new_sum_type->addType(types->v[i]);
  SumType *old_type = lub_cache.get(new_sum_type);
  if (old_type)
    return old_type;
  lub_cache.put(new_sum_type);
  char* name = glomstrings(2, "_sum_type", intstring(uid++));
  SymScope* saveScope = Symboltable::setCurrentScope(commonModule->modScope);
  Symbol* sym = new TypeSymbol(name, new_sum_type);
  new_sum_type->addSymbol(sym);
  Symboltable::setCurrentScope(saveScope);
  return new_sum_type;
}


NilType::NilType(void) :
  Type(TYPE_NIL, NULL)
{}


void NilType::codegen(FILE* outfile) {
  fprintf(outfile, "void* ");
}

Type *getMetaType(Type *t) {
  if (t)
    return t->getMetaType();
  else
    return dtAny->getMetaType();
}
