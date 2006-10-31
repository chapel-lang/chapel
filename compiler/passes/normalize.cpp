/*** normalize
 ***
 *** This pass and function normalizes parsed and scope-resolved AST.
 ***/

#include <sys/time.h>
#include "astutil.h"
#include "expr.h"
#include "passes.h"
#include "../ifa/prim_data.h"
#include "runtime.h"
#include "stmt.h"
#include "symbol.h"
#include "symscope.h"
#include "stringutil.h"
#include "view.h"
#include "build.h"

bool normalized = false;

static void change_method_into_constructor(FnSymbol* fn);
static void enable_scalar_promotion(FnSymbol* fn);
static void iterator_transform(FnSymbol* fn);
static void build_lvalue_function(FnSymbol* fn);
static void normalize_returns(FnSymbol* fn);
static void initialize_out_formals(FnSymbol* fn);
static void insert_formal_temps(FnSymbol* fn);
static void call_constructor_for_class(CallExpr* call);
static void decompose_special_calls(CallExpr* call);
static void hack_resolve_types(Expr* expr);
static void apply_getters_setters(FnSymbol* fn);
static void insert_call_temps(CallExpr* call);
static void fix_user_assign(CallExpr* call);
static void fix_def_expr(DefExpr* def);
static void fold_params(BaseAST* base);
static void expand_var_args(FnSymbol* fn);
static int  tag_generic(FnSymbol* fn);
static void tag_hasVarArgs(FnSymbol* fn);
static void tag_global(FnSymbol* fn);
static void change_types_to_values(BaseAST* base);
static void fixup_array_formals(FnSymbol* fn);
static void clone_parameterized_primitive_methods(FnSymbol* fn);
static void fixup_parameterized_primitive_formals(FnSymbol* fn);


void normalize(void) {
  forv_Vec(ModuleSymbol, mod, allModules) {
    normalize(mod);
  }
  normalized = true;
}

void normalize(BaseAST* base) {
  Vec<BaseAST*> asts;

  asts.clear();
  collect_asts( &asts, base);
  forv_Vec(BaseAST, ast, asts) {
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(ast)) {
      currentLineno = fn->lineno;
      currentFilename = fn->filename;
      fixup_array_formals(fn);
      clone_parameterized_primitive_methods(fn);
      fixup_parameterized_primitive_formals(fn);
      if (fn->fnClass == FN_ITERATOR) {
        enable_scalar_promotion( fn);
        iterator_transform( fn);
      }
      change_method_into_constructor(fn);
    }
  }

  asts.clear();
  collect_asts_postorder(&asts, base);
  forv_Vec(BaseAST, ast, asts) {
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(ast)) {
      currentLineno = fn->lineno;
      currentFilename = fn->filename;
      fixup_array_formals(fn);
      fixup_parameterized_primitive_formals(fn);
      if (fn->buildSetter)
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
      decompose_special_calls(a);
    }
  }

  asts.clear();
  collect_asts_postorder(&asts, base);
  forv_Vec(BaseAST, ast, asts) {
    currentLineno = ast->lineno;
    currentFilename = ast->filename;
    if (FnSymbol* a = dynamic_cast<FnSymbol*>(ast))
      if (!a->defSetGet)
        apply_getters_setters(a);
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
      fix_user_assign(a);
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
  dtAnyInt->isGeneric = true;
  asts.clear();
  collect_asts_postorder(&asts, base);
  int changed = 1;
  while (changed) {
    changed = 0;
    forv_Vec(BaseAST, ast, asts) {
      if (FnSymbol *fn = dynamic_cast<FnSymbol*>(ast)) 
        changed = tag_generic(fn) || changed;
    }
  }

  asts.clear();
  collect_asts_postorder(&asts, base);
  forv_Vec(BaseAST, ast, asts) {
    if (FnSymbol *fn = dynamic_cast<FnSymbol*>(ast)) {
      tag_global(fn);
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

  asts.clear();
  collect_asts_postorder(&asts, base);
  forv_Vec(BaseAST, ast, asts) {
    if (FnSymbol* a = dynamic_cast<FnSymbol*>(ast)) {
      if (!a->isGeneric) {
        change_types_to_values(a);
      }
    }
  }
}


// Create formals for iterator class methods/functions and set _this.
static void
iterator_formals( FnSymbol *fn, ClassType *t, ArgSymbol *cursor=NULL) {
  fn->insertFormalAtTail( new DefExpr( new ArgSymbol( INTENT_BLANK,
                                                         "_yummyMethodToken",
                                                         dtMethodToken)));
  fn->_this = new ArgSymbol( INTENT_BLANK, "this", t);
  fn->insertFormalAtTail( new DefExpr( fn->_this));
  if (cursor) fn->insertFormalAtTail( new DefExpr( cursor));
}


// Create a field in the class for each local variable and replace uses.
static void
iterator_create_fields( FnSymbol *fn, ClassType *ic) {
  ArgSymbol   *_this = new ArgSymbol( INTENT_BLANK, "this", ic);

  // create a field for each formal
  for_alist( DefExpr, formal, fn->formals) {
    if (ArgSymbol *a = dynamic_cast<ArgSymbol*>(formal->sym)) {
      Expr    *etype= (formal->exprType) ? formal->exprType->copy() : NULL;
      consType const_type= (a->intent==INTENT_PARAM) ? VAR_PARAM : VAR_VAR;
      VarSymbol *newfield = new VarSymbol( a->name,
                                           dtUnknown,
                                           VAR_NORMAL,
                                           const_type);
      ic->fields->insertAtTail(new DefExpr(newfield,
                                           NULL,
                                           etype));
      // replace uses in body
      forv_Vec( SymExpr, se, a->uses) {
        se->replace( new CallExpr( ".", _this, new_StringSymbol( se->var->name)));

      }
    }
  }


  // create a field for each local
  Vec<BaseAST*> children;
  collect_asts( &children, fn->body);
  forv_Vec( BaseAST, ast, children) {
    if (DefExpr *def = dynamic_cast<DefExpr*>(ast)) {
      if (VarSymbol *v = dynamic_cast<VarSymbol*>(def->sym)) {
        if (v->isCompilerTemp)
          continue;

         Expr* def_init = def->init;
        Expr* def_type = def->exprType;
        def_init->remove();
        def_type->remove();

        // need to reset default value (make re-entrant)
        if (!def_init)
          def_init = new CallExpr("_init", new CallExpr(".", _this, new_StringSymbol(v->name)));
        def->replace(new CallExpr("=", new CallExpr(".", _this, new_StringSymbol(v->name)), def_init));

        if (def_type) {
          ic->fields->insertAtTail(new DefExpr(v, NULL, def_type->copy()));
        } else {
          ic->fields->insertAtTail(new DefExpr(v, NULL, new CallExpr(PRIMITIVE_TYPEOF, def_init->copy())));
        }

        // replace uses in body
        compute_sym_uses( fn);
        forv_Vec( SymExpr, se, v->uses) {  // replace each use
          se->replace( new CallExpr( ".", _this, new_StringSymbol(se->var->name)));
        }
      }
    }
  }

  // create formals
  for_formals(formal, fn)
    formal->defPoint->remove();

  fn->insertFormalAtTail( new DefExpr( new ArgSymbol( INTENT_BLANK,
                                                         "_yummyMethodToken",
                                                        dtMethodToken)));
  fn->_this = _this;
  fn->insertFormalAtTail( new DefExpr( fn->_this));
}


static ArgSymbol*
iterator_find_arg( char *name, AList *formals) {
  for_alist( DefExpr, de, formals) {
    if (ArgSymbol *a = dynamic_cast<ArgSymbol*>(de->sym)) {
      if (!strcmp( name, a->name)) {
        return a;
      }
    }
  }
  return NULL;
}


// Replace call expressions that reference this with the arg symbol in
// formals.
static void
iterator_constructor_fixup( ClassType *ct) {
  FnSymbol *fn = ct->defaultConstructor;

  for_alist( DefExpr, de, fn->formals) {
    Vec<BaseAST*> asts;
    asts.clear();
    collect_asts( &asts, de);
    forv_Vec( BaseAST, ast, asts) {
      if (CallExpr *ce = dynamic_cast<CallExpr*>( ast)) {
        if (ce->argList->length() > 0) {
          if (SymExpr *arg1e = dynamic_cast<SymExpr*>( ce->argList->get(1))) {
            Symbol *arg1 = arg1e->var;
            if (!strcmp( arg1->name, "this") && 
                (arg1e->typeInfo() == ct)) {
              if (SymExpr *arg2e = dynamic_cast<SymExpr*>( ce->argList->get(2))) {
                Symbol *arg2 = dynamic_cast<VarSymbol*>(arg2e->var);
                char* str;
                if (!get_string(arg2e, &str))
                  INT_FATAL(arg2e, "string literal expected");
                ArgSymbol *a = iterator_find_arg(str, fn->formals);
                if (!a) INT_FATAL( arg2, "could not find arg to replace with");
                ce->replace( new SymExpr( a));
              }
            }
          }
        }
      }
    }
  }
}


// Replace yield statements with a return and label.  Return both a vec of the
// replaced return statements which will be used when constructing getValue 
// and a vec of labels (for cursor returns).
static void
iterator_replace_yields( FnSymbol *fn, 
                         Vec<ReturnStmt*>  *vals_returned,
                         Vec<LabelSymbol*> *labels) {
  uint return_pt= 0;
  Vec<BaseAST*> children;
  LabelSymbol *l= new LabelSymbol( stringcat("return_", intstring( return_pt)));
  labels->add( l);  // base case

  collect_asts( &children, fn->body);
  forv_Vec( BaseAST, ast, children) {
    if (ReturnStmt *rs=dynamic_cast<ReturnStmt*>( ast)) {
      if (rs->yield) {
        return_pt++;
        rs->insertBefore( new ReturnStmt( new_IntSymbol( return_pt)));
        l = new LabelSymbol( stringcat( "return_", intstring( return_pt)));
        labels->add( l);
        rs->insertAfter(new DefExpr(l));
        vals_returned->add( dynamic_cast<ReturnStmt*>(rs->remove()));
      }
    }
  }
}


// Build and insert the jump table for getNextCursor method
static void
iterator_build_jtable( FnSymbol *fn, ArgSymbol *c, Vec<ReturnStmt*> *vals_returned, Vec<LabelSymbol*> *labels) {
  BlockStmt *b= new BlockStmt();
  LabelSymbol *l = labels->first();
  b->insertAtTail(new DefExpr(l)); 
  for( int retpt=0; retpt<=vals_returned->length(); retpt++) {
    b->insertAtHead( new CondStmt( new CallExpr( "==", c, new_IntSymbol( retpt)), new GotoStmt( goto_normal, l)));
    labels->remove( 0);
    l = labels->first();
  }
  fn->body->insertAtHead( b);
  fn->body->insertAtTail( new ReturnStmt( new_IntSymbol( vals_returned->length()+1)));
}


// Build the return value jump table (i.e., getValue method)
static void
iterator_build_vtable( FnSymbol *fn, ArgSymbol *c, Vec<ReturnStmt*> *vals_returned) {
  BlockStmt *b= build_chpl_stmt();
  uint retpt= 1;
  forv_Vec(Expr, stmt, *vals_returned) {
    b->insertAtTail( new CondStmt( new CallExpr( "==", c, new_IntSymbol( retpt)),
                                   stmt));
    retpt++;
  }
  fn->body->insertAtHead( b);
}


static void
iterator_update_this_uses( FnSymbol *fn, DefExpr *newdef, DefExpr *olddef) {
  ArgSymbol *newsym = dynamic_cast<ArgSymbol*>( newdef->sym);
  ArgSymbol *oldsym = dynamic_cast<ArgSymbol*>( olddef->sym);
  ASTMap replace;
  replace.put( oldsym, newsym);
  update_symbols( fn, &replace);
}


static void
iterator_method( FnSymbol *fn) {
  fn->fnClass = FN_FUNCTION;
  fn->isMethod = true;                // method of iterator class
  fn->global = true;                  // other modules need access
  fn->retType = dtUnknown;            // let resolve figures these out
  fn->retExprType = NULL;
}


static void
iterator_transform( FnSymbol *fn) {
  ModuleSymbol*m = fn->getModule();
  char        *classn = stringcat("_iterator_", fn->name);
  ClassType   *ic = new ClassType( CLASS_CLASS);
  TypeSymbol  *ict = new TypeSymbol( classn, ic);
  DefExpr     *ic_def = new DefExpr( ict);
  m->stmts->insertAtHead(ic_def);

  ArgSymbol *cursor = new ArgSymbol(INTENT_BLANK, "cursor", dtInt[INT_SIZE_64]);

  // create getNextCursor
  FnSymbol *nextcf = fn->copy();
  nextcf->name = nextcf->cname = canonicalize_string("getNextCursor");
  iterator_method( nextcf);
  m->stmts->insertAtHead(new DefExpr(nextcf));
  compute_sym_uses( nextcf);
  iterator_create_fields( nextcf, ic);
  nextcf->insertFormalAtTail( new DefExpr( cursor));
  Vec<ReturnStmt*> vals_returned;
  Vec<LabelSymbol*> labels;
  iterator_replace_yields( nextcf, &vals_returned, &labels);
  iterator_build_jtable( nextcf, cursor, &vals_returned, &labels);
  cleanup( ic_def->sym);
  normalize( ic_def);
  iterator_constructor_fixup( ic);
  ic->isIterator = true;
  
  FnSymbol *headcf = new FnSymbol( "getHeadCursor");
  iterator_method( headcf);
  m->stmts->insertAtHead(new DefExpr(headcf));
  iterator_formals( headcf, ic);
  headcf->body->insertAtHead( new ReturnStmt( new CallExpr( new CallExpr( ".", headcf->_this, new_StringSymbol( "getNextCursor")), new_IntSymbol(0))));

  FnSymbol *valuef = new FnSymbol( "getValue");
  iterator_method( valuef);
  m->stmts->insertAtHead(new DefExpr(valuef));
  iterator_formals( valuef, ic, cursor);
  iterator_build_vtable( valuef, cursor, &vals_returned);
  iterator_update_this_uses(valuef, 
                            valuef->getFormal(2)->defPoint, 
                            nextcf->getFormal(2)->defPoint);

  FnSymbol *isvalidcf = new FnSymbol( "isValidCursor?");
  iterator_method( isvalidcf);
  m->stmts->insertAtHead(new DefExpr(isvalidcf));
  isvalidcf->body->insertAtHead( new ReturnStmt( new CallExpr( "!=", cursor, new_IntSymbol(vals_returned.length()+1))));
  iterator_formals( isvalidcf, ic, cursor);

  // iterator -> wrapper function
  fn->fnClass = FN_FUNCTION;
  fn->retType = dtUnknown;
  fn->retExprType = NULL;
  AList actuals;
  for_alist( DefExpr, formal,  fn->formals) {
    ArgSymbol *a = dynamic_cast<ArgSymbol*>(formal->sym);
    actuals.insertAtTail( new SymExpr( a));
  }
  fn->body->replace( new BlockStmt( new ReturnStmt( new CallExpr( ic->defaultConstructor, &actuals))));
  normalize( fn->defPoint);
}


static void
enable_scalar_promotion( FnSymbol *fn) {
  Expr* seqType = fn->retExprType;
  if (!seqType)
    USR_FATAL(fn, "Cannot infer iterator return type yet");
  Type *seqElementType = seqType->typeInfo();
  
  if (scalar_promotion) {
    if (!strcmp("_promoter", fn->name)) {
      if (seqElementType != dtUnknown) {
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
                        for_fields(field, ct)
                          if (!strcmp(field->name, s))
                            field->addPragma("promoter");
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
  fn->defPoint->insertAfter(new DefExpr(new_fn));
  if (fn->_this)
    fn->_this->type->methods.add(new_fn);
  fn->buildSetter = false;
  new_fn->retType = dtVoid;
  new_fn->cname = stringcat("_setter_", fn->cname);
  ArgSymbol* setterToken = new ArgSymbol(INTENT_BLANK, "_st",
                                         dtSetterToken);
  ArgSymbol* lvalue = new ArgSymbol(INTENT_BLANK, "_lvalue", dtAny);
  Expr* exprType = NULL;
  if (new_fn->retExprType) {
    lvalue->type = dtUnknown;
    exprType = new_fn->retExprType;
    exprType->remove();
  }
  new_fn->insertFormalAtTail(new DefExpr(setterToken));
  new_fn->insertFormalAtTail(new DefExpr(lvalue, NULL, exprType));
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
    retval->isCompilerTemp = true;
    retval->canReference = true;
    fn->insertAtHead(new DefExpr(retval));
    fn->insertAtTail(new ReturnStmt(retval));
  }
  bool label_is_used = false;
  forv_Vec(ReturnStmt, ret, rets) {
    if (retval) {
      Expr* ret_expr = ret->expr;
      ret_expr->remove();
      if (fn->retExprType)
        ret_expr = new CallExpr(PRIMITIVE_CAST, fn->retExprType->copy(), ret_expr);
      ret->insertBefore(new CallExpr(PRIMITIVE_MOVE, retval, ret_expr));
    }
    if (ret->next != label->defPoint) {
      ret->replace(new GotoStmt(goto_normal, label));
      label_is_used = true;
    } else {
      ret->remove();
    }
  }
  if (!label_is_used)
    label->defPoint->remove();
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

  Vec<DefExpr*> tempDefs;
  ASTMap subs;

  for_formals_backward(formal, fn) {
    if (formal->intent == INTENT_REF || formal->intent == INTENT_PARAM)
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
      if (ClassType* ct = dynamic_cast<ClassType*>(ts->type)) {
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
  for_actuals(actual, call) {
    actual->remove();
    call->getStmtExpr()->insertBefore
      (new CallExpr(new_name, first_actual->copy(), actual));
  }
  call->getStmtExpr()->remove();
}


static void decompose_special_calls(CallExpr* call) {
  if (call->isResolved())
    return;
  if (!call->argList->isEmpty() > 0) {
    Expr* firstArg = dynamic_cast<Expr*>(call->argList->get(1));
    SymExpr* symArg = dynamic_cast<SymExpr*>(firstArg);
    // don't decompose method calls
    if (symArg && symArg->var == gMethodToken)
      return;
  }
  if (call->isNamed("fread")) {
    Expr* file = dynamic_cast<Expr*>(call->argList->get(1));
    file->remove();
    decompose_multi_actuals(call, "fread", file);
  } else if (call->isNamed("read")) {
    decompose_multi_actuals(call, "fread", new SymExpr(chpl_stdin));
  }
}


static bool can_resolve_type(Expr* type_expr) {
  if (!type_expr)
    return false;
  Type* type = type_expr->typeInfo();
  return type && type != dtUnknown && type != dtAny; // && type != dtNil;
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
  //   call(call or )( indicates partial
  Vec<BaseAST*> asts;
  collect_asts_postorder(&asts, fn);
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
      currentLineno = call->lineno;
      currentFilename = call->filename;
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
            if (var->immediate->const_kind == CONST_KIND_STRING)
              method = var->immediate->v_string;
        if (!method)
          INT_FATAL(call, "No method name for getter or setter");
        Expr* _this = call->get(1);
        _this->remove();
        CallExpr* getter = new CallExpr(method, gMethodToken, _this);
        getter->methodTag = true;
        call->replace(getter);
        if (CallExpr* parent = dynamic_cast<CallExpr*>(getter->parentExpr))
          if (parent->baseExpr == getter)
            getter->partialTag = true;
      } else if (call->isNamed("=")) {
        if (CallExpr* lhs = dynamic_cast<CallExpr*>(call->get(1))) {
          if (lhs->isNamed(".")) {
            char* method = NULL;
            if (SymExpr* symExpr = dynamic_cast<SymExpr*>(lhs->get(2)))
              if (VarSymbol* var = dynamic_cast<VarSymbol*>(symExpr->var))
                if (var->immediate->const_kind == CONST_KIND_STRING)
                  method = var->immediate->v_string;
            if (!method)
              INT_FATAL(call, "No method name for getter or setter");
            Expr* _this = lhs->get(1);
            _this->remove();
            Expr* rhs = call->get(2);
            rhs->remove();
            CallExpr* setter =
              new CallExpr(method, gMethodToken, _this, gSetterToken, rhs);
            call->replace(setter);
          } else {
            Expr* rhs = call->get(2);
            rhs->remove();
            lhs->remove();
            call->replace(lhs);
            lhs->insertAtTail(gSetterToken);
            lhs->insertAtTail(rhs);
          }
        }
      }
    }
  }
}


static void insert_call_temps(CallExpr* call) {
  if (!call->parentExpr || !call->getStmtExpr())
    return;

  if (call == call->getStmtExpr())
    return;
  
  if (dynamic_cast<DefExpr*>(call->parentExpr))
    return;

  if (call->partialTag)
    return;

  if (call->primitive)
    return;

  if (CallExpr* parentCall = dynamic_cast<CallExpr*>(call->parentExpr)) {
    if (parentCall->isPrimitive(PRIMITIVE_MOVE))
      return;
    if (parentCall->isNamed("_init"))
      call = parentCall;
  }

  Expr* stmt = call->getStmtExpr();
  VarSymbol* tmp = new VarSymbol("_tmp", dtUnknown, VAR_NORMAL, VAR_CONST);
  tmp->isCompilerTemp = true;
  tmp->canReference = true;
  call->replace(new SymExpr(tmp));
  stmt->insertBefore(new DefExpr(tmp));
  stmt->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, call));
}


static void fix_user_assign(CallExpr* call) {
  if (!call->parentExpr ||
      call->getStmtExpr() == call->parentExpr ||
      !call->isNamed("="))
    return;
  CallExpr* move = new CallExpr(PRIMITIVE_MOVE, call->get(1)->copy());
  call->replace(move);
  move->insertAtTail(call);
}


static void fix_def_expr(DefExpr* def) {
  if (def->exprType) {
    if (def->init) {
      // parameters can only be assigned once so cast the init
      // expression to the type
      if (VarSymbol* var = dynamic_cast<VarSymbol*>(def->sym)) {
        if (var->consClass == VAR_PARAM) {
          def->insertAfter(new CallExpr(PRIMITIVE_MOVE, def->sym, new CallExpr(PRIMITIVE_CAST, def->exprType->remove(), def->init->remove())));
          return;
        }
      }
      def->insertAfter(new CallExpr(PRIMITIVE_MOVE, def->sym, new CallExpr("=", def->sym, def->init->remove())));
    }
    VarSymbol* typeTemp = new VarSymbol("_typeTmp");
    typeTemp->isTypeVariable = true;
    typeTemp->isTypeVariable = true;
    def->insertBefore(new DefExpr(typeTemp));
    def->insertBefore(new CallExpr(PRIMITIVE_MOVE, typeTemp, new CallExpr("_init", def->exprType->remove())));
    def->insertAfter(new CallExpr(PRIMITIVE_MOVE, def->sym, typeTemp));
  } else if (def->init) {
    def->insertAfter(new CallExpr(PRIMITIVE_MOVE, def->sym, new CallExpr("_copy", def->init->remove())));
  }
}


VarSymbol*
cast_fold(TypeSymbol* ts, VarSymbol* var) {
  VarSymbol* typevar = dynamic_cast<VarSymbol*>(ts->type->defaultValue);
  if (!typevar || !typevar->immediate)
    INT_FATAL("unexpected case in cast_fold");
  Immediate coerce = *typevar->immediate;
  coerce_immediate(var->immediate, &coerce);
  return new_ImmediateSymbol(&coerce);
}


#define FOLD_CALL(name, prim)                             \
  if (call->isNamed(name)) {                              \
    Immediate i3;                                         \
    fold_constant(prim, i1, i2, &i3);                     \
    call->replace(new SymExpr(new_ImmediateSymbol(&i3))); \
    return true;                                          \
  }

#define FIND_PRIMITIVE_TYPE( prim_type, numoftypes, ptype_p)            \
  for (int type=0; type<numoftypes; type++) {                           \
    if (prim_type[type] &&                                              \
        (prim_type[type]->symbol == dynamic_cast<TypeSymbol*>(base->var))) { \
      ptype_p = prim_type;                                              \
      break;                                                            \
    }                                                                   \
  }


static bool
isType(Expr* expr) {
  if (SymExpr* sym = dynamic_cast<SymExpr*>(expr)) {
    if (sym->var->isTypeVariable)
      return true;
    if (dynamic_cast<TypeSymbol*>(sym->var))
      return true;
  } else if (CallExpr* call = dynamic_cast<CallExpr*>(expr)) {
    if (call->isPrimitive(PRIMITIVE_TYPEOF))
      return true;
  }
  return false;
}


static bool
isSubType(Type* sub, Type* super) {
  if (sub == super)
    return true;
  forv_Vec(Type, parent, sub->dispatchParents) {
    if (isSubType(parent, super))
      return true;
  }
  return false;
}


static bool fold_call_expr(CallExpr* call) {
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
      if (construct->isNamed("_build_array_type") ||
          construct->isNamed("_build_sparse_domain_type") ||
          construct->isNamed("_build_domain_type") ||
          construct->isNamed("_build_index_type")) {
        call->replace(construct->remove());
      } else if (FnSymbol* fn = dynamic_cast<FnSymbol*>(construct->isResolved())) {
        if (ClassType* ct = dynamic_cast<ClassType*>(fn->retType)) {
          if (!ct->isGeneric) {
            if (ct->defaultValue)
              call->replace(new CallExpr(PRIMITIVE_CAST, fn->retType->symbol, gNil));
            else
              call->replace(construct->remove());
          }
        }
      }
    } else if (SymExpr* sym = dynamic_cast<SymExpr*>(call->get(1))) {
      TypeSymbol* ts = dynamic_cast<TypeSymbol*>(sym->var);
      if (!ts && sym->var->isTypeVariable)
        ts = sym->var->type->symbol;
      if (ts && !ts->type->isGeneric) {
        if (ts->type->defaultValue)
          call->replace(new CallExpr(PRIMITIVE_CAST, ts, ts->type->defaultValue));
        else if (ts->type->defaultConstructor)
          call->replace(new CallExpr(ts->type->defaultConstructor));
        else
          INT_FATAL(ts, "type has neither defaultValue nor defaultConstructor");
      }
    }
    return false;
  }

  if (call->isNamed("_construct__tuple") && call->argList->length() > 1) {
    if (SymExpr* sym = dynamic_cast<SymExpr*>(call->get(1))) {
      if (VarSymbol* var = dynamic_cast<VarSymbol*>(sym->var)) {
        if (var->immediate) {
          int rank = var->immediate->v_int64;
          if (rank != call->argList->length() - 1) {
            if (call->argList->length() != 2)
              INT_FATAL(call, "bad homogeneous tuple");
            Expr* expr = call->get(2);
            for (int i = 1; i < rank; i++) {
              if (call->getStmtExpr()) {
                VarSymbol* tmp = new VarSymbol("_tmp");
                tmp->isCompilerTemp = true;
                tmp->canReference = true;
                call->getStmtExpr()->insertBefore(new DefExpr(tmp));
                call->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr("_copy", expr->copy())));
                call->insertAtTail(tmp);
              } else {
                call->insertAtTail(new CallExpr("_copy", expr->copy()));
              }
            }
          }
        }
      }
    }
  }

  // fold parameter methods
  if (call->argList->length() == 2) {
    if (SymExpr* symExpr = dynamic_cast<SymExpr*>(call->get(1))) {
      if (symExpr->var == gMethodToken) {
        Type* type = call->get(2)->typeInfo();
        Vec<BaseAST*> keys;
        type->substitutions.get_keys(keys);
        forv_Vec(BaseAST, key, keys) {
          if (Symbol* var = dynamic_cast<Symbol*>(key)) {
            if (call->isNamed(var->name)) {
              if (Symbol* value = dynamic_cast<Symbol*>(type->substitutions.get(key))) {
                call->replace(new SymExpr(value));
              } else if (Type* value = dynamic_cast<Type*>(type->substitutions.get(key))) {
                if (var->isTypeVariable)
                  call->replace(new SymExpr(value->symbol));
              }
            }
          } else if (Type* var = dynamic_cast<Type*>(key)) {
            INT_FATAL("type key encountered");
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

  if (call->isNamed("==")) {
    if (isType(call->get(1)) || isType(call->get(2))) {
      Type* lt = call->get(1)->typeInfo();
      Type* rt = call->get(2)->typeInfo();
      if (lt != dtUnknown && rt != dtUnknown &&
          !lt->isGeneric && !rt->isGeneric)
        call->replace((lt == rt) ? new SymExpr(gTrue) : new SymExpr(gFalse));
    }
  }
  if (call->isNamed("!=")) {
    if (isType(call->get(1)) || isType(call->get(2))) {
      Type* lt = call->get(1)->typeInfo();
      Type* rt = call->get(2)->typeInfo();
      if (lt != dtUnknown && rt != dtUnknown &&
          !lt->isGeneric && !rt->isGeneric)
        call->replace((lt != rt) ? new SymExpr(gTrue) : new SymExpr(gFalse));
    }
  }
  if (call->isPrimitive(PRIMITIVE_ISSUBTYPE)) {
    if (isType(call->get(1)) || isType(call->get(2))) {
      Type* lt = call->get(2)->typeInfo(); // a:t cast is cast(t,a)
      Type* rt = call->get(1)->typeInfo();
      if (lt != dtUnknown && rt != dtUnknown && lt != dtAny &&
          rt != dtAny && !lt->isGeneric) {
        bool is_true = false;
        if (lt->instantiatedFrom == rt)
          is_true = true;
        if (isSubType(lt, rt))
          is_true = true;
        call->replace((is_true) ? new SymExpr(gTrue) : new SymExpr(gFalse));
      }
    }
  }

  // replace call expr of primitive type with appropriate primitive type
  if (call->argList->length() == 1) {
    if (SymExpr* base = dynamic_cast<SymExpr*>(call->baseExpr)) {
      PrimitiveType **ptype_p = NULL;

      FIND_PRIMITIVE_TYPE( dtInt, INT_SIZE_NUM, ptype_p);
      if (!ptype_p) {
        FIND_PRIMITIVE_TYPE( dtUInt, INT_SIZE_NUM, ptype_p);
        if (!ptype_p) {
          FIND_PRIMITIVE_TYPE( dtReal, FLOAT_SIZE_NUM, ptype_p);
          if (!ptype_p) {
            FIND_PRIMITIVE_TYPE( dtComplex, COMPLEX_SIZE_NUM, ptype_p);
            if (!ptype_p) {
              FIND_PRIMITIVE_TYPE( dtImag, FLOAT_SIZE_NUM, ptype_p);
            }
          }
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
                  case  8: tsize = dtInt[INT_SIZE_8]->symbol;  break;
                  case 16: tsize = dtInt[INT_SIZE_16]->symbol; break;
                  case 32: tsize = dtInt[INT_SIZE_32]->symbol; break;
                  case 64: tsize = dtInt[INT_SIZE_64]->symbol; break;
                  default:
                    USR_FATAL( call, "illegal size %d for int", size);
                  }
                  call->replace( new SymExpr(tsize));
                } else if (ptype_p == dtUInt) {
                  switch (size) {
                  case  8: tsize = dtUInt[INT_SIZE_8]->symbol;  break;
                  case 16: tsize = dtUInt[INT_SIZE_16]->symbol; break;
                  case 32: tsize = dtUInt[INT_SIZE_32]->symbol; break;
                  case 64: tsize = dtUInt[INT_SIZE_64]->symbol; break;
                  default:
                    USR_FATAL( call, "illegal size %d for uint", size);
                  }
                  call->replace( new SymExpr(tsize));
                } else if (ptype_p == dtReal) {
                  switch (size) {
                  case 32:  tsize = dtReal[FLOAT_SIZE_32]->symbol;  break;
                  case 64:  tsize = dtReal[FLOAT_SIZE_64]->symbol;  break;
                  case 128: tsize = dtReal[FLOAT_SIZE_128]->symbol; break;
                  default:
                    USR_FATAL( call, "illegal size %d for real", size);
                  }
                  call->replace( new SymExpr(tsize));
                } else if (ptype_p == dtImag) {
                  switch (size) {
                  case 32:  tsize = dtImag[FLOAT_SIZE_32]->symbol;  break;
                  case 64:  tsize = dtImag[FLOAT_SIZE_64]->symbol;  break;
                  case 128: tsize = dtImag[FLOAT_SIZE_128]->symbol; break;
                  default:
                    USR_FATAL( call, "illegal size %d for imag", size);
                  }
                  call->replace( new SymExpr(tsize));
                } else if (ptype_p == dtComplex) {
                  switch (size) {
                  case 64:  tsize = dtComplex[COMPLEX_SIZE_64]->symbol;  break;
                  case 128: tsize = dtComplex[COMPLEX_SIZE_128]->symbol; break;
                  case 256: tsize = dtComplex[COMPLEX_SIZE_256]->symbol; break;
                  default:
                    USR_FATAL( call, "illegal size %d for complex", size);
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

  if (call->isPrimitive(PRIMITIVE_CAST)) {
    if (SymExpr* sym = dynamic_cast<SymExpr*>(call->get(2))) {
      if (VarSymbol* var = dynamic_cast<VarSymbol*>(sym->var)) {
        if (var->immediate) {
          if (SymExpr* sym = dynamic_cast<SymExpr*>(call->get(1))) {
            if (TypeSymbol* ts = dynamic_cast<TypeSymbol*>(sym->var)) {
              if (!is_imag_type(ts->type) && 
                  !is_complex_type(ts->type) && ts->type != dtString) {
                SymExpr* s = new SymExpr(cast_fold(ts, var));
                call->replace(s);
                return true;
              }
            }
          }
        }
      }
    }
  }

  if (call->argList->length() == 2) {
    SymExpr* a1 = dynamic_cast<SymExpr*>(call->get(1));
    SymExpr* a2 = dynamic_cast<SymExpr*>(call->get(2));
    if (a1 && a2) {
      VarSymbol* v1 = dynamic_cast<VarSymbol*>(a1->var);
      VarSymbol* v2 = dynamic_cast<VarSymbol*>(a2->var);
      if (v1 && v2) {
        Immediate* i1 = v1->immediate;
        Immediate* i2 = v2->immediate;
        if (i1 && i2) {
          if (call->get(1)->typeInfo() == dtString ||
              is_real_type(call->get(1)->typeInfo()) ||
              is_imag_type(call->get(1)->typeInfo()) ||
              is_complex_type(call->get(1)->typeInfo()) ||
              call->get(2)->typeInfo() == dtString ||
              is_real_type(call->get(2)->typeInfo()) ||
              is_imag_type(call->get(2)->typeInfo()) ||
              is_complex_type(call->get(2)->typeInfo()))
            return false;
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
          FOLD_CALL("<<", P_prim_lsh);
          FOLD_CALL(">>", P_prim_rsh);
          FOLD_CALL("**", P_prim_pow);
          if (call->isNamed("=")) {
            call->replace(new SymExpr(v2));
            return true;
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
          if (call->get(1)->typeInfo() == dtString ||
              is_real_type(call->get(1)->typeInfo()) ||
              is_imag_type(call->get(1)->typeInfo()) ||
              is_complex_type(call->get(1)->typeInfo()))
            return false;
          FOLD_CALL("+", P_prim_plus);
          FOLD_CALL("-", P_prim_minus);
          FOLD_CALL("~", P_prim_not);
          FOLD_CALL("!", P_prim_lnot);
        }

        /* look for 8? */
      }
    }
  }

  return false;
}


static bool fold_def_expr(DefExpr* def) {
  Symbol* value = NULL;
  CallExpr* move = NULL;
  VarSymbol* defSymVar = dynamic_cast<VarSymbol*>(def->sym);
  if (def->sym->isParam() || 
      (def->sym->isConst() && defSymVar && defSymVar->varClass != VAR_CONFIG)) {
    forv_Vec(SymExpr, sym, def->sym->uses) {
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
        if (!def->sym->isParam() && call->isNamed("=") && call->get(1) == sym) {
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
    move->getStmtExpr()->remove();
    forv_Vec(SymExpr, sym, def->sym->uses) {
      sym->var = value;
    }
    def->remove();
    return true;
  } else
    return false;
}

static void fold_cond_stmt(CondStmt* if_stmt) {
  if (SymExpr* cond = dynamic_cast<SymExpr*>(if_stmt->condExpr)) {
    if (VarSymbol* var = dynamic_cast<VarSymbol*>(cond->var)) {
      if (var->immediate &&
          var->immediate->const_kind == NUM_KIND_UINT &&
          var->immediate->num_index == INT_SIZE_1) {
        if (var->immediate->v_bool == gTrue->immediate->v_bool) {
          Expr* then_stmt = if_stmt->thenStmt;
          then_stmt->remove();
          if_stmt->replace(then_stmt);
        } else if (var->immediate->v_bool == gFalse->immediate->v_bool) {
          Expr* else_stmt = if_stmt->elseStmt;
          if (else_stmt) {
            else_stmt->remove();
            if_stmt->replace(else_stmt);
          } else {
            if_stmt->remove();
          }
        }
      }
    }
  }
}

static void fold_param_for(CallExpr* loop) {
  BlockStmt* block = dynamic_cast<BlockStmt*>(loop->parentExpr);
  if (!block || block->blockTag != BLOCK_PARAM_FOR)
    INT_FATAL(loop, "bad param loop primitive");
  if (loop && loop->isPrimitive(PRIMITIVE_LOOP_PARAM)) {
    if (SymExpr* lse = dynamic_cast<SymExpr*>(loop->get(2))) {
      if (SymExpr* hse = dynamic_cast<SymExpr*>(loop->get(3))) {
        if (SymExpr* sse = dynamic_cast<SymExpr*>(loop->get(4))) {
          if (VarSymbol* lvar = dynamic_cast<VarSymbol*>(lse->var)) {
            if (VarSymbol* hvar = dynamic_cast<VarSymbol*>(hse->var)) {
              if (VarSymbol* svar = dynamic_cast<VarSymbol*>(sse->var)) {
                if (lvar->immediate && hvar->immediate && svar->immediate) {
                  int low = lvar->immediate->v_int64;
                  int high = hvar->immediate->v_int64;
                  int stride = svar->immediate->v_int64;
                  Expr* index_expr = loop->get(1);
                  loop->remove();
                  block->blockTag = BLOCK_NORMAL;
                  Symbol* index = dynamic_cast<SymExpr*>(index_expr)->var;
                  if (stride < 0)
                    INT_FATAL("fix this");
                  for (int i = low; i <= high; i += stride) {
                    VarSymbol* new_index = new VarSymbol(index->name);
                    new_index->consClass = VAR_PARAM;
                    block->insertBefore(new DefExpr(new_index, new_IntSymbol(i)));
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
      if (CallExpr* a = dynamic_cast<CallExpr*>(ast)) {
        if (a->parentSymbol) {// in ast
          if (a->isPrimitive(PRIMITIVE_LOOP_PARAM))
            fold_param_for(a);
          else
            change |= fold_call_expr(a);
        }
      }
      if (CondStmt* a = dynamic_cast<CondStmt*>(ast))
        if (a->parentSymbol) // in ast
          fold_cond_stmt(a);
    }
    compute_sym_uses(base);
    forv_Vec(DefExpr, def, defs) {
      if (def->parentSymbol)
        change |= fold_def_expr(def);
    }
    change_types_to_values(base);
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
      if (call->partialTag) {
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
  for_formals(arg, fn) {
    if (DefExpr* def = dynamic_cast<DefExpr*>(arg->variableExpr)) {
      // recursively handle variable argument list where number of
      // variable arguments is one or more as in ...?k
      if (max_actuals == 0)
        compute_max_actuals();
      for (int i = 1; i <= max_actuals; i++) {
        arg->variableExpr->replace(new SymExpr(new_IntSymbol(i)));
        FnSymbol* new_fn = fn->copy();
        fn->defPoint->insertBefore(new DefExpr(new_fn));
        DefExpr* new_def = def->copy();
        new_def->init = new SymExpr(new_IntSymbol(i));
        new_fn->insertAtHead(new_def);
        ASTMap update;
        update.put(def->sym, new_def->sym);
        update_symbols(new_fn, &update);
        expand_var_args(new_fn);
      }
      fn->defPoint->remove();
    } else if (SymExpr* sym = dynamic_cast<SymExpr*>(arg->variableExpr)) {
      // handle expansion of variable argument list where number of
      // variable arguments is a parameter
      if (VarSymbol* n_var = dynamic_cast<VarSymbol*>(sym->var)) {
        if (n_var->type == dtInt[INT_SIZE_32] && n_var->immediate) {
          int n = n_var->immediate->v_int64;
          CallExpr* tupleCall = new CallExpr("_tuple");
          for (int i = 0; i < n; i++) {
            DefExpr* new_arg_def = arg->defPoint->copy();
            ArgSymbol* new_arg = dynamic_cast<ArgSymbol*>(new_arg_def->sym);
            new_arg->variableExpr = NULL;
            tupleCall->insertAtTail(new SymExpr(new_arg));
            new_arg->name = astr("_e", intstring(i), "_", arg->name);
            new_arg->cname = stringcat("_e", intstring(i), "_", arg->cname);
            arg->defPoint->insertBefore(new_arg_def);
          }
          VarSymbol* var = new VarSymbol(arg->name);
          tupleCall->insertAtHead(new_IntSymbol(n));
          fn->insertAtHead(new DefExpr(var, tupleCall));
          arg->defPoint->remove();
          ASTMap update;
          update.put(arg, var);
          update_symbols(fn, &update);
          build(fn);
        }
      }
    }
  }
}


static void hack_resolve_types(Expr* expr) {
  if (DefExpr* def = dynamic_cast<DefExpr*>(expr)) {
    if (ArgSymbol* arg = dynamic_cast<ArgSymbol*>(def->sym)) {
      if (arg->type == dtUnknown && can_resolve_type(def->exprType)) {
        arg->type = def->exprType->typeInfo();
        def->exprType->remove();
      }
    }
  }
}

static bool
hasGenericArgs(FnSymbol* fn) {
  for_formals(formal, fn) {
    if (formal->type->isGeneric)
      return true;
    if (formal->defPoint->exprType &&
        formal->defPoint->exprType->typeInfo()->isGeneric)
      return true;
    if (formal->intent == INTENT_PARAM)
      return true;
  }
  return false;
}

static int
tag_generic(FnSymbol* fn) {
  if (fn->isGeneric)
    return 0;
  if (hasGenericArgs(fn)) {
    fn->isGeneric = 1; 
    if (fn->retType != dtUnknown && fn->fnClass == FN_CONSTRUCTOR)
      fn->retType->isGeneric = true;
    return 1;
  }
  return 0;
}

static void
tag_hasVarArgs(FnSymbol* fn) {
  fn->hasVarArgs = false;
  if (fn->hasPragma("tuple"))
    if (fn->isGeneric) // not if instantiated
      fn->hasVarArgs = true;
  for_formals(formal, fn)
    if (formal->variableExpr)
      fn->hasVarArgs = true;
}

static void tag_global(FnSymbol* fn) {
  if (fn->global)
    return;
  for_formals(formal, fn) {
    if (ClassType* ct = dynamic_cast<ClassType*>(formal->type))
      if (ct->classTag == CLASS_CLASS &&
          !ct->symbol->hasPragma("domain") &&
          !ct->symbol->hasPragma("array"))
        fn->global = true;
    if (SymExpr* sym = dynamic_cast<SymExpr*>(formal->defPoint->exprType))
      if (ClassType* ct = dynamic_cast<ClassType*>(sym->var->type))
        if (ct->classTag == CLASS_CLASS &&
            !ct->symbol->hasPragma("domain") &&
            !ct->symbol->hasPragma("array"))
          fn->global = true;
  }
  if (fn->global) {
    fn->parentScope->removeVisibleFunction(fn);
    rootScope->addVisibleFunction(fn);
    if (dynamic_cast<FnSymbol*>(fn->defPoint->parentSymbol)) {
      ModuleSymbol* mod = fn->getModule();
      Expr* def = fn->defPoint;
      CallExpr* noop = new CallExpr(PRIMITIVE_NOOP);
      def->insertBefore(noop);
      fn->visiblePoint = noop;
      def->remove();
      mod->stmts->insertAtTail(def);
    }
  }
}

static void
change_types_to_values(BaseAST* base) {
  Vec<BaseAST*> asts;
  collect_top_asts(&asts, base);
  forv_Vec(BaseAST, ast, asts) {
    if (SymExpr* sym = dynamic_cast<SymExpr*>(ast)) {
      if (CallExpr* call = dynamic_cast<CallExpr*>(sym->parentExpr)) {
        if (call->isPrimitive(PRIMITIVE_CAST))
          continue;
        if (call->isPrimitive(PRIMITIVE_CHPL_ALLOC))
          continue;
      }
      if (TypeSymbol* type = dynamic_cast<TypeSymbol*>(sym->var)) {
        CallExpr* typecall = NULL;
        if (type->type->defaultValue)
          typecall = new CallExpr(PRIMITIVE_CAST, type, type->type->defaultValue);
        else if (type->type->defaultConstructor)
          typecall = new CallExpr(type->type->defaultConstructor);
        else
          INT_FATAL(type, "Bad type");
        if (sym->getStmtExpr()) {
          VarSymbol* typeTemp = new VarSymbol("_typeTmp", type->type);
          typeTemp->isCompilerTemp = true;
          typeTemp->isTypeVariable = true;
          sym->getStmtExpr()->insertBefore(new DefExpr(typeTemp));
          sym->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, typeTemp, typecall));
          sym->replace(new SymExpr(typeTemp));
        } else {
          sym->replace(typecall);
        }
      }
    }
  }
}



static void fixup_array_formals(FnSymbol* fn) {
  if (fn->normalizedOnce)
    return;
  fn->normalizedOnce = true;
  Vec<BaseAST*> asts;
  collect_asts(&asts, fn);
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
      if (call->isNamed("_build_array_type")) {
        SymExpr* sym = dynamic_cast<SymExpr*>(call->get(1));
        DefExpr* def = dynamic_cast<DefExpr*>(call->get(1));
        DefExpr* parent = dynamic_cast<DefExpr*>(call->parentExpr);
        if (call->argList->length() == 1)
          if (!parent || !dynamic_cast<ArgSymbol*>(parent->sym) ||
              parent->exprType != call)
            USR_FATAL(call, "array without element type can only "
                      "be used as a formal argument type");
        if (def || (sym && sym->var == gNil) || call->argList->length() == 1) {
          if (!parent || !dynamic_cast<ArgSymbol*>(parent->sym)
              || parent->exprType != call)
            USR_FATAL(call, "array with empty or queried domain can "
                      "only be used as a formal argument type");
          parent->exprType->replace(new SymExpr(chpl_array));
          if (!fn->where) {
            fn->where = new BlockStmt(new SymExpr(gTrue));
            insert_help(fn->where, NULL, fn, fn->argScope);
          }
          Expr* expr = fn->where->body->only();
          if (call->argList->length() == 2)
            expr->replace(new CallExpr("&&", expr->copy(),
                            new CallExpr("==", call->get(2)->remove(),
                              new CallExpr(".", parent->sym, new_StringSymbol("elt_type")))));
          if (def) {
            forv_Vec(BaseAST, ast, asts) {
              if (SymExpr* sym = dynamic_cast<SymExpr*>(ast)) {
                if (sym->var == def->sym)
                  sym->replace(new CallExpr(".", parent->sym, new_StringSymbol("dom")));
              }
            }
          } else if (!sym || sym->var != gNil) {
            VarSymbol* tmp = new VarSymbol(stringcat("_view_", parent->sym->name));
            forv_Vec(BaseAST, ast, asts) {
              if (SymExpr* sym = dynamic_cast<SymExpr*>(ast)) {
                if (sym->var == parent->sym)
                  sym->var = tmp;
              }
            }
            fn->insertAtHead(new CondStmt(
                               new SymExpr(parent->sym),
                                 new CallExpr(PRIMITIVE_MOVE, tmp,
                                   new CallExpr(new CallExpr(".", parent->sym,
                                     new_StringSymbol("view")),
                                     call->get(1)->copy()))));
            fn->insertAtHead(new CallExpr(PRIMITIVE_MOVE, tmp, parent->sym));
            fn->insertAtHead(new DefExpr(tmp));
          }
        } else {  //// DUPLICATED CODE ABOVE AND BELOW
          DefExpr* parent = dynamic_cast<DefExpr*>(call->parentExpr);
          if (parent && dynamic_cast<ArgSymbol*>(parent->sym) && parent->exprType == call) {
            VarSymbol* tmp = new VarSymbol(stringcat("_view_", parent->sym->name));
            forv_Vec(BaseAST, ast, asts) {
              if (SymExpr* sym = dynamic_cast<SymExpr*>(ast)) {
                if (sym->var == parent->sym)
                  sym->var = tmp;
              }
            }
            fn->insertAtHead(new CondStmt(
                               new SymExpr(parent->sym),
                                 new CallExpr(PRIMITIVE_MOVE, tmp,
                                   new CallExpr(new CallExpr(".", parent->sym,
                                     new_StringSymbol("view")),
                                     call->get(1)->copy()))));
            fn->insertAtHead(new CallExpr(PRIMITIVE_MOVE, tmp, parent->sym));
            fn->insertAtHead(new DefExpr(tmp));
          }
        }
      }
    }
  }
}


static void clone_parameterized_primitive_methods(FnSymbol* fn) {
  if (dynamic_cast<ArgSymbol*>(fn->_this)) {
    if (fn->_this->type == dtInt[INT_SIZE_32]) {
      for (int i=INT_SIZE_1; i<INT_SIZE_NUM; i++) {
        if (dtInt[i] && i != INT_SIZE_32) {
          FnSymbol* nfn = fn->copy();
          nfn->_this->type = dtInt[i];
          fn->defPoint->insertBefore(new DefExpr(nfn));
        }
      }
    }
    if (fn->_this->type == dtUInt[INT_SIZE_32]) {
      for (int i=INT_SIZE_1; i<INT_SIZE_NUM; i++) {
        if (dtUInt[i] && i != INT_SIZE_32) {
          FnSymbol* nfn = fn->copy();
          nfn->_this->type = dtUInt[i];
          fn->defPoint->insertBefore(new DefExpr(nfn));
        }
      }
    }
    if (fn->_this->type == dtReal[FLOAT_SIZE_64]) {
      for (int i=FLOAT_SIZE_16; i<FLOAT_SIZE_NUM; i++) {
        if (dtReal[i] && i != FLOAT_SIZE_64) {
          FnSymbol* nfn = fn->copy();
          nfn->_this->type = dtReal[i];
          fn->defPoint->insertBefore(new DefExpr(nfn));
        }
      }
    }
    if (fn->_this->type == dtImag[FLOAT_SIZE_64]) {
      for (int i=FLOAT_SIZE_16; i<FLOAT_SIZE_NUM; i++) {
        if (dtImag[i] && i != FLOAT_SIZE_64) {
          FnSymbol* nfn = fn->copy();
          nfn->_this->type = dtImag[i];
          fn->defPoint->insertBefore(new DefExpr(nfn));
        }
      }
    }
    if (fn->_this->type == dtComplex[COMPLEX_SIZE_128]) {
      for (int i=COMPLEX_SIZE_32; i<COMPLEX_SIZE_NUM; i++) {
        if (dtComplex[i] && i != COMPLEX_SIZE_128) {
          FnSymbol* nfn = fn->copy();
          nfn->_this->type = dtComplex[i];
          fn->defPoint->insertBefore(new DefExpr(nfn));
        }
      }
    }
  }
}


static void
clone_for_parameterized_primitive_formals(FnSymbol* fn,
                                          DefExpr* def,
                                          int width) {
  compute_sym_uses(fn);
  ASTMap map;
  FnSymbol* newfn = fn->copy(&map);
  DefExpr* newdef = dynamic_cast<DefExpr*>(map.get(def));
  newdef->replace(new SymExpr(new_IntSymbol(width)));
  forv_Vec(SymExpr, sym, def->sym->uses) {
    SymExpr* newsym = dynamic_cast<SymExpr*>(map.get(sym));
    newsym->var = new_IntSymbol(width);
  }
  fn->defPoint->insertAfter(new DefExpr(newfn));
  fixup_parameterized_primitive_formals(newfn);
}

static void
fixup_parameterized_primitive_formals(FnSymbol* fn) {
  Vec<BaseAST*> asts;
  collect_top_asts(&asts, fn);
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* call = dynamic_cast<CallExpr*>(ast)) {
      if (call->argList->length() != 1)
        continue;
      if (DefExpr* def = dynamic_cast<DefExpr*>(call->get(1))) {
        if (call->isNamed("int") || call->isNamed("uint")) {
          for( int i=INT_SIZE_1; i<INT_SIZE_NUM; i++)
            if (dtInt[i])
              clone_for_parameterized_primitive_formals(fn, def,
                                                        get_width(dtInt[i]));
          fn->defPoint->remove();
        } else if (call->isNamed("real") || call->isNamed("imag")) {
          for( int i=FLOAT_SIZE_16; i<FLOAT_SIZE_NUM; i++)
            if (dtReal[i])
              clone_for_parameterized_primitive_formals(fn, def,
                                                        get_width(dtReal[i]));
          fn->defPoint->remove();
        } else if (call->isNamed("complex")) {
          for( int i=COMPLEX_SIZE_32; i<COMPLEX_SIZE_NUM; i++)
            if (dtComplex[i])
              clone_for_parameterized_primitive_formals(fn, def,
                                                        get_width(dtComplex[i]));
          fn->defPoint->remove();
        }
      }
    }
  }
}


static void change_method_into_constructor(FnSymbol* fn) {
  if (fn->formals->length() <= 1)
    return;
  if (fn->getFormal(1)->type != dtMethodToken)
    return;
  if (fn->getFormal(2)->type == dtUnknown)
    INT_FATAL(fn, "this argument has unknown type");
  if (strcmp(fn->getFormal(2)->type->symbol->name, fn->name))
    return;
  ClassType* ct = dynamic_cast<ClassType*>(fn->getFormal(2)->type);
  if (!ct)
    INT_FATAL(fn, "constructor on non-class type");
  fn->name = canonicalize_string(stringcat("_construct_", fn->name));
  fn->_this = new VarSymbol("this", ct);
  fn->insertAtHead(new CallExpr(PRIMITIVE_MOVE, fn->_this, new CallExpr(ct->symbol)));
  fn->insertAtHead(new DefExpr(fn->_this));
  fn->insertAtTail(new ReturnStmt(new SymExpr(fn->_this)));
  ASTMap map;
  map.put(fn->getFormal(2), fn->_this);
  fn->formals->get(2)->remove();
  fn->formals->get(1)->remove();
  update_symbols(fn, &map);
  ct->symbol->defPoint->insertBefore(fn->defPoint->remove());
}
