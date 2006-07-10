/*** normalize
 ***
 *** This pass and function normalizes parsed and scope-resolved AST.
 ***/

#include <sys/time.h>
#include "astutil.h"
#include "expr.h"
#include "passes.h"
#include "runtime.h"
#include "stmt.h"
#include "symbol.h"
#include "stringutil.h"
#include "view.h"

bool normalized = false;

static void reconstruct_iterator(FnSymbol* fn);
static void build_lvalue_function(FnSymbol* fn);
static void normalize_returns(FnSymbol* fn);
static void initialize_out_formals(FnSymbol* fn);
static void insert_formal_temps(FnSymbol* fn);
static void call_constructor_for_class(CallExpr* call);
static void decompose_special_calls(CallExpr* call);
static void hack_seqcat_call(CallExpr* call);
static void hack_resolve_types(Expr* expr);
static void apply_getters_setters(FnSymbol* fn);
static void insert_call_temps(CallExpr* call);
static void fix_user_assign(CallExpr* call);
static void fix_def_expr(DefExpr* def);
static void fold_params(BaseAST* base);
static void expand_var_args(FnSymbol* fn);
static int tag_generic(FnSymbol* fn);
static int tag_generic(Type* fn);
static void tag_hasVarArgs(FnSymbol* fn);


void normalize(void) {
  forv_Vec(ModuleSymbol, mod, allModules) {
    normalize(mod);
  }
  normalized = true;
}

void normalize(BaseAST* base) {
  Vec<BaseAST*> asts;

  asts.clear();
  collect_asts_postorder(&asts, base);
  forv_Vec(BaseAST, ast, asts) {
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(ast)) {
      currentLineno = fn->lineno;
      currentFilename = fn->filename;
      if (fn->fnClass == FN_ITERATOR)
        reconstruct_iterator(fn);
      if (fn->retRef)
        build_lvalue_function(fn);
    }
  }

  asts.clear();
  collect_asts(&asts, base);
  forv_Vec(BaseAST, ast, asts) {
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(ast)) {
      normalize_returns(fn);
      initialize_out_formals(fn);
      insert_formal_temps(fn);
    }
  }

  asts.clear();
  collect_asts_postorder(&asts, base);
  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (CallExpr* a = dynamic_cast<CallExpr*>(ast)) {
      call_constructor_for_class(a);
    }
  }

  asts.clear();
  collect_asts_postorder(&asts, base);
  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (CallExpr* a = dynamic_cast<CallExpr*>(ast)) {
      decompose_special_calls(a);
    }
  }

  asts.clear();
  collect_asts_postorder(&asts, base);
  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (CallExpr* a = dynamic_cast<CallExpr*>(ast)) {
      hack_seqcat_call(a);
    } else if (Expr* a = dynamic_cast<Expr*>(ast)) {
      hack_resolve_types(a);
    }
  }

  asts.clear();
  collect_asts_postorder(&asts, base);
  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (Expr* a = dynamic_cast<Expr*>(ast)) {
      hack_resolve_types(a);
    }
  }

  asts.clear();
  collect_asts_postorder(&asts, base);
  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (FnSymbol* a = dynamic_cast<FnSymbol*>(ast)) {
      if (!(a->_setter || a->_getter))
        apply_getters_setters(a);
    }
  }

  asts.clear();
  collect_asts_postorder(&asts, base);
  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (DefExpr* a = dynamic_cast<DefExpr*>(ast)) {
      if (dynamic_cast<VarSymbol*>(a->sym))
        if (dynamic_cast<FnSymbol*>(a->parentSymbol))
          fix_def_expr(a);
    }
  }

  asts.clear();
  collect_asts_postorder(&asts, base);
  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (CallExpr* a = dynamic_cast<CallExpr*>(ast)) {
      insert_call_temps(a);
    }
  }

  asts.clear();
  collect_asts_postorder(&asts, base);
  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (CallExpr* a = dynamic_cast<CallExpr*>(ast)) {
      if (a->isNamed("="))
        fix_user_assign(a);
    }
  }

  asts.clear();
  collect_asts_postorder(&asts, base);
  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (DefExpr* a = dynamic_cast<DefExpr*>(ast)) {
      if (dynamic_cast<VarSymbol*>(a->sym) &&
          dynamic_cast<TypeSymbol*>(a->parentSymbol) &&
          a->exprType)
        a->exprType->remove();
    }
  }

  fold_params(base);

  asts.clear();
  collect_asts(&asts, base);
  forv_Vec(BaseAST, ast, asts) {
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(ast)) {
      expand_var_args(fn);
    }
  }

  dtAny->isGeneric = true;
  asts.clear();
  collect_asts_postorder(&asts, base);
  int changed = 1;
  while (changed) {
    changed = 0;
    forv_Vec(BaseAST, ast, asts) {
      if (FnSymbol *fn = dynamic_cast<FnSymbol*>(ast)) 
        changed = tag_generic(fn) || changed;
      if (DefExpr* a = dynamic_cast<DefExpr*>(ast)) {
        if (TypeSymbol *ts = dynamic_cast<TypeSymbol*>(a->sym))
          changed = tag_generic(ts->definition) || changed;
      }
    }
  }

  asts.clear();
  collect_asts_postorder(&asts, base);
  forv_Vec(BaseAST, ast, asts) {
    if (FnSymbol *fn = dynamic_cast<FnSymbol*>(ast)) {
      tag_hasVarArgs(fn);
    }
  }

  asts.clear();
  collect_asts_postorder(&asts, base);
  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (Expr* a = dynamic_cast<Expr*>(ast)) {
      hack_resolve_types(a);
    }
  }
}


static void reconstruct_iterator(FnSymbol* fn) {
  Expr* seqType = fn->retExpr;
  if (!seqType)
    USR_FATAL(fn, "Cannot infer iterator return type yet");
  Type *seqElementType = seqType->typeInfo();
  if (MetaType* mt = dynamic_cast<MetaType*>(seqElementType))
    seqElementType = mt->base;
  fn->retExpr->remove();

  Symbol* seq = new VarSymbol("_seq_result");
  DefExpr* def = new DefExpr(seq, NULL, new CallExpr(chpl_seq, seqType));

  fn->insertAtHead(def);

  Vec<BaseAST*> asts;
  collect_asts_postorder(&asts, fn->body);
  forv_Vec(BaseAST, ast, asts) {
    if (ReturnStmt* returnStmt = dynamic_cast<ReturnStmt*>(ast)) {
      Expr* expr = returnStmt->expr;
      returnStmt->expr->replace(new SymExpr(seq));
      returnStmt->insertBefore(
        new CallExpr(new CallExpr(".", seq, new_StringSymbol("_yield")), expr));
      if (returnStmt->yield)
        returnStmt->remove();
    }
  }
  fn->insertAtTail(new ReturnStmt(seq));
  fn->retType = dtUnknown;
  fn->fnClass = FN_FUNCTION;

  if (scalar_promotion) {
    if (!strcmp("_promoter", fn->name)) {
      if (seqElementType != dtUnknown) {
        if (is_Value_Type(seqElementType))
          fn->_this->type->scalarPromotionType = seqElementType;
      } else {
        if (CallExpr *c = dynamic_cast<CallExpr*>(seqType)) {
          if (SymExpr *b = dynamic_cast<SymExpr*>(c->baseExpr)) {
            if (!strcmp(".", b->var->name) && c->argList->length() == 2) {
              if (SymExpr *a1 = dynamic_cast<SymExpr*>(c->argList->get(1))) {
                if (a1->var == fn->_this) {
                  if (SymExpr *a2 = dynamic_cast<SymExpr*>(c->argList->get(2))) {
                    if (VarSymbol *vs = dynamic_cast<VarSymbol*>(a2->var)) {
                      if (vs->immediate) {
                        char *s = vs->immediate->v_string;
                        ClassType *ct = dynamic_cast<ClassType*>(fn->_this->type);
                        forv_Vec(TypeSymbol, ts, ct->types)
                          if (!strcmp(ts->name, s))
                            ts->addPragma("promoter");
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
}


static void build_lvalue_function(FnSymbol* fn) {
  FnSymbol* new_fn = fn->copy();
  fn->defPoint->parentStmt->insertAfter(new DefExpr(new_fn));
  if (fn->_this)
    fn->_this->type->methods.add(new_fn);
  new_fn->retRef = false;
  fn->retRef = false;
  new_fn->retType = dtVoid;
  if (new_fn->retExpr) new_fn->retExpr->remove();
  new_fn->cname = stringcat("_setter_", fn->cname);
  ArgSymbol* setterToken = new ArgSymbol(INTENT_BLANK, "_setterTokenDummy",
                                         dtSetterToken);
  ArgSymbol* lvalue = new ArgSymbol(INTENT_BLANK, "_lvalue", fn->retType);
  if (lvalue->type == dtUnknown)
    lvalue->type = dtAny;
  new_fn->formals->insertAtTail(new DefExpr(setterToken));
  new_fn->formals->insertAtTail(new DefExpr(lvalue));
  Vec<BaseAST*> asts;
  collect_asts_postorder(&asts, new_fn->body);
  forv_Vec(BaseAST, ast, asts) {
    if (ReturnStmt* returnStmt = dynamic_cast<ReturnStmt*>(ast)) {
      if (returnStmt->parentSymbol == new_fn) {
        Expr* expr = returnStmt->expr;
        returnStmt->expr->replace(new SymExpr(gVoid));
        returnStmt->insertBefore(new CallExpr("=", expr, lvalue));
      }
    }
  }
}


static void normalize_returns(FnSymbol* fn) {
  Vec<BaseAST*> asts;
  Vec<ReturnStmt*> rets;
  collect_asts(&asts, fn);
  forv_Vec(BaseAST, ast, asts) {
    if (ReturnStmt* returnStmt = dynamic_cast<ReturnStmt*>(ast)) {
      if (returnStmt->parentSymbol == fn) // not in a nested function
        rets.add(returnStmt);
    }
  }
  if (rets.n == 0) {
    fn->insertAtTail(new ReturnStmt(gVoid));
    return;
  }
  if (rets.n == 1) {
    ReturnStmt* ret = rets.v[0];
    if (ret == fn->body->body->last() && dynamic_cast<SymExpr*>(ret->expr))
      return;
  }
  bool returns_void = rets.v[0]->returnsVoid();
  LabelSymbol* label = new LabelSymbol(stringcat("_end_", fn->name));
  fn->insertAtTail(new DefExpr(label));
  VarSymbol* retval = NULL;
  if (returns_void) {
    fn->insertAtTail(new ReturnStmt());
  } else {
    retval = new VarSymbol(stringcat("_ret_", fn->name), fn->retType);
    Expr* type = (fn->retExpr) ? fn->retExpr->copy() : NULL;
    fn->insertAtHead(new DefExpr(retval, NULL, type));
    fn->insertAtTail(new ReturnStmt(retval));
  }
  bool label_is_used = false;
  forv_Vec(ReturnStmt, ret, rets) {
    if (retval) {
      Expr* ret_expr = ret->expr;
      ret_expr->remove();
      ret->insertBefore(new CallExpr(PRIMITIVE_MOVE, retval, ret_expr));
    }
    if (ret->next != label->defPoint->parentStmt) {
      ret->replace(new GotoStmt(goto_normal, label));
      label_is_used = true;
    } else {
      ret->remove();
    }
  }
  if (!label_is_used)
    label->defPoint->parentStmt->remove();
}


static void initialize_out_formals(FnSymbol* fn) {
  for_formals(arg, fn) {
    if (arg->intent == INTENT_OUT) {
      if (arg->defaultExpr)
        fn->insertAtHead(new CallExpr(PRIMITIVE_MOVE, arg, arg->defaultExpr->copy()));
      else
        fn->insertAtHead(new CallExpr(PRIMITIVE_MOVE, arg, new CallExpr("_init", arg)));
    }
    if (arg->intent == INTENT_OUT || arg->intent == INTENT_INOUT)
      arg->intent = INTENT_REF;
  }
}


static void insert_formal_temps(FnSymbol* fn) {
  if (!formalTemps)
    return;

  if (!strcmp("=", fn->name))
    return;

  if (fn->getModule() == prelude)
    return;

  Vec<DefExpr*> tempDefs;
  ASTMap subs;

  for_alist_backward(DefExpr, formalDef, fn->formals) {
    ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
    if (formal->intent == INTENT_REF ||
        formal->intent == INTENT_PARAM ||
        formal->intent == INTENT_TYPE)
      continue;
    VarSymbol* temp = new VarSymbol(stringcat("_", formal->name));
    DefExpr* tempDef = new DefExpr(temp, new SymExpr(formal));
    tempDefs.add(tempDef);
    subs.put(formal, temp);
  }

  update_symbols(fn->body, &subs);

  forv_Vec(DefExpr, tempDef, tempDefs) {
    fn->insertAtHead(tempDef);
  }
}


static void call_constructor_for_class(CallExpr* call) {
  if (SymExpr* baseVar = dynamic_cast<SymExpr*>(call->baseExpr)) {
    if (TypeSymbol* ts = dynamic_cast<TypeSymbol*>(baseVar->var)) {
      if (ClassType* ct = dynamic_cast<ClassType*>(ts->definition)) {
        if (ct->defaultConstructor)
          call->baseExpr->replace(new SymExpr(ct->defaultConstructor->name));
        else
          INT_FATAL(call, "class type has no default constructor");
      }
    }
  }
}


static void
decompose_multi_actuals(CallExpr* call, char* new_name, Expr* first_actual) {
  for_alist(Expr, actual, call->argList) {
    actual->remove();
    call->parentStmt->insertBefore
      (new CallExpr(new_name, first_actual->copy(), actual));
  }
  call->parentStmt->remove();
}


static void decompose_special_calls(CallExpr* call) {
  if (call->isResolved())
    return;
  if (!call->argList->isEmpty() > 0) {
    Expr* firstArg = call->argList->get(1);
    SymExpr* symArg = dynamic_cast<SymExpr*>(firstArg);
    // don't decompose method calls
    if (symArg && symArg->var == methodToken)
      return;
  }
  if (call->isNamed("fread")) {
    Expr* file = call->argList->get(1);
    file->remove();
    decompose_multi_actuals(call, "fread", file);
    call->parentStmt->remove();
  } else if (call->isNamed("read")) {
    decompose_multi_actuals(call, "fread", new SymExpr(chpl_stdin));
  }
}


static void hack_seqcat_call(CallExpr* call) {
  if (call->isNamed("#")) {
    Type* leftType = call->get(1)->typeInfo();
    Type* rightType = call->get(2)->typeInfo();

    // assume dtUnknown may be sequence type, at least one should be
    if (leftType != dtUnknown && rightType != dtUnknown)
      INT_FATAL(call, "Bad # operation");

    // if only one is, change to append or prepend
    if (leftType != dtUnknown) {
      call->replace(new CallExpr(
                      new CallExpr(".", call->get(2)->copy(), 
                                   new_StringSymbol("_prepend")),
                      call->get(1)->copy()));
    } else if (rightType != dtUnknown) {
      call->replace(new CallExpr(
                      new CallExpr(".", call->get(1)->copy(), 
                                   new_StringSymbol("_append")),
                      call->get(2)->copy()));
    }
  }
}


static bool can_resolve_type(Expr* type_expr) {
  if (!type_expr)
    return false;
  Type* type = type_expr->typeInfo();
  return type && type != dtUnknown; // && type != dtNil;
}


static void hack_resolve_types(Expr* expr) {
  if (DefExpr* def = dynamic_cast<DefExpr*>(expr)) {
    if (ArgSymbol* arg = dynamic_cast<ArgSymbol*>(def->sym)) {
      if (arg->intent == INTENT_TYPE && can_resolve_type(def->exprType)) {
        arg->type = getMetaType(def->exprType->typeInfo());
        def->exprType->remove();
      } else if (arg->type == dtUnknown && can_resolve_type(def->exprType)) {
        arg->type = def->exprType->typeInfo();
        if (MetaType* mt = dynamic_cast<MetaType*>(arg->type))
          arg->type = mt->base;
        def->exprType->remove();
      } else if (arg->type == dtUnknown && !def->exprType && can_resolve_type(arg->defaultExpr)) {
        Type *t = arg->defaultExpr->typeInfo();
        if (t != dtNil)
          arg->type = t;
      }
    }
  }
}


static void apply_getters_setters(FnSymbol* fn) {
  // Most generally:
  //   x.f(a) = y --> f(_mt, x)(a, _st, y)
  // which is the same as
  //   call(= call(call(. x "f") a) y) --> call(call(f _mt x) a _st y)
  // Also:
  //   x.f = y --> f(_mt, x, _st, y)
  //   f(a) = y --> f(a, _st, y)
  //   x.f --> f(_mt, x)
  //   x.f(a) --> f(_mt, x)(a)
  // Note:
  //   call(call or )( indicates PARTIAL_OK
  Vec<BaseAST*> asts;
  collect_asts_postorder(&asts, fn);
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
      if (call->getFunction() != fn) // in a nested function, handle
                                     // later, because it may be a
                                     // getter or a setter
        continue;
      if (call->isNamed(".")) { // handle getter
        if (CallExpr* parent = dynamic_cast<CallExpr*>(call->parentExpr))
          if (parent->isNamed("="))
            if (parent->get(1) == call)
              continue; // handle setter below
        char* method = NULL;
        if (SymExpr* symExpr = dynamic_cast<SymExpr*>(call->get(2)))
          if (VarSymbol* var = dynamic_cast<VarSymbol*>(symExpr->var))
            if (var->immediate->const_kind == IF1_CONST_KIND_STRING)
              method = var->immediate->v_string;
        if (!method)
          INT_FATAL(call, "No method name for getter or setter");
        Expr* _this = call->get(1);
        _this->remove();
        CallExpr* getter = new CallExpr(method, methodToken, _this);
        getter->methodTag = true;
        call->replace(getter);
        if (CallExpr* parent = dynamic_cast<CallExpr*>(getter->parentExpr))
          if (parent->baseExpr == getter)
            getter->partialTag = PARTIAL_OK;
      } else if (call->isNamed("=")) {
        if (CallExpr* lhs = dynamic_cast<CallExpr*>(call->get(1))) {
          if (lhs->isNamed(".")) {
            char* method = NULL;
            if (SymExpr* symExpr = dynamic_cast<SymExpr*>(lhs->get(2)))
              if (VarSymbol* var = dynamic_cast<VarSymbol*>(symExpr->var))
                if (var->immediate->const_kind == IF1_CONST_KIND_STRING)
                  method = var->immediate->v_string;
            if (!method)
              INT_FATAL(call, "No method name for getter or setter");
            Expr* _this = lhs->get(1);
            _this->remove();
            Expr* rhs = call->get(2);
            rhs->remove();
            CallExpr* setter =
              new CallExpr(method, methodToken, _this, setterToken, rhs);
            call->replace(setter);
          } else {
            Expr* rhs = call->get(2);
            rhs->remove();
            lhs->remove();
            call->replace(lhs);
            lhs->insertAtTail(setterToken);
            lhs->insertAtTail(rhs);
          }
        }
      }
    }
  }
}


static void insert_call_temps(CallExpr* call) {
  static int uid = 1;

  if (!call->parentExpr || !call->parentStmt)
    return;
  
  if (dynamic_cast<DefExpr*>(call->parentExpr))
    return;

  if (call->partialTag == PARTIAL_OK)
    return;

  if (call->primitive)
    return;

  if (CallExpr* parentCall = dynamic_cast<CallExpr*>(call->parentExpr)) {
    if (parentCall->isPrimitive(PRIMITIVE_MOVE))
      return;
    if (parentCall->isNamed("_init"))
      call = parentCall;
  }

  Stmt* stmt = call->parentStmt;
  VarSymbol* tmp = new VarSymbol("_tmp", dtUnknown, VAR_NORMAL, VAR_CONST);
  tmp->cname = stringcat(tmp->name, intstring(uid++));
  call->replace(new SymExpr(tmp));
  stmt->insertBefore(new DefExpr(tmp));
  stmt->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, call));
}


static void fix_user_assign(CallExpr* call) {
  if (call->parentExpr)
    return;
  CallExpr* move = new CallExpr(PRIMITIVE_MOVE, call->get(1)->copy());
  call->replace(move);
  move->insertAtTail(call);
}


static void fix_def_expr(DefExpr* def) {
  if (def->exprType) {
    bool ignore_type = false;
    if (def->init) {

      // ignore type on parameters if it has an init to avoid a double
      // assignment for now.  this is a hack.  we need to handle the
      // double assignment in the case of "param x : float = 1" where
      // the integer literal is coerced to a float.
      if (VarSymbol* var = dynamic_cast<VarSymbol*>(def->sym))
        if (var->consClass == VAR_PARAM)
          ignore_type = true;

      if (ignore_type)
        def->parentStmt->insertAfter(new CallExpr(PRIMITIVE_MOVE, def->sym, def->init->remove()));
      else
        def->parentStmt->insertAfter(new CallExpr(PRIMITIVE_MOVE, def->sym, new CallExpr("=", def->sym, def->init->remove())));
    }
    if (!ignore_type)
      def->parentStmt->insertAfter(new CallExpr(PRIMITIVE_MOVE, def->sym, new CallExpr("_init", def->exprType->remove())));
    else
      def->exprType->remove();
  } else if (def->init) {
    def->parentStmt->insertAfter(new CallExpr(PRIMITIVE_MOVE, def->sym, new CallExpr("_copy", def->init->remove())));
  }
}


#define FOLD_CALL(name, prim)                             \
  if (call->isNamed(name)) {                              \
    Immediate i3;                                         \
    fold_constant(prim, i1, i2, &i3);                     \
    call->replace(new SymExpr(new_ImmediateSymbol(&i3))); \
    return;                                               \
  }

#define FIND_PRIMITIVE_TYPE( prim_type, numoftypes, ptype_p)            \
  for (int type=0; type<numoftypes; type++) {                           \
    if (prim_type[type] &&                                              \
        (prim_type[type]->symbol == (TypeSymbol*) base->var)) {         \
      ptype_p = prim_type;                                              \
      break;                                                            \
    }                                                                   \
  }


static void fold_call_expr(CallExpr* call) {
  if (call->partialTag == PARTIAL_ALWAYS)
    return;
  if (call->isNamed("_copy")) {
    if (call->argList->length() == 1) {
      if (SymExpr* symExpr = dynamic_cast<SymExpr*>(call->get(1))) {
        if (VarSymbol* var = dynamic_cast<VarSymbol*>(symExpr->var)) {
          if (var->immediate) {
            call->replace(new SymExpr(var));
          }
        }
      }
    }
  }
  if (call->isNamed("_init")) {
    if (CallExpr* construct = dynamic_cast<CallExpr*>(call->get(1))) {
      if (SymExpr* base = dynamic_cast<SymExpr*>(construct->baseExpr)) {
        Symbol* sym = call->lookup(base);
        if (FnSymbol* fn = dynamic_cast<FnSymbol*>(sym)) {
          if (fn->fnClass == FN_CONSTRUCTOR) {
            if (ClassType* ct = dynamic_cast<ClassType*>(fn->retType)) {
              if (ct->classTag == CLASS_CLASS)
                call->replace(new CallExpr(PRIMITIVE_CAST, construct->remove(), gNil));
              else
                call->replace(construct->remove());
            }
          } else if (construct->isNamed("_build_array_type") ||
                     construct->isNamed("_build_sparse_domain_type") ||
                     construct->isNamed("_build_domain_type")) {
            call->replace(construct->remove());
          }
        }
      }
    } else if (SymExpr* sym = dynamic_cast<SymExpr*>(call->get(1))) {
      if (TypeSymbol* ts = dynamic_cast<TypeSymbol*>(sym->var)) {
        if (ts->definition->defaultValue)
          call->replace(new CallExpr(PRIMITIVE_CAST, ts, ts->definition->defaultValue));
        else if (ts->definition->defaultConstructor)
          call->replace(new CallExpr(ts->definition->defaultConstructor));
        else if (!dynamic_cast<VariableType*>(ts->definition))
          INT_FATAL(ts, "type has neither defaultValue nor defaultConstructor");
      }
    }
    return;
  }

  if (call->isNamed("_construct__tuple")) {
    if (SymExpr* sym = dynamic_cast<SymExpr*>(call->get(1))) {
      if (VarSymbol* var = dynamic_cast<VarSymbol*>(sym->var)) {
        if (var->immediate) {
          int rank = var->immediate->v_int64;
          if (rank != call->argList->length() - 1) {
            if (call->argList->length() != 2)
              INT_FATAL(call, "bad homogeneous tuple");
            Expr* expr = call->get(2);
            for (int i = 1; i < rank; i++)
              call->insertAtTail(expr->copy());
          }
        }
      }
    }
  }

  // fold parameter methods
  if (call->argList->length() == 2) {
    if (SymExpr* symExpr = dynamic_cast<SymExpr*>(call->get(1))) {
      if (symExpr->var == methodToken) {
        Type* type = call->get(2)->typeInfo();
        Vec<BaseAST*> keys;
        type->substitutions.get_keys(keys);
        forv_Vec(BaseAST, key, keys) {
          if (Symbol* var = dynamic_cast<Symbol*>(key)) {
            if (call->isNamed(var->name)) {
              if (Symbol* value = dynamic_cast<Symbol*>(type->substitutions.get(key))) {
                call->replace(new SymExpr(value));
              }
            }
          } else if (Type* var = dynamic_cast<Type*>(key)) {
            if (call->isNamed(var->symbol->name)) {
              if (Type* value = dynamic_cast<Type*>(type->substitutions.get(key))) {
                call->replace(new SymExpr(value->symbol));
              }
            }
          }
        }
      }
    }
  }

  if (call->argList->length() == 2) {
    if (call->get(1)->typeInfo() == dtString) // folding not handling strings yet
      return;
    SymExpr* a1 = dynamic_cast<SymExpr*>(call->get(1));
    SymExpr* a2 = dynamic_cast<SymExpr*>(call->get(2));
    if (a1 && a2) {
      VarSymbol* v1 = dynamic_cast<VarSymbol*>(a1->var);
      VarSymbol* v2 = dynamic_cast<VarSymbol*>(a2->var);
      if (v1 && v2) {
        Immediate* i1 = v1->immediate;
        Immediate* i2 = v2->immediate;
        if (i1 && i2) {
          FOLD_CALL("+", P_prim_add);
          FOLD_CALL("-", P_prim_subtract);
          FOLD_CALL("*", P_prim_mult);
          FOLD_CALL("/", P_prim_div);
          FOLD_CALL("&", P_prim_and);
          FOLD_CALL("|", P_prim_or);
          FOLD_CALL("^", P_prim_xor);
          FOLD_CALL("%", P_prim_mod);
          FOLD_CALL("<", P_prim_less);
          FOLD_CALL("<=", P_prim_lessorequal);
          FOLD_CALL(">", P_prim_greater);
          FOLD_CALL(">=", P_prim_greaterorequal);
          FOLD_CALL("==", P_prim_equal);
          FOLD_CALL("!=", P_prim_notequal);
          FOLD_CALL("&&", P_prim_land);
          FOLD_CALL("||", P_prim_lor);
          if (call->isNamed("=")) {
            call->replace(new SymExpr(v2));
            return;
          }
        }
      }
    }
  }

  if (call->argList->length() == 1) {
    SymExpr* a1 = dynamic_cast<SymExpr*>(call->get(1));
    if (a1) {
      VarSymbol* v1 = dynamic_cast<VarSymbol*>(a1->var);
      if (v1) {
        Immediate* i1 = v1->immediate;
        Immediate* i2 = NULL;
        if (i1) {
          FOLD_CALL("+", P_prim_plus);
          FOLD_CALL("-", P_prim_minus);
          FOLD_CALL("~", P_prim_not);
          FOLD_CALL("!", P_prim_lnot);
        }

        /* look for 8? */
      }
    }
  }


  // replace call expr of primitive type with appropriate primitive type
  if (call->argList->length() == 1) {
    if (SymExpr* base = dynamic_cast<SymExpr*>(call->baseExpr)) {
      PrimitiveType **ptype_p = NULL;

      FIND_PRIMITIVE_TYPE( dtInt, INT_TYPE_NUM, ptype_p);
      if (!ptype_p) {
        FIND_PRIMITIVE_TYPE( dtUInt, INT_TYPE_NUM, ptype_p);
        if (!ptype_p) {
          FIND_PRIMITIVE_TYPE( dtFloat, FLOAT_TYPE_NUM, ptype_p);
        }
      }

      if (ptype_p){
        if (SymExpr* arg1 = dynamic_cast<SymExpr*>(call->get(1))) {
          if (VarSymbol* v1 = dynamic_cast<VarSymbol*>(arg1->var)) {
            if (Immediate* imm = v1->immediate) {
              if ((NUM_KIND_INT == imm->const_kind) ||
                  (NUM_KIND_UINT == imm->const_kind)) {
                TypeSymbol *tsize;
                int         size;
                if (NUM_KIND_INT == imm->const_kind) {
                  size = imm->int_value ();
                } else {
                  size = (int) imm->uint_value ();
                }

                if (ptype_p == dtInt) {
                  switch (size) {
                  case  8: tsize = dtInt[INT_TYPE_8]->symbol;  break;
                  case 16: tsize = dtInt[INT_TYPE_16]->symbol; break;
                  case 32: tsize = dtInt[INT_TYPE_32]->symbol; break;
                  case 64: tsize = dtInt[INT_TYPE_64]->symbol; break;
                  default:
                    USR_FATAL( call, "illegal size %d for int", size);
                  }
                  call->replace( new SymExpr(tsize));
                } else if (ptype_p == dtUInt) {
                  switch (size) {
                  case  8: tsize = dtUInt[INT_TYPE_8]->symbol;  break;
                  case 16: tsize = dtUInt[INT_TYPE_16]->symbol; break;
                  case 32: tsize = dtUInt[INT_TYPE_32]->symbol; break;
                  case 64: tsize = dtUInt[INT_TYPE_64]->symbol; break;
                  default:
                    USR_FATAL( call, "illegal size %d for uint", size);
                  }
                  call->replace( new SymExpr(tsize));
                } else if (ptype_p == dtFloat) {
                  switch (size) {
                  case 32:  tsize = dtFloat[FLOAT_TYPE_32]->symbol;  break;
                  case 64:  tsize = dtFloat[FLOAT_TYPE_64]->symbol;  break;
                  case 128: tsize = dtFloat[FLOAT_TYPE_128]->symbol; break;
                  default:
                    USR_FATAL( call, "illegal size %d for float", size);
                  }
                  call->replace( new SymExpr(tsize));
                }
              }
            }
          }
        }
      }
    }
  }
}

static bool fold_def_expr(DefExpr* def) {
  Symbol* value = NULL;
  CallExpr* move = NULL;
  if (def->sym->isParam() || def->sym->isConst()) {
    forv_Vec(SymExpr*, sym, *def->sym->uses) {
      if (CallExpr* call = dynamic_cast<CallExpr*>(sym->parentExpr)) {
        if (call->isPrimitive(PRIMITIVE_MOVE) && call->get(1) == sym) {
          if (SymExpr* val = dynamic_cast<SymExpr*>(call->get(2))) {
            if (VarSymbol* var = dynamic_cast<VarSymbol*>(val->var)) {
              if (var->immediate) {
                value = var;
                move = call;
              }
            } else if (TypeSymbol* type = dynamic_cast<TypeSymbol*>(val->var)) {
              value = type;
              move = call;
            }
          }
        }
        if (call->isNamed("=") && call->get(1) == sym) {
          if (SymExpr* val = dynamic_cast<SymExpr*>(call->get(2))) {
            if (VarSymbol* var = dynamic_cast<VarSymbol*>(val->var)) {
              if (var->immediate) {
                continue;
              }
            }
          }
          return false;
        }
      }
    }
  }
  if (value) {
    move->parentStmt->remove();
    forv_Vec(SymExpr*, sym, *def->sym->uses) {
      sym->var = value;
    }
    def->parentStmt->remove();
    return true;
  } else
    return false;
}

static void fold_cond_stmt(CondStmt* if_stmt) {
  if (SymExpr* cond = dynamic_cast<SymExpr*>(if_stmt->condExpr)) {
    if (!strcmp(cond->var->cname, "true")) {
      Stmt* then_stmt = if_stmt->thenStmt;
      then_stmt->remove();
      if_stmt->replace(then_stmt);
    } else if (!strcmp(cond->var->cname, "false")) {
      Stmt* else_stmt = if_stmt->elseStmt;
      if (else_stmt) {
        else_stmt->remove();
        if_stmt->replace(else_stmt);
      } else {
        if_stmt->remove();
      }
    }
  }
}

static void fold_param_for(BlockStmt* block) {
  if (block->param_low || block->param_high) {
    if (SymExpr* lse = dynamic_cast<SymExpr*>(block->param_low)) {
      if (SymExpr* hse = dynamic_cast<SymExpr*>(block->param_high)) {
        if (VarSymbol* lvar = dynamic_cast<VarSymbol*>(lse->var)) {
          if (VarSymbol* hvar = dynamic_cast<VarSymbol*>(hse->var)) {
            if (lvar->immediate && hvar->immediate) {
              int low = lvar->immediate->v_int64;
              int high = hvar->immediate->v_int64;
              block->param_low->remove();
              block->param_high->remove();
              Expr* index_expr = block->param_index;
              block->param_index->remove();
              block->blockTag = BLOCK_NORMAL;
              Symbol* index = dynamic_cast<SymExpr*>(index_expr)->var;
              for (int i = low; i <= high; i++) {
                VarSymbol* new_index = new VarSymbol(index->name);
                new_index->consClass = VAR_PARAM;
                block->insertBefore(new DefExpr(new_index, new_IntLiteral(i)));
                ASTMap map;
                map.put(index, new_index);
                block->insertBefore(block->copy(&map));
              }
              normalize(block->parentSymbol);
              block->remove();
            }
          }
        }
      }
    }
  }
}

static void fold_params(BaseAST* base) {
  Vec<BaseAST*> asts;
  collect_asts_postorder(&asts, base);
 
  Vec<DefExpr*> defs;
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* a = dynamic_cast<DefExpr*>(ast)) {
      defs.add(a);
    }
  }

  bool change;
  do {
    change = false;
    forv_Vec(BaseAST, ast, asts) {
      if (CallExpr* a = dynamic_cast<CallExpr*>(ast))
        if (a->parentSymbol) // in ast
          fold_call_expr(a);
      if (CondStmt* a = dynamic_cast<CondStmt*>(ast))
        if (a->parentSymbol) // in ast
          fold_cond_stmt(a);
      if (BlockStmt* a = dynamic_cast<BlockStmt*>(ast))
        if (a->parentSymbol) // in ast
          fold_param_for(a);
    }
    compute_sym_uses(base);
    forv_Vec(DefExpr, def, defs) {
      if (def->parentSymbol)
        change |= fold_def_expr(def);
    }
  } while (change);
}


static int max_actuals = 0;

static void
compute_max_actuals() {
  Vec<BaseAST*> asts;
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
      int num_actuals = call->argList->length();
      if (call->partialTag == PARTIAL_OK) {
        if (CallExpr* parent = dynamic_cast<CallExpr*>(call->parentExpr)) {
          num_actuals += parent->argList->length();
        }
      }
      if (num_actuals > max_actuals)
        max_actuals = num_actuals;
    }
  }
}

static void
expand_var_args(FnSymbol* fn) {
  for_alist(DefExpr, arg_def, fn->formals) {
    ArgSymbol* arg = dynamic_cast<ArgSymbol*>(arg_def->sym);
    if (DefExpr* def = dynamic_cast<DefExpr*>(arg->variableExpr)) {
      // recursively handle variable argument list where number of
      // variable arguments is one or more as in ...?k
      if (max_actuals == 0)
        compute_max_actuals();
      for (int i = 1; i <= max_actuals; i++) {
        arg->variableExpr->replace(new_IntLiteral(i));
        FnSymbol* new_fn = fn->copy();
        fn->defPoint->parentStmt->insertBefore(new DefExpr(new_fn));
        DefExpr* new_def = def->copy();
        new_def->init = new_IntLiteral(i);
        new_fn->insertAtHead(new_def);
        ASTMap update;
        update.put(def->sym, new_def->sym);
        update_symbols(new_fn, &update);
        expand_var_args(new_fn);
      }
      fn->defPoint->parentStmt->remove();
    } else if (SymExpr* sym = dynamic_cast<SymExpr*>(arg->variableExpr)) {
      // handle expansion of variable argument list where number of
      // variable arguments is a parameter
      if (VarSymbol* n_var = dynamic_cast<VarSymbol*>(sym->var)) {
        if (n_var->type == dtInt[INT_TYPE_64] && n_var->immediate) {
          int n = n_var->immediate->v_int64;
          AList<Expr>* actual_types = new AList<Expr>();
          AList<Expr>* actuals = new AList<Expr>();
          for (int i = 0; i < n; i++) {
            DefExpr* new_arg_def = arg_def->copy();
            ArgSymbol* new_arg = dynamic_cast<ArgSymbol*>(new_arg_def->sym);
            new_arg->variableExpr = NULL;
            actual_types->insertAtTail(new SymExpr(new_arg));
            actuals->insertAtTail(new SymExpr(new_arg));
            new_arg->name = stringcat("_e", intstring(i), "_", arg->name);
            new_arg->cname = stringcat("_e", intstring(i), "_", arg->cname);
            arg_def->insertBefore(new_arg_def);
          }
          VarSymbol* var = new VarSymbol(arg->name);
          fn->insertAtHead(new DefExpr(var, new CallExpr("_tuple", new_IntLiteral(n), actuals)));
          arg_def->remove();
          ASTMap update;
          update.put(arg, var);
          update_symbols(fn, &update);
          build(fn);
        }
      }
    }
  }
}


static int
genericFunctionArg(FnSymbol *f, Vec<Symbol *> &genericSymbols) {
  int result = 0;
  for (DefExpr* formal = f->formals->first(); formal; 
       formal = f->formals->next()) {
    if (ArgSymbol *ps = dynamic_cast<ArgSymbol *>(formal->sym)) {
      if (TypeSymbol *ts = dynamic_cast<TypeSymbol *>(ps->genericSymbol)) {
        if (ts->definition->isGeneric) {
          assert(dynamic_cast<VariableType*>(ts->definition));
          genericSymbols.set_add(ts);
          result = 1;
          continue;
        }
      }
      if (ps->type && ps->type->isGeneric) {
        genericSymbols.set_add(ps->type->symbol);
        result = 1;
        continue;
      }
      if (ps->intent == INTENT_PARAM) {
        genericSymbols.set_add(ps);
        result = 1;
        continue;
      }
    }
  }
  return result;
}

static int
tag_generic(FnSymbol *f) {
  int changed = 0;
  Vec<Symbol *> genericSymbols;
  if (genericFunctionArg(f, genericSymbols)) {
    changed = !f->isGeneric || changed;
    f->isGeneric = 1; 
    f->genericSymbols.copy(genericSymbols);
    f->genericSymbols.set_to_vec();
    qsort(f->genericSymbols.v, f->genericSymbols.n, sizeof(genericSymbols.v[0]), compar_baseast);
    if (f->retType != dtUnknown && f->fnClass == FN_CONSTRUCTOR)
      f->retType->isGeneric = true;
  }
  return changed;
}

static int
tag_generic(Type *t) {
  Vec<Symbol *> genericSymbols;
  if (ClassType *st = dynamic_cast<ClassType *>(t)) {
    forv_Vec(Symbol, s, st->fields) {
      VarSymbol *vs = dynamic_cast<VarSymbol *>(s);
      if (vs && vs->consClass == VAR_PARAM)
        genericSymbols.set_add(vs);
    }
    forv_Vec(TypeSymbol, s, st->types) {
      if (s->definition->astType == TYPE_VARIABLE) 
        genericSymbols.set_add(s->definition->symbol);
      else
        if (s->definition->isGeneric)
          genericSymbols.set_union(s->definition->genericSymbols);
    }    
  }
  if (genericSymbols.n) {
    genericSymbols.set_to_vec();
    qsort(genericSymbols.v, genericSymbols.n, sizeof(genericSymbols.v[0]), compar_baseast);
    t->isGeneric = 1;
    t->genericSymbols.move(genericSymbols);
  }
  return genericSymbols.n != 0;
}

static void
tag_hasVarArgs(FnSymbol* fn) {
  fn->hasVarArgs = false;
  if (fn->hasPragma("tuple"))
    if (fn->genericSymbols.n) // not if instantiated
      fn->hasVarArgs = true;
  for_formals(formal, fn)
    if (formal->variableExpr)
      fn->hasVarArgs = true;
}
