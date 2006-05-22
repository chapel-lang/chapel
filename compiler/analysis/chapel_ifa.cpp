#include <typeinfo>
#include "chpl.h"
#include "analysis.h"
#include "astutil.h"
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
#include "fail.h"
#include "runtime.h"

#define VARARG_END     0ll
//#define MINIMIZED_MEMORY 1  // minimize the memory used by Sym's... needs valgrind checking for safety
//#define USE_FLOAT_128

#define SYMBOL(_x) (((ASymbol*)(_x)->asymbol)->symbol)

#define OPERATOR_CHAR(_c) \
(((_c > ' ' && _c < '0') || (_c > '9' && _c < 'A') || \
  (_c > 'Z' && _c < 'a') || (_c > 'z')) &&            \
   _c != '_'&& _c != '?' && _c != '$')                \

Vec<AError *> analysis_errors;

#define _EXTERN
#define _INIT = NULL
#include "analysis_ops.h"

class LabelMap : public Map<char *, Stmt *> {};

class AnalysisOp : public gc { public:
  char *name;
  char *internal_name;
  PrimitiveTransferFunctionPtr ptfn;
  Prim *prim;

  AnalysisOp(char *aname, char *aninternal_name, PrimitiveTransferFunctionPtr pfn)
    : name(aname), internal_name(aninternal_name), ptfn(pfn), prim(0)
    {}
  AnalysisOp(char *aname, char *aninternal_name, Prim *ap) 
    : name(aname), internal_name(aninternal_name), ptfn(0), prim(ap)
    {}
};

static Sym *sym_anyscalar = 0;

static Sym *cast_symbol = 0;

static int init_function(FnSymbol *f);
static int build_function(FnSymbol *f);
static void map_asts(Vec<BaseAST *> &syms);
static void build_symbols(Vec<BaseAST *> &syms);
static void build_types(Vec<BaseAST *> &syms, Vec<Type *> *types = NULL);
static void build_classes(Vec<BaseAST *> &syms);
static int gen_if1(BaseAST *ast);
static void finalize_function(Fun *fun, int instantiation);
static Type * to_AST_type(Sym *type);

class ScopeLookupCache : public Map<char *, Vec<Fun *> *> {};
static ScopeLookupCache universal_lookup_cache;
static int finalized_symbols = 0;
static Map<Sym *, SymExpr *> constant_cache;

struct TraverseASTs {
  Vec<BaseAST *> asts;
};

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
ASymbol::source_line() {
  if (symbol) {
    ModuleSymbol *m = symbol->getModule();
    if (!squelch_header_errors || (m && m->modtype == MOD_USER))
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

int
AAST::source_line() {
  ModuleSymbol *m = xast->getModule();
  if (!squelch_header_errors || (m && m->modtype == MOD_USER))
    return line();
  return 0;
}


Sym *
AAST::symbol() {
  if (rval) return rval;
  return sym;
}

IFAAST*
AAST::copy_node(ASTCopyContext* context) {
  AAST *a = new AAST(*this);
  for (int i = 0; i < a->pnodes.n; i++) {
    PNode *pn = context->nmap->get(a->pnodes.v[i]);
    assert(pn);
    a->pnodes.v[i] = pn;
  }
  return a;
}

Vec<Fun *> *
AAST::visible_functions(Sym *arg0) {
  Vec<Fun *> *v = 0;
  if (arg0->fun) {
    Fun *f = arg0->fun;
    v = new Vec<Fun *>;
    v->add(f);
    return v;
  }
  char *name = 0;
  if (arg0->is_symbol)
    name = arg0->name;
  else
    name = if1_cannonicalize_string(if1, "this");
  SymScope* scope = this->xast->parentScope;
  Vec<FnSymbol *> fss;
  scope->getVisibleFunctions(&fss, name);
  v = new Vec<Fun *>;
  forv_Vec(FnSymbol, x, fss)
    v->set_add(x->asymbol->sym->fun);
  Vec<Fun *> *universal = universal_lookup_cache.get(name);
  if (universal)
    v->set_union(*universal);
  return v;
}

void
clone_baseast(BaseAST* old_ast, BaseAST* new_ast, ASTCopyContext *context) {
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
        Fun *new_f = context->fmap->get(old_s->asymbol->sym->fun);
        if (new_f) {
          new_s->asymbol->sym->fun = new_f;
          new_f->sym = new_s->asymbol->sym;
        }
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
  ASTCopyContext local_context;
  local_context.fmap = context->fmap;
  local_context.nmap = new Map<PNode *, PNode *>(*context->nmap);
  local_context.vmap = new Map<Var *, Var *>(*context->vmap);
  ASTMap clone_map;
  DefExpr* def_expr = dynamic_cast<DefExpr*>(xast);
  FnSymbol* orig_fn = dynamic_cast<FnSymbol*>(def_expr->sym);
  FnSymbol *new_fn = orig_fn->clone(&clone_map);
  for (int i = 0; i < clone_map.n; i++) {
    if (clone_map.v[i].key) {
      // handle nested functions
      if (FnSymbol *f = dynamic_cast<FnSymbol*>(clone_map.v[i].key)) {
        if (f != orig_fn) {
          Fun *new_f = f->asymbol->sym->fun->copy(0, local_context.vmap);
          local_context.fmap->map_union(*new_f->fmap);
          local_context.nmap->map_union(*new_f->nmap);
          local_context.vmap->map_union(*new_f->vmap);
        }
      }
    }
  }
  for (int i = 0; i < clone_map.n; i++)
    if (clone_map.v[i].key)
      clone_baseast(clone_map.v[i].key, clone_map.v[i].value, &local_context);
  context->smap.move(local_context.smap);
  return new_fn->defPoint->ainfo;
}

static Sym *
base_type(Sym *s) {
  if (UserType *t = dynamic_cast<UserType*>(SYMBOL(s)))
    return unalias_type(base_type(t->underlyingType->asymbol->sym));
  return unalias_type(s);
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

static void
make_chapel_meta_type(Type *tt) {
  if (tt->astType == TYPE_META)
    return;
  Sym *t = tt->asymbol->sym;
  if (!tt->metaType->asymbol)
    map_type(tt->metaType);
  t->meta_type = tt->metaType->asymbol->sym;
  t->meta_type->meta_type = t;
  t->meta_type->is_meta_type = 1;
  t->meta_type->type = t->meta_type;
  t->meta_type->type_kind = Type_PRIMITIVE;
}

static Sym *
find_or_make_lub_type(Vec<Type *> *types, Sym *t) {
  Type *tt = find_or_make_sum_type(types);
  if (tt->asymbol)
    return tt->asymbol->sym;
  tt->asymbol = (ASymbol*)t->asymbol;
  tt->asymbol->sym = t;
  SYMBOL(tt) = tt;
  assert(tt->asymbol->sym->type_kind == Type_LUB);
  make_chapel_meta_type(tt);
  return t;
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
  return find_or_make_lub_type(&types, t);
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
    if (s->is_constant) {
      VarSymbol *vs = dynamic_cast<VarSymbol*>(SYMBOL(s));
      if (vs && vs->literalType) {
        s->meta_type = vs->literalType->metaType->asymbol->sym;
        s->meta_type->meta_type = s;
      } else
        make_meta_type(s);
    }
    if (s->is_constant || s->is_symbol)
      s->nesting_depth = 0;
    else if (s->type_kind)
      s->nesting_depth = 0;
    if (s->asymbol && SYMBOL(s) && SYMBOL(s)->astType == SYMBOL_VAR) {
      VarSymbol *v = (VarSymbol*)SYMBOL(s);
      if (v->type && v->type != dtUnknown) {
        Sym *t = v->type->asymbol->sym;
        if (t->num_kind || s->is_constant)
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

static void
install_new_asts(Vec<FnSymbol *> &funs, Vec<TypeSymbol *> &types) {
  Vec<BaseAST *> syms, tsyms;
  forv_Vec(FnSymbol, f, funs)
    collect_asts(&syms, f->defPoint->parentStmt);
  forv_Vec(TypeSymbol, t, types)
    collect_asts(&syms, t->defPoint->parentStmt);
  tsyms.move(syms);
  syms.set_union(tsyms);
  syms.set_to_vec();
  qsort(syms.v, syms.n, sizeof(syms.v[0]), compar_baseast);
  qsort(funs.v, funs.n, sizeof(funs.v[0]), compar_baseast);
  map_asts(syms);
  build_types(syms);
  build_symbols(syms);
  forv_Vec(TypeSymbol, new_ts, types) {
    Type* new_t = new_ts->definition;
    if (new_t->instantiatedFrom)
      new_t->asymbol->sym->instantiates = new_t->instantiatedFrom->asymbol->sym;
    for (int i = 0; i < new_t->substitutions.n; i++) if (new_t->substitutions.v[i].key) {
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
    if (Symbol *s = dynamic_cast<Symbol *>(ast)) {
      if (s->asymbol)
        initialize_Sym_for_fa(s->asymbol->sym);
    } else if (Type *t = dynamic_cast<Type *>(ast)) {
      if (t->asymbol)
        initialize_Sym_for_fa(t->asymbol->sym);
    }
  }
  forv_Vec(FnSymbol, f, funs)
    finalize_function(f->asymbol->sym->fun, 1);
  // Enter new nested functions into the dispatch tables 
  //   if their parent was instantiated
  Vec<Fun *> funs_set;
  forv_Vec(FnSymbol, fsym, funs)
    funs_set.set_add(fsym->asymbol->sym->fun);
  forv_Vec(FnSymbol, fsym, funs) {
    Fun *f = fsym->asymbol->sym->fun;
    if ((f->nested_in && funs_set.set_in(f->nested_in)) || fsym->visible)
      add_patterns(pdb->fa, f);
  }
  if1_write_log();
}

static Fun *
install_new_asts(FnSymbol *f) {
  Vec<FnSymbol *> funs;
  funs.add(f);
  Vec<TypeSymbol *> types;
  install_new_asts(funs, types);
  return f->asymbol->sym->fun;
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
    Type *new_type = dynamic_cast<Type*>(subs.get(type));
    if (!new_type)
      new_type = type->instantiate_generic(subs);
    if (new_type)
      return new_type->asymbol->sym;
  }
  return 0;
}

int
ACallbacks::formal_to_generic(Sym *s, Sym **ret_generic, int *ret_bind_to_value) {
  ArgSymbol *p = dynamic_cast<ArgSymbol*>(SYMBOL(s));
  if (!p->isGeneric)
    return 0;
  if (p->genericSymbol) {
    if (p->genericSymbol->astType == SYMBOL_TYPE)
      *ret_generic = dynamic_cast<TypeSymbol*>(p->genericSymbol)->definition->asymbol->sym;
    else 
      *ret_generic = p->genericSymbol->asymbol->sym;
    *ret_bind_to_value = 0;
    return 1;
  }
  if (p->intent == INTENT_PARAM) {
    *ret_generic = s;
    *ret_bind_to_value = 1;
    return 1;
  }
  assert(!"unrecognized generic");
  return 0;
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
  if (fsym->asymbol)
    return fsym->asymbol->sym->fun;
  Fun *fun = install_new_asts(fsym);
  fun->wraps = f;
  return fun;
}

Sym *
ACallbacks::coerce(Sym *actual, Sym *formal) {
  Sym *a = actual->scalar_type(), *f = formal->scalar_type();
  if (a && f) {
    Sym *res = coerce_num(a, f);
    if (res == f)
      return f;
    return NULL;
  }
  if (a && formal == sym_string)
    return sym_string;
  return NULL;
}

Sym *
ACallbacks::promote(Fun *f, Sym *formal, Sym *formal_type, Sym *actual) {
  if (!strcmp(f->sym->name, "=") || formal_type->is_meta_type || formal_type->is_system_type)
    return NULL;
  FnSymbol *fn = dynamic_cast<FnSymbol *>(SYMBOL(f->sym));
  if (fn->isMethod && SYMBOL(formal) == fn->_this)
    return NULL;
  Type *t = dynamic_cast<Type *>(SYMBOL(actual));
  if (t && t->scalarPromotionType) {
    Sym *tt = t->scalarPromotionType->asymbol->sym;
    if (formal_type->specializers.in(tt))
      return formal_type;
  }
  return NULL;
}

static void
convert_substitutions(Fun *f, Map<MPosition *, Sym *> &substitutions, Map<Symbol *, Symbol *> &subs) {
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
          subs.put(aa, type->symbol);
        }
      }
    }
  }
}

Fun *
ACallbacks::coercion_wrapper(Fun *f, Map<MPosition *, Sym *> &substitutions) {
  if (!f->ast) 
    return NULL;
  Map<Symbol *, Symbol *> coercions;
  convert_substitutions(f, substitutions, coercions);
  FnSymbol *fndef = dynamic_cast<FnSymbol *>(SYMBOL(f->sym));
  FnSymbol *fsym = fndef->coercion_wrapper(&coercions);
  if (!fsym)
    return NULL;
  if (fsym->asymbol)
    return fsym->asymbol->sym->fun;
  Fun *fun = install_new_asts(fsym);
  fun->wraps = f;
  return fun;
}

Fun *
ACallbacks::promotion_wrapper(Fun *f, Map<MPosition *, Sym *> &substitutions) {
  if (!f->ast) 
    return NULL;
  Map<Symbol *, Symbol *> promotions;
  convert_substitutions(f, substitutions, promotions);
  FnSymbol *fndef = dynamic_cast<FnSymbol *>(SYMBOL(f->sym));
  FnSymbol *fsym = fndef->promotion_wrapper(&promotions);
  if (!fsym)
    return NULL;
  if (fsym->asymbol)
    return fsym->asymbol->sym->fun;
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
  if (!fsym)
    return NULL;
  if (fsym->asymbol)
    return fsym->asymbol->sym->fun;
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
    if (!t)
      if (TypeSymbol *ts = dynamic_cast<TypeSymbol*>(SYMBOL(s->key)))
        t = ts->definition;
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
  new_ast_functions.clear();
  new_ast_types.clear();
  FnSymbol *fsym = fndef->instantiate_generic(&substitutions);
  // SJD: If the where clause evaluates to false, fsym is NULL.
  if (!fsym)
    return NULL;
  install_new_asts(new_ast_functions, new_ast_types);
  Fun *fun = fsym->asymbol->sym->fun;
  fun->wraps = f;
  return fun;
}

Sym *
ASymbol::clone() {
  if (!symbol) { // internal to analysis
    Sym *s = copy()->sym;
    return s;
  } else {
    ASTCopyContext context;
    Type *type = dynamic_cast<Type*>(symbol);
    TypeSymbol *old_type_symbol = dynamic_cast<TypeSymbol*>(type->symbol);
    ASTMap clone_map;
    TypeSymbol *new_type_symbol = old_type_symbol->clone(&clone_map);
    assert(new_type_symbol);
    for (int i = 0; i < clone_map.n; i++)
      if (clone_map.v[i].key)
        clone_baseast(clone_map.v[i].key, clone_map.v[i].value, &context);
    Sym *new_type = context.smap.get(old_type_symbol->definition->asymbol->sym);
    if (!new_type_symbol->asymbol) // SHOULD BE ASSERT
      clone_baseast(old_type_symbol, new_type_symbol, &context);
    make_chapel_meta_type(dynamic_cast<Type*>(SYMBOL(new_type)));
    for (int i = 0; i < new_type->has.n; i++) {
      Sym *s = context.smap.get(new_type->has.v[i]);
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
    s->nesting_depth = LOCALLY_NESTED;
  return s;
}

static VarSymbol *
is_literal(Symbol* sym) {
  if (VarSymbol* var = dynamic_cast<VarSymbol*>(sym))
    if (var->immediate)
      return var;
  return 0;
}

static VarSymbol *
is_symbol(Symbol* sym) {
  if (VarSymbol* var = dynamic_cast<VarSymbol*>(sym))
    if (var->type == dtSymbol)
      return var;
  return 0;
}

static void
map_const(VarSymbol *var) {
  Immediate *imm = var->immediate;
  Sym *sym = if1->constants.get(imm);
  if (sym)
    return;
  sym = new_Sym();
  sym->is_constant = 1;
  PrimitiveType *ptype = dynamic_cast<PrimitiveType*>(var->type);
  sym->implements.add(ptype->literalType->asymbol->sym);
  sym->specializes.add(ptype->literalType->asymbol->sym);
  sym->imm = *imm;
  if1->constants.put(&sym->imm, sym);
  var->asymbol = new_ASymbol(var, sym);
}

static Sym *
make_symbol(char *name) {
  Sym *s = if1_make_symbol(if1, name);
  return s;
}

static void
map_baseast(BaseAST *s) {
  Symbol *sym = dynamic_cast<Symbol *>(s);
  if (sym) {
    if (sym->asymbol) {
      return;
    } else if (sym->astType == SYMBOL_TYPE) {
      return;
    } else if (VarSymbol *var = is_symbol(sym)) {
      Sym *s = make_symbol(var->name);
      sym->asymbol = new_ASymbol(sym, s);
    } else if (VarSymbol *var = is_literal(sym)) {
      map_const(var);
    } else {
      int basic = (s->astType != SYMBOL_FN) && (s->astType != SYMBOL_ENUM);
      sym->asymbol = new_ASymbol(sym, basic);
      SYMBOL(sym) = sym;
      if (!sym->parentScope) {
        sym->asymbol->sym->nesting_depth = 0;
      } else if (sym->parentScope == rootScope ||
                 dynamic_cast<ModuleSymbol*>(sym->parentScope->astParent)) {
        sym->asymbol->sym->nesting_depth = 0;
      } else if (dynamic_cast<FnSymbol*>(sym->parentScope->astParent) ||
                 dynamic_cast<BlockStmt*>(sym->parentScope->astParent)) {
            sym->asymbol->sym->nesting_depth = sym->nestingDepth();
      }
    }
    if (ifa_verbose > 2 && sym->name)
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
          INT_FATAL(s, "unexpected AST type in map_baseast: %s\n", astTypeName[s->astType]);
        }
      }
    }
  }
}

static void
map_asts(Vec<BaseAST *> &syms) {
  if (ifa_verbose > 2)
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

int
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
        case SYMBOL_TYPE: break;
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
          if (p->isExactMatch)
            s->asymbol->sym->is_exact_match = 1;
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
  make_chapel_meta_type(t);
  if (make_default && t->defaultValue)
    get_defaultVal(t);
  if (t->dispatchParents.n > 0) {
    forv_Vec(Type, tt, t->dispatchParents)
      t->asymbol->sym->inherits_add(tt->asymbol->sym);
  }
  if (t->scalarPromotionType)
    t->asymbol->sym->dispatch_types.add(t->scalarPromotionType->asymbol->sym);
  switch (t->astType) {
    default: assert(!"case");
    case TYPE:
      t->asymbol->sym->type_kind = Type_UNKNOWN;
      break;
    case TYPE_PRIMITIVE: {
      PrimitiveType *tt = dynamic_cast<PrimitiveType*>(t);
      if (t == dtUnknown)
        t->asymbol->sym->type_kind = Type_UNKNOWN;
      if (tt->literalType) {
        tt->asymbol->sym->constant_type = tt->literalType->asymbol->sym;
        tt->literalType->asymbol->sym->type_kind = Type_PRIMITIVE;
      }
      break;
    }
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
      if (tt->classTag == CLASS_RECORD)
        t->asymbol->sym->is_value_type = 1;
      if (tt->symbol->hasPragma("data class"))
        t->asymbol->sym->element = new_sym();
      break;
    }
    case TYPE_META: 
      t->asymbol->sym->is_meta_type = 1;
      t->asymbol->sym->type_kind = Type_PRIMITIVE;
      t->asymbol->sym->type = t->asymbol->sym;
      break;
    case TYPE_VARIABLE: {
      VariableType *tt = dynamic_cast<VariableType*>(t);
      tt->asymbol->sym->type_kind = Type_VARIABLE;
      tt->asymbol->sym->meta_type->type_kind = Type_NONE;
      tt->asymbol->sym->meta_type->type = tt->asymbol->sym;
      tt->asymbol->sym->must_specialize = sym_anytype;
      break;
    }
  }
  if (t->isGeneric)
    t->asymbol->sym->is_generic = true;
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
new_global_variable(Sym *&sym, char *name) {
  if (!sym)
    sym = new_sym(name, 1);
  sym->nesting_depth = 0;
  if1_set_builtin(if1, sym, name);
}

static void
new_primitive_object(Sym *&sym, Sym *sym_type, Symbol *symbol, char *name) {
  if (symbol)
    sym = symbol->asymbol->sym;
  new_global_variable(sym, name);
  sym->type = sym_type;
  sym->is_external = 1;
  sym_type->is_unique_type = 1;
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
builtin_Symbol(Type *dt, Sym **sym, char *name) {
  if (fnostdincs) {
    *sym = new_sym(name);
    return;
  }
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

  sym_bool = dtBool->asymbol->sym;

  sym_int8  = dtInt[IF1_INT_TYPE_8]->asymbol->sym;
  sym_int16 = dtInt[IF1_INT_TYPE_16]->asymbol->sym;
  sym_int32 = dtInt[IF1_INT_TYPE_32]->asymbol->sym;
  sym_int64 = dtInt[IF1_INT_TYPE_64]->asymbol->sym;

  sym_uint8  = dtUInt[IF1_INT_TYPE_8]->asymbol->sym;
  sym_uint16 = dtUInt[IF1_INT_TYPE_16]->asymbol->sym;
  sym_uint32 = dtUInt[IF1_INT_TYPE_32]->asymbol->sym;
  sym_uint64 = dtUInt[IF1_INT_TYPE_64]->asymbol->sym;

  sym_float32  = dtFloat[IF1_FLOAT_TYPE_32]->asymbol->sym;
  sym_float64  = dtFloat[IF1_FLOAT_TYPE_64]->asymbol->sym;
  sym_float128 = dtFloat[IF1_FLOAT_TYPE_128]->asymbol->sym;

  sym_complex64 = dtComplex->asymbol->sym;
  sym_string = dtString->asymbol->sym;
  sym_anynum = dtNumeric->asymbol->sym;
  sym_anyscalar = dtScalar->asymbol->sym;
  sym_any = dtAny->asymbol->sym; 
  sym_object = dtObject->asymbol->sym; 
  sym_nil_type = dtNil->asymbol->sym;
  sym_unknown_type = dtUnknown->asymbol->sym;
  sym_value = dtValue->asymbol->sym;
  sym_void_type = dtVoid->asymbol->sym;
  sym_closure = dtClosure->asymbol->sym;
  sym_symbol = dtSymbol->asymbol->sym;

  new_lub_type(sym_any, "any", VARARG_END);
  new_primitive_type(sym_nil_type, "nil_type");
  new_primitive_type(sym_unknown_type, "unknown_type");
  new_primitive_type(sym_void_type, "void_type");
  new_primitive_type(sym_module, "module");
  new_primitive_type(sym_symbol, "symbol");
  if1_set_symbols_type(if1);
  new_primitive_type(sym_closure, "closure");
  new_primitive_type(sym_continuation, "continuation");
  new_primitive_type(sym_vector, "vector");
  new_primitive_type(sym_void_type, "void");
  if (!sym_object)
    sym_object = new_sym("object", 1);
  sym_object->type_kind = Type_RECORD;
  if1_set_builtin(if1, sym_object, "object");
  new_primitive_type(sym_list, "list");
  new_primitive_type(sym_ref, "ref");
  new_primitive_type(sym_value, "value");
  new_primitive_type(sym_set, "set");

  new_primitive_type(sym_int8,   "int8");
  new_primitive_type(sym_int16,  "int16");
  new_primitive_type(sym_int32,  "int32");
  new_primitive_type(sym_int64,  "int64");
  // new_primitive_type(sym_int128, "int128");

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
               sym_int8, sym_int16, sym_int32, sym_int64,
               sym_bool,
               sym_uint8, sym_uint16, sym_uint32, sym_uint64, 
               VARARG_END);

  new_alias_type(sym_size, "size", sym_int64);
  new_alias_type(sym_enum_element, "enum_element", sym_int64);
  new_primitive_type(sym_float32, "float32");
  new_primitive_type(sym_float64, "float64");
  new_primitive_type(sym_float128, "float128");
  new_alias_type(sym_float, "float", sym_float64);
  new_lub_type(sym_anyfloat, "anyfloat", 
               sym_float32, sym_float64, sym_float128, 
               VARARG_END);
  new_primitive_type(sym_complex32, "complex32");
  new_primitive_type(sym_complex64, "complex64");
#ifdef USE_FLOAT_128
  new_primitive_type(sym_complex128, "complex128");
#endif
  new_primitive_type(sym_complex, "complex");
  new_lub_type(sym_anycomplex, "anycomplex", 
               sym_complex32, sym_complex64, 
#ifdef USE_FLOAT_128
               sym_complex128, 
#endif
      VARARG_END);
  new_lub_type(sym_anynum, "anynum", sym_anyint, sym_anyfloat, sym_anycomplex, VARARG_END);
  new_lub_type(sym_anyscalar, "anyscalar", sym_anynum, sym_string, VARARG_END);
  new_primitive_type(sym_char, "char");
  new_primitive_type(sym_string, "string");
  if (!sym_new_object) {
    sym_new_object = new_sym("new_object", 1);
    if1_set_builtin(if1, sym_new_object, "new_object");
  }

  new_primitive_object(sym_nil, sym_nil_type, gNil, "nil");
  new_primitive_object(sym_unknown, sym_unknown_type, gUnknown, "_unknown");
  new_primitive_object(sym_void, sym_void_type, gVoid, "_void");

  sym_init = new_sym(); // placeholder

  builtin_Symbol(dtUnused, &sym_tuple, "tuple");

  // automatic promotions

  sym_bool->specializes.add(sym_uint8);

  sym_uint8->specializes.add(sym_uint16);
  sym_uint16->specializes.add(sym_uint32);
  sym_uint32->specializes.add(sym_uint64);

  sym_uint32->specializes.add(sym_int32);
  sym_uint64->specializes.add(sym_int64);

  sym_int8->specializes.add(sym_int16);
  sym_int16->specializes.add(sym_int32);
  sym_int32->specializes.add(sym_int64);

  sym_int32->specializes.add(sym_float32);
  sym_int64->specializes.add(sym_float64);

  sym_float32->specializes.add(sym_float64);
  sym_float64->specializes.add(sym_float128);

  sym_float32->specializes.add(sym_complex32);
  sym_float64->specializes.add(sym_complex64);
#ifdef USE_FLOAT_128
  sym_float128->specializes.add(sym_complex128);
#endif

  sym_complex32->specializes.add(sym_complex64);
#ifdef USE_FLOAT_128
  sym_complex64->specializes.add(sym_complex128);
#endif

  sym_anynum->specializes.add(sym_string);

  // defined type hierarchy
  
  sym_any->implements.add(sym_unknown_type);
  sym_any->specializes.add(sym_unknown_type);
  sym_object->implements.add(sym_any);
  sym_object->specializes.add(sym_any);
  sym_nil_type->implements.add(sym_object);
  sym_nil_type->specializes.add(sym_object);
  sym_value->implements.add(sym_any);
  sym_value->specializes.add(sym_any);

  make_chapel_meta_type(dtAny);
  sym_anytype = sym_any->meta_type;
  sym_anytype->implements.add(sym_any);
  sym_anytype->specializes.add(sym_any);

  make_chapel_meta_type(dtNil);
  sym_nil_type->implements.add(sym_nil_type->meta_type);
  sym_nil_type->specializes.add(sym_nil_type->meta_type);

  sym_any->is_system_type = 1;
  sym_value->is_system_type = 1;
  sym_object->is_system_type = 1;
  sym_nil_type->is_system_type = 1;
  sym_unknown_type->is_system_type = 1;
  sym_void_type->is_system_type = 1;
  sym_anytype->is_system_type = 1;

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
      }
      labelmap->put(if1_cannonicalize_string(if1, dynamic_cast<LabelStmt*>(stmt)->defLabel->sym->name), target);
      break;
    }
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

static int 
gen_expr_stmt(BaseAST *a) {
  ExprStmt *expr = dynamic_cast<ExprStmt*>(a);
  expr->ainfo->code = expr->expr->ainfo->code;
  return 0;
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
  if (ast && ast->astType == EXPR_SYM) { 
    SymExpr* v = dynamic_cast<SymExpr* >(ast);
    return v->var->astType;
  }
  return (astType_t)0;
}

static Sym *
gen_primitive(CallExpr *s, Vec<Expr *> &args, Vec<Sym *> &rvals) {
  AnalysisOp *op = s->primitive->analysisOp;
  if (!op)
    INT_FATAL(s, "unknown primitive");
  Prim *p = op->prim;
  if (!p) {
    rvals.insert(0, make_symbol(op->internal_name));
    return sym_primitive;
  } else if (p->pos == 0) {
    // unary or n-ary operation
    rvals.insert(0, make_symbol(p->string));
    return sym_primitive;
  } else {
    // binary operation requires the symbol in the center
    rvals.insert(0, rvals.v[0]);
    rvals.v[1] = make_symbol(p->string);
    return sym_operator;
  }
}

static int
gen_call_expr(CallExpr *s) {
  AAST *ast = s->ainfo;
  ast->rval = new_sym();
  ast->rval->ast = ast;
  if (s->baseExpr)
    if1_gen(if1, &ast->code, s->baseExpr->ainfo->code);
  Vec<Expr *> args;
  s->argList->getElements(args);
  if (args.n == 1 && !args.v[0])
    args.n--;
  Vec<Sym *> rvals;
  Vec<char *> arg_names;
  int some_name = 0;
  arg_names.fill(args.n);
  for (int i = 0; i < args.n; i++) {
    Expr *a = args.v[i];
    if1_gen(if1, &ast->code, a->ainfo->code);
    assert(a->ainfo->rval);
    rvals.add(a->ainfo->rval);
    if (NamedExpr *n = dynamic_cast<NamedExpr*>(a)) {
      arg_names.v[i] = if1_cannonicalize_string(if1, n->name);
      some_name = 1;
    }
  }
  Vec<Sym*> trvals;
  astType_t base_symbol = undef_or_fn_expr(s->baseExpr);
  Sym *base = NULL;
  char *n = s->baseExpr ? s->baseExpr->ainfo->rval->name : 0;
  if (s->isPrimitive(PRIMITIVE_GET_MEMBER) ||
             s->isPrimitive(PRIMITIVE_SET_MEMBER)) {
    rvals.insert(1, make_symbol(s->isPrimitive(PRIMITIVE_GET_MEMBER) ? sym_period->name: sym_setter->name));
    assert(!some_name);
    if (rvals.v[2]->imm.const_kind != IF1_CONST_KIND_STRING) {
      rvals.v[2] = make_symbol("_invalid_member_access");
    } else
      rvals.v[2] = make_symbol(rvals.v[2]->imm.v_string);
    base = sym_operator;
  } else if (s->primitive) {
    base = gen_primitive(s, args, rvals);
  } else if (base_symbol == SYMBOL_UNRESOLVED) {
    assert(n);
    base = make_symbol(n);
  } else if (base_symbol == SYMBOL_FN)
    base = dynamic_cast<FnSymbol*>(dynamic_cast<SymExpr*>(s->baseExpr)->var)->asymbol->sym;
  else {
    base = s->baseExpr ? s->baseExpr->ainfo->rval : 0;
  }
  if (base) {
    trvals.add(base);
    arg_names.insert(0, NULL);
  }
  forv_Sym(r, rvals)
    trvals.add(r);
  Code *send = if1_send1(if1, &ast->code);
  if (some_name)
    send->names.move(arg_names);
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
    if (gen_if1(x) < 0) return -1;
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
  if (gen_if1(s->doStmt) < 0) return -1;
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
  gen_if1(s->caseExpr);
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
  Expr *lhs = assign->get(1), *rhs = assign->get(2);
  if (!lhs->ainfo->sym)
    show_error("assignment to non-lvalue", assign->ainfo);
  if1_gen(if1, &assign->ainfo->code, lhs->ainfo->code);
  if1_gen(if1, &assign->ainfo->code, rhs->ainfo->code);
  Sym *rval = rhs->ainfo->rval;
  if (!rval->nesting_depth) {
    rval = new_sym();
    rval->ast = assign->ainfo;
    if1_move(if1, &assign->ainfo->code, rhs->ainfo->rval, rval, assign->ainfo);
  }
  if (!assign->isPrimitive(PRIMITIVE_MOVE)) {
    Sym *old_rval = rval;
    rval = new_sym();
    rval->ast = assign->ainfo;
    if (f_equal_method) {
      fail("not implemented");
#if 0
      Code *c = if1_send(if1, &assign->ainfo->code, 4, 1, make_symbol("="), method_token,
                         lhs->ainfo->rval, old_rval, rval);
      c->ast = assign->ainfo;
      c->partial = Partial_NEVER;
#endif
    } else {
      Code *c = if1_send(if1, &assign->ainfo->code, 3, 1, make_symbol("="), 
                         lhs->ainfo->rval, old_rval, rval);
      c->ast = assign->ainfo;
      c->partial = Partial_NEVER;
    }
  } else
    if1_move(if1, &assign->ainfo->code, rval, lhs->ainfo->sym, assign->ainfo);
  assign->ainfo->rval = rval;
  return 0;
}

static int
gen_if1(BaseAST *ast) {
  // special cases
  switch (ast->astType) {
    default: break;
    case STMT_WHEN: return 0;
    case STMT_SELECT: gen_select(ast); return 0;
  }
  // recurse
  if (dynamic_cast<Expr*>(ast) || dynamic_cast<Stmt*>(ast)) {
    GET_AST_CHILDREN(ast, getStuff);
    DefExpr* def_expr = dynamic_cast<DefExpr*>(ast);
    if (!def_expr || !dynamic_cast<FnSymbol*>(def_expr->sym))
      forv_BaseAST(a, getStuff.asts)
        if (gen_if1(a) < 0)
          return -1;
  }
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
        if (dynamic_cast<DefExpr*>(exprStmt->expr)) {
          // do nothing
        } else {
          if (gen_expr_stmt(ast) < 0)
            return -1;
        }
      }
      break;
    case STMT_RETURN: {
      ReturnStmt *s = dynamic_cast<ReturnStmt*>(ast);
      Sym *fn = s->getFunction()->asymbol->sym;
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
      Sym *sym = (s->var->astType == SYMBOL_TYPE) 
        ? (((TypeSymbol*)s->var)->definition->asymbol->sym)
        : s->var->asymbol->sym;
      s->ainfo->sym = sym;
      s->ainfo->rval = sym;
      break;
    }
    case EXPR_DEF: {
      DefExpr* s = dynamic_cast<DefExpr*>(ast);
      if (s->sym->asymbol && !s->sym->asymbol->sym->ast)
        s->sym->asymbol->sym->ast = s->ainfo;
      break;
    }
    case EXPR_CALL: {
      CallExpr* call = dynamic_cast<CallExpr*>(ast);
      if (call->isAssign()) {
        if (gen_assignment(call) < 0) return -1;
      } else {
        if (gen_call_expr(call) < 0) return -1;
      }
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
    case EXPR_NAMED: {
      NamedExpr *s = dynamic_cast<NamedExpr *>(ast);
      if1_gen(if1, &s->ainfo->code, s->actual->ainfo->code);
      s->ainfo->rval = s->actual->ainfo->rval;
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
  case OBJECT:
    break;
  }
  return 0;
}

// static void
// fun_where_equal_constant(FnSymbol *f, SymExpr* v, VarSymbol *c) {
//   if (ArgSymbol *s = dynamic_cast<ArgSymbol*>(v->var)) {
//     if (!s->isGeneric) {
//       show_error("where constraint on non-generic %s", f->body->ainfo, s->name);
//       return;
//     }
//     s->asymbol->sym->must_implement = c->asymbol->sym;
//     s->asymbol->sym->must_specialize =  c->asymbol->sym;
//   }
// }

// static void
// fun_where_clause(FnSymbol *f, Expr *w) {
//   CallExpr *op = dynamic_cast<CallExpr*>(w);
//   if (!op)
//     return;
//   SymExpr *base = dynamic_cast<SymExpr*>(op->baseExpr);
//   if (!base)
//     return;
//   if (!strcmp(base->var->name, "&&")) {
//     op->primitive = primitives[PRIMITIVE_LAND];
//     op->baseExpr->remove();
//     fun_where_clause(f, op->get(1));
//     fun_where_clause(f, op->get(2));
//   } else if (!strcmp(base->var->name, "==")) {
//     op->primitive = primitives[PRIMITIVE_EQUAL];
//     op->baseExpr->remove();
//     VarSymbol *c = 0;
//     SymExpr* v = 0;
//     if ((c = get_constant(op->get(1))) && (v = dynamic_cast<SymExpr*>(op->get(2)))) 
//       fun_where_equal_constant(f, v, c);
//     else if ((c = get_constant(op->get(2))) && (v = dynamic_cast<SymExpr*>(op->get(1)))) 
//       fun_where_equal_constant(f, v, c);
//   }
// }

static int
is_this_fun(FnSymbol *f) {
  return !strcmp(f->asymbol->sym->name, "this");
}

static int
is_assign_this_fun(FnSymbol *f) {
  return !strcmp(f->asymbol->sym->name, "=this");
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
  if (is_this_fun(f)) {
    if (is_Sym_OUT(args.v[0]->asymbol->sym))
      out_args.add(args.v[0]->asymbol->sym);
    as[iarg++] = args.v[0]->asymbol->sym;
  } else if (is_assign_this_fun(f)) {
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
    if (f->isMethod) {
      // this
      if (args.n) {
        if (is_Sym_OUT(args.v[0]->asymbol->sym))
          out_args.add(args.v[0]->asymbol->sym);
        as[iarg++] = args.v[0]->asymbol->sym;
      }
    }
    if (!f->isMethod) {
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
  Code *c = if1_send(if1, &body, 4, 0, sym_primitive, sym_reply, fn->cont, fn->ret);
  forv_Sym(r, out_args)
    if1_add_send_arg(if1, c, r);
  c->ast = ast;
  c->partial = Partial_NEVER;
  if1_closure(if1, fn, body, iarg, as);
  fn->ast = ast;
  if (f->_this && f->fnClass != FN_CONSTRUCTOR)
    fn->self = f->_this->asymbol->sym;
  //  fun_where_clause(f, f->whereExpr);
  return 0;
}

static int
init_function(FnSymbol *f) {
  Sym *s = f->asymbol->sym;
  if (ifa_verbose > 2 && f->name)
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
  s->nesting_depth = f->nestingDepth();
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
  if (ifa_verbose > 2)
    printf("build_classes: %d classes\n", classes.n);
  forv_Vec(ClassType, c, classes) {
    Sym *csym = c->asymbol->sym;
    forv_Vec(Symbol, tmp, c->fields)
      csym->has.add(tmp->asymbol->sym);
    forv_Vec(TypeSymbol, tmp, c->types) if (tmp)
      if (tmp->definition->astType == TYPE_USER ||
          tmp->definition->astType == TYPE_VARIABLE)
        csym->has.add(tmp->definition->asymbol->sym);
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
handle_argument(Sym *s, char *name, Fun *fun, int added, MPosition &p, int instantiation) {
  if (s->is_pattern) {
    p.push(1);
    forv_Sym(ss, s->has) {
      added = handle_argument(ss, name, fun, added, p, instantiation);
      p.inc();
    }
    p.pop();
  }
  if (!instantiation) { 
    // non-scoped lookup if any parameteter is specialized on a reference type
    // (is dispatched)
    if (!added && s->must_specialize && 
        is_reference_type(SYMBOL(s->must_specialize)))
    {
      add_to_universal_lookup_cache(name, fun);
      added = 1;
    }
  }
  // record default argument positions
  if (SYMBOL(s)) {
    ArgSymbol *symbol = dynamic_cast<ArgSymbol*>(SYMBOL(s));
    if (symbol && symbol->defaultExpr) {
      assert(symbol->defaultExpr->ainfo);
      fun->default_args.put(cannonicalize_mposition(p), symbol->defaultExpr->ainfo);
    }
  }
  return added;
}

static void 
finalize_function(Fun *fun, int instantiation) {
  int added = 0;
  char *name = fun->sym->has.v[0]->name;
  assert(name);
  if (!strcmp("_chpl_alloc", name))
    fun->is_external = 1;
  FnSymbol *fs = dynamic_cast<FnSymbol*>(SYMBOL(fun->sym));
  if (fs->hasVarArgs)
    fun->is_varargs = 1;
  if (fs->noParens)
    fun->is_eager = 1;
  else
    if (fs->isMethod && !is_this_fun(fs) && !is_assign_this_fun(fs))
      fun->is_lazy = 1;
  if (fs->isMethod)
    fs->_this->asymbol->sym->is_this = 1;
  // add to dispatch cache
  if (!instantiation) {
    if (fs->_this) {
      if (is_reference_type(SYMBOL(fs->_this->type))) {
        if (fs->isMethod) {
          add_to_universal_lookup_cache(name, fun);
          added = 1;
        }
      }
    }
  }
  MPosition p;
  p.push(1);
  forv_Sym(s, fun->sym->has) {
    added = handle_argument(s, name, fun, added, p, instantiation);
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
  if (f->hasPragma("split eager"))
    fun->split_eager = 1;
}

void
ACallbacks::finalize_functions() {
  pdb->fa->array_index_base = 1;
  pdb->fa->tuple_index_base = 1;
  forv_Fun(fun, pdb->funs)
    finalize_function(fun, 0);
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
  cast_symbol = make_symbol("chapel_cast");
}

static void
type_equal_transfer_function(PNode *pn, EntrySet *es) {
  AVar *result = make_AVar(pn->lvals.v[0], es);
  //AVar *type = make_AVar(pn->rvals.v[2], es);
  //AVar *val = make_AVar(pn->rvals.v[3], es);
  update_gen(result, make_abstract_type(sym_bool));
}

static void
cast_value_transfer_function(PNode *pn, EntrySet *es) {
  assert(pn->rvals.n == 4);
  AVar *result = make_AVar(pn->lvals.v[0], es);
  AVar *type = make_AVar(pn->rvals.v[2], es);
  AVar *val = make_AVar(pn->rvals.v[3], es);
  fill_tvals(es->fun, pn, 2);

  AVar *type_tmp = make_AVar(pn->tvals.v[0], es);
  Vec<CreationSet *> type_css;
  forv_CreationSet(cs, type->out->sorted) {
    Sym *ts = cs->sym;
    Sym *type = ts;
    if (type->is_meta_type)
      type = base_type(ts->meta_type);
    if (is_scalar_type(SYMBOL(type->type))) {
      AType *btype = make_abstract_type(type);
      CreationSet *bcs = btype->v[0];
      update_in(result, btype);
      type_css.add(bcs);
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
array_init_transfer_function(PNode *pn, EntrySet *es) {
  AVar *result = make_AVar(pn->lvals.v[0], es);
  AVar *array = make_AVar(pn->rvals.v[2], es);
  AVar *val = make_AVar(pn->rvals.v[4], es);
  forv_CreationSet(a, array->out->sorted) {
    if (a->sym->element)
      flow_vars(val, get_element_avar(a));
  }
  flow_vars(array, result);
}

static void
array_index_transfer_function(PNode *pn, EntrySet *es) {
  AVar *result = make_AVar(pn->lvals.v[0], es);
  AVar *array = make_AVar(pn->rvals.v[2], es);
  set_container(result, array);
  forv_CreationSet(a, array->out->sorted) {
    if (a->sym->element)
      flow_vars(get_element_avar(a), result);
  }
}

static void
array_set_transfer_function(PNode *pn, EntrySet *es) {
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
return_bool_transfer_function(PNode *pn, EntrySet *es) {
  AVar *result = make_AVar(pn->lvals.v[0], es);
  update_gen(result, make_abstract_type(sym_bool));
}

static void
return_int_transfer_function(PNode *pn, EntrySet *es) {
  AVar *result = make_AVar(pn->lvals.v[0], es);
  update_gen(result, make_abstract_type(sym_int));
}

static void
return_float_transfer_function(PNode *pn, EntrySet *es) {
  AVar *result = make_AVar(pn->lvals.v[0], es);
  update_gen(result, make_abstract_type(sym_float));
}

static void
return_string_transfer_function(PNode *pn, EntrySet *es) {
  AVar *result = make_AVar(pn->lvals.v[0], es);
  update_gen(result, make_abstract_type(sym_string));
}

static void
array_pointwise_op(PNode *pn, EntrySet *es) {
  AVar *result = make_AVar(pn->lvals.v[0], es);
  AVar *array = make_AVar(pn->rvals.v[2], es);
  flow_vars(array, result);
}

static void
unimplemented_transfer_function(PNode *pn, EntrySet *es) {
  fail("unimplemented primitive");
}

static void
alloc_transfer_function(PNode *pn, EntrySet *es) {
  AVar *tav = make_AVar(pn->rvals.v[2], es);
  AVar *result = make_AVar(pn->lvals.v[0], es);
  forv_CreationSet(cs, tav->out->sorted) {
    Sym *ts = cs->sym;
    if (ts->is_meta_type) ts = ts->meta_type;
    if (ts->asymbol && is_scalar_type(SYMBOL(ts)))
      ; // update_gen(result, make_abstract_type(ts))  not permitted
    else
      creation_point(result, ts);
  }
}

int 
ast_to_if1(Vec<BaseAST*>& syms) {
  qsort(syms.v, syms.n, sizeof(syms.v[0]), compar_baseast);
  init_symbols();
  map_asts(syms);
  build_builtin_symbols();
  Vec<Type *> types;
  build_types(syms, &types);
  build_symbols(syms);
  if1_set_primitive_types(if1);
  forv_Type(t, types)
    if (t->defaultValue)
      get_defaultVal(t);
  build_classes(syms);
  finalize_types(if1, false);
  if (build_functions(syms) < 0) return -1;
  finalize_symbols(if1);
  build_type_hierarchy();
  finalize_types(if1, false);  // again to catch any new ones
  return 0;
}

int
AST_to_IF1(Vec<BaseAST*>& syms) {
  if (ast_to_if1(syms) < 0)
    fail("unable to analyze AST");
  return 0;
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
  if (!type)
    return dtUnknown;
  if (type == sym_void || type == sym_void->meta_type)
    return dtVoid;
  if (type == sym_symbol)
    return dtSymbol;
  if (type->type_kind == Type_LUB) {
    forv_Sym(s, type->has)
      if (s == sym_void || s == sym_void->meta_type)
        return dtVoid;
  }
  if (type->is_meta_type) {
    if (type->meta_type->asymbol)
      return ((Type*)(SYMBOL(type->meta_type)))->metaType;
    else
      return dtUnknown;
  }
  if (type->type_kind == Type_LUB) {
    int found_nil = 0;
    Vec<Sym *> remains;
    forv_Sym(s, type->has) {
      if (s == sym_nil_type) {
        found_nil = 1;
        continue;
      }
      remains.add(s);
    }
    if (found_nil) {
      if (remains.n) {
        if (remains.n == 1)
          return dynamic_cast<Type*>(SYMBOL(remains.v[0]));
        else {
          Vec<Type *> types;
          type = new_sym();
          type->type_kind = Type_LUB;
          type->has.move(remains);
          forv_Sym(s, type->has) {
            if (Type *tt = dynamic_cast<Type*>(SYMBOL(s)))
              types.add(tt);
            else
              INT_FATAL("non-Type member of Sum type");
          }
          type = find_or_make_lub_type(&types, type);
        }
      } else
        return dtNil;
    }
  }
  if (type->is_fun) // HACK until the AST supports true function tyeps
    return new_SymbolSymbol(type->name)->type;
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

int
call_info(Expr *a, Vec<FnSymbol *> &fns, 
          Vec<Vec<Vec<Type *> *> *> *dispatch, Vec<Vec<int> *> *visibility_points)
{
  FnSymbol* f = a->getFunction();
  fns.clear();
  if (!f) // this is not executable code
    return -1;
  Fun *fun = f->asymbol->sym->fun;
  IFAAST *ast = a->ainfo;
  if (!ast)
    return -1; // this code is not known to analysis
  Vec<Fun *> ff, allff;
  forv_PNode(pn, ast->pnodes) {
    if (pn->code->kind != Code_SEND) 
      continue;
    forv_EntrySet(es, fun->ess) if (es) {
      Vec<AEdge *> *edges = es->out_edge_map.get(pn);
      if (edges) {
        forv_AEdge(e, *edges) {
          allff.set_add(e->fun);
          form_MPositionAVar(x, e->filtered_args) {
            forv_CreationSet(cs, x->value->out->sorted) {
              if (cs->sym != sym_nil_type)
                goto Lok;
            }
            goto Lskip;
          Lok:;
          }
          ff.set_add(e->fun);
        Lskip:;
        }
      }
    }
  }
  if (!ff.n)
    ff.move(allff);
  ff.set_to_vec();
  qsort_by_id(ff);
  if (dispatch) {
    dispatch->fill(ff.n);
    for (int i = 0; i < ff.n; i++)
      dispatch->v[i] = new Vec<Vec<Type *> *>;
  }
  if (visibility_points) {
    visibility_points->fill(ff.n);
    for (int i = 0; i < ff.n; i++)
      visibility_points->v[i] = new Vec<int>;
  }
  for (int i = 0; i < ff.n; i++) {
    Fun *f = ff.v[i];
    FnSymbol *fs = dynamic_cast<FnSymbol *>(SYMBOL(f->sym));
    fns.add(fs);
    if (dispatch) {
      forv_PNode(pn, ast->pnodes) {
        if (pn->code->kind != Code_SEND) 
          continue;
        forv_EntrySet(es, fun->ess) if (es) {
          Vec<AEdge *> *edges = es->out_edge_map.get(pn);
          if (edges) {
            forv_AEdge(e, *edges) {
              if (e->fun == f) {
                dispatch->v[i]->fill(f->positional_arg_positions.n);
                for (int j = 0; j < f->positional_arg_positions.n; j++) {
                  MPosition *x = f->positional_arg_positions.v[j];
                  AType *edge_filter = e->match->formal_filters.get(x);
                  AType *es_filter = e->to->filters.get(x);
                  AType *filter = es_filter ? type_intersection(edge_filter, es_filter) : edge_filter;
                  if (!dispatch->v[i]->v[j])
                    dispatch->v[i]->v[j] = new Vec<Type *>;
                  forv_CreationSet(cs, filter->sorted)
                    dispatch->v[i]->v[j]->set_add(to_AST_type(cs->type));
                }
                forv_PNode(p, e->match->visibility_points) if (p)    
                  visibility_points->v[i]->set_add(p->call_context);
              }
            }
          }
        }
      }
    }
  }
  return 0;
}

int
context_info(Expr *e) {
  IFAAST *ast = e->ainfo;
  if (!ast)
    return -1; // this code is not known to analysis
  forv_PNode(pn, ast->pnodes) {
    if (pn->code->kind != Code_SEND) 
      continue;
    return pn->call_context;
  }
  return -1;
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
    if (fn->asymbol && fn->asymbol->sym->fun)
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
    assert(!t->asymbol);
    return t->definition->asymbol->sym->creators.n != 0;
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
member_info(Sym *t, char *name, int *offset, Type **type, int index = -1) {
  int result = 0;
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
      AVar *iv = name ? cs->var_map.get(name) : 0;
      if (index >= cs->vars.n)
        return -1;
      if (!iv && index >= 0)
        iv = cs->vars.v[index];
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
  if (types->n < 2)
    result = 0;
  if (!iv_type.n)
    *type = NULL;
  else {
    if (iv_type.n == 1)
      tmp = iv_type.v[0];
    else 
      tmp = concrete_type_set_to_type(iv_type);
    *type = to_AST_type(tmp);
  }
  return result;
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
resolve_member(ClassType *t, int index, int *offset, Type **type) {
  return member_info(t->asymbol->sym, NULL, offset, type, index);
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

static AnalysisOp *
S(char *name, PrimitiveTransferFunctionPtr pfn) {
  char internal_name[512];
  strcpy(internal_name, "chapel_");
  strcat(internal_name, name);
  char *new_name = if1_cannonicalize_string(if1, internal_name);
  pdb->fa->primitive_transfer_functions.put(new_name, new RegisteredPrim(pfn));
  return new AnalysisOp(name, new_name, pfn);
}

static AnalysisOp *
P(char *name, Prim *p) {
  char internal_name[512];
  strcpy(internal_name, "chapel_");
  strcat(internal_name, name);
  char *new_name = if1_cannonicalize_string(if1, internal_name);
  return new AnalysisOp(name, new_name, p);
}

void
init_chapel_ifa() {
  ifa_init(new ACallbacks);
  unimplemented_analysis_op = S("unimplemented", unimplemented_transfer_function);
  return_bool_analysis_op = S("return_bool", return_bool_transfer_function);
  return_int_analysis_op = S("return_int", return_int_transfer_function); 
  return_float_analysis_op = S("return_float", return_float_transfer_function);
  return_string_analysis_op = S("return_string", return_string_transfer_function); 
  array_init_analysis_op = S("array_init", array_init_transfer_function);
  array_index_analysis_op = S("array_index", array_index_transfer_function);
  array_set_analysis_op = S("array_set", array_set_transfer_function);
  array_pointwise_op_analysis_op = S("array_pointwise_op", array_pointwise_op);
  unary_minus_analysis_op = P("u-", prim_minus);
  unary_plus_analysis_op = P("u+", prim_plus);
  unary_not_analysis_op = P("u~", prim_not);
  unary_lnot_analysis_op = P("!", prim_lnot);
  add_analysis_op = P("+", prim_add);
  subtract_analysis_op = P("-", prim_subtract);
  mult_analysis_op = P("*", prim_mult);
  div_analysis_op = P("/", prim_div);
  mod_analysis_op = P("%", prim_mod);
  lsh_analysis_op = P("<<", prim_lsh);
  rsh_analysis_op = P(">>", prim_rsh);
  equal_analysis_op = P("==", prim_equal);
  notequal_analysis_op = P("!=", prim_notequal);
  lessorequal_analysis_op = P("<=", prim_lessorequal);
  greaterorequal_analysis_op = P(">=", prim_greaterorequal);
  less_analysis_op = P("<", prim_less);
  greater_analysis_op = P(">", prim_greater);
  and_analysis_op = P("&", prim_and);
  or_analysis_op = P("|", prim_or);
  xor_analysis_op = P("^", prim_xor);
  land_analysis_op = P("&&", prim_land);
  lor_analysis_op = P("||", prim_lor);
  pow_analysis_op = P("**", prim_pow);
  get_member_analysis_op = P(".", prim_period);
  set_member_analysis_op = P(".=", prim_setter);
  cast_analysis_op = S("cast", cast_value_transfer_function);
  type_equal_analysis_op = S("type_equal", type_equal_transfer_function);
  alloc_analysis_op = S("alloc", alloc_transfer_function);
}

