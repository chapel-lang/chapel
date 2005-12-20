#include "functionResolution.h"
#include "astutil.h"
#include "expr.h"
#include "stmt.h"
#include "symtab.h"
#include "stringutil.h"
#include "runtime.h"

void param_compute(Expr* expr);

bool can_dispatch(Type* actualType, Type* formalType);
bool can_dispatch_ne(Type* actualType, Type* formalType) {
  if (actualType != dtAny && formalType == dtAny)
    return true;
  if (actualType == dtNil)
    return true;
  if ((formalType == dtNumeric && actualType == dtBoolean) ||
      (formalType == dtNumeric && actualType == dtInteger) ||
      (formalType == dtNumeric && actualType == dtFloat))
    return true;
  if ((formalType == dtFloat && actualType == dtInteger) ||
      (formalType == dtInteger && actualType == dtBoolean))
    return true; // need coercion wrapper
  forv_Vec(Type, parent, actualType->dispatchParents) {
    if (parent == formalType || can_dispatch(parent, formalType))
      return true;
  }
  return false;
}

bool can_dispatch(Type* actualType, Type* formalType) {
  return (actualType == formalType) || can_dispatch_ne(actualType, formalType);
}

Vec<FnSymbol*> fns; // live functions list
Vec<FnSymbol*> newFns; // new functions list;

void resolve_return_type(FnSymbol* fn);
void resolve_function(FnSymbol* fn);
void resolve_call(CallExpr* call);

class ResolveCalls : public Traversal {
  void ResolveCalls::postProcessExpr(Expr* expr) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(expr)) {
      if (call->opTag == OP_NONE) {
        resolve_call(call);
      } else if (call->opTag == OP_MOVE) {
        if (SymExpr* symExpr = dynamic_cast<SymExpr*>(call->argList->get(1))) {
          if (CallExpr* prim = dynamic_cast<CallExpr*>(call->argList->get(2)))
            if (prim->isNamed("__primitive"))
              return;
          Type* type = call->argList->get(2)->typeInfo();
          if (type == dtNil)
            return;
          if (symExpr->var->type == dtUnknown)
            symExpr->var->type = type;
          else if (symExpr->var->type != type)
            INT_FATAL("type mismatch in function resolution");
        }
      } else if (call->opTag == OP_INIT) {
        AList<Expr>* args = call->argList->copy();
        Expr* arg1 = args->get(1);
        arg1->remove();
        CallExpr* newcall = NULL;
        if (SymExpr* symExpr = dynamic_cast<SymExpr*>(arg1)) {
          if (TypeSymbol* ts = dynamic_cast<TypeSymbol*>(symExpr->var)) {
            Type* type = ts->definition;
            if (type->defaultValue) {
              call->replace(new CastExpr(new SymExpr(type->defaultValue),
                                         arg1, type));
            } else if (type->defaultConstructor) {
              newcall = new CallExpr(type->defaultConstructor->name, args);
              call->replace(newcall);
            } else {
              INT_FATAL(call, "Type without defaultValue in function resolution");
            }
          } else {
            INT_FATAL(call, "init not type");
          }
        } else if (CallExpr* ce = dynamic_cast<CallExpr*>(arg1)) {
          newcall = ce;
          call->replace(newcall);
        }
        if (newcall)
          resolve_call(newcall);
      }
    }
  }
};

bool actual_formal_match(Expr* actual, ArgSymbol* formal) {
  if (formal->intent == INTENT_TYPE ||
      formal->type == dtUnknown)
    return true;
  if (NamedExpr* named = dynamic_cast<NamedExpr*>(actual))
    actual = named->actual;
  if (can_dispatch(actual->typeInfo(), formal->type))
    return true;
  return false;
}


// Return actual-formal map if FnSymbol is viable candidate to call
static void
add_candidate(Map<FnSymbol*,Vec<ArgSymbol*>*>* af_maps,
              CallExpr* call, FnSymbol* fn, bool inst = false) {
  Vec<ArgSymbol*>* af_map = new Vec<ArgSymbol*>();
  Vec<Expr*> fa_map;
  for_alist(DefExpr, formalDef, fn->formals)
    fa_map.add(NULL);
  for_alist(Expr, actual, call->argList)
    af_map->add(NULL);
  int i = -1;
  for_alist(Expr, actual, call->argList) {
    i++;
    if (NamedExpr* named = dynamic_cast<NamedExpr*>(actual)) {
      bool match = false;
      int j = -1;
      for_alist(DefExpr, formalDef, fn->formals) {
        ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
        j++;
        if (!strcmp(named->name, formalDef->sym->name)) {
          match = true;
          af_map->v[i] = formal;
          fa_map.v[j] = actual;
          break;
        }
      }
      if (!match)
        return;
    }
  }
  i = -1;
  for_alist(Expr, actual, call->argList) {
    i++;
    if (!dynamic_cast<NamedExpr*>(actual)) {
      int j = -1;
      bool match = false;
      for_alist(DefExpr, formalDef, fn->formals) {
        ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
        j++;
        if (!fa_map.v[j]) {
          af_map->v[i] = formal;
          fa_map.v[j] = actual;
          match = true;
          break;
        }
      }
      if (!match)
        return;
    }
  }

  if (!inst) {
    ASTMap subs;
    int j = -1;
    for_alist(DefExpr, formalDef, fn->formals) {
      ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
      j++;
      if (formal->intent == INTENT_TYPE) {
        Expr* actual = fa_map.v[j];
        if (NamedExpr* named = dynamic_cast<NamedExpr*>(actual))
          actual = named->actual;
        TypeSymbol* ts = dynamic_cast<TypeSymbol*>(formal->genericSymbol);
        if (!ts)
          INT_FATAL(formalDef, "Unanticipated genericSymbol");
        subs.put(ts->definition, actual->typeInfo());
      } else if (formal->intent == INTENT_PARAM) {
        Expr* actual = fa_map.v[j];
        if (NamedExpr* named = dynamic_cast<NamedExpr*>(actual))
          actual = named->actual;
        if (SymExpr* symExpr = dynamic_cast<SymExpr*>(actual))
          if (symExpr->var->isParam())
            subs.put(formal, symExpr->var);
      }
    }
    if (subs.n) {
      Vec<FnSymbol*> inst_fns;
      Vec<TypeSymbol*> inst_ts;
      FnSymbol* inst_fn = fn->instantiate_generic(&subs, &inst_fns, &inst_ts);
      newFns.set_add(inst_fn);
      if (inst_fn->whereExpr) {
        param_compute(inst_fn->whereExpr);
        if (SymExpr* symExpr = dynamic_cast<SymExpr*>(inst_fn->whereExpr)) {
          VarSymbol* param = dynamic_cast<VarSymbol*>(symExpr->var);
          if (param != chpl_true && param != chpl_false)
            INT_FATAL(fn, "Illegal where expression");
          if (param == chpl_false)
            return;
        } else {
          INT_FATAL(fn, "Illegal where expression");
          return;
        }
      }
      add_candidate(af_maps, call, inst_fn, true);
      return;
    }
  }

  int j = -1;
  for_alist(DefExpr, formalDef, fn->formals) {
    ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
    j++;
    if (fa_map.v[j] && !actual_formal_match(fa_map.v[j], formal))
      return;
    if (!fa_map.v[j] && !formal->defaultExpr)
      return;
  }
  af_maps->put(fn, af_map);
}
                          

void resolve_call(CallExpr* call) {
  if (CallExpr* partial = dynamic_cast<CallExpr*>(call->baseExpr)) {
    if (partial->typeInfo() == dtUnknown) {
      call->baseExpr->replace(partial->baseExpr->copy());
      call->argList->insertAtHead(partial->argList->copy());
    }
  }

  SymExpr* base = dynamic_cast<SymExpr*>(call->baseExpr);

  if (base && !strcmp(base->var->name, "__primitive")) // ignore __primitive
    return;

  if (base && !strcmp(base->var->name, "_chpl_alloc")) { // resolve alloc immediately
    base->var = Symboltable::lookupInternal("_chpl_alloc");
    return;
  }

  if (base) {
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(base->var)) {
      resolve_function(fn);
      return; // return if already resolved
    }
  }

  if (!base ||
      dynamic_cast<VarSymbol*>(base->var) ||
      dynamic_cast<ArgSymbol*>(base->var)) {
    CallExpr* newCall = new CallExpr("this", call->baseExpr->copy(),
                                     call->argList->copy());
    call->replace(newCall);
    call = newCall;
    base = dynamic_cast<SymExpr*>(call->baseExpr);
  }

  Vec<FnSymbol*> visibleFns; // visible function candidates
  char* canon_name = cannonicalize_string(base->var->name);
  call->parentScope->getVisibleFunctions(&visibleFns, canon_name);

  for (int i = 0; i < visibleFns.n; i++)
    if (newFns.set_in(visibleFns.v[i]))
      visibleFns.v[i] = NULL;

  Map<FnSymbol*,Vec<ArgSymbol*>*> af_maps; // viable function candidates
                                           // and actual to formal mapping

  forv_Vec(FnSymbol, visibleFn, visibleFns) {
    if (!visibleFn)
      continue;
    add_candidate(&af_maps, call, visibleFn);
  }

  if (af_maps.n > 1) { // remove strictly worse matches
    for (int i = 0; i < af_maps.n; i++) {
      if (!af_maps.v[i].key)
        continue;
      for (int j = i + 1; j < af_maps.n; j++) {
        if (!af_maps.v[j].key || !af_maps.v[i].key)
          continue;
        bool j_better = false;
        bool j_worse = false;

        // do the candidates have default values?
        bool i_defaults = af_maps.v[i].key->formals->length() > call->argList->length();
        bool j_defaults = af_maps.v[j].key->formals->length() > call->argList->length();
        if (i_defaults && !j_defaults)
          j_better = true;
        if (!i_defaults && j_defaults)
          j_worse = true;

        // do actuals map better to one candidate?
        int k = 0;
        for_alist(Expr, actual, call->argList) {
          ArgSymbol* i_formal = af_maps.v[i].value->v[k];
          ArgSymbol* j_formal = af_maps.v[j].value->v[k];
          if (i_formal->intent != INTENT_TYPE &&
              j_formal->intent != INTENT_TYPE) {
            if (can_dispatch_ne(j_formal->type, i_formal->type))
              j_better = true;
            if (can_dispatch_ne(i_formal->type, j_formal->type))
              j_worse = true;
          }
          k++;
        }

        if (j_worse && !j_better)
          af_maps.v[j].key = NULL;
        if (j_better && !j_worse)
          af_maps.v[i].key = NULL;
      }
    }
  }

  if (af_maps.n == 0) {
    if (call->partialTag == PARTIAL_OK)
      return;
    else {
      USR_FATAL(call, "Unresolved function call");
      return;
    }
  }

  bool ambiguous = false;
  Vec<ArgSymbol*>* af_map = 0;
  FnSymbol* best = NULL;

  for (int i = 0; i < af_maps.n; i++) {
    if (af_maps.v[i].key) {
      if (!best) {
        best = af_maps.v[i].key;
        af_map = af_maps.v[i].value;
      } else {
        ambiguous = true;
      }
    }
  }

  if (ambiguous) {
    USR_FATAL_CONT(call, "Ambiguous function call, candidates are:");
    for (int i = 0; i < af_maps.n; i++)
      USR_FATAL_CONT(af_maps.v[i].key, "%s", af_maps.v[i].key->name);
    USR_FATAL(call, "Unable to resolve function call");
  }

  if (call->partialTag == PARTIAL_OK && !best->noParens)
    return;

  // build default_wrapper
  if (best->formals->length() > call->argList->length()) {
    Vec<Symbol*> defaults;
    for_alist(DefExpr, formalDef, best->formals) {
      ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
      bool used = false;
      forv_Vec(ArgSymbol, arg, *af_map) {
        if (arg == formal)
          used = true;
      }
      if (!used)
        defaults.add(formal);
    }
    FnSymbol* default_wrapper = best->default_wrapper(&defaults);
    newFns.set_add(default_wrapper);

    // update af_map and best
    DefExpr* newFormalDef = default_wrapper->formals->first();
    for_alist(DefExpr, formalDef, best->formals) {
      ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
      for (int i = 0; i < af_map->n; i++) {
        if (af_map->v[i] == formal) {
          ArgSymbol* newFormal = dynamic_cast<ArgSymbol*>(newFormalDef->sym);
          af_map->v[i] = newFormal;
          newFormalDef = default_wrapper->formals->next();
        }
      }
    }
    best = default_wrapper;
  }

  // build order wrapper
  bool order_wrapper_required = false;
  Map<Symbol*,Symbol*> formals_to_formals;
  int i = 0;
  for_alist(DefExpr, formalDef, best->formals) {
    i++;
    ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);

    int j = 0;
    forv_Vec(ArgSymbol, af, *af_map) {
      j++;
      if (af == formal) {
        if (i != j)
          order_wrapper_required = true;
        formals_to_formals.put(formal, af_map->v[i-1]);
      }
    }
  }
  if (order_wrapper_required) {
    best = best->order_wrapper(&formals_to_formals);
    newFns.set_add(best);
  }

  // build coercion wrapper
  // NEED TO WRITE

  // clone underspecified functions
  {
    ASTMap formal_types;
    Expr* actual = call->argList->first();
    for_alist(DefExpr, formalDef, best->formals) {
      ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
      if (formal->type == dtAny ||
          formal->type == dtUnknown ||
          formal->type == dtNumeric) {
        Expr* temp = actual;
        if (NamedExpr* named = dynamic_cast<NamedExpr*>(temp))
          temp = named->actual;
        if (temp->typeInfo() == dtUnknown)
          INT_FATAL(temp, "Cannot determine type of actual");
        formal_types.put(formal, temp->typeInfo());
      }
      actual = call->argList->next();
    }
    if (formal_types.n) {
      best = best->clone_generic(&formal_types);
      newFns.set_add(best);
    }
  }

  base->var = best;
  resolve_function(best);
  if (best->hasPragma("builtin"))
    call->makeOp();
}

void resolve_function(FnSymbol* fn) {
  if (fns.set_in(fn))
    return;

  fns.set_add(fn);

  TRAVERSE(fn->formals, new ResolveCalls(), true);

  for_alist(DefExpr, formalDef, fn->formals) {
    ArgSymbol* arg = dynamic_cast<ArgSymbol*>(formalDef->sym);
    if (arg->type == dtUnknown ||
        arg->type == dtAny ||
        arg->type == dtNumeric ||
        arg->isGeneric) {
      INT_FATAL(fn, "Generic function should have been cloned");
      return;
    }
  }

  TRAVERSE(fn->defPoint, new ResolveCalls(), true);

  if (fn->fnClass == FN_CONSTRUCTOR)
    TRAVERSE(fn->typeBinding->defPoint, new ResolveCalls(), true);

  resolve_return_type(fn);
}

void functionResolution(void) {
  fns.set_add(chpl_main);
  TRAVERSE(chpl_main, new ResolveCalls(), true);

  Vec<TypeSymbol*> dead_types;
  Vec<FnSymbol*> all_fns;
  collect_functions(&all_fns);
  forv_Vec(FnSymbol, fn, all_fns) {
    if (!fns.set_in(fn)) {
      if (fn->fnClass == FN_CONSTRUCTOR)
        dead_types.add(fn->typeBinding);
      fn->defPoint->parentStmt->remove();
    }
  }
  forv_Vec(TypeSymbol, type, dead_types) {
    type->defPoint->parentStmt->remove();
  }
}


void resolve_return_type(FnSymbol* fn) {
  if (fn->retType != dtUnknown)
    return;

  if (fn->defPoint->exprType) {
    fn->retType = fn->defPoint->exprType->typeInfo();
    return;
  }

  Type* return_type = dtVoid;
  Vec<BaseAST*> asts;
  collect_asts(&asts, fn);
  forv_Vec(BaseAST, ast, asts) {
    if (ReturnStmt* returnStmt = dynamic_cast<ReturnStmt*>(ast)) {
      if (returnStmt->expr) {
        if (return_type == dtVoid) {
          return_type = returnStmt->expr->typeInfo();
        } else if (return_type != returnStmt->expr->typeInfo()) {
          USR_FATAL(fn, "Unable to resolve return type of function");
        }
      }
    }
  }
  fn->retType = return_type;
}

static void
param_reduce(CallExpr* call) {
  if (call->argList->length() != 2)
    return;
  SymExpr* lsym = dynamic_cast<SymExpr*>(call->argList->get(1));
  SymExpr* rsym = dynamic_cast<SymExpr*>(call->argList->get(2));
  if (!lsym || !rsym)
    return;
  if (!lsym->isParam() || !rsym->isParam())
    return;
  if (call->opTag == OP_EQUAL) {
    if (lsym->var == rsym->var)
      call->replace(new SymExpr(chpl_true));
    else
      call->replace(new SymExpr(chpl_false));
  }
}

class ParamCompute : public Traversal {
  void ParamCompute::postProcessExpr(Expr* expr) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(expr)) {
      param_reduce(call);
    }
  }
};

void param_compute(Expr* expr) {
  TRAVERSE(expr, new ParamCompute(), false);
}
