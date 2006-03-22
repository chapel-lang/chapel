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
#include "symtab.h"
#include "stringutil.h"
#include "../traversals/view.h"

bool normalized = false;

static void reconstruct_iterator(FnSymbol* fn);
static void build_lvalue_function(FnSymbol* fn);
static void normalize_returns(FnSymbol* fn);
static void insert_type_default_temp(UserType* userType);
static void initialize_out_formals(FnSymbol* fn);
static void insert_formal_temps(FnSymbol* fn);
static void call_constructor_for_class(CallExpr* call);
static void decompose_special_calls(CallExpr* call);
static void hack_seqcat_call(CallExpr* call);
static void convert_user_primitives(CallExpr* call);
static void hack_resolve_types(Expr* expr);
static void apply_getters_setters(FnSymbol* fn);
static void insert_call_temps(CallExpr* call);
static void fix_user_assign(CallExpr* call);
static void fix_def_expr(DefExpr* def, VarSymbol* var);
static void fold_params(Vec<DefExpr*>* defs, Vec<BaseAST*>* asts);
static void expand_var_args(FnSymbol* fn);
static int tag_generic(FnSymbol* fn);
static int tag_generic(Type* fn);
static void tag_hasVarArgs(FnSymbol* fn);
static void resolve_formal_types(FnSymbol* fn);

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
  collect_asts(&asts, base);
  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;

    if (TypeSymbol* ts = dynamic_cast<TypeSymbol*>(ast)) {
      if (UserType* userType = dynamic_cast<UserType*>(ts->definition)) {
        insert_type_default_temp(userType);
      }
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
    if (CallExpr* a = dynamic_cast<CallExpr*>(ast)) {
      insert_call_temps(a);
    }
  }

  asts.clear();
  collect_asts_postorder(&asts, base);
  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (DefExpr* a = dynamic_cast<DefExpr*>(ast)) {
      if (VarSymbol* var = dynamic_cast<VarSymbol*>(a->sym))
        if (dynamic_cast<FnSymbol*>(a->parentSymbol))
          fix_def_expr(a, var);
    }
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

  asts.clear();
  collect_asts_postorder(&asts, base);
  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (CallExpr* a = dynamic_cast<CallExpr*>(ast)) {
      convert_user_primitives(a);
    }
  }

  compute_sym_uses(base);
  Vec<DefExpr*> defs;
  asts.clear();
  collect_asts_postorder(&asts, base);
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* a = dynamic_cast<DefExpr*>(ast)) {
      defs.add(a);
    }
  }
  fold_params(&defs, &asts);

  asts.clear();
  collect_asts(&asts, base);
  forv_Vec(BaseAST, ast, asts) {
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(ast)) {
      expand_var_args(fn);
    }
  }

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
      resolve_formal_types(fn);
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


static TypeSymbol* create_iterator_next_function(FnSymbol* fn) {
  static int uid = 1;

  ClassType* ct = new ClassType(CLASS_CLASS);
  TypeSymbol* ts =
    new TypeSymbol(stringcat("_iterator_state", intstring(uid++)), ct);
  AList<Stmt>* decl_stmts = new AList<Stmt>();
  FnSymbol* next = fn->copy();
  next->name = stringcat("_next_", fn->name);
  next->cname = stringcat("_next_", fn->cname);
  ArgSymbol* state = new ArgSymbol(INTENT_BLANK, "_state", ct);
  next->formals->insertAtHead(new DefExpr(state));
  fn->defPoint->parentStmt->insertBefore(new DefExpr(next));
  // find local variable declarations
  Map<Symbol*,Symbol*> localsToFields;
  Vec<BaseAST*> asts;
  collect_asts(&asts, next);
  int field_uid = 1;
  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = dynamic_cast<DefExpr*>(ast)) {
      if (dynamic_cast<VarSymbol*>(def->sym)) {
        DefExpr* fieldDef = def->copy();
        fieldDef->sym->name =
          stringcat("_state", intstring(field_uid++), "_", fieldDef->sym->name);
        decl_stmts->insertAtTail(fieldDef);
        localsToFields.put(def->sym, fieldDef->sym);
        def->parentStmt->remove();
      }
    }
  }
  asts.clear();
  collect_asts(&asts, next);
  forv_Vec(BaseAST, ast, asts) {
    if (SymExpr* symExpr = dynamic_cast<SymExpr*>(ast)) {
      if (dynamic_cast<VarSymbol*>(symExpr->var)) {
        if (Symbol* field = localsToFields.get(symExpr->var)) {
          symExpr->replace(
            new CallExpr(".", state, new_StringSymbol(field->name)));
        }
      }
    }
  }
  ct->addDeclarations(decl_stmts);
  fn->defPoint->parentStmt->insertBefore(new DefExpr(ts));
  cleanup(ts->defPoint->parentStmt);
  cleanup(next->defPoint->parentStmt);
  return ts;
}


static void reconstruct_iterator(FnSymbol* fn) {
  if (0) create_iterator_next_function(fn);
  Expr* seqType = fn->retExpr;
  Type *seqElementType = seqType->typeInfo();
  if (!seqType)
    USR_FATAL(fn, "Cannot infer iterator return type yet");
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
        fn->typeBinding->definition->scalarPromotionType = seqElementType;
        if (!run_interpreter)
          fn->typeBinding->definition->dispatchParents.add(seqElementType);
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
                        ClassType *ct = dynamic_cast<ClassType*>(fn->typeBinding->definition);
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
  if (fn->typeBinding)
    fn->typeBinding->definition->methods.add(new_fn);
  new_fn->retRef = false;
  fn->retRef = false;
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
  fn->insertAtTail(new LabelStmt(label));
  VarSymbol* retval = NULL;
  if (returns_void) {
    fn->insertAtTail(new ReturnStmt());
  } else {
    retval = new VarSymbol(stringcat("_ret_", fn->name), fn->retType);
    Expr* type = fn->retExpr;
    type->remove();
    if (!type)
      retval->noDefaultInit = true;
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
        mod->initFn->insertAtHead(def);
      } else {
        Stmt* insert_point = outer_symbol->defPoint->parentStmt;
        insert_point->insertBefore(def);
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
    if (arg->intent == INTENT_OUT) {
      if (arg->defaultExpr)
        fn->insertAtHead(new CallExpr(PRIMITIVE_MOVE, arg, arg->defaultExpr->copy()));
      else
        fn->insertAtHead(new CallExpr(PRIMITIVE_MOVE, arg, new CallExpr(PRIMITIVE_INIT, arg)));
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


static CallExpr* lineno_info(CallExpr* call) {
  CallExpr* errorInfo;
  if (printChplLineno) {
    errorInfo = new CallExpr("fwrite", 
                             chpl_stdout, 
                             chpl_input_filename, 
                             new_StringLiteral(":"), 
                             chpl_input_lineno);
  } else {
    errorInfo = new CallExpr("fwrite", 
                   chpl_stdout,
                   new_StringLiteral(stringcat(call->filename, 
                                               ":", 
                                               intstring(call->lineno))));
  }
  return errorInfo;
}


static void buildAssertStatement(CallExpr* call) {
    AList<Stmt>* blockStmt = new AList<Stmt>();
    CallExpr* assertFailed = new CallExpr("fwrite", chpl_stdout, new_StringLiteral("Assertion failed: "));
    blockStmt->insertAtTail(assertFailed);
    CallExpr* printLineno = lineno_info(call);
    blockStmt->insertAtTail(printLineno);
    Expr* assertArg = call->argList->get(1);
    assertArg->remove();
    CallExpr* fwritelnCall = new CallExpr("fwriteln", chpl_stdout);
    if (call->argList->length() > 1) {
      blockStmt->insertAtTail(fwritelnCall->copy());
    }
    for_alist(Expr, actual, call->argList) {
      actual->remove();
      blockStmt->insertAtTail(new CallExpr("fwrite", chpl_stdout, actual));
    }
    blockStmt->insertAtTail(fwritelnCall->copy());
    CallExpr* exitCall = new CallExpr("exit", new_IntLiteral(0));
    blockStmt->insertAtTail(exitCall);
    Expr* assert_cond = new CallExpr("!", assertArg->copy());
    BlockStmt* assert_body = new BlockStmt(blockStmt);
    call->parentStmt->insertBefore(new CondStmt(assert_cond, assert_body));
    decompose_special_calls(printLineno);
    call->parentStmt->remove();
}


static void buildHaltStatement(CallExpr* call) {
    CallExpr* haltReached = new CallExpr("fwrite", chpl_stdout, new_StringLiteral("Halt reached: "));
    call->parentStmt->insertBefore(haltReached);
    CallExpr* printLineno = lineno_info(call);
    call->parentStmt->insertBefore(printLineno);
    decompose_special_calls(printLineno);
    CallExpr* fwritelnCall = new CallExpr("fwriteln", chpl_stdout);
    call->parentStmt->insertBefore(fwritelnCall->copy());
    CallExpr* exitCall = new CallExpr("exit", new_IntLiteral(0));
    call->parentStmt->insertAfter(exitCall);
    call->parentStmt->insertAfter(fwritelnCall->copy());
    decompose_multi_actuals(call, "fwrite", new SymExpr(chpl_stdout));
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
  if (call->isNamed("assert")) {
    buildAssertStatement(call);
  } else if (call->isNamed("halt")) {
    buildHaltStatement(call);
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
    call->parentStmt->insertAfter(new CallExpr("fwriteln", file));
    decompose_multi_actuals(call, "fwrite", file);
  } else if (call->isNamed("read")) {
    decompose_multi_actuals(call, "fread", new SymExpr(chpl_stdin));
  } else if (call->isNamed("write")) {
    decompose_multi_actuals(call, "fwrite", new SymExpr(chpl_stdout));
  } else if (call->isNamed("writeln")) {
    call->parentStmt->insertAfter(new CallExpr("fwriteln", chpl_stdout));
    decompose_multi_actuals(call, "fwrite", new SymExpr(chpl_stdout));
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
  if (DefExpr* def_expr = dynamic_cast<DefExpr*>(expr)) {
    if (ArgSymbol* arg = dynamic_cast<ArgSymbol*>(def_expr->sym)) {
      if (arg->intent == INTENT_TYPE && can_resolve_type(def_expr->exprType)) {
        arg->type = getMetaType(def_expr->exprType->typeInfo());
        def_expr->exprType->remove();
      } else if (arg->type == dtUnknown &&
                 can_resolve_type(def_expr->exprType)) {
        arg->type = def_expr->exprType->typeInfo();
        def_expr->exprType->remove();
      }
    } else if (VarSymbol* var = dynamic_cast<VarSymbol*>(def_expr->sym)) {
      if (var->type == dtUnknown && can_resolve_type(def_expr->exprType)) {
        Type* type = def_expr->exprType->typeInfo();
        if (type == dtString || dynamic_cast<EnumType*>(type)) {
          var->type = type;
          def_expr->exprType->remove();
        }
      } else if (var->type == dtUnknown &&
                 !def_expr->exprType &&
                 can_resolve_type(def_expr->init)) {
        Type* type = def_expr->init->typeInfo();
        if (type == dtString || dynamic_cast<EnumType*>(type))
          var->type = type;
      }
    }
  }

  if (CastExpr* castExpr = dynamic_cast<CastExpr*>(expr)) {
    if (castExpr->type == dtUnknown && can_resolve_type(castExpr->newType)) {
      castExpr->type = castExpr->newType->typeInfo();
      castExpr->newType = NULL;
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
            lhs->argList->insertAtTail(new SymExpr(setterToken));
            lhs->argList->insertAtTail(rhs);
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

  if (call->partialTag != PARTIAL_NEVER)
    return;

  if (call->primitive || call->isNamed("__primitive"))
    return;

  if (CallExpr* parentCall = dynamic_cast<CallExpr*>(call->parentExpr))
    if (parentCall->isPrimitive(PRIMITIVE_MOVE))
      return;

  Stmt* stmt = call->parentStmt;
  VarSymbol* tmp = new VarSymbol("_tmp", dtUnknown, VAR_NORMAL, VAR_CONST);
  tmp->cname = stringcat(tmp->name, intstring(uid++));
  tmp->noDefaultInit = true;
  call->replace(new SymExpr(tmp));
  stmt->insertBefore(new DefExpr(tmp, call));
}


static void fix_user_assign(CallExpr* call) {
  if (call->parentExpr)
    return;
  CallExpr* move = new CallExpr(PRIMITIVE_MOVE, call->get(1)->copy());
  call->replace(move);
  move->argList->insertAtTail(call);
}


static void fix_def_expr(DefExpr* def, VarSymbol* var) {
  static int uid = 1;
  SymExpr* initSymExpr = dynamic_cast<SymExpr*>(def->init);
  bool no_init = initSymExpr && initSymExpr->var == gUnspecified;
  if (no_init || var->noDefaultInit) {
    if (no_init)
      def->init->remove();
    if (var->type == dtUnspecified)
      var->type = dtUnknown;
    if (def->init)
      def->parentStmt->insertAfter(new CallExpr(PRIMITIVE_MOVE, var, def->init->copy()));
  } else if (var->type != dtUnknown) {
    AList<Stmt>* stmts = new AList<Stmt>();
    VarSymbol* tmp = new VarSymbol("_defTmp", dtUnknown, VAR_NORMAL, VAR_CONST);
    tmp->noDefaultInit = true;
    tmp->cname = stringcat(tmp->name, intstring(uid++));
    stmts->insertAtTail(new DefExpr(tmp));
    stmts->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_INIT, var->type->symbol)));
    if (def->init)
      stmts->insertAtTail(new CallExpr(PRIMITIVE_MOVE, var, new CallExpr("=", tmp, def->init->copy())));
    else
      stmts->insertAtTail(new CallExpr(PRIMITIVE_MOVE, var, tmp));
    def->parentStmt->insertAfter(stmts);
  } else if (def->exprType) {
    AList<Stmt>* stmts = new AList<Stmt>();
    VarSymbol* tmp = new VarSymbol("_defTmp", dtUnknown, VAR_NORMAL, VAR_CONST);
    tmp->noDefaultInit = true;
    tmp->cname = stringcat(tmp->name, intstring(uid++));
    stmts->insertAtTail(new DefExpr(tmp));
    stmts->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_INIT, def->exprType->copy())));
    if (def->init)
      stmts->insertAtTail(new CallExpr(PRIMITIVE_MOVE, var, new CallExpr("=", tmp, def->init->copy())));
    else
      stmts->insertAtTail(new CallExpr(PRIMITIVE_MOVE, var, tmp));
    def->parentStmt->insertAfter(stmts);
  } else if (def->init) {
    AList<Stmt>* stmts = new AList<Stmt>();
    VarSymbol* tmp1 = new VarSymbol("_defTmp1", dtUnknown, VAR_NORMAL, VAR_CONST);
    tmp1->noDefaultInit = true;
    tmp1->cname = stringcat(tmp1->name, intstring(uid++));
    stmts->insertAtTail(new DefExpr(tmp1));
    VarSymbol* tmp2 = new VarSymbol("_defTmp2", dtUnknown, VAR_NORMAL, VAR_CONST);
    tmp2->noDefaultInit = true;
    tmp2->cname = stringcat(tmp2->name, intstring(uid++));
    stmts->insertAtTail(new DefExpr(tmp2));
    stmts->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp1, def->init->copy()));
    stmts->insertAtTail(new CallExpr(PRIMITIVE_MOVE, tmp2, new CallExpr(PRIMITIVE_INIT, tmp1)));
    stmts->insertAtTail(new CallExpr(PRIMITIVE_MOVE, var, new CallExpr("=", tmp2, tmp1)));
    def->parentStmt->insertAfter(stmts);
  } else {
    def->parentStmt->insertAfter(new CallExpr(PRIMITIVE_MOVE, var, new CallExpr(PRIMITIVE_INIT, gUnspecified)));
  }
  def->exprType->remove();
  def->init->remove();
  var->noDefaultInit = true;
}


static void convert_user_primitives(CallExpr* call) {
  if (call->isNamed("__primitive")) {
    if (!call->argList->length() > 0)
      INT_FATAL(call, "primitive with no name");
    SymExpr *s = dynamic_cast<SymExpr*>(call->argList->get(1));
    if (!s)
      INT_FATAL(call, "primitive with no name");
    VarSymbol *str = dynamic_cast<VarSymbol*>(s->var);
    if (!str || !str->immediate || str->immediate->const_kind != IF1_CONST_KIND_STRING)
      INT_FATAL(call, "primitive with non-literal string name");
    PrimitiveOp *prim = primitives_map.get(str->immediate->v_string);
    if (!prim)
      INT_FATAL(call, "primitive not found '%s'", str->immediate->v_string);
    s->remove();
    call->replace(new CallExpr(prim, call->argList));
  }
}


#define FOLD_CALL(name, prim)                             \
  if (call->isNamed(name)) {                              \
    Immediate i3;                                         \
    fold_constant(prim, i1, i2, &i3);                     \
    call->replace(new SymExpr(new_ImmediateSymbol(&i3))); \
    return;                                               \
  }

static void fold_call_expr(CallExpr* call) {
  if (call->partialTag == PARTIAL_ALWAYS)
    return;
  if (call->isPrimitive(PRIMITIVE_INIT)) {
    if (!no_infer) {
      if (CallExpr* construct = dynamic_cast<CallExpr*>(call->get(1))) {
        if (SymExpr* base = dynamic_cast<SymExpr*>(construct->baseExpr)) {
          Symbol* sym = Symboltable::lookupFromScope(base->var->name, call->parentScope);
          if (FnSymbol* fn = dynamic_cast<FnSymbol*>(sym)) {
            if (fn->fnClass == FN_CONSTRUCTOR) {
              if (ClassType* ct = dynamic_cast<ClassType*>(fn->retType)) {
                if (ct->classTag == CLASS_CLASS) {
                  call->replace(new SymExpr(gNil));
                }
              }
            }
          }
        }
      }
    }
    if (call->get(1)->typeInfo() == dtInt)
      call->replace(new SymExpr(dtInt->defaultValue));
    return;
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

static void fold_params(Vec<DefExpr*>* defs, Vec<BaseAST*>* asts) {
  bool change;
  do {
    change = false;
    forv_Vec(BaseAST*, ast, *asts) {
      if (CallExpr* a = dynamic_cast<CallExpr*>(ast))
        if (a->parentSymbol) // in ast
          fold_call_expr(a);
      if (CondStmt* a = dynamic_cast<CondStmt*>(ast))
        if (a->parentSymbol) // in ast
          fold_cond_stmt(a);
    }
    forv_Vec(DefExpr*, def, *defs) {
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
        if (n_var->type == dtInt && n_var->immediate) {
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
//           if (n == 1) {
//             Expr* actual = actuals->only();
//             actual->remove();
//             fn->insertAtHead(new DefExpr(var, actual));
//           } else
          if (arg->type != dtUnknown) {
            int i = n;
            for_alist_backward(Expr, actual, actuals) {
              actual->remove();
              fn->insertAtHead(
                new CallExpr("=",
                  new CallExpr(var, new_IntLiteral(i)), actual));
              i--;
            }
            fn->insertAtHead(
              new DefExpr(var,
                new CallExpr("_htuple", arg->type->symbol, new_IntLiteral(n))));
          } else {
            fn->insertAtHead(
              new DefExpr(var,
                new CallExpr(stringcat("_construct__tuple", intstring(n)),
                             actual_types, actuals)));
          }
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
checkGeneric(BaseAST *ast, Vec<Symbol *> *genericSymbols = 0) {
  int result = 0;
  switch (ast->astType) {
    case EXPR_SYM: {
      SymExpr* v = dynamic_cast<SymExpr* >(ast);
      if (TypeSymbol *ts = dynamic_cast<TypeSymbol*>(v->var))
        if (ts->definition->isGeneric) {
          if (!genericSymbols)
            return 1;
          else {
            genericSymbols->set_add(ts);
            result = 1;
          }
        }
      break;
    }
    default: break;
  }
  DefExpr* def_expr = dynamic_cast<DefExpr*>(ast);
  if (!def_expr || !dynamic_cast<FnSymbol*>(def_expr->sym)) {
    Vec<BaseAST *> asts;
    get_ast_children(ast, asts);
    forv_BaseAST(a, asts)
      if (checkGeneric(a, genericSymbols)) {
        if (!genericSymbols)
          return 1;
        else
          result = 1;
      }
  }
  return result;
}

static int
genericFunctionArg(FnSymbol *f, Vec<Symbol *> &genericSymbols) {
  for (DefExpr* formal = f->formals->first(); formal; 
       formal = f->formals->next()) {
    if (ArgSymbol *ps = dynamic_cast<ArgSymbol *>(formal->sym)) {
      if (TypeSymbol *ts = dynamic_cast<TypeSymbol *>(ps->genericSymbol)) {
        if (ts->definition->isGeneric) {
          assert(dynamic_cast<VariableType*>(ts->definition));
          genericSymbols.set_add(ts);
          return 1;
        }
      }
      if (ps->type && ps->type->isGeneric) {
        genericSymbols.set_add(ps->type->symbol);
        return 1;
      }
      if (ps->intent == INTENT_PARAM) {
        genericSymbols.set_add(ps);
        return 1;
      }
    }
  }
  return 0;
}

static int
tag_generic(FnSymbol *f) {
  int changed = 0;
  Vec<Symbol *> genericSymbols;
  if (genericFunctionArg(f, genericSymbols) || checkGeneric(f->body, &genericSymbols)) {
    changed = !f->isGeneric || changed;
    f->isGeneric = 1; 
    f->genericSymbols.copy(genericSymbols);
    f->genericSymbols.set_to_vec();
    qsort(f->genericSymbols.v, f->genericSymbols.n, sizeof(genericSymbols.v[0]), compar_baseast);
//     if (int i = f->nestingDepth()) {
//       for (int j = 1; j <= i; j++) {
//         FnSymbol *ff = f->nestingParent(i);
//         changed = !ff->isGeneric || changed;
//         genericSymbols.set_union(ff->genericSymbols);
//         ff->genericSymbols.copy(genericSymbols);
//         ff->genericSymbols.set_to_vec();
//         qsort(ff->genericSymbols.v, ff->genericSymbols.n, sizeof(genericSymbols.v[0]), compar_baseast);
//         ff->isGeneric = 1;
//       }
//     }
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
  for_alist(DefExpr, formalDef, fn->formals) {
    ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
    if (formal->variableExpr)
      fn->hasVarArgs = true;
  }
}

static void
resolve_formal_types(FnSymbol* fn) {
  if (!fn->isGeneric) {
    for_alist(DefExpr, formalDef, fn->formals) {
      if (dynamic_cast<CallExpr*>(formalDef->exprType)) {
        resolve_type_expr(formalDef->exprType);
        hack_resolve_types(formalDef);
      }
    }
  }
}
