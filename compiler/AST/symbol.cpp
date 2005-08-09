#include <typeinfo>
#include "analysis.h"
#include "files.h"
#include "misc.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "symtab.h"
#include "sym.h"
#include "fun.h"
#include "../traversals/buildClassConstructorsEtc.h"
#include "../traversals/clearTypes.h"
#include "../traversals/updateSymbols.h"
#include "../symtab/collectFunctions.h"
#include "../traversals/findTypeVariables.h"
#include "../passes/preAnalysisCleanup.h"

Symbol *gNil = 0;
Symbol *gUnspecified = 0;

#define DUPLICATE_INSTANTIATION_CACHE 1

Symbol::Symbol(astType_t astType, char* init_name, Type* init_type, 
               bool init_exportMe) :
  BaseAST(astType),
  name(init_name),
  cname(name),
  type(init_type),
  exportMe(init_exportMe),
  isDead(false),
  keepLive(false),
  defPoint(NULL),
  asymbol(0),
  overload(NULL),
  isUnresolved(false)
{}


void Symbol::verify(void) {
  INT_FATAL(this, "Symbol::verify() should never be called");
}


void Symbol::setParentScope(SymScope* init_parentScope) {
  parentScope = init_parentScope;
}


Symbol*
Symbol::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  INT_FATAL(this, "Illegal call to Symbol::copy");
  return NULL;
}


void Symbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  INT_FATAL(this, "Unexpected call to Symbol::replaceChild");
}


void Symbol::traverse(Traversal* traversal, bool atTop) {
  SymScope* saveScope = NULL;
  if (atTop) {
    saveScope = Symboltable::setCurrentScope(parentScope);
  }
  if (traversal->processTop || !atTop) {
    currentLineno = lineno;
    currentFilename = filename;
    traversal->preProcessSymbol(this);
  }
  if (atTop || traversal->exploreChildSymbols) {
    if (atTop) {
      traverseDefSymbol(traversal);
    }
    else {
      traverseSymbol(traversal);
    }
  }
  if (traversal->processTop || !atTop) {
    currentLineno = lineno;
    currentFilename = filename;
    traversal->postProcessSymbol(this);
  }
  if (atTop) {
    Symboltable::setCurrentScope(saveScope);
  }
}


void Symbol::traverseDef(Traversal* traversal, bool atTop) {
  SymScope* saveScope = NULL;
  if (atTop) {
    saveScope = Symboltable::setCurrentScope(parentScope);
  }
  if (traversal->processTop || !atTop) {
    currentLineno = lineno;
    currentFilename = filename;
    traversal->preProcessSymbol(this);
  }
  traverseDefSymbol(traversal);
  if (traversal->processTop || !atTop) {
    currentLineno = lineno;
    currentFilename = filename;
    traversal->postProcessSymbol(this);
  }
  if (atTop) {
    Symboltable::setCurrentScope(saveScope);
  }
}


void Symbol::traverseSymbol(Traversal* traversal) {
  
}


void Symbol::traverseDefSymbol(Traversal* traversal) {
}


bool Symbol::isConst(void) {
  return true;
}

//Roxana: not all symbols are parameter symbols
bool Symbol::isParam(){
        return false;
}

void Symbol::print(FILE* outfile) {
  fprintf(outfile, "%s", name);
}

bool Symbol::lessThan(Symbol* s1, Symbol* s2) { 
    return strcmp(s1->name, s2->name) < 0;
}    

bool Symbol::equalWith(Symbol* s1, Symbol* s2) { 
    return strcmp(s1->name, s2->name) == 0;
}    

bool Symbol::greaterThan(Symbol* s1, Symbol* s2) { 
    return strcmp(s1->name, s2->name) > 0;
}    

void Symbol::codegen(FILE* outfile) {
  if (hasPragma("codegen data")) {
    TypeSymbol* typeSymbol = dynamic_cast<TypeSymbol*>(this);
    ClassType* dataType = dynamic_cast<ClassType*>(typeSymbol->definition);
    dataType->methods.v[0]->retType->codegen(outfile);
    fprintf(outfile, "*", cname);
  } else {
    fprintf(outfile, "%s", cname);
  }
}


void Symbol::codegenDef(FILE* outfile) {
  INT_FATAL(this, "Unanticipated call to Symbol::codegenDef");
}


void Symbol::codegenPrototype(FILE* outfile) { }


void Symbol::printDef(FILE* outfile) {
  print(outfile);
}


FnSymbol* Symbol::getFnSymbol(void) {
  return NULL;
}


Symbol* Symbol::getSymbol(void) {
  return this;
}


Type* Symbol::typeInfo(void) {
  return type;
}


UnresolvedSymbol::UnresolvedSymbol(char* init_name, char* init_cname) :
  Symbol(SYMBOL_UNRESOLVED, init_name)
{
  if (init_cname) {
    cname = init_cname;
  }
  isUnresolved = true;
}


void UnresolvedSymbol::verify(void) {
  if (astType != SYMBOL_UNRESOLVED) {
    INT_FATAL(this, "Bad UnresolvedSymbol::astType");
  }
}


void UnresolvedSymbol::codegen(FILE* outfile) {
  INT_FATAL(this, "ERROR:  Cannot codegen an unresolved symbol.");
  fprintf(outfile, "%s /* unresolved */ ", name);
}


UnresolvedSymbol*
UnresolvedSymbol::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new UnresolvedSymbol(copystring(name));
}


void UnresolvedSymbol::traverseDefSymbol(Traversal* traversal) {
  TRAVERSE(this, traversal, false);
}


VarSymbol::VarSymbol(char* init_name,
                     Type* init_type,
                     varType init_varClass, 
                     consType init_consClass) :
  Symbol(SYMBOL_VAR, init_name, init_type),
  varClass(init_varClass),
  consClass(init_consClass),
  noDefaultInit(false)
{
  if (name) { // ensure this is not a sentinel
#ifdef NUMBER_VAR_SYMBOLS_UNIQUELY
    static int uid = 0;
    cname = glomstrings(4, name, "__", intstring(uid++), "__");
#endif
    /** SJD hack because __init_fn is not set up with a scope **/
    if (Symbol* init_fn = Symboltable::getCurrentScope()->symContext) {
      if (!strncmp("__init_", init_fn->name, 7)) {
        Symboltable::defineInScope(this, Symboltable::getCurrentScope()->parent);
      } else {
        Symboltable::define(this);
      }
    } else {
      Symboltable::define(this);
    }
  }
}


void VarSymbol::verify(void) {
  if (astType != SYMBOL_VAR) {
    INT_FATAL(this, "Bad VarSymbol::astType");
  }
}


VarSymbol*
VarSymbol::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  VarSymbol* newVarSymbol = 
    new VarSymbol(copystring(name), type, varClass, consClass);
  newVarSymbol->cname = copystring(cname);
  newVarSymbol->noDefaultInit = noDefaultInit;
  return newVarSymbol;
}


void VarSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  type->replaceChild(old_ast, new_ast);
}


void VarSymbol::traverseDefSymbol(Traversal* traversal) {
  TRAVERSE(type, traversal, false);
}


void VarSymbol::printDef(FILE* outfile) {
  if (varClass == VAR_CONFIG) {
    fprintf(outfile, "config ");
  }
  if (varClass == VAR_STATE) {
    fprintf(outfile, "state ");
  }
  //Roxana -- introduced various types of constness: const, param, nothing (var)
  if (consClass == VAR_CONST) {
    fprintf(outfile, "const ");
  } else if (consClass == VAR_PARAM){
        fprintf(outfile, "param");
  }
  else {
    fprintf(outfile, "var ");
  }
  print(outfile);
  fprintf(outfile, ": ");
  type->print(outfile);
}


bool VarSymbol::initializable(void) {
  switch (parentScope->type) {
  case SCOPE_FUNCTION:
  case SCOPE_LOCAL:
  case SCOPE_MODULE:
    return true;
  case SCOPE_INTRINSIC:
  case SCOPE_PRELUDE:
  case SCOPE_POSTPARSE:
  case SCOPE_PARAM:
  case SCOPE_FORLOOP:
  case SCOPE_FORALLEXPR:
  case SCOPE_CLASS:
    return false;
  default:
    INT_FATAL(this, "unhandled case in needsTypeInitialization()");
  }
  return false;
}

//Roxana
bool VarSymbol::isConst(void) {
  return (consClass == VAR_CONST);
}

//Roxana
bool VarSymbol::isParam(void){
  return (consClass == VAR_PARAM);
}

bool Symbol::isThis(void) {
  FnSymbol *f = dynamic_cast<FnSymbol*>(defPoint->parentSymbol);
  if (!f || !f->_this)
    return 0;
  else
    return f->_this == this;
}
 
void VarSymbol::codegenDef(FILE* outfile) {
  // need to ensure that this can be realized in C as a const, and
  // move its initializer here if it can be
  if (0 && (consClass == VAR_CONST)) {
    fprintf(outfile, "const ");
  }
  type->codegen(outfile);
  if (varClass == VAR_REF)
    fprintf(outfile, "*");
  fprintf(outfile, " ");
  this->codegen(outfile);
  if (this->initializable() && varClass != VAR_REF) {
    type->codegenSafeInit(outfile);
  }
  fprintf(outfile, ";\n");
}


static char* paramTypeNames[NUM_PARAM_TYPES] = {
  "",
  "in",
  "inout",
  "out",
  "const",
  "ref"
};


ParamSymbol::ParamSymbol(paramType init_intent, char* init_name, 
                         Type* init_type) :
  Symbol(SYMBOL_PARAM, init_name, init_type),
  intent(init_intent),
  typeVariable(NULL),
  isGeneric(false)
{
  if (name) {  // ensure this is not a sentinel
    Symboltable::define(this);
  }
  if (intent == PARAM_PARAMETER || intent == PARAM_TYPE)
    isGeneric = true;
}


void ParamSymbol::verify(void) {
  if (astType != SYMBOL_PARAM) {
    INT_FATAL(this, "Bad ParamSymbol::astType");
  }
}


ParamSymbol*
ParamSymbol::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  ParamSymbol *ps = new ParamSymbol(intent, copystring(name), type);
  if (typeVariable)
    ps->typeVariable = typeVariable;
  ps->isGeneric = isGeneric;
  ps->cname = copystring(cname);
  return ps;
}


void ParamSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  type->replaceChild(old_ast, new_ast);
}


void ParamSymbol::traverseDefSymbol(Traversal* traversal) {
  TRAVERSE(type, traversal, false);
  TRAVERSE(typeVariable, traversal, false);
}


void ParamSymbol::printDef(FILE* outfile) {
  fprintf(outfile, "%s ", paramTypeNames[intent]);
  Symbol::print(outfile);
  fprintf(outfile, ": ");
  type->print(outfile);
}


bool ParamSymbol::requiresCPtr(void) {
  return (intent == PARAM_OUT || intent == PARAM_INOUT || 
          (intent == PARAM_REF && type->astType == TYPE_PRIMITIVE));
}


bool ParamSymbol::requiresCopyBack(void) {
  return intent == PARAM_OUT || intent == PARAM_INOUT;
}


bool ParamSymbol::requiresCTmp(void) {
  return type->requiresCParamTmp(intent);
}


bool ParamSymbol::isConst(void) {
  // TODO: need to also handle case of PARAM_BLANK for scalar types
  return (intent == PARAM_CONST); 
}


void ParamSymbol::codegen(FILE* outfile) {
  bool requiresDeref = requiresCPtr();
 
  if (requiresDeref) {
    fprintf(outfile, "(*");
  }
  Symbol::codegen(outfile);
  if (requiresDeref) {
    fprintf(outfile, ")");
  }
}


void ParamSymbol::codegenDef(FILE* outfile) {
  type->codegen(outfile);
  if (requiresCPtr()) {
    fprintf(outfile, "* const");
  }
  fprintf(outfile, " ");
  Symbol::codegen(outfile);
}

TypeSymbol::TypeSymbol(char* init_name, Type* init_definition) :
  Symbol(SYMBOL_TYPE, init_name, init_definition ? init_definition->metaType : NULL), 
  definition(init_definition)
{
  if (definition)
    Symboltable::define(this);
  else
    isUnresolved = true;
}


void TypeSymbol::verify(void) {
  if (astType != SYMBOL_TYPE) {
    INT_FATAL(this, "Bad TypeSymbol::astType");
  }

  definition->verify();
}


TypeSymbol*
TypeSymbol::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  Type* new_definition = COPY_INTERNAL(definition);
  TypeSymbol* new_definition_symbol = new TypeSymbol(copystring(name), new_definition);
  new_definition->addSymbol(new_definition_symbol);
  if (ClassType* stype =
      dynamic_cast<ClassType*>(new_definition)) {
    stype->structScope->setContext(NULL, new_definition_symbol);
  }
  new_definition_symbol->cname = copystring(cname);
  return new_definition_symbol;
}


TypeSymbol* TypeSymbol::clone(Map<BaseAST*,BaseAST*>* map) {
  static int uid = 1; // Unique ID for cloned classes
  ClassType* old_class_type = dynamic_cast<ClassType*>(definition);
  if (!old_class_type) {
    INT_FATAL(this, "Attempt to clone non-class type");
  }
  SymScope* save_scope = Symboltable::setCurrentScope(parentScope);
  TypeSymbol* clone = copy(true, map);
  if (ClassType* newClassType = dynamic_cast<ClassType*>(clone->definition)) {
    ClassType* oldClassType = dynamic_cast<ClassType*>(definition);
    if (!oldClassType) {
      INT_FATAL(this, "Cloning of ClassType went horribly wrong");
    }
    newClassType->typeParents.add(oldClassType);
  }
  clone->cname = glomstrings(3, clone->cname, "_clone_", intstring(uid++));
  DefExpr* new_def_expr = new DefExpr(clone);
  ClassType* new_class_type = dynamic_cast<ClassType*>(clone->definition);
  new_class_type->structScope->setContext(NULL, clone, new_def_expr);
  defPoint->parentStmt->insertBefore(new ExprStmt(new_def_expr));
  Symboltable::setCurrentScope(save_scope);
  return clone;
}


void TypeSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == type)
    type = dynamic_cast<Type*>(new_ast);
  else
    definition->replaceChild(old_ast, new_ast);
}


void TypeSymbol::traverseDefSymbol(Traversal* traversal) {
  TRAVERSE(type, traversal, false);
  TRAVERSE_DEF(definition, traversal, false);
}


void TypeSymbol::codegenPrototype(FILE* outfile) {
  if (isDead) {
    return;
  }
  definition->codegenPrototype(outfile);
}


void TypeSymbol::codegenDef(FILE* outfile) {
  if (isDead) {
    return;
  }

  definition->codegenDef(outfile);
}


FnSymbol::FnSymbol(char* init_name, TypeSymbol* init_typeBinding) :
  Symbol(SYMBOL_FN, init_name, new FnType()),
  formals(NULL),
  retType(NULL),
  _this(NULL),
  _setter(NULL),
  _getter(NULL),
  body(NULL),
  typeBinding(init_typeBinding),
  fnClass(FN_FUNCTION),
  whereExpr(NULL),
  noparens(false),
  isGeneric(false),
  instantiatedFrom(NULL)
{
  Symboltable::define(this);
  method_type = NON_METHOD;
}


void FnSymbol::verify(void) {
  if (astType != SYMBOL_FN) {
    INT_FATAL(this, "Bad FnSymbol::astType");
  }
}


void FnSymbol::continueDef(AList<DefExpr>* init_formals, Type* init_retType, bool isRef, Expr *init_whereExpr) {
  formals = init_formals;
  retType = init_retType;
  retRef = isRef;
  whereExpr = init_whereExpr;
}


void FnSymbol::finishDef(BlockStmt* init_body, SymScope* init_paramScope,
                         bool init_exportMe) {
  body = init_body;
  exportMe = init_exportMe;
  paramScope = init_paramScope;

  if (strcmp(name, "main") == 0 && 
      (parentScope->type == SCOPE_MODULE || 
       parentScope->type == SCOPE_POSTPARSE) &&
      formals->isEmpty()) {
    if (!mainFn) {
      mainFn = this;
      exportMe = true;
      cname = copystring("_chpl_main");
    } else {
      USR_FATAL(this, "main multiply defined -- first occurrence at %s",
                mainFn->stringLoc());
    }
  }
}


FnSymbol* FnSymbol::getFnSymbol(void) {
  return this;
}


FnSymbol*
FnSymbol::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  char* copy_name = copystring(name);
  FnSymbol* copy = new FnSymbol(copy_name, typeBinding);
  copy->method_type = method_type;
  copy->fnClass = fnClass;
  Symboltable::startFnDef(copy);
  copy->_getter = _getter; // If it is a cloned class we probably want this
  copy->_setter = _setter; //  to point to the new member, but how do we do that
  copy->_this = _this;
  copy->isGeneric = isGeneric;
  AList<DefExpr>* new_formals =
    dynamic_cast<AList<DefExpr>*>(CLONE_INTERNAL(formals));
  Symboltable::continueFnDef(copy, new_formals, retType, retRef,
                             CLONE_INTERNAL(whereExpr));
  BlockStmt* new_body = 
    dynamic_cast<BlockStmt*>(CLONE_INTERNAL(body));
  if (body != NULL && new_body == NULL) {
    INT_FATAL(body, "function body was not a BlockStmt!?");
  }
  copy->cname = copystring(cname);
  return Symboltable::finishFnDef(copy, new_body, exportMe);
}


void FnSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == body) {
    body = dynamic_cast<BlockStmt*>(new_ast);
  } else if (old_ast == formals) {
    formals = dynamic_cast<AList<DefExpr>*>(new_ast);
  } else if (old_ast == whereExpr) {
    whereExpr = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in FnSymbol::replaceChild");
  }
}


void FnSymbol::traverseDefSymbol(Traversal* traversal) {
  SymScope* saveScope = NULL;

  if (paramScope) {
    saveScope = Symboltable::setCurrentScope(paramScope);
  }
  TRAVERSE(formals, traversal, false);
  TRAVERSE(type, traversal, false);
  TRAVERSE(body, traversal, false);
  TRAVERSE(retType, traversal, false);
  TRAVERSE(whereExpr, traversal, false);
  if (paramScope) {
    Symboltable::setCurrentScope(saveScope);
  }
}


FnSymbol* FnSymbol::clone(Map<BaseAST*,BaseAST*>* map) {
  static int uid = 1; // Unique ID for cloned functions
  SymScope* save_scope = Symboltable::setCurrentScope(parentScope);
  Stmt* copyStmt = defPoint->parentStmt->copy(true, map, NULL);
  ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(copyStmt);
  DefExpr* defExpr = dynamic_cast<DefExpr*>(exprStmt->expr);
  defExpr->sym->cname =
    glomstrings(3, cname, "_clone_", intstring(uid++));
  defPoint->parentStmt->insertAfter(copyStmt);
  Symboltable::setCurrentScope(save_scope);
  TRAVERSE(copyStmt, new ClearTypes(), true);
  TRAVERSE(defPoint, new ClearTypes(), true); // only do this until
                                              // uncloned is not used
  return dynamic_cast<FnSymbol*>(defExpr->sym);
}


FnSymbol* FnSymbol::coercion_wrapper(Map<Symbol*,Symbol*>* coercion_substitutions) {
  static int uid = 1; // Unique ID for wrapped functions
  currentLineno = lineno;
  currentFilename = filename;
  SymScope* saveScope = Symboltable::setCurrentScope(parentScope);
  FnSymbol* wrapperFn = new FnSymbol(name);
  wrapperFn->cname = glomstrings(3, cname, "_coercion_wrapper_", intstring(uid++));
  wrapperFn = Symboltable::startFnDef(wrapperFn);
  wrapperFn->method_type = method_type;
  wrapperFn->fnClass = fnClass;
  wrapperFn->addPragma("inline");

  AList<DefExpr>* wrapperFormals = new AList<DefExpr>();
  for (DefExpr* formal = formals->first(); formal; formal = formals->next()) {
    ParamSymbol* newFormal = dynamic_cast<ParamSymbol*>(formal->sym->copy());
    wrapperFormals->insertAtTail(new DefExpr(newFormal));
    Symbol* coercionSubstitution = coercion_substitutions->get(formal->sym);
    if (TypeSymbol *ts = dynamic_cast<TypeSymbol*>(coercionSubstitution)) {
      newFormal->type = ts->definition;
    }
  }
  Symboltable::continueFnDef(wrapperFn, wrapperFormals, retType, retRef);

  BlockStmt* wrapperBlock = Symboltable::startCompoundStmt();

  AList<Stmt>* wrapperBody = new AList<Stmt>();
  AList<Expr>* wrapperActuals = new AList<Expr>();
  for (DefExpr* formal = formals->first(), *wrapperFormal = wrapperFormals->first(); 
       formal; 
       formal = formals->next(), wrapperFormal = wrapperFormals->next()) {
    Symbol* coercionSubstitution = coercion_substitutions->get(formal->sym);
    if (coercionSubstitution) {
      char* tempName = glomstrings(2, "_coercion_temp_", formal->sym->name);
      VarSymbol* temp = new VarSymbol(tempName, formal->sym->type);
      DefExpr* tempDefExpr = new DefExpr(temp, new Variable(wrapperFormal->sym));
      wrapperBody->insertAtTail(new ExprStmt(tempDefExpr));
      wrapperActuals->insertAtTail(new Variable(temp));
    } else {
      wrapperActuals->insertAtTail(new Variable(wrapperFormal->sym));
    }
  }

  Expr* fn_call = new CallExpr(this, wrapperActuals);
  if (function_returns_void(this)) {
    wrapperBody->insertAtTail(new ExprStmt(fn_call));
  } else {
    wrapperBody->insertAtTail(new ReturnStmt(fn_call));
  }
  wrapperBlock = Symboltable::finishCompoundStmt(wrapperBlock, wrapperBody);
  DefExpr* defExpr = new DefExpr(Symboltable::finishFnDef(wrapperFn, wrapperBlock));
  defPoint->parentStmt->insertBefore(new ExprStmt(defExpr));
  Symboltable::setCurrentScope(saveScope);
  return wrapperFn;
}


FnSymbol* FnSymbol::default_wrapper(Vec<Symbol*>* defaults) {
  static int uid = 1; // Unique ID for wrapped functions
  currentLineno = lineno;
  currentFilename = filename;
  SymScope* save_scope = Symboltable::setCurrentScope(parentScope);
  FnSymbol* wrapper_fn = Symboltable::startFnDef(new FnSymbol(name));
  wrapper_fn->addPragma("inline");
  wrapper_fn->method_type = method_type;
  wrapper_fn->fnClass = fnClass;
  wrapper_fn->cname =
    glomstrings(3, cname, "_default_params_wrapper_", intstring(uid++));
  AList<DefExpr>* wrapper_formals = new AList<DefExpr>();
  for_alist(DefExpr, formal, formals) {
    if (!defaults->set_in(formal->sym)) {
      wrapper_formals->insertAtTail(formal->copy(true));
    }
  }
  Symboltable::continueFnDef(wrapper_fn, wrapper_formals, retType, retRef, COPY(whereExpr));
  AList<Stmt>* wrapper_body = new AList<Stmt>();
  Vec<Expr*> temps;
  for_alist_backward(DefExpr, formal, formals) {
    if (defaults->set_in(formal->sym)) {
      char* temp_name = glomstrings(2, "_default_param_temp_", formal->sym->name);
      VarSymbol* temp_symbol = new VarSymbol(temp_name, formal->sym->type);
      // mark default parameters as being of the nominal type of the corresponding
      // non-default parameter.
      DefExpr* temp_def_expr =
        new DefExpr(temp_symbol,
                    (dynamic_cast<ParamSymbol*>(formal->sym)->intent == PARAM_OUT)
                    ? NULL
                    : formal->sym->defPoint->init->copy(),
                    (formal->sym->defPoint->exprType)
                    ? formal->sym->defPoint->exprType->copy()
                    : NULL);
      wrapper_body->insertAtHead(new ExprStmt(temp_def_expr));
      ParamSymbol *ps = dynamic_cast<ParamSymbol*>(formal->sym);
      if (formal->sym->type != dtUnknown &&
          ps->intent != PARAM_OUT && ps->intent != PARAM_INOUT)
        temps.add(new CastExpr(new Variable(temp_symbol), NULL, formal->sym->type));
      else
        temps.add(new Variable(temp_symbol));
    }
  }
  AList<Expr>* actuals = new AList<Expr>();
  DefExpr* wrapper_formal = wrapper_formals->first();
  for_alist(DefExpr, formal, formals) {
    if (defaults->set_in(formal->sym)) {
      actuals->insertAtTail(temps.pop());
    } else {
      actuals->insertAtTail(new Variable(wrapper_formal->sym));
      wrapper_formal = wrapper_formals->next();
    }
  }
  CallExpr* fn_call = new CallExpr(this, actuals);
  if (function_returns_void(this)) {
    wrapper_body->insertAtTail(new ExprStmt(fn_call));
  } else {
    wrapper_body->insertAtTail(new ReturnStmt(fn_call));
  }
  Symboltable::finishFnDef(wrapper_fn, new BlockStmt(wrapper_body));
  DefExpr* wrapper_expr = new DefExpr(wrapper_fn);
  ExprStmt* wrapper_stmt = new ExprStmt(wrapper_expr);
  wrapper_stmt->copyPragmas(defPoint->parentStmt->pragmas);
  defPoint->parentStmt->insertAfter(wrapper_stmt);
  Symboltable::setCurrentScope(save_scope);
  return dynamic_cast<FnSymbol*>(wrapper_expr->sym);
}


FnSymbol* FnSymbol::order_wrapper(Map<Symbol*,Symbol*>* formals_to_actuals) {
  static int uid = 1; // Unique ID for wrapped functions
  currentLineno = lineno;
  currentFilename = filename;
  SymScope* save_scope = Symboltable::setCurrentScope(parentScope);
  FnSymbol* wrapper_fn = new FnSymbol(name);
  wrapper_fn->addPragma("inline");
  wrapper_fn->cname = glomstrings(3, cname, "_ord_wrapper_", intstring(uid++));
  wrapper_fn = Symboltable::startFnDef(wrapper_fn);
  wrapper_fn->method_type = method_type;
  wrapper_fn->fnClass = fnClass;

  AList<DefExpr>* wrapper_formals = new AList<DefExpr>();
  for (int i = 0; i < formals_to_actuals->n; i++) {
    DefExpr* tmp = formals->first();
    for (int j = 0; j < formals_to_actuals->n; j++) {
      if (formals_to_actuals->v[i].key == formals_to_actuals->v[j].value) {
        wrapper_formals->insertAtTail(new DefExpr(tmp->sym->copy()));
      }
      tmp = formals->next();
    }
  }
  Symboltable::continueFnDef(wrapper_fn, wrapper_formals, retType, retRef);

  AList<Expr>* actuals = new AList<Expr>();
  for (int i = 0; i < formals_to_actuals->n; i++) {
    DefExpr* tmp = wrapper_formals->first();
    for (int j = 0; j < formals_to_actuals->n; j++) {
      if (formals_to_actuals->v[i].value == formals_to_actuals->v[j].key) {
        actuals->insertAtTail(new Variable(tmp->sym));
      }
      tmp = wrapper_formals->next();
    }
  }

  Expr* fn_call = new CallExpr(this, actuals);
  AList<Stmt>* body = new AList<Stmt>();
  if (function_returns_void(this)) {
    body->insertAtTail(new ExprStmt(fn_call));
  } else {
    body->insertAtTail(new ReturnStmt(fn_call));
  }
  DefExpr* def_expr = new DefExpr(Symboltable::finishFnDef(wrapper_fn, new BlockStmt(body)));
  defPoint->parentStmt->insertBefore(new ExprStmt(def_expr));
  Symboltable::setCurrentScope(save_scope);
  return wrapper_fn;
}


static bool
instantiate_update_expr(Map<BaseAST*,BaseAST*>* substitutions, Expr* expr,
                        Map<BaseAST*,BaseAST*>* copyMap) {
  Map<BaseAST *, BaseAST *> map;
  map.copy(*substitutions);
  // for type variables, add TypeSymbols into the map as well
  for (int i = 0; i < substitutions->n; i++)
    if (Type *t = dynamic_cast<Type*>(substitutions->v[i].key))
      if (Type *tt = dynamic_cast<Type*>(substitutions->v[i].value))
        map.put(t->symbol, tt->symbol);
  UpdateSymbols *updater = new UpdateSymbols(&map, copyMap);
  TRAVERSE(expr, updater, true);
  return updater->changed;
}


static void
instantiate_add_subs(Map<BaseAST*,BaseAST*>* substitutions, Map<BaseAST*,BaseAST*>* map) {
  Map<BaseAST *, BaseAST*> tmp;
  tmp.copy(*substitutions);
  for (int i = 0; i < tmp.n; i++) {
    if (tmp.v[i].key) {
      if (BaseAST *b = map->get(tmp.v[i].key))
        substitutions->put(b, tmp.v[i].value);
    }
  }
}


#ifdef DUPLICATE_INSTANTIATION_CACHE
/** This is a quick cache implementation that isn't perfect **/
class Inst : public gc {
 public:
  FnSymbol* fn;
  FnSymbol* newfn;
  Map<BaseAST*,BaseAST*>* subs;
};

static Vec<Inst*>* icache = NULL;
#endif

FnSymbol* 
FnSymbol::instantiate_generic(Map<BaseAST*,BaseAST*>* map,
                              Map<BaseAST*,BaseAST*>* substitutions) {
#ifdef DUPLICATE_INSTANTIATION_CACHE
  if (!icache) {
    icache = new Vec<Inst*>();
  } else {
    forv_Vec(Inst, tmp, *icache) {
      if (tmp->fn == this) {
        if (substitutions->n == 1 &&
            tmp->subs->n == 1 &&
            substitutions->v[0].key == tmp->subs->v[0].key &&
            substitutions->v[0].value == tmp->subs->v[0].value) {
          return tmp->newfn;
          /** already instantiated **/
        }
        if (substitutions->n == 2 &&
            tmp->subs->n == 2 &&
            substitutions->v[0].key == tmp->subs->v[0].key &&
            substitutions->v[1].key == tmp->subs->v[1].key &&
            substitutions->v[0].value == tmp->subs->v[0].value &&
            substitutions->v[1].value == tmp->subs->v[1].value) {
          return tmp->newfn;
          /** already instantiated **/
        }
      }
    }
  }
  Inst* inst = new Inst();
  inst->fn = this;
  inst->subs = new Map<BaseAST*,BaseAST*>();
  inst->subs->copy(*substitutions);
#endif

  FnSymbol* copy = NULL;
  static int uid = 1; // Unique ID for cloned functions
  currentLineno = lineno;
  currentFilename = filename;
  TypeSymbol* clone = NULL;
  if (fnClass == FN_CONSTRUCTOR) {
    TypeSymbol* typeSym = dynamic_cast<TypeSymbol*>(retType->symbol);
    SymScope* save_scope = Symboltable::setCurrentScope(typeSym->parentScope);
    clone = typeSym->clone(map);
    instantiate_add_subs(substitutions, map);
    ClassType* cloneType = dynamic_cast<ClassType*>(clone->definition);
    Vec<TypeSymbol *> types;
    types.move(cloneType->types);
    for (int i = 0; i < types.n; i++) {
      if (types.v[i] && substitutions->get(types.v[i]->definition)) {
        types.v[i]->defPoint->parentStmt->remove();
        types.v[i]->parentScope->remove(types.v[i]);
      } else
        cloneType->types.add(types.v[i]);
    }
    instantiate_update_expr(substitutions, clone->defPoint, map);
    substitutions->put(typeSym->definition, clone->definition);
    Symboltable::setCurrentScope(save_scope);

    cloneType->instantiatedFrom = retType;
    cloneType->substitutions.copy(*substitutions);
    tagGenerics(cloneType);

    Vec<FnSymbol*> functions;
    collectFunctionsFromScope(typeSym->parentScope, &functions);
    
    Vec<BaseAST*> genericParameters;
    for (int i = 0; i < substitutions->n; i++)
      if (VariableType *t = dynamic_cast<VariableType*>(substitutions->v[i].key)) {
        genericParameters.set_add(t);
        genericParameters.set_add(t->symbol);
      } else if (ParamSymbol *s = dynamic_cast<ParamSymbol*>(substitutions->v[i].key))
        if (s->isGeneric)
          genericParameters.set_add(s);

    forv_Vec(FnSymbol, fn, functions) {
      if (functionContainsAnyAST(fn, &genericParameters)) {
        //printf("  instantiating %s\n", fn->cname);
        SymScope* save_scope = Symboltable::setCurrentScope(fn->parentScope);
        Stmt* fnStmt = fn->defPoint->parentStmt->copy(true, map);
        ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(fnStmt);
        DefExpr* defExpr = dynamic_cast<DefExpr*>(exprStmt->expr);
        defExpr->sym->cname =
          glomstrings(3, defExpr->sym->cname, "_instantiate_", intstring(uid++));
        fn->defPoint->parentStmt->insertBefore(fnStmt);
        instantiate_add_subs(substitutions, map);
        instantiate_update_expr(substitutions, defExpr, map);
        Symboltable::setCurrentScope(save_scope);
        FnSymbol* fnClone = dynamic_cast<FnSymbol*>(defExpr->sym);
        if (fn == this) {
          copy = fnClone;
        }
        if (fn->typeBinding == typeSym) {
          clone->definition->methods.add(fnClone);
          fnClone->typeBinding = clone;
          fnClone->method_type = fn->method_type;
        }
        if (typeSym->definition->defaultConstructor == fn) {
          clone->definition->defaultConstructor = fnClone;
        }
        fnClone->instantiatedFrom = fn;
        fnClone->substitutions.copy(*substitutions);
        tagGenerics(fnClone);
      } else {
        //printf("  not instantiating %s\n", fn->cname);
      }
    }
  } else {
    SymScope* save_scope = Symboltable::setCurrentScope(parentScope);
    Stmt* fnStmt = defPoint->parentStmt->copy(true, map);
    ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(fnStmt);
    DefExpr* defExpr = dynamic_cast<DefExpr*>(exprStmt->expr);
    defPoint->parentStmt->insertBefore(fnStmt);
    instantiate_add_subs(substitutions, map);
    instantiate_update_expr(substitutions, defExpr, map);
    defExpr->sym->cname =
      glomstrings(3, defExpr->sym->cname, "_instantiate_", intstring(uid++));
    copy = dynamic_cast<FnSymbol*>(defExpr->sym);
    Symboltable::setCurrentScope(save_scope);

    FnSymbol *newFn = dynamic_cast<FnSymbol*>(defExpr->sym);
    newFn->instantiatedFrom = this;
    newFn->substitutions.copy(*substitutions);
    tagGenerics(newFn);
  }

  if (!copy) {
    INT_FATAL(this, "Instantiation error");
  }

#ifdef DUPLICATE_INSTANTIATION_CACHE
  inst->newfn = copy;
  icache->add(inst);
#endif

  //printf("finished instantiating %s\n", cname);
  
  return copy;
}


void FnSymbol::printDef(FILE* outfile) {
  fprintf(outfile, "function ");
  print(outfile);
  fprintf(outfile, "(");
  if (formals) {
    formals->print(outfile, ";\n");
  }
  fprintf(outfile, ")");
  if (retType == dtVoid) {
    fprintf(outfile, " ");
  } else {
    fprintf(outfile, ": ");
    retType->print(outfile);
    fprintf(outfile, " ");
  }
  body->print(outfile);
  fprintf(outfile, "\n\n");
}


void FnSymbol::codegenHeader(FILE* outfile) {
  if (!exportMe && !parentScope->commonModuleIsFirst()) {
    fprintf(outfile, "static ");
  }
  if (retType == dtUnknown) {
    retType = return_type_info(this);
    INT_WARNING(this, "return type unknown, calling analysis late");
  }
  retType->codegen(outfile);
//   if (is_Value_Type(retType) && _getter)
//     fprintf(outfile, "*");
  fprintf(outfile, " ");
  this->codegen(outfile);
  fprintf(outfile, "(");
  if (!formals) {
    fprintf(outfile, "void");
  } else {
    bool first = true;
    for_alist(DefExpr, formal, formals) {
      if (!first) {
        fprintf(outfile, ", ");
      }
      formal->sym->codegenDef(outfile);
      first = false;
    }
  }
  fprintf(outfile, ")");
}


void FnSymbol::codegenPrototype(FILE* outfile) {
  if (defPoint && defPoint->parentStmt) {
    if (defPoint->parentStmt->hasPragma("no codegen")) {
      return;
    }
  }

  codegenHeader(outfile);
  fprintf(outfile, ";\n");
}


void FnSymbol::codegenDef(FILE* outfile) {
  if (defPoint && defPoint->parentStmt) {
    if (defPoint->parentStmt->hasPragma("no codegen")) {
      return;
    }
  }

  if (!isDead) {
    if (fnClass == FN_CONSTRUCTOR) {
      fprintf(outfile, "/* constructor */\n");
    }

    codegenHeader(outfile);

    // while these braces seem like they should be extraneous since
    // all function bodies are BlockStmts, it turns out that they are
    // not because in C the function's parameter scope is the same as
    // its local variable scope; so if we have a parameter and a local
    // variable of name x (as in trivial/bradc/vardecls1b.chpl), these
    // extra braces are required to make the generated code work out.
    fprintf(outfile, " {\n");
    inFunction = true;
    justStartedGeneratingFunction = true;
    body->codegen(outfile);
    inFunction = false;
    fprintf(outfile, "\n");
    fprintf(outfile, "}\n");
    fprintf(outfile, "\n\n");
  }
}

FnSymbol* FnSymbol::mainFn;

void FnSymbol::init(void) {
  mainFn = NULL;
}


int Symbol::nestingDepth() {
  if (!defPoint) // labels
    return 0;
  if (!defPoint->parentStmt) // entry point
    return 0;
  Symbol *s = defPoint->parentStmt->parentSymbol;
  int d = 0;
  while (s->astType == SYMBOL_FN) {
    d++;
    s = s->defPoint->parentStmt->parentSymbol;
  }
  return d;
}


FnSymbol *Symbol::nestingParent(int i) {
  if (!defPoint) // labels
    return 0;
  if (!defPoint->parentStmt) // entry point
    return 0;
  Symbol *s = defPoint->parentStmt->parentSymbol;
  while (s->astType == SYMBOL_FN) {
    i--;
    if (i >= 0)
      return dynamic_cast<FnSymbol*>(s);
    s = s->defPoint->parentStmt->parentSymbol;
  }
  return 0;
}


EnumSymbol::EnumSymbol(char* init_name) :
  Symbol(SYMBOL_ENUM, init_name)
{
  type = dtInteger;
  Symboltable::define(this);
}


void EnumSymbol::verify(void) {
  if (astType != SYMBOL_ENUM) {
    INT_FATAL(this, "Bad EnumSymbol::astType");
  }
}


EnumSymbol*
EnumSymbol::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new EnumSymbol(copystring(name));
}


void EnumSymbol::traverseDefSymbol(Traversal* traversal) { }


void EnumSymbol::codegenDef(FILE* outfile) { }


ModuleSymbol::ModuleSymbol(char* init_name, modType init_modtype) :
  Symbol(SYMBOL_MODULE, init_name),
  modtype(init_modtype),
  stmts(new AList<Stmt>),
  initFn(NULL),
  modScope(NULL)
{ }


void ModuleSymbol::verify(void) {
  if (astType != SYMBOL_MODULE) {
    INT_FATAL(this, "Bad ModuleSymbol::astType");
  }
}


ModuleSymbol*
ModuleSymbol::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  INT_FATAL(this, "Illegal call to ModuleSymbol::copy");
  return NULL;
}


void ModuleSymbol::setModScope(SymScope* init_modScope) {
  modScope = init_modScope;
}


void ModuleSymbol::codegenDef(void) {
  fileinfo outfileinfo;
  fileinfo extheadfileinfo;
  fileinfo intheadfileinfo;

  inUserModule = (modtype == MOD_USER);
  openCFiles(name, &outfileinfo, &extheadfileinfo, &intheadfileinfo);

  fprintf(codefile, "#include \"stdchpl.h\"\n");
  fprintf(codefile, "#include \"_chpl_header.h\"\n");
  fprintf(codefile, "#include \"_CommonModule.h\"\n");
  fprintf(codefile, "#include \"_CommonModule-internal.h\"\n");

  fprintf(codefile, "#include \"%s\"\n", extheadfileinfo.filename);
  fprintf(codefile, "#include \"%s\"\n", intheadfileinfo.filename);

  fprintf(codefile, "\n");

  modScope->codegenFunctions(codefile);
  //  stmts->codegen(codefile, "\n");

  closeCFiles(&outfileinfo, &extheadfileinfo, &intheadfileinfo);
}


void ModuleSymbol::startTraversal(Traversal* traversal) {
  SymScope* prevScope = NULL;

  if (modScope) {
    prevScope = Symboltable::setCurrentScope(modScope);
  }
  stmts->traverse(traversal, false);
  if (modScope) {
    Symboltable::setCurrentScope(prevScope);
  }
}


void ModuleSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == stmts) {
    stmts = dynamic_cast<AList<Stmt>*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in ModuleSymbol::replaceChild");
  }
}


/** SJD: Makes sense for this to take place of above startTraversal **/
void ModuleSymbol::traverseDefSymbol(Traversal* traversal) {
  startTraversal(traversal);
}


static bool stmtIsGlob(Stmt* stmt) {

  if (stmt == NULL) {
    INT_FATAL("Non-Stmt found in StmtIsGlob");
  }
  if (ExprStmt* expr_stmt = dynamic_cast<ExprStmt*>(stmt)) {
    if (DefExpr* defExpr = dynamic_cast<DefExpr*>(expr_stmt->expr)) {
      if (dynamic_cast<FnSymbol*>(defExpr->sym) ||
          dynamic_cast<TypeSymbol*>(defExpr->sym)) {
        return true;
      }
    }
  }
  return false;
}


void ModuleSymbol::createInitFn(void) {
  char* fnName = glomstrings(2, "__init_", name);
  AList<Stmt>* globstmts = NULL;
  AList<Stmt>* initstmts = NULL;
  AList<Stmt>* definition = stmts;

  // BLC: code to run user modules once only
  char* runOnce = NULL;
  if (modtype == MOD_USER) {
    runOnce = glomstrings(3, "__run_", name, "_firsttime");
    SymScope* saveScope = Symboltable::setCurrentScope(commonModule->modScope);
    // create a boolean variable to guard module initialization
    DefExpr* varDefExpr = Symboltable::defineSingleVarDef(runOnce,
                                                          dtBoolean,
                                                          new BoolLiteral(true),
                                                          VAR_NORMAL,
                                                          VAR_VAR);
    // insert its definition in the common module's init function
    commonModule->initFn->body->body->insertAtHead(new ExprStmt(varDefExpr));
    Symboltable::setCurrentScope(saveScope);
 
    // insert a set to false at the beginning of the current module's
    // definition (we'll wrap it in a conditional just below, after
    // filtering)
    Expr* assignVar = new CallExpr(OP_GETSNORM,
                                   new Variable(new UnresolvedSymbol(runOnce)),
                                   new BoolLiteral(false));
    definition->insertAtHead(new ExprStmt(assignVar));
  }

  definition->filter(stmtIsGlob, globstmts, initstmts);

  definition = globstmts;
  BlockStmt* initFunBody;
  if (initstmts->isEmpty()) {
    initFunBody = new BlockStmt(new AList<Stmt>());
  } else {
    initFunBody = new BlockStmt(initstmts);
  }
  
  if (runOnce) {
    // wrap initializer function body in conditional
    Stmt* testRun = new CondStmt(new Variable(new UnresolvedSymbol(runOnce)), 
                                 initFunBody);
    // and replace it
    initFunBody = new BlockStmt(new AList<Stmt>(testRun));
  }
                    
  DefExpr* initFunDef = Symboltable::defineFunction(fnName, NULL, 
                                                    dtVoid, initFunBody, 
                                                    true);
  initFn = dynamic_cast<FnSymbol*>(initFunDef->sym);
  {
    Stmt* initstmt = initstmts->first();
    while (initstmt) {
      initstmt->parentSymbol = initFn;
      initstmt = initstmts->next();
    }
    initFunBody->parentSymbol = initFn;
  }

  definition->insertAtTail(new ExprStmt(initFunDef));

  stmts = definition;
}


bool ModuleSymbol::isFileModule(void) {
  return (lineno == 0);
}


LabelSymbol::LabelSymbol(char* init_name) :
  Symbol(SYMBOL_LABEL, init_name, NULL) {
  Symboltable::define(this);
}


void LabelSymbol::verify(void) {
  if (astType != SYMBOL_LABEL) {
    INT_FATAL(this, "Bad LabelSymbol::astType");
  }
}


void LabelSymbol::codegenDef(FILE* outfile) { }

void
initSymbol() {
  gNil = Symboltable::defineSingleVarDef("nil", dtNil, NULL, VAR_NORMAL, VAR_CONST)->sym;
  gUnspecified = Symboltable::defineSingleVarDef("_", dtUnknown, NULL, VAR_NORMAL, VAR_CONST)->sym;
}
