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
  metaType(NULL),
  isGeneric(false),
  instantiatedFrom(NULL)
{ 
  if (astType != TYPE_META)
    metaType = new MetaType(this);
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
  if (old_ast == defaultVal) {
    defaultVal = dynamic_cast<Expr*>(new_ast);
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


EnumType::EnumType(AList<DefExpr>* init_constants) :
  Type(TYPE_ENUM, new Variable(init_constants->first()->sym)),
  constants(init_constants)
{ }


EnumType*
EnumType::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  EnumType* copy = new EnumType(CLONE_INTERNAL(constants));
  copy->addSymbol(symbol);
  return copy;
}


void EnumType::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == defaultVal) {
    defaultVal = dynamic_cast<Expr*>(new_ast);
  } else if (old_ast == constants) {
    constants = dynamic_cast<AList<DefExpr>*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in Type::replaceChild");
  }
}


void EnumType::traverseDefType(Traversal* traversal) {
  TRAVERSE(constants, traversal, false);
  TRAVERSE(defaultVal, traversal, false);
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


AList<Stmt>* EnumType::buildDefaultWriteFunctionBody(ParamSymbol* arg) {
  return new AList<Stmt>(new ExprStmt(new ParenOpExpr(new Variable(Symboltable::lookupInternal("_EnumWriteStopgap")), new AList<Expr>(new Variable(arg)))));
}


bool EnumType::hasDefaultReadFunction(void) {
  return true;
}


AList<Stmt>* EnumType::buildDefaultReadFunctionBody(ParamSymbol* arg) {
  return new AList<Stmt>(new ExprStmt(new ParenOpExpr(new Variable(Symboltable::lookupInternal("_EnumReadStopgap")), new AList<Expr>(new Variable(arg)))));
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


UserType*
UserType::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  UserType* copy = new UserType(COPY_INTERNAL(defExpr),
                                COPY_INTERNAL(defType),
                                COPY_INTERNAL(defaultVal));
  copy->addSymbol(symbol);
  return copy;
}


void UserType::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == defaultVal) {
    defaultVal = dynamic_cast<Expr*>(new_ast);
  } else if (old_ast == defExpr) {
    defExpr = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in Type::replaceChild");
  }
}


void UserType::traverseDefType(Traversal* traversal) {
  TRAVERSE(defExpr, traversal, false);
  TRAVERSE(defType, traversal, false);
  TRAVERSE(defaultVal, traversal, false);
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


Type* UserType::getType(void) {
  return defType;
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
    ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(old_decls);
    DefExpr* defExpr = exprStmt ? dynamic_cast<DefExpr*>(exprStmt->expr) : NULL;
    if (defExpr && dynamic_cast<FnSymbol*>(defExpr->sym)) {
      copy_type->methods.add(dynamic_cast<FnSymbol*>(defExpr->sym));
    } else {
      new_decls->insertAtTail(CLONE_INTERNAL(old_decls));
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
    ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(tmp);
    DefExpr* defExpr = exprStmt ? dynamic_cast<DefExpr*>(exprStmt->expr) : NULL;
    if (defExpr) {
      if (FnSymbol* sym = dynamic_cast<FnSymbol*>(defExpr->sym)) {
        sym->typeBinding = this->symbol;
        if (sym->fnClass != FN_CONSTRUCTOR) {
          sym->method_type = PRIMARY_METHOD;
        }
        methods.add(sym);
      } else if (TypeSymbol* sym = dynamic_cast<TypeSymbol*>(defExpr->sym)) {
        types.add(sym);
      } else if (VarSymbol* sym = dynamic_cast<VarSymbol*>(defExpr->sym)) {
        fields.add(sym);
      } else if (ParamSymbol* sym = dynamic_cast<ParamSymbol*>(defExpr->sym)) {
        fields.add(sym);
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
    INT_FATAL(this, "Unexpected case in Type::replaceChild");
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
  return t && (dynamic_cast<StructuralType*>(t) || t->astType == TYPE_SUM);
}


void StructuralType::buildConstructorBody(AList<Stmt>* stmts, Symbol* _this, 
                                          AList<DefExpr>* arguments) {
  forv_Vec(Symbol, tmp, fields) {
    if (is_Scalar_Type(tmp->type))
      continue;
    Expr* lhs = new MemberAccess(new Variable(_this), tmp);
    Expr* varInitExpr = new VarInitExpr(new MemberAccess(new Variable(_this), tmp));
    Expr* assign_expr = new AssignOp(GETS_NORM, lhs, varInitExpr);
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
    Expr* lhs = new MemberAccess(new Variable(_this), tmp);
    Expr* rhs = NULL;
    if (analyzeAST) {
      rhs = new Variable(ptmp->sym);
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


void StructuralType::codegenStructName(FILE* outfile) {
  fprintf(outfile, "_");
  symbol->codegen(outfile);
  fprintf(outfile,", *");
  symbol->codegen(outfile);
}


void StructuralType::codegenPrototype(FILE* outfile) {
  fprintf(outfile, "typedef struct __");
  symbol->codegen(outfile);
  fprintf(outfile, " ");
  codegenStructName(outfile);
  fprintf(outfile, ";\n");
}


void StructuralType::codegenMemberAccessOp(FILE* outfile) {
  fprintf(outfile, "->");
}


bool StructuralType::blankIntentImpliesRef(void) {
  return false;
}


bool StructuralType::implementedUsingCVals(void) {
  return false;
}


bool StructuralType::hasDefaultWriteFunction(void) {
  return true;
}


AList<Stmt>* StructuralType::buildDefaultWriteFunctionBody(ParamSymbol* arg) {
  AList<Stmt>* body = new AList<Stmt>();
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
  forv_Vec(Symbol, tmp, fields) {
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


ClassType*
ClassType::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
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
  StructuralType(TYPE_RECORD), isPattern(false)
{}


RecordType*
RecordType::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  RecordType* copy_type = new RecordType();
  copyGuts(copy_type, clone, map);
  copy_type->isPattern = isPattern;
  return copy_type;
}


UnionType::UnionType(void) :
  StructuralType(TYPE_UNION),
  fieldSelector(NULL)
{}


UnionType*
UnionType::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
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
                                     AList<DefExpr>* arguments) {
  AList<Expr>* args = new AList<Expr>(new Variable(_this));
  Expr* arg2 = new Variable(fieldSelector->constants->first()->sym);
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
  fprintf(outfile, "->_chpl_union.");
}


bool UnionType::hasDefaultWriteFunction(void) {
  return true;
}


AList<Stmt>* UnionType::buildDefaultWriteFunctionBody(ParamSymbol* arg) {
  return new AList<Stmt>(new ExprStmt(new ParenOpExpr(new Variable(Symboltable::lookupInternal("_UnionWriteStopgap")), new AList<Expr>(new Variable(arg)))));
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
  fprintf(outfile, ";\n");
}

void SumType::codegenStructName(FILE* outfile) {
  symbol->codegen(outfile);
}

VariableType::VariableType(Type *init_type) :
  Type(TYPE_VARIABLE, NULL), 
  type(init_type)
{}


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
  TypeSymbol* sym = new TypeSymbol(name, new_sum_type);
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
    return t->metaType;
  else
    return dtAny->metaType;
}

void findInternalTypes(void) {
  dtTuple = Symboltable::lookupInternalType("Tuple")->type;
  dtIndex = Symboltable::lookupInternalType("Index")->type;
  dtDomain = Symboltable::lookupInternalType("Domain")->type;
  dtArray = Symboltable::lookupInternalType("Array")->type;
}
