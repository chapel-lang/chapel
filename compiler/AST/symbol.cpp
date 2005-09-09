#include <typeinfo>
#include "if1.h"
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
#include "../traversals/instantiate.h"
#include "../traversals/buildClassHierarchy.h"
#include "../symtab/collectFunctions.h"
#include "../traversals/findTypeVariables.h"
#include "../passes/preAnalysisCleanup.h"

Symbol *gNil = 0;
Symbol *gUnspecified = 0;

#define DUPLICATE_INSTANTIATION_CACHE 1

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
    dataType->fields.v[1]->type->codegen(outfile);
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
  if (prev || next) {
    INT_FATAL(this, "Symbol is in AList");
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
  immediate(NULL),
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


void VarSymbol::print(FILE* outfile) {
  if (immediate)
    print_imm(outfile, *immediate);
  else
    fprintf(outfile, "%s", name);
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
  case SCOPE_LOCAL:
  case SCOPE_MODULE:
    return true;
  case SCOPE_INTRINSIC:
  case SCOPE_PRELUDE:
  case SCOPE_POSTPARSE:
  case SCOPE_ARG:
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


ArgSymbol::ArgSymbol(intentTag init_intent, char* init_name, 
                         Type* init_type) :
  Symbol(SYMBOL_ARG, init_name, init_type),
  intent(init_intent),
  variableTypeSymbol(NULL),
  isGeneric(false)
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
ArgSymbol::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  ArgSymbol *ps = new ArgSymbol(intent, copystring(name), type);
  if (variableTypeSymbol)
    ps->variableTypeSymbol = variableTypeSymbol;
  ps->isGeneric = isGeneric;
  ps->cname = copystring(cname);
  return ps;
}


void ArgSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  type->replaceChild(old_ast, new_ast);
}


void ArgSymbol::traverseDefSymbol(Traversal* traversal) {
  TRAVERSE(type, traversal, false);
  TRAVERSE(variableTypeSymbol, traversal, false);
}


void ArgSymbol::printDef(FILE* outfile) {
  fprintf(outfile, "%s ", intentTagNames[intent]);
  Symbol::print(outfile);
  fprintf(outfile, ": ");
  type->print(outfile);
}


bool ArgSymbol::requiresCPtr(void) {
  return (intent == INTENT_OUT || intent == INTENT_INOUT || 
          (intent == INTENT_REF && type->astType == TYPE_PRIMITIVE));
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
TypeSymbol::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  Type* new_definition = COPY_INTERNAL(definition);
  TypeSymbol* new_definition_symbol = new TypeSymbol(copystring(name), new_definition);
  new_definition->addSymbol(new_definition_symbol);
  new_definition_symbol->cname = copystring(cname);
  return new_definition_symbol;
}


TypeSymbol* TypeSymbol::clone(Map<BaseAST*,BaseAST*>* map) {
  static int uid = 1;
  ClassType* originalClass = dynamic_cast<ClassType*>(definition);
  if (!originalClass) {
    INT_FATAL(this, "Attempt to clone non-class type");
  }
  TypeSymbol* clone = copy(true, map);
  ClassType* newClass = dynamic_cast<ClassType*>(clone->definition);
  if (!newClass) {
    INT_FATAL(this, "Class cloning went horribly wrong");
  }
  clone->cname = glomstrings(3, clone->cname, "_clone_", intstring(uid++));
  defPoint->parentStmt->insertBefore(new ExprStmt(new DefExpr(clone)));
  clone->copyPragmas(pragmas);
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
  isSetter(false),
  isGeneric(false),
  _this(NULL),
  _setter(NULL),
  _getter(NULL),
  method_type(NON_METHOD),
  instantiatedFrom(NULL)
{ }


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
FnSymbol::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  FnSymbol* copy = new FnSymbol(name,
                                typeBinding,
                                CLONE_INTERNAL(formals),
                                retType,
                                CLONE_INTERNAL(whereExpr),
                                CLONE_INTERNAL(body),
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


FnSymbol* FnSymbol::clone(Map<BaseAST*,BaseAST*>* map) {
  static int uid = 1;
  Stmt* copyStmt = defPoint->parentStmt->copy(true, map, NULL);
  ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(copyStmt);
  DefExpr* defExpr = dynamic_cast<DefExpr*>(exprStmt->expr);
  defExpr->sym->cname = glomstrings(3, cname, "_clone_", intstring(uid++));
  defPoint->parentStmt->insertAfter(copyStmt);
  TRAVERSE(copyStmt, new ClearTypes(), true);
  TRAVERSE(defPoint, new ClearTypes(), true);
  defPoint->parentStmt->next->copyPragmas(defPoint->parentStmt->pragmas);
  return dynamic_cast<FnSymbol*>(defExpr->sym);
}


FnSymbol* FnSymbol::coercion_wrapper(Map<Symbol*,Symbol*>* coercion_substitutions) {
  static int uid = 1;

  AList<DefExpr>* wrapper_formals = new AList<DefExpr>();
  AList<Expr>* wrapper_actuals = new AList<Expr>();
  BlockStmt* wrapper_body = new BlockStmt();
  for_alist(DefExpr, formal, formals) {
    Symbol* newFormal = formal->sym->copy();
    wrapper_formals->insertAtTail(new DefExpr(newFormal));
    Symbol* coercionSubstitution = coercion_substitutions->get(formal->sym);
    if (TypeSymbol *ts = dynamic_cast<TypeSymbol*>(coercionSubstitution)) {
      newFormal->type = ts->definition;
      char* tempName = glomstrings(2, "_coercion_temp_", formal->sym->name);
      VarSymbol* temp = new VarSymbol(tempName, formal->sym->type);
      DefExpr* tempDefExpr = new DefExpr(temp, new SymExpr(newFormal));
      wrapper_body->body->insertAtTail(new ExprStmt(tempDefExpr));
      wrapper_actuals->insertAtTail(new SymExpr(temp));
    } else {
      wrapper_actuals->insertAtTail(new SymExpr(newFormal));
    }
  }

  Expr* fn_call = new CallExpr(this, wrapper_actuals);
  if (function_returns_void(this)) {
    wrapper_body->body->insertAtTail(new ExprStmt(fn_call));
  } else {
    wrapper_body->body->insertAtTail(new ReturnStmt(fn_call));
  }

  FnSymbol* wrapper_fn = new FnSymbol(name, typeBinding, wrapper_formals,
                                      retType, NULL, wrapper_body,
                                      fnClass, noParens, retRef);
  wrapper_fn->method_type = method_type;
  wrapper_fn->cname = glomstrings(3, cname, "_coerce_wrap", intstring(uid++));
  wrapper_fn->addPragma("inline");
  defPoint->parentStmt->insertAfter(new ExprStmt(new DefExpr(wrapper_fn)));
  defPoint->parentStmt->next->copyPragmas(defPoint->parentStmt->pragmas);
  reset_file_info(wrapper_fn->defPoint->parentStmt, lineno, filename);
  return wrapper_fn;
}


FnSymbol* FnSymbol::default_wrapper(Vec<Symbol*>* defaults) {
  static int uid = 1;

  AList<DefExpr>* wrapper_formals = new AList<DefExpr>();
  AList<Expr>* wrapper_actuals = new AList<Expr>();
  BlockStmt* wrapper_body = new BlockStmt();
  for_alist(DefExpr, formal, formals) {
    if (!defaults->set_in(formal->sym)) {
      Symbol* newFormal = formal->sym->copy();
      wrapper_formals->insertAtTail(new DefExpr(newFormal));
      wrapper_actuals->insertAtTail(new SymExpr(newFormal));
    } else {
      intentTag formal_intent = dynamic_cast<ArgSymbol*>(formal->sym)->intent;
      char* temp_name = glomstrings(2, "_default_temp_", formal->sym->name);
      VarSymbol* temp = new VarSymbol(temp_name, formal->sym->type);
      Expr* temp_init = NULL;
      if (formal_intent != INTENT_OUT)
        temp_init = formal->sym->defPoint->init->copy();
      wrapper_body->body->insertAtHead(new ExprStmt(new DefExpr(temp, temp_init)));

      if (formal->sym->type != dtUnknown &&
          formal_intent != INTENT_OUT &&
          formal_intent != INTENT_INOUT)
        wrapper_actuals->insertAtTail(new CastExpr(new SymExpr(temp), NULL, formal->sym->type));
      else
        wrapper_actuals->insertAtTail(new SymExpr(temp));
    }
  }

  wrapper_body->body->insertAtTail(
    function_returns_void(this)
      ? new ExprStmt(new CallExpr(this, wrapper_actuals))
      : new ReturnStmt(new CallExpr(this, wrapper_actuals)));

  FnSymbol* wrapper_fn = new FnSymbol(name, typeBinding, wrapper_formals,
                                      retType, NULL, wrapper_body,
                                      fnClass, noParens, retRef);
  wrapper_fn->method_type = method_type;
  wrapper_fn->cname = glomstrings(3, cname, "_default_wrap", intstring(uid++));
  wrapper_fn->addPragma("inline");
  defPoint->parentStmt->insertAfter(new ExprStmt(new DefExpr(wrapper_fn)));
  defPoint->parentStmt->next->copyPragmas(defPoint->parentStmt->pragmas);
  reset_file_info(wrapper_fn->defPoint->parentStmt, lineno, filename);
  return wrapper_fn;
}


FnSymbol* FnSymbol::order_wrapper(Map<Symbol*,Symbol*>* formals_to_formals) {
  static int uid = 1;

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
  wrapper_fn->cname = glomstrings(3, cname, "_order_wrap", intstring(uid++));
  wrapper_fn->addPragma("inline");
  defPoint->parentStmt->insertBefore(new ExprStmt(new DefExpr(wrapper_fn)));
  reset_file_info(wrapper_fn->defPoint->parentStmt, lineno, filename);
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


/** This is a quick cache implementation that isn't perfect **/
class Inst : public gc {
 public:
  FnSymbol* fn;
  FnSymbol* newfn;
  Map<BaseAST*,BaseAST*>* subs;
};

static bool subs_match(Map<BaseAST*,BaseAST*>* s1,
                       Map<BaseAST*,BaseAST*>* s2) {
  if (s1->n != s2->n)
    return false;
  for (int i = 0; i < s1->n; i++) {
    if (s1->v[0].key != s2->v[0].key || s1->v[0].value != s2->v[0].value)
      return false;
  }
  return true;
}


FnSymbol* 
FnSymbol::instantiate_generic(Map<BaseAST*,BaseAST*>* map,
                              Map<BaseAST*,BaseAST*>* substitutions) {
  static int uid = 1;

#ifdef DUPLICATE_INSTANTIATION_CACHE
  static Vec<Inst*>* icache = new Vec<Inst*>();
  forv_Vec(Inst, tmp, *icache)
    if (tmp->fn == this && subs_match(substitutions, tmp->subs))
      return tmp->newfn;
  Inst* inst = new Inst();
  inst->fn = this;
  inst->subs = new Map<BaseAST*,BaseAST*>();
  inst->subs->copy(*substitutions);
#endif

  FnSymbol* copy = NULL;
  currentLineno = lineno;
  currentFilename = filename;
  TypeSymbol* clone = NULL;
  if (fnClass == FN_CONSTRUCTOR) {
    TypeSymbol* typeSym = dynamic_cast<TypeSymbol*>(retType->symbol);
    clone = typeSym->clone(map);
    instantiate_add_subs(substitutions, map);
    ClassType* cloneType = dynamic_cast<ClassType*>(clone->definition);
    Vec<TypeSymbol *> types;
    types.move(cloneType->types);
    for (int i = 0; i < types.n; i++) {
      if (types.v[i] && substitutions->get(types.v[i]->definition)) {
        types.v[i]->defPoint->parentStmt->remove();
      } else
        cloneType->types.add(types.v[i]);
    }
    instantiate_update_expr(substitutions, clone->defPoint, map);
    substitutions->put(typeSym->definition, clone->definition);

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
      } else if (ArgSymbol *s = dynamic_cast<ArgSymbol*>(substitutions->v[i].key))
        if (s->isGeneric)
          genericParameters.set_add(s);

    forv_Vec(FnSymbol, fn, functions) {
      if (functionContainsAnyAST(fn, &genericParameters)) {
        //printf("  instantiating %s\n", fn->cname);
        Stmt* fnStmt = fn->defPoint->parentStmt->copy(true, map);
        ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(fnStmt);
        DefExpr* defExpr = dynamic_cast<DefExpr*>(exprStmt->expr);
        defExpr->sym->cname =
          glomstrings(3, defExpr->sym->cname, "_instantiate_", intstring(uid++));
        fn->defPoint->parentStmt->insertBefore(fnStmt);
        instantiate_add_subs(substitutions, map);
        instantiate_update_expr(substitutions, defExpr, map);
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
        // WARNING: this type of iterator is not reentrant, do not
        //          use if nesting is possible -jbp
        for (DefExpr* formal = fnClone->formals->first(); formal; 
             formal = fnClone->formals->next()) {
          if (ArgSymbol *ps = dynamic_cast<ArgSymbol *>(formal->sym)) {
            if (TypeSymbol *ts = dynamic_cast<TypeSymbol *>(ps->variableTypeSymbol)) {
              if (ts->definition->astType != TYPE_VARIABLE)
                ps->type = ts->type;
            }
          }
        }
        fnClone->instantiatedFrom = fn;
        fnClone->substitutions.copy(*substitutions);
        tagGenerics(fnClone);
      } else {
        //printf("  not instantiating %s\n", fn->cname);
      }
    }
  } else {
    Stmt* fnStmt = defPoint->parentStmt->copy(true, map);
    ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(fnStmt);
    DefExpr* defExpr = dynamic_cast<DefExpr*>(exprStmt->expr);
    defPoint->parentStmt->insertBefore(fnStmt);
    instantiate_add_subs(substitutions, map);
    instantiate_update_expr(substitutions, defExpr, map);
    defExpr->sym->cname =
      glomstrings(3, defExpr->sym->cname, "_instantiate_", intstring(uid++));
    copy = dynamic_cast<FnSymbol*>(defExpr->sym);

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


FnSymbol* 
FnSymbol::preinstantiate_generic(Map<BaseAST*,BaseAST*>* substitutions) {
  if (fnClass != FN_CONSTRUCTOR) {
    INT_FATAL(this, "preinstantiate not called on constructor");
  }

  static int uid = 1;
  static Vec<Inst*>* icache = new Vec<Inst*>();

  forv_Vec(Inst, tmp, *icache)
    if (tmp->fn == this && subs_match(substitutions, tmp->subs))
      return tmp->newfn;

  Inst* inst = new Inst();
  inst->fn = this;
  inst->subs = new Map<BaseAST*,BaseAST*>();
  inst->subs->copy(*substitutions);

  for (int i = 0; i < substitutions->n; i++) {
    if (Type* t1 = dynamic_cast<Type*>(substitutions->v[i].key)) {
      Type* t2 = dynamic_cast<Type*>(substitutions->v[i].value);
      substitutions->put(t1->symbol, t2->symbol);
    }
  }

  Map<BaseAST*,BaseAST*> map;
  TypeSymbol* clone = retType->symbol->clone(&map);

  for (int i = 0; i < map.n; i++) {
    bool sub = false;
    for (int j = 0; j < substitutions->n; j++) {
      if (map.v[i].key == substitutions->v[j].key) {
        substitutions->put(map.v[i].value, substitutions->v[j].value);
        sub = true;
      }
    }
    if (!sub)
      substitutions->put(map.v[i].key, map.v[i].value);
  }

  TRAVERSE(clone, new UpdateSymbols(substitutions), true);
  TRAVERSE(clone->defPoint, new BuildClassHierarchy(), true);

  substitutions->put(retType->symbol, clone);
  substitutions->put(retType, clone->definition);

  ClassType* cloneType = dynamic_cast<ClassType*>(clone->definition);
  Vec<TypeSymbol *> types;
  types.move(cloneType->types);
  for (int i = 0; i < types.n; i++) {
    if (types.v[i] && substitutions->get(types.v[i]->definition)) {
      types.v[i]->defPoint->parentStmt->remove();
    } else
      cloneType->types.add(types.v[i]);
  }

  Vec<FnSymbol*> functions;
  collectFunctionsFromScope(clone->parentScope, &functions);

  Vec<BaseAST*> ov;
  ov.add(retType->symbol);
  ov.add(retType);

  FnSymbol* newfn = NULL;

  Vec<FnSymbol*> fclones;

  forv_Vec(FnSymbol, fn, functions) {
    if (functionContainsAnyAST(fn, &ov)) {
      map.clear();
      FnSymbol* fclone = fn->copy(true, &map);

      for (int i = 0; i < map.n; i++) {
        bool sub = false;
        for (int j = 0; j < substitutions->n; j++) {
          if (map.v[i].key == substitutions->v[j].key) {
            substitutions->put(map.v[i].value, substitutions->v[j].value);
            sub = true;
          }
        }
        if (!sub)
          substitutions->put(map.v[i].key, map.v[i].value);
      }

      fclones.add(fclone);
      fclone->instantiatedFrom = fn;
      fclone->cname = glomstrings(3, clone->cname, "_inst_", intstring(uid++));
      fn->defPoint->parentStmt->insertBefore(new ExprStmt(new DefExpr(fclone)));
      fn->defPoint->parentStmt->prev->copyPragmas(fn->defPoint->parentStmt->pragmas);
      TRAVERSE(fclone, new UpdateSymbols(substitutions), true);

      if (fn == this)
        newfn = fclone;

      if (fn->typeBinding == retType->symbol) {
        cloneType->methods.add(fclone);
        fclone->typeBinding = clone;
        fclone->method_type = fn->method_type;
      }

      if (retType->defaultConstructor == fn) {
        cloneType->defaultConstructor = fclone;
      }
    }
  }

  if (!newfn)
    INT_FATAL(this, "Preinstantiation error");

  inst->newfn = newfn;
  icache->add(inst);

  TRAVERSE(clone, new Instantiate(), true);
  forv_Vec(FnSymbol, fclone, fclones) {
    TRAVERSE(fclone, new Instantiate(), true);
  }

  return newfn;
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
  if (prev || next) {
    INT_FATAL(this, "Symbol is in AList");
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
    // create a boolean variable to guard module initialization
    DefExpr* varDefExpr = new DefExpr(new VarSymbol(runOnce, dtBoolean),
                                      new_BoolLiteral(true));
    // insert its definition in the common module's init function
    commonModule->initFn->body->body->insertAtHead(new ExprStmt(varDefExpr));
 
    // insert a set to false at the beginning of the current module's
    // definition (we'll wrap it in a conditional just below, after
    // filtering)
    Expr* assignVar = new CallExpr(OP_GETSNORM,
                                   new SymExpr(new UnresolvedSymbol(runOnce)),
                                   new_BoolLiteral(false));
    definition->insertAtHead(new ExprStmt(assignVar));
  }

  definition->filter(stmtIsGlob, globstmts, initstmts);

  definition = globstmts;
  BlockStmt* initFunBody;
  if (initstmts->isEmpty()) {
    initFunBody = new BlockStmt();
  } else {
    initFunBody = new BlockStmt(initstmts);
  }
  initFunBody->blkScope = modScope;
  if (runOnce) {
    // put conditional in front of body
    Stmt* testRun =
      new CondStmt(
        new CallExpr(
          OP_LOGNOT,
          new SymExpr(
            new UnresolvedSymbol(runOnce))), 
        new ReturnStmt(NULL));
    initFunBody->body->insertAtHead(testRun);
  }

  DefExpr* initFunDef = Symboltable::defineFunction(fnName, NULL, 
                                                    dtVoid, initFunBody);
  definition->insertAtHead(new ExprStmt(initFunDef));
  initFn = dynamic_cast<FnSymbol*>(initFunDef->sym);
  stmts = definition;
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
LabelSymbol::copyInner(bool clone, Map<BaseAST*,BaseAST*>* map) {
  LabelSymbol* copy = new LabelSymbol(copystring(name));
  copy->cname = copystring(cname);
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

Symbol *new_StringSymbol(char *str) {
  Immediate imm;
  imm.const_kind = IF1_CONST_KIND_STRING;
  imm.v_string = if1_cannonicalize_string(if1, str);
  VarSymbol *s = uniqueConstantsHash.get(&imm);
  if (s)
    return s;
  s = new VarSymbol(glomstrings(2, "_literal_", intstring(literal_id++)), dtString);
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

Symbol *new_BoolSymbol(bool b) {
  Immediate imm;
  imm.v_bool = b;
  imm.const_kind = IF1_NUM_KIND_UINT;
  imm.num_index = IF1_INT_TYPE_1;
  VarSymbol *s = uniqueConstantsHash.get(&imm);
  if (s)
    return s;
  s = new VarSymbol(glomstrings(2, "_literal_", intstring(literal_id++)), dtBoolean);
  if (b)
    s->cname = "true";
  else
    s->cname = "false";
  s->immediate = new Immediate;
  *s->immediate = imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}

Symbol *new_IntSymbol(long b) {
  Immediate imm;
  imm.v_int64 = b;
  imm.const_kind = IF1_NUM_KIND_INT;
  imm.num_index = IF1_INT_TYPE_64;
  VarSymbol *s = uniqueConstantsHash.get(&imm);
  if (s)
    return s;
  s = new VarSymbol(glomstrings(2, "_literal_", intstring(literal_id++)), dtInteger);
  char n[80];
  sprintf(n, "%ld", b);
  s->cname = dupstr(n);
  s->immediate = new Immediate;
  *s->immediate = imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}

Symbol *new_FloatSymbol(char *n, double b) {
  (void)n;
  Immediate imm;
  imm.v_float64 = b;
  imm.const_kind = IF1_NUM_KIND_FLOAT;
  imm.num_index = IF1_FLOAT_TYPE_64;
  VarSymbol *s = uniqueConstantsHash.get(&imm);
  if (s)
    return s;
  s = new VarSymbol(glomstrings(2, "_literal_", intstring(literal_id++)), dtFloat);
  s->immediate = new Immediate;
  s->cname = dupstr(n);
  *s->immediate = imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}

Symbol *new_ComplexSymbol(char *n, double r, double i) {
  (void)n;
  Immediate imm;
  imm.v_complex64.r = r;
  imm.v_complex64.i = i;
  imm.const_kind = IF1_NUM_KIND_COMPLEX;
  imm.num_index = IF1_FLOAT_TYPE_64;
  VarSymbol *s = uniqueConstantsHash.get(&imm);
  if (s)
    return s;
  s = new VarSymbol(glomstrings(2, "_literal_", intstring(literal_id++)), dtComplex);
  s->immediate = new Immediate;
  s->cname = dupstr(n);
  *s->immediate = imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}
