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
#include "../traversals/updateSymbols.h"


static void reconstruct_iterator(FnSymbol* fn);
static void build_lvalue_function(FnSymbol* fn);
static void normalize_returns(FnSymbol* fn);
static void insert_type_default_temp(UserType* userType);
static void initialize_out_formals(FnSymbol* fn);
static void insert_formal_temps(FnSymbol* fn);
static void call_constructor_for_class(CallExpr* call);
static void normalize_for_loop(ForLoopStmt* stmt);

void normalize(void) {
  Vec<FnSymbol*> fns;
  Vec<BaseAST*> asts;
  collect_functions(&fns);
  forv_Vec(FnSymbol, fn, fns) {
    if (fn->fnClass == FN_ITERATOR)
      reconstruct_iterator(fn);
    if (fn->retRef)
      build_lvalue_function(fn);
    normalize_returns(fn);
    initialize_out_formals(fn);
    insert_formal_temps(fn);
  }

  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts) {
    if (VarSymbol* vs = dynamic_cast<VarSymbol*>(ast)) {
      if (vs->type == dtUnknown && !vs->defPoint->exprType) {
        vs->noDefaultInit = true;
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
    if (CallExpr* a = dynamic_cast<CallExpr*>(ast)) {
      call_constructor_for_class(a);
    } else if (ForLoopStmt* a = dynamic_cast<ForLoopStmt*>(ast)) {
      normalize_for_loop(a);
    }
  }
}


static void reconstruct_iterator(FnSymbol* fn) {
  Expr* seqType = NULL;
  if (fn->retType != dtUnknown) {
    seqType = new SymExpr(fn->retType->symbol);
  } else if (fn->defPoint->exprType) {
    seqType = fn->defPoint->exprType->copy();
  } else {
    Vec<BaseAST*> asts;
    collect_asts(&asts, fn->body);
    Type* type = dtUnknown;
    forv_Vec(BaseAST, ast, asts) {
      if (ReturnStmt* returnStmt = dynamic_cast<ReturnStmt*>(ast))
        if (returnStmt->expr)
          type = returnStmt->expr->typeInfo();
    }
    if (type != dtUnknown) {
      seqType = new SymExpr(type->symbol);
    } else {
      USR_FATAL(fn, "Unable to infer type of iterator");
    }
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
      newStmt = new ExprStmt(new CallExpr(OP_GETS, e, lvalue));
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
    retval->noDefaultInit = true;
    Expr* type = NULL;
    if (fn->defPoint->exprType)
      type = fn->defPoint->exprType->copy();
    fn->insertAtHead(new ExprStmt(new DefExpr(retval, NULL, type)));
    fn->insertAtTail(new ReturnStmt(retval));
  }
  forv_Vec(ReturnStmt, ret, rets) {
    if (retval) {
      if (no_infer) {
        Expr* ret_expr = ret->expr;
        ret->expr->remove();
        CallExpr* call = new CallExpr("_move", retval, ret_expr);
        call->opTag = OP_GETS;
        ret->insertBefore(new ExprStmt(call));
      } else {
        Expr* ret_expr = ret->expr;
        ret->expr->remove();
        ret->insertBefore(new ExprStmt(new CallExpr(OP_GETS, retval, ret_expr)));
      }
    }
    ret->replace(new GotoStmt(goto_normal, label));
  }
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
    if (arg->defPoint->init && arg->intent == INTENT_OUT)
      fn->body->insertAtHead(
        new ExprStmt(
          new CallExpr(OP_GETS, arg, arg->defPoint->init->copy())));
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

  TRAVERSE(fn->body, new UpdateSymbols(&subs), true);

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
