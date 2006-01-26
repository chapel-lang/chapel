#include "functionResolution.h"
#include "astutil.h"
#include "expr.h"
#include "stmt.h"
#include "symtab.h"
#include "stringutil.h"
#include "runtime.h"

resolve_call_error_type resolve_call_error;

void param_compute(Expr* expr);

bool can_dispatch(Symbol* actualParam, Type* actualType, Type* formalType);
bool can_dispatch_ne(Symbol* actualParam, Type* actualType, Type* formalType) {
  if (actualType != dtAny && formalType == dtAny)
    return true;
  if (actualType == dtNil)
    if (ClassType* ct = dynamic_cast<ClassType*>(formalType))
      if (ct->classTag == CLASS_CLASS)
        return true;
  if ((formalType == dtNumeric && actualType == dtBoolean) ||
      (formalType == dtNumeric && actualType == dtInteger) ||
      (formalType == dtNumeric && actualType == dtFloat))
    return true;
  if ((formalType == dtFloat && actualType == dtInteger) ||
      (formalType == dtInteger && actualType == dtBoolean))
    return true; // need coercion wrapper
  if (LiteralType* lt = dynamic_cast<LiteralType*>(formalType))
    if (lt->literal == actualParam)
      return true;
  forv_Vec(Type, parent, actualType->dispatchParents) {
    if (parent == formalType || can_dispatch(actualParam, parent, formalType))
      return true;
  }
  return false;
}

bool can_dispatch(Symbol* actualParam, Type* actualType, Type* formalType) {
  return (actualType == formalType) ||
    can_dispatch_ne(actualParam, actualType, formalType);
}

Vec<FnSymbol*> fns; // live functions list
Vec<FnSymbol*> newFns; // new functions list;

void resolve_return_type(FnSymbol* fn);
void resolve_function(FnSymbol* fn);
void resolve_asts(BaseAST* base);
void resolve_op(CallExpr* call);

bool actual_formal_match(Symbol* actual_param, Type* actual_type, ArgSymbol* formal) {
  if (formal->intent == INTENT_TYPE || formal->type == dtUnknown)
    return true;
  if (can_dispatch(actual_param, actual_type, formal->type))
    return true;
  return false;
}


// Return actual-formal map if FnSymbol is viable candidate to call
static void
add_candidate(Map<FnSymbol*,Vec<ArgSymbol*>*>* candidateFns,
              FnSymbol* fn,
              Vec<Type*>* actual_types,
              Vec<Symbol*>* actual_params,
              Vec<char*>* actual_names,
              bool inst = false) {
  int num_actuals = actual_types->n;
  int num_formals = fn->formals ? fn->formals->length() : 0;

  Vec<ArgSymbol*>* actual_formals = new Vec<ArgSymbol*>();
  Vec<Type*> formal_actuals;
  Vec<Symbol*> formal_params;
  for (int i = 0; i < num_formals; i++) {
    formal_actuals.add(NULL);
    formal_params.add(NULL);
  }
  for (int i = 0; i < num_actuals; i++)
    actual_formals->add(NULL);
  for (int i = 0; i < num_actuals; i++) {
    if (actual_names->v[i]) {
      bool match = false;
      int j = -1;
      for_alist(DefExpr, formalDef, fn->formals) {
        ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
        j++;
        if (!strcmp(actual_names->v[i], formal->name)) {
          match = true;
          actual_formals->v[i] = formal;
          formal_actuals.v[j] = actual_types->v[i];
          formal_params.v[j] = actual_params->v[i];
          break;
        }
      }
      if (!match)
        return;
    }
  }
  for (int i = 0; i < num_actuals; i++) {
    if (!actual_names->v[i]) {
      bool match = false;
      int j = -1;
      for_alist(DefExpr, formalDef, fn->formals) {
        ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
        j++;
        if (!formal_actuals.v[j]) {
          match = true;
          actual_formals->v[i] = formal;
          formal_actuals.v[j] = actual_types->v[i];
          formal_params.v[j] = actual_params->v[i];
          break;
        }
      }
      if (!match)
        return;
    }
  }
  if (!inst) {
    ASTMap subs;
    for (int i = 0; i < num_actuals; i++) {
      if (actual_formals->v[i]->intent == INTENT_TYPE) {
        TypeSymbol* ts =
          dynamic_cast<TypeSymbol*>(actual_formals->v[i]->genericSymbol);
        if (!ts)
          INT_FATAL(actual_formals->v[i], "Unanticipated genericSymbol");
        subs.put(ts->definition, actual_types->v[i]);
      } else if (actual_formals->v[i]->intent == INTENT_PARAM) {
        if (actual_params->v[i])
          subs.put(actual_formals->v[i], actual_params->v[i]);
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
      add_candidate(candidateFns, inst_fn, actual_types, actual_params, actual_names, true);
      return;
    }
  }

  int j = -1;
  for_alist(DefExpr, formalDef, fn->formals) {
    ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
    j++;
    if (formal_actuals.v[j] &&
        !actual_formal_match(formal_params.v[j], formal_actuals.v[j], formal))
      return;
    if (!formal_actuals.v[j] && !formal->defaultExpr)
      return;
  }
  candidateFns->put(fn, actual_formals);
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
  if (call->isPrimitive(PRIMITIVE_MOVE)) {
    if (SymExpr* symExpr = dynamic_cast<SymExpr*>(call->argList->get(1))) {
      if (CallExpr* prim = dynamic_cast<CallExpr*>(call->argList->get(2))) {
        if (prim->typeInfo() == dtUnknown)
          return;
      }
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
  } else if (call->primitive &&
             ((!strcmp(call->primitive->name, ".")) ||
              (!strcmp(call->primitive->name, ".=")))) {
    ClassType* ct = dynamic_cast<ClassType*>(call->get(1)->typeInfo());
    if (!ct) {
      INT_FATAL(call, "Cannot resolve member access");
      return;
    }
    call->member = Symboltable::lookupInScope(
      dynamic_cast<VarSymbol*>(dynamic_cast<SymExpr*>(call->get(2))->var)->immediate->v_string,
      ct->structScope);
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(call->parentSymbol))
      if (fn->fnClass == FN_CONSTRUCTOR &&
          !strcmp(call->primitive->name, ".=") && 
          call->member->type == dtUnknown)
        call->member->type = call->get(3)->typeInfo();
    if (call->member->type == dtUnknown)
      INT_FATAL(call, "Unable to resolve type of member access");
  } else if (call->primitive && !strcmp(call->primitive->name, "init")) {
    Type* type = call->get(1)->typeInfo();
    if (type->defaultValue) {
      call->replace(new CastExpr(new SymExpr(type->defaultValue), NULL, type));
    } else if (type->defaultConstructor) {
      CallExpr* c = new_default_constructor_call(type);
      call->replace(c);
      resolve_asts(c);
    } else {
      INT_FATAL(call, "Type without defaultValue in function resolution");
    }
  }
}


static FnSymbol*
build_default_wrapper(FnSymbol* fn,
                      Vec<ArgSymbol*>* actual_formals) {
  FnSymbol* wrapper = fn;
  int num_actuals = actual_formals->n;
  int num_formals = fn->formals ? fn->formals->length() : 0;
  if (num_formals > num_actuals) {
    Vec<Symbol*> defaults;
    for_alist(DefExpr, formalDef, fn->formals) {
      ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
      bool used = false;
      forv_Vec(ArgSymbol, arg, *actual_formals) {
        if (arg == formal)
          used = true;
      }
      if (!used)
        defaults.add(formal);
    }
    wrapper = fn->default_wrapper(&defaults);
    newFns.set_add(wrapper);

    // update actual_formals for use in build_order_wrapper
    DefExpr* newFormalDef = wrapper->formals->first();
    for_alist(DefExpr, formalDef, fn->formals) {
      ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
      for (int i = 0; i < actual_formals->n; i++) {
        if (actual_formals->v[i] == formal) {
          ArgSymbol* newFormal = dynamic_cast<ArgSymbol*>(newFormalDef->sym);
          actual_formals->v[i] = newFormal;
          newFormalDef = wrapper->formals->next();
        }
      }
    }
  }
  return wrapper;
}


static FnSymbol*
build_order_wrapper(FnSymbol* fn,
                    Vec<ArgSymbol*>* actual_formals) {
  bool order_wrapper_required = false;
  Map<Symbol*,Symbol*> formals_to_formals;
  int i = 0;
  for_alist(DefExpr, formalDef, fn->formals) {
    i++;
    ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);

    int j = 0;
    forv_Vec(ArgSymbol, af, *actual_formals) {
      j++;
      if (af == formal) {
        if (i != j)
          order_wrapper_required = true;
        formals_to_formals.put(formal, actual_formals->v[i-1]);
      }
    }
  }
  if (order_wrapper_required) {
    fn = fn->order_wrapper(&formals_to_formals);
    newFns.set_add(fn);
  }
  return fn;
}


static FnSymbol*
clone_underspecified_function(FnSymbol* fn,
                              CallExpr* call) {
  ASTMap formal_types;
  Expr* actual = call->argList->first();
  for_alist(DefExpr, formalDef, fn->formals) {
    ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
    if (formal->type == dtAny ||
        formal->type == dtUnknown ||
        formal->type == dtNumeric) {
      if (actual->typeInfo() == dtUnknown)
        INT_FATAL(actual, "Cannot determine type of actual");
      formal_types.put(formal, actual->typeInfo());
    }
    actual = call->argList->next();
  }
  if (formal_types.n) {
    fn = fn->clone_generic(&formal_types);
    newFns.set_add(fn);
  }
  return fn;
}


FnSymbol*
resolve_call(CallExpr* call,
             Vec<Type*>* actual_types,
             Vec<Symbol*>* actual_params,
             Vec<char*>* actual_names) {
  resolve_call_error = CALL_NO_ERROR;

  if (call->isNamed("_chpl_alloc"))
    return dynamic_cast<FnSymbol*>(Symboltable::lookupInternal("_chpl_alloc"));

  if (call->isResolved())
    return call->isResolved();

  SymExpr* base = dynamic_cast<SymExpr*>(call->baseExpr);
  char* name = base->var->name;

  FnSymbol *fn = 
    resolve_call(call, name, actual_types, actual_params, actual_names, call->partialTag);
  if (!fn) {
    switch (resolve_call_error) {
      default: break;
      case CALL_AMBIGUOUS:
        USR_WARNING(call, "Ambiguous function call");
        break;
      case CALL_UNKNOWN:
        USR_WARNING(call, "Unresolved function call");
        break;
    }
  }
  return fn;
}

FnSymbol*
resolve_call(BaseAST* ast,
             char *name,
             Vec<Type*>* actual_types,
             Vec<Symbol*>* actual_params,
             Vec<char*>* actual_names,
             PartialTag partialTag,
             FnSymbol *fnSymbol)
{
  Map<FnSymbol*,Vec<ArgSymbol*>*> candidateFns; // candidate functions

  if (!fnSymbol) {
    char* canon_name = cannonicalize_string(name);
    Vec<FnSymbol*> visibleFns;                    // visible functions
    ast->parentScope->getVisibleFunctions(&visibleFns, canon_name);
    forv_Vec(FnSymbol, visibleFn, visibleFns)
      if (!newFns.set_in(visibleFn))
        add_candidate(&candidateFns, visibleFn, actual_types, actual_params, actual_names);
  } else 
    add_candidate(&candidateFns, fnSymbol, actual_types, actual_params, actual_names);

  FnSymbol* best = NULL;
  Vec<ArgSymbol*>* actual_formals = 0;
  for (int i = 0; i < candidateFns.n; i++) {
    if (candidateFns.v[i].key) {
      best = candidateFns.v[i].key;
      actual_formals = candidateFns.v[i].value;
      for (int j = 0; j < candidateFns.n; j++) {
        if (i != j && candidateFns.v[j].key) {
          Vec<ArgSymbol*>* actual_formals2 = candidateFns.v[j].value;
          for (int k = 0; k < actual_formals->n; k++) {
            ArgSymbol* arg = actual_formals->v[k];
            ArgSymbol* arg2 = actual_formals2->v[k];
            if (arg->intent != INTENT_TYPE && arg2->intent != INTENT_TYPE) {
              if (can_dispatch_ne(NULL, arg2->type, arg->type)) {
                best = NULL;
                break;
              }
            }
          }
          if (!best)
            break;
        }
      }
      if (best)
        break;
    }
  }

  if (!best && candidateFns.n > 0) {
    resolve_call_error = CALL_AMBIGUOUS;
    return NULL;
  }

  if (partialTag == PARTIAL_OK && (!best || !best->noParens)) {
    resolve_call_error = CALL_PARTIAL;
    return NULL;
  }

  if (!best) {
    resolve_call_error = CALL_UNKNOWN;
    return NULL;
  }

  best = build_default_wrapper(best, actual_formals);
  best = build_order_wrapper(best, actual_formals);
  // need to implement build coercion wrapper
  return best;
}

void resolve_for_loop(CallExpr* call) {
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
}

void resolve_asts(BaseAST* base) {
  Vec<BaseAST*> asts;
  collect_asts_postorder(&asts, base);
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
      if (!call->baseExpr)
        resolve_op(call);
      else {
        if (CallExpr* partial = dynamic_cast<CallExpr*>(call->baseExpr)) {
          if (partial->typeInfo() == dtUnknown) {
            call->baseExpr->replace(partial->baseExpr->copy());
            call->argList->insertAtHead(partial->argList->copy());
          }
        }
        SymExpr* base = dynamic_cast<SymExpr*>(call->baseExpr);
        if (!base ||
            dynamic_cast<VarSymbol*>(base->var) ||
            dynamic_cast<ArgSymbol*>(base->var)) {
          Expr* baseExpr = call->baseExpr;
          call->baseExpr->replace(new SymExpr("this"));
          call->argList->insertAtHead(baseExpr->copy());
        }
        Vec<Type*> actual_types;
        Vec<Symbol*> actual_params;
        Vec<char*> actual_names;
        for_alist(Expr, actual, call->argList) {
          actual_types.add(actual->typeInfo());
          SymExpr* symExpr;
          if (NamedExpr* named = dynamic_cast<NamedExpr*>(actual)) {
            actual_names.add(named->name);
            symExpr = dynamic_cast<SymExpr*>(named->actual);
          } else {
            actual_names.add(NULL);
            symExpr = dynamic_cast<SymExpr*>(actual);
          }
          if (symExpr && symExpr->var->isParam()) {
            actual_params.add(symExpr->var);
          } else {
            actual_params.add(NULL);
          }
        }
        FnSymbol* fn = resolve_call(call, &actual_types,
                                    &actual_params, &actual_names);
        if (fn) {
          if (!call->isResolved())
            fn = clone_underspecified_function(fn, call);
          if (!call->isNamed("_chpl_alloc"))
            resolve_function(fn);
          call->baseExpr->replace(new SymExpr(fn));
          resolve_for_loop(call);
          if (fn->hasPragma("builtin"))
            call->makeOp();
        } else if (resolve_call_error != CALL_PARTIAL) {
          INT_FATAL(call, "Doh!");
        }
      }
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
  if (!no_infer || run_interpreter)
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
  if (call->isPrimitive(PRIMITIVE_EQUAL)) {
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
