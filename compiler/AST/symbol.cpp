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

Symbol *gNil = 0;


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
  pragmas(NULL),
  asymbol(0),
  overload(NULL)
{}


void Symbol::setParentScope(SymScope* init_parentScope) {
  parentScope = init_parentScope;
}


Symbol* Symbol::copy(bool clone, Map<BaseAST*,BaseAST*>* map) {
  if (!this) {
    return this;
  }
  if (map == NULL) {
    map = new Map<BaseAST*,BaseAST*>();
  }
  Symbol* new_symbol = copyInternal(clone, map);
  TRAVERSE(new_symbol, new UpdateSymbols(map), true);
  return new_symbol;
}


Symbol* Symbol::copyInternal(bool clone, Map<BaseAST*,BaseAST*>* map) {
  Symbol* new_symbol = copySymbol(clone, map);

  new_symbol->copyFrom = this;
  new_symbol->pragmas = pragmas;
  new_symbol->lineno = lineno;
  new_symbol->filename = filename;
  if (map) {
    map->put(this, new_symbol);
  }
  return new_symbol;
}


Symbol* Symbol::copySymbol(bool clone, Map<BaseAST*,BaseAST*>* map) {
  INT_FATAL(this, "Symbol::copySymbol() not anticipated to be needed");
  return NULL;
}


void Symbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  INT_FATAL(this, "Unexpected call to Symbol::replaceChild(old, new)");
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


bool Symbol::hasPragma(char* str) {
  Pragma* pr = pragmas->first();
  while (pr) {
    if (!strcmp(pr->str, str)) {
      return true;
    }
    pr = pragmas->next();
  }
  return false;
}


void Symbol::addPragma(char* str) {
  if (pragmas) {
    pragmas->insertAtTail(new Pragma(copystring(str)));
  } else {
    pragmas = new AList<Pragma>(new Pragma(copystring(str)));
  }
}


void Symbol::codegen(FILE* outfile) {
  if (hasPragma("codegen data")) {
    StructuralType* dataType = dynamic_cast<StructuralType*>(type);
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


void Symbol::setDefPoint(DefExpr* init_defPoint) {
  defPoint = init_defPoint;
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
}


void UnresolvedSymbol::codegen(FILE* outfile) {
  INT_FATAL(this, "ERROR:  Cannot codegen an unresolved symbol.");
}


Symbol* UnresolvedSymbol::copySymbol(bool clone, Map<BaseAST*,BaseAST*>* map) {
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
  aspect(NULL),
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


Symbol* VarSymbol::copySymbol(bool clone, Map<BaseAST*,BaseAST*>* map) {
  VarSymbol* newVarSymbol = 
    new VarSymbol(copystring(name), type, varClass, consClass);
  newVarSymbol->aspect = aspect;
  newVarSymbol->noDefaultInit = noDefaultInit;
  return newVarSymbol;
}


void VarSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  type->replaceChild(old_ast, new_ast);
}


void VarSymbol::traverseDefSymbol(Traversal* traversal) {
  SymScope* saveScope = NULL;
  /** SJD: assumes no nested arrays, should use a traversal to push scopes **/
  if (ArrayType* array_type = dynamic_cast<ArrayType*>(type)) {
    if (ForallExpr* forall = dynamic_cast<ForallExpr*>(array_type->domain)) {
      if (forall->indexScope) {
        saveScope = Symboltable::setCurrentScope(forall->indexScope);
      }
    }
  }
  TRAVERSE(type, traversal, false);
  if (saveScope) {
    Symboltable::setCurrentScope(saveScope);
  }
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
  case SCOPE_INTERNAL_PRELUDE:
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
  FnSymbol *f = dynamic_cast<FnSymbol*>(defPoint->parentStmt->parentSymbol);
  if (!f || !f->_this)
    return 0;
  else
    return f->_this == this;
}

void VarSymbol::codegenDef(FILE* outfile) {
  if (parentScope->type == SCOPE_MODULE) {
    outfile = exportMe ? extheadfile : intheadfile;
    if (!exportMe && !parentScope->commonModuleIsFirst()) {
      fprintf(outfile, "static ");
    }
  }

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
                         Type* init_type, Expr* init_init) :
  Symbol(SYMBOL_PARAM, init_name, init_type),
  intent(init_intent),
  init(init_init),
  typeVariable(NULL),
  isGeneric(false)
{
  if (name) {  // ensure this is not a sentinel
    Symboltable::define(this);
  }
  if (intent == PARAM_PARAMETER || intent == PARAM_TYPE)
    isGeneric = true;
}


Symbol* ParamSymbol::copySymbol(bool clone, Map<BaseAST*,BaseAST*>* map) {
  ParamSymbol *ps = new ParamSymbol(intent, copystring(name), type, init->copyInternal(clone, map));
  if (typeVariable)
    ps->typeVariable = typeVariable;
  ps->isGeneric = isGeneric;
  return ps;
}


void ParamSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == init) {
    init = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in ParamSymbol::replaceChild(old, new)");
  }
}


void ParamSymbol::traverseDefSymbol(Traversal* traversal) {
  TRAVERSE(type, traversal, false);
  TRAVERSE(init, traversal, false);
  TRAVERSE(typeVariable, traversal, false);
}


void ParamSymbol::printDef(FILE* outfile) {
  fprintf(outfile, "%s ", paramTypeNames[intent]);
  Symbol::print(outfile);
  fprintf(outfile, ": ");
  type->print(outfile);
}


bool ParamSymbol::requiresCPtr(void) {
  return (((intent == PARAM_OUT || intent == PARAM_INOUT || 
            (intent == PARAM_REF && !is_Reference_Type(type))) && 
           type->outParamNeedsPtr()) ||
          (intent == PARAM_BLANK && type->blankIntentImpliesRef()));
}


bool ParamSymbol::requiresCopyBack(void) {
  return ((intent == PARAM_OUT || intent == PARAM_INOUT) &&
          type->outParamNeedsPtr());
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
  Symbol(SYMBOL_TYPE, init_name, init_definition)
{
  Symboltable::define(this);
}


Symbol* TypeSymbol::copySymbol(bool clone, Map<BaseAST*,BaseAST*>* map) {
  Type* new_type = type->copyInternal(clone, map);
  TypeSymbol* new_type_symbol = new TypeSymbol(copystring(name), new_type);
  new_type->addSymbol(new_type_symbol);
  if (StructuralType* stype =
      dynamic_cast<StructuralType*>(new_type)) {
    stype->structScope->setContext(NULL, new_type_symbol);
  }
  return new_type_symbol;
}


TypeSymbol* TypeSymbol::clone(Map<BaseAST*,BaseAST*>* map) {
  static int uid = 1; // Unique ID for cloned classes
  StructuralType* old_class_type = dynamic_cast<StructuralType*>(type);
  if (!old_class_type) {
    INT_FATAL(this, "Attempt to clone non-class type");
  }
  SymScope* save_scope = Symboltable::setCurrentScope(parentScope);
  TypeSymbol* clone = dynamic_cast<TypeSymbol*>(copy(true, map));
  if (ClassType* newClassType = dynamic_cast<ClassType*>(clone->type)) {
    ClassType* oldClassType = dynamic_cast<ClassType*>(type);
    if (!oldClassType) {
      INT_FATAL(this, "Cloning of ClassType went horribly wrong");
    }
    newClassType->parentClasses.add(oldClassType);
  }
  clone->cname = glomstrings(3, clone->cname, "_clone_", intstring(uid++));
  DefExpr* new_def_expr = new DefExpr(clone);
  StructuralType* new_class_type = dynamic_cast<StructuralType*>(clone->type);
  new_class_type->structScope->setContext(NULL, clone, new_def_expr);
  defPoint->insertBefore(new_def_expr);
  Symboltable::setCurrentScope(save_scope);
  return clone;
}


void TypeSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  type->replaceChild(old_ast, new_ast);
}


void TypeSymbol::traverseDefSymbol(Traversal* traversal) {
  TRAVERSE_DEF(type, traversal, false);
}


void TypeSymbol::codegenPrototype(FILE* outfile) {
  if (isDead) {
    return;
  }

  FILE* deffile = outfile;
  /* if in file scope, hoist to internal header so that it will be
     defined before global variables at file scope. */  
  if (type->symbol->parentScope->type == SCOPE_MODULE) { 
    deffile = exportMe ? extheadfile : intheadfile;
  }
  type->codegenPrototype(deffile);
}


void TypeSymbol::codegenDef(FILE* outfile) {
  if (isDead) {
    return;
  }

  FILE* deffile = outfile;
  /* if in file scope, hoist to internal header so that it will be
     defined before global variables at file scope. */  
  if (type->symbol->parentScope->type == SCOPE_MODULE) { 
    deffile = exportMe ? extheadfile : intheadfile;
  }
  type->codegenDef(deffile);

  type->codegenStringToType(outfile);
  type->codegenConfigVarRoutines(outfile);
}


/***
 *** SJD: I'm assuming a tuple with component types that are all
 *** primitive types and I'm declaring this thing with a mangled name
 *** in the commonModule.  This won't be possible when we support
 *** tuples of different types.  In this case, they may have to be
 *** defined in the scope they are used.
 ***/
TypeSymbol* TypeSymbol::lookupOrDefineTupleTypeSymbol(Vec<Type*>* components) {
  char* name = glomstrings(1, "_tuple");
  forv_Vec(Type, component, *components) {
    name = glomstrings(3, name, "_", component->symbol->name);
  }
  SymScope* saveScope = Symboltable::setCurrentScope(commonModule->modScope);
  TypeSymbol* tupleSym =
    dynamic_cast<TypeSymbol*>(Symboltable::lookupInCurrentScope(name));
  if (!tupleSym) {
    TupleType* tupleType = new TupleType();
    forv_Vec(Type, component, *components) {
      tupleType->addType(component);
    }
    tupleSym = new TypeSymbol(name, tupleType);
    tupleType->addSymbol(tupleSym);
    DefExpr* defExpr = new DefExpr(tupleSym);
    DefStmt* defStmt = new DefStmt(new AList<DefExpr>(defExpr));
    tupleType->structScope->setContext(NULL, tupleSym, defExpr);
    commonModule->stmts->insertAtHead(defStmt);
    buildDefaultStructuralTypeMethods(tupleType);
  }
  Symboltable::setCurrentScope(saveScope);
  return tupleSym;
}


FnSymbol::FnSymbol(char* init_name, Symbol* init_typeBinding) :
  Symbol(SYMBOL_FN, init_name, new FnType()),
  formals(NULL),
  retType(NULL),
  _this(NULL),
  _setter(NULL),
  _getter(NULL),
  body(NULL),
  typeBinding(init_typeBinding),
  isConstructor(false),
  whereExpr(NULL)
{
  Symboltable::define(this);
  method_type = NON_METHOD;
}


void FnSymbol::continueDef(AList<ParamSymbol>* init_formals, Type* init_retType, bool isRef, Expr *init_whereExpr) {
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


Symbol* FnSymbol::copySymbol(bool clone, Map<BaseAST*,BaseAST*>* map) {
  char* copy_name;
  if (_getter) {
    copy_name = glomstrings(2, "_chplget_", name);
  } else {
    copy_name = copystring(name);
  }
  FnSymbol* copy = new FnSymbol(copy_name, typeBinding);
  copy->method_type = method_type;
  copy->isConstructor = isConstructor;
  Symboltable::startFnDef(copy);
  if (_getter) {
    copy->name = copystring(name);
  }
  copy->_getter = _getter; // If it is a cloned class we probably want this
  copy->_setter = _setter; //  to point to the new member, but how do we do that
  copy->_this = _this;
  AList<ParamSymbol>* new_formals = formals->copyInternal(clone, map);
  Symboltable::continueFnDef(copy, new_formals, retType, retRef, whereExpr->copyInternal(clone, map));
  BlockStmt* new_body = 
    dynamic_cast<BlockStmt*>(body->copyInternal(clone, map));
  if (body != NULL && new_body == NULL) {
    INT_FATAL(body, "function body was not a BlockStmt!?");
  }
  return Symboltable::finishFnDef(copy, new_body, exportMe);
}


void FnSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == body) {
    body = dynamic_cast<BlockStmt*>(new_ast);
  } else if (old_ast == whereExpr) {
    whereExpr = dynamic_cast<Expr*>(new_ast);
  } else {
    bool found = false;
    for (ParamSymbol* param = formals->first(); param; param = formals->next()) {
      if (old_ast == param->init) {
        param->init = dynamic_cast<Expr*>(new_ast);
        found = true;
      }
    }
    if (!found) {
      INT_FATAL(this, "Unexpected case in FnSymbol::replaceChild(old, new)");
    }
  }
}


void FnSymbol::traverseDefSymbol(Traversal* traversal) {
  SymScope* saveScope = NULL;

  if (paramScope) {
    saveScope = Symboltable::setCurrentScope(paramScope);
  }
  formals->traverseDef(traversal, false);
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
  DefExpr* this_copy = NULL;
  SymScope* save_scope = Symboltable::setCurrentScope(parentScope);
  Expr* expr_copy = defPoint->copy(true, map, NULL);
  if (this_copy = dynamic_cast<DefExpr*>(expr_copy)) {
    this_copy->sym->cname =
      glomstrings(3, cname, "_clone_", intstring(uid++));
    defPoint->insertAfter(this_copy);
  } else {
    INT_FATAL(this, "Unreachable statement in FnSymbol::clone reached");
  }
  Symboltable::setCurrentScope(save_scope);
  TRAVERSE(this_copy, new ClearTypes(), true);
  TRAVERSE(defPoint, new ClearTypes(), true); // only do this until uncloned is not used
  FnSymbol* fn = dynamic_cast<FnSymbol*>(this_copy->sym);
  return fn;
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
  wrapperFn->isConstructor = isConstructor;

  AList<ParamSymbol>* wrapperFormals = new AList<ParamSymbol>();
  for (ParamSymbol* formal = formals->first(); formal; formal = formals->next()) {
    ParamSymbol* newFormal = dynamic_cast<ParamSymbol*>(formal->copy());
    wrapperFormals->insertAtTail(newFormal);
    Symbol* coercionSubstitution = coercion_substitutions->get(formal);
    if (coercionSubstitution) {
      newFormal->type = coercionSubstitution->type;
    }
  }
  Symboltable::continueFnDef(wrapperFn, wrapperFormals, retType, retRef);

  BlockStmt* wrapperBlock = Symboltable::startCompoundStmt();

  AList<Stmt>* wrapperBody = new AList<Stmt>();
  AList<Expr>* wrapperActuals = new AList<Expr>();
  for (Symbol* formal = formals->first(), *wrapperFormal = wrapperFormals->first(); 
       formal; 
       formal = formals->next(), wrapperFormal = wrapperFormals->next()) {
    Symbol* coercionSubstitution = coercion_substitutions->get(formal);
    if (coercionSubstitution) {
      char* tempName = glomstrings(2, "_coercion_temp_", formal->name);
      VarSymbol* temp = new VarSymbol(tempName, formal->type);
      DefExpr* tempDefExpr = new DefExpr(temp, new UserInitExpr(new Variable(wrapperFormal)));
      wrapperBody->insertAtTail(new DefStmt(new AList<DefExpr>(tempDefExpr)));
      wrapperActuals->insertAtTail(new Variable(temp));
    } else {
      wrapperActuals->insertAtTail(new Variable(wrapperFormal));
    }
  }
  wrapperBody->insertAtTail(new ExprStmt(new FnCall(new Variable(this), 
                                                    wrapperActuals)));

  wrapperBlock = Symboltable::finishCompoundStmt(wrapperBlock, wrapperBody);

  DefExpr* defExpr = new DefExpr(Symboltable::finishFnDef(wrapperFn,
                                                          wrapperBlock));
  defPoint->insertBefore(defExpr);
  Symboltable::setCurrentScope(saveScope);
  return wrapperFn;
}


FnSymbol* FnSymbol::default_wrapper(Vec<Symbol*>* defaults) {
  static int uid = 1; // Unique ID for wrapped functions
  currentLineno = lineno;
  currentFilename = filename;
  FnSymbol* wrapper_symbol;
  Vec<Symbol*> for_removal;
  SymScope* save_scope = Symboltable::setCurrentScope(parentScope);
  wrapper_symbol = new FnSymbol(name);
  wrapper_symbol->method_type = method_type;
  wrapper_symbol->isConstructor = isConstructor;
  wrapper_symbol->cname =
    glomstrings(3, cname, "_default_params_wrapper_", intstring(uid++));
  wrapper_symbol = Symboltable::startFnDef(wrapper_symbol);
  AList<ParamSymbol>* wrapper_formals = formals->copy();
  ParamSymbol* actuals = wrapper_formals->first();
  AList<Expr>* argList = new AList<Expr>(new Variable(actuals));
  actuals = wrapper_formals->next();
  while (actuals) {
    argList->insertAtTail(new Variable(actuals));
    actuals = wrapper_formals->next();
  }
  ParenOpExpr* fn_call = new ParenOpExpr(new Variable(this), argList);
  Symboltable::pushScope(SCOPE_LOCAL);
  AList<Stmt>* wrapper_body = new AList<Stmt>();
  if (retType == dtVoid || (retType == dtUnknown && function_returns_void(this))) {
    wrapper_body->insertAtTail(new ExprStmt(fn_call));
  } else {
    wrapper_body->insertAtTail(new ReturnStmt(fn_call));
  }
  Vec<ParamSymbol *> vformals, vwformals;
  Vec<Variable *> vargs;
  for (ParamSymbol* formal = formals->first(); formal; formal = formals->next())
    vformals.add(formal);
  for (ParamSymbol* formal = wrapper_formals->first(); formal; formal = wrapper_formals->next())
    vwformals.add(formal);
  for (Expr* a = argList->first(); a; a = argList->next()) {
    Variable* var = dynamic_cast<Variable*>(a);
    if (var == NULL) {
      INT_FATAL(a, "bad assumption about argList");
    }
    vargs.add(var);
  }
  for (int i = 0; i< vwformals.n; i++) {
    if (defaults->set_in(vformals.v[i])) {
      ParamSymbol *formal = vwformals.v[i];
      char* temp_name = glomstrings(2, "_default_param_temp_", formal->name);
      VarSymbol* temp_symbol = new VarSymbol(temp_name, formal->type);
      if (formal->type != dtUnknown)
        temp_symbol->aspect = formal->type;
      DefExpr* temp_def_expr =
        new DefExpr(temp_symbol,
                    (formal->intent == PARAM_OUT)
                    ? NULL
                    : new UserInitExpr(((ParamSymbol*)formal)->init->copy()));
      DefStmt* temp_def_stmt = new DefStmt(new AList<DefExpr>(temp_def_expr));
      wrapper_body->insertAtHead(temp_def_stmt);
      vargs.v[i]->var = temp_symbol;
      if (formal == wrapper_formals->first())
        wrapper_formals->popHead();
      if (formal->prev)
        formal->prev->next = formal->next;
      if (formal->next)
        formal->next->prev = formal->prev;
      for_removal.add(formal);
    }
  }
  SymScope* block_scope = Symboltable::popScope();
  BlockStmt* wrapper_block = new BlockStmt(wrapper_body, block_scope);
  Symboltable::continueFnDef(wrapper_symbol, wrapper_formals, retType, retRef);
  block_scope->stmtContext = wrapper_block;
  DefExpr* wrapper_expr = new DefExpr(Symboltable::finishFnDef(wrapper_symbol,
                                                               wrapper_block));
  forv_Vec(Symbol, sym, for_removal)
    wrapper_symbol->paramScope->remove(sym);
  defPoint->insertAfter(wrapper_expr);
  Symboltable::setCurrentScope(save_scope);
  return dynamic_cast<FnSymbol*>(wrapper_expr->sym);
}


FnSymbol* FnSymbol::order_wrapper(Map<Symbol*,Symbol*>* formals_to_actuals) {
  static int uid = 1; // Unique ID for wrapped functions
  currentLineno = lineno;
  currentFilename = filename;
  SymScope* save_scope = Symboltable::setCurrentScope(parentScope);
  FnSymbol* wrapper_fn = new FnSymbol(name);
  wrapper_fn->cname = glomstrings(3, cname, "_ord_wrapper_", intstring(uid++));
  wrapper_fn = Symboltable::startFnDef(wrapper_fn);
  wrapper_fn->method_type = method_type;
  wrapper_fn->isConstructor = isConstructor;

  AList<ParamSymbol>* wrapper_formals = new AList<ParamSymbol>();
  for (int i = 0; i < formals_to_actuals->n - 1; i++) {
    ParamSymbol* tmp = formals->first();
    for (int j = 0; j < formals_to_actuals->n - 1; j++) {
      if (formals_to_actuals->v[i].key == formals_to_actuals->v[j].value) {
        wrapper_formals->insertAtTail(dynamic_cast<ParamSymbol*>(tmp->copy()));
      }
      tmp = formals->next();
    }
  }

  Symboltable::continueFnDef(wrapper_fn, wrapper_formals, retType, retRef);

  AList<Expr>* actuals = new AList<Expr>();
  for (int i = 0; i < formals_to_actuals->n - 1; i++) {
    Symbol* tmp = wrapper_formals->first();
    for (int j = 0; j < formals_to_actuals->n - 1; j++) {
      if (formals_to_actuals->v[i].value == formals_to_actuals->v[j].key) {
        actuals->insertAtTail(new Variable(tmp));
      }
      tmp = wrapper_formals->next();
    }
  }

  Stmt* fn_call = new ExprStmt(new ParenOpExpr(new Variable(this), actuals));
  BlockStmt* body = new BlockStmt(new AList<Stmt>(fn_call));
  DefExpr* def_expr = new DefExpr(Symboltable::finishFnDef(wrapper_fn, body));
  defPoint->insertBefore(def_expr);
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


FnSymbol* 
FnSymbol::instantiate_generic(Map<BaseAST*,BaseAST*>* map,
                              Map<BaseAST*,BaseAST*>* substitutions) {
  FnSymbol* copy = NULL;
  static int uid = 1; // Unique ID for cloned functions
  currentLineno = lineno;
  currentFilename = filename;
  TypeSymbol* clone = NULL;
  if (isConstructor) {
    TypeSymbol* typeSym = dynamic_cast<TypeSymbol*>(retType->symbol);
    SymScope* save_scope = Symboltable::setCurrentScope(typeSym->parentScope);
    clone = typeSym->clone(map);
    instantiate_add_subs(substitutions, map);
    StructuralType* cloneType = dynamic_cast<StructuralType*>(clone->type);
    Vec<TypeSymbol *> types;
    types.move(cloneType->types);
    for (int i = 0; i < types.n; i++) {
      if (types.v[i] && substitutions->get(types.v[i]->type)) {
        types.v[i]->defPoint->parentStmt->remove();
        types.v[i]->parentScope->remove(types.v[i]);
      } else
        cloneType->types.add(types.v[i]);
    }
    instantiate_update_expr(substitutions, clone->defPoint, map);
    substitutions->put(typeSym->type, clone->type);
    Symboltable::setCurrentScope(save_scope);

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
        DefExpr* fnDef = dynamic_cast<DefExpr*>(fn->defPoint->copy(true, map));
        fnDef->sym->cname = glomstrings(3, fnDef->sym->cname, "_instantiate_", intstring(uid++));
        fn->defPoint->insertBefore(fnDef);
        instantiate_add_subs(substitutions, map);
        instantiate_update_expr(substitutions, fnDef, map);
        Symboltable::setCurrentScope(save_scope);
        FnSymbol* fnClone = dynamic_cast<FnSymbol*>(fnDef->sym);
        if (fn == this) {
          copy = fnClone;
        }
        if (fn->typeBinding == typeSym) {
          clone->type->methods.add(fnClone);
          fnClone->typeBinding = clone;
          fnClone->method_type = fn->method_type;
        }
        if (typeSym->type->defaultConstructor == fn) {
          clone->type->defaultConstructor = fnClone;
        }
      } else {
        //printf("  not instantiating %s\n", fn->cname);
      }
    }
  } else {
    SymScope* save_scope = Symboltable::setCurrentScope(parentScope);
    DefExpr* fnDef = dynamic_cast<DefExpr*>(defPoint->copy(true, map));
    defPoint->insertBefore(fnDef);
    instantiate_add_subs(substitutions, map);
    instantiate_update_expr(substitutions, fnDef, map);
    fnDef->sym->cname =
      glomstrings(3, fnDef->sym->cname, "_instantiate_", intstring(uid++));
    copy = dynamic_cast<FnSymbol*>(fnDef->sym);
    Symboltable::setCurrentScope(save_scope);
  }

  if (!copy) {
    INT_FATAL(this, "Instantiation error");
  }

  //printf("finished instantiating %s\n", cname);
  
  return copy;
}


void FnSymbol::printDef(FILE* outfile) {
  fprintf(outfile, "function ");
  print(outfile);
  fprintf(outfile, "(");
  if (formals) {
    formals->printDef(outfile, ";\n");
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
  if (is_Value_Type(retType) && _getter)
    fprintf(outfile, "*");
  fprintf(outfile, " ");
  this->codegen(outfile);
  fprintf(outfile, "(");
  if (!formals) {
    fprintf(outfile, "void");
  } else {
    formals->codegenDef(outfile, ", ");
  }
  fprintf(outfile, ")");
}


void FnSymbol::codegenDef(FILE* outfile) {
  FILE* headfile;

  if (defPoint && defPoint->parentStmt) {
    for (Pragma* pragma = defPoint->parentStmt->pragmas->first();
         pragma;
         pragma = defPoint->parentStmt->pragmas->next()) {
      if (!strcmp(pragma->str, "no codegen")) {
        return;
      }
    }
  }

  if (!isDead) {
    if (exportMe) {
      headfile = extheadfile;
    } else {
      headfile = intheadfile;
    }
    codegenHeader(headfile);
    fprintf(headfile, ";\n");

    if (isConstructor) {
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
    body->codegen(outfile);
    fprintf(outfile, "\n");
    fprintf(outfile, "}\n");
    fprintf(outfile, "\n\n");
  }
}

FnSymbol* FnSymbol::mainFn;

void FnSymbol::init(void) {
  mainFn = NULL;
}



EnumSymbol::EnumSymbol(char* init_name, Expr* init_init, int init_val) :
  Symbol(SYMBOL_ENUM, init_name),
  init(init_init),
  val(init_val)
{
  if (init_name != NULL) {
    Symboltable::define(this);
  }
}


Symbol* EnumSymbol::copySymbol(bool clone, Map<BaseAST*,BaseAST*>* map) {
  return new EnumSymbol(copystring(name), init->copyInternal(clone, map), val);
}


void EnumSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == init) {
    init = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in EnumSymbol::replaceChild(old, new)");
  }
}


void EnumSymbol::traverseDefSymbol(Traversal* traversal) { }


void EnumSymbol::setValues(AList<EnumSymbol>* symList) {
  EnumSymbol* tmp = symList->first();
  int tally = 0;

  while (tmp) {
    if (tmp->init) {
      if (tmp->init->isComputable() == false) {
        USR_FATAL(tmp->init, "Enumerator value for %s must be integer parameter", tmp->name);
      }
      tally = tmp->init->intVal();
    }
    tmp->val = tally++;
    tmp = symList->next();
  }
}


void EnumSymbol::codegenDef(FILE* outfile) {
  /* Do nothing */
}


ModuleSymbol::ModuleSymbol(char* init_name, modType init_modtype) :
  Symbol(SYMBOL_MODULE, init_name),
  modtype(init_modtype),
  stmts(new AList<Stmt>),
  initFn(NULL),
  modScope(NULL)
{
  uses.clear();
}


void ModuleSymbol::setModScope(SymScope* init_modScope) {
  modScope = init_modScope;
}


void ModuleSymbol::codegenDef(void) {
  fileinfo outfileinfo;
  fileinfo extheadfileinfo;
  fileinfo intheadfileinfo;

  openCFiles(name, &outfileinfo, &extheadfileinfo, &intheadfileinfo);

  fprintf(codefile, "#include \"stdchpl.h\"\n");

  fprintf(codefile, "#include \"_CommonModule.h\"\n");
  fprintf(codefile, "#include \"_CommonModule-internal.h\"\n");

  forv_Vec(ModuleSymbol, use, uses) {
    fprintf(codefile, "#include \"%s.h\"\n", use->name);
  }

  fprintf(codefile, "#include \"%s\"\n", extheadfileinfo.filename);
  fprintf(codefile, "#include \"%s\"\n", intheadfileinfo.filename);

  fprintf(codefile, "\n");

  modScope->codegen(codefile, "\n");
  stmts->codegen(codefile, "\n");

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
    INT_FATAL(this, "Unexpected case in ModuleSymbol::replaceChild(old, new)");
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
  if (DefStmt* def_stmt = dynamic_cast<DefStmt*>(stmt)) {
    if (def_stmt->definesFunctions() || def_stmt->definesTypes()) {
      return true;
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
    DefStmt* varDefStmt = Symboltable::defineSingleVarDefStmt(runOnce,
                                                              dtBoolean,
                                                              new BoolLiteral("true", true),
                                                              VAR_NORMAL,
                                                              VAR_VAR);
    // insert its definition in the common module's init function
    commonModule->initFn->body->body->insertAtHead(varDefStmt);
    Symboltable::setCurrentScope(saveScope);
 
    // insert a set to false at the beginning of the current module's
    // definition (we'll wrap it in a conditional just below, after
    // filtering)
    Expr* assignVar = new AssignOp(GETS_NORM, 
                                   new Variable(new UnresolvedSymbol(runOnce)), 
                                   new BoolLiteral("false", false));
    definition->insertAtHead(new ExprStmt(assignVar));
  }

  definition->filter(stmtIsGlob, globstmts, initstmts);

  definition = globstmts;
  BlockStmt* initFunBody;
  if (initstmts->isEmpty()) {
    initFunBody = new BlockStmt(new AList<Stmt>(new NoOpStmt()));
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
                    
  DefStmt* initFunDef = Symboltable::defineFunction(fnName, NULL, 
                                                    dtVoid, initFunBody, 
                                                    true);
  initFn = dynamic_cast<FnSymbol*>(initFunDef->defExprls->only()->sym);
  {
    Stmt* initstmt = initstmts->first();
    while (initstmt) {
      initstmt->parentSymbol = initFn;
      initstmt = initstmts->next();
    }
    initFunBody->parentSymbol = initFn;
  }

  definition->insertAtTail(initFunDef);

  stmts = definition;
}


bool ModuleSymbol::isFileModule(void) {
  return (lineno == 0);
}


LabelSymbol::LabelSymbol(char* init_name) :
  Symbol(SYMBOL_LABEL, init_name, NULL) {
  Symboltable::define(this);
}


void LabelSymbol::codegenDef(FILE* outfile) { }


ForwardingSymbol::ForwardingSymbol(Symbol* init_forward, char* rename) :
  Symbol(SYMBOL_FORWARDING, rename, NULL),
  forward(init_forward),
  renamed(rename != NULL)
{
  if (!rename) {
    name = copystring(forward->name);
    cname = copystring(forward->cname);
  }
  Symboltable::define(this);
}


void ForwardingSymbol::codegenDef(FILE* outfile) { }


FnSymbol* ForwardingSymbol::getFnSymbol(void) {
  return forward->getFnSymbol();
}


Symbol* ForwardingSymbol::getSymbol(void) {
  return forward->getSymbol();
}


Type* ForwardingSymbol::typeInfo(void) {
  return forward->typeInfo();
}
