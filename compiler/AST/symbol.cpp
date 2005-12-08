#include <typeinfo>
#include "astutil.h"
#include "analysis.h"
#include "files.h"
#include "misc.h"
#include "runtime.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "symtab.h"
#include "astutil.h"
#include "../traversals/buildClassHierarchy.h"
#include "../traversals/createConfigVarTable.h"
#include "../traversals/findTypeVariables.h"
#include "../traversals/instantiate.h"
#include "../passes/preAnalysisCleanup.h"

FnSymbol* chpl_main = NULL;
Symbol *gNil = 0;
Symbol *gUnspecified = 0;

/*** Instantiation Cache vvv ***/
class Inst : public gc {
 public:
  Inst(FnSymbol* iOldFn, FnSymbol* iNewFn, ASTMap* iSubs) :
    oldFn(iOldFn), newFn(iNewFn), subs(new ASTMap(*iSubs)) { }
  FnSymbol* oldFn;
  FnSymbol* newFn;
  ASTMap* subs;
};
static Vec<Inst*> icache;

static bool 
subs_match(ASTMap* s1, ASTMap* s2) {
  form_Map(ASTMapElem, e, *s1)
    if (s2->get(e->key) != e->value)
      return false;
  form_Map(ASTMapElem, e, *s2)
    if (s1->get(e->key) != e->value)
      return false;
  return true;
}

static FnSymbol*
check_icache(FnSymbol* fn, ASTMap* substitutions) {
  forv_Vec(Inst, inst, icache)
    if (inst->oldFn == fn && subs_match(substitutions, inst->subs))
      return inst->newFn;
  return NULL;
}

static void
add_icache(FnSymbol* newFn) {
  icache.add(new Inst(newFn->instantiatedFrom, newFn, &newFn->substitutions));
}
/*** Instantiation Cache ^^^ ***/


/*** Default Wrapper Cache vvv ***/
class DWCacheItem : public gc {
 public:
  DWCacheItem(FnSymbol* iOldFn, FnSymbol* iNewFn, Vec<Symbol*>* iDefaults) :
    oldFn(iOldFn), newFn(iNewFn), defaults(new Vec<Symbol*>(*iDefaults)) { }
  FnSymbol* oldFn;
  FnSymbol* newFn;
  Vec<Symbol*>* defaults;
};
static Vec<DWCacheItem*> dwcache;

static bool 
dw_match(Vec<Symbol*>* d1, Vec<Symbol*>* d2) {
  forv_Vec(Symbol, d, *d1)
    if (!d2->in(d))
      return false;
  forv_Vec(Symbol, d, *d2)
    if (!d1->in(d))
      return false;
  return true;
}

static FnSymbol*
check_dwcache(FnSymbol* fn, Vec<Symbol*>* defaults) {
  forv_Vec(DWCacheItem, item, dwcache)
    if (item->oldFn == fn && dw_match(defaults, item->defaults))
      return item->newFn;
  return NULL;
}

static void
add_dwcache(FnSymbol* newFn, FnSymbol* oldFn, Vec<Symbol*>* defaults) {
  dwcache.add(new DWCacheItem(oldFn, newFn, defaults));
}
/*** Default Wrapper Cache ^^^ ***/


Symbol::Symbol(astType_t astType, char* init_name, Type* init_type) :
  BaseAST(astType),
  name(init_name),
  cname(name),
  type(init_type),
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
Symbol::copyInner(ASTMap* map) {
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
  return false;
}


bool Symbol::isParam(){
  return false;
}


bool Symbol::isThis(void) {
  FnSymbol *f = dynamic_cast<FnSymbol*>(defPoint->parentSymbol);
  if (!f || !f->_this)
    return 0;
  else
    return f->_this == this;
}


void Symbol::print(FILE* outfile) {
  fprintf(outfile, "%s", name);
}


void Symbol::codegen(FILE* outfile) {
  if (hasPragma("data class")) {
    TypeSymbol* typeSymbol = dynamic_cast<TypeSymbol*>(this);
    ClassType* dataType = dynamic_cast<ClassType*>(typeSymbol->definition);
    dataType->fields.v[1]->type->codegen(outfile);
    fprintf(outfile, "*");
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
  if (prev || next) {
    INT_FATAL(this, "Symbol is in AList");
  }
}


void UnresolvedSymbol::codegen(FILE* outfile) {
  INT_FATAL(this, "ERROR:  Cannot codegen an unresolved symbol.");
  fprintf(outfile, "%s /* unresolved */ ", name);
}


UnresolvedSymbol*
UnresolvedSymbol::copyInner(ASTMap* map) {
  return new UnresolvedSymbol(stringcpy(name));
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
  immediate(NULL),
  literalType(NULL),
  noDefaultInit(false)
{ }


void VarSymbol::verify(void) {
  if (astType != SYMBOL_VAR) {
    INT_FATAL(this, "Bad VarSymbol::astType");
  }
  if (prev || next) {
    INT_FATAL(this, "Symbol is in AList");
  }
}


VarSymbol*
VarSymbol::copyInner(ASTMap* map) {
  VarSymbol* newVarSymbol = 
    new VarSymbol(stringcpy(name), type, varClass, consClass);
  newVarSymbol->cname = stringcpy(cname);
  assert(!newVarSymbol->immediate);
  assert(!newVarSymbol->literalType);
  newVarSymbol->noDefaultInit = noDefaultInit;
  return newVarSymbol;
}


void VarSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  type->replaceChild(old_ast, new_ast);
}


void VarSymbol::traverseDefSymbol(Traversal* traversal) {
  TRAVERSE(type, traversal, false);
  TRAVERSE(literalType, traversal, false);
}


bool VarSymbol::isConst(void) {
  return (consClass == VAR_CONST);
}


bool VarSymbol::isParam(void){
  return (consClass == VAR_PARAM) || immediate;
}


void VarSymbol::print(FILE* outfile) {
  if (immediate) {
    bool isString = (immediate->const_kind == IF1_CONST_KIND_STRING);
    if (isString) {
      if (CreateConfigVarTable::running) {
        fprintf(outfile, "\\\"");
      } else {
        fprintf(outfile, "\"");
      }
    }
    fprint_imm(outfile, *immediate);
    if (isString) {
      if (CreateConfigVarTable::running) {
        fprintf(outfile, "\\\"");
      } else {
        fprintf(outfile, "\"");
      }
    }
  } else
    Symbol::print(outfile);
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


void VarSymbol::codegenDef(FILE* outfile) {
  // need to ensure that this can be realized in C as a const, and
  // move its initializer here if it can be
  if (0 && (consClass == VAR_CONST)) {
    fprintf(outfile, "const ");
  }
  type->codegen(outfile);
  fprintf(outfile, " ");
  this->codegen(outfile);
  fprintf(outfile, ";\n");
}


static char* intentTagNames[NUM_INTENT_TYPES] = {
  "",
  "in",
  "inout",
  "out",
  "const",
  "ref",
  "param",
  "type"
};


ArgSymbol::ArgSymbol(intentTag iIntent, char* iName, 
                     Type* iType, Expr* iDefaultExpr) :
  Symbol(SYMBOL_ARG, iName, iType),
  intent(iIntent),
  defaultExpr(iDefaultExpr),
  genericSymbol(NULL),
  isGeneric(false),
  isExactMatch(false)
{
  if (intent == INTENT_PARAM || intent == INTENT_TYPE)
    isGeneric = true;
}


void ArgSymbol::verify(void) {
  if (astType != SYMBOL_ARG) {
    INT_FATAL(this, "Bad ArgSymbol::astType");
  }
  if (prev || next) {
    INT_FATAL(this, "Symbol is in AList");
  }
}


ArgSymbol*
ArgSymbol::copyInner(ASTMap* map) {
  ArgSymbol *ps = new ArgSymbol(intent, stringcpy(name),
                                type, COPY_INT(defaultExpr));
  if (genericSymbol)
    ps->genericSymbol = genericSymbol;
  ps->isGeneric = isGeneric;
  ps->isExactMatch = isExactMatch;
  ps->cname = stringcpy(cname);
  return ps;
}


void ArgSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == defaultExpr)
    defaultExpr = dynamic_cast<Expr*>(new_ast);
  else
    type->replaceChild(old_ast, new_ast);
}


void ArgSymbol::traverseDefSymbol(Traversal* traversal) {
  TRAVERSE(defaultExpr, traversal, false);
  TRAVERSE(type, traversal, false);
  TRAVERSE(genericSymbol, traversal, false);
}


void ArgSymbol::printDef(FILE* outfile) {
  fprintf(outfile, "%s ", intentTagNames[intent]);
  Symbol::print(outfile);
  fprintf(outfile, ": ");
  type->print(outfile);
}


bool ArgSymbol::requiresCPtr(void) {
  return intent == INTENT_OUT || intent == INTENT_INOUT ||
    (intent == INTENT_REF && type->astType == TYPE_PRIMITIVE);
}


bool ArgSymbol::requiresCopyBack(void) {
  return intent == INTENT_OUT || intent == INTENT_INOUT;
}


bool ArgSymbol::requiresCTmp(void) {
  return type->requiresCParamTmp(intent);
}


bool ArgSymbol::isConst(void) {
  return (intent == INTENT_CONST || intent == INTENT_BLANK); 
}


bool ArgSymbol::isRef(void) {
  if (intent == INTENT_REF)
    return true;
  if (is_Reference_Type(type))
    return true;
  return false;
}


bool ArgSymbol::isCopyOut(void) {
  return intent == INTENT_INOUT || intent == INTENT_OUT;
}


void ArgSymbol::codegen(FILE* outfile) {
  bool requiresDeref = requiresCPtr();
 
  if (requiresDeref) {
    fprintf(outfile, "(*");
  }
  Symbol::codegen(outfile);
  if (requiresDeref) {
    fprintf(outfile, ")");
  }
}


void ArgSymbol::codegenDef(FILE* outfile) {
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
  if (!definition)
    isUnresolved = true;
  else
    definition->addSymbol(this);
}


void TypeSymbol::verify(void) {
  if (astType != SYMBOL_TYPE) {
    INT_FATAL(this, "Bad TypeSymbol::astType");
  }
  if (prev || next) {
    INT_FATAL(this, "Symbol is in AList");
  }
  definition->verify();
}


TypeSymbol*
TypeSymbol::copyInner(ASTMap* map) {
  Type* new_definition = COPY_INT(definition);
  TypeSymbol* new_definition_symbol = new TypeSymbol(stringcpy(name), new_definition);
  new_definition->addSymbol(new_definition_symbol);
  new_definition_symbol->cname = stringcpy(cname);
  return new_definition_symbol;
}


TypeSymbol* TypeSymbol::clone(ASTMap* map) {
  ClassType* originalClass = dynamic_cast<ClassType*>(definition);
  if (!originalClass) {
    INT_FATAL(this, "Attempt to clone non-class type");
  }
  TypeSymbol* clone = copy(map);
  ClassType* newClass = dynamic_cast<ClassType*>(clone->definition);
  if (!newClass) {
    INT_FATAL(this, "Class cloning went horribly wrong");
  }
  clone->cname = stringcat("_clone_", clone->cname);
  defPoint->parentStmt->insertBefore(new ExprStmt(new DefExpr(clone)));
  clone->addPragmas(&pragmas);
  if (no_infer)
    newClass->dispatchParents.copy(originalClass->dispatchParents);
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
  definition->codegenPrototype(outfile);
}


void TypeSymbol::codegenDef(FILE* outfile) {
  definition->codegenDef(outfile);
}


FnSymbol::FnSymbol(char* initName,
                   TypeSymbol* initTypeBinding,
                   AList<DefExpr>* initFormals,
                   Type* initRetType,
                   Expr* initWhereExpr,
                   BlockStmt* initBody,
                   fnType initFnClass,
                   bool initNoParens,
                   bool initRetRef) :
  Symbol(SYMBOL_FN, initName, new FnType()),
  typeBinding(initTypeBinding),
  formals(initFormals),
  retType(initRetType),
  whereExpr(initWhereExpr),
  body(initBody),
  fnClass(initFnClass),
  noParens(initNoParens),
  retRef(initRetRef),
  argScope(NULL),
  isInitFn(false),
  isSetter(false),
  isGeneric(false),
  _this(NULL),
  _setter(NULL),
  _getter(NULL),
  method_type(NON_METHOD),
  instantiatedFrom(NULL),
  basicBlocks(NULL),
  calledBy(NULL),
  calls(NULL)
{
  if (!body)
    body = new BlockStmt();
}


void FnSymbol::verify(void) {
  if (astType != SYMBOL_FN) {
    INT_FATAL(this, "Bad FnSymbol::astType");
  }
  if (prev || next) {
    INT_FATAL(this, "Symbol is in AList");
  }
}


FnSymbol* FnSymbol::getFnSymbol(void) {
  return this;
}


FnSymbol*
FnSymbol::copyInner(ASTMap* map) {
  FnSymbol* copy = new FnSymbol(name,
                                typeBinding,
                                COPY_INT(formals),
                                retType,
                                COPY_INT(whereExpr),
                                COPY_INT(body),
                                fnClass,
                                noParens,
                                retRef);
  copy->cname = cname;
  copy->isSetter = isSetter;
  copy->isGeneric = isGeneric;
  copy->_this = _this;
  copy->_setter = _setter;
  copy->_getter = _getter; // If it is a cloned class we probably want
                           // this to point to the new member, but how
                           // do we do that
  copy->method_type = method_type;
  return copy;
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

  if (argScope) {
    saveScope = Symboltable::setCurrentScope(argScope);
  }
  TRAVERSE(formals, traversal, false);
  TRAVERSE(type, traversal, false);
  TRAVERSE(body, traversal, false);
  TRAVERSE(retType, traversal, false);
  TRAVERSE(whereExpr, traversal, false);
  if (argScope) {
    Symboltable::setCurrentScope(saveScope);
  }
}


static bool function_returns_void(FnSymbol* fn) {
  if (fn->retType == dtVoid)
    return true;

  if (fn->retType != dtUnknown || fn->defPoint->exprType)
    return false;

  Vec<BaseAST*> asts;
  collect_asts(&asts, fn);
  forv_Vec(BaseAST, ast, asts) {
    if (ReturnStmt* returnStmt = dynamic_cast<ReturnStmt*>(ast)) {
      return !returnStmt->expr;
    }
  }

  return true;
}


FnSymbol* FnSymbol::clone(ASTMap* map) {
  Stmt* copyStmt = defPoint->parentStmt->copy(map, NULL);
  ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(copyStmt);
  DefExpr* defExpr = dynamic_cast<DefExpr*>(exprStmt->expr);
  // BLC: we don't want to rename any clones that we build in the
  // prelude, because they likely refer to external functions for
  // which clones will not be built
  if (defPoint->parentScope->type != SCOPE_PRELUDE) {
    defExpr->sym->cname = stringcat("_clone_", cname);
  }
  defPoint->parentStmt->insertAfter(copyStmt);
  clear_type_info(copyStmt);
  clear_type_info(defPoint);
  defExpr->sym->addPragmas(&pragmas);
  return dynamic_cast<FnSymbol*>(defExpr->sym);
}


FnSymbol* FnSymbol::coercion_wrapper(Map<Symbol*,Symbol*>* coercion_substitutions) {
  AList<DefExpr>* wrapper_formals = new AList<DefExpr>();
  AList<Expr>* wrapper_actuals = new AList<Expr>();
  BlockStmt* wrapper_body = new BlockStmt();
  for_alist(DefExpr, formal, formals) {
    Symbol* newFormal = formal->sym->copy();
    wrapper_formals->insertAtTail(new DefExpr(newFormal));
    Symbol* coercionSubstitution = coercion_substitutions->get(formal->sym);
    if (TypeSymbol *ts = dynamic_cast<TypeSymbol*>(coercionSubstitution)) {
      newFormal->type = ts->definition;
      char* tempName = stringcat("_coercion_temp_", formal->sym->name);
      VarSymbol* temp = new VarSymbol(tempName, formal->sym->type);
      DefExpr* tempDefExpr = new DefExpr(temp, new SymExpr(newFormal));
      wrapper_body->insertAtTail(new ExprStmt(tempDefExpr));
      wrapper_actuals->insertAtTail(new SymExpr(temp));
    } else {
      wrapper_actuals->insertAtTail(new SymExpr(newFormal));
    }
  }

  Expr* fn_call = new CallExpr(this, wrapper_actuals);
  if (function_returns_void(this)) {
    wrapper_body->insertAtTail(new ExprStmt(fn_call));
  } else {
    wrapper_body->insertAtTail(new ReturnStmt(fn_call));
  }

  FnSymbol* wrapper_fn = new FnSymbol(name, typeBinding, wrapper_formals,
                                      retType, NULL, wrapper_body,
                                      fnClass, noParens, retRef);
  wrapper_fn->method_type = method_type;
  wrapper_fn->cname = stringcat("_coerce_wrap_", cname);
  wrapper_fn->addPragma("inline");
  defPoint->parentStmt->insertAfter(new ExprStmt(new DefExpr(wrapper_fn)));
  wrapper_fn->addPragmas(&pragmas);
  reset_file_info(wrapper_fn->defPoint->parentStmt, lineno, filename);
  return wrapper_fn;
}


FnSymbol* FnSymbol::default_wrapper(Vec<Symbol*>* defaults) {
  if (FnSymbol* cached = check_dwcache(this, defaults))
    return cached;

  AList<DefExpr>* wrapper_formals = new AList<DefExpr>();
  AList<Expr>* wrapper_actuals = new AList<Expr>();
  BlockStmt* wrapper_body = new BlockStmt();
  ASTMap map;
  for_alist(DefExpr, formalDef, formals) {
    ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
    if (!defaults->set_in(formal)) {
      Symbol* newFormal = formal->copy();
      map.put(formal, newFormal);
      wrapper_formals->insertAtTail(new DefExpr(newFormal));
      wrapper_actuals->insertAtTail(new SymExpr(newFormal));
    } else {
      char* temp_name = stringcat("_default_temp_", formal->name);
      VarSymbol* temp = new VarSymbol(temp_name, formal->type);
      map.put(formal, temp);
      Expr* temp_init = NULL;
      Expr* temp_type = NULL;
      if (formal->intent != INTENT_OUT)
        temp_init = formal->defaultExpr->copy();
      if (no_infer && formalDef->exprType)
        temp_type = formalDef->exprType->copy();
      wrapper_body->insertAtTail(new ExprStmt(new DefExpr(temp, temp_init, temp_type)));

      if (formal->type != dtUnknown &&
          formal->intent != INTENT_OUT &&
          formal->intent != INTENT_INOUT)
        wrapper_actuals->insertAtTail(new CastExpr(new SymExpr(temp), NULL, formal->type));
      else
        wrapper_actuals->insertAtTail(new SymExpr(temp));
    }
  }
  update_symbols(wrapper_body, &map);

  wrapper_body->insertAtTail(
    function_returns_void(this)
      ? new ExprStmt(new CallExpr(this, wrapper_actuals))
      : new ReturnStmt(new CallExpr(this, wrapper_actuals)));

  FnSymbol* wrapper_fn = new FnSymbol(name, typeBinding, wrapper_formals,
                                      retType, NULL, wrapper_body,
                                      fnClass, noParens, retRef);
  wrapper_fn->method_type = method_type;
  wrapper_fn->cname = stringcat("_default_wrap_", cname);
  wrapper_fn->addPragma("inline");
  defPoint->parentStmt->insertAfter(new ExprStmt(new DefExpr(wrapper_fn)));
  wrapper_fn->addPragmas(&pragmas);
  reset_file_info(wrapper_fn->defPoint->parentStmt, lineno, filename);
  add_dwcache(wrapper_fn, this, defaults);
  return wrapper_fn;
}


FnSymbol* FnSymbol::order_wrapper(Map<Symbol*,Symbol*>* formals_to_formals) {
  Map<Symbol*,Symbol*> old_to_new;
  for_alist(DefExpr, formal, formals) {
    old_to_new.put(formal->sym, formal->sym->copy());
  }

  AList<DefExpr>* wrapper_formals = new AList<DefExpr>();
  AList<Expr>* wrapper_actuals = new AList<Expr>();
  for_alist(DefExpr, formal, formals) {
    wrapper_formals->insertAtTail(new DefExpr(old_to_new.get(formals_to_formals->get(formal->sym))));
    wrapper_actuals->insertAtTail(new SymExpr(old_to_new.get(formal->sym)));
  }

  Stmt* stmt = (function_returns_void(this))
    ? new ExprStmt(new CallExpr(this, wrapper_actuals))
    : new ReturnStmt(new CallExpr(this, wrapper_actuals));

  FnSymbol* wrapper_fn = new FnSymbol(name, typeBinding, wrapper_formals,
                                      retType, NULL, new BlockStmt(stmt),
                                      fnClass, noParens, retRef);
  wrapper_fn->method_type = method_type;
  wrapper_fn->cname = stringcat("_order_wrap_", cname);
  wrapper_fn->addPragma("inline");
  defPoint->parentStmt->insertBefore(new ExprStmt(new DefExpr(wrapper_fn)));
  reset_file_info(wrapper_fn->defPoint->parentStmt, lineno, filename);
  return wrapper_fn;
}


//  iterator _forall() : (integer, integer)
//    forall i in _forall(1)
//      forall j in _forall(2)
//        yield (i, j);
static FnSymbol*
buildMultidimensionalIterator(ClassType* type, int rank) {

  FnSymbol* _forall = new FnSymbol("_forall", type->symbol, NULL, dtUnknown);
  _forall->fnClass = FN_ITERATOR;
  ArgSymbol* _this = new ArgSymbol(INTENT_REF, "this", type);
  _forall->_this = _this;
  _forall->method_type = PRIMARY_METHOD;
  _forall->formals = new AList<DefExpr>(new DefExpr(_this));

  _forall->formals->insertAtHead(new DefExpr(new ArgSymbol(INTENT_REF, "_methodTokenDummy", dtMethodToken)));

  _forall->body = new BlockStmt();

  VarSymbol* _seq_result = new VarSymbol("_seq_result");
  _forall->insertAtTail(new ExprStmt(new DefExpr(_seq_result)));
  AList<Expr>* args = new AList<Expr>();
  for (int i = 1; i <= rank; i++) {
    args->insertAtTail(new SymExpr(dtInteger->symbol));
  }
  if (rank > 1)
    _seq_result->defPoint->exprType = new CallExpr("_construct_seq", new CallExpr(stringcat("_construct__tuple", intstring(rank)), args));
  else
    _seq_result->defPoint->exprType = new CallExpr("_construct_seq", args);

  CallExpr* yield = new CallExpr(stringcat("_construct__tuple", intstring(rank)));
  CallExpr* partial = new CallExpr("_yield", methodToken, _seq_result);
  partial->partialTag = PARTIAL_OK;
  Stmt* loop = new ExprStmt(new CallExpr(partial, yield));
  for (int i = 1; i <= rank; i++) {
    VarSymbol* index = new VarSymbol(stringcat("_i", intstring(i)), dtInteger);
    CallExpr* partial = new CallExpr("_forall", methodToken, _this);
    partial->partialTag = PARTIAL_OK;
    CallExpr *inner = 0;
    loop = new ForLoopStmt(FORLOOPSTMT_FORALL,
             new AList<DefExpr>(new DefExpr(index)),
             new AList<Expr>(
               new CallExpr(
               (inner = new CallExpr("_forall", methodToken,
                 new CallExpr(partial, new_IntLiteral(i)))))),
             loop);
    inner->partialTag = PARTIAL_OK;
    if (rank > 1) {
      yield->argList->insertAtHead(new SymExpr(dtInteger->symbol));
      yield->argList->insertAtTail(new NamedExpr(stringcat("_f", intstring(i)), new SymExpr(index)));
    } else {
      yield->replace(new SymExpr(index));
    }
  }

  _forall->insertAtTail(loop);

  _forall->insertAtTail(new ReturnStmt(_seq_result));

  type->symbol->defPoint->parentStmt->insertBefore
    (new ExprStmt(new DefExpr(_forall))); //, NULL, _seq_result->defPoint->exprType->copy())));
  return _forall;
}


static void
instantiate_update_expr(ASTMap* substitutions, Expr* expr) {
  ASTMap map;
  map.copy(*substitutions);
  // for type variables, add TypeSymbols into the map as well
  for (int i = 0; i < substitutions->n; i++)
    if (Type *t = dynamic_cast<Type*>(substitutions->v[i].key))
      if (Type *tt = dynamic_cast<Type*>(substitutions->v[i].value))
        map.put(t->symbol, tt->symbol);
  update_symbols(expr, &map);
}


static void
instantiate_add_subs(ASTMap* substitutions, ASTMap* map) {
  ASTMap tmp;
  tmp.copy(*substitutions);
  for (int i = 0; i < tmp.n; i++) {
    if (tmp.v[i].key) {
      if (BaseAST *b = map->get(tmp.v[i].key))
        substitutions->put(b, tmp.v[i].value);
    }
  }
}


static FnSymbol*
instantiate_function(FnSymbol *fn, ASTMap *all_subs, ASTMap *generic_subs, ASTMap *map) {
  Stmt* fnStmt = fn->defPoint->parentStmt->copy(map);
  ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(fnStmt);
  DefExpr* defExpr = dynamic_cast<DefExpr*>(exprStmt->expr);
  defExpr->sym->cname = stringcat("_inst_", defExpr->sym->cname);
  fn->defPoint->parentStmt->insertBefore(fnStmt);
  instantiate_add_subs(all_subs, map);
  instantiate_update_expr(all_subs, defExpr);
  FnSymbol* fnClone = dynamic_cast<FnSymbol*>(defExpr->sym);
  fnClone->instantiatedFrom = fn;
  fnClone->substitutions.copy(*generic_subs);
  tagGenerics(fnClone);
  for_alist(DefExpr, formal, fnClone->formals) {
    if (ArgSymbol *ps = dynamic_cast<ArgSymbol *>(formal->sym)) {
      if (TypeSymbol *ts = dynamic_cast<TypeSymbol *>(ps->genericSymbol)) {
        if (ts->definition->astType != TYPE_VARIABLE) {
          ps->type = ts->type;
          ps->isExactMatch = true;
        }
      }
      if (all_subs->get(ps) && ps->intent == INTENT_PARAM) {
        ps->intent = INTENT_BLANK;
        ps->isGeneric = false;
        if (!no_infer) {
          if (VarSymbol *vs = dynamic_cast<VarSymbol*>(all_subs->get(ps))) {
            if (vs->literalType)
              ps->type = vs->literalType;
          }
        }
      }
    }
  }
  return fnClone;
}


FnSymbol*
FnSymbol::clone_generic(ASTMap* formal_types) {
  // use same cache as instantiate generic since they won't collide
  if (FnSymbol* cached = check_icache(this, formal_types))
    return cached;
  Stmt* fnStmt = defPoint->parentStmt->copy();
  ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(fnStmt);
  DefExpr* defExpr = dynamic_cast<DefExpr*>(exprStmt->expr);
  FnSymbol* fnClone = dynamic_cast<FnSymbol*>(defExpr->sym);
  fnClone->cname = stringcat("_clone_", fnClone->cname);
  defPoint->parentStmt->insertBefore(fnStmt);
  DefExpr* oldFormalDef = formals->first();
  for_alist(DefExpr, formalDef, fnClone->formals) {
    ArgSymbol* oldFormal = dynamic_cast<ArgSymbol*>(oldFormalDef->sym);
    ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
    if (Type* type = dynamic_cast<Type*>(formal_types->get(oldFormal))) {
      formal->type = type;
      formalDef->exprType = NULL;
    }
    oldFormalDef = formals->next();
  }
  fnClone->instantiatedFrom = this;
  fnClone->substitutions.copy(*formal_types);
  add_icache(fnClone);
  return fnClone;
}


static bool
function_requires_instantiation(FnSymbol* fn, Type* type) {
  if (fn->retType == type)
    return true;
  for_alist(DefExpr, formalDef, fn->formals) {
    ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
    if (formal->type == type)
      return true;
  }
  return false;
}


FnSymbol*
FnSymbol::instantiate_generic(ASTMap* generic_substitutions,
                              Vec<FnSymbol*>* new_functions,
                              Vec<TypeSymbol*>* new_types) {
  if (FnSymbol* cached = check_icache(this, generic_substitutions))
    return cached;
  ASTMap substitutions(*generic_substitutions);
  FnSymbol* newfn = NULL;
  currentLineno = lineno;
  currentFilename = filename;
  ASTMap map;
  if (fnClass == FN_CONSTRUCTOR) {
    TypeSymbol* clone = retType->symbol->clone(&map);
    new_types->add(clone);
    instantiate_add_subs(&substitutions, &map);

    ClassType* cloneType = dynamic_cast<ClassType*>(clone->definition);
    cloneType->instantiatedFrom = retType;

    Vec<TypeSymbol *> types;
    types.move(cloneType->types);
    for (int i = 0; i < types.n; i++) {
      if (types.v[i] && substitutions.get(types.v[i]->definition)) {
        types.v[i]->defPoint->parentStmt->remove();
      } else
        cloneType->types.add(types.v[i]);
    }

    instantiate_update_expr(&substitutions, clone->defPoint);
    substitutions.put(retType, clone->definition);

    cloneType->substitutions.copy(*generic_substitutions);
    tagGenerics(cloneType);

    Vec<BaseAST*> genericParameters;
    for (int i = 0; i < substitutions.n; i++)
      if (VariableType *t = dynamic_cast<VariableType*>(substitutions.v[i].key)) {
        genericParameters.set_add(t);
        genericParameters.set_add(t->symbol);
      } else if (ArgSymbol *s = dynamic_cast<ArgSymbol*>(substitutions.v[i].key))
        if (s->isGeneric)
          genericParameters.set_add(s);
    genericParameters.set_add(retType);
    genericParameters.set_add(retType->symbol);

    Vec<FnSymbol*> fns;
    collect_functions(&fns);
    forv_Vec(FnSymbol, fn, fns) {
      if (function_requires_instantiation(fn, retType)) {
        FnSymbol *fnClone = instantiate_function(fn, &substitutions, generic_substitutions, &map);
        new_functions->add(fnClone);
        if (fn == this) {
          newfn = fnClone;
        }
        if (fn->typeBinding == retType->symbol) {
          cloneType->methods.add(fnClone);
          fnClone->typeBinding = clone;
          fnClone->method_type = fn->method_type;
        }
        if (retType->defaultConstructor == fn)
          cloneType->defaultConstructor = fnClone;
      }
    }

    if (clone->hasPragma("instantiate multidimensional iterator")) {
      Symbol* rank_sym = Symboltable::lookupInScope("rank", argScope);
      BaseAST* rank_ast = generic_substitutions->get(rank_sym);
      int rank = (int)(dynamic_cast<VarSymbol*>(rank_ast)->immediate->v_int64);
      new_functions->add(buildMultidimensionalIterator(cloneType, rank));
    }

  } else {
    newfn = instantiate_function(this, &substitutions, generic_substitutions, &map);
    new_functions->add(newfn);
  }
  if (!newfn) {
    INT_FATAL(this, "Instantiation error");
  }
  add_icache(newfn);
  return newfn;
}


void FnSymbol::printDef(FILE* outfile) {
  fprintf(outfile, "function ");
  print(outfile);
  fprintf(outfile, "(");
  if (formals) {
    formals->print(outfile, ", ");
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
  if (retType == dtUnknown) {
    retType = return_type_info(this);
    INT_WARNING(this, "return type unknown, calling analysis late");
  }
  retType->codegen(outfile);
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
  if (hasPragma("no codegen"))
    return;
  codegenHeader(outfile);
  fprintf(outfile, ";\n");
}


void FnSymbol::codegenDef(FILE* outfile) {
  if (hasPragma("no codegen"))
    return;

  if (fnClass == FN_CONSTRUCTOR) {
    fprintf(outfile, "/* constructor */\n");
  }

  codegenHeader(outfile);
  fprintf(outfile, " ");

  // while these braces seem like they should be extraneous since
  // all function bodies are BlockStmts, it turns out that they are
  // not because in C the function's parameter scope is the same as
  // its local variable scope; so if we have a parameter and a local
  // variable of name x (as in trivial/bradc/vardecls1b.chpl), these
  // extra braces are required to make the generated code work out.
  // SJD: extra braces no longer necessary because of mangling
  // SJD: Why should we not make the same scope restriction??
  justStartedGeneratingFunction = true;
  body->codegen(outfile);
  fprintf(outfile, "\n\n");
}


void
FnSymbol::insertAtHead(Stmt* stmt) {
  body->insertAtHead(stmt);
}


void
FnSymbol::insertAtTail(Stmt* stmt) {
  body->insertAtTail(stmt);
}


void
FnSymbol::insertAtHead(AList<Stmt>* stmt) {
  body->insertAtHead(stmt);
}


void
FnSymbol::insertAtTail(AList<Stmt>* stmt) {
  body->insertAtTail(stmt);
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
}


void EnumSymbol::verify(void) {
  if (astType != SYMBOL_ENUM) {
    INT_FATAL(this, "Bad EnumSymbol::astType");
  }
  if (prev || next) {
    INT_FATAL(this, "Symbol is in AList");
  }
}


EnumSymbol*
EnumSymbol::copyInner(ASTMap* map) {
  return new EnumSymbol(stringcpy(name));
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
  if (prev || next) {
    INT_FATAL(this, "Symbol is in AList");
  }
}


ModuleSymbol*
ModuleSymbol::copyInner(ASTMap* map) {
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


bool ModuleSymbol::isFileModule(void) {
  return (lineno == 0);
}


int ModuleSymbol::numUserModules(Vec<ModuleSymbol*>* moduleList) {
  int numUserModules = 0;
  forv_Vec(ModuleSymbol, mod, *moduleList) {
    if (mod->modtype == MOD_USER) {
      numUserModules++;
    }
  }

  return numUserModules;
}


LabelSymbol::LabelSymbol(char* init_name) :
  Symbol(SYMBOL_LABEL, init_name, NULL)
{ 
 
}


void LabelSymbol::verify(void) {
  if (astType != SYMBOL_LABEL) {
    INT_FATAL(this, "Bad LabelSymbol::astType");
  }
  if (prev || next) {
    INT_FATAL(this, "Symbol is in AList");
  }
}

LabelSymbol* 
LabelSymbol::copyInner(ASTMap* map) {
  LabelSymbol* copy = new LabelSymbol(stringcpy(name));
  copy->cname = stringcpy(cname);
  return copy;
}

void LabelSymbol::codegenDef(FILE* outfile) { }

void
initSymbol() {
  gNil = new VarSymbol("nil", dtNil, VAR_NORMAL, VAR_CONST);
  rootScope->define(gNil); // SJD: Should intrinsics have DefExprs?
  gUnspecified = new VarSymbol("_", dtUnknown, VAR_NORMAL, VAR_CONST);
  rootScope->define(gUnspecified); // SJD: Should intrinsics have DefExprs?
}

static int literal_id = 1;
HashMap<Immediate *, ImmHashFns, VarSymbol *> uniqueConstantsHash;

VarSymbol *new_StringSymbol(char *str) {
  Immediate imm;
  imm.const_kind = IF1_CONST_KIND_STRING;
  imm.v_string = cannonicalize_string(str);
  VarSymbol *s = uniqueConstantsHash.get(&imm);
  if (s)
    return s;
  s = new VarSymbol(stringcat("_literal_", intstring(literal_id++)), dtString);
  int l = strlen(str);
  char *n = (char*)MALLOC(l + 3);
  strcpy(n + 1, str);
  n[0]='\"';
  n[l+1]='\"';
  n[l+2]=0;
  s->cname = n;
  s->immediate = new Immediate;
  *s->immediate = imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}

VarSymbol *new_BoolSymbol(bool b) {
  Immediate imm;
  imm.v_bool = b;
  imm.const_kind = IF1_NUM_KIND_UINT;
  imm.num_index = IF1_INT_TYPE_1;
  VarSymbol *s = uniqueConstantsHash.get(&imm);
  if (s)
    return s;
  s = new VarSymbol(stringcat("_literal_", intstring(literal_id++)), dtBoolean);
  if (b)
    s->cname = "true";
  else
    s->cname = "false";
  s->immediate = new Immediate;
  *s->immediate = imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}

VarSymbol *new_IntSymbol(long b) {
  Immediate imm;
  imm.v_int64 = b;
  imm.const_kind = IF1_NUM_KIND_INT;
  imm.num_index = IF1_INT_TYPE_64;
  VarSymbol *s = uniqueConstantsHash.get(&imm);
  if (s)
    return s;
  s = new VarSymbol(stringcat("_literal_", intstring(literal_id++)), dtInteger);
  char n[80];
  sprintf(n, "%ld", b);
  s->cname = dupstr(n);
  s->immediate = new Immediate;
  *s->immediate = imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}

VarSymbol *new_FloatSymbol(char *n, double b) {
  (void)n;
  Immediate imm;
  imm.v_float64 = b;
  imm.const_kind = IF1_NUM_KIND_FLOAT;
  imm.num_index = IF1_FLOAT_TYPE_64;
  VarSymbol *s = uniqueConstantsHash.get(&imm);
  if (s)
    return s;
  s = new VarSymbol(stringcat("_literal_", intstring(literal_id++)), dtFloat);
  s->immediate = new Immediate;
  s->cname = dupstr(n);
  *s->immediate = imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}

VarSymbol *new_ComplexSymbol(char *n, double r, double i) {
  (void)n;
  Immediate imm;
  imm.v_complex64.r = r;
  imm.v_complex64.i = i;
  imm.const_kind = IF1_NUM_KIND_COMPLEX;
  imm.num_index = IF1_FLOAT_TYPE_64;
  VarSymbol *s = uniqueConstantsHash.get(&imm);
  if (s)
    return s;
  s = new VarSymbol(stringcat("_literal_", intstring(literal_id++)), dtComplex);
  s->immediate = new Immediate;
  s->cname = dupstr(n);
  *s->immediate = imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}
