#include <typeinfo>
#include "geysa.h"
#include "analysis.h"
#include "chapel_ifa.h"
#include "expr.h"
#include "files.h"
#include "baseAST.h"
#include "misc.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"
#include "builtin.h"
#include "if1.h"
#include "fa.h"
#include "pdb.h"
#include "pnode.h"
#include "var.h"
#include "fun.h"
#include "prim.h"
#include "pattern.h"
#include "clone.h"
#include "ast.h"
#include "../passes/preAnalysisCleanup.h"

#define VARARG_END     0ll
#define MAKE_USER_TYPE_BE_DEFINITION            1
//#define USE_SCOPE_LOOKUP_CACHE                  1
//#define MINIMIZED_MEMORY 1  // minimize the memory used by Sym's... needs valgrind checking for safety

#define SYMBOL(_x) (((ASymbol*)(_x)->asymbol)->symbol)

#define OPERATOR_CHAR(_c) \
(((_c > ' ' && _c < '0') || (_c > '9' && _c < 'A') || \
  (_c > 'Z' && _c < 'a') || (_c > 'z')) &&            \
   _c != '_'&& _c != '?' && _c != '$')                \

Vec<AError *> analysis_errors;

class LabelMap : public Map<char *, Stmt *> {};

class CloneCallback : public gc {
 public:
  virtual void clone(BaseAST* old_ast, BaseAST* new_ast) = 0;
};

class AnalysisCloneCallback : public CloneCallback {
 public:
  ASTCopyContext *context;
  void clone(BaseAST* old_ast, BaseAST* new_ast);
  AnalysisCloneCallback() : context(0) {}
};

static Sym *expr_simple_seq_symbol = 0;
static Sym *expr_domain_symbol = 0;
static Sym *expr_create_domain_symbol = 0;
static Sym *expr_reduce_symbol = 0;
static Sym *write_symbol = 0;
static Sym *writeln_symbol = 0;
static Sym *read_symbol = 0;
static Sym *array_index_symbol = 0;
static Sym *array_set_symbol = 0;
static Sym *cast_symbol = 0;
static Sym *method_token = 0;
static Sym *setter_token = 0;
static Sym *make_seq_symbol = 0;
static Sym *chapel_defexpr_symbol = 0;

static Sym *sym_index = 0;
static Sym *sym_domain = 0;
static Sym *sym_array = 0;
static Sym *sym_sequence = 0;
static Sym *sym_locale = 0;

static int init_function(FnSymbol *f);
static int build_function(FnSymbol *f);
static void map_asts(Vec<BaseAST *> &syms);
static void build_symbols(Vec<BaseAST *> &syms);
static void build_types(Vec<BaseAST *> &syms, Vec<Type *> *types = NULL);
static void build_classes(Vec<BaseAST *> &syms);
static int gen_if1(BaseAST *ast, BaseAST *parent = 0);
static void finalize_function(Fun *fun);
static Type * to_AST_type(Sym *type);

class ScopeLookupCache : public Map<char *, Vec<Fun *> *> {};
static ScopeLookupCache universal_lookup_cache;
static int finalized_symbols = 0;
static Map<Sym *, SymExpr *> constant_cache;

void
init_chapel_ifa() {
  ifa_init(new ACallbacks);
}

struct TraverseASTs {
  Vec<BaseAST *> asts;
};

#define GET_ALL_CHILDREN(_s, _x) \
  TraverseASTs _x;\
  get_ast_children(_s, _x.asts, 1);
#define GET_AST_CHILDREN(_s, _x) \
  TraverseASTs _x;\
  get_ast_children(_s, _x.asts);

ASymbol::ASymbol() : symbol(0) {
}

char *
cannonicalize_string(char *s) {
  return if1_cannonicalize_string(if1, s);
}

char* 
ASymbol::pathname() {
  if (symbol && symbol->filename)
    return symbol->filename;
  else
    return 0;
}

int 
ASymbol::line() {
  if (symbol && symbol->lineno)
    return symbol->lineno;
  else
    return 0;
}

int 
ASymbol::log_line() {
  if (symbol) {
    ModuleSymbol *m = symbol->getModule();
    if (m && m->modtype == MOD_USER)
      return symbol->lineno;
  }
  return 0;
}

int 
ASymbol::ast_id() {
  if (symbol)
    return symbol->id;
  else
    return 0;
}

AAST::AAST() : xast(0), code(0), sym(0), rval(0) {
  label[0] = label[1] = 0;
}

char
*AAST::pathname() { 
  return xast->filename;
}

int
AAST::line() {
  return xast->lineno;
}

Sym *
AAST::symbol() {
  if (rval) return rval;
  return sym;
}

IFAAST*
AAST::copy_node(ASTCopyContext* context) {
  AAST *a = new AAST(*this);
  for (int i = 0; i < a->pnodes.n; i++)
    a->pnodes.v[i] = context->nmap->get(a->pnodes.v[i]);
  return a;
}

Vec<Fun *> *
AAST::visible_functions(Sym *arg0) {
  if (arg0->fun) {
    Fun *f = arg0->fun;
    if (!arg0->fun->vec_of_one) {
      arg0->fun->vec_of_one = new Vec<Fun *>;
      arg0->fun->vec_of_one->add(f);
    }
    return arg0->fun->vec_of_one;
  }
  Vec<Fun *> *v = 0;
  char *name = 0;
  if (arg0->is_symbol)
    name = arg0->name;
  else
    name = if1_cannonicalize_string(if1, "this");
  SymScope* scope = this->xast->parentScope;
#ifdef USE_SCOPE_LOOKUP_CACHE
  ScopeLookupCache *sym_cache = 0;
  if (sym_cache && (v = sym_cache->get(name))) 
    return v;
#endif
  Vec<FnSymbol *> fss;
  scope->getVisibleFunctions(&fss, name);
  v = new Vec<Fun *>;
  forv_Vec(FnSymbol, x, fss)
    v->set_add(x->asymbol->sym->fun);
  Vec<Fun *> *universal = universal_lookup_cache.get(name);
  if (universal)
    v->set_union(*universal);
#ifdef USE_SCOPE_LOOKUP_CACHE
  if (!sym_cache)
    sym_cache = scope->lookupCache = new ScopeLookupCache;
  sym_cache->put(name, v);
#endif
  return v;
}

void
AnalysisCloneCallback::clone(BaseAST* old_ast, BaseAST* new_ast) {
  if (isSomeStmt(new_ast->astType)) {
    Stmt *new_s = dynamic_cast<Stmt*>(new_ast), *old_s = dynamic_cast<Stmt*>(old_ast);
    if (old_s->ainfo) {
      new_s->ainfo = (AAST*)old_s->ainfo->copy_node(context);
      new_s->ainfo->xast = new_s;
    }
  } else if (isSomeExpr(new_ast->astType)) {
    Expr *new_e = dynamic_cast<Expr*>(new_ast), *old_e = dynamic_cast<Expr*>(old_ast);
    if (old_e->ainfo) {
      new_e->ainfo = (AAST*)old_e->ainfo->copy_node(context);
      new_e->ainfo->xast = new_e;
    }
  } else if (isSomeSymbol(new_ast->astType)) {
    Symbol *new_s = dynamic_cast<Symbol*>(new_ast), *old_s = dynamic_cast<Symbol*>(old_ast);
    if (old_s->asymbol) {
      new_s->asymbol = old_s->asymbol->copy();
      SYMBOL(new_s) = new_s;
      context->smap.put(old_s->asymbol->sym, new_s->asymbol->sym);
      if (context->vmap && old_s->asymbol->sym->var) {
        new_s->asymbol->sym->var = context->vmap->get(old_s->asymbol->sym->var);
        if (!new_s->asymbol->sym->var)
          new_s->asymbol->sym->var = old_s->asymbol->sym->var;
      }
      if (old_s->asymbol->sym->fun) {
        Fun *new_f = context->fmap.get(old_s->asymbol->sym->fun);
        if (new_f)
          new_s->asymbol->sym->fun = new_f;
      }
    }
  } else if (isSomeType(new_ast->astType)) {
    Type *new_s = dynamic_cast<Type*>(new_ast), *old_s = dynamic_cast<Type*>(old_ast);
    if (old_s->asymbol) {
      new_s->asymbol = old_s->asymbol->copy();
      SYMBOL(new_s) = new_s;
      context->smap.put(old_s->asymbol->sym, new_s->asymbol->sym);
    }
  } else
    fail("clone of Type unsupported");
}

IFAAST *
AAST::copy_tree(ASTCopyContext* context) {
  AnalysisCloneCallback callback;
  callback.context = context;
  ASTMap clone_map;
  DefExpr* def_expr = dynamic_cast<DefExpr*>(xast);
  FnSymbol* orig_fn = dynamic_cast<FnSymbol*>(def_expr->sym);
  FnSymbol *new_fn = orig_fn->clone(&clone_map);
  for (int i = 0; i < clone_map.n; i++)
    if (clone_map.v[i].key)
      callback.clone(clone_map.v[i].key, clone_map.v[i].value);
  return new_fn->defPoint->ainfo;
}

static void
close_symbols(Vec<AList<Stmt> *> &stmts, Vec<BaseAST *> &syms) {
  Vec<BaseAST *> set;
  forv_Vec(AList<Stmt>*, a, stmts) {
    Stmt* stmt = a->first();
    while (stmt) {
      set.set_add(stmt);
      syms.add(stmt);
      stmt = a->next();
    }
  }
  forv_BaseAST(s, syms) {
    GET_ALL_CHILDREN(s, getStuff);
    forv_BaseAST(ss, getStuff.asts) {
      assert(ss);
      if (set.set_add(ss))
        syms.add(ss);
    }
    if (s->astType == SYMBOL_FN) {
      Vec<BaseAST*> asts;
      collect_asts(&asts, dynamic_cast<FnSymbol*>(s));
      forv_BaseAST(x, asts)
        if (set.set_add(x))
          syms.add(x);
    }
  }
  forv_Type(t, builtinTypes) {
    if (set.set_add(t))
      syms.add(t);
    if (t->symbol)
      if (set.set_add(t->symbol))
        syms.add(t->symbol);
  }
}

static Sym *
base_type(Sym *s) {
  if (UserType *t = dynamic_cast<UserType*>(SYMBOL(s)))
    return base_type(t->underlyingType->asymbol->sym);
  return s;
}

static void
set_global_scope(Sym *s) {
  s->function_scope = 0;
  s->global_scope = 1;
}

static ASymbol *
new_ASymbol(char *name) {
  ASymbol *s = new ASymbol;
  s->sym = new Sym;
  s->sym->asymbol = s;
  if1_register_sym(if1, s->sym, name);
  return s;
}

static ASymbol *
new_ASymbol(Symbol *symbol, int basic = 0) {
  char *name = 0;
  if (symbol)
    name = symbol->name;
  ASymbol *s = new ASymbol;
  if (basic)
#ifdef MINIMIZED_MEMORY
    s->sym = (Sym*)new BasicSym;
#else
    s->sym = (Sym*)new Sym;
#endif
  else
    s->sym = new Sym;
  s->sym->asymbol = s;
  if1_register_sym(if1, s->sym, name);
  s->symbol = symbol;
  return s;
}

static ASymbol *
new_ASymbol(Symbol *symbol, Sym *sym) {
  char *name = 0;
  if (symbol)
    name = symbol->name;
  ASymbol *s = (ASymbol*)sym->asymbol;
  if (!s) {
    s = new ASymbol;
    sym->asymbol = s;
  }
  assert(!s->symbol || s->symbol == symbol);
  assert(!s->sym || s->sym == sym);
  s->sym = sym;
  s->sym->asymbol = s;  
  s->symbol = symbol;
  return s;
}

ASymbol * 
ASymbol::copy() {
  ASymbol *s = new ASymbol;
  s->sym = sym->copy();
  s->sym->asymbol = s;
  if (s->sym->type_kind != Type_NONE)
    s->sym->type = s->sym;
  return s;
}

Sym *
ACallbacks::make_LUB_type(Sym *t) {
  Vec<Type *> types;
  Sym *basic = 0;
  forv_Sym(s, t->has) {
    Sym *b = to_basic_type(s);
    if (b) {
      if (!basic)
        basic = b;
      else if (basic != b)
        fail("mixed primitive types");
    }
    Type *ttt = dynamic_cast<Type *>(SYMBOL(s));
    if (ttt)
      types.set_add(ttt);
  }
  types.set_to_vec();
  if (types.n == 0)
    return sym_void;
  if (types.n == 1)
    return types.v[0]->asymbol->sym;
  Type *tt = find_or_make_sum_type(&types);
  if (tt->asymbol)
    return tt->asymbol->sym;
  make_meta_type(t);
  tt->symbol->asymbol = (ASymbol*)t->meta_type->asymbol;
  tt->symbol->asymbol->sym = t->meta_type;
  SYMBOL(tt->symbol) = tt->symbol;
  tt->asymbol = (ASymbol*)t->asymbol;
  tt->asymbol->sym = t;
  SYMBOL(tt) = tt;
  assert(tt->asymbol->sym->type_kind == Type_LUB);
  return t;
}

Sym *
ACallbacks::new_Sym(char *name) {
  return new_ASymbol(name)->sym;
}

static void
finalize_symbols(IF1 *i) {
  for (int x = finalized_symbols; x < i->allsyms.n; x++) {
    Sym *s = i->allsyms.v[x];
    if (SYMBOL(s)) {
      if (Type* type = dynamic_cast<Type*>(SYMBOL(s))) {
        if (type->instantiatedFrom) {
          assert(!s->instantiates || s->instantiates == type->instantiatedFrom->asymbol->sym);
          s->instantiates = type->instantiatedFrom->asymbol->sym;
        }
      }
      if (FnSymbol* fn = dynamic_cast<FnSymbol*>(SYMBOL(s))) {
        if (fn->instantiatedFrom)
          s->instantiates = fn->instantiatedFrom->asymbol->sym;
      }
    }
    if (s->is_constant)
      make_meta_type(s);
    if (s->is_constant || s->is_symbol)
      set_global_scope(s);
    else
      if (s->type_kind)
        set_global_scope(s);
    if (s->asymbol && SYMBOL(s) && SYMBOL(s)->astType == SYMBOL_VAR) {
      VarSymbol *v = (VarSymbol*)SYMBOL(s);
      if (v->type && v->type != dtUnknown) {
        Sym *t = v->type->asymbol->sym;
        if (t->num_kind)
          s->type = t;
        if (s->is_constant)
          s->type = t;
      }
    }
    compute_type_size(s);
  }
  finalized_symbols = i->allsyms.n;
}

static Sym *
BaseAST_to_Sym(BaseAST *b) {
  if (Symbol *s = dynamic_cast<Symbol *>(b))
    return s->asymbol->sym;
  if (Type *t = dynamic_cast<Type *>(b))
    return t->asymbol->sym;
  assert(!"BaseAST_to_Sym");
  return 0;
}

static Fun *
install_new_asts(FnSymbol *f, Vec<FnSymbol *> &funs, Vec<TypeSymbol *> &types) {
  Vec<BaseAST *> syms;
  forv_Vec(FnSymbol, f, funs) {
    collect_asts(&syms, f->defPoint->parentStmt);
  }
  forv_Vec(TypeSymbol, t, types) {
    collect_asts(&syms, t->defPoint->parentStmt);
  }
  qsort(syms.v, syms.n, sizeof(syms.v[0]), compar_baseast);
  qsort(funs.v, funs.n, sizeof(funs.v[0]), compar_baseast);
  map_asts(syms);
  build_types(syms);
  build_symbols(syms);
  forv_Vec(TypeSymbol, new_ts, types) {
    Type* new_t = new_ts->definition;
    new_t->asymbol->sym->instantiates = new_t->instantiatedFrom->asymbol->sym;
    for (int i; i < new_t->substitutions.n; i++) if (new_t->substitutions.v[i].key) {
      Sym *value = BaseAST_to_Sym(new_t->substitutions.v[i].value);
      // don't map yourself
      if (value != new_t->asymbol->sym)
        new_t->asymbol->sym->substitutions.put(
          BaseAST_to_Sym(new_t->substitutions.v[i].key), value);
    }
  }
  finalize_types(if1, false);
  forv_Vec(FnSymbol, f, funs) {
    if (init_function(f) < 0 || build_function(f) < 0) 
      fail("unable to instantiate generic/wrapper");
    if1_finalize_closure(if1, f->asymbol->sym);
  }
  finalize_symbols(if1);
  build_type_hierarchy();
  build_classes(syms);
  finalize_types(if1, false);
  forv_Vec(FnSymbol, f, funs) {
    Fun *fun = new Fun(f->asymbol->sym);
    build_arg_positions(fun);
    pdb->add(fun);
  }
  forv_BaseAST(ast, syms) {
    if (Symbol *s = dynamic_cast<Symbol *>(ast))
      initialize_Sym_for_fa(s->asymbol->sym);
    else if (Type *t = dynamic_cast<Type *>(ast))
      initialize_Sym_for_fa(t->asymbol->sym);
  }
  forv_Vec(FnSymbol, f, funs) {
    build_patterns(pdb->fa, f->asymbol->sym->fun);
    finalize_function(f->asymbol->sym->fun);
  }
  if1_write_log();
  return f->asymbol->sym->fun;
}

static Fun *
install_new_asts(FnSymbol *f) {
  Vec<FnSymbol *> funs;
  funs.add(f);
  Vec<TypeSymbol *> types;
  return install_new_asts(f, funs, types);
}

static Type *
Sym_to_Type(Sym *s) {
  if (Type *t = dynamic_cast<Type*>(SYMBOL(s)))
    return t;
  return (dynamic_cast<TypeSymbol*>(SYMBOL(s)))->definition;
}

static SymExpr *
get_constant_SymExpr(Sym *c) {
  SymExpr *e = constant_cache.get(c);
  if (e)
    return e;
  e = new SymExpr(dynamic_cast<Symbol*>(SYMBOL(c)));  
  constant_cache.put(c, e);
  return e;
}

Sym *
ACallbacks::instantiate(Sym *s, Map<Sym *, Sym *> &substitutions) {
  Sym *tt = substitutions.get(s);
  if (tt) {
    Type *type = NULL;
    if (ArgSymbol *p = dynamic_cast<ArgSymbol*>(SYMBOL(s))) {
      if (p->isGeneric && p->genericSymbol)
        if (TypeSymbol *ts = dynamic_cast<TypeSymbol*>(p->genericSymbol))
          type = ts->definition;
    } else
      type = dynamic_cast<Type*>(SYMBOL(s));
    if (!type)
      return 0;
    ASTMap subs;
    form_SymSym(ss, substitutions) {
      if (ArgSymbol *p = dynamic_cast<ArgSymbol*>(SYMBOL(ss->key))) {
        if (p->isGeneric && p->genericSymbol && p->genericSymbol->astType == SYMBOL_TYPE)
          subs.put(dynamic_cast<TypeSymbol*>(p->genericSymbol)->definition, Sym_to_Type(ss->value));
        else
          subs.put(p, get_constant_SymExpr(ss->value));
      } else
        subs.put(dynamic_cast<Type*>(SYMBOL(ss->key)), Sym_to_Type(ss->value));
    }
    Type *new_type = type->instantiate_generic(subs);
    return new_type->asymbol->sym;
  }
  return 0;
}

Sym *
ACallbacks::formal_to_generic(Sym *s) {
  ArgSymbol *p = dynamic_cast<ArgSymbol*>(SYMBOL(s));
  if (!p->isGeneric || !p->genericSymbol)
    return 0;
  if ( p->genericSymbol->astType == SYMBOL_TYPE)
    return dynamic_cast<TypeSymbol*>(p->genericSymbol)->definition->asymbol->sym;
  return p->genericSymbol->asymbol->sym;
}

Fun *
ACallbacks::order_wrapper(Fun *f, Map<MPosition *, MPosition *> &substitutions) {
  if (!f->ast) 
    return NULL;
  FnSymbol *fndef = dynamic_cast<FnSymbol *>(SYMBOL(f->sym));
  Map<Symbol *, Symbol *> formals_to_formals;
  forv_MPosition(p, f->positional_arg_positions) {
    Sym *sym1 = f->arg_syms.get(p);
    MPosition *acp = substitutions.get(p);
    MPosition *p2 = acp ? acp : p;
    Sym *sym2 = f->arg_syms.get(p2);
    Symbol *symbol1 = dynamic_cast<Symbol*>(SYMBOL(sym1));
    Symbol *symbol2 = dynamic_cast<Symbol*>(SYMBOL(sym2));   
    if (symbol1 || symbol2) {
      assert(symbol1 && symbol2);
      formals_to_formals.put(symbol2, symbol1);
    }
  }
  FnSymbol *fsym = fndef->order_wrapper(&formals_to_formals);
  Fun *fun = install_new_asts(fsym);
  fun->wraps = f;
  return fun;
}

Fun *
ACallbacks::coercion_wrapper(Fun *f, Map<MPosition *, Sym *> &substitutions) {
  if (!f->ast) 
    return NULL;
  Map<Symbol *, Symbol *> coercions;
  forv_MPosition(p, f->positional_arg_positions) {
    Sym *sym = f->arg_syms.get(p);
    Symbol *symbol = sym->asymbol ? dynamic_cast<Symbol*>(SYMBOL(sym)) : 0;
    if (symbol) {
      Sym *type_sym = substitutions.get(p);
      if (type_sym) {
        Type *type = dynamic_cast<Type*>(SYMBOL(type_sym));
        Sym *a = f->arg_syms.get(p);
        if (a->asymbol && SYMBOL(a)) {
          Symbol *aa = dynamic_cast<Symbol*>(SYMBOL(a));
          coercions.put(aa, type->symbol);
        }
      }
    }
  }
  FnSymbol *fndef = dynamic_cast<FnSymbol *>(SYMBOL(f->sym));
  FnSymbol *fsym = fndef->coercion_wrapper(&coercions);
  Fun *fun = install_new_asts(fsym);
  fun->wraps = f;
  return fun;
}

Fun *
ACallbacks::default_wrapper(Fun *f, Vec<MPosition *> &default_args) {
  if (!f->ast) 
    return NULL;
  Vec<Symbol *> defaults;
  forv_MPosition(p, default_args) {
    Sym *sym = f->arg_syms.get(p);
    Symbol *symbol = sym->asymbol ? dynamic_cast<Symbol*>(SYMBOL(sym)) : 0;
    if (symbol)
      defaults.set_add(symbol);
  }
  FnSymbol *fndef = dynamic_cast<FnSymbol *>(SYMBOL(f->sym));
  FnSymbol *fsym = fndef->default_wrapper(&defaults);
  Fun *fun = install_new_asts(fsym);
  fun->wraps = f;
  return fun;
}

Fun *
ACallbacks::instantiate_generic(Fun *f, Map<Sym *, Sym *> &generic_substitutions) {
  if (!f->ast) 
    return NULL;
  ASTMap substitutions;
  form_SymSym(s, generic_substitutions) {
    Type *t = dynamic_cast<Type*>(SYMBOL(s->key));
#if 0
    // treat type variables as ?t
    if (!t)
      if (TypeSymbol *ts = dynamic_cast<TypeSymbol*>(SYMBOL(s->key)))
        t = ts->definition;
#endif
    ArgSymbol *p = dynamic_cast<ArgSymbol*>(SYMBOL(s->key));
    if (!t)
      if (p && p->isGeneric && p->genericSymbol && p->genericSymbol->astType == SYMBOL_TYPE)
        t = dynamic_cast<TypeSymbol*>(p->genericSymbol)->definition;
    if (t)
      substitutions.put(t, Sym_to_Type(s->value));
    else
      substitutions.put(p, get_constant_SymExpr(s->value)->var);
  }
  FnSymbol *fndef = dynamic_cast<FnSymbol *>(SYMBOL(f->sym));
  ASTMap map;
  Vec<FnSymbol*> new_functions;
  Vec<TypeSymbol*> new_types;
  FnSymbol *fsym = fndef->instantiate_generic(&substitutions, &new_functions, &new_types);
  Fun *fun = install_new_asts(fsym, new_functions, new_types);
  fun->wraps = f;
  return fun;
}

Sym *
ASymbol::clone() {
  if (!symbol) { // internal to analysis
    Sym *s = copy()->sym;
    return s;
  } else {
    AnalysisCloneCallback callback;
    ASTCopyContext context;
    callback.context = &context;
    Type *type = dynamic_cast<Type*>(symbol);
    TypeSymbol *old_type_symbol = dynamic_cast<TypeSymbol*>(type->symbol);
    ASTMap clone_map;
    TypeSymbol *new_type_symbol = old_type_symbol->clone(&clone_map);
    assert(new_type_symbol);
    for (int i = 0; i < clone_map.n; i++)
      if (clone_map.v[i].key)
        callback.clone(clone_map.v[i].key, clone_map.v[i].value);
    Sym *new_type = callback.context->smap.get(old_type_symbol->definition->asymbol->sym);
    if (!new_type_symbol->asymbol) // SHOULD BE ASSERT
      callback.clone(old_type_symbol, new_type_symbol);
    new_type->meta_type = new_type_symbol->asymbol->sym;
    new_type_symbol->asymbol->sym->meta_type = new_type;
    assert(new_type_symbol->asymbol->sym->is_meta_type);
    for (int i = 0; i < new_type->has.n; i++) {
      Sym *s = callback.context->smap.get(new_type->has.v[i]);
      assert(s);
      new_type->has.v[i] = s;
    }
    return new_type;
  }
}

static Sym *
new_sym(char *name = 0, int global = 0) {
  Sym *s = new_ASymbol(name)->sym;
  if (!global)
    s->function_scope = 1;
  else
    s->global_scope = 1;
  return s;
}

static void
map_type(Type *t) {
  if (t->symbol) {
    t->asymbol = new_ASymbol(t->symbol->name);
    SYMBOL(t) = t;
  } else {
    t->asymbol = new_ASymbol("<anonymous>");
    SYMBOL(t) = t;
  }
}

VarSymbol *
is_literal(Symbol* sym) {
  if (VarSymbol* var = dynamic_cast<VarSymbol*>(sym))
    if (var->immediate)
      return var;
  return 0;
}

Sym *
map_const(IF1 *p, Immediate *imm) {
  Sym *sym = p->constants.get(imm);
  if (sym)
    return sym;
  sym = new_Sym();
  sym->is_constant = 1;
  sym->imm = *imm;
  p->constants.put(&sym->imm, sym);
  return sym;
}

static void
map_baseast(BaseAST *s) {
  Symbol *sym = dynamic_cast<Symbol *>(s);
  if (sym) {
    if (sym->asymbol)
      return;
    if (VarSymbol *var = is_literal(sym)) {
      Sym *s = map_const(if1, var->immediate);
      sym->asymbol = new_ASymbol(sym, s);
    } else {
      int basic = (s->astType != SYMBOL_FN) && (s->astType != SYMBOL_ENUM);
      sym->asymbol = new_ASymbol(sym, basic);
      SYMBOL(sym) = sym;
    
      if (!sym->parentScope) {
        sym->asymbol->sym->global_scope = 1;
      } else {
        switch (sym->parentScope->type) {
          default: assert(0);
          case SCOPE_INTRINSIC:
          case SCOPE_PRELUDE:
          case SCOPE_MODULE:
          case SCOPE_POSTPARSE:
            sym->asymbol->sym->global_scope = 1;
            break;
          case SCOPE_LETEXPR:
          case SCOPE_ARG:
          case SCOPE_LOCAL:
          case SCOPE_FORLOOP:
            sym->asymbol->sym->function_scope = 1;
            sym->asymbol->sym->nesting_depth = sym->nestingDepth();
            break;
          case SCOPE_CLASS: // handled as the symbols appears in code
            break;
        }
      }
    }
    if (verbose_level > 2 && sym->name)
      printf("map_asts: found Symbol '%s'\n", sym->name);
  } else {
    Type *t = dynamic_cast<Type *>(s);
    if (t) {
      if (t->asymbol)
        return;
      map_type(t);
    } else {
      Expr *e = dynamic_cast<Expr *>(s);
      if (e) {
        if (e->ainfo)
          return;
        e->ainfo = new AAST;
        e->ainfo->xast = e;
      } else {
        Stmt *st = dynamic_cast<Stmt *>(s);
        if (st) {
          if (st->ainfo)
            return;
          st->ainfo = new AAST;
          st->ainfo->xast = s;
        } else {
          INT_FATAL(s, "Unexpected AST type in map_baseast: %s\n", astTypeName[s->astType]);
        }
      }
    }
  }
}

static void
map_asts(Vec<BaseAST *> &syms) {
  if (verbose_level > 2)
    printf("map_asts: %d\n", syms.n);
  forv_BaseAST(s, syms)
    map_baseast(s);
}

static void
build_enum_element(Sym *enum_sym, Sym *element_sym, int i) {
  element_sym->inherits_add(enum_sym);
  element_sym->type = enum_sym;
  element_sym->meta_type = enum_sym->meta_type;
  element_sym->imm.v_int64 = i;
  element_sym->is_constant = 1;
}

static int
is_reference_type(BaseAST *at) {
  return is_Reference_Type(dynamic_cast<Type*>(at));
}

static int
is_scalar_type(BaseAST *at) {
  Type *t = dynamic_cast<Type*>(at);
  assert(t);
  if (t == dtString)
    return true; // EXCEPTION: the low level treats strings as scalars!
  return is_Scalar_Type(t);
}

static int
is_scalar_type_symbol(BaseAST *at) {
  TypeSymbol *ts = dynamic_cast<TypeSymbol*>(at);
  assert(ts);
  return is_scalar_type(ts->definition);
}

static inline int
is_scalar_or_reference_type(Type *t) {
  return is_scalar_type(t) || is_reference_type(t);
}

static void
build_symbols(Vec<BaseAST *> &syms) {
  forv_BaseAST(ss, syms) {
    Symbol *s = dynamic_cast<Symbol *>(ss);
    if (s) { 
      switch (s->astType) {
        case SYMBOL_TYPE: {
          TypeSymbol *t = dynamic_cast<TypeSymbol*>(s);
          if (t->definition->astType == TYPE_VARIABLE)
            t->asymbol->sym->must_specialize = sym_anyclass;
          break;
        }
        case SYMBOL_ARG: {
          ArgSymbol *p = dynamic_cast<ArgSymbol*>(s);
          Sym *psym = s->asymbol->sym;
          switch (p->intent) {
            default: break;
            case INTENT_IN: psym->intent = Sym_IN; break;
            case INTENT_INOUT: psym->intent = Sym_INOUT; break;
            case INTENT_OUT: psym->intent = Sym_OUT; break;
            case INTENT_CONST: psym->is_read_only = 1; break;
          }
          if (p->hasPragma("clone_for_constants"))
            psym->clone_for_constants = 1;
          ClassType *rt = dynamic_cast<ClassType*>(p->type);
          if (rt && rt->isPattern) {
            p->asymbol->sym->is_pattern = 1;
            forv_Sym(s, rt->asymbol->sym->has)
              p->asymbol->sym->has.add(s);
          }
          if (p->isGeneric)
            s->asymbol->sym->is_generic = 1;
          if (s->type->astType == TYPE_META) {
            MetaType *t = dynamic_cast<MetaType*>(s->type);
            s->asymbol->sym->must_specialize = t->asymbol->sym;
          } else {
            if (!p->asymbol->sym->is_pattern && s->type && s->type != dtUnknown) {
              if (s->asymbol->sym->intent != Sym_OUT)
                s->asymbol->sym->must_implement_and_specialize(s->type->asymbol->sym);
              else
                s->asymbol->sym->must_implement = s->type->asymbol->sym;
            }
          }
          break;
        }
        default: break;
      }
    }
  }
}

static void
build_patterns(Vec<BaseAST *> &syms) {
  forv_BaseAST(ss, syms) {
    Symbol *s = dynamic_cast<Symbol *>(ss);
    if (s) { 
      switch (s->astType) {
        case SYMBOL_ARG: {
          ArgSymbol *p = dynamic_cast<ArgSymbol*>(s);
          ClassType *rt = dynamic_cast<ClassType*>(p->type);
          if (rt && rt->isPattern) {
            forv_Sym(s, rt->asymbol->sym->has)
              p->asymbol->sym->has.add(s);
          }
        }
        default: break;
      }
    }
  }
}

static Sym *
get_defaultVal(Type *t) {
  return t->defaultValue->asymbol->sym;
}

static Sym *
build_type(Type *t, bool make_default = true) {
  if (t->symbol) {
    t->asymbol->sym->meta_type = t->symbol->asymbol->sym;
    t->symbol->asymbol->sym->meta_type = t->asymbol->sym;
  }
  make_meta_type(t->asymbol->sym);
  if (make_default && t->defaultValue)
    get_defaultVal(t);
  switch (t->astType) {
    default: assert(!"case");
    case TYPE:
      t->asymbol->sym->type_kind = Type_UNKNOWN;
      break;
    case TYPE_PRIMITIVE:
      if (t == dtUnknown)
        t->asymbol->sym->type_kind = Type_UNKNOWN;
      break;
    case TYPE_FN:
      t->asymbol->sym->type_kind = Type_FUN;
      break;
    case TYPE_ENUM: {
      EnumType *tt = dynamic_cast<EnumType*>(t);
      t->asymbol->sym->type_kind = Type_TAGGED;
      t->asymbol->sym->inherits_add(sym_enum_element);
      Vec<DefExpr *> elements;
      tt->constants->getElements(elements);
      long i = 0;
      forv_Vec(DefExpr, def, elements) {
        Sym *ss = def->sym->asymbol->sym;
        if (def->init && !get_int(def->init, &i)) {
          USR_FATAL(def->init, 
                    "Enum symbols can only be initialized to integer literals currently.");
        }
        build_enum_element(t->asymbol->sym, ss, i);
        t->asymbol->sym->has.add(ss);
        i++;
      }
      break;
    }
    case TYPE_LITERAL: {
      LiteralType *tt = dynamic_cast<LiteralType*>(t);
      t->asymbol->sym->type_kind = Type_ALIAS;
      t->asymbol->sym->alias = tt->literal->asymbol->sym;
      break;
    }
    case TYPE_USER: {
      UserType *tt = dynamic_cast<UserType*>(t);
      t->asymbol->sym->type_kind = Type_ALIAS;
      t->asymbol->sym->alias = tt->underlyingType->asymbol->sym;
      break;
    }
    case TYPE_CLASS: {
      ClassType *tt = dynamic_cast<ClassType*>(t);
      t->asymbol->sym->type_kind = Type_RECORD;
      if (tt->classTag == CLASS_RECORD ||
          tt->classTag == CLASS_UNION ||
          tt->classTag == CLASS_VALUECLASS)
        t->asymbol->sym->is_value_class = 1;
      if (tt->classTag == CLASS_UNION)
        t->asymbol->sym->is_union_class = 1;
      if (tt->dispatchParents.n > 0) {
        forv_Vec(Type, ttt, tt->dispatchParents)
          t->asymbol->sym->inherits_add(ttt->asymbol->sym);
      }
      if (t->asymbol->sym == sym_sequence)
        t->asymbol->sym->element = new_sym();
      break;
    }
    case TYPE_META: {
      MetaType *tt = dynamic_cast<MetaType*>(t);
      if (tt->base->symbol)
        tt->asymbol->sym = tt->base->symbol->asymbol->sym;
      break;
    }
    case TYPE_VARIABLE: {
      VariableType *tt = dynamic_cast<VariableType*>(t);
      tt->asymbol->sym->type_kind = Type_VARIABLE;
      tt->asymbol->sym->meta_type->type_kind = Type_NONE;
      tt->asymbol->sym->meta_type->type = tt->asymbol->sym;
      break;
    }
  }
  // Note: Rewrite using new typeParents or dispatchParents vectors
  //  if (t->parentType)
  //    t->asymbol->sym->must_implement_and_specialize(t->parentType->asymbol->sym);
  return t->asymbol->sym;
}

static void
build_types(Vec<BaseAST *> &syms, Vec<Type *> *atypes) {
  Vec<Type *> ttypes;
  Vec<Type *> *types  = atypes ? atypes : &ttypes;
  forv_BaseAST(s, syms) {
    Type *t = dynamic_cast<Type *>(s);
    if (t) 
      types->add(t);
  }
  forv_Type(t, *types)
    build_type(t, false);
}

static void
new_primitive_type(Sym *&sym, char *name) {
  name = if1_cannonicalize_string(if1, name);
  if (!sym)
    sym = new_sym(name, 1);
  else
    sym->name = name;
  sym->type_kind = Type_PRIMITIVE;
  if1_set_builtin(if1, sym, name);
}

static void
new_alias_type(Sym *&sym, char *name, Sym *alias) {
  if (!sym)
    sym = new_sym(name, 1);
  sym->type_kind = Type_ALIAS;
  sym->alias = alias;
  if1_set_builtin(if1, sym, name);
}

static void
new_lub_type(Sym *&sym, char *name, ...)  {
  if (!sym)
    sym = new_sym(name, 1);
  sym->type_kind = Type_LUB;
  if1_set_builtin(if1, sym, name);
  va_list ap;
  va_start(ap, name);
  Sym *s = 0;
  do {
    if ((s = va_arg(ap, Sym*)))
      sym->has.add(s);
  } while (s);
  forv_Sym(ss, sym->has)
    ss->inherits_add(sym);
}

static void
new_global_variable(Sym *&sym, char *name) {
  if (!sym)
    sym = new_sym(name, 1);
  sym->global_scope = 1;
  if1_set_builtin(if1, sym, name);
}

static void
builtin_Symbol(Type *dt, Sym **sym, char *name) {
  if (!dt->asymbol)
    map_type(dt);
  *sym = dt->asymbol->sym;
  if1_set_builtin(if1, *sym, name);
  if (!dt->asymbol->sym->type_kind)
    dt->asymbol->sym->type_kind = Type_PRIMITIVE;
  SYMBOL(*sym) = dt;
}

static void
build_builtin_symbols() {
  if (!sym_system) {
    sym_system = new_sym("system", 1);
    if1_set_builtin(if1, sym_system, "system");
  }
  if (!sym_system->init)
    sym_system->init = new_sym("__init", 1);
  build_module(sym_system, sym_system->init);

  sym_void = dtVoid->asymbol->sym;
  sym_null = dtNil->asymbol->sym;
  sym_unknown = dtUnknown->asymbol->sym;
  sym_bool = dtBoolean->asymbol->sym;
  sym_int64 = dtInteger->asymbol->sym;
  sym_float64 = dtFloat->asymbol->sym;
  sym_complex64 = dtComplex->asymbol->sym;
  sym_string = dtString->asymbol->sym;
  sym_anynum = dtNumeric->asymbol->sym;
  sym_any = dtAny->asymbol->sym; 
  sym_object = dtObject->asymbol->sym; 

  new_lub_type(sym_anyclass, "anyclass", VARARG_END);
  sym_anyclass->meta_type = sym_anyclass;
  new_lub_type(sym_any, "any", VARARG_END);
  new_primitive_type(sym_null, "null");
  new_primitive_type(sym_module, "module");
  new_primitive_type(sym_symbol, "symbol");
  if1_set_symbols_type(if1);
  new_primitive_type(sym_function, "function");
  new_primitive_type(sym_continuation, "continuation");
  new_primitive_type(sym_vector, "vector");
  new_primitive_type(sym_void, "void");
  new_primitive_type(sym_unknown, "unknown");
  if (!sym_object)
    sym_object = new_sym("object", 1);
  sym_object->type_kind = Type_RECORD;
  if1_set_builtin(if1, sym_object, "object");
  new_primitive_type(sym_list, "list");
  new_primitive_type(sym_ref, "ref");
  new_primitive_type(sym_value, "value");
  new_primitive_type(sym_set, "set");
  new_primitive_type(sym_int8, "int8");
  new_primitive_type(sym_int16, "int16");
  new_primitive_type(sym_int32, "int32");
  new_primitive_type(sym_int64, "int64");
  new_alias_type(sym_int, "int", sym_int64);
  new_primitive_type(sym_true, "true");
  new_primitive_type(sym_false, "false");
  new_primitive_type(sym_bool, "bool");
  sym_true->inherits_add(sym_bool);
  sym_false->inherits_add(sym_bool);
  new_primitive_type(sym_uint8, "uint8");
  new_primitive_type(sym_uint16, "uint16");
  new_primitive_type(sym_uint32, "uint32");
  new_primitive_type(sym_uint64, "uint64");
  new_alias_type(sym_uint, "uint", sym_uint64);
  new_lub_type(sym_anyint, "anyint", 
               sym_int8, sym_int16, sym_int32, sym_int64, sym_bool,
               sym_uint8, sym_uint16, sym_uint32, sym_uint64, VARARG_END);
  new_alias_type(sym_size, "size", sym_int64);
  new_alias_type(sym_enum_element, "enum_element", sym_int64);
  new_primitive_type(sym_float32, "float32");
  new_primitive_type(sym_float64, "float64");
  new_primitive_type(sym_float128, "float128");
  new_alias_type(sym_float, "float", sym_float64);
  new_lub_type(sym_anyfloat, "anyfloat", 
               sym_float32, sym_float64, sym_float128, VARARG_END);
  new_primitive_type(sym_complex32, "complex32");
  new_primitive_type(sym_complex64, "complex64");
  new_primitive_type(sym_complex128, "complex128");
  new_primitive_type(sym_complex, "complex");
  new_lub_type(sym_anycomplex, "anycomplex", 
               sym_complex32, sym_complex64, sym_complex128, VARARG_END);
  new_lub_type(sym_anynum, "anynum", sym_anyint, sym_anyfloat, sym_anycomplex, VARARG_END);
  new_primitive_type(sym_char, "char");
  new_primitive_type(sym_string, "string");
  if (!sym_new_object) {
    sym_new_object = new_sym("new_object", 1);
    if1_set_builtin(if1, sym_new_object, "new_object");
  }

  sym_nil = gNil->asymbol->sym;
  new_global_variable(sym_nil, "nil");
  sym_nil->type = sym_null;
  sym_nil->is_external = 1;

  sym_init = new_sym(); // placeholder

  builtin_Symbol(dtSequence, &sym_sequence, "sequence");
  builtin_Symbol(dtTuple, &sym_tuple, "tuple");
  builtin_Symbol(dtIndex, &sym_index, "index");
  builtin_Symbol(dtDomain, &sym_domain, "domain");
  builtin_Symbol(dtArray, &sym_array, "array");
  builtin_Symbol(dtLocale, &sym_locale, "locale");

  // automatic promotions

  sym_bool->specializes.add(sym_int8);
  sym_int8->specializes.add(sym_int16);
  sym_int16->specializes.add(sym_int32);
  sym_int32->specializes.add(sym_int64);

  sym_int32->specializes.add(sym_float32);
  sym_int64->specializes.add(sym_float64);

  sym_float32->specializes.add(sym_complex32);
  sym_float64->specializes.add(sym_complex64);
  sym_float128->specializes.add(sym_complex128);

  sym_complex32->specializes.add(sym_complex64);
  sym_complex64->specializes.add(sym_complex128);

  sym_anynum->specializes.add(sym_string);

#define S(_n) assert(sym_##_n);
#include "builtin_symbols.h"
#undef S
}

static Stmt *
label_target(Stmt *stmt) {
  Stmt *target = stmt;
  while (target && target->astType == STMT_LABEL)
    target = dynamic_cast<LabelStmt*>(target)->stmt;
  if (!target)
    target = stmt;
  return target;
}

static int
define_labels(BaseAST *ast, LabelMap *labelmap) {
  Stmt *stmt = dynamic_cast<Stmt *>(ast);
  switch (stmt->astType) {
    case STMT_LABEL: {
      Stmt *target = label_target(stmt);
      switch (target->astType) {
        default:
          target->ainfo->label[0] = if1_alloc_label(if1);
          target->ainfo->label[1] = target->ainfo->label[0];
          break;
        case STMT_WHILELOOP:
        case STMT_FORLOOP:
          // handled below
          break;
      }
      labelmap->put(if1_cannonicalize_string(if1, dynamic_cast<LabelStmt*>(stmt)->defLabel->sym->name), target);
      break;
    }
    case STMT_WHILELOOP:
    case STMT_FORLOOP:
      stmt->ainfo->label[0] = if1_alloc_label(if1);
      stmt->ainfo->label[1] = if1_alloc_label(if1);
      break;
    default: break;
  }
  GET_AST_CHILDREN(ast, getStuff);
  forv_BaseAST(a, getStuff.asts)
    if (isSomeStmt(a->astType))
      define_labels(a, labelmap);
  return 0;
}

static int
resolve_labels(BaseAST *ast, LabelMap *labelmap,
               Label *return_label, Label *break_label = 0, Label *continue_label = 0)
{
  Stmt *stmt = dynamic_cast<Stmt *>(ast);
  switch (stmt->astType) {
    case STMT_WHILELOOP:
    case STMT_FORLOOP:
      continue_label = stmt->ainfo->label[0];
      break_label = stmt->ainfo->label[1];
      break;
    case STMT_RETURN:
      stmt->ainfo->label[0] = return_label;
      break;
    case STMT_GOTO: {
      GotoStmt *s = dynamic_cast<GotoStmt*>(ast);
      Stmt *target = labelmap->get(if1_cannonicalize_string(if1, s->label->name));
      if (!target)
        return show_error("unresolved label %s", s->ainfo, s->label->name);
      else 
        stmt->ainfo->label[0] = target->ainfo->label[0];
      break;
    }
    default: break;
  }
  GET_AST_CHILDREN(ast, getStuff);
  forv_BaseAST(a, getStuff.asts)
    if (isSomeStmt(a->astType))
      if (resolve_labels(a, labelmap, return_label, break_label, continue_label) < 0)
        return -1;
  return 0;
}

static Sym *
make_symbol(char *name) {
  Sym *s = if1_make_symbol(if1, name);
  s->global_scope = 1;
  return s;
}

static Sym *
constructor_name(Type *t) {
  return t->defaultConstructor->asymbol->sym;
}

static int
gen_one_defexpr(VarSymbol *var, DefExpr *def) {
  Type *type = var->type;
#ifdef MAKE_USER_TYPE_BE_DEFINITION
  while (type->astType == TYPE_USER)
    type = ((UserType*)type)->underlyingType;
#endif
  Sym *s = var->asymbol->sym;
  AAST *ast = def->ainfo;
  ast->sym = s;
  s->ast = ast;
  s->is_var = 1;
  switch (var->varClass) {
    case VAR_NORMAL: break;
    case VAR_REF: break;
    case VAR_CONFIG: s->is_external = 1; break;
    default: return show_error("unhandled variable class", ast);
  }
  switch (var->consClass) {
    case VAR_CONST: s->is_read_only = 1; break;
    case VAR_VAR: break;
    case VAR_PARAM: break;
    default: assert(!"unknown constant class");
  }
  if (type != dtUnknown) {
    if (!is_reference_type(type))
      s->type = unalias_type(type->asymbol->sym);
    else
      s->must_implement = unalias_type(type->asymbol->sym);
  }
  Expr *init = def->init;
  switch (type->astType) { 
    case TYPE_VARIABLE:
    case TYPE_META:
      type = dtUnknown;  // as yet unknown
      break;
    default:
      break;
  }
  int no_default_init = var->noDefaultInit ;
 // optimizations || (init && (is_reference_type(type) || (is_scalar_type(type) && type == init->typeInfo())));
  Sym *lhs = s;
  if (!no_default_init) {
    lhs = new_sym();
    lhs->ast = ast;
    if (def->exprType)
      if1_gen(if1, &ast->code, def->exprType->ainfo->code);
    Code *c = if1_send(if1, &ast->code, 3, 1, sym_primitive,
                       chapel_defexpr_symbol, type->asymbol->sym, lhs);
    c->ast = ast;
    c->partial = Partial_NEVER;
    if (def->exprType)
      if1_add_send_arg(if1, c, def->exprType->ainfo->rval);
    if (SymExpr *e = dynamic_cast<SymExpr*>(init)) {
      if (e->var == gNil)
        init = NULL;
    }
    if (!init)
      if1_move(if1, &ast->code, lhs, s, ast);
  }
  if (init) {
    if1_gen(if1, &ast->code, init->ainfo->code);
    Sym *val = init->ainfo->rval;
    if (no_default_init)
      if1_move(if1, &ast->code, val, ast->sym, ast);
    else {
      Sym *old_val = val;
      val = new_sym();
      val->ast = ast;
      if (f_equal_method) {
        Code *c = if1_send(if1, &ast->code, 4, 1, make_symbol("="), method_token, 
                           lhs, old_val, val);
        c->ast = ast;
        c->partial = Partial_NEVER;
      } else {
        Code *c = if1_send(if1, &ast->code, 3, 1, make_symbol("="), lhs, old_val, val);
        c->ast = ast;
        c->partial = Partial_NEVER;
      }
      if1_move(if1, &ast->code, val, ast->sym, ast);
    }
  }
  return 0;
}

static int
gen_defexpr(DefExpr* defExpr) {
  if (VarSymbol* var = dynamic_cast<VarSymbol*>(defExpr->sym))
    if (gen_one_defexpr(var, defExpr))
      return -1;
  if1_gen(if1, &defExpr->parentStmt->ainfo->code, defExpr->ainfo->code);
  return 0;
}

static int gen_expr_stmt(BaseAST *a) {
  ExprStmt *expr = dynamic_cast<ExprStmt*>(a);
  expr->ainfo->code = expr->expr->ainfo->code;
  return 0;
}

static int
gen_while(BaseAST *a) {
  WhileLoopStmt *s = dynamic_cast<WhileLoopStmt*>(a);
  Code *body_code = 0;
  if1_gen(if1, &body_code, s->block->ainfo->code);
  if1_loop(if1, &s->ainfo->code, s->ainfo->label[0], s->ainfo->label[1],
           s->condition->ainfo->rval, 0, 
           s->condition->ainfo->code, 0, 
           body_code, s->ainfo);
  return 0;
}

static int
gen_forall_internal(AAST *ainfo, Code *body, Vec<Symbol*> &indices, Vec<Expr*> &iterators) {
  if (indices.n != 1)
    fail("Forall without single indice");
  if (iterators.n != 1)
    fail("Forall without single iterator");

  // setup code: evaluate iterators and get loop variable
  Code *setup_code = 0, *send;
  Sym *indice = indices.v[0]->asymbol->sym;
  if1_gen(if1, &setup_code, iterators.v[0]->ainfo->code);
  Sym *iter = iterators.v[0]->ainfo->rval;
  Sym *loop_var = new_sym();
  loop_var->ast = ainfo;
  Sym *ltmp = new_sym();
  ltmp->ast = ainfo;
  send = if1_send(if1, &setup_code, 2, 1, make_symbol("_forall_start"), 
                  iter, ltmp);
  send->ast = ainfo;
  send->partial = Partial_NEVER;
  if1_move(if1, &setup_code, ltmp, loop_var, ainfo);

  // check for loop termination
  Code *condition_code = 0;
  Sym *condition_rval = new_sym();
  send = if1_send(if1, &condition_code, 3, 1, make_symbol("_forall_valid"), 
                  iter, loop_var, condition_rval);
  send->ast = ainfo;
  send->partial = Partial_NEVER;
  // get index
  Sym *itmp = new_sym();
  itmp->ast = ainfo;
  send = if1_send(if1, &condition_code, 3, 1, make_symbol("_forall_index"), 
                  iter, loop_var, itmp);
  send->ast = ainfo;
  send->partial = Partial_NEVER;
  if1_move(if1, &condition_code, itmp, indice, ainfo);

  // next index code
  Sym *ntmp = new_sym();
  ntmp->ast = ainfo;
  send = if1_send(if1, &body, 3, 1, make_symbol("_forall_next"), 
                  iter, loop_var, ntmp);
  send->ast = ainfo;
  send->partial = Partial_NEVER;
  if1_move(if1, &body, ntmp, loop_var, ainfo);

  if (!ainfo->label[0])
    ainfo->label[0] = if1_alloc_label(if1);
  if (!ainfo->label[1])
    ainfo->label[1] = if1_alloc_label(if1);
  // build loop
  if1_loop(if1, &ainfo->code, ainfo->label[0], ainfo->label[1],
           condition_rval, setup_code, 
           condition_code, 0, 
           body, ainfo);
  return 0;
}

static int
gen_for(BaseAST *a) {
  ForLoopStmt *s = dynamic_cast<ForLoopStmt*>(a);
  Code *body = 0;
  if1_gen(if1, &body, s->innerStmt->ainfo->code);
  Vec<Symbol*> indices;
  Vec<DefExpr*> indexDefs;
  s->indices->getElements(indexDefs);
  forv_Vec(DefExpr, indexDef, indexDefs)
    indices.add(indexDef->sym);
  Vec<Expr*> iterators;
  iterators.add(s->iterators->only());
  return gen_forall_internal(s->ainfo, body, indices, iterators);
}

static int
gen_cond(AAST *ast, AAST *xcond, AAST *xthen, AAST *xelse) {
  if1_if(if1, &ast->code, xcond->code, xcond->rval, 
         xthen->code, xthen->rval, xelse ? xelse->code : 0, 
         xelse ? xelse->rval : 0, ast->rval, ast);
  return 0;
}

static astType_t
undef_or_fn_expr(Expr *ast) {
  if (ast->astType == EXPR_SYM) { 
    SymExpr* v = dynamic_cast<SymExpr* >(ast);
    return v->var->astType;
  }
  return (astType_t)0;
}

static int
is_this_member_access(BaseAST *a) {
  MemberAccess *ma = dynamic_cast<MemberAccess*>(a);
  if (!ma)
    return 0;
  SymExpr* v = dynamic_cast<SymExpr*>(ma->base);
  if (!v)
    return 0;
  if (v->var->isThis())
    return 1;
  return 0;
}

static Sym *
gen_assign_rhs(CallExpr *s) {
  s->ainfo->rval = new_sym();
  s->ainfo->rval->ast = s->ainfo;
  s->ainfo->sym = s->get(1)->ainfo->sym;
  if1_gen(if1, &s->ainfo->code, s->get(2)->ainfo->code);
  Sym *rval = new_sym();
  rval->ast = s->ainfo;
  if1_move(if1, &s->ainfo->code, s->get(2)->ainfo->rval, rval, s->ainfo);
  return rval;
}

static int
gen_set_member(MemberAccess *ma, CallExpr *base_ast) {
  FnSymbol *fn = ma->getStmt()->parentFunction();
  AAST *ast = base_ast->ainfo;
  int equal = !fn || (!fn->_setter && (fn->fnClass != FN_CONSTRUCTOR || !is_this_member_access(ma)));
  assert(!equal);
  ast->rval = new_sym();
  ast->rval->ast = base_ast->ainfo;
  Sym *rhs = gen_assign_rhs(base_ast);
  Sym *selector = make_symbol(ma->member->asymbol->sym->name);
  Code *c = if1_send(if1, &ast->code, 5, 1, sym_operator, ma->base->ainfo->rval, 
                     make_symbol(".="), selector, rhs, ast->rval);
  c->ast = ast;
  c->partial = Partial_NEVER;
  return 0;
}

static Sym *
make_temp(Sym *s, AAST *ast) {
  if (1 || s->function_scope)
    return s;
  Sym *tmp = new_sym();
  tmp->ast = ast;
  if1_move(if1, &ast->code, s, tmp, ast);
  return tmp;
}

static int
gen_paren_op(CallExpr *s) {
  AAST *ast = s->ainfo;
  ast->rval = new_sym();
  ast->rval->ast = ast;
  if1_gen(if1, &ast->code, s->baseExpr->ainfo->code);
  Vec<Expr *> args;
  s->argList->getElements(args);
  if (args.n == 1 && !args.v[0])
    args.n--;
  Vec<Sym *> rvals;
  forv_Vec(Expr, a, args) {
    if1_gen(if1, &ast->code, a->ainfo->code);
    assert(a->ainfo->rval);
    rvals.add(a->ainfo->rval);
  }
  astType_t base_symbol = undef_or_fn_expr(s->baseExpr);
  Sym *base = NULL;
  char *n = s->baseExpr->ainfo->rval->name, *str;
  if (n && !strcmp(n, "__primitive")) {
    if (args.n > 0 && get_string(args.v[0], &str) &&
        if1->primitives->prim_map[0][0].get(if1_cannonicalize_string(if1, str)))
    {
      rvals.v[0] = if1_get_builtin(if1, str);
      base = 0;
    } else if (args.n == 3 && get_string(args.v[1], &str) &&
               if1->primitives->prim_map[1][1].get(if1_cannonicalize_string(if1, str))) {
      rvals.v[1] = make_symbol(str);
      base = sym_operator;
    } else
      base = sym_primitive;
  } else if (base_symbol == SYMBOL_UNRESOLVED) {
    assert(n);
    base = make_symbol(n);
  } else if (base_symbol == SYMBOL_FN)
    base = dynamic_cast<FnSymbol*>(dynamic_cast<SymExpr*>(s->baseExpr)->var)->asymbol->sym;
  else
    base = s->baseExpr->ainfo->rval;
  int need_temps = s->partialTag != PARTIAL_NEVER;
  Vec<Sym*> trvals;
  if (base)
    trvals.add(need_temps ? make_temp(base, ast) : base);
  forv_Sym(r, rvals)
    trvals.add(need_temps ? make_temp(r, ast) : r);
  Code *send = if1_send1(if1, &ast->code);
  send->ast = ast;
  forv_Sym(r, trvals)
    if1_add_send_arg(if1, send, r);
  if1_add_send_result(if1, send, ast->rval);
  send->partial = s->partialTag == PARTIAL_OK ? Partial_OK :
    (s->partialTag == PARTIAL_NEVER ? Partial_NEVER : Partial_ALWAYS);
  ast->sym = ast->rval;
  return 0;
}

static int
gen_when(WhenStmt *s, SelectStmt *ss, Label *l) {
  Vec<Expr*> cases;
  s->caseExprs->getElements(cases);
  Sym *cond = NULL;
  forv_Expr(x, cases) {
    if (gen_if1(x, s) < 0) return -1;
    if1_gen(if1, &s->ainfo->code, x->ainfo->code);
    Sym *tmp = new_sym();
    Code *c = if1_send(if1, &s->ainfo->code, 3, 1, make_symbol("=="),
                       ss->caseExpr->ainfo->rval, x->ainfo->rval, tmp);
    c->ast = s->ainfo;
    c->partial = Partial_NEVER;
    if (!cond)
      cond = tmp;
    else {
      Sym *new_cond = new_sym();
      c = if1_send(if1, &s->ainfo->code, 4, 1, sym_operator, cond, make_symbol("&&"), tmp, new_cond);
      c->ast = s->ainfo;
      c->partial = Partial_NEVER;
      cond = new_cond;
    }
  }
  if (gen_if1(s->doStmt, s) < 0) return -1;
  if (cond) {
    Code *ifgoto = if1_if_goto(if1, &s->ainfo->code, cond, s->ainfo);
    if1_if_label_true(if1, ifgoto, if1_label(if1, &s->ainfo->code, s->ainfo));
    if1_gen(if1, &s->ainfo->code, s->doStmt->ainfo->code);
    if1_goto(if1, &s->ainfo->code, l);
    if1_if_label_false(if1, ifgoto, if1_label(if1, &s->ainfo->code, s->ainfo));
  } else {
    if1_gen(if1, &s->ainfo->code, s->doStmt->ainfo->code);
    if1_goto(if1, &s->ainfo->code, l);
  }
  return 0;
}

static int
gen_select(BaseAST *a) {
  SelectStmt *s = dynamic_cast<SelectStmt*>(a);
  Vec<WhenStmt*> whens;
  s->whenStmts->getElements(whens);
  gen_if1(s->caseExpr, s);
  if1_gen(if1, &s->ainfo->code, s->caseExpr->ainfo->code);
  Label *l = if1_alloc_label(if1);
  forv_Vec(WhenStmt, x, whens) {
    if (gen_when(x, s, l) < 0) return -1;
    if1_gen(if1, &s->ainfo->code, x->ainfo->code);
  }
  if1_label(if1, &s->ainfo->code, s->ainfo, l);
  return 0;
}

static int
gen_assignment(CallExpr *assign) {
  // handle assignment
  if1_gen(if1, &assign->ainfo->code, assign->get(1)->ainfo->code);
  Sym *rval = gen_assign_rhs(assign);
  SymExpr* lhs_var = dynamic_cast<SymExpr*>(assign->get(1));
  Symbol *lhs_symbol = lhs_var ? dynamic_cast<Symbol *>(lhs_var->var) : 0;
  FnSymbol *f = assign->parentFunction();
  int constructor_assignment = 0;
  if (f->fnClass == FN_CONSTRUCTOR) {
    if (MemberAccess *m = dynamic_cast<MemberAccess*>(assign->get(1))) {
      if (SymExpr* v = dynamic_cast<SymExpr*>(m->base)) {
        if (v->var->isThis())
          constructor_assignment = 1;
      }
    }
  }
  VarSymbol *lhs_var_symbol = lhs_var ? dynamic_cast<VarSymbol*>(lhs_var->var) : 0;
  // reasons to not overload an assignment
  //  - in constructor, setter or getter
  //  - symbol is noDefaultInit
  //  - symbol has no declared type and no initializer (i.e. var x; x = ....)
  //  - symbol is "this"
  int operator_equal = 
    !(constructor_assignment || 
      (lhs_var_symbol && lhs_var_symbol->noDefaultInit) ||
      (lhs_symbol && (lhs_symbol->type == dtUnknown && !lhs_symbol->defPoint->init)) ||
      (lhs_symbol && lhs_symbol->isThis()))
    ;
  // always overload assignment if the variable was initalized
  operator_equal = operator_equal || (lhs_symbol && lhs_symbol->defPoint->init);
  if (operator_equal) {
    Sym *old_rval = rval;
    rval = new_sym();
    rval->ast = assign->ainfo;
    Sym *told_rval = new_sym();
    told_rval->ast = assign->ainfo;
    if1_move(if1, &assign->ainfo->code, old_rval, told_rval, assign->ainfo);
    if (f_equal_method) {
      Code *c = if1_send(if1, &assign->ainfo->code, 4, 1, make_symbol("="), method_token,
                         assign->get(1)->ainfo->rval, told_rval, rval);
      c->ast = assign->ainfo;
      c->partial = Partial_NEVER;
    } else {
      Code *c = if1_send(if1, &assign->ainfo->code, 3, 1, make_symbol("="), 
                         assign->get(1)->ainfo->rval, told_rval, rval);
      c->ast = assign->ainfo;
      c->partial = Partial_NEVER;
    }
  }
  if (!assign->get(1)->ainfo->sym)
    show_error("assignment to non-lvalue", assign->ainfo);
  if (MemberAccess *ma = dynamic_cast<MemberAccess*>(assign->get(1))) {
    Sym *selector = make_symbol(ma->member->asymbol->sym->name);
    Code *c = if1_send(if1, &assign->ainfo->code, 5, 1, sym_operator, ma->base->ainfo->rval, 
                       make_symbol(".="), selector, rval, assign->ainfo->rval);
    c->ast = assign->ainfo;
    c->partial = Partial_NEVER;
  } else {
    if1_move(if1, &assign->ainfo->code, rval, assign->ainfo->rval, assign->ainfo);
    if1_move(if1, &assign->ainfo->code, assign->ainfo->rval, assign->get(1)->ainfo->sym, assign->ainfo);
  }
  return 0;
}

static int
gen_if1(BaseAST *ast, BaseAST *parent) {
  // special cases
  switch (ast->astType) {
    default: break;
    case STMT_WHEN: return 0;
    case STMT_SELECT: gen_select(ast); return 0;
  }
  // recurse
  GET_AST_CHILDREN(ast, getStuff);
  DefExpr* def_expr = dynamic_cast<DefExpr*>(ast);
  if (!def_expr || !dynamic_cast<FnSymbol*>(def_expr->sym))
    forv_BaseAST(a, getStuff.asts)
      if (gen_if1(a, ast) < 0)
        return -1;
  // bottom's up
  switch (ast->astType) {
    case STMT: assert(!ast); break;
    case STMT_LABEL: {
      LabelStmt *s = dynamic_cast<LabelStmt*>(ast);
      Stmt *target = label_target(s);
      if1_label(if1, &s->ainfo->code, s->stmt->ainfo, target->ainfo->label[0]);
      if1_gen(if1, &s->ainfo->code, s->stmt->ainfo->code);
      break;
    }
    case STMT_GOTO: {
      Stmt *s = dynamic_cast<Stmt*>(ast);
      Code *c = if1_goto(if1, &s->ainfo->code, s->ainfo->label[0]);
      c->ast = s->ainfo;
      break;
    }
    case STMT_EXPR:
      if (ExprStmt* exprStmt = dynamic_cast<ExprStmt*>(ast)) {
        if (DefExpr* defExpr = dynamic_cast<DefExpr*>(exprStmt->expr)) {
          if (dynamic_cast<VarSymbol*>(defExpr->sym) && 
              gen_defexpr(defExpr) < 0)
            return -1;
        } else {
          if (gen_expr_stmt(ast) < 0)
            return -1;
        }
      }
      break;
    case STMT_RETURN: {
      ReturnStmt *s = dynamic_cast<ReturnStmt*>(ast);
      Sym *fn = s->parentFunction()->asymbol->sym;
      if (s->expr) {
        fn->fun_returns_value = 1;
        if1_gen(if1, &s->ainfo->code, s->expr->ainfo->code);
        if (s->expr->ainfo->rval == NULL) s->expr->print(stdout);
        if1_move(if1, &s->ainfo->code, s->expr->ainfo->rval, fn->ret, s->ainfo);
      } else 
        if1_move(if1, &s->ainfo->code, sym_void, fn->ret, s->ainfo);
      Code *c = if1_goto(if1, &s->ainfo->code, s->ainfo->label[0]);
      c->ast = s->ainfo;
      break;
    }
    case STMT_BLOCK: {
      BlockStmt *s = dynamic_cast<BlockStmt*>(ast);
      Vec<Stmt *> stmts;
      s->body->getElements(stmts);
      forv_Stmt(ss, stmts)
        if1_gen(if1, &s->ainfo->code, ss->ainfo->code);
      break;
    }
    case STMT_WHILELOOP: gen_while(ast); break;
    case STMT_FORLOOP: gen_for(ast); break;
    case STMT_COND: {
      CondStmt *s = dynamic_cast<CondStmt*>(ast);
      gen_cond(s->ainfo, s->condExpr->ainfo, s->thenStmt->ainfo, 
               s->elseStmt ? s->elseStmt->ainfo : 0); 
      break;
    }
    case STMT_WHEN: assert(!"case"); break;
    case STMT_SELECT: assert(!"case"); break;
    case EXPR: {
      Expr *s = dynamic_cast<Expr*>(ast);
      assert(!ast); 
      s->ainfo->rval = sym_nil;
      break;
    }
    case EXPR_SYM: {
      SymExpr* s = dynamic_cast<SymExpr*>(ast);
      Sym *sym = s->var->asymbol->sym;
      switch (SYMBOL(sym)->astType) {
        default: break;
        case SYMBOL_TYPE: 
          if (parent && parent->astType == EXPR_MEMBERACCESS)
            sym = ((TypeSymbol*)SYMBOL(sym))->definition->asymbol->sym->meta_type;
          else
            sym = ((TypeSymbol*)SYMBOL(sym))->definition->asymbol->sym;
          break;
      }
      s->ainfo->sym = sym;
      s->ainfo->rval = sym;
      break;
    }
    case EXPR_DEF: break;
    case EXPR_INIT: break;
    case EXPR_MEMBERACCESS: {
      MemberAccess *s = dynamic_cast<MemberAccess*>(ast);
      s->ainfo->rval = new_sym();
      s->ainfo->rval->ast = s->ainfo;
      s->ainfo->sym = s->ainfo->rval;
      if1_gen(if1, &s->ainfo->code, s->base->ainfo->code);
      Sym *op = make_symbol(".");
      Sym *selector = make_symbol(s->member->asymbol->sym->name);
      Code *c = if1_send(if1, &s->ainfo->code, 4, 1, sym_operator,
                         s->base->ainfo->rval, op, selector,
                         s->ainfo->rval);
      c->ast = s->ainfo;
      c->partial = Partial_NEVER;
      break;
    }
    case EXPR_LET: {
      LetExpr *s = dynamic_cast<LetExpr *>(ast);
      DefExpr* def_expr = s->symDefs->first();
      while (def_expr) {
        VarSymbol *vs = dynamic_cast<VarSymbol*>(def_expr->sym);
        if1_gen(if1, &s->ainfo->code, def_expr->init->ainfo->code);
        if1_move(if1, &s->ainfo->code, def_expr->init->ainfo->rval, vs->asymbol->sym, s->ainfo);
        def_expr = s->symDefs->next();
      }
      if1_gen(if1, &s->ainfo->code, s->innerExpr->ainfo->code);
      s->ainfo->rval = s->innerExpr->ainfo->rval;
      break;
    }
    case EXPR_COND: {
      CondExpr *s = dynamic_cast<CondExpr *>(ast);
      s->ainfo->rval = new_sym();
      s->ainfo->rval->ast = s->ainfo;
      gen_cond(s->ainfo, s->condExpr->ainfo, s->thenExpr->ainfo, s->elseExpr->ainfo);
      break;
    }
    case EXPR_CALL: {
      CallExpr* call = dynamic_cast<CallExpr*>(ast);
      if (call->opTag == OP_GETSNORM) {
        FnSymbol *f = call->parentFunction();
        int is_member = call->get(1)->astType == EXPR_MEMBERACCESS;
        if (f->fnClass == FN_CONSTRUCTOR && is_member) {
          if (gen_set_member(dynamic_cast<MemberAccess*>(call->get(1)), call) < 0) return -1;
          break;
        }
        if (gen_assignment(call) < 0) return -1;
        break;
      }
      if (gen_paren_op(call) < 0) return -1;
      break;
    }
    case EXPR_CAST: {
      CastExpr *s = dynamic_cast<CastExpr *>(ast);
      s->ainfo->rval = new_sym();
      s->ainfo->rval->ast = s->ainfo;
      if (s->type)
        s->ainfo->rval->aspect = base_type(s->type->asymbol->sym);
      if (s->newType)
        if1_gen(if1, &s->ainfo->code, s->newType->ainfo->code);
      if1_gen(if1, &s->ainfo->code, s->expr->ainfo->code);
      Sym *typeval = s->newType ? s->newType->ainfo->rval : s->type->asymbol->sym;
      Code *send = if1_send(if1, &s->ainfo->code, 4, 1, sym_primitive, cast_symbol, 
                            typeval, s->expr->ainfo->rval, s->ainfo->rval);
      send->ast = s->ainfo;
      send->partial = Partial_NEVER;
      break;
    }
    case EXPR_REDUCE: {
      ReduceExpr *s = dynamic_cast<ReduceExpr *>(ast);
      s->ainfo->rval = new_sym();
      s->ainfo->rval->ast = s->ainfo;
      if1_gen(if1, &s->ainfo->code, s->redDim->only()->ainfo->code);
      if1_gen(if1, &s->ainfo->code, s->argExpr->ainfo->code);
      Code *send = if1_send(if1, &s->ainfo->code, 5, 1, sym_primitive, expr_reduce_symbol, 
                            s->reduceType->asymbol->sym, s->redDim->only()->ainfo->rval, 
                            s->argExpr->ainfo->rval, s->ainfo->rval);
      send->ast = s->ainfo;
      send->partial = Partial_NEVER;
      break;
    }
    case EXPR_NAMED: {
      NamedExpr *s = dynamic_cast<NamedExpr *>(ast);
      s->ainfo->rval = new_sym();
      s->ainfo->rval->ast = s->ainfo;
      if1_move(if1, &s->ainfo->code, s->actual->ainfo->rval, s->ainfo->rval, s->ainfo);
      s->ainfo->rval->arg_name = if1_cannonicalize_string(if1, s->name);
      break;
    }
  case EXPR_IMPORT: break;
  case SYMBOL:
  case SYMBOL_UNRESOLVED:
  case SYMBOL_MODULE:
  case SYMBOL_VAR:
  case SYMBOL_ARG:
  case SYMBOL_TYPE:
  case SYMBOL_FN:
  case SYMBOL_ENUM:
  case SYMBOL_LABEL:
  case TYPE:
  case TYPE_PRIMITIVE:
  case TYPE_FN:
  case TYPE_ENUM:
  case TYPE_USER:
  case TYPE_LITERAL:
  case TYPE_CLASS:
  case TYPE_META:
  case TYPE_SUM:
  case TYPE_VARIABLE:
  case AST_TYPE_END:
  case LIST:
    assert(!"case");
    break;
  }
  return 0;
}

static void
fun_where_equal_constant(FnSymbol *f, SymExpr* v, VarSymbol *c) {
  if (ArgSymbol *s = dynamic_cast<ArgSymbol*>(v->var)) {
    if (!s->isGeneric) {
      show_error("where constraint on non-generic %s", f->body->ainfo, s->name);
      return;
    }
    s->asymbol->sym->must_implement = c->asymbol->sym;
    s->asymbol->sym->must_specialize =  c->asymbol->sym;
  }
}

static void
fun_where_clause(FnSymbol *f, Expr *w) {
  CallExpr *op = dynamic_cast<CallExpr*>(w);
  if (!op)
    return;
  if (op->opTag == OP_LOGAND) {
    fun_where_clause(f, op->get(1));
    fun_where_clause(f, op->get(2));
  } if (op->opTag == OP_EQUAL) {
    VarSymbol *c = 0;
    SymExpr* v = 0;
    if ((c = get_constant(op->get(1))) && (v = dynamic_cast<SymExpr*>(op->get(2)))) 
      fun_where_equal_constant(f, v, c);
    else if ((c = get_constant(op->get(2))) && (v = dynamic_cast<SymExpr*>(op->get(1)))) 
      fun_where_equal_constant(f, v, c);
  }
}

static int
gen_fun(FnSymbol *f) {
  Sym *fn = f->asymbol->sym;
  AAST* ast = f->defPoint->ainfo;
  Vec<ArgSymbol *> args;
  Vec<Sym *> out_args;
  for_alist(DefExpr, formal, f->formals) {
    args.add(dynamic_cast<ArgSymbol*>(formal->sym));
  }
  Sym *as[args.n + 4];
  int iarg = 0;
  assert(f->asymbol->sym->name);
  if (strcmp(f->asymbol->sym->name, "=this") == 0) {
    if (is_Sym_OUT(args.v[0]->asymbol->sym))
      out_args.add(args.v[0]->asymbol->sym);
    as[iarg++] = args.v[0]->asymbol->sym;
  } else if (strcmp(f->asymbol->sym->name, "this") == 0) {
    if (is_Sym_OUT(args.v[0]->asymbol->sym))
      out_args.add(args.v[0]->asymbol->sym);
    as[iarg++] = args.v[0]->asymbol->sym;
  } else {
    int setter = f->asymbol->sym->name[0] == '=' && 
      f->asymbol->sym->name[1] &&
      !OPERATOR_CHAR(f->asymbol->sym->name[1]);
    Sym *s = new_sym(f->asymbol->sym->name + (setter ? 1 : 0));
    s->ast = ast;
    s->must_specialize = make_symbol(s->name);
    as[iarg++] = s;
    if (f->method_type != NON_METHOD) {
      // this
      if (args.n) {
        if (is_Sym_OUT(args.v[0]->asymbol->sym))
          out_args.add(args.v[0]->asymbol->sym);
        as[iarg++] = args.v[0]->asymbol->sym;
      }
    }
    if (f->method_type == NON_METHOD) {
      if (args.n) {
        if (is_Sym_OUT(args.v[0]->asymbol->sym))
          out_args.add(args.v[0]->asymbol->sym);
        as[iarg++] = args.v[0]->asymbol->sym;
      }
    }
  }
  for (int i = 1; i < args.n; i++) {
    if (is_Sym_OUT(args.v[i]->asymbol->sym))
      out_args.add(args.v[i]->asymbol->sym);
    as[iarg++] = args.v[i]->asymbol->sym;
  }
  Code *body = 0;
  if1_gen(if1, &body, f->body->ainfo->code);
  if1_move(if1, &body, sym_void, fn->ret, ast);
  if1_label(if1, &body, ast, ast->label[0]);
  Code *c = if1_send(if1, &body, 3, 0, sym_reply, fn->cont, fn->ret);
  forv_Sym(r, out_args)
    if1_add_send_arg(if1, c, r);
  c->ast = ast;
  c->partial = Partial_NEVER;
  if1_closure(if1, fn, body, iarg, as);
  fn->ast = ast;
  if (f->_this && f->fnClass != FN_CONSTRUCTOR)
    fn->self = f->_this->asymbol->sym;
  fun_where_clause(f, f->whereExpr);
  return 0;
}

static int
init_function(FnSymbol *f) {
  Sym *s = f->asymbol->sym;
  if (verbose_level > 2 && f->name)
    printf("build_functions: %s\n", f->name);
  if (f == chpl_main) {
    if1_set_builtin(if1, s, "init");
    sym_init = s;
  }
  s->cont = new_sym();
  AAST* ast = f->defPoint->ainfo;
  s->cont->ast = ast;
  s->ret = new_sym();
  s->ret->ast = ast;
  s->labelmap = new LabelMap;
  set_global_scope(s);
  return 0;
}

static int
build_function(FnSymbol *f) {
  if (define_labels(f->body, f->asymbol->sym->labelmap) < 0) return -1;
  AAST* ast = f->defPoint->ainfo;
  Label *return_label = ast->label[0] = if1_alloc_label(if1);
  if (resolve_labels(f->body, f->asymbol->sym->labelmap, return_label) < 0) return -1;
  if (gen_if1(f->body) < 0) return -1;
  if (gen_fun(f) < 0) return -1;
  return 0;
}

static void
build_classes(Vec<BaseAST *> &syms) {
  Vec<ClassType *> classes;
  forv_BaseAST(s, syms)
    if (s->astType == TYPE_CLASS)
      classes.add(dynamic_cast<ClassType*>(s)); 
  if (verbose_level > 2)
    printf("build_classes: %d classes\n", classes.n);
  forv_Vec(ClassType, c, classes) {
    Sym *csym = c->asymbol->sym;
    forv_Vec(Symbol, tmp, c->fields)
      csym->has.add(tmp->asymbol->sym);
    forv_Vec(TypeSymbol, tmp, c->types) if (tmp)
      if (tmp->definition->astType == TYPE_USER || 
          (fnewvardef && tmp->definition->astType == TYPE_VARIABLE))
        csym->has.add(tmp->asymbol->sym);
  }
  build_patterns(syms);
}

static int
build_functions(Vec<BaseAST *> &syms) {
  forv_BaseAST(s, syms)
    if (s->astType == SYMBOL_FN)
      if (init_function(dynamic_cast<FnSymbol*>(s)) < 0)
        return -1;
  forv_BaseAST(s, syms)
    if (s->astType == SYMBOL_FN)
      if (build_function(dynamic_cast<FnSymbol*>(s)) < 0)
        return -1;
  return 0;
}

static void
add_to_universal_lookup_cache(char *name, Fun *fun) {
  Vec<Fun *> *v = universal_lookup_cache.get(name);
  if (!v)
    v = new Vec<Fun *>;
  v->add(fun);
  universal_lookup_cache.put(name, v);
}

static int
handle_argument(Sym *s, char *name, Fun *fun, int added, MPosition &p) {
  if (s->is_pattern) {
    p.push(1);
    forv_Sym(ss, s->has) {
      added = handle_argument(ss, name, fun, added, p);
      p.inc();
    }
    p.pop();
  }
  // non-scoped lookup if any parameteter is specialized on a reference type
  // (is dispatched)
  if (!added && s->must_specialize && 
      is_reference_type(SYMBOL(s->must_specialize)))
  {
    add_to_universal_lookup_cache(name, fun);
    added = 1;
  }
  // record default argument positions
  if (SYMBOL(s)) {
    ArgSymbol *symbol = dynamic_cast<ArgSymbol*>(SYMBOL(s));
    if (symbol && symbol->defPoint->init) {
      assert(symbol->defPoint->init->ainfo);
      fun->default_args.put(cannonicalize_mposition(p), symbol->defPoint->init->ainfo);
    }
  }
  return added;
}

static void 
finalize_function(Fun *fun) {
  int added = 0;
  char *name = fun->sym->has.v[0]->name;
  assert(name);
  if (!strcmp("_chpl_alloc", name))
    fun->is_external = 1;
  FnSymbol *fs = dynamic_cast<FnSymbol*>(SYMBOL(fun->sym));
  if (fs->noParens)
    fun->eager_evaluation = 1;
  if (fs->typeBinding) {
    if (is_reference_type(SYMBOL(fs->typeBinding))) {
      if (fs->method_type != NON_METHOD) {
        add_to_universal_lookup_cache(name, fun);
        added = 1;
      }
    }
  }
  MPosition p;
  p.push(1);
  forv_Sym(s, fun->sym->has) {
    added = handle_argument(s, name, fun, added, p);
    p.inc();
  }
  // check nesting
  if (fs->defPoint->parentStmt) {
    if (FnSymbol *fn = dynamic_cast<FnSymbol*>(fs->defPoint->parentSymbol)) {
      if (!fun->nested_in) {
        fun->nested_in = fn->asymbol->sym->fun;
        if (fun->nested_in)
          fun->nested_in->nested.add(fun);
      } else 
        assert(fun->nested_in == fn->asymbol->sym->fun);
    }
  }
  if (fs->isGeneric)
    fun->is_generic = 1;
  // check pragmas
  Sym *fn = fun->sym;
  FnSymbol *f = dynamic_cast<FnSymbol*>(SYMBOL(fn));
  if (f->hasPragma("split unique"))
    fun->split_unique = 1;
}

void
ACallbacks::finalize_functions() {
  pdb->fa->method_token = unique_AVar(new Var(method_token), GLOBAL_CONTOUR);
  pdb->fa->array_index_base = 1;
  pdb->fa->tuple_index_base = 1;
  forv_Fun(fun, pdb->funs)
    finalize_function(fun);
}

// C++ boilerplate clutter
AError::AError(AError_kind akind, AVar *acall, AType *atype, AVar *aavar) :
  kind(akind), call(acall), type(atype), avar(aavar)
{
}

void
AError::get_member_names(Vec<char *> &names) {
  forv_CreationSet(cs, *type)
    names.add(cs->sym->name);
}

AType *
AError::get_type(AVar *aavar) {
  return aavar->out;
}

void
AError::get_types(Vec<Type *> &types) {
  forv_CreationSet(cs, *type)
    types.set_add(to_AST_type(cs->sym->type));
  types.set_to_vec();
  qsort(types.v, types.n, sizeof(types.v[0]), compar_baseast);
}

void
AError::get_callers(AVar *call, Vec<AVar *> &callers) {
  if (call->contour_is_entry_set) {
    EntrySet *es = (EntrySet*)call->contour;
    AEdge **last = es->edges.last();
    for (AEdge **x = es->edges.first(); x < last; x++) if (*x)
      callers.set_add(make_AVar((*x)->pnode->lvals.v[0], (*x)->from));
    callers.set_to_vec();
  }
}

BaseAST *
AError::get_def_BaseAST(AVar *acall) {
  if (acall->var->def && acall->var->def->code && acall->var->def->code->ast)
    return ((AAST*)acall->var->def->code->ast)->xast;
  return NULL;
}

BaseAST *
AError::get_BaseAST(AVar *acall) {
  if (acall->var->sym->asymbol)
    return SYMBOL(acall->var->sym);
  return NULL;
}

int 
analysis_error(AError_kind akind, AVar *acall, AType *atype, AVar *aavar) {
  AError *ae = new AError(akind, acall, atype, aavar);
  analysis_errors.add(ae);
  return -1;
}

void reportAnalysisErrors(Vec<AError*>* analysis_errors) {
  USR_WARNING("Analysis errors detected, scroll down for analysis messages");
  forv_Vec(AError, error, *analysis_errors) {
    USR_FATAL_CONT("High-level error message");
  }
  USR_WARNING("Analysis messages follow");
}

void
ACallbacks::report_analysis_errors(Vec<ATypeViolation*> &type_violations) {
  forv_ATypeViolation(v, type_violations) {
    switch (v->kind) {
      case ATypeViolation_PRIMITIVE_ARGUMENT:
      case ATypeViolation_SEND_ARGUMENT:
        analysis_error(AERROR_CALL_ARGUMENT, v->send, v->type, v->av); break;
      case ATypeViolation_DISPATCH_AMBIGUITY:
        analysis_error(AERROR_CALL_ARGUMENT, v->send, v->type, v->av); break;
      case ATypeViolation_MEMBER:
        analysis_error(AERROR_CALL_ARGUMENT, v->send, v->type, v->av); break;
      case ATypeViolation_MATCH:
        analysis_error(AERROR_CALL_ARGUMENT, v->send, v->type, v->av); break;
      case ATypeViolation_NOTYPE:
        analysis_error(AERROR_CALL_ARGUMENT, v->send, v->type, v->av); break;
      case ATypeViolation_BOXING:
        analysis_error(AERROR_CALL_ARGUMENT, v->send, v->type, v->av); break;
    }
  }
  reportAnalysisErrors(&analysis_errors);
}

static void
init_symbols() {
  expr_simple_seq_symbol = make_symbol("expr_simple_seq");
  expr_domain_symbol = make_symbol("expr_domain");
  expr_create_domain_symbol = make_symbol("expr_create_domain");
  expr_reduce_symbol = make_symbol("expr_reduce");
  cast_symbol = make_symbol("cast");
  make_seq_symbol = make_symbol("make_seq");
  chapel_defexpr_symbol = make_symbol("chapel_defexpr");
  write_symbol = make_symbol("write");
  writeln_symbol = make_symbol("writeln");
  read_symbol = make_symbol("read");
  array_index_symbol = make_symbol("array_index");
  array_set_symbol = make_symbol("array_set");
}

static void
print_ast(BaseAST *a, Vec<BaseAST *> &asts) {
  if (!asts.set_add(a)) {
    printf("(%d *)", (int)a->astType);
    return;
  }
  printf("(%d", (int)a->astType);
  GET_AST_CHILDREN(a, getStuff);
  if (getStuff.asts.n)
    printf(" ");
  forv_BaseAST(b, getStuff.asts)
    print_ast(b, asts);
  printf(")");
}

static void
print_ast(BaseAST *a) {
  Vec<BaseAST *> asts;
  print_ast(a, asts);
  printf("\n");
}

static void
print_baseast(BaseAST *a, Vec<BaseAST *> &asts) {
  if (!asts.set_add(a)) {
    printf("(%s *)", astTypeName[a->astType]);
    return;
  }
  printf("(%s", astTypeName[a->astType]);
  GET_AST_CHILDREN(a, getStuff);
  if (getStuff.asts.n)
    printf(" ");
  forv_BaseAST(b, getStuff.asts)
    print_ast(b, asts);
  printf(")");
}

static void
print_one_baseast(BaseAST *a) {
  Vec<BaseAST *> asts;
  print_baseast(a, asts);
  printf("\n");
}

static void
debug_new_ast(Vec<AList<Stmt> *> &stmts, Vec<BaseAST *> &syms) {
  if (verbose_level > 2) {
    forv_Vec(AList<Stmt>*, list, stmts) {
      Stmt* s = list->first();
      while (s) {
        print_one_baseast(s);
        s = list->next();
      }
    }
    forv_BaseAST(s, syms) {
      DefExpr* def_expr = dynamic_cast<DefExpr*>(s);
      if (def_expr && dynamic_cast<FnSymbol*>(def_expr->sym)) {
        print_ast(dynamic_cast<FnSymbol*>(def_expr->sym)->body);
      } else {
        Type *t = dynamic_cast<Type*>(s); 
        if (t && t->symbol)
          printf("Type: %s cname %s\n", t->symbol->name, t->symbol->cname); 
      }
    }
  }
}

static void
cast_value(PNode *pn, EntrySet *es) {
  assert(pn->rvals.n == 4);
  AVar *result = make_AVar(pn->lvals.v[0], es);
  AVar *type = make_AVar(pn->rvals.v[2], es);
  AVar *val = make_AVar(pn->rvals.v[3], es);
  fill_tvals(es->fun, pn, 2);

  AVar *type_tmp = make_AVar(pn->tvals.v[0], es);
  Vec<CreationSet *> type_css;
  forv_CreationSet(cs, type->out->sorted) {
    Sym *ts = cs->sym;
    if (ts->type->asymbol) {
      if (ts->type->is_meta_type) {
        if (is_scalar_type_symbol(SYMBOL(ts->type))) {
          AType *btype = make_abstract_type(base_type(ts->meta_type));
          CreationSet *bcs = btype->v[0];
          update_in(result, btype);
          type_css.add(bcs);
        }
      } else {
        if (is_scalar_type(SYMBOL(ts->type))) {
          AType *btype = make_abstract_type(base_type(ts));
          CreationSet *bcs = btype->v[0];
          update_in(result, btype);
          type_css.add(bcs);
        }
      }
    }
  }
  flow_var_type_permit(type_tmp, make_AType(type_css));
  flow_vars(type, type_tmp);
  flow_vars(type_tmp, result);

  AVar *val_tmp = make_AVar(pn->tvals.v[1], es);
  Vec<CreationSet *> val_css;
  forv_CreationSet(cs, val->out->sorted) {
    Sym *ts = cs->sym;
    if (ts->type->asymbol) {
      if (ts->type->is_meta_type) {
        if (!is_scalar_type(SYMBOL(ts->type->meta_type)))
          val_css.add(cs);
      } else
        if (!is_scalar_type(SYMBOL(ts->type)))
          val_css.add(cs);
    }
  }
  flow_var_type_permit(val_tmp, make_AType(val_css));
  flow_vars(val, val_tmp);
  flow_vars(val_tmp, result);
}

static void
expr_domain(PNode *pn, EntrySet *es) {
  assert(0);
}

static void
expr_reduce(PNode *pn, EntrySet *es) {
  assert(0);
}

static void
expr_simple_seq(PNode *pn, EntrySet *es) {
  AVar *container = make_AVar(pn->lvals.v[0], es);
  CreationSet *cs = creation_point(container, sym_sequence);
  AVar *element = get_element_avar(cs);
  update_gen(element,  make_abstract_type(sym_int));
}

static void
expr_create_domain(PNode *pn, EntrySet *es) {
  AVar *container = make_AVar(pn->lvals.v[0], es);
  creation_point(container, sym_domain);
}

static void
write_transfer_function(PNode *pn, EntrySet *es) {
  AVar *result = make_AVar(pn->lvals.v[0], es);
  update_gen(result, make_abstract_type(sym_int));
}

static void
read_transfer_function(PNode *pn, EntrySet *es) {
  AVar *result = make_AVar(pn->lvals.v[0], es);
  update_gen(result, make_abstract_type(sym_int));
}

static void
array_index(PNode *pn, EntrySet *es) {
  AVar *result = make_AVar(pn->lvals.v[0], es);
  AVar *array = make_AVar(pn->rvals.v[2], es);
  set_container(result, array);
  forv_CreationSet(a, array->out->sorted) {
    if (a->sym->element)
      flow_vars(get_element_avar(a), result);
  }
}

static void
array_set(PNode *pn, EntrySet *es) {
  AVar *result = make_AVar(pn->lvals.v[0], es);
  AVar *array = make_AVar(pn->rvals.v[2], es);
  AVar *val = make_AVar(pn->rvals.v[pn->rvals.n-1], es);
  set_container(result, array);
  forv_CreationSet(a, array->out->sorted) {
    if (a->sym->element) {
      if (a->sym->element->type && a->sym->element->type->asymbol 
          && is_scalar_type(SYMBOL(a->sym->element->type)))
        update_gen(get_element_avar(a), make_abstract_type(a->sym->element->type));
      else
        flow_vars(val, get_element_avar(a));
    }
  }
  flow_vars(array, result);
}

static void
ptr_eq(PNode *pn, EntrySet *es) {
  AVar *result = make_AVar(pn->lvals.v[0], es);
  update_gen(result, make_abstract_type(sym_int));
}

static void
ptr_neq(PNode *pn, EntrySet *es) {
  AVar *result = make_AVar(pn->lvals.v[0], es);
  update_gen(result, make_abstract_type(sym_int));
}

static void
array_pointwise_op(PNode *pn, EntrySet *es) {
  AVar *result = make_AVar(pn->lvals.v[0], es);
  AVar *array = make_AVar(pn->rvals.v[2], es);
  flow_vars(array, result);
}

static void
string_op(PNode *pn, EntrySet *es) {
  AVar* result = make_AVar(pn->lvals.v[0], es);
  update_gen(result, make_abstract_type(sym_string));
}

static void
make_seq(PNode *pn, EntrySet *es) {
  AVar *result = make_AVar(pn->lvals.v[0], es);
  CreationSet *cs = creation_point(result, sym_sequence);
  AVar *element = get_element_avar(cs);
  for (int i = 2; i < pn->rvals.n; i++) {
    AVar *av = make_AVar(pn->rvals.v[i], es);
    flow_vars(av, element);
  }
  update_gen(result, make_AType(cs));
}

static void
seqcat_seq(PNode *pn, EntrySet *es) {
  AVar *result = make_AVar(pn->lvals.v[0], es);
  AVar *s1 = make_AVar(pn->rvals.v[2], es);
  AVar *s2 = make_AVar(pn->rvals.v[2], es);
  forv_CreationSet(a, s1->out->sorted) {
    AVar *ea = get_element_avar(a);
    forv_CreationSet(b, s2->out->sorted) {
      AVar *eb = get_element_avar(b);
      flow_vars(eb, ea);
    }
  }
  flow_vars(s1, result);
}

static void
seqcat_element(PNode *pn, EntrySet *es) {
  AVar *result = make_AVar(pn->lvals.v[0], es);
  AVar *s1 = make_AVar(pn->rvals.v[2], es);
  AVar *s2 = make_AVar(pn->rvals.v[2], es);
  forv_CreationSet(a, s1->out->sorted) {
    AVar *ea = get_element_avar(a);
    flow_vars(s2, ea);
  }
  flow_vars(s1, result);
}

static void
indextype_get(PNode *pn, EntrySet *es) {
  AVar *result = make_AVar(pn->lvals.v[0], es);
  AVar *i = make_AVar(pn->rvals.v[2], es);
  forv_CreationSet(a, i->out->sorted) {
    AVar *ea = get_element_avar(a);
    flow_vars(ea, result);
  }
}

static void
indextype_set(PNode *pn, EntrySet *es) {
  AVar *result = make_AVar(pn->lvals.v[0], es);
  AVar *i = make_AVar(pn->rvals.v[2], es);
  AVar *x = make_AVar(pn->rvals.v[3], es);
  forv_CreationSet(a, i->out->sorted) {
    AVar *ea = get_element_avar(a);
    flow_vars(x, ea);
  }
  flow_vars(x, result);
}

static void
chapel_defexpr(PNode *pn, EntrySet *es) {
  AVar *tav = make_AVar(pn->rvals.v[2], es);
  AVar *result = make_AVar(pn->lvals.v[0], es);
  int type_expr = pn->rvals.n > 3;
  if (type_expr)
    tav = make_AVar(pn->rvals.v[3], es);
  forv_CreationSet(tt, tav->out->sorted) {
    Sym *type_sym = !type_expr ? tt->sym->meta_type : tt->sym;
    Type *type = dynamic_cast<Type*>(type_sym->asymbol ? SYMBOL(type_sym) : 0);
    if (!type) {
      if (!type_expr)
        creation_point(result, type_sym);
      else
        update_gen(result, make_AType(tt));
    } else {
      if (type->defaultValue) {
        Sym *val = get_defaultVal(type);
        Var *v = val->var;
        if (!v)
          v = val->var = new Var(val);
        AVar *av = make_AVar(v, es);
        add_var_constraint(av);
        flow_vars(av, result);
      } else if (type->defaultConstructor) {
        if (!type_expr) {
          Sym *c = constructor_name(type);
          Var *cvar = c->var;
          if (!cvar)
            cvar = c->var = new Var(c);
          AVar *cavar = make_AVar(cvar, es);
          AType *ctype = make_abstract_type(c);
          CreationSet *cs = ctype->v[0];
          update_gen(cavar, ctype);
          Vec<AVar *> args;
          function_dispatch(pn, es, cavar, cs, args, Partial_NEVER);
        } else
          update_gen(result, make_AType(tt));
      } else if (type == dtUnknown) {
        update_gen(result, make_abstract_type(sym_null));
      } else
        fail("Type without defaultValue or defaultConstructor");
    }
  }
}

static void
chpl_alloc(PNode *pn, EntrySet *es) {
  AVar *tav = make_AVar(pn->rvals.v[2], es);
  AVar *result = make_AVar(pn->lvals.v[0], es);
  forv_CreationSet(cs, *tav->out) {
    Sym *ts = cs->sym;
    if (ts->is_meta_type) ts = ts->meta_type;
    if (ts->asymbol && is_scalar_type(SYMBOL(ts)))
      ; // update_gen(result, make_abstract_type(ts))  not permitted
    else
      creation_point(result, ts);
  }
}

static void
pure_return(PNode *pn, EntrySet *es) {
  AVar *tav = make_AVar(pn->rvals.v[2], es);
  AVar *result = make_AVar(pn->lvals.v[0], es);
  forv_CreationSet(cs, *tav->out) {
    Sym *ts = cs->sym;
    if (ts->is_meta_type) ts = ts->meta_type;
    if (ts->asymbol && is_scalar_type(SYMBOL(ts)))
      update_gen(result, make_abstract_type(ts));
    else
      // creation_point(result, ts) not permitted
      ;
  }
}

int 
ast_to_if1(Vec<AList<Stmt> *> &stmts) {
  Vec<BaseAST *> syms;
  close_symbols(stmts, syms);
  qsort(syms.v, syms.n, sizeof(syms.v[0]), compar_baseast);
  init_symbols();
  debug_new_ast(stmts, syms);
  map_asts(syms);
  build_builtin_symbols();
  Vec<Type *> types;
  build_types(syms, &types);
  build_symbols(syms);
  method_token = Symboltable::lookupInternal("_methodToken")->asymbol->sym;
  setter_token = Symboltable::lookupInternal("_setterToken")->asymbol->sym;
  if1_set_primitive_types(if1);
  forv_Type(t, types)
    if (t->defaultValue)
      get_defaultVal(t);
  build_classes(syms);
  finalize_types(if1, false);
  if (build_functions(syms) < 0) return -1;
#define REG(_n, _f) pdb->fa->primitive_transfer_functions.put(_n, new RegisteredPrim(_f));
#define SREG(_n, _f) pdb->fa->primitive_transfer_functions.put(if1_cannonicalize_string(if1,_n), new RegisteredPrim(_f));
  REG(expr_simple_seq_symbol->name, expr_simple_seq);
  REG(expr_domain_symbol->name, expr_domain);
  REG(expr_create_domain_symbol->name, expr_create_domain);
  REG(expr_reduce_symbol->name, expr_reduce);
  REG(cast_symbol->name, cast_value);
  REG(write_symbol->name, write_transfer_function);
  REG(writeln_symbol->name, write_transfer_function);
  REG(read_symbol->name, read_transfer_function);
  REG(array_index_symbol->name, array_index);
  REG(array_set_symbol->name, array_set);
  REG(make_seq_symbol->name, make_seq);
  REG(chapel_defexpr_symbol->name, chapel_defexpr);
  SREG("ptr_eq", ptr_eq);
  SREG("ptr_neq", ptr_neq);
  SREG("array_pointwise_op", array_pointwise_op);
  SREG("string_op", string_op);
  SREG("seqcat_seq", seqcat_seq);
  SREG("seqcat_element", seqcat_element);
  SREG("indextype_get", indextype_get);
  SREG("indextype_set", indextype_set);
  SREG("chpl_alloc", chpl_alloc);
  SREG("pure_return", pure_return);
  finalize_symbols(if1);
  build_type_hierarchy();
  finalize_types(if1, false);  // again to catch any new ones
  return 0;
}

int
AST_to_IF1(Vec<AList<Stmt> *> &stmts) {
  if (ast_to_if1(stmts) < 0)
    fail("unable to analyze AST");
  return 0;
}

void 
print_AST_Expr_types(BaseAST *ast) {
  GET_AST_CHILDREN(ast, getStuff);
  forv_BaseAST(a, getStuff.asts)
    print_AST_Expr_types(a);
  Expr *x = dynamic_cast<Expr*>(ast);
  if (x) {
    if (x->ainfo->rval && x->ainfo->rval->var) {
      printf("%s %d %s %d\n", x->ainfo->rval->name ? x->ainfo->rval->name : "", 
                           x->ainfo->rval->id, 
                           x->ainfo->rval->var->type->name ?  x->ainfo->rval->var->type->name : "", 
                           x->ainfo->rval->var->type->id);
      printf("%X\n", (int)(intptr_t)type_info(x->ainfo));
    }
    Type *t = type_info(ast);
    assert(t);
  }
}

void 
print_AST_types() {
  forv_Fun(f, pdb->fa->funs) {
    AAST *a = dynamic_cast<AAST *>(f->ast);
    DefExpr* defExpr = dynamic_cast<DefExpr*>(a->xast);
    FnSymbol* fn = dynamic_cast<FnSymbol*>(defExpr->sym);
    print_AST_Expr_types(fn->body);
  }
}

static void
ast_sym_info(BaseAST *a, Symbol *s, IFAAST **ast, Sym **sym) {
  *ast = 0;
  *sym = 0;
  if (a) {
    Expr *e = dynamic_cast<Expr *>(a);
    if (e)
      *ast = e->ainfo;
    else {
      Stmt *stmt = dynamic_cast<Stmt *>(a);
      if (stmt)
        *ast = stmt->ainfo;
      else {
        Symbol *symbol = dynamic_cast<Symbol *>(a);
        if (symbol) {
          if (symbol->asymbol)
            *sym = symbol->asymbol->sym;
        } else {
          Type *t = dynamic_cast<Type *>(a);
          *sym = t->asymbol->sym;
        }
      }
    }
  }
  if (!sym)
    *sym = s->asymbol->sym;
}

static Type *
to_AST_type(Sym *type) {
  if (type == sym_void || type == sym_void->meta_type)
    return dtVoid;
#ifdef COMPLETE_TYPING
  assert(type);
#endif
  if (!type)
    return dtUnknown;
  if (type->is_meta_type) {
    if (type->meta_type->asymbol)
      return ((Type*)(SYMBOL(type->meta_type)))->metaType;
    else
      return dtUnknown;
  }
  ASymbol *asymbol = (ASymbol*)type->asymbol;
  BaseAST *atype = asymbol->symbol;
  if (!atype)
    atype = SYMBOL(asymbol->sym->meta_type);
  Type *btype = dynamic_cast<Type *>(atype);
  if (!btype) {
    TypeSymbol *ts = dynamic_cast<TypeSymbol *>(atype);
    if (ts)
      btype = ts->definition;
  }
#ifdef COMPLETE_TYPING
  assert(btype);
#endif
  if (!btype)
    return dtUnknown;
  return btype;
}

Type *
type_info(BaseAST *a, Symbol *s) {
  if (TypeSymbol *ts = dynamic_cast<TypeSymbol*>(a))
    return ts->type;
  IFAAST *ast = 0;
  Sym *sym = 0;
  ast_sym_info(a, s, &ast, &sym);
  Sym *type = 0;
  if (ast) {
    type = type_info(ast, sym);
    goto Ldone;
  }
#ifdef COMPLETE_TYPING
  assert(sym);
#endif
  if (!sym)
    return dtUnknown;
  if (sym->type_kind && sym->type_kind != Type_VARIABLE) {
    type = sym->meta_type;
    goto Ldone;
  }
  if (sym->type && sym->type->type_kind == Type_TAGGED) {
    type = sym->type;
    goto Ldone;
  }
  if (sym->var && sym->var->type) {
    type = sym->var->type;
    goto Ldone;
  }
  if (sym->type) {
    type = sym->type;
    goto Ldone;
  }
 Ldone:
  return to_AST_type(type);
}

Type *
return_type_info(FnSymbol *fn) {
  if (fn->asymbol && fn->asymbol->sym)
    return to_AST_type(fn->asymbol->sym->fun->rets.v[0]->type);
  else
    return dtUnknown;  // analysis not run
}

static int
is_operator_name(char *name) {
  if (name[0] == '=' && !name[1])
    return false;
  if (OPERATOR_CHAR(name[0]) && (!name[1] || OPERATOR_CHAR(name[1])))
    return true;
  return false;
}

static int
is_assign(char *name) {
  return (name[0] == '=' && !name[1]);
}

int
call_info(Expr* a, Vec<FnSymbol *> &fns, int find_type) {
  FnSymbol* f = a->getStmt()->parentFunction();
  fns.clear();
  if (!f) // this is not executable code
    return -1;
  Fun *fun = f->asymbol->sym->fun;
  IFAAST *ast = a->ainfo;
  if (!ast)
    return -1; // this code is not known to analysis
  PNode *found_pn = NULL;
  forv_PNode(pn, ast->pnodes) {
    if (pn->code->kind != Code_SEND)
      continue;
    Vec<Fun *> *ff = fun->calls.get(pn);
    if (ff) {
      forv_Fun(f, *ff) {
        FnSymbol *fs = dynamic_cast<FnSymbol *>(SYMBOL(f->sym));
        assert(fs);
        switch (find_type) {
          case CALL_INFO_FIND_SINGLE: break;
          case CALL_INFO_FIND_ALL: break;
          case CALL_INFO_FIND_OPERATOR: 
            if (!is_operator_name(fs->name)) continue;
            break;
          case CALL_INFO_FIND_FUNCTION:
            if (is_operator_name(fs->name)) continue;
            break;
          case CALL_INFO_FIND_ASSIGN:
            if (!is_assign(fs->name)) continue;
            break;
          case CALL_INFO_FIND_NON_ASSIGN:
           if (is_assign(fs->name)) continue;
            break;
        }
        if (found_pn && found_pn != pn && find_type != CALL_INFO_FIND_ALL)
          fail("bad call to call_info");
        found_pn = pn;
        fns.add(fs);
      }
    }
  }
  return 0;
}

int 
constant_info(BaseAST *a, Vec<SymExpr *> &constants, Symbol *s) {
  constants.clear();
  IFAAST *ast = 0;
  Sym *sym = 0;
  ast_sym_info(a, s, &ast, &sym);
  if (!ast && !sym)
    return constants.n;
  Vec<Sym *> consts;
  constant_info(ast, consts, sym);
  forv_Sym(ss, consts)
    constants.add(get_constant_SymExpr(ss));
  return constants.n;
}

int
function_is_used(FnSymbol *fn) {
  if (if1->callback) {
    if (fn->asymbol)
      return fn->asymbol->sym->fun->ess.n != 0;
    else
      return false;
  } else
    return true; // analysis not run   
}

int
type_is_used(TypeSymbol *t) {
  if (if1->callback) {
    if (!dynamic_cast<ClassType*>(t->definition))
      return true;
    if (t->asymbol)
      return t->asymbol->sym->meta_type->creators.n != 0;
    else
      return false;
  } else
    return true; // analysis not run   
}

int
AST_is_used(BaseAST *a, Symbol *s) {
  IFAAST *ast = 0;
  Sym *sym = 0;
  ast_sym_info(a, s, &ast, &sym);
  if (ast && !type_info(ast, sym))
    return 0;
  if (!sym)
    return 0;
  if (!sym->var)
    return 0;
  return sym->type != 0;
}

static int
member_info(Sym *t, char *name, int *offset, Type **type) {
  int oresult = -1;
  Vec<Sym *> iv_type;
  Vec<Sym *> ttypes, *types = 0;
  if (t->type_kind == Type_LUB)
    types = &t->has;
  else {
    ttypes.add(t);
    types = &ttypes;
  }
  forv_Sym(s, *types) {
    forv_CreationSet(cs, s->creators) {
      AVar *iv = cs->var_map.get(name);
      if (iv) {
        if (oresult >= 0 && oresult != iv->ivar_offset)
          return analysis_error(AERROR_MISMATCH_OFFSETS);
        oresult = iv->ivar_offset;
        iv_type.set_add(iv->type);
      }
    }
  }
  *offset = oresult;
  Sym *tmp = 0;
  if (!iv_type.n)
    *type = NULL;
  else {
    if (iv_type.n == 1)
      tmp = iv_type.v[0];
    else 
      tmp = concrete_type_set_to_type(iv_type);
    *type = dynamic_cast<Type *>(SYMBOL(tmp));
  }
  return 0;
}

int
resolve_member_access(Expr *e, int *offset, Type **type) {
  *offset = -1;
  *type = 0;
  if (e->ainfo->pnodes.n < 1)
    return analysis_error(AERROR_BAD_STATE);
  PNode *pn = e->ainfo->pnodes.v[0];
  if (pn->code->kind != Code_SEND)
    return analysis_error(AERROR_BAD_STATE);
  if (pn->rvals.n < 4)
    return analysis_error(AERROR_BAD_STATE);
  if (pn->rvals.v[0]->sym != sym_operator)
    return analysis_error(AERROR_BAD_STATE);
  Sym *obj_type = pn->rvals.v[1]->type;
  char *sel = pn->rvals.v[3]->sym->name;
  return member_info(obj_type, sel, offset, type);
}

int
resolve_member(ClassType *t, VarSymbol *v, int *offset, Type **type) {
  return member_info(t->asymbol->sym, v->name, offset, type);
}

int
structural_subtypes(Type *t, Vec<Type *> subtypes) {
  if (!t->asymbol)
    return -1;
  subtypes.clear();
  Sym *s = t->asymbol->sym;
  forv_Sym(ss, s->specializers) if (ss) {
    Type *tt = dynamic_cast<Type *>(SYMBOL(ss)); assert(tt);
    subtypes.add(tt);
  }
  qsort(subtypes.v, subtypes.n, sizeof(subtypes.v[0]), compar_syms);
  return 0;
}

Type *
element_type_info(TypeSymbol *t) {
  if (t->definition->asymbol && t->definition->asymbol->sym && t->definition->asymbol->sym->element)
    return to_AST_type(t->definition->asymbol->sym->element->type);
  else
    return dtUnknown;  // analysis not run
}

float
execution_frequence_info(Expr *expr) {
  IFAAST *ast = expr->ainfo;
  if (!ast)
    return -1.0; // this code is not known to analysis    
  float freq = 0.0;
  forv_PNode(pn, ast->pnodes) {
    if (pn->code->kind != Code_SEND)
      continue;
    freq += pn->execution_frequency;
  }
  return freq;
}

float
execution_frequence_info(FnSymbol *fn) {
  return fn->asymbol->sym->fun->execution_frequency;
}
