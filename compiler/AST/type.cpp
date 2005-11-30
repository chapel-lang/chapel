#include <typeinfo>
#define TYPE_EXTERN
#include "expr.h"
#include "files.h"
#include "misc.h"
#include "stringutil.h"
#include "symbol.h"
#include "symtab.h"
#include "type.h"
#include "../traversals/fixup.h"
#include "../traversals/updateSymbols.h"
#include "../passes/filesToAST.h"


// Utilities for building write functions
static ExprStmt* genWriteStmt(ArgSymbol* fileArg, Expr* arg) {
  return new ExprStmt(new CallExpr("fwrite", fileArg, arg));
}

static void addWriteStmt(AList<Stmt>* body, ArgSymbol* fileArg, Expr* arg) {
  body->insertAtTail(genWriteStmt(fileArg, arg));
}


Type::Type(astType_t astType, Symbol* init_defaultVal) :
  BaseAST(astType),
  symbol(NULL),
  defaultValue(init_defaultVal),
  defaultConstructor(NULL),
  asymbol(NULL),
  metaType(NULL),
  isGeneric(false),
  instantiatedFrom(NULL)
{ 
  typeParents.clear();
  dispatchParents.clear();
  if (astType != TYPE_META)
    metaType = new MetaType(this);
}


void Type::verify(void) {
  INT_FATAL(this, "Type::verify() should never be called");
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
  TRAVERSE(defaultValue, traversal, false);
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
  if (this == dtBoolean ||
      this == dtInteger ||
      this == dtFloat) {
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


void PrimitiveType::verify(void) {
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


void FnType::verify(void) {
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
{ }


void EnumType::verify(void) {
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


void EnumType::traverseDefType(Traversal* traversal) {
  TRAVERSE(constants, traversal, false);
  TRAVERSE(defaultValue, traversal, false);
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
  AList<WhenStmt>* selectWhenStmts = new AList<WhenStmt>();
  for_alist(DefExpr, constant, this->constants) {
    Expr* constantName = new_StringLiteral(constant->sym->name);
    ExprStmt* IOCall = genWriteStmt(fileArg, constantName);
    AList<Expr>* whenExpr = new AList<Expr>(new SymExpr(constant->sym));
    WhenStmt* thisWhenStmt = new WhenStmt(whenExpr, IOCall);
    selectWhenStmts->insertAtTail(thisWhenStmt);
  }
  return new AList<Stmt>(new SelectStmt(new SymExpr(arg), selectWhenStmts));
}


bool EnumType::hasDefaultReadFunction(void) {
  return true;
}


AList<Stmt>* EnumType::buildDefaultReadFunctionBody(ArgSymbol* fileArg, ArgSymbol* arg) {
  AList<Stmt>* readBodyStmts = new AList<Stmt>();
  Symbol* chplStdin = Symboltable::lookupInFileModuleScope("stdin");
  Symbol* valString = new VarSymbol("valString", dtString, VAR_NORMAL);
  ExprStmt* valStringDef = new ExprStmt(dynamic_cast<Expr*>(new DefExpr(valString)));
  readBodyStmts->insertAtTail(valStringDef);
  ExprStmt* freadCall = new ExprStmt(new CallExpr("fread", chplStdin, valString));
  readBodyStmts->insertAtTail(freadCall);
  Stmt* haltStmt = new ExprStmt(new CallExpr("halt", 
                                  new_StringLiteral("***Error: Not of "), 
                                  new_StringLiteral(arg->type->symbol->name), 
                                  new_StringLiteral(" type***")));
  Stmt* elseStmt = haltStmt;

  for_alist_backward(DefExpr, constant, this->constants) {
    Expr* symName = new_StringLiteral(constant->sym->name);
    Expr* cond = new CallExpr(OP_EQUAL, valString, symName);
    Stmt* thenStmt = new ExprStmt(new CallExpr(OP_GETS, arg, constant->sym));
    elseStmt = new CondStmt(cond, thenStmt, elseStmt);
    
  }
  readBodyStmts->insertAtTail(elseStmt);
  return readBodyStmts;
}


LiteralType::LiteralType(VarSymbol* init_literal) :
  Type(TYPE_LITERAL, NULL),
  literal(init_literal)
{ }


void LiteralType::verify(void) {
  if (astType != TYPE_LITERAL) {
    INT_FATAL(this, "Bad LiteralType::astType");
  }
  if (prev || next) {
    INT_FATAL(this, "Type is in AList");
  }
}


LiteralType*
LiteralType::copyInner(ASTMap* map) {
  LiteralType* copy = new LiteralType(COPY_INT(literal));
  if (map) {
    map->put(metaType, copy->metaType);
    map->put(metaType->symbol, copy->metaType->symbol);
  }
  copy->addSymbol(symbol);
  return copy;
}


void LiteralType::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == literal) {
    literal = dynamic_cast<VarSymbol*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in Type::replaceChild");
  }
}


void LiteralType::traverseDefType(Traversal* traversal) {
  TRAVERSE(literal, traversal, false);
}


void LiteralType::printDef(FILE* outfile) {
  fprintf(outfile, "type ");
  symbol->print(outfile);
  fprintf(outfile, " = ");
  literal->print(outfile);
}


void LiteralType::codegenDef(FILE* outfile) {
  fprintf(outfile, "typedef ");
  literal->type->codegen(outfile);
  fprintf(outfile, " ");
  symbol->codegen(outfile);
  fprintf(outfile, ";\n");
}

void LiteralType::codegenDefaultFormat(FILE* outfile, bool isRead) {
  literal->type->codegenDefaultFormat(outfile, isRead);
}


UserType::UserType(Type* init_underlyingType, Expr* init_defaultExpr) :
  Type(TYPE_USER, NULL),
  typeExpr(NULL),
  underlyingType(init_underlyingType),
  defaultExpr(NULL)
{}


UserType::UserType(Expr* init_typeExpr, Expr* init_defaultExpr) :
  Type(TYPE_USER, NULL),
  typeExpr(init_typeExpr),
  underlyingType(dtUnknown),
  defaultExpr(init_defaultExpr)
{}


UserType::UserType(Expr* init_typeExpr, Type* init_underlyingType, Expr* init_defaultExpr) :
  Type(TYPE_USER, NULL),
  typeExpr(init_typeExpr),
  underlyingType(init_underlyingType),
  defaultExpr(init_defaultExpr)
{}


void UserType::verify(void) {
  if (astType != TYPE_USER) {
    INT_FATAL(this, "Bad UserType::astType");
  }
  if (prev || next) {
    INT_FATAL(this, "Type is in AList");
  }
}


UserType*
UserType::copyInner(ASTMap* map) {
  UserType* copy = new UserType(COPY_INT(typeExpr),
                                COPY_INT(underlyingType),
                                COPY_INT(defaultExpr));
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
  } else if (old_ast == defaultExpr) {
    defaultExpr = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in Type::replaceChild");
  }
}


void UserType::traverseDefType(Traversal* traversal) {
  TRAVERSE(typeExpr, traversal, false);
  TRAVERSE(underlyingType, traversal, false);
  TRAVERSE(defaultExpr, traversal, false);
  TRAVERSE(defaultValue, traversal, false);
}


void UserType::printDef(FILE* outfile) {
  fprintf(outfile, "type ");
  symbol->print(outfile);
  fprintf(outfile, " = ");
  defaultExpr->print(outfile);
}


void UserType::codegenDef(FILE* outfile) {
  fprintf(outfile, "typedef ");
  underlyingType->codegen(outfile);
  fprintf(outfile, " ");
  symbol->codegen(outfile);
  fprintf(outfile, ";\n");
}


void UserType::codegenDefaultFormat(FILE* outfile, bool isRead) {
  underlyingType->codegenDefaultFormat(outfile, isRead);
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
  initFn = NULL;
}


void ClassType::verify(void) {
  if (astType != TYPE_CLASS) {
    INT_FATAL(this, "Bad ClassType::astType");
  }
  if (prev || next) {
    INT_FATAL(this, "Type is in AList");
  }
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
  for_alist(Stmt, stmt, newDeclarations) {
    if (ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(stmt)) {
      if (DefExpr* defExpr = dynamic_cast<DefExpr*>(exprStmt->expr)) {
        if (FnSymbol* fn = dynamic_cast<FnSymbol*>(defExpr->sym)) {
          fn->typeBinding = this->symbol;
          if (fn->fnClass != FN_CONSTRUCTOR) {
            fn->method_type = PRIMARY_METHOD;
          }
          methods.add(fn);
        }
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
  for_alist(Stmt, stmt, declarationList) {
    if (ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(stmt)) {
      if (DefExpr* defExpr = dynamic_cast<DefExpr*>(exprStmt->expr)) {
        if (TypeSymbol* ts = dynamic_cast<TypeSymbol*>(defExpr->sym)) {
          types.add(ts);
        } else if (dynamic_cast<VarSymbol*>(defExpr->sym) ||
                   dynamic_cast<ArgSymbol*>(defExpr->sym)) {
          fields.add(defExpr->sym);
        }
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


void ClassType::traverseDefType(Traversal* traversal) {
  SymScope* prevScope = NULL;
  if (structScope) {
    prevScope = Symboltable::setCurrentScope(structScope);
  }
  TRAVERSE(inherits, traversal, false);
  TRAVERSE(declarationList, traversal, false);
  TRAVERSE(defaultValue, traversal, false);
  if (structScope) {
    Symboltable::setCurrentScope(prevScope);
  }
}


bool
ClassType::isNominalType() {
  return classTag == CLASS_CLASS || classTag == CLASS_VALUECLASS;
}


int
is_Scalar_Type(Type *t) {
  if (UserType *ut = dynamic_cast<UserType*>(t))
    return is_Scalar_Type(ut->underlyingType);
  if (LiteralType *lt = dynamic_cast<LiteralType*>(t))
    return is_Scalar_Type(lt->literal->type);
  return t && 
    t != dtUnknown && 
    t != dtString && 
    t != dtNil && 
    t != dtAny && 
    (t->astType == TYPE_PRIMITIVE || t->astType == TYPE_ENUM);
}


int
is_Value_Type(Type *t) {
  if (UserType *ut = dynamic_cast<UserType*>(t))
    return is_Value_Type(ut->underlyingType);
  ClassType* ct = dynamic_cast<ClassType*>(t);
  return ct && (ct->classTag == CLASS_RECORD
                || ct->classTag == CLASS_VALUECLASS);
}


int
is_Reference_Type(Type *t) {
  if (UserType *ut = dynamic_cast<UserType*>(t))
    return is_Reference_Type(ut->underlyingType);
  if (!t)
    return false;
  if (t == dtNil || t->astType == TYPE_SUM)
    return true;
  ClassType* ct = dynamic_cast<ClassType*>(t);
  return (ct &&
          (ct->classTag == CLASS_CLASS));
}

void ClassType::codegenDef(FILE* outfile) {
  fprintf(outfile, "struct __");
  symbol->codegen(outfile);
  fprintf(outfile, " {\n");
  bool printedSomething = false; // BLC: this is to avoid empty structs, illegal in C
  AList<Stmt>* stmtList = declarationList;
  if (use_class_init)
    stmtList = initFn->body->body;
  for_alist(Stmt, stmt, stmtList) {
    if (ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(stmt)) {
      if (DefExpr* defExpr = dynamic_cast<DefExpr*>(exprStmt->expr)) {
        if (VarSymbol* var = dynamic_cast<VarSymbol*>(defExpr->sym)) {
          var->codegenDef(outfile);
          printedSomething = true;
        }
      }
    }
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
      new AList<Stmt>(genWriteStmt(fileArg, new_StringLiteral("nil")));
    fwriteNil->insertAtTail(new ReturnStmt());
    BlockStmt* blockStmt = new BlockStmt(fwriteNil);
    Symbol* nil = Symboltable::lookupInternal("nil", SCOPE_INTRINSIC);
    Expr* argIsNil = new CallExpr(OP_EQUAL, arg, nil);
    body->insertAtTail(new CondStmt(argIsNil, blockStmt));
  }

  if (classTag == CLASS_CLASS || classTag == CLASS_VALUECLASS) {
    addWriteStmt(body, fileArg, new_StringLiteral("{"));
  } else {
    addWriteStmt(body, fileArg, new_StringLiteral("("));
  }

  bool first = true;
  forv_Vec(Symbol, tmp, fields) {
    if (!first) {
      addWriteStmt(body, fileArg, new_StringLiteral(", "));
    }
    addWriteStmt(body, fileArg, new_StringLiteral(tmp->name));
    addWriteStmt(body, fileArg, new_StringLiteral(" = "));
    addWriteStmt(body, fileArg, new MemberAccess(arg, tmp));
    first = false;
  }

  if (classTag == CLASS_CLASS || classTag == CLASS_VALUECLASS) {
    addWriteStmt(body, fileArg, new_StringLiteral("}"));
  } else {
    addWriteStmt(body, fileArg, new_StringLiteral(")"));
  }

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


void MetaType::verify(void) {
  if (astType != TYPE_META) {
    INT_FATAL(this, "Bad MetaType::astType");
  }
  if (prev || next) {
    INT_FATAL(this, "Type is in AList");
  }
}


void MetaType::traverseDefType(Traversal* traversal) {
  TRAVERSE(base, traversal, false);
}

SumType::SumType() :
  Type(TYPE_SUM, gNil)
{
}

void SumType::verify(void) {
  if (astType != TYPE_SUM) {
    INT_FATAL(this, "Bad SumType::astType");
  }
  if (prev || next) {
    INT_FATAL(this, "Type is in AList");
  }
}


void SumType::addType(Type* additionalType) {
  components.add(additionalType);
}

void SumType::codegenDef(FILE* outfile) {
  fprintf(outfile, "typedef void *");
  symbol->codegen(outfile);
  fprintf(outfile, ";\n");
}

void SumType::codegenStructName(FILE* outfile) {
  symbol->codegen(outfile);
}

VariableType::VariableType(Type *init_type) :
  Type(TYPE_VARIABLE, NULL), 
  type(init_type)
{
  isGeneric = true;
}


void VariableType::verify(void) {
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

void VariableType::traverseDefType(Traversal* traversal) {
  TRAVERSE(type, traversal, false);
}


void initType(void) {
  dtUnknown = Symboltable::definePrimitiveType("???", "???");
  dtVoid = Symboltable::definePrimitiveType("void", "void");
  dtNil = Symboltable::definePrimitiveType("_nilType", "_nilType");

  dtBoolean = Symboltable::definePrimitiveType("boolean", "_boolean", new_BoolSymbol(false));
  dtInteger = Symboltable::definePrimitiveType("integer", "_integer64", new_IntSymbol(0));
  dtFloat = Symboltable::definePrimitiveType("float", "_float64", new_FloatSymbol("0.0", 0.0));
  // This should point to the complex type defined in modules/standard/_chpl_complex.chpl
  dtComplex = Symboltable::definePrimitiveType("complex", "_complex128", new_ComplexSymbol("_MAKE_COMPLEX64(0.0,0.0)", 0.0, 0.0));
  dtString = Symboltable::definePrimitiveType("string", "_string");

  dtNumeric = Symboltable::definePrimitiveType("numeric", "_numeric");
  dtAny = Symboltable::definePrimitiveType("any", "_any");
  dtLocale = Symboltable::definePrimitiveType("locale", "_locale", NULL);
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
  SumType* new_sum_type = new SumType;
  for (int i = 0; i < types->n; i++)
    new_sum_type->addType(types->v[i]);
  SumType *old_type = lub_cache.get(new_sum_type);
  if (old_type)
    return old_type;
  lub_cache.put(new_sum_type);
  char* name = stringcat("_sum_type", intstring(uid++));
  TypeSymbol* sym = new TypeSymbol(name, new_sum_type);
  commonModule->modScope->define(sym); // SJD: Yuck, sum types should
                                       // have a DefExpr and be
                                       // inserted
  return new_sum_type;
}


Type *getMetaType(Type *t) {
  if (t)
    return t->metaType;
  else
    return dtAny->metaType;
}

void findInternalTypes(void) {
  dtMethodToken = Symboltable::lookupInternalType("_methodTokenType")->definition;
  dtSetterToken = Symboltable::lookupInternalType("_setterTokenType")->definition;

  dtObject = dynamic_cast<ClassType*>(Symboltable::lookupInternalType("object")->definition);
  dtValue = dynamic_cast<ClassType*>(Symboltable::lookupInternalType("value")->definition);

  // These should all be eliminated.  Note they almost are since they
  // are MetaTypes, not the types in the prelude.
  dtTuple = Symboltable::lookupInternalType("Tuple")->type;
  dtDomain = Symboltable::lookupInternalType("Domain")->type;
  dtArray = Symboltable::lookupInternalType("Array")->type;
  dtSequence = Symboltable::lookupInternalType("_seq")->type;

  // SJD: Can't do this when dtString is defined because
  // prelude hasn't been made yet.  Need to do it after.
  dtString->defaultConstructor =
    dynamic_cast<FnSymbol*>(Symboltable::lookupInternal("_init_string"));
  initConfigFn =
    dynamic_cast<FnSymbol*>(Symboltable::lookupInternal("_INIT_CONFIG"));
}


LiteralType *
new_LiteralType(VarSymbol *literal_var) {
  static int uid = 1;
  assert(literal_var->immediate);
  if (literal_var->literalType)
    return literal_var->literalType;
  literal_var->literalType = new LiteralType(literal_var);
  char* name = stringcat("_literal_type", intstring(uid++));
  TypeSymbol* sym = new TypeSymbol(name, literal_var->literalType);
  literal_var->literalType->addSymbol(sym);
  commonModule->stmts->insertAtTail(new ExprStmt(new DefExpr(sym)));
  literal_var->literalType->defaultValue = literal_var;
  return literal_var->literalType;
}

