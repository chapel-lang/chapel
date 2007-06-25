#include <typeinfo>
#include "astutil.h"
#include "bb.h"
#include "build.h"
#include "files.h"
#include "iterator.h"
#include "misc.h"
#include "runtime.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "symscope.h"
#include "astutil.h"
#include "passes.h"

// for instantiate to see function resolution stuff
extern bool tag_generic(FnSymbol* fn);
extern Map<Symbol*,Symbol*> paramMap;

FnSymbol *chpl_main = NULL;

Symbol *gNil = NULL;
Symbol *gUnknown = NULL;
Symbol *gUnspecified = NULL;
Symbol *gMethodToken = NULL;
Symbol *gVoid = NULL;
Symbol *gFile = NULL;
Symbol *gTimer = NULL;
Symbol *gMutex_p = NULL;
Symbol *gCondVar_p = NULL;

VarSymbol *gTrue = NULL;
VarSymbol *gFalse = NULL;
VarSymbol *gBoundsChecking = NULL;

/*** ASTMap Cache vvv ***/
class Inst {
 public:
  Inst(FnSymbol* iOldFn, FnSymbol* iNewFn, ASTMap* iSubs) :
    oldFn(iOldFn), newFn(iNewFn), subs(new ASTMap(*iSubs)) { }
  FnSymbol* oldFn;
  FnSymbol* newFn;
  ASTMap* subs;
};

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
checkMapCache(Vec<Inst*>* cache, FnSymbol* fn, ASTMap* substitutions) {
  forv_Vec(Inst, inst, *cache)
    if (inst->oldFn == fn && subs_match(substitutions, inst->subs))
      return inst->newFn;
  return NULL;
}
/*
static void
removeMapCache(Vec<Inst*>* cache, FnSymbol* fn, ASTMap* substitutions) {
  forv_Vec(Inst, inst, *cache)
    if (inst->oldFn == fn && subs_match(substitutions, inst->subs))
      inst->newFn = NULL;
}
*/
static void
addMapCache(Vec<Inst*>* cache, FnSymbol* oldFn, FnSymbol* newFn, ASTMap* subs) {
//   if (oldFn->id == 34813) {
//     printf("%d -> %d ", oldFn->id, newFn->id);
//     form_Map(ASTMapElem, e, *subs) {
//       if (Type* t = dynamic_cast<Type*>(e->value))
//         printf("%s [%d] ", t->symbol->name, e->value->id);
//       else
//         printf("(not a type) [%d] ", e->value->id);
//     }
//     printf("\n");
//   }
  cache->add(new Inst(oldFn, newFn, subs));
}

static Vec<Inst*> icache; // instantiation cache
static Vec<Inst*> cw_cache; // coercion wrappers cache
/*** ASTMap Cache ^^^ ***/


/*** Default Wrapper Cache vvv ***/
class DWCacheItem {
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


Symbol::Symbol(astType_t astType, const char* init_name, Type* init_type) :
  BaseAST(astType),
  name(canonicalize_string(init_name)),
  cname(name),
  type(init_type),
  defPoint(NULL),
  overload(NULL),
  isUserAlias(false),
  isCompilerTemp(false),
  isExprTemp(false),
  isTypeVariable(false),
  canParam(false),
  canType(false),
  isConcurrent(false)
{}


Symbol::~Symbol() {
}


void Symbol::verify() {
  BaseAST::verify();
  if (defPoint && !defPoint->parentSymbol && !dynamic_cast<ModuleSymbol*>(this))
    INT_FATAL(this, "Symbol::defPoint is not in AST");
  if (defPoint && this != defPoint->sym)
    INT_FATAL(this, "Symbol::defPoint != Sym::defPoint->sym");
}


void Symbol::clean() {
  BaseAST::clean();
  uses.clear();
  defs.clear();
}


void Symbol::setParentScope(SymScope* init_parentScope) {
  parentScope = init_parentScope;
}


bool Symbol::inTree(void) {
  if (dynamic_cast<ModuleSymbol*>(this))
    return true;
  if (defPoint)
    return defPoint->inTree();
  else
    return false;
}


Type* Symbol::typeInfo(void) {
  return type;
}


Symbol*
Symbol::copyInner(ASTMap* map) {
  INT_FATAL(this, "Illegal call to Symbol::copy");
  return NULL;
}


void Symbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  INT_FATAL(this, "Unexpected call to Symbol::replaceChild");
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


void Symbol::codegen(FILE* outfile) {
  fprintf(outfile, "%s", cname);
}


void Symbol::codegenDef(FILE* outfile) {
  INT_FATAL(this, "Unanticipated call to Symbol::codegenDef");
}


void Symbol::codegenPrototype(FILE* outfile) { }


FnSymbol* Symbol::getFnSymbol(void) {
  return NULL;
}


Symbol* Symbol::getSymbol(void) {
  return this;
}


char* Symbol::hasPragma(char* str) {
  forv_Vec(char, pragma, pragmas) {
    if (!strcmp(pragma, str))
      return pragma;
  }
  if (!dynamic_cast<ModuleSymbol*>(this) && getModule())
    return getModule()->hasPragma(str);
  return NULL;
}


void Symbol::removePragma(char* str) {
  for (int i = 0; i < pragmas.n; i++)
    if (!strcmp(pragmas.v[i], str))
      pragmas.v[i] = "";
  if (!dynamic_cast<ModuleSymbol*>(this) && getModule())
    getModule()->removePragma(str);
}


char* Symbol::hasPragmaPrefix(char* str) {
  forv_Vec(char, pragma, pragmas) {
    if (!strncmp(pragma, str, strlen(str)))
      return pragma;
  }
  if (!dynamic_cast<ModuleSymbol*>(this) && getModule())
    return getModule()->hasPragmaPrefix(str);
  return NULL;
}


bool Symbol::isImmediate() {
  return false;
}


UnresolvedSymbol::UnresolvedSymbol(const char* init_name) :
  Symbol(SYMBOL_UNRESOLVED, init_name)
{ }


void UnresolvedSymbol::verify() {
  Symbol::verify();
  if (astType != SYMBOL_UNRESOLVED) {
    INT_FATAL(this, "Bad UnresolvedSymbol::astType");
  }
}


void UnresolvedSymbol::codegen(FILE* outfile) {
  INT_FATAL(this, "ERROR:  Cannot codegen an unresolved symbol.");
  fprintf(outfile, "%s /* unresolved */ ", name);
}


UnresolvedSymbol*
UnresolvedSymbol::copyInner(ASTMap* map) {
  return new UnresolvedSymbol(name);
}


VarSymbol::VarSymbol(char    *init_name,
                     Type    *init_type,
                     varType  init_varClass, 
                     consType init_consClass) :
  Symbol(SYMBOL_VAR, init_name, init_type),
  varClass(init_varClass),
  consClass(init_consClass),
  immediate(NULL),
  refc(NULL),
  refcMutex(NULL)
{ }


VarSymbol::~VarSymbol() {
  if (immediate)
    delete immediate;
}


void VarSymbol::verify() {
  Symbol::verify();
  if (astType != SYMBOL_VAR) {
    INT_FATAL(this, "Bad VarSymbol::astType");
  }
}


VarSymbol*
VarSymbol::copyInner(ASTMap* map) {
  VarSymbol* newVarSymbol = new VarSymbol(name, type, varClass, consClass);
  newVarSymbol->cname = cname;
  newVarSymbol->isUserAlias = isUserAlias;
  newVarSymbol->isCompilerTemp = isCompilerTemp;
  newVarSymbol->isExprTemp = isExprTemp;
  newVarSymbol->isTypeVariable = isTypeVariable;
  newVarSymbol->canParam = canParam;
  newVarSymbol->canType = canType;
  newVarSymbol->isConcurrent = isConcurrent;
  assert(!newVarSymbol->immediate);
  return newVarSymbol;
}


void VarSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  type->replaceChild(old_ast, new_ast);
}


bool VarSymbol::isConst(void) {
  return (consClass == VAR_CONST);
}


bool VarSymbol::isParam(void){
  return (consClass == VAR_PARAM) || immediate;
}


void VarSymbol::codegen(FILE* outfile) {
  if (immediate && immediate->const_kind == CONST_KIND_STRING) {
    fprintf(outfile, "\"%s\"", immediate->v_string);
  } else if (immediate &&
             immediate->const_kind == NUM_KIND_UINT &&
             immediate->num_index == INT_SIZE_1) {
    fprintf(outfile, immediate->uint_value() ? "true" : "false");
  } else if (immediate &&
             immediate->const_kind == NUM_KIND_INT) {
    int64 iconst = immediate->int_value();
    if (iconst == (1ll<<63)) {
      fprintf(outfile, "-INT64(9223372036854775807) - INT64(1)");
    } else if (iconst <= -2147483648ll || iconst >= 2147483647ll) {
      fprintf(outfile, "INT64(%lld)", iconst);
    } else {
      fprintf(outfile, "%lld", iconst);
    }
  } else if (immediate &&
             immediate->const_kind == NUM_KIND_UINT) {
    uint64 uconst = immediate->uint_value();
    fprintf(outfile, "UINT64(%lld)", uconst);
  } else {
    fprintf(outfile, "%s", cname);
  }
}


void VarSymbol::codegenDef(FILE* outfile) {
  if (type == dtVoid)
    return;
  // need to ensure that this can be realized in C as a const, and
  // move its initializer here if it can be
  if (0 && (consClass == VAR_CONST)) {
    fprintf(outfile, "const ");
  }
  type->codegen(outfile);
  fprintf(outfile, " ");
  fprintf(outfile, "%s;\n", cname);
}


bool VarSymbol::isImmediate() {
  return immediate != NULL;
}


ArgSymbol::ArgSymbol(intentTag iIntent, char* iName, 
                     Type* iType, Expr* iDefaultExpr,
                     Expr* iVariableExpr) :
  Symbol(SYMBOL_ARG, iName, iType),
  intent(iIntent),
  defaultExpr(iDefaultExpr),
  variableExpr(iVariableExpr),
  isGeneric(false),
  instantiatedFrom(NULL),
  instantiatedParam(false)
{
  if (intent == INTENT_PARAM)
    isGeneric = true;
}


void ArgSymbol::verify() {
  Symbol::verify();
  if (astType != SYMBOL_ARG) {
    INT_FATAL(this, "Bad ArgSymbol::astType");
  }
}


ArgSymbol*
ArgSymbol::copyInner(ASTMap* map) {
  ArgSymbol *ps = new ArgSymbol(intent, name, type,
                                COPY_INT(defaultExpr), COPY_INT(variableExpr));
  ps->isGeneric = isGeneric;
  ps->cname = cname;
  ps->instantiatedParam = instantiatedParam;
  ps->isTypeVariable = isTypeVariable;
  return ps;
}


void ArgSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == defaultExpr)
    defaultExpr = dynamic_cast<Expr*>(new_ast);
  else if (old_ast == variableExpr)
    variableExpr = dynamic_cast<Expr*>(new_ast);
  else
    type->replaceChild(old_ast, new_ast);
}


bool ArgSymbol::requiresCPtr(void) {
  if (intent == INTENT_REF ||
      (!strcmp(name, "this") && is_complex_type(type)))
    return true;
  ClassType* ct = dynamic_cast<ClassType*>(type);
  if (ct && ct->classTag != CLASS_CLASS)
    return true;
  return false;
}


bool ArgSymbol::isConst(void) {
  return intent == INTENT_BLANK || intent == INTENT_CONST;
}


void ArgSymbol::codegen(FILE* outfile) {
  if (requiresCPtr())
    fprintf(outfile, "(*");
  fprintf(outfile, "%s", cname);
  if (requiresCPtr())
    fprintf(outfile, ")");
}


void ArgSymbol::codegenDef(FILE* outfile) {
  type->codegen(outfile);
  if (requiresCPtr())
    fprintf(outfile, "* const");
  fprintf(outfile, " ");
  fprintf(outfile, "%s", cname);
}


TypeSymbol::TypeSymbol(char* init_name, Type* init_type) :
  Symbol(SYMBOL_TYPE, init_name, init_type)
{
  isTypeVariable = true;
  if (!type)
    INT_FATAL(this, "TypeSymbol constructor called without type");
  type->addSymbol(this);
  gTypes.add(this);
}


void TypeSymbol::verify() {
  Symbol::verify();
  if (astType != SYMBOL_TYPE) {
    INT_FATAL(this, "Bad TypeSymbol::astType");
  }
  if (type->symbol != this)
    INT_FATAL(this, "TypeSymbol::type->symbol != TypeSymbol");
}


TypeSymbol*
TypeSymbol::copyInner(ASTMap* map) {
  Type* new_type = COPY_INT(type);
  TypeSymbol* new_type_symbol = new TypeSymbol(name, new_type);
  new_type->addSymbol(new_type_symbol);
  new_type_symbol->cname = cname;
  return new_type_symbol;
}


void TypeSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  type->replaceChild(old_ast, new_ast);
}


void TypeSymbol::codegenPrototype(FILE* outfile) {
  type->codegenPrototype(outfile);
}


void TypeSymbol::codegenDef(FILE* outfile) {
  type->codegenDef(outfile);
}


FnSymbol::FnSymbol(char* initName) :
  Symbol(SYMBOL_FN, initName),
  formals(new AList()),
  setter(NULL),
  retType(dtUnknown),
  where(NULL),
  retExprType(NULL),
  body(new BlockStmt()),
  fnClass(FN_FUNCTION),
  noParens(false),
  retRef(false),
  defSetGet(false),
  isParam(false),
  iteratorInfo(NULL),
  argScope(NULL),
  isGeneric(false),
  _this(NULL),
  isMethod(false),
  instantiatedFrom(NULL),
  instantiatedTo(NULL),
  visiblePoint(NULL),
  visible(true),
  global(false),
  basicBlocks(NULL),
  calledBy(NULL),
  isWrapper(false),
  normalizedOnce(false)
{
  substitutions.clear();
  gFns.add(this);
  formals->parent = this;
}


FnSymbol::~FnSymbol() {
  if (iteratorInfo)
    delete iteratorInfo;
  if (instantiatedTo)
    delete instantiatedTo;
  if (basicBlocks) {
    forv_Vec(BasicBlock, bb, *basicBlocks) {
      delete bb;
    }
    delete basicBlocks;
  }
  if (calledBy)
    delete calledBy;
  if (argScope)
    delete argScope;
  delete formals;
}


void FnSymbol::verify() {
  Symbol::verify();
  if (astType != SYMBOL_FN) {
    INT_FATAL(this, "Bad FnSymbol::astType");
  }
  if (normalized && !hasPragma("auto ii")) {
    CallExpr* last = dynamic_cast<CallExpr*>(body->body->last());
    if (!last || !last->isPrimitive(PRIMITIVE_RETURN))
      INT_FATAL(this, "Last statement in normalized function is not a return");
  }
  if (formals->parent != this)
    INT_FATAL(this, "Bad AList::parent in FnSymbol");
}


FnSymbol* FnSymbol::getFnSymbol(void) {
  return this;
}


FnSymbol*
FnSymbol::copyInner(ASTMap* map) {
  FnSymbol* copy = new FnSymbol(name);
  for_formals(formal, this) {
    copy->insertFormalAtTail(COPY_INT(formal->defPoint));
  }
  copy->setter = COPY_INT(setter);
  copy->retType = retType;
  copy->where = COPY_INT(where);
  copy->body = COPY_INT(body);
  copy->fnClass = fnClass;
  copy->noParens = noParens;
  copy->retRef = retRef;
  copy->retExprType = COPY_INT(retExprType);
  copy->cname = cname;
  copy->isGeneric = isGeneric;
  copy->_this = _this;
  copy->isMethod = isMethod;
  copy->visible = visible;
  copy->global = global;
  copy->isParam = isParam;
  copy->canParam = canParam;
  return copy;
}


void FnSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == body) {
    body = dynamic_cast<BlockStmt*>(new_ast);
  } else if (old_ast == where) {
    where = dynamic_cast<BlockStmt*>(new_ast);
  } else if (old_ast == setter) {
    setter = dynamic_cast<DefExpr*>(new_ast);
  } else if (old_ast == retExprType) {
    retExprType = dynamic_cast<Expr*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in FnSymbol::replaceChild");
  }
}


static bool
returns_void(FnSymbol* fn) {
  CallExpr* ret = dynamic_cast<CallExpr*>(fn->body->body->last());
  if (!ret || !ret->isPrimitive(PRIMITIVE_RETURN))
    INT_FATAL(fn, "Function is not in normal form");
  return ret->typeInfo() == dtVoid;
}


static CallExpr*
make_method_call_partial(CallExpr* call) {
  call->partialTag = true;
  CallExpr* outer = new CallExpr(call);
  while (call->argList->length() > 2) {
    Expr* arg = call->get(3);
    arg->remove();
    outer->insertAtTail(arg);
  }
  return outer;
}


static FnSymbol*
build_empty_wrapper(FnSymbol* fn) {
  FnSymbol* wrapper = new FnSymbol(fn->name);
  wrapper->isWrapper = true;
  wrapper->visible = false;
  wrapper->addPragmas(&fn->pragmas);
  wrapper->addPragma("inline");
  wrapper->noParens = fn->noParens;
  if (fn->fnClass != FN_ITERATOR) { // getValue is ref, not iterator
    wrapper->retRef = fn->retRef;
    if (fn->setter)
      wrapper->setter = fn->setter->copy();
  }
  wrapper->isParam = fn->isParam;
  wrapper->isMethod = fn->isMethod;
  return wrapper;
}


FnSymbol*
FnSymbol::coercion_wrapper(ASTMap* coercion_map) {
  // return cached if we already created this coercion wrapper
  if (FnSymbol* cached = checkMapCache(&cw_cache, this, coercion_map))
    return cached;

  FnSymbol* wrapper = build_empty_wrapper(this);
  wrapper->retType = retType;
  wrapper->cname = stringcat("_coerce_wrap_", cname);
  CallExpr* call = new CallExpr(this);
  for_formals(formal, this) {
    Symbol* wrapper_formal = formal->copy();
    if (_this == formal)
      wrapper->_this = wrapper_formal;
    wrapper->insertFormalAtTail(wrapper_formal);
    if (TypeSymbol *ts = dynamic_cast<TypeSymbol*>(coercion_map->get(formal))) {
      wrapper_formal->type = ts->type;
      if (ts->hasPragma( "synchronization primitive")) {
        // check if this is a member access
        DefExpr *mt;
        if ((this->formals->length() > 0) &&
            (mt = dynamic_cast<DefExpr*>(this->formals->get(1))) &&
            (mt->sym->type == dtMethodToken) &&
            (_this == this->getFormal(2))) {
          // call->insertAtTail( new CallExpr( "readXX", wrapper_formal));
          call->insertAtTail( new CallExpr( "value", gMethodToken, wrapper_formal));
        } else {
          if (ts->hasPragma( "sync var"))
            call->insertAtTail( new CallExpr( "readFE", wrapper_formal));
          else   // else, single var case
            call->insertAtTail( new CallExpr( "readFF", wrapper_formal));
        }
      } else if (ts->hasPragma("ref") && !formal->isTypeVariable) {
        VarSymbol* tmp = new VarSymbol("_tmp");
        tmp->isCompilerTemp = true;
        wrapper->insertAtTail(new DefExpr(tmp));
        wrapper->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_GET_REF, wrapper_formal)));
        call->insertAtTail(tmp);
      } else {
        call->insertAtTail(new CallExpr(PRIMITIVE_CAST, formal->type->symbol, wrapper_formal));
      }
    } else {
      call->insertAtTail(wrapper_formal);
    }
  }
  if (isMethod && !noParens)
    call = make_method_call_partial(call);
  if (retType == dtVoid || (retType == dtUnknown && returns_void(this)))
    wrapper->insertAtTail(call);
  else
    wrapper->insertAtTail(new CallExpr(PRIMITIVE_RETURN, call));
  defPoint->insertAfter(new DefExpr(wrapper));
  reset_file_info(wrapper->defPoint, lineno, filename);
  normalize(wrapper);
  addMapCache(&cw_cache, this, wrapper, coercion_map);
  return wrapper;
}


FnSymbol* FnSymbol::default_wrapper(Vec<Symbol*>* defaults) {
  if (FnSymbol* cached = check_dwcache(this, defaults))
    return cached;
  FnSymbol* wrapper = build_empty_wrapper(this);
  wrapper->retType = retType;
  wrapper->cname = stringcat("_default_wrap_", cname);
  CallExpr* call = new CallExpr(this);
  ASTMap copy_map;
  for_formals(formal, this) {
    if (!defaults->in(formal)) {
      Symbol* wrapper_formal = formal->copy();
      if (_this == formal)
        wrapper->_this = wrapper_formal;
      copy_map.put(formal, wrapper_formal);
      wrapper->insertFormalAtTail(wrapper_formal);
      call->insertAtTail(wrapper_formal);
    } else {
      char* temp_name = stringcat("_default_temp_", formal->name);
      VarSymbol* temp = new VarSymbol(temp_name);
      temp->isCompilerTemp = true;
      copy_map.put(formal, temp);
      Expr* temp_init = NULL;
      Expr* temp_type = NULL;
      if (formal->intent != INTENT_OUT)
        temp_init = formal->defaultExpr->copy();
      if (SymExpr* symExpr = dynamic_cast<SymExpr*>(temp_init))
        if (symExpr->var == gNil)
          temp_init = NULL;
      if (formal->defPoint->exprType)
        temp_type = formal->defPoint->exprType->copy();
      else if (formal->type != dtUnknown && !formal->type->isGeneric)
        temp_type = new SymExpr(formal->type->symbol);
      if (formal->type->symbol->hasPragma("synchronization primitive") ||
          formal->type->symbol->hasPragma("ref"))
        temp_type = new CallExpr("_init", formal->type->symbol);
      wrapper->insertAtTail(new DefExpr(temp, temp_init, temp_type));
      call->insertAtTail(temp);
    }
  }
  update_symbols(wrapper->body, &copy_map);
  if (isMethod)
    call = make_method_call_partial(call);
  if (returns_void(this)) {
    wrapper->insertAtTail(call);
  } else if (fnClass != FN_ITERATOR) {
    wrapper->insertAtTail(new CallExpr(PRIMITIVE_RETURN, call));
  } else {
    VarSymbol* index = new VarSymbol("_i");
    index->isCompilerTemp = true;
    wrapper->insertAtTail(new DefExpr(index));
    wrapper->insertAtTail(build_for_expr(new SymExpr(index), call, new SymExpr(index)));
    wrapper->fnClass = FN_ITERATOR;
  }
  defPoint->insertAfter(new DefExpr(wrapper));

  reset_file_info(wrapper->defPoint, lineno, filename);

  // Make the line numbers match the numbers from the formals they map to
  for_formals(formal, this) {
    if (Symbol* sym = dynamic_cast<Symbol*>(copy_map.get(formal))) {
      sym->defPoint->lineno = formal->lineno;
      sym->defPoint->filename = formal->filename;
      sym->lineno = formal->lineno;
      sym->filename = formal->filename;
    }
  }
  add_dwcache(wrapper, this, defaults);
  normalize(wrapper);
  return wrapper;
}


FnSymbol* FnSymbol::order_wrapper(Map<Symbol*,Symbol*>* order_map) {
  FnSymbol* wrapper = build_empty_wrapper(this);
  wrapper->cname = stringcat("_order_wrap_", cname);
  CallExpr* call = new CallExpr(this);
  ASTMap copy_map;
  for_formals(formal, this) {
    Symbol* wrapper_formal = formal->copy();
    if (_this == formal)
      wrapper->_this = wrapper_formal;
    copy_map.put(formal, wrapper_formal);
  }
  for_formals(formal, this) {
    wrapper->insertFormalAtTail(copy_map.get(order_map->get(formal)));
    call->insertAtTail(copy_map.get(formal));
  }
  if (isMethod)
    call = make_method_call_partial(call);
  if (returns_void(this))
    wrapper->insertAtTail(call);
  else
    wrapper->insertAtTail(new CallExpr(PRIMITIVE_RETURN, call));
  defPoint->insertBefore(new DefExpr(wrapper));
  clear_file_info(wrapper->defPoint);
  normalize(wrapper);
  return wrapper;
}


FnSymbol* FnSymbol::promotion_wrapper(Map<Symbol*,Symbol*>* promotion_subs,
                                      bool square) {
  FnSymbol* wrapper = build_empty_wrapper(this);
  wrapper->cname = stringcat("_promotion_wrap_", cname);
  CallExpr* indicesCall = new CallExpr("_tuple"); // destructured in build
  CallExpr* iterator = new CallExpr(square ? "_build_domain" : "_build_tuple");
  CallExpr* actualCall = new CallExpr(this);
  int i = 1;
  for_formals(formal, this) {
    Symbol* new_formal = formal->copy();
    if (_this == formal)
      wrapper->_this = new_formal;
    if (Symbol* sub = promotion_subs->get(formal)) {
      TypeSymbol* ts = dynamic_cast<TypeSymbol*>(sub);
      if (!ts)
        INT_FATAL(this, "error building promotion wrapper");
      new_formal->type = ts->type;
      wrapper->insertFormalAtTail(new_formal);
      iterator->insertAtTail(new_formal);
      VarSymbol* index = new VarSymbol(astr("_p_i_", istr(i)));
      index->isCompilerTemp = true;
      wrapper->insertAtTail(new DefExpr(index));
      indicesCall->insertAtTail(index);
      actualCall->insertAtTail(index);
    } else {
      wrapper->insertFormalAtTail(new_formal);
      actualCall->insertAtTail(new_formal);
    }
    i++;
  }

  if (isMethod && !noParens)
    actualCall = make_method_call_partial(actualCall);
  BaseAST* indices = indicesCall;
  if (indicesCall->argList->length() == 1)
    indices = indicesCall->argList->only()->remove();
  else
    indicesCall->insertAtHead(new_IntSymbol(indicesCall->argList->length()));
  if (returns_void(this)) {
    wrapper->insertAtTail(new BlockStmt(build_for_block(BLOCK_FORALL,
                                         indices, iterator,
                                         new BlockStmt(actualCall))));
  } else {
    wrapper->insertAtTail(build_for_expr(indices, iterator, actualCall));
    wrapper->fnClass = FN_ITERATOR;
    //    wrapper->retRef = false;
    wrapper->removePragma("inline");
  }
  defPoint->insertBefore(new DefExpr(wrapper));
  clear_file_info(wrapper->defPoint);
  normalize(wrapper);
  return wrapper;
}

static void
copyGenericSub(ASTMap& subs, FnSymbol* root, FnSymbol* fn, BaseAST* key, BaseAST* value) {
  if (!strcmp("_construct__tuple", root->name)) {
    if (Symbol* sym = dynamic_cast<Symbol*>(key)) {
      if (sym->name[0] == 'x') {
        subs.put((BaseAST*)(atoi(sym->name+1)), value);
        return;
      }
    }
  }
  if (dynamic_cast<ArgSymbol*>(key)) {
    if (root != fn) {
      int i = 1;
      for_formals(formal, fn) {
        if (formal == key) {
          subs.put(root->getFormal(i), value);
          return;
        }
        i++;
      }
    }
  }
  subs.put(key, value);
}

static FnSymbol*
instantiate_function(FnSymbol *fn, ASTMap *generic_subs, Type* newType) {
  ASTMap map;

  FnSymbol* clone = fn->copy(&map);
  clone->isGeneric = false;
  clone->visible = false;
  clone->instantiatedFrom = fn;

  FnSymbol* root = fn;
  while (root->instantiatedFrom && root->formals->length() == root->instantiatedFrom->formals->length())
    root = root->instantiatedFrom;

  if (fn->instantiatedFrom)
    form_Map(ASTMapElem, e, fn->substitutions)
      copyGenericSub(clone->substitutions, root, fn, e->key, e->value);
  form_Map(ASTMapElem, e, *generic_subs)
    copyGenericSub(clone->substitutions, root, fn, e->key, e->value);

//   if (root->id == 53713) {
//     printf("add %d: ", root->id);
//     form_Map(ASTMapElem, e, clone->substitutions) {
//       if (Type* t = dynamic_cast<Type*>(e->value))
//         printf("%s [%x->%d] ", t->symbol->name, (unsigned int)e->key, e->value->id);
//       else
//         printf("(not a type) [%x->%d] ", (unsigned int)e->key, e->value->id);
//     }
//     printf("\n");
//   }
  addMapCache(&icache, root, clone, &clone->substitutions);
  fn->defPoint->insertBefore(new DefExpr(clone));

  // add parameter instantiations to parameter map for function resolution
  for (int i = 0; i < generic_subs->n; i++) {
    if (ArgSymbol* arg = dynamic_cast<ArgSymbol*>(generic_subs->v[i].key)) {
      if (arg->intent == INTENT_PARAM) {
        Symbol* key = dynamic_cast<Symbol*>(map.get(arg));
        Symbol* val = dynamic_cast<Symbol*>(generic_subs->v[i].value);
        if (!key || !val)
          INT_FATAL("error building parameter map in instantiation");
        paramMap.put(key, val);
      }
    }
  }

  // extend parameter map if parameter intent argument is instantiated
  // again; this may happen because the type is omitted and the
  // argument is later instantiated based on the type of the parameter
  for_formals(arg, fn) {
    if (paramMap.get(arg)) {
      Symbol* key = dynamic_cast<Symbol*>(map.get(arg));
      Symbol* val = paramMap.get(arg);
      if (!key || !val)
        INT_FATAL("error building parameter map in instantiation");
      paramMap.put(key, val);
    }
  }

  // update body of constructors with a return type substitution
  ASTMap subs;
  if (newType) {
    subs.put(fn->retType, newType);
    subs.put(fn->retType->symbol, newType->symbol);
  }
  update_symbols(clone, &subs);

  for_formals(formal, fn) {
    ArgSymbol* cloneFormal = dynamic_cast<ArgSymbol*>(map.get(formal));
    if (generic_subs->get(formal) && formal->intent == INTENT_PARAM) {
      cloneFormal->intent = INTENT_BLANK;
      cloneFormal->isGeneric = false;
      cloneFormal->instantiatedParam = true;
      if (cloneFormal->type->isGeneric)
        cloneFormal->type = paramMap.get(cloneFormal)->type;
      if (cloneFormal->defaultExpr)
        cloneFormal->defaultExpr->remove();
      cloneFormal->defaultExpr = new SymExpr(gNil);
      cloneFormal->defaultExpr->parentSymbol = cloneFormal;
    } else if (Type* t = dynamic_cast<Type*>(generic_subs->get(formal))) {
      cloneFormal->isGeneric = false;
      cloneFormal->instantiatedFrom = formal->type;
      cloneFormal->type = t;
      if (cloneFormal->defaultExpr)
        cloneFormal->defaultExpr->remove();
      cloneFormal->defaultExpr = new SymExpr(gNil);
      cloneFormal->defaultExpr->parentSymbol = cloneFormal;
    }
  }
  return clone;
}

static void
instantiate_tuple(FnSymbol* fn) {
  ClassType* tuple = dynamic_cast<ClassType*>(fn->retType);
  int64 size = dynamic_cast<VarSymbol*>(tuple->substitutions.v[0].value)->immediate->int_value();
  Expr* last = fn->body->body->last();
  for (int i = 1; i <= size; i++) {
    char* name = stringcat("x", intstring(i));
    ArgSymbol* arg = new ArgSymbol(INTENT_BLANK, name, dtAny, new SymExpr(gNil));
    fn->insertFormalAtTail(arg);
    last->insertBefore(new CallExpr(PRIMITIVE_SET_MEMBER, fn->_this,
                                    new_StringSymbol(name), arg));
    VarSymbol* field = new VarSymbol(name);
    tuple->fields->insertAtTail(new DefExpr(field));
  }
  fn->removePragma("tuple");
}

FnSymbol*
instantiate_tuple_get(FnSymbol* fn) {
  VarSymbol* var = dynamic_cast<VarSymbol*>(fn->substitutions.get(fn->instantiatedFrom->getFormal(2)));
  if (!var || var->immediate->const_kind != NUM_KIND_INT)
    return fn;
  int64 index = var->immediate->int_value();
  if (index <= 0 || index >= dynamic_cast<ClassType*>(fn->_this->type)->fields->length()) {
    fn->body->replace(new BlockStmt(new CallExpr(PRIMITIVE_ERROR, new_StringSymbol(astr("tuple index out-of-bounds error (", intstring(index), ")")))));
  } else {
    char* name = stringcat("x", intstring(index));
    fn->body->replace(new BlockStmt(new CallExpr(PRIMITIVE_RETURN, new CallExpr(PRIMITIVE_GET_MEMBER, fn->_this, new_StringSymbol(name)))));
  }
  normalize(fn);
  return fn;
}

FnSymbol*
instantiate_tuple_copy(FnSymbol* fn) {
  if (fn->formals->length() != 1)
    INT_FATAL(fn, "tuple copy function has more than one argument");
  ArgSymbol* arg = fn->getFormal(1);
  ClassType* ct = dynamic_cast<ClassType*>(arg->type);
  CallExpr* call = new CallExpr(ct->defaultConstructor->name);
  call->insertAtTail(new CallExpr(".", arg, new_StringSymbol("size")));
  for (int i = 1; i < ct->fields->length(); i++)
    call->insertAtTail(new CallExpr(arg, new_IntSymbol(i)));
  fn->body->replace(new BlockStmt(new CallExpr(PRIMITIVE_RETURN, call)));
  normalize(fn);
  return fn;
}


FnSymbol*
instantiate_tuple_init(FnSymbol* fn) {
  if (fn->formals->length() != 1)
    INT_FATAL(fn, "tuple init function has more than one argument");
  ArgSymbol* arg = fn->getFormal(1);
  ClassType* ct = dynamic_cast<ClassType*>(arg->type);
  CallExpr* call = new CallExpr(ct->defaultConstructor->name);
  call->insertAtTail(new CallExpr(".", arg, new_StringSymbol("size")));
  for (int i = 1; i < ct->fields->length(); i++)
    call->insertAtTail(new CallExpr("_init", new CallExpr(arg, new_IntSymbol(i))));
  fn->body->replace(new BlockStmt(new CallExpr(PRIMITIVE_RETURN, call)));
  normalize(fn);
  return fn;
}


FnSymbol*
instantiate_tuple_hash( FnSymbol* fn) {
  if (fn->formals->length() != 1)
    INT_FATAL(fn, "tuple hash function has more than one argument");
  ArgSymbol  *arg = fn->getFormal(1);
  ClassType  *ct = dynamic_cast<ClassType*>(arg->type);
  CallExpr *ret;
  if (ct->fields->length() < 0) {
    ret = new CallExpr(PRIMITIVE_RETURN, new_IntSymbol(0, INT_SIZE_64));
  } else {
    CallExpr *call = NULL;
    bool first = true;
    for (int i=1; i<ct->fields->length(); i++) {
      CallExpr *field_access = new CallExpr( arg, new_IntSymbol(i)); 
      if (first) {
        call =  new CallExpr( "_indefinite_hash", field_access);
        first = false;
      } else {
        call = new CallExpr( "^", 
                             new CallExpr( "_indefinite_hash",
                                           field_access),
                             new CallExpr( "<<",
                                           call,
                                           new_IntSymbol(17)));
      }
    }
    // YAH, make sure that we do not return a negative hash value for now
    call = new CallExpr( "&", new_IntSymbol( 0x7fffffffffffffffLL, INT_SIZE_64), call);
    ret = new CallExpr(PRIMITIVE_RETURN, new CallExpr("_cast", dtInt[INT_SIZE_64]->symbol, call));
  }
  fn->body->replace( new BlockStmt( ret));
  normalize(fn);
  return fn;
}


static int
count_instantiate_with_recursion(Type* t) {
  if (t->instantiatedWith) {
    int high = 0;
    forv_Vec(Type, s, *t->instantiatedWith) {
      int count = count_instantiate_with_recursion(s);
      if (count > high)
        high = count;
    }
    return high + 1;
  }
  return 0;
}


FnSymbol*
FnSymbol::instantiate_generic(ASTMap* generic_substitutions) {
  Vec<BaseAST*> keys;
  generic_substitutions->get_keys( keys);
  forv_Vec(BaseAST, key, keys) {
    if (Type* t = dynamic_cast<Type*>(generic_substitutions->get( key))) {
      if (t->isGeneric)
        INT_FATAL(this, "illegal instantiation with a generic type");
      if (t->symbol->hasPragma( "synchronization primitive") && !hasPragma("ref")) {
        if (!hasPragma("synchronization primitive") ||
            (isMethod && (t->instantiatedFrom != _this->type))) {
          // allow types to be instantiated to sync types
          Symbol* arg = dynamic_cast<Symbol*>(key);
          if (!arg || !arg->isTypeVariable) {
            // instantiation of a non-type formal of sync type loses sync
            Type  *base_type = dynamic_cast<Type*>( t->substitutions.v[0].value);
            generic_substitutions->put( key, base_type);
          }
        }
      } else if (!hasPragma("tuple") &&
                 t->symbol->hasPragma("ref") && !hasPragma("ref")) {
        generic_substitutions->put(key, t->getField("_val")->type);
      }
    }

  }

  // return cached if we did this instantiation already
  FnSymbol* root = this;
  while (root->instantiatedFrom && root->formals->length() == root->instantiatedFrom->formals->length())
    root = root->instantiatedFrom;

  ASTMap all_substitutions;
  if (instantiatedFrom)
    form_Map(ASTMapElem, e, substitutions)
      copyGenericSub(all_substitutions, root, this, e->key, e->value);
  form_Map(ASTMapElem, e, *generic_substitutions)
    copyGenericSub(all_substitutions, root, this, e->key, e->value);

//   if (root->id == 53713) {
//     printf("chk %d: ", root->id);
//     form_Map(ASTMapElem, e, all_substitutions) {
//       if (Type* t = dynamic_cast<Type*>(e->value))
//         printf("%s [%x->%d] ", t->symbol->name, (unsigned int)e->key, e->value->id);
//       else
//         printf("(not a type) [%x->%d] ", (unsigned int)e->key, e->value->id);
//     }
//   }
  if (FnSymbol* cached = checkMapCache(&icache, root, &all_substitutions)) {
//     if (root->id == 53713) printf("cached\n");
    return cached;
  }
//   if (root->id == 53713) printf("\n");

  //  static int uid = 1;
  FnSymbol* newfn = NULL;
  currentLineno = lineno;
  currentFilename = filename;

  // check for infinite recursion by limiting the number of
  // instantiations of a particular type or function
  if (instantiatedTo == NULL)
    instantiatedTo = new Vec<FnSymbol*>();
  if (instantiatedTo->n >= instantiation_limit &&
      // disable error on functions in base module
      //  because folding is done via instantiation
      //  caution: be careful developing in the base module
      baseModule != getModule()) {
    if (fnClass == FN_CONSTRUCTOR) {
      USR_FATAL_CONT(retType, "Type '%s' has been instantiated too many times",
                     retType->symbol->name);
    } else {
      USR_FATAL_CONT(this, "Function '%s' has been instantiated too many times",
                     name);
    }
    USR_PRINT("  If this is intentional, try increasing"
              " the instantiation limit from %d", instantiation_limit);
    USR_STOP();
  }

  if (fnClass == FN_CONSTRUCTOR) {
    if (!dynamic_cast<ClassType*>(retType))
      INT_FATAL("bad instantiation of non-class type");

    // copy generic class type
    TypeSymbol* clone = retType->symbol->copy();

    // make scope of instantiation "use" module of instantiating types
    // why??? --sjd
    Vec<BaseAST*> values;
    generic_substitutions->get_values(values);
    forv_Vec(BaseAST, value, values)
      if (Type* type = dynamic_cast<Type*>(value))
        if (!dynamic_cast<PrimitiveType*>(type))
          if (!retType->symbol->hasPragma("ref"))
            defPoint->parentScope->addModuleUse(type->getModule());

    // compute instantiatedWith vector and rename instantiated type
    clone->name = astr(clone->name, "(");
    clone->cname = stringcat(clone->cname, "_A_");
    bool first = false;
    for (int i = 0; i < generic_substitutions->n; i++) {
      if (BaseAST* value = generic_substitutions->v[i].value) {
        if (Type* type = dynamic_cast<Type*>(value)) {
          if (!first && !strncmp(clone->name, "_tuple", 6)) {
            clone->name = astr("(");
            clone->cname = "_tuple_A_";
          }
          if (first) {
            clone->name = astr(clone->name, ",");
            clone->cname = stringcat(clone->cname, "_");
          }
          clone->cname = stringcat(clone->cname, type->symbol->cname);
          clone->name = astr(clone->name, type->symbol->name);
          if (!clone->type->instantiatedWith)
            clone->type->instantiatedWith = new Vec<Type*>();
          clone->type->instantiatedWith->add(type);
          first = true;
        } else if (Symbol* symbol = dynamic_cast<Symbol*>(value)) {
          if (first) {
            clone->name = astr(clone->name, ",");
            clone->cname = stringcat(clone->cname, "_");
          }
          VarSymbol* var = dynamic_cast<VarSymbol*>(symbol);
          if (var && var->immediate) {
            char imm[128];
            sprint_imm(imm, *var->immediate);
            clone->name = astr(clone->name, imm);
            clone->cname = stringcat(clone->cname, imm);
          } else {
            clone->name = astr(clone->name, symbol->cname);
            clone->cname = stringcat(clone->cname, symbol->cname);
          }
          first = true;
        }
      }
    }
    clone->name = astr(clone->name, ")");
    clone->cname = stringcat(clone->cname, "_B");

    retType->symbol->defPoint->insertBefore(new DefExpr(clone));
    clone->addPragmas(&pragmas);
    if (clone->hasPragma( "synchronization primitive"))
      clone->type->defaultValue = NULL;
    clone->type->substitutions.copy(retType->substitutions);
    clone->type->dispatchParents.copy(retType->dispatchParents);
    forv_Vec(Type, t, retType->dispatchParents) {
      t->dispatchChildren.add(clone->type);
    }
    if (clone->type->dispatchChildren.n)
      INT_FATAL(this, "Generic type has subtypes");

    clone->type->instantiatedFrom = retType;

    if (count_instantiate_with_recursion(clone->type) >= 6)
      USR_FATAL(clone->type, "Recursive type instantiation limit reached");

    clone->type->substitutions.map_union(*generic_substitutions);

    newfn = instantiate_function(this, generic_substitutions, clone->type);
    clone->type->defaultConstructor = newfn;
    newfn->retType = clone->type;
    if (hasPragma("tuple"))
      instantiate_tuple(newfn);

  } else {
    newfn = instantiate_function(this, generic_substitutions, NULL);

    if (hasPragma("tuple get"))
      newfn = instantiate_tuple_get(newfn);

    if (hasPragma("tuple copy"))
      newfn = instantiate_tuple_copy(newfn);

    if (hasPragma("tuple init"))
      newfn = instantiate_tuple_init(newfn);

    if (hasPragma("tuple hash function"))  // finish generating hash function?
      newfn = instantiate_tuple_hash( newfn);
  }

  instantiatedTo->add(newfn);

  if (newfn->formals->length() > 1 &&
      newfn->getFormal(1)->type == dtMethodToken)
    newfn->getFormal(2)->type->methods.add(newfn);

  //normalize(newfn);
  tag_generic(newfn);

  return newfn;
}


void FnSymbol::codegenHeader(FILE* outfile) {
  retType->codegen(outfile);
  fprintf(outfile, " ");
  fprintf(outfile, "%s", cname);
  fprintf(outfile, "(");
  if (formals->length() == 0) {
    fprintf(outfile, "void");
  } else {
    bool first = true;
    for_formals(formal, this) {
      if (!first) {
        fprintf(outfile, ", ");
      }
      formal->codegenDef(outfile);
      first = false;
    }
  }
  fprintf(outfile, ")");
}


void FnSymbol::codegenPrototype(FILE* outfile) {
  codegenHeader(outfile);
  fprintf(outfile, ";\n");
}


static void
codegenNullAssignments(FILE* outfile, char* cname, ClassType* ct) {
  if (ct->classTag == CLASS_CLASS)
    fprintf(outfile, "%s = NULL;\n", cname);
  else {
    for_fields(field, ct) {
      if (ClassType* fct = dynamic_cast<ClassType*>(field->type)) {
        char buffer[1024];
        strcpy(buffer, cname);
        strcat(buffer, ".");
        strcat(buffer, field->cname);
        codegenNullAssignments(outfile, buffer, fct);
      }
    }
  }
}


void FnSymbol::codegenDef(FILE* outfile) {
  if (strcmp(saveCDir, "") && filename) {
    char* name = strrchr(filename, '/');
    name = (!name) ? filename : name + 1;
    fprintf(outfile, "/* %s:%d */\n", name, lineno);
  }

  codegenHeader(outfile);

  fprintf(outfile, " {\n");
  Vec<BaseAST*> asts;
  collect_top_asts(&asts, body);
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = dynamic_cast<DefExpr*>(ast))
      if (!dynamic_cast<TypeSymbol*>(def->sym))
        if (def->sym->parentScope->astParent->getModule()->block !=
            def->sym->parentScope->astParent)
          def->sym->codegenDef(outfile);
  }
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = dynamic_cast<DefExpr*>(ast)) {
      if (!dynamic_cast<TypeSymbol*>(def->sym)) {
        if (ClassType* ct = dynamic_cast<ClassType*>(def->sym->type)) {
          codegenNullAssignments(outfile, def->sym->cname, ct);
        }
      }
    }
  }
  body->codegen(outfile);
  fprintf(outfile, "}\n\n");
}


void
FnSymbol::insertAtHead(Expr* ast) {
  body->insertAtHead(ast);
}


void
FnSymbol::insertAtTail(Expr* ast) {
  body->insertAtTail(ast);
}


Symbol*
FnSymbol::getReturnSymbol() {
  CallExpr* ret = dynamic_cast<CallExpr*>(body->body->last());
  if (!ret || !ret->isPrimitive(PRIMITIVE_RETURN))
    INT_FATAL(this, "function is not normal");
  SymExpr* sym = dynamic_cast<SymExpr*>(ret->get(1));
  if (!sym)
    INT_FATAL(this, "function is not normal");
  return sym->var;
}


Symbol*
FnSymbol::getReturnLabel() {
  CallExpr* ret = dynamic_cast<CallExpr*>(body->body->last());
  if (!ret || !ret->isPrimitive(PRIMITIVE_RETURN))
    INT_FATAL(this, "function is not normal");
  if (DefExpr* def = dynamic_cast<DefExpr*>(ret->prev))
    if (Symbol* sym = dynamic_cast<LabelSymbol*>(def->sym))
      return sym;
  return NULL;
}


void
FnSymbol::insertBeforeReturn(Expr* ast) {
  CallExpr* ret = dynamic_cast<CallExpr*>(body->body->last());
  if (!ret || !ret->isPrimitive(PRIMITIVE_RETURN))
    INT_FATAL(this, "function is not normal");
  Expr* last = ret;
  if (DefExpr* def = dynamic_cast<DefExpr*>(last->prev))
    if (dynamic_cast<LabelSymbol*>(def->sym))
      last = last->prev; // label before return
  last->insertBefore(ast);
}


void
FnSymbol::insertBeforeReturnAfterLabel(Expr* ast) {
  CallExpr* ret = dynamic_cast<CallExpr*>(body->body->last());
  if (!ret || !ret->isPrimitive(PRIMITIVE_RETURN))
    INT_FATAL(this, "function is not normal");
  ret->insertBefore(ast);
}


void
FnSymbol::insertAtHead(AList* ast) {
  body->insertAtHead(ast);
}


void
FnSymbol::insertAtTail(AList* ast) {
  body->insertAtTail(ast);
}


void
FnSymbol::insertFormalAtHead(BaseAST* ast) {
  if (ArgSymbol* arg = dynamic_cast<ArgSymbol*>(ast))
    formals->insertAtHead(new DefExpr(arg));
  else if (DefExpr* def = dynamic_cast<DefExpr*>(ast))
    formals->insertAtHead(def);
  else
    INT_FATAL(ast, "Bad argument to FnSymbol::insertFormalAtHead");
}


void
FnSymbol::insertFormalAtTail(BaseAST* ast) {
  if (ArgSymbol* arg = dynamic_cast<ArgSymbol*>(ast))
    formals->insertAtTail(new DefExpr(arg));
  else if (DefExpr* def = dynamic_cast<DefExpr*>(ast))
    formals->insertAtTail(def);
  else
    INT_FATAL(ast, "Bad argument to FnSymbol::insertFormalAtTail");
}


ArgSymbol*
FnSymbol::getFormal(int i) {
  return dynamic_cast<ArgSymbol*>(dynamic_cast<DefExpr*>(formals->get(i))->sym);
}


EnumSymbol::EnumSymbol(char* init_name) :
  Symbol(SYMBOL_ENUM, init_name)
{ }


void EnumSymbol::verify() {
  Symbol::verify();
  if (astType != SYMBOL_ENUM) {
    INT_FATAL(this, "Bad EnumSymbol::astType");
  }
}


EnumSymbol*
EnumSymbol::copyInner(ASTMap* map) {
  return new EnumSymbol(name);
}

bool EnumSymbol::isParam(void) { return true; }

void EnumSymbol::codegenDef(FILE* outfile) { }


ModuleSymbol::ModuleSymbol(char* init_name, modType init_modtype) :
  Symbol(SYMBOL_MODULE, init_name),
  modtype(init_modtype),
  block(new BlockStmt()),
  initFn(NULL)
{
  rootScope->define(this);
  registerModule(this);
  block->blkScope = new SymScope(block, rootScope);
  block->parentSymbol = this;
  block->parentScope = rootScope;
}


ModuleSymbol::~ModuleSymbol() { }


void ModuleSymbol::verify() {
  Symbol::verify();
  if (astType != SYMBOL_MODULE) {
    INT_FATAL(this, "Bad ModuleSymbol::astType");
  }
}


ModuleSymbol*
ModuleSymbol::copyInner(ASTMap* map) {
  INT_FATAL(this, "Illegal call to ModuleSymbol::copy");
  return NULL;
}


void ModuleSymbol::codegenDef(FILE* outfile) {
  block->blkScope->codegenFunctions(outfile);
}


void ModuleSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == block) {
    block = dynamic_cast<BlockStmt*>(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in ModuleSymbol::replaceChild");
  }
}


LabelSymbol::LabelSymbol(char* init_name) :
  Symbol(SYMBOL_LABEL, init_name, NULL)
{ 
 
}


void LabelSymbol::verify() {
  Symbol::verify();
  if (astType != SYMBOL_LABEL) {
    INT_FATAL(this, "Bad LabelSymbol::astType");
  }
}

LabelSymbol* 
LabelSymbol::copyInner(ASTMap* map) {
  LabelSymbol* copy = new LabelSymbol(name);
  copy->cname = cname;
  return copy;
}

void LabelSymbol::codegenDef(FILE* outfile) { }

static int literal_id = 1;
HashMap<Immediate *, ImmHashFns, VarSymbol *> uniqueConstantsHash;


VarSymbol *new_StringSymbol(char *str) {
  Immediate imm;
  imm.const_kind = CONST_KIND_STRING;
  imm.v_string = canonicalize_string(str);
  VarSymbol *s = uniqueConstantsHash.get(&imm);
  if (s)
    return s;
  s = new VarSymbol("_literal_string", dtString);
  rootScope->define(s);
  s->immediate = new Immediate;
  *s->immediate = imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}

VarSymbol *new_IntSymbol(long long int b, IF1_int_type size) {
  Immediate imm;
  switch (size) {
  case INT_SIZE_8  : imm.v_int8   = b; break;
  case INT_SIZE_16 : imm.v_int16  = b; break;
  case INT_SIZE_32 : imm.v_int32  = b; break;
  case INT_SIZE_64 : imm.v_int64  = b; break;
    // case INT_SIZE_128: imm.v_int128 = b; break;
  default:
    INT_FATAL( "unknown INT_SIZE");
  }
  imm.const_kind = NUM_KIND_INT;
  imm.num_index = size;
  VarSymbol *s = uniqueConstantsHash.get(&imm);
  if (s)
    return s;
  s = new VarSymbol("_literal_int", dtInt[size]);
  rootScope->define(s);
  s->immediate = new Immediate;
  *s->immediate = imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}

VarSymbol *new_UIntSymbol(unsigned long long int b, IF1_int_type size) {
  Immediate imm;
  switch (size) {
  case INT_SIZE_8  : imm.v_uint8   = b; break;
  case INT_SIZE_16 : imm.v_uint16  = b; break;
  case INT_SIZE_32 : imm.v_uint32  = b; break;
  case INT_SIZE_64 : imm.v_uint64  = b; break;
    // case INT_SIZE_128: imm.v_uint128 = b; break;
  default:
    INT_FATAL( "unknown INT_SIZE");
  }
  imm.const_kind = NUM_KIND_UINT;
  imm.num_index = size;
  VarSymbol *s = uniqueConstantsHash.get(&imm);
  if (s)
    return s;
  s = new VarSymbol("_literal_uint", dtUInt[size]);
  rootScope->define(s);
  s->immediate = new Immediate;
  *s->immediate = imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}

VarSymbol *new_RealSymbol(char *n, long double b, IF1_float_type size) {
  Immediate imm;
  switch (size) {
  case FLOAT_SIZE_32  : imm.v_float32  = b; break;
  case FLOAT_SIZE_64  : imm.v_float64  = b; break;
  case FLOAT_SIZE_128 : imm.v_float128 = b; break;
  default:
    INT_FATAL( "unknown FLOAT_SIZE");
  }
  imm.const_kind = NUM_KIND_FLOAT;
  imm.num_index = size;
  VarSymbol *s = uniqueConstantsHash.get(&imm);
  if (s)
    return s;
  s = new VarSymbol(stringcat("_literal_", intstring(literal_id++)), dtReal[size]);
  rootScope->define(s);
  s->cname = dupstr(n);
  s->immediate = new Immediate;
  *s->immediate = imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}

VarSymbol *new_ImagSymbol(char *n, long double b, IF1_float_type size) {
  Immediate imm;
  switch (size) {
  case FLOAT_SIZE_32  : imm.v_float32  = b; break;
  case FLOAT_SIZE_64  : imm.v_float64  = b; break;
  case FLOAT_SIZE_128 : imm.v_float128 = b; break;
  default:
    INT_FATAL( "unknown FLOAT_SIZE");
  }
  imm.const_kind = NUM_KIND_IMAG;
  imm.num_index = size;
  VarSymbol *s = uniqueConstantsHash.get(&imm);
  if (s)
    return s;
  s = new VarSymbol(stringcat("_literal_", intstring(literal_id++)), dtImag[size]);
  rootScope->define(s);
  s->cname = dupstr(n);
  s->immediate = new Immediate;
  *s->immediate = imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}

VarSymbol *new_ComplexSymbol(char *n, long double r, long double i, IF1_complex_type size) {
  Immediate imm;
  switch (size) {
  case COMPLEX_SIZE_64: 
    imm.v_complex64.r  = r; 
    imm.v_complex64.i  = i; 
    break;
  case COMPLEX_SIZE_128: 
    imm.v_complex128.r = r; 
    imm.v_complex128.i = i; 
    break;
  case COMPLEX_SIZE_256: 
    imm.v_complex256.r = r; 
    imm.v_complex256.i = i; 
    break;
  default:
    INT_FATAL( "unknown COMPLEX_SIZE for complex");
  }
  imm.const_kind = NUM_KIND_COMPLEX;
  imm.num_index = size;
  VarSymbol *s = uniqueConstantsHash.get(&imm);
  if (s)
    return s;
  s = new VarSymbol(stringcat("_literal_", intstring(literal_id++)), dtComplex[size]);
  rootScope->define(s);
  s->immediate = new Immediate;
  s->cname = dupstr(n);
  *s->immediate = imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}

VarSymbol *new_ImmediateSymbol(Immediate *imm) {
  VarSymbol *s = uniqueConstantsHash.get(imm);
  if (s)
    return s;
  PrimitiveType *t = immediate_type(imm);
  s = new VarSymbol(stringcat("_literal_", intstring(literal_id++)), t);
  s->immediate = new Immediate;
  char str[512], *ss = str;
  if (imm->const_kind == CONST_KIND_STRING)
    ss = imm->v_string;
  else
    sprint_imm(str, *imm);
  s->cname = dupstr(ss);
  *s->immediate = *imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}

PrimitiveType *
immediate_type(Immediate *imm) {
  switch (imm->const_kind) {
    case CONST_KIND_STRING: 
      return dtString;
    case NUM_KIND_UINT:
      return dtUInt[imm->num_index];
    case NUM_KIND_INT:
      return dtInt[imm->num_index];
    case NUM_KIND_FLOAT:
      return dtReal[imm->num_index];
    case NUM_KIND_IMAG:
      return dtImag[imm->num_index];
    case NUM_KIND_COMPLEX:
      return dtComplex[imm->num_index];
    default: 
      USR_FATAL("bad immediate type");
      break;
  }
  return NULL;
}


int
compareSymbol(const void* v1, const void* v2) {
  Symbol* s1 = *(Symbol**)v1;
  Symbol* s2 = *(Symbol**)v2;
  ModuleSymbol* m1 = s1->getModule();
  ModuleSymbol* m2 = s2->getModule();
  if (m1 != m2) {
    if (m1->modtype < m2->modtype)
      return -1;
    if (m1->modtype > m2->modtype)
      return 1;
    return strcmp(m1->cname, m2->cname);
  }
  int result = strcmp(s1->type->symbol->cname, s2->type->symbol->cname);
  if (!result)
    result = strcmp(s1->cname, s2->cname);
  return result;
}
