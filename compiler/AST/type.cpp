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
  asymbol(NULL),
  metaType(NULL),
  isGeneric(false),
  instantiatedFrom(NULL),
  instantiatedWith(NULL),
  clonedFrom(NULL)
{ 
  typeParents.clear();
  dispatchParents.clear();
  if (astType != TYPE_META)
    metaType = new MetaType(this);
}


void Type::verify() {
  BaseAST::verify();
  if (parentSymbol)
    INT_FATAL(this, "Type has parentSymbol set");
}


void Type::addSymbol(TypeSymbol* newsymbol) {
  symbol = newsymbol;
}


Type*
Type::copyInner(ASTMap* map) {
  INT_FATAL(this, "Illegal call to Type::copy");
  return NULL;
}


Type *Type::instantiate_generic(ASTMap &substitutions) {
  if (astType == TYPE_VARIABLE) {
    if (Type *t = dynamic_cast<Type*>(substitutions.get(this)))
      return t;
  }
  return 0;
}


void Type::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == defaultValue) {
    defaultValue = dynamic_cast<Symbol*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in Type::replaceChild");
  }
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
      this == dtInt[INT_TYPE_8]  ||
      this == dtInt[INT_TYPE_16] ||
      this == dtInt[INT_TYPE_32] ||
      this == dtInt[INT_TYPE_64] ||
      this == dtUInt[INT_TYPE_1]  ||
      this == dtUInt[INT_TYPE_8]  ||
      this == dtUInt[INT_TYPE_16] ||
      this == dtUInt[INT_TYPE_32] ||
      this == dtUInt[INT_TYPE_64] ||
      this == dtFloat[FLOAT_TYPE_32] ||
      this == dtFloat[FLOAT_TYPE_64] ||
      this == dtFloat[FLOAT_TYPE_128]) {
    return true;
  } else {
    return false;
 }
}

bool Type::hasDefaultWriteFunction(void) {
  return false;
}


AList<Stmt>* Type::buildDefaultWriteFunctionBody(ArgSymbol* fileArg, ArgSymbol* arg) {
  return new AList<Stmt>();
}


bool Type::hasDefaultReadFunction(void) {
  return false;
}


AList<Stmt>* Type::buildDefaultReadFunctionBody(ArgSymbol* fileArg, ArgSymbol* arg) {
  return new AList<Stmt>();
}


PrimitiveType::PrimitiveType(Symbol *init) :
  Type(TYPE_PRIMITIVE, init)
{}


void PrimitiveType::verify() {
  Type::verify();
  if (astType != TYPE_PRIMITIVE) {
    INT_FATAL(this, "Bad PrimitiveType::astType");
  }
  if (prev || next) {
    INT_FATAL(this, "Type is in AList");
  }
}


FnType::FnType(void) :
  Type(TYPE_FN, NULL)
{}


void FnType::verify() {
  Type::verify();
  if (astType != TYPE_FN) {
    INT_FATAL(this, "Bad FnType::astType");
  }
  if (prev || next) {
    INT_FATAL(this, "Type is in AList");
  }
}


void FnType::codegen(FILE* outfile) {
  INT_FATAL(this, "FnType is not yet implemented!\n");
}


void FnType::codegenDef(FILE* outfile) {
  INT_FATAL(this, "FnType is not yet implemented\n");
}


EnumType::EnumType(AList<DefExpr>* init_constants) :
  Type(TYPE_ENUM, init_constants->first()->sym),
  constants(init_constants)
{
  for_alist(DefExpr, def, constants)
    def->sym->type = this;
}


void EnumType::verify() {
  Type::verify();
  if (astType != TYPE_ENUM) {
    INT_FATAL(this, "Bad EnumType::astType");
  }
  if (prev || next) {
    INT_FATAL(this, "Type is in AList");
  }
}


EnumType*
EnumType::copyInner(ASTMap* map) {
  EnumType* copy = new EnumType(COPY_INT(constants));
  if (map) {
    map->put(metaType, copy->metaType);
    map->put(metaType->symbol, copy->metaType->symbol);
  }
  copy->addSymbol(symbol);
  return copy;
}


void EnumType::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == defaultValue) {
    defaultValue = dynamic_cast<Symbol*>(new_ast);
  } else if (old_ast == constants) {
    constants = dynamic_cast<AList<DefExpr>*>(new_ast);
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


AList<Stmt>* EnumType::buildDefaultWriteFunctionBody(ArgSymbol* fileArg, ArgSymbol* arg) {
  CondStmt* body = NULL;
  for_alist(DefExpr, constant, constants) {
    Expr* constantName = new_StringLiteral(constant->sym->name);
    body = new CondStmt(new CallExpr("==", arg, constant->sym),
                        new ExprStmt(new CallExpr("fwrite", fileArg, constantName)),
                        body);
  }
  return new AList<Stmt>(body);
}


bool EnumType::hasDefaultReadFunction(void) {
  return true;
}


AList<Stmt>* EnumType::buildDefaultReadFunctionBody(ArgSymbol* fileArg, ArgSymbol* arg) {
  AList<Stmt>* body = new AList<Stmt>();
  Symbol* valString = new VarSymbol("valString");
  body->insertAtTail(new DefExpr(valString, new_StringLiteral("")));
  body->insertAtTail(new CallExpr("fread", fileArg, valString));
  Stmt* haltStmt = new ExprStmt(new CallExpr("halt", 
                                  new_StringLiteral("***Error: Not of "), 
                                  new_StringLiteral(arg->type->symbol->name), 
                                  new_StringLiteral(" type***")));
  Stmt* elseStmt = haltStmt;

  for_alist_backward(DefExpr, constant, this->constants) {
    Expr* symName = new_StringLiteral(constant->sym->name);
    Expr* cond = new CallExpr("==", valString, symName);
    Stmt* thenStmt = new ExprStmt(new CallExpr("=", arg, constant->sym));
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
  if (prev || next) {
    INT_FATAL(this, "Type is in AList");
  }
}


UserType*
UserType::copyInner(ASTMap* map) {
  UserType* copy = new UserType(COPY_INT(typeExpr));
  if (map) { 
    map->put(metaType, copy->metaType);
    map->put(metaType->symbol, copy->metaType->symbol);
  }
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


void UserType::codegenDef(FILE* outfile) {
  INT_FATAL(this, "UserType should be removed by codegen time");
}


void UserType::codegenDefaultFormat(FILE* outfile, bool isRead) {
  INT_FATAL(this, "UserType should be removed by codegen time");
}


ClassType::ClassType(ClassTag initClassTag) :
  Type(TYPE_CLASS, NULL),
  classTag(initClassTag),
  structScope(NULL),
  declarationList(new AList<Stmt>()),
  inherits(new AList<Expr>())
{
  if (classTag == CLASS_CLASS) { // set defaultValue to nil to keep it
                                 // from being constructed
    defaultValue = gNil;
  }
  fields.clear();
  methods.clear();
  types.clear();
  isPattern = false;
  fieldSelector = NULL;
}


void ClassType::verify() {
  Type::verify();
  if (astType != TYPE_CLASS) {
    INT_FATAL(this, "Bad ClassType::astType");
  }
  if (prev || next) {
    INT_FATAL(this, "Type is in AList");
  }
  if (classTag != CLASS_CLASS && classTag != CLASS_RECORD)
    INT_FATAL(this, "Bad ClassType::classTag");
}


ClassType*
ClassType::copyInner(ASTMap* map) {
  ClassType* copy_type = new ClassType(classTag);
  if (map) {
    map->put(metaType, copy_type->metaType);
    map->put(metaType->symbol, copy_type->metaType->symbol);
  }
  AList<Stmt>* new_decls = new AList<Stmt>();
  for (Stmt* old_decls = declarationList->first();
       old_decls;
       old_decls = declarationList->next()) {
    ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(old_decls);
    DefExpr* defExpr = exprStmt ? dynamic_cast<DefExpr*>(exprStmt->expr) : NULL;
    if (defExpr && dynamic_cast<FnSymbol*>(defExpr->sym)) {
      copy_type->methods.add(dynamic_cast<FnSymbol*>(defExpr->sym));
    } else {
      new_decls->insertAtTail(COPY_INT(old_decls));
    }
  }
  copy_type->addDeclarations(new_decls);
  copy_type->isPattern = isPattern;
  return copy_type;
}


void ClassType::addDeclarations(AList<Stmt>* newDeclarations,
                                Stmt* beforeStmt) {
  Vec<BaseAST*> asts;
  collect_top_asts(&asts, newDeclarations);
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* defExpr = dynamic_cast<DefExpr*>(ast)) {
      if (FnSymbol* fn = dynamic_cast<FnSymbol*>(defExpr->sym)) {
        methods.add(fn);
        fn->typeBinding = this->symbol;
        if (fn->fnClass != FN_CONSTRUCTOR)
          fn->isMethod = true;
      }
    }
  }

  if (beforeStmt) {
    beforeStmt->insertBefore(newDeclarations);
  } else {
    declarationList->insertAtTail(newDeclarations);
  }
  types.clear();
  fields.clear();

  asts.clear();
  collect_top_asts(&asts, declarationList);
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* defExpr = dynamic_cast<DefExpr*>(ast)) {
      if (TypeSymbol* ts = dynamic_cast<TypeSymbol*>(defExpr->sym)) {
        types.add(ts);
      } else if (dynamic_cast<VarSymbol*>(defExpr->sym) ||
                 dynamic_cast<ArgSymbol*>(defExpr->sym)) {
        fields.add(defExpr->sym);
      }
    }
  }
}


void ClassType::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == defaultValue) {
    defaultValue = dynamic_cast<Symbol*>(new_ast);
  } else if (old_ast == declarationList) {
    declarationList = dynamic_cast<AList<Stmt>*>(new_ast);
  } else if (old_ast == inherits) {
    inherits = dynamic_cast<AList<Expr>*>(new_ast);
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
  for_alist(Stmt, stmt, declarationList) {
    if (ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(stmt)) {
      if (DefExpr* defExpr = dynamic_cast<DefExpr*>(exprStmt->expr)) {
        if (VarSymbol* var = dynamic_cast<VarSymbol*>(defExpr->sym)) {
          var->codegenDef(outfile);
          printedSomething = true;
        }
      }
    }
  }
  if (symbol->hasPragma("data class")) {
    fields.v[1]->type->codegen(outfile);
    fprintf(outfile, "* _data;\n");
  }
  if (!printedSomething) {
    fprintf(outfile, "int _emptyStructPlaceholder;\n");
  }
  fprintf(outfile, "};\n\n");
}


void ClassType::codegenStructName(FILE* outfile) {
  fprintf(outfile, "_");
  symbol->codegen(outfile);
  fprintf(outfile,", *");
  symbol->codegen(outfile);
}


void ClassType::codegenPrototype(FILE* outfile) {
  fprintf(outfile, "typedef struct __");
  symbol->codegen(outfile);
  fprintf(outfile, " ");
  codegenStructName(outfile);
  fprintf(outfile, ";\n");
}


bool ClassType::implementedUsingCVals(void) {
  return false;
}


bool ClassType::hasDefaultWriteFunction(void) {
  return true;
}


AList<Stmt>* ClassType::buildDefaultWriteFunctionBody(ArgSymbol* fileArg, ArgSymbol* arg) {
  AList<Stmt>* body = new AList<Stmt>();
  if (classTag == CLASS_CLASS) {
    AList<Stmt>* fwriteNil =
      new AList<Stmt>(new CallExpr("fwrite", fileArg, new_StringLiteral("nil")));
    fwriteNil->insertAtTail(new ReturnStmt());
    BlockStmt* blockStmt = new BlockStmt(fwriteNil);
    Expr* argIsNil = new CallExpr("==", arg, gNil);
    body->insertAtTail(new CondStmt(argIsNil, blockStmt));
  }

  if (classTag == CLASS_CLASS) {
    body->insertAtTail(new CallExpr("fwrite", fileArg, new_StringLiteral("{")));
  } else {
    body->insertAtTail(new CallExpr("fwrite", fileArg, new_StringLiteral("(")));
  }

  bool first = true;
  forv_Vec(Symbol, tmp, fields) {
    if (!first) {
      body->insertAtTail(new CallExpr("fwrite", fileArg, new_StringLiteral(", ")));
    }
    body->insertAtTail(new CallExpr("fwrite", fileArg, new_StringLiteral(tmp->name)));
    body->insertAtTail(new CallExpr("fwrite", fileArg, new_StringLiteral(" = ")));
    body->insertAtTail(new CallExpr("fwrite", fileArg, 
                                    new CallExpr(".", arg, new_StringSymbol(tmp->name))));
    first = false;
  }

  if (classTag == CLASS_CLASS) {
    body->insertAtTail(new CallExpr("fwrite", fileArg, new_StringLiteral("}")));
  } else {
    body->insertAtTail(new CallExpr("fwrite", fileArg, new_StringLiteral(")")));
  }

  return body;
 }


bool ClassType::hasDefaultReadFunction(void) {
  return true;
}


AList<Stmt>* ClassType::buildDefaultReadFunctionBody(ArgSymbol* fileArg, ArgSymbol* arg) {
  AList<Stmt>* body = new AList<Stmt>();
  Symbol* ignoreWhiteSpace = new VarSymbol("ignoreWhiteSpace");
  body->insertAtTail(new DefExpr(ignoreWhiteSpace, new_IntLiteral(1)));
  Symbol* matchingCharWasRead = new VarSymbol("matchingCharWasRead");
  body->insertAtTail(new DefExpr(matchingCharWasRead, new_IntLiteral((int64)0)));
  CallExpr* fileArgFP = new CallExpr(PRIMITIVE_GET_MEMBER, fileArg, new_StringSymbol("fp"));
  CallExpr* readOpenBrace = new CallExpr("_readLitChar", fileArgFP, new_StringLiteral("{"), ignoreWhiteSpace);
  body->insertAtTail(new CallExpr("=", matchingCharWasRead, readOpenBrace));
  CallExpr* notRead = new CallExpr("!", matchingCharWasRead);
  Stmt* readError = new ExprStmt(new CallExpr("_classReadError"));
  CondStmt* readErrorCond = new CondStmt(notRead, readError);
  body->insertAtTail(readErrorCond);
  bool first = true;
  forv_Vec(Symbol, tmp, fields) {
    if (!first) {
      CallExpr* readComma = new CallExpr("_readLitChar", fileArgFP->copy(), new_StringLiteral(","), ignoreWhiteSpace);
      body->insertAtTail(new CallExpr("=", matchingCharWasRead, readComma));
      body->insertAtTail(readErrorCond->copy());
    }  
    Symbol* fieldName = new VarSymbol("fieldName");
    body->insertAtTail(new DefExpr(fieldName, new_StringLiteral("")));
    CallExpr* readFieldName = new CallExpr("fread", fileArg, fieldName);
    body->insertAtTail(readFieldName);
    Expr* name = new_StringLiteral(tmp->name);
    Expr* confirmFieldName = new CallExpr("!=", fieldName, name);
    CondStmt* fieldNameCond = new CondStmt(confirmFieldName, readError->copy());
    body->insertAtTail(fieldNameCond);
    CallExpr* readEqualSign = new CallExpr("_readLitChar", fileArgFP->copy(), new_StringLiteral("="), ignoreWhiteSpace);
    body->insertAtTail(new CallExpr("=", matchingCharWasRead, readEqualSign));
    body->insertAtTail(readErrorCond->copy());
    CallExpr* argName = new CallExpr(PRIMITIVE_GET_MEMBER, arg, name->copy());
    CallExpr* readValue = new CallExpr("fread", fileArg, argName);
    body->insertAtTail(readValue);
    first = false;
  }
  CallExpr* readCloseBrace = new CallExpr("_readLitChar", fileArgFP->copy(), new_StringLiteral("}"), ignoreWhiteSpace);
  body->insertAtTail(new CallExpr("=", matchingCharWasRead, readCloseBrace));
  body->insertAtTail(readErrorCond->copy());
  return body;
}


MetaType::MetaType(Type* init_base) :
  Type(TYPE_META, NULL),
  base(init_base)
{
  static int uid = 1;
  char* name = stringcat("_meta_type", intstring(uid++));
  metaType = this;
  symbol = new TypeSymbol(name, NULL);
  symbol->definition = this;
  symbol->type = this;
}


void MetaType::verify() {
  Type::verify();
  if (astType != TYPE_META) {
    INT_FATAL(this, "Bad MetaType::astType");
  }
  if (prev || next) {
    INT_FATAL(this, "Type is in AList");
  }
}


VariableType::VariableType(Type *init_type) :
  Type(TYPE_VARIABLE, NULL), 
  type(init_type)
{
  isGeneric = true;
}


void VariableType::verify() {
  Type::verify();
  if (astType != TYPE_VARIABLE) {
    INT_FATAL(this, "Bad VariableType::astType");
  }
  if (prev || next) {
    INT_FATAL(this, "Type is in AList");
  }
}


VariableType*
VariableType::copyInner(ASTMap* map) {
  VariableType *copy = new VariableType(type);
  if (map) { 
    map->put(metaType, copy->metaType);
    map->put(metaType->symbol, copy->metaType->symbol);
  }
  return copy;
}


void VariableType::codegen(FILE* outfile) {
  INT_FATAL(this, "Unanticipated call to VariableType::codegen");
}


static PrimitiveType* 
createPrimitiveType(char *name, char *cname, char *ltname = NULL, char *ltcname = NULL) {
  PrimitiveType* pt = new PrimitiveType(NULL);
  TypeSymbol* ts = new TypeSymbol(name, pt);
  ts->cname = cname;
  rootScope->define(ts);
  return pt;
}


// Create new primitive type for integers. Specify name for now. Though it will 
// probably be something like int1, int8, etc. in the end. In that case
// we can just specify the width.
#define INIT_PRIMITIVE_INT( name, width)                                \
  dtInt[INT_TYPE_ ## width] = createPrimitiveType (name,            \
                                  "_int" #width, "_intLiteral" #width,  \
                                  "_int" #width "Literal");             \
  dtInt[INT_TYPE_ ## width]->defaultValue = new_IntSymbol( 0,       \
                                                INT_TYPE_ ## width)

#define INIT_PRIMITIVE_UINT( name, width)                               \
  dtUInt[INT_TYPE_ ## width] = createPrimitiveType (name,           \
                                   "_uint" #width, "_uintLiteral" #width,  \
                                   "_uint" #width "Literal");              \
  dtUInt[INT_TYPE_ ## width]->defaultValue = new_UIntSymbol( 0,        \
                                                 INT_TYPE_ ## width)

#define INIT_PRIMITIVE_FLOAT( name, width)                              \
  dtFloat[FLOAT_TYPE_ ## width] = createPrimitiveType (name,        \
                                   "_float" #width, "_floatLiteral" #width,   \
                                   "_float" #width "Literal");                \
  dtFloat[FLOAT_TYPE_ ## width]->defaultValue = new_FloatSymbol( "0.0", 0.0, FLOAT_TYPE_ ## width)
  

#define CREATE_DEFAULT_SYMBOL(primType, gSym, name)                     \
  gSym = new VarSymbol (name, primType, VAR_NORMAL, VAR_CONST);         \
  rootScope->define (gSym);                                             \
  primType->defaultValue = gSym

  
void initPrimitiveTypes(void) {
  rootScope = new SymScope(NULL, NULL);

  dtNil = createPrimitiveType ("_nilType", "_nilType");
  CREATE_DEFAULT_SYMBOL (dtNil, gNil, "nil");
  
  dtUnknown = createPrimitiveType ("_unknownType", "_unknownType");
  CREATE_DEFAULT_SYMBOL (dtUnknown, gUnknown, "_unknown");

  dtVoid = createPrimitiveType ("void", "void");
  CREATE_DEFAULT_SYMBOL (dtVoid, gVoid, "_void");

  dtBool = createPrimitiveType ("bool", "_bool",
                                             "_boolLiteral", "_boolLiteral");

  // Create initial compiler module and its scope
  compilerModule = build_module("_chpl_compiler", MOD_STANDARD, new AList<Stmt>());

  CREATE_DEFAULT_SYMBOL (dtBool, gFalse, "false");
  gFalse->immediate = new Immediate;
  gFalse->immediate->v_bool = false;
  gFalse->immediate->const_kind = NUM_KIND_UINT;
  gFalse->immediate->num_index = INT_TYPE_1;
  uniqueConstantsHash.put(gFalse->immediate, gFalse);
  dtBool->defaultValue = gFalse;

  gTrue = new VarSymbol("true", dtBool, VAR_NORMAL, VAR_CONST);
  rootScope->define(gTrue); // SJD: Should intrinsics have DefExprs?
  gTrue->immediate = new Immediate;
  gTrue->immediate->v_bool = true;
  gTrue->immediate->const_kind = NUM_KIND_UINT;
  gTrue->immediate->num_index = INT_TYPE_1;
  uniqueConstantsHash.put(gTrue->immediate, gTrue);

  // WAW: could have a loop, but the following unrolling is more explicit.
  INIT_PRIMITIVE_INT( "int", 64);
  INIT_PRIMITIVE_INT( "_int8", 8);
  INIT_PRIMITIVE_INT( "_int16", 16);
  INIT_PRIMITIVE_INT( "_int32", 32);

  INIT_PRIMITIVE_UINT( "uint", 64);
  INIT_PRIMITIVE_UINT( "_uint8", 8);
  INIT_PRIMITIVE_UINT( "_uint16", 16);
  INIT_PRIMITIVE_UINT( "_uint32", 32);

  INIT_PRIMITIVE_FLOAT( "float", 64);
  INIT_PRIMITIVE_FLOAT( "_float32", 32);
  INIT_PRIMITIVE_FLOAT( "_float128", 128);

  // This should point to the complex type defined in modules/standard/_chpl_complex.chpl
  dtComplex = createPrimitiveType ("complex", "_complex128",
                                   "_complexLiteral", "_complex128Literal");
  dtComplex->defaultValue = new_ComplexSymbol("_MAKE_COMPLEX64(0.0,0.0)", 
                                              0.0, 0.0);

  dtString = createPrimitiveType( "string", "_string",
                                  "_stringLiteral", "_stringLiteral");

  dtSymbol = createPrimitiveType( "symbol", "_symbol", 
                                  "_symbolLiteral", "_symbolLiteral");

  dtMutex = createPrimitiveType( "mutex", "_chpl_mutex_t", 
                                  "_symbolMutex", "_symbolMutex");

  dtAny = createPrimitiveType ("any", "_any");
}

Type *getMetaType(Type *t) {
  if (t)
    return t->metaType;
  else
    return dtAny->metaType;
}

void findInternalTypes(void) {
  dtMethodToken = dynamic_cast<TypeSymbol*>(baseModule->lookup("_methodTokenType"))->definition;
  dtSetterToken = dynamic_cast<TypeSymbol*>(baseModule->lookup("_setterTokenType"))->definition;
  if (!fnostdincs)
    dtFile = dynamic_cast<TypeSymbol*>(fileModule->lookup("file"))->definition;
  dtObject = dynamic_cast<ClassType*>(dynamic_cast<TypeSymbol*>(prelude->lookup("object"))->definition);
  dtObject->symbol->cname = "void*";
  dtValue = dynamic_cast<ClassType*>(dynamic_cast<TypeSymbol*>(prelude->lookup("value"))->definition);

  // SJD: Can't do this when dtString is defined because
  // prelude hasn't been made yet.  Need to do it after.
  dtString->defaultConstructor =
    dynamic_cast<FnSymbol*>(prelude->lookup("_init_string"));
}


void 
complete_closure(ClassType *ct, Vec<Type *> types) {
  currentLineno = 0;
  AList<Stmt>* decls = new AList<Stmt>();
  for (int i = 1; i <= types.n; i++) {
    char* fieldName = stringcat("_f", intstring(i));
    VarSymbol* field = new VarSymbol(fieldName, types.v[i-1]);
    decls->insertAtTail(new DefExpr(field));
  }
  ct->addDeclarations(decls);
}


bool is_int_type(Type *t) {
  return
    t == dtInt[INT_TYPE_8] ||
    t == dtInt[INT_TYPE_16] ||
    t == dtInt[INT_TYPE_32] ||
    t == dtInt[INT_TYPE_64];
}


bool is_uint_type(Type *t) {
  return
    t == dtUInt[INT_TYPE_8] ||
    t == dtUInt[INT_TYPE_16] ||
    t == dtUInt[INT_TYPE_32] ||
    t == dtUInt[INT_TYPE_64];
}


bool is_float_type(Type *t) {
  return
    t == dtFloat[FLOAT_TYPE_32] ||
    t == dtFloat[FLOAT_TYPE_64] ||
    t == dtFloat[FLOAT_TYPE_128];
}


int get_width(Type *t) {
  if (t == dtInt[INT_TYPE_8] || t == dtUInt[INT_TYPE_8])
    return 8;
  if (t == dtInt[INT_TYPE_16] || t == dtUInt[INT_TYPE_16])
    return 16;
  if (t == dtInt[INT_TYPE_32] || t == dtUInt[INT_TYPE_32])
    return 32;
  if (t == dtInt[INT_TYPE_64] || t == dtUInt[INT_TYPE_64])
    return 64;
  if (t == dtFloat[FLOAT_TYPE_32])
    return 32;
  if (t == dtFloat[FLOAT_TYPE_64])
    return 64;
  if (t == dtFloat[FLOAT_TYPE_128])
    return 128;
  INT_FATAL(t, "Unknown bit width");
  return 0;
}
