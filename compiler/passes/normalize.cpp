/*** normalize
 ***
 *** This pass and function normalizes parsed and scope-resolved AST.
 ***/

#include "astutil.h"
#include "expr.h"
#include "passes.h"
#include "runtime.h"
#include "runtime.h"
#include "stmt.h"
#include "symbol.h"
#include "symtab.h"
#include "stringutil.h"
#include "../traversals/inlineFunctions.h"


static void reconstruct_iterator(FnSymbol* fn);
static void build_lvalue_function(FnSymbol* fn);
static void normalize_returns(FnSymbol* fn);
static void insert_type_default_temp(UserType* userType);
static void initialize_out_formals(FnSymbol* fn);
static void insert_formal_temps(FnSymbol* fn);
static void call_constructor_for_class(CallExpr* call);
static void normalize_for_loop(ForLoopStmt* stmt);
static void decompose_special_calls(CallExpr* call);
static void hack_array_constructor_call(CallExpr* call);
static void hack_domain_constructor_call(CallExpr* call);
static void hack_seqcat_call(CallExpr* call);
static void hack_typeof_call(CallExpr* call);
static void hack_resolve_types(Expr* expr);
static void apply_getters_setters(BaseAST* ast);
static void insert_call_temps(CallExpr* call);
static void fix_def_expr(DefExpr* def);

void normalize(void) {
  Vec<FnSymbol*> fns;
  Vec<BaseAST*> asts;

  collect_functions(&fns);
  forv_Vec(FnSymbol, fn, fns) {
    currentLineno = fn->lineno;
    currentFilename = fn->filename;
    if (fn->fnClass == FN_ITERATOR)
      reconstruct_iterator(fn);
    if (fn->retRef)
      build_lvalue_function(fn);
    normalize_returns(fn);
    initialize_out_formals(fn);
    insert_formal_temps(fn);
  }

  asts.clear();
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (!use_alloc) {
      if (VarSymbol* vs = dynamic_cast<VarSymbol*>(ast)) {
        if (vs->type == dtUnknown && !vs->defPoint->exprType) {
          vs->noDefaultInit = true;
        }
      }
    }

    if (TypeSymbol* ts = dynamic_cast<TypeSymbol*>(ast)) {
      if (UserType* userType = dynamic_cast<UserType*>(ts->definition)) {
        insert_type_default_temp(userType);
      }
    }
  }

  asts.clear();
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (CallExpr* a = dynamic_cast<CallExpr*>(ast)) {
      call_constructor_for_class(a);
    } else if (ForLoopStmt* a = dynamic_cast<ForLoopStmt*>(ast)) {
      normalize_for_loop(a);
    }
  }

  asts.clear();
  collect_asts_postorder(&asts);
  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (CallExpr* a = dynamic_cast<CallExpr*>(ast)) {
      decompose_special_calls(a);
    }
  }

  asts.clear();
  collect_asts_postorder(&asts);
  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (CallExpr* a = dynamic_cast<CallExpr*>(ast)) {
      hack_array_constructor_call(a);
      hack_domain_constructor_call(a);
      hack_seqcat_call(a);
      hack_typeof_call(a);
    } else if (Expr* a = dynamic_cast<Expr*>(ast)) {
      hack_resolve_types(a);
    }
  }

  asts.clear();
  collect_asts_postorder(&asts);
  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (Expr* a = dynamic_cast<Expr*>(ast)) {
      hack_resolve_types(a);
    }
  }

  asts.clear();
  collect_asts_postorder(&asts);
  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (FnSymbol* a = dynamic_cast<FnSymbol*>(ast)) {
      if (!(a->_setter || a->_getter ||
            (!no_infer && a->fnClass == FN_CONSTRUCTOR)))
        apply_getters_setters(a);
    }
  }

  asts.clear();
  collect_asts_postorder(&asts);
  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (CallExpr* a = dynamic_cast<CallExpr*>(ast)) {
      insert_call_temps(a);
    }
  }

  if (use_alloc) {
    asts.clear();
    collect_asts_postorder(&asts);
    forv_Vec(BaseAST, ast, asts) {
      currentLineno = ast->lineno;
      currentFilename = ast->filename;
      if (DefExpr* a = dynamic_cast<DefExpr*>(ast)) {
        if (dynamic_cast<VarSymbol*>(a->sym) &&
            dynamic_cast<FnSymbol*>(a->parentSymbol))
          fix_def_expr(a);
      }
    }
  }

  asts.clear();
  collect_asts_postorder(&asts);
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
}


static void reconstruct_iterator(FnSymbol* fn) {
  Expr* seqType = NULL;
  if (fn->retType != dtUnknown) {
    seqType = new SymExpr(fn->retType->symbol);
  } else if (fn->defPoint->exprType) {
    seqType = fn->defPoint->exprType;
    seqType->remove();
  } else {
    Vec<BaseAST*> asts;
    collect_asts(&asts, fn->body);
    forv_Vec(BaseAST, ast, asts) {
      if (ReturnStmt* returnStmt = dynamic_cast<ReturnStmt*>(ast))
        if (returnStmt->expr)
          if (!seqType)
            seqType = new CallExpr("typeof", returnStmt->expr->copy());
          else
            USR_FATAL(fn, "Unable to infer type of iterator");
    }
    if (!seqType)
      USR_FATAL(fn, "Unable to infer type of iterator");
  }

  Symbol* seq = new VarSymbol("_seq_result");
  DefExpr* def = new DefExpr(seq, NULL, new CallExpr(chpl_seq, seqType));

  fn->insertAtHead(new ExprStmt(def));

  Vec<BaseAST*> asts;
  collect_asts_postorder(&asts, fn->body);
  forv_Vec(BaseAST, ast, asts) {
    if (ReturnStmt* returnStmt = dynamic_cast<ReturnStmt*>(ast)) {
      Expr* expr = returnStmt->expr;
      returnStmt->expr->replace(new SymExpr(seq));
      returnStmt->insertBefore(
        new ExprStmt(new CallExpr(new MemberAccess(seq, "_yield"), expr)));
      if (returnStmt->yield)
        returnStmt->remove();
    }
  }
  fn->insertAtTail(new ReturnStmt(seq));
  fn->retType = dtUnknown;
  if (fn->defPoint->exprType)
    fn->defPoint->exprType->replace(def->exprType->copy());
  else if (no_infer) {
    DefExpr* tmp = fn->defPoint;
    tmp->replace(new DefExpr(fn, NULL, def->exprType->copy()));
  }
}


static AList<Stmt>* handle_return_expr(Expr* e, Symbol* lvalue) {
  Stmt* newStmt = NULL;
  if (e)
    if (CondExpr *ce = dynamic_cast<CondExpr *>(e))
      newStmt = new CondStmt(ce->condExpr, 
                             handle_return_expr(ce->thenExpr, lvalue),
                             handle_return_expr(ce->elseExpr, lvalue));
    else
      newStmt = new ExprStmt(new CallExpr("=", e, lvalue));
  return new AList<Stmt>(newStmt);
}

static void build_lvalue_function(FnSymbol* fn) {
  FnSymbol* new_fn = fn->copy();
  fn->defPoint->parentStmt->insertAfter(new ExprStmt(new DefExpr(new_fn)));
  if (fn->typeBinding)
    fn->typeBinding->definition->methods.add(new_fn);
  new_fn->retRef = false;
  new_fn->retType = dtVoid;
  new_fn->cname = stringcat("_setter_", fn->cname);
  ArgSymbol* setterToken = new ArgSymbol(INTENT_REF, "_setterTokenDummy",
                                         dtSetterToken);
  ArgSymbol* lvalue = new ArgSymbol(INTENT_BLANK, "_lvalue", fn->retType);
  new_fn->formals->insertAtTail(new DefExpr(setterToken));
  new_fn->formals->insertAtTail(new DefExpr(lvalue));
  Vec<BaseAST*> asts;
  collect_asts_postorder(&asts, new_fn->body);
  forv_Vec(BaseAST, ast, asts) {
    if (ReturnStmt* returnStmt = dynamic_cast<ReturnStmt*>(ast)) {
      Expr* expr = returnStmt->expr;
      returnStmt->expr->remove();
      returnStmt->insertBefore(handle_return_expr(expr, lvalue));
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
  if (rets.n == 0)
    return;
  if (rets.n == 1) {
    ReturnStmt* ret = rets.v[0];
    if (ret == fn->body->body->last() &&
        (!ret->expr ||
         dynamic_cast<SymExpr*>(ret->expr)))
      return;
  }
  bool returns_void = rets.v[0]->expr == NULL;
  LabelSymbol* label = new LabelSymbol(stringcat("_end_", fn->name));
  fn->insertAtTail(new LabelStmt(label));
  VarSymbol* retval = NULL;
  if (returns_void) {
    fn->insertAtTail(new ReturnStmt());
  } else {
    retval = new VarSymbol(stringcat("_ret_", fn->name), fn->retType);
    if (!use_alloc)
      retval->noDefaultInit = true;
    Expr* type = fn->defPoint->exprType;
    type->remove();
    if (!type)
      retval->noDefaultInit = true;
    fn->insertAtHead(new ExprStmt(new DefExpr(retval, NULL, type)));
    fn->insertAtTail(new ReturnStmt(retval));
  }
  bool label_is_used = false;
  forv_Vec(ReturnStmt, ret, rets) {
    if (retval) {
      Expr* ret_expr = ret->expr;
      ret->expr->remove();
      ret->insertBefore(new ExprStmt(new CallExpr(OP_MOVE, retval, ret_expr)));
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


static void insert_type_default_temp(UserType* userType) {
  TypeSymbol* sym = userType->symbol;
  if (userType->underlyingType == dtUnknown &&
      userType->typeExpr &&
      userType->typeExpr->typeInfo() != dtUnknown) {
    userType->underlyingType = userType->typeExpr->typeInfo();
    userType->typeExpr = NULL;
    if (userType->defaultExpr) {
      char* temp_name = stringcat("_init_", sym->name);
      Type* temp_type = userType;
      Expr *temp_init = userType->defaultExpr->copy();
      Symbol* parent_symbol = sym->defPoint->parentStmt->parentSymbol;
      Symbol* outer_symbol = sym;
      while (dynamic_cast<TypeSymbol*>(parent_symbol)) {
        parent_symbol = parent_symbol->defPoint->parentStmt->parentSymbol;
        outer_symbol = outer_symbol->defPoint->parentStmt->parentSymbol;
      }
      VarSymbol* temp = new VarSymbol(temp_name, temp_type);
      DefExpr* def = new DefExpr(temp, temp_init);
      if (ModuleSymbol* mod = dynamic_cast<ModuleSymbol*>(parent_symbol)) {
        mod->initFn->insertAtHead(new ExprStmt(def));
      } else {
        Stmt* insert_point = outer_symbol->defPoint->parentStmt;
        insert_point->insertBefore(new ExprStmt(def));
      }
      userType->defaultValue = temp;
      userType->defaultExpr = NULL;
      temp->noDefaultInit = true;
    } else if (userType->underlyingType->defaultConstructor) 
      userType->defaultConstructor = userType->underlyingType->defaultConstructor;
    else if (userType->underlyingType->defaultValue)
      userType->defaultValue = userType->underlyingType->defaultValue;
  }
}


static void initialize_out_formals(FnSymbol* fn) {
  for_alist(DefExpr, argDef, fn->formals) {
    ArgSymbol* arg = dynamic_cast<ArgSymbol*>(argDef->sym);
    if (arg->defaultExpr && arg->intent == INTENT_OUT)
      fn->body->insertAtHead(
        new ExprStmt(new CallExpr(OP_MOVE, arg, arg->defaultExpr->copy())));
  }
}


static void insert_formal_temps(FnSymbol* fn) {
  if (!formalTemps)
    return;

  if (!strcmp("=", fn->name))
    return;

  Vec<DefExpr*> tempDefs;
  ASTMap subs;

  for_alist_backward(DefExpr, formalDef, fn->formals) {
    ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
    if (formal->intent == INTENT_REF)
      continue;
    Type *type = formal->type;
    if (type == dtAny || type == dtNumeric)
      type = dtUnknown;
    if (formal->intent == INTENT_PARAM ||
        formal->intent == INTENT_TYPE ||
        formal->genericSymbol ||
        type->isGeneric)
      return;
    VarSymbol* temp = new VarSymbol(stringcat("_", formal->name), formal->type);
    DefExpr* tempDef = new DefExpr(temp, new SymExpr(formal));
    if (formalDef->exprType)
      tempDef->exprType = formalDef->exprType->copy();
    tempDefs.add(tempDef);
    subs.put(formal, temp);
  }

  update_symbols(fn->body, &subs);

  forv_Vec(DefExpr, tempDef, tempDefs) {
    fn->insertAtHead(new ExprStmt(tempDef));
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


static void normalize_for_loop(ForLoopStmt* stmt) {
  stmt->iterators->only()->replace(
    new CallExpr(
      new MemberAccess(
        stmt->iterators->only(),
        "_forall")));
  if (no_infer) {
    DefExpr* index = stmt->indices->only();
    Expr* type = stmt->iterators->only()->copy();
    type = new CallExpr(new MemberAccess(type, "_last"));
    type = new CallExpr(new MemberAccess(type, "_element"));
    if (!index->exprType)
      index->replace(new DefExpr(index->sym, NULL, type));
  }
}


static void
decompose_multi_actuals(CallExpr* call, char* new_name, Expr* first_actual) {
  for_alist(Expr, actual, call->argList) {
    actual->remove();
    call->parentStmt->insertBefore
      (new ExprStmt(new CallExpr(new_name, first_actual->copy(), actual)));
  }
  call->parentStmt->remove();
}


static void decompose_special_calls(CallExpr* call) {
  if (call->isNamed("assert")) {
    if (call->argList->length() != 1) {
      USR_FATAL(call->argList, "Assert takes exactly one "
                "expression; you've given it %d.", call->argList->length());
    }
    CallExpr* halt_call = new CallExpr("halt", new_StringLiteral(stringcat
      ("***Error:  Assertion at ",
       call->filename,
       ":",
       intstring(call->lineno),
       " failed***")));
    Expr* arg = call->argList->get(1);
    call->argList->get(1)->remove();
    Expr* assert_cond = new CallExpr("not", arg);
    BlockStmt* assert_body = new BlockStmt(new ExprStmt(halt_call));
    call->parentStmt->insertBefore(new CondStmt(assert_cond, assert_body));
    call->parentStmt->remove();
    decompose_special_calls(halt_call);
  } else if (call->isNamed("halt")) {
    call->parentStmt->insertAfter(new ExprStmt(new CallExpr("exit", new_IntLiteral(0))));
    call->parentStmt->insertAfter(new ExprStmt(new CallExpr("fwriteln", chpl_stdout)));
    decompose_multi_actuals(call, "fwrite", new SymExpr(chpl_stdout));
  } else if (call->isNamed("fread")) {
    Expr* file = call->argList->get(1);
    file->remove();
    decompose_multi_actuals(call, "fread", file);
    call->parentStmt->remove();
  } else if (call->isNamed("fwrite")) {
    Expr* file = call->argList->get(1);
    file->remove();
    decompose_multi_actuals(call, "fwrite", file);
  } else if (call->isNamed("fwriteln")) {
    Expr* file = call->argList->get(1);
    file->remove();
    call->parentStmt->insertAfter(new ExprStmt(new CallExpr("fwriteln", file)));
    decompose_multi_actuals(call, "fwrite", file);
  } else if (call->isNamed("read")) {
    decompose_multi_actuals(call, "fread", new SymExpr(chpl_stdin));
  } else if (call->isNamed("write")) {
    decompose_multi_actuals(call, "fwrite", new SymExpr(chpl_stdout));
  } else if (call->isNamed("writeln")) {
    call->parentStmt->insertAfter(new ExprStmt(new CallExpr("fwriteln", chpl_stdout)));
    decompose_multi_actuals(call, "fwrite", new SymExpr(chpl_stdout));
  }
}


static Expr* hack_rank(Expr* domain) {
  if (SymExpr* symExpr = dynamic_cast<SymExpr*>(domain)) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(symExpr->var->defPoint->exprType)) {
      if (call->isNamed("_construct__adomain")) {
        return call->argList->first()->copy();
      }
    }
  }
  INT_FATAL("Could not determine rank of array");
  return NULL;
}


static void hack_array_constructor_call(CallExpr* call) {
  if (call->isNamed("_construct__aarray")) {
    if (DefExpr* def = dynamic_cast<DefExpr*>(call->parentExpr)) {
      call->parentStmt->insertAfter(
        new ExprStmt(new CallExpr(new MemberAccess(def->sym, "myinit"))));
      call->parentStmt->insertAfter(
        new ExprStmt(new CallExpr("=",
          new MemberAccess(def->sym, "dom"),
          call->argList->last()->copy())));
      call->argList->last()->replace(hack_rank(call->argList->last()));
    }
  }
}


static void hack_domain_constructor_call(CallExpr* call) {
  if (call->isNamed("_construct__adomain_lit")) {
    Stmt* stmt = call->parentStmt;
    VarSymbol* _adomain_tmp = new VarSymbol("_adomain_tmp");
    stmt->insertBefore(
      new ExprStmt(
        new DefExpr(_adomain_tmp, NULL,
          new CallExpr("_construct__adomain_lit",
            new_IntLiteral(call->argList->length())))));
    call->replace(new SymExpr(_adomain_tmp));
    int dim = 1;
    for_alist(Expr, arg, call->argList) {
      stmt->insertBefore(
        new ExprStmt(
          new CallExpr(
            new MemberAccess(_adomain_tmp, "_set"),
            new_IntLiteral(dim), arg->copy())));
      dim++;
    }
  } else if (call->isNamed("domain")) {
    if (call->argList->length() != 1)
      USR_FATAL(call, "Domain type cannot yet be inferred");
    if (call->argList->only()->typeInfo() != dtInteger)
      USR_FATAL(call, "Non-arithmetic domains not yet supported");
    call->baseExpr->replace(new SymExpr("_construct__adomain"));
  }
}


static void hack_seqcat_call(CallExpr* call) {
    if (call->isOp(OP_SEQCAT)) {
      Type* leftType = call->get(1)->typeInfo();
      Type* rightType = call->get(2)->typeInfo();

      // assume dtUnknown may be sequence type, at least one should be
      if (leftType != dtUnknown && rightType != dtUnknown)
        INT_FATAL(call, "Bad # operation");

      // if only one is, change to append or prepend
      if (leftType != dtUnknown) {
        call->replace(new CallExpr(
                        new MemberAccess(call->get(2)->copy(), "_prepend"),
                        call->get(1)->copy()));
      } else if (rightType != dtUnknown) {
        call->replace(new CallExpr(
                        new MemberAccess(call->get(1)->copy(), "_append"),
                        call->get(2)->copy()));
      }
    }
}


static void hack_typeof_call(CallExpr* call) {
  if (call->isNamed("typeof")) {
    Type* type = call->argList->only()->typeInfo();
    if (type != dtUnknown) {
      call->replace(new SymExpr(type->symbol));
    } else if (SymExpr* base = dynamic_cast<SymExpr*>(call->argList->only())) {
      if (base->var->defPoint->exprType) {
        call->replace(base->var->defPoint->exprType->copy());
      } else {
        // NOTE we remove the typeof function even if we can't
        // resolve the type before analysis
        Expr* arg = call->argList->only();
        arg->remove();
        call->replace(arg);
      }
    } else {
      // NOTE we remove the typeof function even if we can't
      // resolve the type before analysis
      Expr* arg = call->argList->only();
      arg->remove();
      call->replace(arg);
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
  if (DefExpr* def_expr = dynamic_cast<DefExpr*>(expr)) {
    if (ArgSymbol* arg = dynamic_cast<ArgSymbol*>(def_expr->sym)) {
      if (arg->intent == INTENT_TYPE && can_resolve_type(def_expr->exprType)) {
        arg->type = getMetaType(def_expr->exprType->typeInfo());
      } else if (arg->type == dtUnknown &&
                 can_resolve_type(def_expr->exprType)) {
        arg->type = def_expr->exprType->typeInfo();
        def_expr->exprType->remove();
      }
    } else if (VarSymbol* var = dynamic_cast<VarSymbol*>(def_expr->sym)) {
      if (var->type == dtUnknown && can_resolve_type(def_expr->exprType)) {
        var->type = def_expr->exprType->typeInfo();
        def_expr->exprType->remove();
      } else if (var->type == dtUnknown &&
                 !def_expr->exprType &&
                 can_resolve_type(def_expr->init)) {
        var->type = def_expr->init->typeInfo();
      }
    }
  }

  if (CastExpr* castExpr = dynamic_cast<CastExpr*>(expr)) {
    if (castExpr->type == dtUnknown && can_resolve_type(castExpr->newType)) {
      castExpr->type = castExpr->newType->typeInfo();
      castExpr->newType = NULL;
    }
  }

  if (no_infer) {
    if (DefExpr* def = dynamic_cast<DefExpr*>(expr)) {
      if (def->sym->type == dtUnknown) {
        if (def->init && !def->exprType) {
          def->exprType = def->init->copy();
          fixup(def->exprType, def->init);
        }
      }
    }
  }

  if (DefExpr* def = dynamic_cast<DefExpr*>(expr)) {
    if (TypeSymbol* ts = dynamic_cast<TypeSymbol*>(def->sym)) {
      if (UserType* userType = dynamic_cast<UserType*>(ts->definition)) {
        if (userType->underlyingType == dtUnknown && 
            can_resolve_type(userType->typeExpr)) {
          userType->underlyingType = userType->typeExpr->typeInfo();
          userType->typeExpr = NULL;
          if (!userType->defaultValue) {
            if (userType->underlyingType->defaultValue) {
              userType->defaultValue = userType->underlyingType->defaultValue;
              ASTContext context;
              context.parentScope = userType->symbol->defPoint->parentScope;
              context.parentSymbol = userType->symbol;
              context.parentStmt = NULL;
              context.parentExpr = NULL;
              insertHelper(userType->defaultValue, context);
            } else {
              userType->defaultConstructor =
                userType->underlyingType->defaultConstructor;
            }
          }
        }
      }
    }
  }

  if (DefExpr* def = dynamic_cast<DefExpr*>(expr)) {
    if (VarSymbol* var = dynamic_cast<VarSymbol*>(def->sym)) {
      if (ClassType* ct = dynamic_cast<ClassType*>(var->type)) {
        forv_Vec(TypeSymbol, type, ct->types) {
          if (dynamic_cast<VariableType*>(type->definition)) {
            INT_FATAL(var, "Illegal declaration, type is generic");
          }
        }
      }
    }
  }
}


#define REPLACE(_x)              \
  do {                           \
    BaseAST* replacement = _x;   \
    ast->replace(replacement);   \
    ast = replacement;           \
    goto Ldone;                  \
  } while (0)


static void apply_getters_setters(BaseAST* ast) {
  // Most generally:
  //   x.f(1) = y ---> f(_mt, x, 1, _st, y)
  // or
  //   CallExpr(=, CallExpr(MemberAccess(x, "f"), 1), y) --->
  //     CallExpr("f", _mt, x, 1, _st, y)
  // though, it could be just
  //           a MemberAccess without a CallExpr
  //           a CallExpr without a MemberAccess
  // SJD: Commment needs to be updated with PARTIAL_OK
  CallExpr *call = dynamic_cast<CallExpr*>(ast), *assign = 0;
  if (call && call->isNamed("=")) {
    assign = call;
    call = dynamic_cast<CallExpr*>(assign->get(1));
  }
  BaseAST *base = call ? call->baseExpr : (assign ? assign->get(1) : ast);
  if (MemberAccess* memberAccess = dynamic_cast<MemberAccess*>(base)) {
    Expr *rhs = assign ? assign->argList->get(2)->copy() : 0;
    // build the main accessor/setter
    if (call) {
      CallExpr *lhs = new CallExpr(memberAccess->member->name, 
                                   methodToken,
                                   memberAccess->base->copy());
      lhs->partialTag = PARTIAL_OK;
      AList<Expr>* arguments = call ? call->argList->copy() : new AList<Expr>;
      if (rhs) {
        arguments->insertAtTail(new SymExpr(setterToken));
        arguments->insertAtTail(rhs);
      }
      lhs = new CallExpr(lhs, arguments);
      REPLACE(lhs);
    } else {
      AList<Expr>* arguments = call ? call->argList->copy() : new AList<Expr>;
      arguments->insertAtHead(memberAccess->base->copy());
      arguments->insertAtHead(new SymExpr(methodToken));
      if (rhs) {
        arguments->insertAtTail(new SymExpr(setterToken));
        arguments->insertAtTail(rhs);
      }
      REPLACE(new CallExpr(memberAccess->member->name, arguments));
    }
  } 
  if (assign) {
    Expr *rhs = assign->argList->get(2)->copy();
    if (call) {
      AList<Expr>* arguments = call->argList->copy();
      arguments->insertAtTail(new SymExpr(setterToken));
      arguments->insertAtTail(rhs);
      REPLACE(new CallExpr(call->baseExpr->copy(), arguments));
    } else
      REPLACE(new CallExpr("=", assign->argList->get(1)->copy(), rhs));
  } 
 Ldone:
  // top down, on the modified AST
  Vec<BaseAST *> asts;
  get_ast_children(ast, asts);
  forv_BaseAST(a, asts)
    apply_getters_setters(a);
}


static void insert_call_temps(CallExpr* call) {
  static int uid = 1;

  if (!call->parentExpr || !call->parentStmt)
    return;
  
  if (dynamic_cast<DefExpr*>(call->parentExpr))
    return;

  if (call->partialTag != PARTIAL_NEVER)
    return;

  if (call->isNamed("typeof") || call->isNamed("__primitive"))
    return;

  Stmt* stmt = call->parentStmt;
  VarSymbol* tmp = new VarSymbol(stringcat("_tmp"));
  tmp->cname = stringcat(tmp->name, intstring(uid++));
  tmp->noDefaultInit = true;
  call->replace(new SymExpr(tmp));
  stmt->insertBefore(new ExprStmt(new DefExpr(tmp, call)));

  if (SymExpr* base = dynamic_cast<SymExpr*>(call->baseExpr)) {
    if (!strncmp(base->var->name, "_let_fn", 7)) {
      Stmt* stmt = inline_call(call);
      Vec<BaseAST*> asts;
      collect_asts_postorder(&asts, stmt);
      forv_Vec(BaseAST, ast, asts) {
        currentLineno = ast->lineno;
        currentFilename = ast->filename;
        if (CallExpr* a = dynamic_cast<CallExpr*>(ast)) {
          insert_call_temps(a);
        }
      }
      base->var->defPoint->parentStmt->remove();
    }
  }
}


static void fix_def_expr(DefExpr* def) {
  static int uid = 1;
  if (dynamic_cast<VarSymbol*>(def->sym)->noDefaultInit) {
    if (def->init)
      def->parentStmt->insertAfter(new ExprStmt(new CallExpr(OP_MOVE, def->sym, def->init->copy())));
  } else if (def->sym->type != dtUnknown) {
    AList<Stmt>* stmts = new AList<Stmt>();
    VarSymbol* tmp = new VarSymbol("_defTmp");
    tmp->cname = stringcat(tmp->name, intstring(uid++));
    tmp->noDefaultInit = true;
    stmts->insertAtTail(new ExprStmt(new DefExpr(tmp)));
    stmts->insertAtTail(new ExprStmt(new CallExpr(OP_MOVE, tmp, new CallExpr(OP_INIT, def->sym->type->symbol))));
    if (def->init)
      stmts->insertAtTail(new ExprStmt(new CallExpr(OP_MOVE, def->sym, new CallExpr("=", tmp, def->init->copy()))));
    else
      stmts->insertAtTail(new ExprStmt(new CallExpr(OP_MOVE, def->sym, tmp)));
    def->parentStmt->insertAfter(stmts);
  } else if (def->exprType) {
    AList<Stmt>* stmts = new AList<Stmt>();
    VarSymbol* tmp = new VarSymbol("_defTmp");
    tmp->cname = stringcat(tmp->name, intstring(uid++));
    tmp->noDefaultInit = true;
    stmts->insertAtTail(new ExprStmt(new DefExpr(tmp)));
    stmts->insertAtTail(new ExprStmt(new CallExpr(OP_MOVE, tmp, new CallExpr(OP_INIT, def->exprType->copy()))));
    if (def->init)
      stmts->insertAtTail(new ExprStmt(new CallExpr(OP_MOVE, def->sym, new CallExpr("=", tmp, def->init->copy()))));
    else
      stmts->insertAtTail(new ExprStmt(new CallExpr(OP_MOVE, def->sym, tmp)));
    def->parentStmt->insertAfter(stmts);
  } else if (def->init) {
    AList<Stmt>* stmts = new AList<Stmt>();
    VarSymbol* tmp1 = new VarSymbol("_defTmp1");
    tmp1->cname = stringcat(tmp1->name, intstring(uid++));
    tmp1->noDefaultInit = true;
    stmts->insertAtTail(new ExprStmt(new DefExpr(tmp1)));
    VarSymbol* tmp2 = new VarSymbol("_defTmp2");
    tmp2->cname = stringcat(tmp2->name, intstring(uid++));
    tmp2->noDefaultInit = true;
    stmts->insertAtTail(new ExprStmt(new DefExpr(tmp2)));
    stmts->insertAtTail(new ExprStmt(new CallExpr(OP_MOVE, tmp1, def->init->copy())));
    stmts->insertAtTail(new ExprStmt(new CallExpr(OP_MOVE, tmp2, new CallExpr(OP_INIT, tmp1))));
    stmts->insertAtTail(new ExprStmt(new CallExpr(OP_MOVE, def->sym, new CallExpr("=", tmp2, tmp1))));
    def->parentStmt->insertAfter(stmts);
  } else {
    def->parentStmt->insertAfter(new ExprStmt(new CallExpr(OP_MOVE, def->sym, new CallExpr(OP_INIT, gUnspecified))));
  }
}
