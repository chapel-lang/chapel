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
#include "../passes/filesToAST.h"


// Utilities for building write functions
static void addWriteStmt(AList<Stmt>* body, Expr* arg) {
  body->insertAtTail(new ExprStmt(new CallExpr("write", arg)));
}


Type::Type(astType_t astType, Expr* init_defaultVal) :
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
Type::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  INT_FATAL(this, "Illegal call to Type::copy");
  return NULL;
}


Type *Type::instantiate_generic(Map<BaseAST *, BaseAST *> &substitutions) {
  if (astType == TYPE_VARIABLE) {
    if (Type *t = dynamic_cast<Type*>(substitutions.get(this)))
      return t;
  }
  return 0;
}


void Type::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == defaultValue) {
    defaultValue = dynamic_cast<Expr*>(new_ast);
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


AList<Stmt>* Type::buildDefaultWriteFunctionBody(ArgSymbol* arg) {
  return new AList<Stmt>();
}


bool Type::hasDefaultReadFunction(void) {
  return false;
}


AList<Stmt>* Type::buildDefaultReadFunctionBody(ArgSymbol* arg) {
  return new AList<Stmt>();
}


PrimitiveType::PrimitiveType(Expr *initExpr) :
  Type(TYPE_PRIMITIVE, initExpr)
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
  Type(TYPE_ENUM, new SymExpr(init_constants->first()->sym)),
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
EnumType::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  EnumType* copy = new EnumType(CLONE_INTERNAL(constants));
  copy->addSymbol(symbol);
  return copy;
}


void EnumType::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == defaultValue) {
    defaultValue = dynamic_cast<Expr*>(new_ast);
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


AList<Stmt>* EnumType::buildDefaultWriteFunctionBody(ArgSymbol* arg) {
  return new AList<Stmt>(new ExprStmt(new CallExpr("_EnumWriteStopgap",
                                                   new SymExpr(arg))));
}


bool EnumType::hasDefaultReadFunction(void) {
  return true;
}


AList<Stmt>* EnumType::buildDefaultReadFunctionBody(ArgSymbol* arg) {
  return new AList<Stmt>(new ExprStmt(new CallExpr("_EnumReadStopgap",
                                                   new SymExpr(arg))));
}


UserType::UserType(Type* init_defType, Expr* init_defaultVal) :
  Type(TYPE_USER, init_defaultVal),
  defExpr(NULL),
  defType(init_defType)
{}


UserType::UserType(Expr* init_defExpr, Expr* init_defaultVal) :
  Type(TYPE_USER, init_defaultVal),
  defExpr(init_defExpr),
  defType(dtUnknown)
{}


UserType::UserType(Expr* init_defExpr, Type* init_defType, Expr* init_defaultVal) :
  Type(TYPE_USER, init_defaultVal),
  defExpr(init_defExpr),
  defType(init_defType)
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
UserType::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  UserType* copy = new UserType(COPY_INTERNAL(defExpr),
                                COPY_INTERNAL(defType),
                                COPY_INTERNAL(defaultValue));
  copy->addSymbol(symbol);
  return copy;
}


void UserType::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == defaultValue) {
    defaultValue = dynamic_cast<Expr*>(new_ast);
  } else if (old_ast == defExpr) {
    defExpr = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in Type::replaceChild");
  }
}


void UserType::traverseDefType(Traversal* traversal) {
  TRAVERSE(defExpr, traversal, false);
  TRAVERSE(defType, traversal, false);
  TRAVERSE(defaultValue, traversal, false);
}


void UserType::printDef(FILE* outfile) {
  fprintf(outfile, "type ");
  symbol->print(outfile);
  fprintf(outfile, " = ");
  defExpr->print(outfile);
}


void UserType::codegenDef(FILE* outfile) {
  fprintf(outfile, "typedef ");
  defType->codegen(outfile);
  fprintf(outfile, " ");
  symbol->codegen(outfile);
  fprintf(outfile, ";\n");
}


void UserType::codegenDefaultFormat(FILE* outfile, bool isRead) {
  defType->codegenDefaultFormat(outfile, isRead);
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
    defaultValue = new SymExpr(gNil);
  }
  fields.clear();
  methods.clear();
  types.clear();
  isPattern = false;
  fieldSelector = NULL;
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
ClassType::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  ClassType* copy_type = new ClassType(classTag);
  AList<Stmt>* new_decls = new AList<Stmt>();
  for (Stmt* old_decls = declarationList->first();
       old_decls;
       old_decls = declarationList->next()) {
    ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(old_decls);
    DefExpr* defExpr = exprStmt ? dynamic_cast<DefExpr*>(exprStmt->expr) : NULL;
    if (defExpr && dynamic_cast<FnSymbol*>(defExpr->sym)) {
      copy_type->methods.add(dynamic_cast<FnSymbol*>(defExpr->sym));
    } else {
      new_decls->insertAtTail(CLONE_INTERNAL(old_decls));
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
    defaultValue = dynamic_cast<Expr*>(new_ast);
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
    return is_Scalar_Type(ut->defType);
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
    return is_Value_Type(ut->defType);
  ClassType* ct = dynamic_cast<ClassType*>(t);
  return ct && (ct->classTag == CLASS_RECORD
                || ct->classTag == CLASS_UNION
                || ct->classTag == CLASS_VALUECLASS);
}


int
is_Reference_Type(Type *t) {
  if (UserType *ut = dynamic_cast<UserType*>(t))
    return is_Reference_Type(ut->defType);
  if (!t)
    return false;
  if (t == dtNil || t->astType == TYPE_SUM)
    return true;
  ClassType* ct = dynamic_cast<ClassType*>(t);
  return (ct &&
          (ct->classTag == CLASS_CLASS));
}

static Expr *
init_expr(Type *t) {
  if (t->defaultValue)
    return t->defaultValue->copy();
  else if (t->defaultConstructor)
    return new CallExpr(t->defaultConstructor);
  else
    return new SymExpr(gNil);
}

void ClassType::buildConstructorBody(AList<Stmt>* stmts, Symbol* _this, 
                                          AList<DefExpr>* arguments) {
  if (classTag == CLASS_UNION) {
    AList<Expr>* args = new AList<Expr>(new SymExpr(_this));
    Expr* arg2 = new SymExpr(fieldSelector->constants->first()->sym);
    args->insertAtTail(arg2);
    CallExpr* init_function = new CallExpr(Symboltable::lookupInternal("_UNION_SET"), args);
    ExprStmt* init_stmt = new ExprStmt(init_function);
    stmts->insertAtTail(init_stmt);
    return;
  }

  forv_Vec(Symbol, tmp, fields) {
    if (is_Scalar_Type(tmp->type))
      continue;
    Expr* varInitExpr = init_expr(tmp->type);
    Expr* lhs = new MemberAccess(new SymExpr(_this), tmp);
    Expr* assign_expr = new CallExpr(OP_GETSNORM, lhs, varInitExpr);
    Stmt* assign_stmt = new ExprStmt(assign_expr);
    stmts->insertAtTail(assign_stmt);
  }

  DefExpr* ptmp = arguments->first();
  forv_Vec(TypeSymbol, tmp, types) {
    if (dynamic_cast<VariableType*>(tmp->definition)) {
      if (analyzeAST) {
        // Have type variable in class and type variable in parameter
        // Should I do anything with these?
        ptmp = arguments->next();
      }
    }
  }
  forv_Vec(Symbol, tmp, fields) {
    Expr* lhs = new MemberAccess(new SymExpr(_this), tmp);
    Expr* rhs = NULL;
    if (analyzeAST) {
      rhs = new SymExpr(ptmp->sym);
    } else {
      rhs = tmp->defPoint->init ? tmp->defPoint->init->copy() : new SymExpr(gNil);
      if (tmp->defPoint->init) {
        tmp->defPoint->init->remove();
      }
    }
    if (rhs) {
      Expr* assign_expr = new CallExpr(OP_GETSNORM, lhs, rhs);
      Stmt* assign_stmt = new ExprStmt(assign_expr);
      stmts->insertAtTail(assign_stmt);
    }
    if (analyzeAST) {
      ptmp = arguments->next();
    }
  }
}


void ClassType::codegenStartDefFields(FILE* outfile) {
  if (classTag == CLASS_UNION) {
    fieldSelector->codegen(outfile);
    fprintf(outfile, " _chpl_union_tag;\n");
    fprintf(outfile, "union {\n");
  }
}

void ClassType::codegenStopDefFields(FILE* outfile) {
  if (classTag == CLASS_UNION) {
    fprintf(outfile, "} _chpl_union;\n");
  }
}


void ClassType::codegenDef(FILE* outfile) {
  fprintf(outfile, "struct __");
  symbol->codegen(outfile);
  fprintf(outfile, " {\n");
  codegenStartDefFields(outfile);
  bool printedSomething = false; // BLC: this is to avoid empty structs, illegal in C
  for (Stmt* tmp = declarationList->first(); tmp; tmp = declarationList->next()) {
    if (ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(tmp)) {
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
  codegenStopDefFields(outfile);
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


void ClassType::codegenMemberAccessOp(FILE* outfile) {
  if (classTag == CLASS_UNION) {
    fprintf(outfile, "->_chpl_union.");
  } else {
    fprintf(outfile, "->");
  }
}


bool ClassType::implementedUsingCVals(void) {
  return false;
}


bool ClassType::hasDefaultWriteFunction(void) {
  return true;
}


AList<Stmt>* ClassType::buildDefaultWriteFunctionBody(ArgSymbol* arg) {
  if (classTag == CLASS_UNION) {
    return new AList<Stmt>(new ExprStmt(new CallExpr("_UnionWriteStopgap",
                                                     new SymExpr(arg))));
  }
  AList<Stmt>* body = new AList<Stmt>();
  if (classTag == CLASS_CLASS) {
    AList<Stmt>* writeNil =
      new AList<Stmt>(new ExprStmt(new CallExpr("write", new StringLiteral("nil"))));
    writeNil->insertAtTail(new ReturnStmt(NULL));
    BlockStmt* blockStmt = new BlockStmt(writeNil);
    Symbol* nil = Symboltable::lookupInternal("nil", SCOPE_INTRINSIC);
    Expr* argIsNil = new CallExpr(OP_EQUAL, new SymExpr(arg), new SymExpr(nil));
    body->insertAtTail(new CondStmt(argIsNil, blockStmt));
  }

  if (classTag == CLASS_CLASS || classTag == CLASS_VALUECLASS) {
    addWriteStmt(body, new StringLiteral("{"));
  } else {
    addWriteStmt(body, new StringLiteral("("));
  }

  bool first = true;
  forv_Vec(Symbol, tmp, fields) {
    if (!first) {
      addWriteStmt(body, new StringLiteral(", "));
    }
    addWriteStmt(body, new StringLiteral(tmp->name));
    addWriteStmt(body, new StringLiteral(" = "));
    addWriteStmt(body, new MemberAccess(new SymExpr(arg), tmp));
    first = false;
  }

  if (classTag == CLASS_CLASS || classTag == CLASS_VALUECLASS) {
    addWriteStmt(body, new StringLiteral("}"));
  } else {
    addWriteStmt(body, new StringLiteral(")"));
  }

  return body;
 }


static char* buildFieldSelectorName(ClassType* classType, Symbol* field, 
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
  return glomstrings(4, "_", classType->symbol->name, "_union_id_", fieldName);
}


void ClassType::buildFieldSelector(void) {
  AList<DefExpr>* id_list = new AList<DefExpr>();

  /* build list of enum symbols */
  char* id_name = buildFieldSelectorName(this, NULL);
  EnumSymbol* id_symbol = new EnumSymbol(id_name);
  id_list->insertAtTail(new DefExpr(id_symbol));
  forv_Vec(Symbol, tmp, fields) {
    id_name = buildFieldSelectorName(this, tmp);
    id_symbol = new EnumSymbol(id_name);
    id_list->insertAtTail(new DefExpr(id_symbol));
  }

  /* build enum type */
  EnumType* enum_type = new EnumType(id_list);
  char* enum_name = buildFieldSelectorName(this, NULL, true);
  TypeSymbol* enum_symbol = new TypeSymbol(enum_name, enum_type);
  enum_type->addSymbol(enum_symbol);

  /* build definition of enum */
  DefExpr* def_expr = new DefExpr(enum_symbol);
  symbol->defPoint->parentStmt->insertBefore(new ExprStmt(def_expr));

  fieldSelector = enum_type;
}


CallExpr* ClassType::buildSafeUnionAccessCall(unionCall type, Expr* base, 
                                                 Symbol* field) {
  AList<Expr>* args = new AList<Expr>(base->copy());
  char* id_tag = buildFieldSelectorName(this, field);
  args->insertAtTail(new SymExpr(Symboltable::lookupFromScope(id_tag, structScope)));
  if (type == UNION_CHECK) {
    args->insertAtTail(new StringLiteral(base->filename));
    args->insertAtTail(new IntLiteral(base->lineno));
  }
  
  switch (type) {
  case UNION_SET:
    return new CallExpr(unionSetFn, args);
  case UNION_CHECK:
    return new CallExpr(unionCheckFn, args);
  case UNION_CHECK_QUIET:
    return new CallExpr(unionQuietCheckFn, args);
  default:
    INT_FATAL(this, "Unable to build safe union access call");
  }
  return NULL;
}


MetaType::MetaType(Type* init_base) :
  Type(TYPE_META, NULL),
  base(init_base)
{
  static int uid = 1;
  char* name = glomstrings(2, "_meta_type", intstring(uid++));
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
  Type(TYPE_SUM, new SymExpr(Symboltable::lookupInternal("nil", SCOPE_INTRINSIC)))
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
{}


void VariableType::verify(void) {
  if (astType != TYPE_VARIABLE) {
    INT_FATAL(this, "Bad VariableType::astType");
  }
  if (prev || next) {
    INT_FATAL(this, "Type is in AList");
  }
}


VariableType*
VariableType::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new VariableType(type);
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

  dtBoolean = Symboltable::definePrimitiveType("boolean", "_boolean", new BoolLiteral(false));
  dtInteger = Symboltable::definePrimitiveType("integer", "_integer64", new IntLiteral(0));
  dtFloat = Symboltable::definePrimitiveType("float", "_float64", new FloatLiteral("0.0", 0.0));
  // This should point to the complex type defined in modules/standard/_chpl_complex.chpl
  dtComplex = Symboltable::definePrimitiveType("complex", "_complex128", new FloatLiteral("0.0", 0.0));
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
  char* name = glomstrings(2, "_sum_type", intstring(uid++));
  SymScope* saveScope = Symboltable::setCurrentScope(commonModule->modScope);
  TypeSymbol* sym = new TypeSymbol(name, new_sum_type);
  Symboltable::define(sym);
  new_sum_type->addSymbol(sym);
  Symboltable::setCurrentScope(saveScope);
  return new_sum_type;
}


Type *getMetaType(Type *t) {
  if (t)
    return t->metaType;
  else
    return dtAny->metaType;
}

void findInternalTypes(void) {
  dtObject = dynamic_cast<ClassType*>(Symboltable::lookupInternalType("object")->definition);
  dtValue = dynamic_cast<ClassType*>(Symboltable::lookupInternalType("value")->definition);

  // These should all be eliminated.  Note they almost are since they
  // are MetaTypes, not the types in the prelude.
  dtTuple = Symboltable::lookupInternalType("Tuple")->type;
  dtIndex = Symboltable::lookupInternalType("Index")->type;
  dtDomain = Symboltable::lookupInternalType("Domain")->type;
  dtArray = Symboltable::lookupInternalType("Array")->type;
  dtSequence = Symboltable::lookupInternalType("_seq")->type;

  // SJD: Can't do this when dtString is defined because
  // prelude hasn't been made yet.  Need to do it after.
  dtString->defaultConstructor =
    dynamic_cast<FnSymbol*>(Symboltable::lookupInternal("_init_string"));
  initConfigFn =
    dynamic_cast<FnSymbol*>(Symboltable::lookupInternal("_INIT_CONFIG"));
  unionSetFn =
    dynamic_cast<FnSymbol*>(Symboltable::lookupInternal("_UNION_SET"));
  unionCheckFn =
    dynamic_cast<FnSymbol*>(Symboltable::lookupInternal("_UNION_CHECK"));
  unionQuietCheckFn =
    dynamic_cast<FnSymbol*>(Symboltable::lookupInternal("_UNION_CHECK_QUIET"));

}
