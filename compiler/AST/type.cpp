#include <typeinfo>
#define TYPE_EXTERN
#include "astutil.h"
#include "build.h"
#include "expr.h"
#include "files.h"
#include "misc.h"
#include "runtime.h"
#include "stringutil.h"
#include "symbol.h"
#include "symscope.h"
#include "type.h"


Type::Type(astType_t astType, Symbol* init_defaultVal) :
  BaseAST(astType),
  scalarPromotionType(NULL),
  symbol(NULL),
  defaultValue(init_defaultVal),
  defaultConstructor(NULL),
  isGeneric(false),
  instantiatedFrom(NULL),
  instantiatedWith(NULL)
{}


Type::~Type() {
  if (instantiatedWith)
    delete instantiatedWith;
}


void Type::verify() {
  BaseAST::verify();
}


void Type::addSymbol(TypeSymbol* newsymbol) {
  symbol = newsymbol;
}


bool Type::inTree(void) {
  if (symbol)
    return symbol->inTree();
  else
    return false;
}


Type*
Type::copyInner(ASTMap* map) {
  INT_FATAL(this, "Illegal call to Type::copy");
  return NULL;
}


void Type::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == defaultValue) {
    defaultValue = dynamic_cast<Symbol*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in Type::replaceChild");
  }
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
  INT_FATAL(this, "Unexpected call to Type::codegenDef");
}


void Type::codegenPrototype(FILE* outfile) { }


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


bool Type::requiresCParamTmp(intentTag intent) {
  if (intent == INTENT_BLANK) {
    intent = INTENT_CONST;
  }
  switch (intent) {
  case INTENT_BLANK:
    INT_FATAL(this, "should never have reached INTENT_BLANK case");
  case INTENT_CONST:
    // if something's passed by const it won't be modified and
    // won't need a temp?
    return false;
  case INTENT_IN:
    // if these are implemented using C's pass-by-value, then C
    // effectively puts in the temp for us
    if (implementedUsingCVals()) {
      return false;
    } else {
      return true;
    }
  case INTENT_INOUT:
    // here a temp is probably always needed in order to avoid
    // affecting the original value
  case INTENT_OUT:
    // and here it's needed to set up the default value of the type
    return true;
  case INTENT_REF:
    // here, a temp should never be needed
    return false;
  case INTENT_PARAM:
  case INTENT_TYPE:
    return false;  // Should never be encountered EVENTUALLY
  default:
    INT_FATAL(this, "case not handled in requiresCParamTmp");
    return false;
  }
}


bool Type::implementedUsingCVals(void) {
  if (this == dtBool ||
      this == dtInt[INT_SIZE_8]  ||
      this == dtInt[INT_SIZE_16] ||
      this == dtInt[INT_SIZE_32] ||
      this == dtInt[INT_SIZE_64] ||
      this == dtUInt[INT_SIZE_1]  ||
      this == dtUInt[INT_SIZE_8]  ||
      this == dtUInt[INT_SIZE_16] ||
      this == dtUInt[INT_SIZE_32] ||
      this == dtUInt[INT_SIZE_64] ||
      this == dtReal[FLOAT_SIZE_32] ||
      this == dtReal[FLOAT_SIZE_64] ||
      this == dtReal[FLOAT_SIZE_128]) {
    return true;
  } else {
    return false;
 }
}

bool Type::hasDefaultWriteFunction(void) {
  return false;
}


BlockStmt* Type::buildDefaultWriteFunctionBody(ArgSymbol* fileArg, ArgSymbol* arg) {
  INT_FATAL(this, "unexpected call to Type::buildDefaultWriteFunctionBody");
  return NULL;
}


bool Type::hasDefaultReadFunction(void) {
  return false;
}


BlockStmt* Type::buildDefaultReadFunctionBody(ArgSymbol* fileArg, ArgSymbol* arg) {
  INT_FATAL(this, "unexpected call to Type::buildDefaultWriteFunctionBody");
  return NULL;
}


Symbol* Type::getField(char* name) {
  INT_FATAL(this, "getField not called on ClassType");
  return NULL;
}


PrimitiveType::PrimitiveType(Symbol *init) :
  Type(TYPE_PRIMITIVE, init)
{}


void PrimitiveType::verify() {
  Type::verify();
  if (astType != TYPE_PRIMITIVE) {
    INT_FATAL(this, "Bad PrimitiveType::astType");
  }
}


EnumType::EnumType(AList* init_constants) :
  Type(TYPE_ENUM, dynamic_cast<DefExpr*>(init_constants->first())->sym),
  constants(init_constants)
{
  for_alist(DefExpr, def, constants)
    def->sym->type = this;
  constants->parent = this;
}


EnumType::~EnumType() {
  delete constants;
}


void EnumType::verify() {
  Type::verify();
  if (astType != TYPE_ENUM) {
    INT_FATAL(this, "Bad EnumType::astType");
  }
  if (constants->parent != this)
    INT_FATAL(this, "Bad AList::parent in EnumType");
}


EnumType*
EnumType::copyInner(ASTMap* map) {
  EnumType* copy = new EnumType(COPY_INT(constants));
  copy->addSymbol(symbol);
  return copy;
}


void EnumType::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == defaultValue) {
    defaultValue = dynamic_cast<Symbol*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in Type::replaceChild");
  }
}


void EnumType::printDef(FILE* outfile) {
  printf("enum ");
  symbol->print(outfile);
  printf(" = ");
  bool first = true;
  for_alist(DefExpr, constant, constants) {
    if (!first) {
      fprintf(outfile, " | ");
    } else {
      first = false;
    }
    constant->sym->print(outfile);
  }
}


void EnumType::codegenDef(FILE* outfile) {
  fprintf(outfile, "typedef enum {\n");
  bool first = true;
  for_alist(DefExpr, constant, constants) {
    if (!first) {
      fprintf(outfile, ", ");
    } else {
      first = false;
    }
    constant->sym->codegen(outfile);
    if (constant->init) {
      fprintf(outfile, " = ");
      constant->init->codegen(outfile);
    }
  }
  fprintf(outfile, "} ");
  symbol->codegen(outfile);
  fprintf(outfile, ";\n\n");
}



void EnumType::codegenStringToType(FILE* outfile) {
  fprintf(outfile, "int _convert_string_to_enum");
  symbol->codegen(outfile);
  fprintf(outfile, "(char* inputString, ");
  symbol->codegen(outfile);
  fprintf(outfile, "* val) {\n");
  for_alist(DefExpr, constant, constants) {
    fprintf(outfile, "if (strcmp(inputString, \"");
    constant->sym->codegen(outfile);
    fprintf(outfile, "\") == 0) {\n");
    fprintf(outfile, "*val = ");
    constant->sym->codegen(outfile);
    fprintf(outfile, ";\n");
    fprintf(outfile, "} else ");
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


BlockStmt* EnumType::buildDefaultWriteFunctionBody(ArgSymbol* fileArg, ArgSymbol* arg) {
  CondStmt* body = NULL;
  for_alist(DefExpr, constant, constants) {
    body = new CondStmt(new CallExpr("==", arg, constant->sym),
                        new CallExpr("fwrite", fileArg, new_StringSymbol(constant->sym->name)),
                        body);
  }
  return new BlockStmt(body);
}


bool EnumType::hasDefaultReadFunction(void) {
  return true;
}


BlockStmt* EnumType::buildDefaultReadFunctionBody(ArgSymbol* fileArg, ArgSymbol* arg) {
  BlockStmt* body = new BlockStmt();
  Symbol* valString = new VarSymbol("valString");
  body->insertAtTail(new DefExpr(valString, new_StringSymbol("")));
  body->insertAtTail(new CallExpr("fread", fileArg, valString));
  Expr* elseStmt = new CallExpr("halt", 
                                new_StringSymbol("***Error: Not of "), 
                                new_StringSymbol(arg->type->symbol->name), 
                                new_StringSymbol(" type***"));
  for_alist_backward(DefExpr, constant, this->constants) {
    Expr* cond = new CallExpr("==", valString, new_StringSymbol(constant->sym->name));
    Expr* thenStmt = new CallExpr("=", arg, constant->sym);
    elseStmt = new CondStmt(cond, thenStmt, elseStmt);
    
  }
  body->insertAtTail(elseStmt);
  return body;
}


UserType::UserType(Expr* init_typeExpr) :
  Type(TYPE_USER, NULL),
  typeExpr(init_typeExpr)
{}


void UserType::verify() {
  Type::verify();
  if (astType != TYPE_USER) {
    INT_FATAL(this, "Bad UserType::astType");
  }
}


UserType*
UserType::copyInner(ASTMap* map) {
  UserType* copy = new UserType(COPY_INT(typeExpr));
  copy->addSymbol(symbol);
  return copy;
}


void UserType::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == defaultValue) {
    defaultValue = dynamic_cast<Symbol*>(new_ast);
  } else if (old_ast == typeExpr) {
    typeExpr = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in Type::replaceChild");
  }
}


void UserType::printDef(FILE* outfile) {
  fprintf(outfile, "type ");
  symbol->print(outfile);
}


ClassType::ClassType(ClassTag initClassTag) :
  Type(TYPE_CLASS, NULL),
  classTag(initClassTag),
  isIterator(false),
  structScope(NULL),
  fields(new AList()),
  inherits(new AList())
{
  if (classTag == CLASS_CLASS) { // set defaultValue to nil to keep it
                                 // from being constructed
    defaultValue = gNil;
  }
  methods.clear();
  fields->parent = this;
  inherits->parent = this;
}


ClassType::~ClassType() {
  if (structScope)
    delete structScope;
  delete fields;
  delete inherits;
}


void ClassType::verify() {
  Type::verify();
  if (astType != TYPE_CLASS) {
    INT_FATAL(this, "Bad ClassType::astType");
  }
  if (classTag != CLASS_CLASS &&
      classTag != CLASS_RECORD &&
      classTag != CLASS_UNION)
    INT_FATAL(this, "Bad ClassType::classTag");
  if (fields->parent != this || inherits->parent != this)
    INT_FATAL(this, "Bad AList::parent in ClassType");
}


ClassType*
ClassType::copyInner(ASTMap* map) {
  ClassType* copy_type = new ClassType(classTag);
  copy_type->isIterator = isIterator;
  copy_type->fields->insertAtTail(COPY_INT(fields));
  copy_type->inherits->insertAtTail(COPY_INT(inherits));
  for_fields(field, copy_type) {
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(field))
      copy_type->methods.add(fn);
  }
  return copy_type;
}


void ClassType::addDeclarations(AList* stmts, bool tail) {
  Vec<BaseAST*> asts;
  collect_top_asts(&asts, stmts);

  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = dynamic_cast<DefExpr*>(ast)) {
      if (FnSymbol* fn = dynamic_cast<FnSymbol*>(def->sym)) {
        methods.add(fn);
        fn->_this = new ArgSymbol(INTENT_BLANK, "this", this);
        fn->insertFormalAtHead(new DefExpr(fn->_this));
        if (strcmp("this", fn->name))
          fn->insertFormalAtHead(new DefExpr(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken)));
        fn->isMethod = true;
      }
      if (def->parentSymbol || def->list)
        def->remove();
      if (tail)
        fields->insertAtTail(def);
      else
        fields->insertAtHead(def);
    }
  }
}


void ClassType::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == defaultValue) {
    defaultValue = dynamic_cast<Symbol*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in Type::replaceChild");
  }
}


bool
ClassType::isNominalType() {
  return classTag == CLASS_CLASS;
}


void ClassType::codegenDef(FILE* outfile) {
  fprintf(outfile, "struct __");
  symbol->codegen(outfile);
  fprintf(outfile, " {\n");
  bool printedSomething = false; // BLC: this is to avoid empty structs, illegal in C
  if (classTag == CLASS_CLASS) {
    fprintf(outfile, "_int64 _cid;\n");
    printedSomething = true;
  } else if (classTag == CLASS_UNION) {
    fprintf(outfile, "_int64 _uid;\n");
    fprintf(outfile, "union {\n");
  }
  for_fields(field, this) {
    field->codegenDef(outfile);
    printedSomething = true;
  }
  if (classTag == CLASS_UNION) {
    if (!printedSomething)
      fprintf(outfile, "int _emptyUnionPlaceholder;\n");
    fprintf(outfile, "};\n");
    printedSomething = true;
  }
  if (symbol->hasPragma("data class")) {
    dynamic_cast<DefExpr*>(fields->get(1))->sym->type->codegen(outfile);
    fprintf(outfile, "* _data;\n");
  }
  if (!printedSomething) {
    fprintf(outfile, "int _emptyStructPlaceholder;\n");
  }
  fprintf(outfile, "};\n\n");
}


void ClassType::codegenPrototype(FILE* outfile) {
  if (classTag == CLASS_CLASS)
    fprintf(outfile, "typedef struct __%s _%s, *%s;\n",
            symbol->cname, symbol->cname, symbol->cname);
  else
    fprintf(outfile, "typedef struct __%s %s;\n",
            symbol->cname, symbol->cname);
}


bool ClassType::implementedUsingCVals(void) {
  return false;
}


bool ClassType::hasDefaultWriteFunction(void) {
  return true;
}


BlockStmt* ClassType::buildDefaultWriteFunctionBody(ArgSymbol* fileArg, ArgSymbol* arg) {
  BlockStmt* body = new BlockStmt();
  if (classTag == CLASS_CLASS) {
    BlockStmt* fwriteNil = new BlockStmt();
    fwriteNil->insertAtTail(new CallExpr("fwrite", fileArg, new_StringSymbol("nil")));
    fwriteNil->insertAtTail(new ReturnStmt());
    body->insertAtTail(new CondStmt(new CallExpr("==", arg, gNil),
                                    fwriteNil));
  }

  if (classTag == CLASS_CLASS) {
    body->insertAtTail(new CallExpr("fwrite", fileArg, new_StringSymbol("{")));
  } else {
    body->insertAtTail(new CallExpr("fwrite", fileArg, new_StringSymbol("(")));
  }

  if (classTag == CLASS_UNION) {
    CondStmt* cond = NULL;
    for_fields(tmp, this) {
      BlockStmt* writeFieldBlock = new BlockStmt();
      writeFieldBlock->insertAtTail(new CallExpr("fwrite", fileArg, new_StringSymbol(tmp->name)));
      writeFieldBlock->insertAtTail(new CallExpr("fwrite", fileArg, new_StringSymbol(" = ")));
      writeFieldBlock->insertAtTail(new CallExpr("fwrite", fileArg, 
                                      new CallExpr(".", arg, new_StringSymbol(tmp->name))));
      cond = new CondStmt(new CallExpr(PRIMITIVE_UNION_GETID, arg, new_IntSymbol(tmp->id)), writeFieldBlock, cond);
    }
    body->insertAtTail(cond);
  } else {
    bool first = true;
    for_fields(tmp, this) {
      if (tmp->isTypeVariable)
        continue;
      if (!first) {
        body->insertAtTail(new CallExpr("fwrite", fileArg, new_StringSymbol(", ")));
      }
      body->insertAtTail(new CallExpr("fwrite", fileArg, new_StringSymbol(tmp->name)));
      body->insertAtTail(new CallExpr("fwrite", fileArg, new_StringSymbol(" = ")));
      body->insertAtTail(new CallExpr("fwrite", fileArg, 
                                      new CallExpr(".", arg, new_StringSymbol(tmp->name))));
      first = false;
    }
  }

  if (classTag == CLASS_CLASS) {
    body->insertAtTail(new CallExpr("fwrite", fileArg, new_StringSymbol("}")));
  } else {
    body->insertAtTail(new CallExpr("fwrite", fileArg, new_StringSymbol(")")));
  }

  return body;
}


bool ClassType::hasDefaultReadFunction(void) {
  return true;
}


BlockStmt* ClassType::buildDefaultReadFunctionBody(ArgSymbol* fileArg, ArgSymbol* arg) {
  BlockStmt* body = new BlockStmt();
  Symbol* ignoreWhiteSpace = new VarSymbol("ignoreWhiteSpace");
  body->insertAtTail(new DefExpr(ignoreWhiteSpace, new SymExpr(gTrue)));
  Symbol* matchingCharWasRead = new VarSymbol("matchingCharWasRead");
  body->insertAtTail(new DefExpr(matchingCharWasRead, new_IntSymbol((int64)0)));
  CallExpr* fileArgFP = new CallExpr(".", fileArg, new_StringSymbol("fp"));
  CallExpr* readOpenBrace = new CallExpr("_readLitChar", fileArgFP, new_StringSymbol("{"), ignoreWhiteSpace);
  body->insertAtTail(new CallExpr("=", matchingCharWasRead, readOpenBrace));
  CallExpr* notRead = new CallExpr("!", matchingCharWasRead);
  Expr* readError = new CallExpr("halt", new_StringSymbol("Read of the class failed: "), new CallExpr("_get_errno"));
  CondStmt* readErrorCond = new CondStmt(notRead, readError);
  body->insertAtTail(readErrorCond);
  bool first = true;
  for_fields(tmp, this) {
    if (tmp->isTypeVariable)
      continue;
    if (!first) {
      CallExpr* readComma = new CallExpr("_readLitChar", fileArgFP->copy(), new_StringSymbol(","), ignoreWhiteSpace);
      body->insertAtTail(new CallExpr("=", matchingCharWasRead, readComma));
      body->insertAtTail(readErrorCond->copy());
    }  
    Symbol* fieldName = new VarSymbol("fieldName");
    body->insertAtTail(new DefExpr(fieldName, new_StringSymbol("")));
    CallExpr* readFieldName = new CallExpr("fread", fileArg, fieldName);
    body->insertAtTail(readFieldName);
    Symbol* name = new_StringSymbol(tmp->name);
    Expr* confirmFieldName = new CallExpr("!=", fieldName, name);
    CondStmt* fieldNameCond = new CondStmt(confirmFieldName, readError->copy());
    body->insertAtTail(fieldNameCond);
    CallExpr* readEqualSign = new CallExpr("_readLitChar", fileArgFP->copy(), new_StringSymbol("="), ignoreWhiteSpace);
    body->insertAtTail(new CallExpr("=", matchingCharWasRead, readEqualSign));
    body->insertAtTail(readErrorCond->copy());
    CallExpr* argName = new CallExpr(".", arg, name);
    CallExpr* readValue = new CallExpr("fread", fileArg, argName);
    body->insertAtTail(readValue);
    first = false;
  }
  CallExpr* readCloseBrace = new CallExpr("_readLitChar", fileArgFP->copy(), new_StringSymbol("}"), ignoreWhiteSpace);
  body->insertAtTail(new CallExpr("=", matchingCharWasRead, readCloseBrace));
  body->insertAtTail(readErrorCond->copy());
  return body;
}



Symbol* ClassType::getField(char* name) {
  for_fields(sym, this) {
    if (!strcmp(sym->name, name))
      return sym;
  }
  INT_FATAL(this, "field not in class in getField");
  return NULL;
}


Symbol* ClassType::getField(int i) {
  return dynamic_cast<DefExpr*>(fields->get(i))->sym;
}


static PrimitiveType* 
createPrimitiveType( char *name, char *cname) {
  PrimitiveType* pt = new PrimitiveType(NULL);
  TypeSymbol* ts = new TypeSymbol(name, pt);
  ts->cname = cname;
  rootScope->define(ts);
  return pt;
}


// Create new primitive type for integers. Specify name for now. Though it will 
// probably be something like int1, int8, etc. in the end. In that case
// we can just specify the width (i.e., size).
#define INIT_PRIMITIVE_INT( name, width)                                 \
  dtInt[INT_SIZE_ ## width] = createPrimitiveType (name, "_int" #width); \
  dtInt[INT_SIZE_ ## width]->defaultValue = new_IntSymbol( 0, INT_SIZE_ ## width)

#define INIT_PRIMITIVE_UINT( name, width)                                  \
  dtUInt[INT_SIZE_ ## width] = createPrimitiveType (name, "_uint" #width); \
  dtUInt[INT_SIZE_ ## width]->defaultValue = new_UIntSymbol( 0, INT_SIZE_ ## width)

#define INIT_PRIMITIVE_REAL( name, width)                                     \
  dtReal[FLOAT_SIZE_ ## width] = createPrimitiveType (name, "_real" #width); \
  dtReal[FLOAT_SIZE_ ## width]->defaultValue = new_RealSymbol( "0.0", 0.0, FLOAT_SIZE_ ## width)
  
#define INIT_PRIMITIVE_IMAG( name, width)                               \
  dtImag[FLOAT_SIZE_ ## width] = createPrimitiveType (name, "_imag" #width); \
  dtImag[FLOAT_SIZE_ ## width]->defaultValue = new_ImagSymbol( "0.0", 0.0, FLOAT_SIZE_ ## width)
  
#define INIT_PRIMITIVE_COMPLEX( name, width)                                   \
  dtComplex[COMPLEX_SIZE_ ## width]= createPrimitiveType (name, "_complex" #width); \
  dtComplex[COMPLEX_SIZE_ ## width]->defaultValue = new_ComplexSymbol(         \
                                  "_chpl_complex" #width "(0.0, 0.0)",         \
                                   0.0, 0.0, COMPLEX_SIZE_ ## width)

#define CREATE_DEFAULT_SYMBOL(primType, gSym, name)                     \
  gSym = new VarSymbol (name, primType, VAR_NORMAL, VAR_CONST);         \
  rootScope->define (gSym);                                             \
  primType->defaultValue = gSym


void initPrimitiveTypes(void) {
  rootScope = new SymScope(NULL, NULL);

  dtNil = createPrimitiveType ("_nilType", "_nilType");
  CREATE_DEFAULT_SYMBOL (dtNil, gNil, "nil");
  
  dtUnknown = createPrimitiveType ("_unknown", "_unknown");
  CREATE_DEFAULT_SYMBOL (dtUnknown, gUnknown, "_gunknown");

  dtVoid = createPrimitiveType ("void", "void");
  CREATE_DEFAULT_SYMBOL (dtVoid, gVoid, "_void");

  dtObject = createPrimitiveType("object", "_chpl_object");
  dtObject->defaultValue = gNil;
  dtValue = createPrimitiveType("value", "_chpl_value");

  dtBool = createPrimitiveType ("bool", "_bool");

  // Create initial compiler module and its scope
  compilerModule = build_module("_chpl_compiler", MOD_STANDARD, new AList());

  CREATE_DEFAULT_SYMBOL (dtBool, gFalse, "false");
  gFalse->immediate = new Immediate;
  gFalse->immediate->v_bool = false;
  gFalse->immediate->const_kind = NUM_KIND_UINT;
  gFalse->immediate->num_index = INT_SIZE_1;
  uniqueConstantsHash.put(gFalse->immediate, gFalse);
  dtBool->defaultValue = gFalse;

  gTrue = new VarSymbol("true", dtBool, VAR_NORMAL, VAR_CONST);
  rootScope->define(gTrue); // SJD: Should intrinsics have DefExprs?
  gTrue->immediate = new Immediate;
  gTrue->immediate->v_bool = true;
  gTrue->immediate->const_kind = NUM_KIND_UINT;
  gTrue->immediate->num_index = INT_SIZE_1;
  uniqueConstantsHash.put(gTrue->immediate, gTrue);

  gBoundsChecking = new VarSymbol("boundsChecking", dtBool, VAR_NORMAL, VAR_CONST);
  rootScope->define(gBoundsChecking);
  if (no_bounds_checking) {
    gBoundsChecking->immediate = new Immediate;
    *gBoundsChecking->immediate = *gFalse->immediate;
  } else {
    gBoundsChecking->immediate = new Immediate;
    *gBoundsChecking->immediate = *gTrue->immediate;
  }

  // WAW: could have a loop, but the following unrolling is more explicit.
  INIT_PRIMITIVE_INT( "int(8)", 8);
  INIT_PRIMITIVE_INT( "int(16)", 16);
  INIT_PRIMITIVE_INT( "int", 32);
  INIT_PRIMITIVE_INT( "int(64)", 64);            // default size

  INIT_PRIMITIVE_UINT( "uint(8)", 8);
  INIT_PRIMITIVE_UINT( "uint(16)", 16);
  INIT_PRIMITIVE_UINT( "uint", 32);
  INIT_PRIMITIVE_UINT( "uint(64)", 64);          // default size

  INIT_PRIMITIVE_REAL( "real(32)", 32);
  INIT_PRIMITIVE_REAL( "real", 64);            // default size
  INIT_PRIMITIVE_REAL( "real(128)", 128);

  INIT_PRIMITIVE_IMAG( "imag(32)", 32);
  INIT_PRIMITIVE_IMAG( "imag", 64);            // default size
  INIT_PRIMITIVE_IMAG( "imag(128)", 128);

  INIT_PRIMITIVE_COMPLEX( "complex(64)", 64);
  INIT_PRIMITIVE_COMPLEX( "complex", 128);       // default size
  INIT_PRIMITIVE_COMPLEX( "complex(256)", 256);

  dtString = createPrimitiveType( "string", "_string");
  dtString->defaultValue = new_StringSymbol("");

  dtSymbol = createPrimitiveType( "symbol", "_symbol"); 

  dtFile = createPrimitiveType ("_file", "_cfile");
  CREATE_DEFAULT_SYMBOL(dtFile, gFile, "0");

  dtTimer = createPrimitiveType("_timervalue", "_timervalue");
  CREATE_DEFAULT_SYMBOL(dtTimer, gTimer, "_new_timer()");

  dtMutex = createPrimitiveType( "_mutex", "_chpl_mutex_t"); 
  dtMutex_p = createPrimitiveType( "_mutex_p", "_chpl_mutex_p"); 
  CREATE_DEFAULT_SYMBOL (dtMutex_p, gMutex_p, "NULL");
  dtCondVar_p = createPrimitiveType( "_condvar_p", "_chpl_condvar_p"); 
  CREATE_DEFAULT_SYMBOL (dtCondVar_p, gCondVar_p, "NULL");

  dtAny = createPrimitiveType ("_any", "_any");
  dtAnyInt = createPrimitiveType ("_anyint", "_anyint");
  dtMethodToken = createPrimitiveType ("_MT", "_MT");
  CREATE_DEFAULT_SYMBOL (dtMethodToken, gMethodToken, "_mt");
  dtSetterToken = createPrimitiveType ("_ST", "_ST");
  CREATE_DEFAULT_SYMBOL (dtSetterToken, gSetterToken, "_st");
}


bool is_int_type(Type *t) {
  return
    t == dtInt[INT_SIZE_8] ||
    t == dtInt[INT_SIZE_16] ||
    t == dtInt[INT_SIZE_32] ||
    t == dtInt[INT_SIZE_64];
}


bool is_uint_type(Type *t) {
  return
    t == dtUInt[INT_SIZE_8] ||
    t == dtUInt[INT_SIZE_16] ||
    t == dtUInt[INT_SIZE_32] ||
    t == dtUInt[INT_SIZE_64];
}


bool is_real_type(Type *t) {
  return
    t == dtReal[FLOAT_SIZE_32] ||
    t == dtReal[FLOAT_SIZE_64] ||
    t == dtReal[FLOAT_SIZE_128];
}


bool is_imag_type(Type *t) {
  return
    t == dtImag[FLOAT_SIZE_32] ||
    t == dtImag[FLOAT_SIZE_64] ||
    t == dtImag[FLOAT_SIZE_128];
}


bool is_complex_type(Type *t) {
  return
    t == dtComplex[COMPLEX_SIZE_64] ||
    t == dtComplex[COMPLEX_SIZE_128] ||
    t == dtComplex[COMPLEX_SIZE_256];
}


int get_width(Type *t) {
  if (t == dtInt[INT_SIZE_8] || t == dtUInt[INT_SIZE_8])
    return 8;
  if (t == dtInt[INT_SIZE_16] || t == dtUInt[INT_SIZE_16])
    return 16;
  if (t == dtInt[INT_SIZE_32] || 
      t == dtUInt[INT_SIZE_32] ||
      t == dtReal[FLOAT_SIZE_32] ||
      t == dtImag[FLOAT_SIZE_32])
    return 32;
  if (t == dtInt[INT_SIZE_64] || 
      t == dtUInt[INT_SIZE_64] ||
      t == dtReal[FLOAT_SIZE_64] ||
      t == dtImag[FLOAT_SIZE_64] ||
      t == dtComplex[COMPLEX_SIZE_64])
    return 64;
  if (t == dtReal[FLOAT_SIZE_128] ||
      t == dtImag[FLOAT_SIZE_128] ||
      t == dtComplex[COMPLEX_SIZE_128])
    return 128;
  if (t == dtComplex[COMPLEX_SIZE_256])
    return 256;
  INT_FATAL(t, "Unknown bit width");
  return 0;
}
