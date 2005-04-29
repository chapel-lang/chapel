#include <typeinfo>
#include "geysa.h"
#include "analysis.h"
#include "expr.h"
#include "files.h"
#include "../traversals/getstuff.h"
#include "link.h"
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

#define VARARG_END     0ll

//#define MINIMIZED_MEMORY 1  // minimize the memory used by Sym's... needs valgrind checking of Boehm GC for safety

class LabelMap : public Map<char *, Stmt *> {};

static Sym *domain_start_index_symbol = 0;
static Sym *domain_next_index_symbol = 0;
static Sym *domain_valid_index_symbol = 0;
static Sym *expr_simple_seq_symbol = 0;
static Sym *expr_domain_symbol = 0;
static Sym *expr_create_domain_symbol = 0;
static Sym *expr_reduce_symbol = 0;
static Sym *write_symbol = 0;
static Sym *writeln_symbol = 0;
static Sym *read_symbol = 0;
static Sym *flood_symbol = 0;
static Sym *completedim_symbol = 0;
static Sym *array_index_symbol = 0;
static Sym *array_set_symbol = 0;
static Sym *sizeof_symbol = 0;
static Sym *cast_symbol = 0;
static Sym *method_symbol = 0;
static Sym *make_seq_symbol = 0;
static Sym *make_chapel_tuple_symbol = 0;

static Sym *sym_index = 0;
static Sym *sym_domain = 0;
static Sym *sym_array = 0;
static Sym *sym_sequence = 0;
static Sym *sym_locale = 0;

static int init_function(FnSymbol *f);
static int build_function(FnSymbol *f);
static void map_asts(Vec<BaseAST *> &syms);
static void build_symbols(Vec<BaseAST *> &syms);
static void build_types(Vec<BaseAST *> &syms);
static int gen_if1(BaseAST *ast, BaseAST *parent = 0);

class ScopeLookupCache : public Map<char *, Vec<Fun *> *> {};
static ScopeLookupCache universal_lookup_cache;
static int finalized_symbols = 0;


ASymbol::ASymbol() : symbol(0), sym(0) {
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

AInfo::AInfo() : xast(0), code(0), sym(0), rval(0) {
  label[0] = label[1] = 0;
}

char
*AInfo::pathname() { 
  return xast->filename;
}

int
AInfo::line() {
  return xast->lineno;
}

Sym *
AInfo::symbol() {
  if (rval) return rval;
  return sym;
}

AST*
AInfo::copy_node(ASTCopyContext* context) {
  AInfo *a = new AInfo(*this);
  for (int i = 0; i < a->pnodes.n; i++)
    a->pnodes.v[i] = context->nmap->get(a->pnodes.v[i]);
  return a;
}

Vec<Fun *> *
AInfo::visible_functions(Sym *arg0) {
  if (arg0->fun) {
    if (!arg0->fun->vec_of_one) {
      arg0->fun->vec_of_one = new Vec<Fun *>;
      arg0->fun->vec_of_one->add(arg0->fun);
    }
    return arg0->fun->vec_of_one;
  }
  Vec<Fun *> *v = 0;
  char *name = 0;
  if (arg0->is_symbol)
    name = arg0->name;
  else
    name = if1_cannonicalize_string(if1, "this");
  Expr *e = dynamic_cast<Expr *>(this->xast);
  Stmt *s = 0;
  if (e)
    s = e->getStmt();
  else
    s = dynamic_cast<Stmt *>(this->xast);
  ScopeLookupCache *sym_cache = 0;
  Symbol *symbol = 0;
  SymScope* scope = 0;
  if (ModuleSymbol* mod = dynamic_cast<ModuleSymbol*>(s->parentSymbol))
    scope = mod->modScope;
  else if (FnSymbol* fn = dynamic_cast<FnSymbol*>(s->parentSymbol))
    scope = fn->paramScope;
  else { INT_FATAL(s, "Unexpected case"); }
  sym_cache = scope->lookupCache;
  if (sym_cache && (v = sym_cache->get(name))) 
    return v;
  Vec<FnSymbol *> *fss = scope->visibleFunctions.get(name);
  v = new Vec<Fun *>;
  if (fss)
    forv_Vec(FnSymbol, x, *fss)
      v->set_add(x->asymbol->sym->fun);
  Vec<Fun *> *universal = universal_lookup_cache.get(name);
  if (universal)
    v->set_union(*universal);
  if (symbol && !sym_cache)
    sym_cache = scope->lookupCache = new ScopeLookupCache;
  if (sym_cache)
    sym_cache->put(name, v);
  return v;
}

void
AnalysisCloneCallback::clone(BaseAST* old_ast, BaseAST* new_ast) {
  if (isSomeStmt(new_ast->astType)) {
    Stmt *new_s = dynamic_cast<Stmt*>(new_ast);
    Stmt *old_s = dynamic_cast<Stmt*>(old_ast);
    if (old_s->ainfo) {
      new_s->ainfo = (AInfo*)old_s->ainfo->copy_node(context);
      new_s->ainfo->xast = new_s;
    }
  } else if (isSomeExpr(new_ast->astType)) {
    Expr *new_e = dynamic_cast<Expr*>(new_ast);
    Expr *old_e = dynamic_cast<Expr*>(old_ast);
    if (old_e->ainfo) {
      new_e->ainfo = (AInfo*)old_e->ainfo->copy_node(context);
      new_e->ainfo->xast = new_e;
    }
  } else if (isSomeSymbol(new_ast->astType)) {
    Symbol *new_s = dynamic_cast<Symbol*>(new_ast);
    Symbol *old_s = dynamic_cast<Symbol*>(old_ast);
    if (old_s->asymbol) {
      new_s->asymbol = old_s->asymbol->copy();
      new_s->asymbol->symbol = new_s;
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
    Type *new_s = dynamic_cast<Type*>(new_ast);
    Type *old_s = dynamic_cast<Type*>(old_ast);
    if (old_s->asymbol) {
      new_s->asymbol = (ASymbol*)old_s->asymbol->copy();
      new_s->asymbol->symbol = new_s;
      context->smap.put(old_s->asymbol->sym, new_s->asymbol->sym);
    }
  } else
    assert(!"clone of Type unsupported");
}

AST *
AInfo::copy_tree(ASTCopyContext* context) {
  AnalysisCloneCallback callback;
  callback.context = context;
  Map<BaseAST*,BaseAST*> clone_map;
  DefExpr* def_expr = dynamic_cast<DefExpr*>(xast);
  FnSymbol* orig_fn = dynamic_cast<FnSymbol*>(def_expr->sym);
  FnSymbol *new_fn = orig_fn->clone(&callback, &clone_map);
  return new_fn->defPoint->ainfo;
}

static void
close_symbols(Vec<Stmt *> &stmts, Vec<BaseAST *> &syms) {
  Vec<BaseAST *> set;
  forv_Stmt(a, stmts)
    while (a) {
      set.set_add(a);
      syms.add(a);
      a = dynamic_cast<Stmt *>(a->next);
    }
  forv_BaseAST(s, syms) if (s) {
    GetStuff getStuff(GET_ALL);
    TRAVERSE(s, &getStuff, true);
    forv_BaseAST(ss, getStuff.asts) if (ss) {
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
    Type *ttt = dynamic_cast<Type *>(s->asymbol->symbol);
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
  tt->symbol->asymbol = t->meta_type->asymbol;
  tt->symbol->asymbol->sym = t->meta_type;
  tt->asymbol = t->asymbol;
  tt->asymbol->sym = t;
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
    if (s->is_constant || s->is_symbol)
      set_global_scope(s);
    else
      if (s->type_kind)
        set_global_scope(s);
    if (s->asymbol && s->asymbol->symbol && s->asymbol->symbol->astType == SYMBOL_VAR) {
      VarSymbol *v = (VarSymbol*)s->asymbol->symbol;
      if (v->type && v->type != dtUnknown) {
        Sym *t = v->type->asymbol->sym;
        if (t->num_kind)
          s->type = t;
      }
    }
  }
  finalized_symbols = i->allsyms.n;
}

static Fun *
install_new_function(FnSymbol *f) {
  Vec<BaseAST *> syms;
  collect_asts(&syms, f);
  syms.add(f);
  syms.add(f->defPoint);
  map_asts(syms);
  build_symbols(syms);
  build_types(syms);
  finalize_types(if1);
  if (init_function(f) < 0 || build_function(f) < 0) 
    assert(!"unable to instantiate generic/wrapper");
  if1_finalize_closure(if1, f->asymbol->sym);
  build_type_hierarchy();
  finalize_symbols(if1);
  Fun *fun = new Fun(f->asymbol->sym);
  build_arg_positions(fun);
  pdb->add(fun);
  return fun;
}

Sym *
ACallbacks::instantiate(Sym *s, Map<Sym *, Sym *> &substitutions) {
  if (s->type_kind) {
    Sym *tt = substitutions.get(s);
    if (tt) {
      Map<Type *, Type *> subs;
      form_SymSym(s, substitutions)
        subs.put(dynamic_cast<Type*>(s->key->asymbol->symbol), 
                 dynamic_cast<Type*>(s->value->asymbol->symbol));
      Type *type = dynamic_cast<Type*>(s->asymbol->symbol);
      Type *new_type = type->instantiate_generic(subs);
      assert(tt == new_type->asymbol->sym);
      return tt;
    }
  }
  return 0;
}

Fun *
ACallbacks::order_wrapper(Match *m) {
  if (!m->fun->ast) 
    return NULL;
  FnSymbol *fndef = dynamic_cast<FnSymbol *>(m->fun->sym->asymbol->symbol);
  FnSymbol *f = fndef->order_wrapper(&m->formal_to_actual_position);
  Fun *fun = install_new_function(f);
  fun->wraps = m->fun;
  return fun;
}

Fun *
ACallbacks::coercion_wrapper(Match *m) {
  if (!m->fun->ast) 
    return NULL;
#ifdef NEW_COERCION_WRAPPER
  Map<Symbol *, Symbol *> coercions;
  forv_MPosition(p, m->fun->positional_arg_positions) {
    Sym *sym = m->fun->arg_syms.get(p);
    Symbol *symbol = sym->asymbol ? dynamic_cast<Symbol*>(sym->asymbol->symbol) : 0;
    if (symbol) {
      Sym *type_sym = m->coercion_substitutions.get(p);
      if (type_sym) {
        Type *type = dynamic_cast<Type*>(type_sym->asymbol->symbol);
        Sym *a = m->fun->arg_syms.get(p);
        if (a->asymbol && a->asymbol->symbol) {
          Symbol *aa = dynamic_cast<Symbol*>(a->asymbol->symbol);
          coercions.put(aa, type->symbol);
        }
      }
    }
  }
#else
  Map<MPosition *, Symbol *> coercions;
  form_MPositionSym(s, m->coercion_substitutions) {
    Type *type = dynamic_cast<Type*>(s->value->asymbol->symbol);
    coercions.put(s->key, type->symbol);
  }
#endif
  FnSymbol *fndef = dynamic_cast<FnSymbol *>(m->fun->sym->asymbol->symbol);
  FnSymbol *f = fndef->coercion_wrapper(&coercions);
  Fun *fun = install_new_function(f);
  fun->wraps = m->fun;
  return fun;
}

Fun *
ACallbacks::default_wrapper(Match *m) {
  if (!m->fun->ast) 
    return NULL;
  FnSymbol *fndef = dynamic_cast<FnSymbol *>(m->fun->sym->asymbol->symbol);
  FnSymbol *f = fndef->default_wrapper(&m->default_args);
  Fun *fun = install_new_function(f);
  fun->wraps = m->fun;
  return fun;
}

Fun *
ACallbacks::instantiate_generic(Match *m) {
  if (!m->fun->ast) 
    return NULL;
  Map<Type *, Type *> substitutions;
  form_SymSym(s, m->generic_substitutions)
    substitutions.put(dynamic_cast<Type*>(s->key->asymbol->symbol), 
                      dynamic_cast<Type*>(s->value->asymbol->symbol));
  FnSymbol *fndef = dynamic_cast<FnSymbol *>(m->fun->sym->asymbol->symbol);
  FnSymbol *f = fndef->instantiate_generic(&substitutions);
  Fun *fun = install_new_function(f);
  fun->wraps = m->fun;
  return fun;
}

Sym *
ASymbol::clone(CloneCallback *callback) {
  AnalysisCloneCallback *c = dynamic_cast<AnalysisCloneCallback *>(callback);
  if (!symbol) { // internal to analysis
    Sym *s = copy()->sym;
    c->context->smap.put(this->sym, s);
    return s;
  } else {
    Type *type = dynamic_cast<Type*>(symbol);
    TypeSymbol *old_type_symbol = dynamic_cast<TypeSymbol*>(type->symbol);
    Map<BaseAST*,BaseAST*> clone_map;
    TypeSymbol *new_type_symbol = old_type_symbol->clone(c, &clone_map);
    assert(new_type_symbol);
    Sym *new_type = c->context->smap.get(old_type_symbol->type->asymbol->sym);
    if (!new_type_symbol->asymbol) // SHOULD BE ASSERT
      callback->clone(old_type_symbol, new_type_symbol);
    new_type->meta_type = new_type_symbol->asymbol->sym;
    new_type_symbol->asymbol->sym->meta_type = new_type;
    assert(new_type_symbol->asymbol->sym->is_meta_type);
    for (int i = 0; i < new_type->has.n; i++) {
      Sym *s = c->context->smap.get(new_type->has.v[i]);
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
    t->asymbol->symbol = t;
  }
  else {
    t->asymbol = new_ASymbol("BOGUS");
    t->asymbol->symbol = t;
  }
}

static void
map_baseast(BaseAST *s) {
  Symbol *sym = dynamic_cast<Symbol *>(s);
  if (sym) {
    if (sym->asymbol)
      return;
    int basic = (s->astType != SYMBOL_FN) && (s->astType != SYMBOL_ENUM);
    sym->asymbol = new_ASymbol(sym, basic);
    sym->asymbol->symbol = sym;
    if (sym->astType == SYMBOL_VAR && sym->type == dtNil) {
      assert(!sym_nil);
      sym_nil = sym->asymbol->sym;
    }
    if (!sym->parentScope) {
      sym->asymbol->sym->global_scope = 1;
    } else {
      switch (sym->parentScope->type) {
        default: assert(0);
        case SCOPE_INTRINSIC:
        case SCOPE_INTERNAL_PRELUDE:
        case SCOPE_PRELUDE:
        case SCOPE_MODULE:
        case SCOPE_POSTPARSE:
          sym->asymbol->sym->global_scope = 1;
          break;
        case SCOPE_LETEXPR:
        case SCOPE_PARAM:
        case SCOPE_FUNCTION:
        case SCOPE_LOCAL:
        case SCOPE_FORLOOP:
        case SCOPE_FORALLEXPR:
          sym->asymbol->sym->function_scope = 1;
          break;
        case SCOPE_CLASS: // handled as the symbols appears in code
          break;
      }
    }
    if (sym->astType == SYMBOL_PARAM) {
      ParamSymbol *s = dynamic_cast<ParamSymbol *>(sym);
      switch (s->intent) {
        default: break;
        case PARAM_IN: sym->asymbol->sym->intent = Sym_IN; break;
        case PARAM_INOUT: sym->asymbol->sym->intent = Sym_INOUT; break;
        case PARAM_OUT: sym->asymbol->sym->intent = Sym_OUT; break;
        case PARAM_CONST: sym->asymbol->sym->is_read_only = 1; break;
      }
      // handle pragmas
      Pragma *pr = sym->pragmas;
      while (pr) {
        if (!strcmp(pr->str, "clone_for_constants"))
          s->asymbol->sym->clone_for_constants = 1;
        pr = dynamic_cast<Pragma *>(pr->next);
      }
    }
    if (verbose_level > 1 && sym->name)
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
        e->ainfo = new AInfo;
        e->ainfo->xast = e;
      } else {
        Stmt *st = dynamic_cast<Stmt *>(s);
        if (st->ainfo)
          return;
        st->ainfo = new AInfo;
        st->ainfo->xast = s;
      }
    }
  }
}

static void
map_asts(Vec<BaseAST *> &syms) {
  if (verbose_level > 1)
    printf("map_asts: %d\n", syms.n);
  forv_BaseAST(s, syms)
    map_baseast(s);
}

static void 
build_record_type(Type *t, Sym *parent = 0) {
  t->asymbol->sym->type_kind = Type_RECORD;
  if (parent)
    t->asymbol->sym->inherits_add(parent);
}

static void
build_enum_element(Sym *enum_sym, Sym *element_sym, int i) {
  element_sym->inherits_add(enum_sym);
  element_sym->type = enum_sym;
  element_sym->meta_type = element_sym;
  element_sym->imm.v_int64 = i;
  element_sym->is_constant = 1;
}

static int
is_reference_type(BaseAST *t) {
  return (t && (t->astType == TYPE_NIL || t->astType == TYPE_CLASS));
}

static int
is_scalar_type(BaseAST *t) {
  return t != dtUnknown && (t->astType == TYPE_BUILTIN || t->astType == TYPE_ENUM);
}

static inline int
scalar_or_reference(Type *t) {
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
          if (t->type->astType == TYPE_VARIABLE)
            t->asymbol->sym->must_specialize = sym_anyclass;
          break;
        }
        case SYMBOL_PARAM: {
          if (s->type && s->type != dtUnknown) {
            if (s->asymbol->sym->intent != Sym_OUT)
              s->asymbol->sym->must_implement_and_specialize(s->type->asymbol->sym);
            else
              s->asymbol->sym->must_implement = s->type->asymbol->sym;
          }
          break;
        }
        default: break;
      }
    }
  }
}

static Sym *
build_type(Type *t) {
  if (t->symbol) {
    t->asymbol->sym->meta_type = t->symbol->asymbol->sym;
    t->symbol->asymbol->sym->meta_type = t->asymbol->sym;
  }
  make_meta_type(t->asymbol->sym);
  switch (t->astType) {
    default: assert(!"case");
    case TYPE:
      t->asymbol->sym->type_kind = Type_UNKNOWN;
      break;
    case TYPE_BUILTIN: break;
    case TYPE_ENUM: {
      t->asymbol->sym->type_kind = Type_TAGGED;
      t->asymbol->sym->inherits_add(sym_enum_element);
      GetSymbols* getSymbols = new GetSymbols();
      TRAVERSE_LS(t, getSymbols, true);
      for (int i = 0; i < getSymbols->symbols.n; i++) {
        BaseAST *s = getSymbols->symbols.v[i];
        Sym *ss = dynamic_cast<Symbol*>(s)->asymbol->sym;
        build_enum_element(t->asymbol->sym, ss, i);
        t->asymbol->sym->has.add(ss);
      }
      break;
    }
    case TYPE_DOMAIN: 
      build_record_type(t, sym_domain);
      break;
    case TYPE_INDEX: {
      IndexType *it = dynamic_cast<IndexType*>(t);
      build_record_type(t, sym_index);
      Sym *s = it->asymbol->sym;
      s->element = new_sym();
      s->element->type = it->idxType->asymbol->sym;
      s->element->is_var = 1;
      s->element->is_external = 1;
      if (it->domainType)
        s->domain = it->domainType->asymbol->sym;
      break;
    }
    case TYPE_ARRAY: {
      ArrayType *at = dynamic_cast<ArrayType*>(t);
      build_record_type(t, sym_array); 
      Sym *s = at->asymbol->sym;
      s->element = new_sym();
      s->element->type = at->elementType->asymbol->sym;
      s->element->is_var = 1;
      s->element->is_external = 1;
      s->domain = at->domainType->asymbol->sym;
      break;
    }
    case TYPE_TUPLE: {
#if 0
      TupleType *tt = dynamic_cast<TupleType*>(t);
      forv_Vec(Type, c, tt->components) {
        Sym *x = new_sym();
        x->ast = c->asymbol->sym->ast;
        x->type = c->asymbol->sym;
        t->asymbol->sym->has.add(x);
      }
#endif
      t->asymbol->sym->type_kind = Type_PRODUCT;
      t->asymbol->sym->inherits_add(sym_tuple);
      break;
    }
    case TYPE_USER: {
      UserType *tt = dynamic_cast<UserType*>(t);
      t->asymbol->sym->type_kind = Type_ALIAS;
      t->asymbol->sym->alias = tt->definition->asymbol->sym;
      break;
    }
    case TYPE_LIKE: {
      LikeType *tt = dynamic_cast<LikeType*>(t);
      if (tt->expr->astType == EXPR_VARIABLE) {
        Variable *v = (Variable*)tt->expr;
        if (v->var->type && v->var->type != dtUnknown) {
          t->asymbol->sym->type_kind = Type_ALIAS;
          t->asymbol->sym->alias = v->var->type->asymbol->sym;
          break;
        }
      }
      INT_FATAL(t, "No analysis support for 'like'");
    }
    case TYPE_SEQ: {
      SeqType *tt = dynamic_cast<SeqType*>(t);
      Sym *s = tt->asymbol->sym;
      s->element = new_sym();
      build_record_type(t, sym_sequence);
      break;
    }
    case TYPE_CLASS:
    case TYPE_RECORD:
    case TYPE_UNION: 
    {
      StructuralType *tt = dynamic_cast<StructuralType*>(t);
      t->asymbol->sym->type_kind = Type_RECORD;
      if (t->astType == TYPE_RECORD || t->astType == TYPE_UNION)
        t->asymbol->sym->is_value_class = 1;
      if (t->astType == TYPE_UNION)
        t->asymbol->sym->is_union_class = 1;
      if (tt->parentStruct)
        t->asymbol->sym->inherits_add(tt->parentStruct->asymbol->sym);
      else
        t->asymbol->sym->inherits_add(sym_object);
      if (t->asymbol->sym == sym_sequence)
        t->asymbol->sym->element = new_sym();
      break;
    }
    case TYPE_VARIABLE: {
      VariableType *tt = dynamic_cast<VariableType*>(t);
      tt->asymbol->sym->type_kind = Type_VARIABLE;
      break;
    }
    case TYPE_NIL: {
      break;
    }
  }
  return t->asymbol->sym;
}

static void
build_types(Vec<BaseAST *> &syms) {
  Vec<Type *> types;
  forv_BaseAST(s, syms) {
    Type *t = dynamic_cast<Type *>(s);
    if (t) 
      types.add(t);
  }
  forv_Type(t, types)
    build_type(t);
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
  (*sym)->asymbol->symbol = dt;
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
  new_lub_type(sym_anynum, "anynum", sym_bool, sym_anyint, sym_anyfloat, sym_anycomplex, VARARG_END);
  new_primitive_type(sym_char, "char");
  new_primitive_type(sym_string, "string");
  if (!sym_new_object) {
    sym_new_object = new_sym("new_object", 1);
    if1_set_builtin(if1, sym_new_object, "new_object");
  }

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

  sym_complex128->specializes.add(sym_string);

#define S(_n) assert(sym_##_n);
#include "builtin_symbols.h"
#undef S
}

static int
import_symbols(Vec<BaseAST *> &syms) {
  map_asts(syms);
  build_builtin_symbols();
  build_symbols(syms);
  build_types(syms);
  return 0;
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
      labelmap->put(if1_cannonicalize_string(if1, dynamic_cast<LabelStmt*>(stmt)->label->name), target);
      break;
    }
    case STMT_WHILELOOP:
    case STMT_FORLOOP:
      stmt->ainfo->label[0] = if1_alloc_label(if1);
      stmt->ainfo->label[1] = if1_alloc_label(if1);
      break;
    default: break;
  }
  GetStmts* getStmts = new GetStmts();
  TRAVERSE_LS(ast, getStmts, true);
  forv_BaseAST(a, getStmts->stmts)
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
  GetStmts* getStmts = new GetStmts();
  TRAVERSE_LS(ast, getStmts, true);
  forv_BaseAST(a, getStmts->stmts)
    if (resolve_labels(a, labelmap, return_label, break_label, continue_label) < 0)
      return -1;
  return 0;
}

static Sym *
gen_set_array(Sym *array, ArrayType *at, Sym *val, AInfo *ast) {
  // currently just a hack to set the first element
  Code **c = &ast->code;
  Sym *index_sym = new_sym();
  Code *start_index = if1_send(if1, c, 3, 1, sym_primitive, domain_start_index_symbol, 
                               at->domainType->asymbol->sym, index_sym);
  start_index->ast = ast;
  Sym *res = new_sym();
  Code *set_element = if1_send(if1, c, 5, 1, sym_primitive, array_set_symbol, 
                                  array, index_sym, val, res);
  set_element->ast = ast;
  return res;
}

static Sym *
get_defaultVal(Type *t) {
  Variable *v = dynamic_cast<Variable*>(t->defaultVal);
  if (v)
    return v->var->asymbol->sym;
  assert(dynamic_cast<Literal*>(t->defaultVal));
  if (!t->defaultVal->ainfo->sym)
    gen_if1(t->defaultVal);
  return t->defaultVal->ainfo->rval;
}

static Sym *
constructor_name(Type *t) {
  return if1_make_symbol(if1, t->defaultConstructor->asymbol->sym->name);
}

static void
gen_alloc(Sym *s, Sym *type, AInfo *ast, int is_this = 0) {
  Code **c = &ast->code;
  Code *send = 0;
  StructuralType *ct = dynamic_cast<StructuralType*>(type->asymbol->symbol);
  if (ct && (ct->astType == TYPE_RECORD || ct->astType == TYPE_UNION) && !is_this)
    send = if1_send(if1, c, 1, 1, constructor_name(ct), s);
  if (!send) {
    Sym *tmp = new_sym();
    send = if1_send(if1, c, 2, 1, sym_new, type, tmp);
    if1_move(if1, c, tmp, s, ast);
  }
  send->ast = ast;
  if (type->asymbol->symbol->astType == TYPE_ARRAY) {
    ArrayType *at = dynamic_cast<ArrayType*>(type->asymbol->symbol);
    Sym *ret = new_sym();
    if (at->elementType->astType == TYPE_ARRAY) {
      gen_alloc(ret, at->elementType->asymbol->sym, ast);
    } else if (at->elementType->defaultVal) {
      if1_move(if1, c, get_defaultVal(at->elementType), ret, ast);
    } else if (at->elementType->defaultConstructor) {
      Code *send = if1_send(if1, c, 1, 1, constructor_name(at->elementType), ret);
      send->ast = ast;
    } else
      ret = sym_nil;;
    gen_set_array(s, at, ret, ast);
  }
  send->ast = ast;
}

static Sym *
gen_coerce(Sym *s, Sym *type, Code **c, AST *ast) {
  Sym *ret = new_sym();
  Code *send = if1_send(if1, c, 3, 1, sym_coerce, type, s, ret);
  send->ast = ast;
  return ret;
}

static int
gen_one_vardef(VarSymbol *var, DefExpr *def) {
  Type *type = var->type;
  Sym *s = var->asymbol->sym;
  AInfo *ast = def->ainfo;
  ast->sym = s;
  switch (var->varClass) {
    case VAR_NORMAL: break;
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
    if (!is_reference_type(type)) {
      s->type = unalias_type(type->asymbol->sym);
      s->is_var = 1;
    } else
      s->must_implement = unalias_type(type->asymbol->sym);
  }
  FnSymbol *f = def->parentStmt->parentFunction();
  Expr *init = def->init;
  int this_constructor = f && f->isConstructor && var->isThis();
  int this_is_init = type->defaultVal &&
    dynamic_cast<Variable*>(type->defaultVal) &&
    dynamic_cast<Variable*>(type->defaultVal)->var == var;
  if (s->is_var && !scalar_or_reference(type)) {
    switch (type->astType) { 
        // ruled out by conditionals above
      case TYPE_ENUM:
      case TYPE_BUILTIN: 
      case TYPE_CLASS:
      case TYPE_NIL:
        // do not make it to analysis
      case TYPE_LIKE:
      case TYPE_UNRESOLVED:
      case TYPE_VARIABLE:
      case TYPE_SUM:
      case TYPE_STRUCTURAL:
      default:
        assert(!"impossible");
        goto Lstandard; 
      case TYPE_SEQ:
      case TYPE_USER:
      case TYPE_TUPLE: 
        goto Lstandard;
      case TYPE_DOMAIN:
      case TYPE_INDEX:
      {
        Sym *tmp = new_sym();
        Code *send = if1_send(if1, &ast->code, 2, 1, sym_new, type->asymbol->sym, tmp);
        if1_move(if1, &ast->code, tmp, s, ast);
        send->ast = ast;
        break;
      }
      case TYPE_RECORD:
      case TYPE_UNION:
      {
        int is_this = f && f->_this == var;
        if (!is_this)
          goto Lstandard;
        Sym *tmp = new_sym();
        Code *send = if1_send(if1, &ast->code, 2, 1, sym_new, type->asymbol->sym, tmp);
        if1_move(if1, &ast->code, tmp, s, ast);
        send->ast = ast;
        break;
      }
      case TYPE_ARRAY:
        gen_alloc(s, s->type, ast, f && f->_this == var);
        break;
    }
  } else if (!init) {
    // THIS IS THE STANDARD CODE
  Lstandard:
    if (!this_is_init && var->type->defaultVal) {
      if1_move(if1, &ast->code, get_defaultVal(var->type), ast->sym, ast);
    } else if (var->type->defaultConstructor) {
      Code *send = if1_send(if1, &ast->code, 1, 1, constructor_name(var->type), ast->sym);
      send->ast = ast;
    } else if (!this_constructor)
      if1_move(if1, &ast->code, sym_nil, ast->sym, ast);
  }
  if (init) {
    if (type->astType == TYPE_ARRAY) {
      ArrayType *at = dynamic_cast<ArrayType*>(type->asymbol->symbol);
      if1_gen(if1, &ast->code, init->ainfo->code);
      gen_set_array(s, at, init->ainfo->rval, ast);
    } else {
      // THIS IS THE STANDARD CODE
      Sym *val = init->ainfo->rval;
      if (is_scalar_type(type)) {
        if1_gen(if1, &ast->code, init->ainfo->code);
        if (type != init->typeInfo())
          val = gen_coerce(val, s->type, &ast->code, ast);
        if1_move(if1, &ast->code, val, ast->sym, ast);
      } else if (!is_reference_type(type) && type != dtUnknown) {
        Sym *old_val = val;
        val = new_sym();
        Code *c = if1_send(if1, &init->ainfo->code, 3, 1, if1_make_symbol(if1, "="), 
                           ast->sym, old_val, val);
        
        c->ast = init->ainfo;
        if1_gen(if1, &ast->code, init->ainfo->code);
      } else {
        if1_gen(if1, &ast->code, init->ainfo->code);
        if1_move(if1, &ast->code, val, ast->sym, ast);
      }
    }
  }
  return 0;
}

static int
gen_vardef(BaseAST *a) {
  DefStmt *def = dynamic_cast<DefStmt*>(a);
  for (DefExpr* def_expr = def->defExprls; def_expr; def_expr = nextLink(DefExpr, def_expr)) {
    for (VarSymbol *var = dynamic_cast<VarSymbol*>(def_expr->sym); var;
         var = dynamic_cast<VarSymbol*>(var->next))
      if (gen_one_vardef(var, def_expr))
        return -1;
    if1_gen(if1, &def->ainfo->code, def_expr->ainfo->code);
  }
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
  Vec<Stmt*> body;
  getLinkElements(body, s->body);
  forv_Stmt(ss, body)
    if1_gen(if1, &body_code, ss->ainfo->code);
  if1_loop(if1, &s->ainfo->code, s->ainfo->label[0], s->ainfo->label[1],
           s->condition->ainfo->rval, 0, 
           s->condition->ainfo->code, 0, 
           body_code, s->ainfo);
  return 0;
}

static int
gen_forall_internal(AInfo *ainfo, Code *body, Vec<Symbol*> &indices, Vec<Expr*> &domains) {
  // setup code: evaluate domains and get starting indices
  Code *setup_code = 0, *send;
  forv_Expr(d, domains)
    if1_gen(if1, &setup_code, d->ainfo->code);
  send = if1_send(if1, &setup_code, 2, 0, sym_primitive, domain_start_index_symbol);
  forv_Expr(d, domains)
    if1_add_send_arg(if1, send, d->ainfo->rval);
  forv_Symbol(i, indices)
    if1_add_send_result(if1, send, i->asymbol->sym);
  send->ast = ainfo;

  // loop condition code
  Code *condition_code = 0;
  Sym *condition_rval = new_sym();
  send = if1_send(if1, &condition_code, 2, 1, sym_primitive, domain_valid_index_symbol,
                  condition_rval);
  forv_Expr(d, domains)
    if1_add_send_arg(if1, send, d->ainfo->rval);
  forv_Symbol( i, indices)
    if1_add_send_arg(if1, send, i->asymbol->sym);
  send->ast = ainfo;

  // next index code
  send = if1_send(if1, &body, 2, 0, sym_primitive, domain_next_index_symbol);
  forv_Expr(d, domains)
    if1_add_send_arg(if1, send, d->ainfo->rval);
  forv_Symbol( i, indices)
    if1_add_send_arg(if1, send, i->asymbol->sym);
  forv_Symbol( i, indices)
    if1_add_send_result(if1, send, i->asymbol->sym);
  send->ast = ainfo;

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
  Vec<Stmt*> body_stmts;
  getLinkElements(body_stmts, s->body);
  forv_Stmt(ss, body_stmts)
    if1_gen(if1, &body, ss->ainfo->code);
  Vec<Symbol*> indices;
  Vec<DefExpr*> indexDefs;
  DefExpr* index_def = dynamic_cast<DefExpr*>(s->indices);
  getLinkElements(indexDefs, index_def);
  forv_Vec(DefExpr, indexDef, indexDefs)
    indices.add(indexDef->sym);
  Vec<Expr*> domains;
  domains.add(s->domain);
  return gen_forall_internal(s->ainfo, body, indices, domains);
}

static int
gen_cond(BaseAST *a) {
  CondStmt *s = dynamic_cast<CondStmt*>(a);
  AInfo *ifelse;
  if (!s->elseStmt) {
    ifelse = 0;
  }
  else {
    ifelse = s->elseStmt->ainfo;
  }
  if1_if(if1, &s->ainfo->code, s->condExpr->ainfo->code, s->condExpr->ainfo->rval, 
         s->thenStmt->ainfo->code, s->thenStmt->ainfo->rval, ifelse ? ifelse->code : 0, 
         ifelse ? ifelse->rval : 0, 
         s->ainfo->rval, s->ainfo);
  return 0;
}

static astType_t
undef_or_fn_expr(Expr *ast) {
  if (ast->astType == EXPR_VARIABLE) { 
    Variable *v = dynamic_cast<Variable *>(ast);
    return v->var->astType;
  }
  return (astType_t)0;
}

static Sym *
gen_destruct_sym(Tuple *e, AST *ast) {
  Sym *s = new_sym();
  s->is_pattern = 1;
  s->must_implement_and_specialize(sym_tuple);
  Vec<Expr *> exprs;
  getLinkElements(exprs, e->exprs);
  forv_Expr(ee, exprs) {
    if (ee->astType == EXPR_TUPLE)
      s->has.add(gen_destruct_sym(dynamic_cast<Tuple*>(ee), ast));
    else {
      if (ee->astType != EXPR_VARIABLE)
        show_error("non-variable or tuple in destructuring assignment", ast);
      else {
        s->has.add(dynamic_cast<Variable*>(ee)->var->asymbol->sym);
      }
    }
  }
  return s;
}

static int
gen_destruct(Tuple *left, Expr *right, Expr *base_ast) {
  AInfo *ast = base_ast->ainfo;
  ast->rval = gen_destruct_sym(left, ast);
  ast->rval->ast = ast;
  if1_gen(if1, &ast->code, right->ainfo->code);
  Code *s = if1_send(if1, &ast->code, 2, 1, sym_destruct, right->ainfo->rval, ast->rval);
  s->ast = ast;
  return 0;
}

static int
is_this_member_access(BaseAST *a) {
  MemberAccess *ma = dynamic_cast<MemberAccess*>(a);
  if (!ma)
    return 0;
  Variable *v = dynamic_cast<Variable*>(ma->base);
  if (!v)
    return 0;
  if (v->var->isThis())
    return 1;
  return 0;
}

static int
gen_set_member(MemberAccess *ma, Expr *rhs, Expr *base_ast) {
  FnSymbol *fn = ma->getStmt()->parentFunction();
  AInfo *ast = base_ast->ainfo;
  ast->rval = new_sym();
  ast->rval->ast = ast;
  if1_gen(if1, &ast->code, ma->base->ainfo->code);
  if1_gen(if1, &ast->code, rhs->ainfo->code);
  Code *c = 0;
  if (!fn || (!fn->_setter && (!fn->isConstructor || !is_this_member_access(ma)))) {
    char sel[1024];
    strcpy(sel, "set_");
    strcat(sel, ma->member->asymbol->sym->name);
    Sym *selector = if1_make_symbol(if1, sel);
    c = if1_send(if1, &ast->code, 4, 1, selector, method_symbol,
                 ma->base->ainfo->rval, rhs->ainfo->rval, ast->rval);
  } else {
    Sym *selector = if1_make_symbol(if1, ma->member->asymbol->sym->name);
    c = if1_send(if1, &ast->code, 5, 1, sym_operator, ma->base->ainfo->rval, 
                 if1_make_symbol(if1, ".="), selector, rhs->ainfo->rval, ast->rval);
  }
  c->ast = ast;
  c->partial = Partial_NEVER;
  return 0;
}

static int
gen_get_member(MemberAccess *ma) {
  AInfo *ast = ma->ainfo;
  ast->rval = new_sym();
  ast->rval->ast = ast;
  if1_gen(if1, &ast->code, ma->base->ainfo->code);
  char *sel = ma->member->asymbol->sym->name;
  Sym *selector = if1_make_symbol(if1, sel);
  Code *c = if1_send(if1, &ast->code, 3, 1, selector, method_symbol,
                     ma->base->ainfo->rval, ast->rval);
  c->ast = ast;
  c->partial = Partial_NEVER;
  return 0;
}

static int
gen_paren_op(ParenOpExpr *s, Expr *rhs = 0, AInfo *ast = 0) {
  if (!ast)
    ast = s->ainfo;
  if (s->baseExpr->astType == EXPR_MEMBERACCESS && !rhs) {
    if (!s->argList) {
      ast->rval = s->baseExpr->ainfo->rval;
      ast->code = s->baseExpr->ainfo->code;
      s->baseExpr->ainfo->send->partial = Partial_NEVER;
      return 0;
    } else
      s->baseExpr->ainfo->send->partial = Partial_ALWAYS;
  }
  ast->rval = new_sym();
  ast->rval->ast = ast;
  if1_gen(if1, &ast->code, s->baseExpr->ainfo->code);
  Vec<Expr *> args;
  getLinkElements(args, s->argList);
  if (args.n == 1 && !args.v[0])
    args.n--;
  Vec<Sym *> rvals;
  if (rhs) {
    char nn[1024];
    if (s->baseExpr->astType == SYMBOL_UNRESOLVED) {
      strcpy(nn, "set_");
      strcat(nn, s->baseExpr->ainfo->sym->name);
      rvals.add(if1_make_symbol(if1, nn));
      rvals.add(method_symbol);
    } else {
      strcpy(nn, "set");
      rvals.add(if1_make_symbol(if1, nn));
      rvals.add(method_symbol);
      rvals.add(s->baseExpr->ainfo->rval);
    }
  }
  forv_Vec(Expr, a, args) {
    if1_gen(if1, &ast->code, a->ainfo->code);
    rvals.add(a->ainfo->rval);
  }
  if (rhs) {
    if1_gen(if1, &ast->code, rhs->ainfo->code);
    rvals.add(rhs->ainfo->rval);
  }
  astType_t base_symbol = undef_or_fn_expr(s->baseExpr);
  Sym *base = NULL;
  char *n = s->baseExpr->ainfo->sym->name;
  if (n && !strcmp(n, "__primitive")) {
    if (args.n > 0 && dynamic_cast<StringLiteral*>(args.v[0]) &&
        if1->primitives->prim_map[0][0].get(
          if1_cannonicalize_string(if1, dynamic_cast<StringLiteral*>(args.v[0])->str))) 
    {
      rvals.v[0] = if1_get_builtin(if1, dynamic_cast<StringLiteral*>(args.v[0])->str);
      base = 0;
    } else if (args.n == 3 && dynamic_cast<StringLiteral*>(args.v[1]) &&
               if1->primitives->prim_map[1][1].get(
                 if1_cannonicalize_string(if1, dynamic_cast<StringLiteral*>(args.v[1])->str))) {
      rvals.v[1] = if1_make_symbol(if1, rvals.v[1]->constant);
      base = sym_operator;
    } else
      base = sym_primitive;
  } else if (base_symbol == SYMBOL_UNRESOLVED) {
    assert(n);
    if (!rhs)
      base = if1_make_symbol(if1, n);
  } else if (base_symbol == SYMBOL_FN)
    base = dynamic_cast<FnSymbol*>(dynamic_cast<Variable*>(s->baseExpr)->var)->asymbol->sym;
  else {
    if (!rhs)
      base = s->baseExpr->ainfo->rval;
  }
  Code *send = if1_send1(if1, &ast->code);
  send->ast = ast;
  if (base)
    if1_add_send_arg(if1, send, base);
  forv_Sym(r, rvals)
    if1_add_send_arg(if1, send, r);
  if1_add_send_result(if1, send, ast->rval);
  send->partial = Partial_NEVER;
  ast->sym = ast->rval;
  return 0;
}

static int
gen_set(ParenOpExpr *p, Expr *rhs, Expr *base_ast) {
  AInfo *ast = base_ast->ainfo;
  if (1 || p->baseExpr->astType == EXPR_MEMBERACCESS)
    return gen_paren_op(p, rhs, ast);
  ast->rval = new_sym();
  ast->rval->ast = ast;
  if1_gen(if1, &ast->code, p->baseExpr->ainfo->code);
  if1_gen(if1, &ast->code, rhs->ainfo->code);
  Sym *selector = if1_make_symbol(if1, "set");
  Code *c = if1_send(if1, &ast->code, 4, 1, selector, method_symbol,
                     p->baseExpr->ainfo->rval, rhs->ainfo->rval, ast->rval);
  c->ast = ast;
  c->partial = Partial_NEVER;
  return 0;
}

static int
gen_if1(BaseAST *ast, BaseAST *parent) {
  // bottom's up
  GetStuff getStuff(GET_STMTS|GET_EXPRS);
  TRAVERSE(ast, &getStuff, true);
  DefStmt* def_stmt = dynamic_cast<DefStmt*>(ast);
  if (!def_stmt || !def_stmt->fnDef())
    forv_BaseAST(a, getStuff.asts)
      if (gen_if1(a, ast) < 0)
        return -1;
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
    case STMT_NOOP: break;
    case STMT_WITH: break;
    case STMT_USE: break;
    case STMT_DEF:
      if (DefStmt* def_stmt = dynamic_cast<DefStmt*>(ast)) {
        if (def_stmt->varDef() && gen_vardef(def_stmt) < 0) return -1;
      }
      break;
    case STMT_EXPR: if (gen_expr_stmt(ast) < 0) return -1; break;
    case STMT_RETURN: {
      ReturnStmt *s = dynamic_cast<ReturnStmt*>(ast);
      Sym *fn = s->parentFunction()->asymbol->sym;
      if (s->expr) {
        fn->fun_returns_value = 1;
        if1_gen(if1, &s->ainfo->code, s->expr->ainfo->code);
        if1_move(if1, &s->ainfo->code, s->expr->ainfo->rval, fn->ret, s->ainfo);
      }
      Code *c = if1_goto(if1, &s->ainfo->code, s->ainfo->label[0]);
      c->ast = s->ainfo;
      break;
    }
    case STMT_BLOCK: {
      BlockStmt *s = dynamic_cast<BlockStmt*>(ast);
      Vec<Stmt *> stmts;
      getLinkElements(stmts, s->body);
      forv_Stmt(ss, stmts)
        if1_gen(if1, &s->ainfo->code, ss->ainfo->code);
      break;
    }
    case STMT_WHILELOOP: gen_while(ast); break;
    case STMT_FORLOOP: gen_for(ast); break;
    case STMT_COND: gen_cond(ast); break;
      
    case EXPR: {
      Expr *s = dynamic_cast<Expr*>(ast);
      assert(!ast); 
      s->ainfo->rval = sym_nil;
      break;
    }
    case EXPR_LITERAL: assert(!"case"); break;
    case EXPR_BOOLLITERAL: {
      BoolLiteral *s = dynamic_cast<BoolLiteral*>(ast);
      Sym *c = if1_const(if1, sym_bool, s->str);
      c->imm.v_bool = s->val;
      s->ainfo->rval = c;
      break;
    }
    case EXPR_INTLITERAL: {
      IntLiteral *s = dynamic_cast<IntLiteral*>(ast);
      Sym *c = if1_const(if1, sym_int64, s->str);
      c->imm.v_int64 = s->val;
      s->ainfo->rval = c;
      break;
    }
    case EXPR_FLOATLITERAL: {
      FloatLiteral *s = dynamic_cast<FloatLiteral*>(ast);
      Sym *c = if1_const(if1, sym_float64, s->str);
      c->imm.v_float64 = s->val;
      s->ainfo->rval = c;
      break;
    }
    case EXPR_COMPLEXLITERAL: {
      ComplexLiteral *s = dynamic_cast<ComplexLiteral*>(ast);
      Sym *c = if1_const(if1, sym_complex64, s->str);
      c->imm.v_complex64.r = s->realVal;
      c->imm.v_complex64.i = s->imagVal;
      s->ainfo->rval = c;
      break;
    }
    case EXPR_STRINGLITERAL: {
      StringLiteral *s = dynamic_cast<StringLiteral*>(ast);
      Sym *c = if1_const(if1, sym_string, s->str);
      s->ainfo->rval = c;
      break;
    }
    case EXPR_VARIABLE: {
      Variable *s = dynamic_cast<Variable*>(ast);
      Sym *sym = s->var->asymbol->sym;
      switch (sym->asymbol->symbol->astType) {
        default: break;
        case SYMBOL_TYPE: 
          if (parent && parent->astType == EXPR_MEMBERACCESS)
            sym = ((TypeSymbol*)sym->asymbol->symbol)->type->asymbol->sym->meta_type;
          else
            sym = ((TypeSymbol*)sym->asymbol->symbol)->type->asymbol->sym;
          break;
      }
      s->ainfo->sym = sym;
      s->ainfo->rval = sym;
      break;
    }
    case EXPR_VARINIT: {
      VarInitExpr *s = dynamic_cast<VarInitExpr*>(ast);
      if (s->symbol->type->defaultVal) {
        s->ainfo->rval = new_sym();
        if1_move(if1, &s->ainfo->code, get_defaultVal(s->symbol->type), s->ainfo->rval, s->ainfo);
      } else if (s->symbol->type->defaultConstructor) {
        s->ainfo->rval = new_sym();
        Code *send = if1_send(if1, &s->ainfo->code, 1, 1, constructor_name(s->symbol->type), s->ainfo->rval);
        send->ast = s->ainfo;
      } else
        s->ainfo->rval = sym_nil;
      break;
    }
    case EXPR_USERINIT: {
      UserInitExpr *s = dynamic_cast<UserInitExpr*>(ast);
      s->ainfo->code = s->expr->ainfo->code;
      s->ainfo->rval = s->expr->ainfo->rval;
      break;
    }
    case EXPR_DEF: break;
    case EXPR_UNOP: {
      UnOp *s = dynamic_cast<UnOp*>(ast);
      s->ainfo->rval = new_sym();
      s->ainfo->rval->ast = s->ainfo;
      if1_gen(if1, &s->ainfo->code, s->operand->ainfo->code);
      Sym *op = 0;
      switch (s->type) {
        default: assert(!"case");
        case UNOP_PLUS: op = if1_make_symbol(if1, "+"); break;
        case UNOP_MINUS: op = if1_make_symbol(if1, "-"); break;
        case UNOP_LOGNOT: op = if1_make_symbol(if1, "!"); break;
        case UNOP_BITNOT: op = if1_make_symbol(if1, "~"); break;
      }
      
      Code *c = if1_send(if1, &s->ainfo->code, 3, 1, sym_operator, op, 
                         s->operand->ainfo->rval, s->ainfo->rval);
      c->ast = s->ainfo;
      break;
    }
    case EXPR_SPECIALBINOP:
    case EXPR_BINOP: {
      BinOp *s = dynamic_cast<BinOp*>(ast);
      s->ainfo->rval = new_sym();
      s->ainfo->rval->ast = s->ainfo;
      if1_gen(if1, &s->ainfo->code, s->left->ainfo->code);
      if1_gen(if1, &s->ainfo->code, s->right->ainfo->code);
      Sym *op = 0;
      switch (s->type) {
        default: assert(!"case");
        case BINOP_SEQCAT: op = if1_make_symbol(if1, "#"); break;
        case BINOP_PLUS: op = if1_make_symbol(if1, "+"); break;
        case BINOP_MINUS: op = if1_make_symbol(if1, "-"); break;
        case BINOP_MULT: op = if1_make_symbol(if1, "*"); break;
        case BINOP_DIV: op = if1_make_symbol(if1, "/"); break;
        case BINOP_MOD: op = if1_make_symbol(if1, "mod"); break;
        case BINOP_EQUAL: op = if1_make_symbol(if1, "=="); break;
        case BINOP_LEQUAL: op = if1_make_symbol(if1, "<="); break;
        case BINOP_GEQUAL: op = if1_make_symbol(if1, ">="); break;
        case BINOP_GTHAN: op = if1_make_symbol(if1, ">"); break;
        case BINOP_LTHAN: op = if1_make_symbol(if1, "<"); break;
        case BINOP_NEQUAL: op = if1_make_symbol(if1, "!="); break;
        case BINOP_BITAND: op = if1_make_symbol(if1, "&"); break;
        case BINOP_BITOR: op = if1_make_symbol(if1, "|"); break;
        case BINOP_BITXOR: op = if1_make_symbol(if1, "^"); break;
        case BINOP_LOGAND: op = if1_make_symbol(if1, "and"); break;
        case BINOP_LOGOR: op = if1_make_symbol(if1, "or"); break;
        case BINOP_EXP: op = if1_make_symbol(if1, "**"); break;
        case BINOP_BY: op = if1_make_symbol(if1, "by"); break;
      }
      Code *c = if1_send(if1, &s->ainfo->code, 3, 1, op,
                         s->left->ainfo->rval, s->right->ainfo->rval,
                         s->ainfo->rval);
      c->ast = s->ainfo;
      break;
    }
    case EXPR_MEMBERACCESS: {
      MemberAccess *s = dynamic_cast<MemberAccess*>(ast);
      FnSymbol *fn = s->getStmt()->parentFunction();
      int in_assign_or_funcall = 
        parent && (parent->astType == EXPR_ASSIGNOP ||
                   parent->astType == EXPR_ARRAYREF ||
                   parent->astType == EXPR_TUPLESELECT ||
                   parent->astType == EXPR_FNCALL ||
                   parent->astType == EXPR_PARENOP);
      if ((!fn->_getter &&
                   (!fn->isConstructor || !is_this_member_access(ast))) 
          && !in_assign_or_funcall) 
      {
        if (gen_get_member(s) < 0)
          return -1;
        break;
      }
      s->ainfo->rval = new_sym();
      s->ainfo->rval->ast = s->ainfo;
      s->ainfo->sym = s->ainfo->rval;
      if1_gen(if1, &s->ainfo->code, s->base->ainfo->code);
      Sym *op = if1_make_symbol(if1, ".");
      Sym *selector = if1_make_symbol(if1, s->member->asymbol->sym->name);
      Code *c = if1_send(if1, &s->ainfo->code, 4, 1, sym_operator,
                         s->base->ainfo->rval, op, selector,
                         s->ainfo->rval);
      c->ast = s->ainfo;
      c->partial = Partial_NEVER; // assume this is a member
      s->ainfo->send = c;
      break;
    }
    case EXPR_ASSIGNOP: {
      AssignOp *s = dynamic_cast<AssignOp*>(ast);
      if (s->left->astType == EXPR_TUPLE) {
        if (gen_destruct(dynamic_cast<Tuple*>(s->left), s->right, s) < 0)
          return -1;
        break;
      }
      if (s->left->astType == EXPR_MEMBERACCESS) {
        if (gen_set_member(dynamic_cast<MemberAccess*>(s->left), s->right, s) < 0)
          return -1;
        break;
      }
      if (s->left->astType == EXPR_ARRAYREF ||
          s->left->astType == EXPR_TUPLESELECT ||
          s->left->astType == EXPR_FNCALL ||
          s->left->astType == EXPR_PARENOP) 
      {
        if (gen_set(dynamic_cast<ParenOpExpr*>(s->left), s->right, s) < 0)
          return -1;
        break;
      }
      s->ainfo->rval = new_sym();
      s->ainfo->rval->ast = s->ainfo;
      s->ainfo->sym = s->left->ainfo->sym;
      if1_gen(if1, &s->ainfo->code, s->left->ainfo->code);
      if1_gen(if1, &s->ainfo->code, s->right->ainfo->code);
      Sym *op = 0;
      Sym *rval = s->right->ainfo->rval;
      switch (s->type) {
        default: assert(!"case");
        case GETS_NORM: op = 0; break;
        case GETS_PLUS: op = if1_make_symbol(if1, "+"); break;
        case GETS_MINUS: op = if1_make_symbol(if1, "-"); break;
        case GETS_MULT: op = if1_make_symbol(if1, "*"); break;
        case GETS_DIV: op = if1_make_symbol(if1, "/"); break;
        case GETS_BITAND: op = if1_make_symbol(if1, "&"); break;
        case GETS_BITOR: op = if1_make_symbol(if1, "|"); break;
        case GETS_BITXOR: op = if1_make_symbol(if1, "^"); break;
      }
      if (op) {
        Sym *old_rval = rval;
        rval = new_sym();
        rval->ast = s->ainfo;
        Code *c = if1_send(if1, &s->ainfo->code, 3, 1, op,
                           s->left->ainfo->rval, old_rval, rval);
        c->ast = s->ainfo;
      }
      Variable *variable = dynamic_cast<Variable*>(s->left);
      Symbol *symbol = variable ? dynamic_cast<Symbol *>(variable->var) : 0;
      Sym *type = symbol ? symbol->type->asymbol->sym->type : 0;
      FnSymbol *f = s->getStmt()->parentFunction();
      int constructor_assignment = 0;
      int getter_setter = f->_setter || f->_getter;
      if (f->isConstructor) {
        MemberAccess *m = dynamic_cast<MemberAccess*>(s->left);
        if (m) {
          Variable *v = dynamic_cast<Variable*>(m->base);
          if (v) {
            if (v->var->isThis())
              constructor_assignment = 1;
          }
        }
      }
      int operator_equal = 
        !(constructor_assignment || getter_setter ||
            (symbol && (symbol->isThis() || (symbol && scalar_or_reference(symbol->type)))));
      if (operator_equal) {
        Sym *old_rval = rval;
        rval = new_sym();
        rval->ast = s->ainfo;
        Sym *told_rval = new_sym();
        if1_move(if1, &s->ainfo->code, old_rval, told_rval, s->ainfo);
        Code *c = if1_send(if1, &s->ainfo->code, 3, 1, if1_make_symbol(if1, "="), 
                           s->left->ainfo->rval, told_rval, rval);
        c->ast = s->ainfo;
      }
      if (!s->left->ainfo->sym)
        show_error("assignment to non-lvalue", s->ainfo);
      if (symbol && symbol->type && is_scalar_type(symbol->type) &&
          !operator_equal && symbol->type != s->right->typeInfo())
        rval = gen_coerce(rval, type, &s->ainfo->code, s->ainfo);
      if1_move(if1, &s->ainfo->code, rval, s->ainfo->rval, s->ainfo);
      if (!symbol || symbol->type == dtUnknown || !operator_equal)
        if1_move(if1, &s->ainfo->code, s->ainfo->rval, s->left->ainfo->sym, s->ainfo);
      break;
    }
    case EXPR_SEQ: {
      SeqExpr *s = dynamic_cast<SeqExpr *>(ast);
      s->ainfo->sym = s->ainfo->rval = new_sym();
      s->ainfo->rval->ast = s->ainfo;
      Vec<Expr *> args;
      getLinkElements(args, s->exprls);
      forv_Vec(Expr, a, args)
        if1_gen(if1, &s->ainfo->code, a->ainfo->code);
      Code *send = if1_send1(if1, &s->ainfo->code);
      send->ast = s->ainfo;
      if1_add_send_arg(if1, send, sym_primitive);
      if1_add_send_arg(if1, send, make_seq_symbol);
      forv_Vec(Expr, a, args)
        if1_add_send_arg(if1, send, a->ainfo->rval);
      if1_add_send_result(if1, send, s->ainfo->rval);
      break;
    }
    case EXPR_SIMPLESEQ: {
      SimpleSeqExpr *s = dynamic_cast<SimpleSeqExpr *>(ast);
      s->ainfo->rval = new_sym();
      s->ainfo->rval->ast = s->ainfo;
      if1_gen(if1, &s->ainfo->code, s->lo->ainfo->code);
      if1_gen(if1, &s->ainfo->code, s->hi->ainfo->code);
      if1_gen(if1, &s->ainfo->code, s->str->ainfo->code);
      Code *send = if1_send(if1, &s->ainfo->code, 5, 1, sym_primitive, expr_simple_seq_symbol, 
                            s->lo->ainfo->rval, s->hi->ainfo->rval, s->str->ainfo->rval, 
                            s->ainfo->rval);
      send->ast = s->ainfo;
      break;
    }
    case EXPR_FLOOD: {
      FloodExpr *s = dynamic_cast<FloodExpr *>(ast);
      s->ainfo->rval = flood_symbol;
      break;
    }
    case EXPR_COMPLETEDIM: {
      CompleteDimExpr *s = dynamic_cast<CompleteDimExpr *>(ast);
      s->ainfo->rval = completedim_symbol;
      break;
    }
    case EXPR_LET: {
      LetExpr *s = dynamic_cast<LetExpr *>(ast);
      DefExpr* def_expr = s->symDefs;
      while (def_expr) {
        VarSymbol *vs = dynamic_cast<VarSymbol*>(def_expr->sym);
        while (vs) {
          if1_gen(if1, &s->ainfo->code, def_expr->init->ainfo->code);
          if1_move(if1, &s->ainfo->code, def_expr->init->ainfo->rval, vs->asymbol->sym, s->ainfo);
          vs = dynamic_cast<VarSymbol*>(vs->next);
        }
        def_expr = nextLink(DefExpr, def_expr);
      }
      if1_gen(if1, &s->ainfo->code, s->innerExpr->ainfo->code);
      s->ainfo->rval = s->innerExpr->ainfo->rval;
      break;
    }
    case EXPR_FORALL: {
      ForallExpr *s = dynamic_cast<ForallExpr *>(ast);
      s->ainfo->rval = new_sym();
      s->ainfo->rval->ast = s->ainfo;
      Vec<Expr *> domains;
      getLinkElements(domains, s->domains);
      Vec<Symbol *> indices;
      DefExpr* def_expr = dynamic_cast<DefExpr*>(s->indices);
      Symbol* indices_syms = def_expr ? def_expr->sym : 0;
      getLinkElements(indices, indices_syms);
      if (s->forallExpr) { // forall expression
        Code *body = 0;
        forv_Vec(Expr, d, domains)
          if1_gen(if1, &body, d->ainfo->code);
        if (gen_forall_internal(s->ainfo, body, indices, domains) < 0)
          return -1;
      } else {
        forv_Vec(Expr, d, domains)
          if1_gen(if1, &s->ainfo->code, d->ainfo->code);
        Code *send = if1_send(if1, &s->ainfo->code, 2, 1, sym_primitive, expr_create_domain_symbol, 
                              s->ainfo->rval);
        forv_Vec(Expr, d, domains)
          if1_add_send_arg(if1, send, d->ainfo->rval);
        send->ast = s->ainfo;
      }
      break;
    }
    case EXPR_IOCALL: {
      IOCall *s = dynamic_cast<IOCall *>(ast);
      s->ainfo->rval = new_sym();
      s->ainfo->rval->ast = s->ainfo;
      if1_gen(if1, &s->ainfo->code, s->baseExpr->ainfo->code);
      Vec<Expr *> args;
      getLinkElements(args, s->argList);
      if (args.n == 1 && !args.v[0])
        args.n--;
      forv_Vec(Expr, a, args)
        if1_gen(if1, &s->ainfo->code, a->ainfo->code);
      Code *send = if1_send1(if1, &s->ainfo->code);
      send->ast = s->ainfo;
      if1_add_send_arg(if1, send, sym_primitive);
      switch (s->ioType) {
        case IO_WRITELN:
          if1_add_send_arg(if1, send, writeln_symbol);
          break;
        case IO_WRITE:
          if1_add_send_arg(if1, send, write_symbol);
          break;
        case IO_READ:
          if1_add_send_arg(if1, send, read_symbol); 
          break;
      }
      forv_Vec(Expr, a, args)
        if1_add_send_arg(if1, send, a->ainfo->rval);
      if1_add_send_result(if1, send, s->ainfo->rval);
      break;
    }
    case EXPR_ARRAYREF: // **************** CURRENTLY UNUSED ****************
    case EXPR_TUPLESELECT:
    case EXPR_FNCALL:
    case EXPR_PARENOP:
      if (gen_paren_op(dynamic_cast<ParenOpExpr *>(ast)) < 0)
        return -1;
      break;
    case EXPR_CAST: {
      CastExpr *s = dynamic_cast<CastExpr *>(ast);
      s->ainfo->rval = new_sym();
      s->ainfo->rval->ast = s->ainfo;
      if1_gen(if1, &s->ainfo->code, s->expr->ainfo->code);
      Code *send = if1_send(if1, &s->ainfo->code, 4, 1, sym_primitive, cast_symbol, 
                            s->newType->asymbol->sym->meta_type, s->expr->ainfo->rval, s->ainfo->rval);
      send->ast = s->ainfo;
      break;
    }
    case EXPR_REDUCE: {
      ReduceExpr *s = dynamic_cast<ReduceExpr *>(ast);
      s->ainfo->rval = new_sym();
      s->ainfo->rval->ast = s->ainfo;
      if1_gen(if1, &s->ainfo->code, s->redDim->ainfo->code);
      if1_gen(if1, &s->ainfo->code, s->argExpr->ainfo->code);
      Code *send = if1_send(if1, &s->ainfo->code, 5, 1, sym_primitive, expr_reduce_symbol, 
                            s->reduceType->asymbol->sym, s->redDim->ainfo->rval, 
                            s->argExpr->ainfo->rval, s->ainfo->rval);
      send->ast = s->ainfo;
      break;
    }
    case EXPR_TUPLE: {
      Tuple *s = dynamic_cast<Tuple *>(ast);
      s->ainfo->sym = s->ainfo->rval = new_sym();
      s->ainfo->rval->ast = s->ainfo;
      Vec<Expr *> args;
      getLinkElements(args, s->exprs);
      forv_Vec(Expr, a, args)
        if1_gen(if1, &s->ainfo->code, a->ainfo->code);
      Code *send = if1_send1(if1, &s->ainfo->code);
      send->ast = s->ainfo;
      if1_add_send_arg(if1, send, sym_primitive);
      if1_add_send_arg(if1, send, make_chapel_tuple_symbol);
      forv_Vec(Expr, a, args)
        if1_add_send_arg(if1, send, a->ainfo->rval);
      if1_add_send_result(if1, send, s->ainfo->rval);
      break;
    }
    case EXPR_SIZEOF: {
      SizeofExpr *s = dynamic_cast<SizeofExpr *>(ast);
      s->ainfo->rval = new_sym();
      s->ainfo->rval->ast = s->ainfo;
      Code *send = if1_send(if1, &s->ainfo->code, 3, 1, sym_primitive, sizeof_symbol, 
                            s->type->asymbol->sym->meta_type, s->ainfo->rval);
      send->ast = s->ainfo;
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
  case SYMBOL:
  case SYMBOL_UNRESOLVED:
  case SYMBOL_MODULE:
  case SYMBOL_VAR:
  case SYMBOL_PARAM:
  case SYMBOL_TYPE:
  case SYMBOL_FN:
  case SYMBOL_ENUM:
  case SYMBOL_LABEL:
  case TYPE:
  case TYPE_BUILTIN:
  case TYPE_ENUM:
  case TYPE_DOMAIN:
  case TYPE_INDEX:
  case TYPE_SEQ:
  case TYPE_ARRAY:
  case TYPE_USER:
  case TYPE_LIKE:
  case TYPE_STRUCTURAL:
  case TYPE_CLASS:
  case TYPE_RECORD:
  case TYPE_UNION:
  case TYPE_TUPLE:
  case TYPE_SUM:
  case TYPE_VARIABLE:
  case TYPE_UNRESOLVED:
  case TYPE_NIL:
  case AST_TYPE_END:
    assert(!"case");
    break;
  }
  return 0;
}

static int
gen_fun(FnSymbol *f) {
  Sym *fn = f->asymbol->sym;
  AInfo* ast = f->defPoint->ainfo;
  Vec<Symbol *> args;
  Vec<Sym *> out_args;
  getLinkElements(args, f->formals);
  Sym *as[args.n + 3];
  int iarg = 0;
  assert(f->asymbol->sym->name);
  if (strcmp(f->asymbol->sym->name, "this") != 0) {
    Sym *s = new_sym(f->asymbol->sym->name);
    s->ast = ast;
    s->must_specialize = if1_make_symbol(if1, f->asymbol->sym->name);
    as[iarg++] = s;
    if (f->method_type != NON_METHOD) {
      Sym *s = new_sym(method_symbol->name);
      s->ast = ast;
      s->must_specialize = method_symbol;
      as[iarg++] = s;
    }
  }
  for (int i = 0; i < args.n; i++) {
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
  if1_closure(if1, fn, body, iarg, as);
  fn->ast = ast;
  if (f->_this && iarg > 1)  // HACK FOR CONSTRUCTORS WHICH SET _this
    fn->self = f->_this->asymbol->sym;
  return 0;
}

static int
init_function(FnSymbol *f) {
  Sym *s = f->asymbol->sym;
  if (verbose_level > 1 && f->name)
    printf("build_functions: %s\n", f->name);
  if (s->name && !strcmp("__init_entryPoint", s->name)) {
    if1_set_builtin(if1, s, "init");
    sym_init = s;
  }
  s->cont = new_sym();
  AInfo* ast = f->defPoint->ainfo;
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
  AInfo* ast = f->defPoint->ainfo;
  Label *return_label = ast->label[0] = if1_alloc_label(if1);
  if (resolve_labels(f->body, f->asymbol->sym->labelmap, return_label) < 0) return -1;
  if (gen_if1(f->body) < 0) return -1;
  if (gen_fun(f) < 0) return -1;
  return 0;
}

static int
build_classes(Vec<BaseAST *> &syms) {
  Vec<StructuralType *> classes;
  forv_BaseAST(s, syms)
    if (s->astType == TYPE_CLASS || 
        s->astType == TYPE_RECORD || 
        s->astType == TYPE_TUPLE || 
        s->astType == TYPE_UNION)
      classes.add(dynamic_cast<StructuralType*>(s)); 
  if (verbose_level > 1)
    printf("build_classes: %d classes\n", classes.n);
  forv_Vec(StructuralType, c, classes) {
    Sym *csym = c->asymbol->sym;
    forv_Vec(VarSymbol, tmp, c->fields)
      csym->has.add(tmp->asymbol->sym);
  }
  return 0;
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

void
ACallbacks::finalize_functions() {
  pdb->fa->method_token = unique_AVar(new Var(method_symbol), GLOBAL_CONTOUR);
  pdb->fa->array_index_base = 1;
  pdb->fa->tuple_index_base = 1;
  forv_Fun(fun, pdb->funs) {
    int added = 0;
    char *name = fun->sym->has.v[0]->name;
    assert(name);
    FnSymbol *fs = dynamic_cast<FnSymbol*>(fun->sym->asymbol->symbol);
    if (fs->classBinding && fs->classBinding->type) {
      if (is_reference_type(fs->classBinding->asymbol->symbol)) {
        if (fs->method_type != NON_METHOD) {
          add_to_universal_lookup_cache(name, fun);
          added = 1;
        }
      }
    }
    MPosition p;
    p.push(1);
    forv_Sym(s, fun->sym->has) {
      assert(!s->is_pattern); // not yet supported
      // non-scoped lookup if any parameteter is specialized on a reference type
      // (is dispatched)
      if (!added && s->must_specialize && 
          is_reference_type(s->must_specialize->asymbol->symbol)) 
      {
        add_to_universal_lookup_cache(name, fun);
        added = 1;
      }
      // record default argument positions
      if (s->asymbol->symbol) {
        ParamSymbol *symbol = dynamic_cast<ParamSymbol*>(s->asymbol->symbol);
        if (symbol && symbol->init) {
          assert(symbol->init->ainfo);
          fun->default_args.put(cannonicalize_mposition(p), symbol->init->ainfo);
        }
      }
      p.inc();
    }
    // check pragmas
    Sym *fn = fun->sym;
    FnSymbol *f = dynamic_cast<FnSymbol*>(fn->asymbol->symbol);
    Pragma *pr = f->defPoint->pragmas;
    while (pr) {
      if (!strcmp(pr->str, "test pragma"))
        printf("test pragma\n");
      pr = dynamic_cast<Pragma *>(pr->next);
    }
  }
}


static void
init_symbols() {
  domain_start_index_symbol = if1_make_symbol(if1, "domain_start_index");
  domain_next_index_symbol = if1_make_symbol(if1, "domain_next_index");
  domain_valid_index_symbol = if1_make_symbol(if1, "domain_valid_index");
  expr_simple_seq_symbol = if1_make_symbol(if1, "expr_simple_seq");
  expr_domain_symbol = if1_make_symbol(if1, "expr_domain");
  expr_create_domain_symbol = if1_make_symbol(if1, "expr_create_domain");
  expr_reduce_symbol = if1_make_symbol(if1, "expr_reduce");
  sizeof_symbol = if1_make_symbol(if1, "sizeof");
  cast_symbol = if1_make_symbol(if1, "cast");
  method_symbol = if1_make_symbol(if1, "__method");
  make_seq_symbol = if1_make_symbol(if1, "make_seq");
  make_chapel_tuple_symbol = if1_make_symbol(if1, "make_chapel_tuple");
  write_symbol = if1_make_symbol(if1, "write");
  writeln_symbol = if1_make_symbol(if1, "writeln");
  read_symbol = if1_make_symbol(if1, "read");
  array_index_symbol = if1_make_symbol(if1, "array_index");
  array_set_symbol = if1_make_symbol(if1, "array_set");
  flood_symbol = if1_make_symbol(if1, "*");
  completedim_symbol = if1_make_symbol(if1, "..");
}

static void
print_ast(BaseAST *a, Vec<BaseAST *> &asts) {
  if (!asts.set_add(a)) {
    printf("(%d *)", (int)a->astType);
    return;
  }
  printf("(%d", (int)a->astType);
  GetStuff getStuff(GET_STMTS|GET_EXPRS);
  TRAVERSE(a, &getStuff, true);
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
    printf("(%d *)", (int)a->astType);
    return;
  }
  printf("(%d", (int)a->astType);
  GetStuff* getStuff = new GetStuff(GET_STMTS|GET_EXPRS);
  TRAVERSE(a, getStuff, true);
  if (getStuff->asts.n)
    printf(" ");
  forv_BaseAST(b, getStuff->asts)
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
debug_new_ast(Vec<Stmt *> &stmts, Vec<BaseAST *> &syms) {
  if (verbose_level > 1) {
    forv_Stmt(s, stmts)
      print_one_baseast(s);
    forv_BaseAST(s, syms) {
      DefStmt* def_stmt = dynamic_cast<DefStmt*>(s);
      if (def_stmt && def_stmt->fnDef()) {
        print_ast(def_stmt->fnDef()->body);
      } else {
        Type *t = dynamic_cast<Type*>(s); 
        if (t) 
          printf("Type: %s cname %s\n", t->symbol->name, t->symbol->cname); 
      }
    }
  }
}

static void
domain_start_index(PNode *pn, EntrySet *es) {
  forv_Var(v, pn->lvals) {
    AVar *index = make_AVar(v, es);
    update_in(index, make_abstract_type(sym_int));
  }
}

static void
domain_next_index(PNode *pn, EntrySet *es) {
  forv_Var(v, pn->lvals) {
    AVar *index = make_AVar(v, es);
    update_in(index, make_abstract_type(sym_int));
  }
}

static void
integer_result(PNode *pn, EntrySet *es) {
  AVar *result = make_AVar(pn->lvals.v[0], es);
  update_in(result, make_abstract_type(sym_int));
}

static void
cast_value(PNode *pn, EntrySet *es) {
  AVar *result = make_AVar(pn->lvals.v[0], es);
  AVar *t = make_AVar(pn->rvals.v[2], es);
  assert(pn->rvals.n == 4);
  Sym *ts = t->var->sym->meta_type;
  if (ts) {
    if (ts->asymbol && is_scalar_type(ts->asymbol->symbol))
      update_in(result, make_abstract_type(ts));
    else
      creation_point(result, ts);
  }
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
  update_in(element,  make_abstract_type(sym_int));
}

static void
expr_create_domain(PNode *pn, EntrySet *es) {
  AVar *container = make_AVar(pn->lvals.v[0], es);
  creation_point(container, sym_domain);
}

static void
write_transfer_function(PNode *pn, EntrySet *es) {
  AVar *result = make_AVar(pn->lvals.v[0], es);
  update_in(result, make_abstract_type(sym_int));
}

static void
read_transfer_function(PNode *pn, EntrySet *es) {
  AVar *result = make_AVar(pn->lvals.v[0], es);
  update_in(result, make_abstract_type(sym_int));
}

static void
array_index(PNode *pn, EntrySet *es) {
  AVar *result = make_AVar(pn->lvals.v[0], es);
  AVar *array = make_AVar(pn->rvals.v[2], es);
  set_container(result, array);
  forv_CreationSet(a, *array->out) if (a) {
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
  forv_CreationSet(a, *array->out) if (a) {
    if (a->sym->element) {
      if (a->sym->element->type && a->sym->element->type->asymbol 
          && is_scalar_type(a->sym->element->type->asymbol->symbol))
        update_in(get_element_avar(a), make_abstract_type(a->sym->element->type));
      else
        flow_vars(val, get_element_avar(a));
    }
  }
  flow_vars(array, result);
}

static void
ptr_eq(PNode *pn, EntrySet *es) {
  AVar *result = make_AVar(pn->lvals.v[0], es);
  update_in(result, make_abstract_type(sym_int));
}

static void
ptr_neq(PNode *pn, EntrySet *es) {
  AVar *result = make_AVar(pn->lvals.v[0], es);
  update_in(result, make_abstract_type(sym_int));
}

static void
array_pointwise_op(PNode *pn, EntrySet *es) {
  AVar *result = make_AVar(pn->lvals.v[0], es);
  AVar *array = make_AVar(pn->rvals.v[2], es);
  flow_vars(array, result);
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
  update_in(result, make_AType(cs));
}

static Sym *
get_tuple_type(int n) {
  Vec<Type *> components;
  for (int i = 0; i < n; i++)
    components.add(dtUnknown);
  TypeSymbol *ts = TypeSymbol::lookupOrDefineTupleTypeSymbol(&components);
  if (ts->asymbol)
    return ts->type->asymbol->sym;
  map_baseast(ts);
  map_baseast(ts->type);
  TupleType *tt = dynamic_cast<TupleType*>(ts->type);
  forv_Vec(VarSymbol, x, tt->fields)
    map_baseast(x);
  Vec<BaseAST*> asts;
  asts.add(ts->type);
  build_classes(asts);
  finalize_symbols(if1);
  finalize_types(if1);
  Sym *t = build_type(ts->type);
  t->type = t;
  forv_Sym(ss, t->has)
    if (!ss->var)
      ss->var = new Var(ss);
  build_type_hierarchy();
  return t;
}

static void
make_chapel_tuple(PNode *pn, EntrySet *es) {
  Sym *t = get_tuple_type(pn->rvals.n - 2);
  prim_make(pn, es, t, 2);
}

static void
seqcat_seq(PNode *pn, EntrySet *es) {
  AVar *result = make_AVar(pn->lvals.v[0], es);
  AVar *s1 = make_AVar(pn->rvals.v[2], es);
  AVar *s2 = make_AVar(pn->rvals.v[2], es);
  forv_CreationSet(a, *s1->out) if (a) {
    AVar *ea = get_element_avar(a);
    forv_CreationSet(b, *s2->out) if (b) {
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
  forv_CreationSet(a, *s1->out) if (a) {
    AVar *ea = get_element_avar(a);
    flow_vars(s2, ea);
  }
  flow_vars(s1, result);
}

static void
indextype_get(PNode *pn, EntrySet *es) {
  AVar *result = make_AVar(pn->lvals.v[0], es);
  AVar *i = make_AVar(pn->rvals.v[2], es);
  forv_CreationSet(a, *i->out) if (a) {
    AVar *ea = get_element_avar(a);
    flow_vars(ea, result);
  }
}

static void
indextype_set(PNode *pn, EntrySet *es) {
  AVar *result = make_AVar(pn->lvals.v[0], es);
  AVar *i = make_AVar(pn->rvals.v[2], es);
  AVar *x = make_AVar(pn->rvals.v[3], es);
  forv_CreationSet(a, *i->out) if (a) {
    AVar *ea = get_element_avar(a);
    flow_vars(x, ea);
  }
  flow_vars(x, result);
}

int 
ast_to_if1(Vec<Stmt *> &stmts) {
  Vec<BaseAST *> syms;
  close_symbols(stmts, syms);
  init_symbols();
  debug_new_ast(stmts, syms);
  if (import_symbols(syms) < 0) return -1;
  if1_set_primitive_types(if1);
  if (build_classes(syms) < 0) return -1;
  finalize_types(if1);
  if (build_functions(syms) < 0) return -1;
#define REG(_n, _f) pdb->fa->primitive_transfer_functions.put(_n, new RegisteredPrim(_f));
  REG(domain_start_index_symbol->name, domain_start_index);
  REG(domain_next_index_symbol->name, domain_next_index);
  REG(domain_valid_index_symbol->name, integer_result);
  REG(expr_simple_seq_symbol->name, expr_simple_seq);
  REG(expr_domain_symbol->name, expr_domain);
  REG(expr_create_domain_symbol->name, expr_create_domain);
  REG(expr_reduce_symbol->name, expr_reduce);
  REG(sizeof_symbol->name, integer_result);
  REG(cast_symbol->name, cast_value);
  REG(write_symbol->name, write_transfer_function);
  REG(writeln_symbol->name, write_transfer_function);
  REG(read_symbol->name, read_transfer_function);
  REG(array_index_symbol->name, array_index);
  REG(array_set_symbol->name, array_set);
  REG(make_seq_symbol->name, make_seq);
  REG(make_chapel_tuple_symbol->name, make_chapel_tuple);
  REG(if1_cannonicalize_string(if1, "ptr_eq"), ptr_eq);
  REG(if1_cannonicalize_string(if1, "ptr_neq"), ptr_neq);
  REG(if1_cannonicalize_string(if1, "array_pointwise_op"), array_pointwise_op);
  REG(if1_cannonicalize_string(if1, "seqcat_seq"), seqcat_seq);
  REG(if1_cannonicalize_string(if1, "seqcat_element"), seqcat_element);
  REG(if1_cannonicalize_string(if1, "indextype_get"), indextype_get);
  REG(if1_cannonicalize_string(if1, "indextype_set"), indextype_set);
  build_type_hierarchy();
  finalize_symbols(if1);
  finalize_types(if1);  // again to catch any new ones
  return 0;
}

int
AST_to_IF1(Vec<Stmt *> &stmts) {
  if (ast_to_if1(stmts) < 0)
    fail("unable to analyze AST\n");
  return 0;
}

void 
print_AST_Expr_types(BaseAST *ast) {
  GetStuff getStuff(GET_STMTS|GET_EXPRS);
  TRAVERSE(ast, &getStuff, true);
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
    AInfo *a = dynamic_cast<AInfo *>(f->ast);
    DefStmt* def_stmt = dynamic_cast<DefStmt*>(a->xast);
    FnSymbol* fn = def_stmt->fnDef();
    print_AST_Expr_types(fn->body);
  }
}

static void
ast_sym_info(BaseAST *a, Symbol *s, AST **ast, Sym **sym) {
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
#ifdef COMPLETE_TYPING
  assert(type);
#endif
  if (!type)
    return dtUnknown;
  ASymbol *asymbol = type->asymbol;
  BaseAST *atype = asymbol->symbol;
  if (!atype)
    atype = asymbol->sym->meta_type->asymbol->symbol;
  Type *btype = dynamic_cast<Type *>(atype);
  if (!btype) {
    TypeSymbol *ts = dynamic_cast<TypeSymbol *>(atype);
    if (ts)
      btype = ts->type;
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
  AST *ast = 0;
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
  if (sym->type) {
    type = sym->type;
    goto Ldone;
  }
  if (sym->var) {
    type = sym->var->type;
    goto Ldone;
  }
 Ldone:
  return to_AST_type(type);
}

Type *
return_type_info(FnSymbol *fn) {
  if (fn->asymbol && fn->asymbol->sym)
    return to_AST_type(fn->asymbol->sym->ret->var->type);
  else
    return dtUnknown;  // analysis not run
}

void 
call_info(Expr* a, Vec<FnSymbol *> &fns) {
  FnSymbol* f = a->getStmt()->parentFunction();
  fns.clear();
  if (!f) // this is not executable code
    return;
  Fun *ff = f->asymbol->sym->fun;
  AST *ast = 0;
  Expr *e = dynamic_cast<Expr *>(a);
  if (e)
    ast = e->ainfo;
  else {
    Stmt *stmt = dynamic_cast<Stmt *>(a);
    if (stmt)
       ast = stmt->ainfo;
  }
  if (!ast)
    return; // this code is not known to analysis
  assert(ast);
  Vec<Fun *> funs;
  call_info(ff, ast, funs);
  forv_Fun(f, funs) {
    FnSymbol *fs = dynamic_cast<FnSymbol *>(f->sym->asymbol->symbol);
    assert(fs);
    fns.add(fs);
  }
}

int 
constant_info(BaseAST *a, Vec<Symbol *> &constants, Symbol *s) {
  constants.clear();
  AST *ast = 0;
  Sym *sym = 0;
  ast_sym_info(a, s, &ast, &sym);
  Vec<Sym *> consts;
  constant_info(ast, consts, sym);
  forv_Sym(ss, consts) {
    Symbol *fs = dynamic_cast<Symbol *>(ss->asymbol->symbol);
    assert(fs);
    constants.add(fs);
  }
  return constants.n;
}

int 
resolve_symbol(UnresolvedSymbol* us, MemberAccess* ma, Symbol* &s) {
  if (ma->ainfo->pnodes.n != 1)
    return -1;
  PNode *pn = ma->ainfo->pnodes.v[0];
  if (pn->code->kind != Code_SEND)
    return -2;
  ModuleSymbol *mod = dynamic_cast<ModuleSymbol*>(ma->getStmt()->parentSymbol);
  Vec<Fun *> *fns = 0;
  if (mod)
    fns = mod->initFn->asymbol->sym->fun->calls.get(pn);
  else
    fns = ma->getStmt()->parentSymbol->asymbol->sym->fun->calls.get(pn);
  if (!fns) {
    Sym *obj_type = pn->rvals.v[1]->type;
    char *sel = pn->rvals.v[3]->sym->name;
    Sym *iv = 0;
    forv_Sym(s, obj_type->has) {
      if (s->name == sel) {
        if (!iv)
          iv = s;
        else if (iv != s)
          return -3;
      }
    }
    if (iv) {
      BaseAST *sym = iv->asymbol->symbol;
      if (!sym)
        return -5;
      s = dynamic_cast<Symbol*>(sym);
      if (!s)
        return -6;
      return 0;
    }
    if (ma->parentExpr && ma->parentExpr->astType == EXPR_PARENOP) {
      ParenOpExpr *p = dynamic_cast<ParenOpExpr*>(ma->parentExpr);
      if (p->baseExpr == ma) {
        if (p->ainfo->pnodes.n != 1)
          return -7;
        pn = p->ainfo->pnodes.v[0];
        if (pn->code->kind != Code_SEND)
          return -8;
        fns = ma->getStmt()->parentFunction()->asymbol->sym->fun->calls.get(pn);
        if (!fns)
          return -9;
        if (fns->n > 1)
          return -10;
        BaseAST *sym = fns->v[0]->sym->asymbol->symbol;
        if (!sym)
          return -11;
        s = dynamic_cast<Symbol*>(sym);
        if (!s)
          return -12;
        return 0;
      }
    }
  } 
  return -13;
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
    if (t->asymbol) {
      assert(t->asymbol->sym->is_meta_type);
      if (is_scalar_type(t->type) 
          || t->type->astType == TYPE_SUM 
          || t->type->astType == TYPE_INDEX
          || (t->type->astType == TYPE_USER && 
              type_is_used(dynamic_cast<TypeSymbol*>(dynamic_cast<UserType*>(t->type)->definition->symbol))))
        return true;
      int res = t->asymbol->sym->meta_type->creators.n != 0;
      return res;
    } else
      return false;
  } else
    return true; // analysis not run   
}

int
AST_is_used(BaseAST *a, Symbol *s) {
  AST *ast = 0;
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

static void
member_info(Sym *t, char *name, int *offset, Type **type) {
  int oresult = -1;
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
          fail("missmatched offsets");
        oresult = iv->ivar_offset;
      }
    }
  }
  *offset = oresult;
  *type = dynamic_cast<Type *>(t->asymbol->symbol);
}

void
resolve_member_access(MemberAccess *ma, int *offset, Type **type) {
  assert(ma->ainfo->pnodes.n == 1);
  PNode *pn = ma->ainfo->pnodes.v[0];
  assert(pn->code->kind == Code_SEND);
  Sym *obj_type = pn->rvals.v[1]->type;
  char *sel = pn->rvals.v[3]->sym->name;
  member_info(obj_type, sel, offset, type);
}

void
resolve_member(StructuralType *t, VarSymbol *v, int *offset, Type **type) {
  member_info(t->asymbol->sym, v->name, offset, type);
}

void
structural_subtypes(Type *t, Vec<Type *> subtypes) {
  subtypes.clear();
  Sym *s = t->asymbol->sym;
  forv_Sym(ss, s->specializers) if (ss) {
    Type *tt = dynamic_cast<Type *>(ss->asymbol->symbol); assert(tt);
    subtypes.add(tt);
  }
}

int
function_returns_void(FnSymbol *fn) {
  return !fn->asymbol->sym->fun_returns_value;
}

Type *
element_type_info(TypeSymbol *t) {
  if (t->type->asymbol && t->type->asymbol->sym && t->type->asymbol->sym->element)
    return to_AST_type(t->type->asymbol->sym->element->type);
  else
    return dtUnknown;  // analysis not run
}
