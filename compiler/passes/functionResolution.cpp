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
void resolve_asts(BaseAST* base);
void resolve_call(CallExpr* call);
void resolve_op(CallExpr* call);
void resolve_dot(MemberAccess* dot);


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
        resolve_asts(inst_fn->whereExpr);
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
                          

void resolve_dot(MemberAccess* dot) {
  ClassType* ct = dynamic_cast<ClassType*>(dot->base->typeInfo());
  if (!ct) {
    INT_FATAL(dot, "Cannot resolve member access");
    return;
  }
  dot->member = Symboltable::lookupInScope(dot->member->name, ct->structScope);
  if (FnSymbol* fn = dynamic_cast<FnSymbol*>(dot->parentSymbol)) {
    if (fn->fnClass == FN_CONSTRUCTOR) {
      if (CallExpr* call = dynamic_cast<CallExpr*>(dot->parentExpr)) {
        if (dot->member->type == dtUnknown) {
          dot->member->type = call->get(2)->typeInfo();
        }
      }
    }
  }
  if (dot->member->type == dtUnknown)
    INT_FATAL(dot, "Unable to resolve type of member access");
}


CallExpr* new_default_constructor_call(Type* type) {
  if (!type->defaultConstructor)
    INT_FATAL(type, "Cannot build default constructor for type");
  CallExpr* call = new CallExpr(type->defaultConstructor->name);
  if (type->substitutions.n > 0) {
    Vec<BaseAST*> keys;
    type->substitutions.get_keys(keys);
    forv_Vec(BaseAST, key, keys) {
      char* name = NULL;
      if (Type* k = dynamic_cast<Type*>(key)) {
        name = stringcpy(k->symbol->name);
      } else if (Symbol* k = dynamic_cast<Symbol*>(key)) {
        name = stringcpy(k->name);
      } else {
        INT_FATAL(type, "Unexpected case in new_default_constructor_call");
      }
      Expr* actual = NULL;
      BaseAST* value = type->substitutions.get(key);
      if (Type* v = dynamic_cast<Type*>(value)) {
        if (v->defaultConstructor) {
          actual = new_default_constructor_call(v);
        } else {
          actual = new SymExpr(v->symbol);
        }
      } else if (Symbol* v = dynamic_cast<Symbol*>(value)) {
        actual = new SymExpr(v);
      } else {
        INT_FATAL(type, "Unexpected case in new_default_constructor_call");
      }
      call->argList->insertAtTail(new NamedExpr(name, actual));
    }
  }
  return call;
}

void resolve_op(CallExpr* call) {
  if (call->opTag == OP_MOVE) {
    if (SymExpr* symExpr = dynamic_cast<SymExpr*>(call->argList->get(1))) {
      if (CallExpr* prim = dynamic_cast<CallExpr*>(call->argList->get(2)))
        if (prim->isNamed("__primitive") || (prim->primitive && strcmp(prim->primitive->name, "init")))
          return;
      Type* type = call->argList->get(2)->typeInfo();
      if (type != dtNil) {
        if (symExpr->var->type == dtUnknown)
          symExpr->var->type = type;
        else if (symExpr->var->type != type)
          INT_FATAL("type mismatch in function resolution");
      }
      if (symExpr->var->type == dtUnknown)
        INT_FATAL("Unable to resolve type");
    }
  } else if (call->primitive && !strcmp(call->primitive->name, "init")) {
    Type* type = call->get(1)->typeInfo();
    if (type->defaultValue) {
      call->replace(new CastExpr(new SymExpr(type->defaultValue), NULL, type));
    } else if (type->defaultConstructor) {
      CallExpr* c = new_default_constructor_call(type);
      call->replace(c);
      resolve_asts(c);
/*
      if (CallExpr* icall = dynamic_cast<CallExpr*>(call->get(1))) {
        icall->remove();
        call->replace(icall);
      } else {
        CallExpr* newcall = new CallExpr(type->defaultConstructor->name);
        call->replace(newcall);
        resolve_call(newcall);
      }
*/
    } else {
      INT_FATAL(call, "Type without defaultValue in function resolution");
    }
  }
}


void resolve_call(CallExpr* call) {
  if (call->isNamed("__primitive"))
    return;

  if (call->isNamed("_chpl_alloc")) {
    call->baseExpr->replace(new SymExpr(Symboltable::lookupInternal("_chpl_alloc")));
    return;
  }

  if (CallExpr* partial = dynamic_cast<CallExpr*>(call->baseExpr)) {
    if (partial->typeInfo() == dtUnknown) {
      call->baseExpr->replace(partial->baseExpr->copy());
      call->argList->insertAtHead(partial->argList->copy());
    }
  }

  SymExpr* base = dynamic_cast<SymExpr*>(call->baseExpr);

  if (call->isResolved()) {
    resolve_function(call->isResolved());
    return;
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
    resolve_function(best);
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
    resolve_function(best);
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

  if (ForLoopStmt* loop = dynamic_cast<ForLoopStmt*>(call->parentStmt)) {
    if (loop->iterators->only() == call) {
      Symbol* index = loop->indices->only()->sym;
      if (index->type == dtUnknown) {
        Type* type = call->typeInfo();
        if (ClassType* seq = dynamic_cast<ClassType*>(type)) {
          if (ClassType* seqNode = dynamic_cast<ClassType*>(seq->fields.v[1]->type)) {
            index->type = seqNode->fields.v[0]->type;
          }
        }
      }
      if (index->type == dtUnknown) {
        INT_FATAL(index, "Could not determine type of index");
      }
    }
  }

  if (best->hasPragma("builtin"))
    call->makeOp();
}

void resolve_asts(BaseAST* base) {
  Vec<BaseAST*> asts;
  collect_asts_postorder(&asts, base);
  forv_Vec(BaseAST, ast, asts) {
    if (MemberAccess* dot = dynamic_cast<MemberAccess*>(ast)) {
      resolve_dot(dot);
    } else if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
      if (!call->baseExpr)
        resolve_op(call);
      else
        resolve_call(call);
    }
  }
}

void resolve_function(FnSymbol* fn) {
  if (fns.set_in(fn))
    return;

  fns.set_add(fn);

  resolve_asts(fn->formals);

  for_alist(DefExpr, formalDef, fn->formals) {
    ArgSymbol* arg = dynamic_cast<ArgSymbol*>(formalDef->sym);
    if (arg->type == dtUnknown && formalDef->exprType) {
      arg->type = formalDef->exprType->typeInfo();
    }
    if (arg->type == dtUnknown ||
        arg->type == dtAny ||
        arg->type == dtNumeric ||
        arg->isGeneric) {
      INT_FATAL(fn, "Generic function should have been cloned");
      return;
    }
  }

  resolve_asts(fn->body);

  if (fn->fnClass == FN_CONSTRUCTOR)
    resolve_asts(fn->typeBinding->defPoint);

  resolve_return_type(fn);
}

void functionResolution(void) {
  if (!no_infer)
    return;
  resolve_function(chpl_main);

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
