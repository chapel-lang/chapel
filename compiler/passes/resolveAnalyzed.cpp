#include "astutil.h"
#include "build.h"
#include "expr.h"
#include "passes.h"
#include "runtime.h"
#include "stmt.h"
#include "symbol.h"
#include "symtab.h"
#include "stringutil.h"


static void resolve_type(Symbol* sym);
static void resolve_symbol(CallExpr* call);


void resolve_analyzed(void) {
  Vec<BaseAST*> asts;
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts) {
    if (TypeSymbol* ts = dynamic_cast<TypeSymbol*>(ast)) {
      if (!type_is_used(ts))
        ts->defPoint->parentStmt->remove();
    } else if (FnSymbol* fs = dynamic_cast<FnSymbol*>(ast)) {
      if (!function_is_used(fs))
        fs->defPoint->parentStmt->remove();
    }
  }

  asts.clear();
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts) {
    if (ArgSymbol* as = dynamic_cast<ArgSymbol*>(ast)) {
      if (as->defaultExpr)
        as->defaultExpr->remove();
      if (as->defPoint->exprType)
        as->defPoint->exprType->remove();
    }
  }

  forv_Vec(BaseAST, ast, asts) {
    if (Symbol* a = dynamic_cast<Symbol*>(ast)) {
      if (!dynamic_cast<LabelSymbol*>(a))
        resolve_type(a);
    }
  }

  asts.clear();
  collect_asts(&asts);
  forv_Vec(BaseAST, ast, asts) {
    if (TypeSymbol* ts = dynamic_cast<TypeSymbol*>(ast)) {
      if (ClassType *ct = dynamic_cast<ClassType *>(ts->definition)) {
        if (ct->typeParents.in(dtClosure)) {
          Vec<Type *> types;
          for (int i = 0;; i++) {
            int offset;
            Type *type = 0;
            if (resolve_member(ct, i, &offset, &type) < 0)
              break;
            types.add(type);
          }
          types.add(dtInt[IF1_INT_TYPE_64]); // context
          complete_closure(ct, types);
        }
      }
    }
  }
  build(closureModule);

  asts.clear();
  collect_asts_postorder(&asts);
  forv_Vec(BaseAST, ast, asts) {
    if (CallExpr* a = dynamic_cast<CallExpr*>(ast)) {
      resolve_symbol(a);
    }
  }

  remove_named_exprs();
  remove_static_actuals();
  remove_static_formals();
}


static void resolve_type(Symbol* sym) {
  if (FnSymbol* fn = dynamic_cast<FnSymbol*>(sym)) {
    fn->retType = return_type_info(fn);
    if (fn->retType == dtUnknown)
      INT_FATAL(fn, "Analysis unable to determine return type of function");
  } else if (!dynamic_cast<UnresolvedSymbol*>(sym) &&
             !dynamic_cast<ModuleSymbol*>(sym)) {
    Type* analysis_type = type_info(sym);
    if (!(dynamic_cast<EnumType*>(sym->type) && analysis_type == dtInt[IF1_INT_TYPE_64]))
      sym->type = analysis_type;
    if (sym->type == dtUnknown)
      INT_FATAL(sym, "Analysis unable to determine type of symbol");
  }
}

static Expr *
build_dispatch_tree(CallExpr *call, AList<Expr> *arguments, Expr *context, Vec<FnSymbol*> &fns, 
                    Vec<Vec<Vec<Type *> *> *> &dispatch, Vec<Vec<int> *> &dispatch_context, int a);

static Expr *
build_dispatch_tree_cond(CallExpr *call, AList<Expr> *arguments, Expr *context, 
                         Expr *cond_expr, Vec<FnSymbol*> &true_fns, Vec<FnSymbol*> &false_fns, 
                         Vec<Vec<Vec<Type *> *> *> &dispatch, Vec<Vec<int> *> &dispatch_context, int a) 
{
  Expr *true_expr = 0, *false_expr = 0;
  if (true_fns.set_count() == 1)
    true_expr = new CallExpr(true_fns.first(), arguments->copy());
  else
    true_expr = build_dispatch_tree(call, arguments, context, true_fns, dispatch, dispatch_context, a + 1);
  if (false_fns.set_count() == 1)
    false_expr = new CallExpr(false_fns.first(), arguments->copy());
  else
    false_expr = build_dispatch_tree(call, arguments, context, false_fns, dispatch, dispatch_context, a);
  FnSymbol* if_fn = build_if_expr(cond_expr, true_expr, false_expr);
  if_fn->retType = true_expr->typeInfo();
  if_fn->retRef = false;
  call->parentStmt->insertBefore(new DefExpr(if_fn));
  normalize(if_fn);
  return new CallExpr(if_fn);
}

static Expr *
build_dispatch_tree(CallExpr *call, AList<Expr> *arguments, Expr *context, Vec<FnSymbol*> &fns, 
                    Vec<Vec<Vec<Type *> *> *> &dispatch, Vec<Vec<int> *> &dispatch_context, int a) 
{
  if (a >= dispatch.v[0]->n) {
    if (a >= dispatch.v[0]->n + 1) {
      USR_FATAL(call, "Unable to build table dispatch for ambiguous call");
    }
    if (fns.set_count() != 1) {
      Vec<int> *c = 0;
      for (int i = 0; i < fns.n; i++) {
        if (fns.v[i]) {
          if (!c) {
            c = dispatch_context.v[i];
            continue;
          }
          Vec<int> diff;
          c->set_disjunction(*dispatch_context.v[i], diff);
          if (diff.n) {
            int cc = diff.first();
            Vec<FnSymbol *> true_fns, false_fns;
            true_fns.fill(fns.n);
            false_fns.fill(fns.n);
            for (int i = 0; i < fns.n; i++) {
              if (fns.v[i]) {
                if (dispatch_context.v[i]->set_in(cc))
                  true_fns.v[i] = fns.v[i];
                else
                  false_fns.v[i] = fns.v[i];
              }
            }
            return build_dispatch_tree_cond(
              call, arguments, context,
              new CallExpr(PRIMITIVE_EQUAL, context->copy(), new SymExpr(new_IntSymbol(cc))),
              true_fns, false_fns, dispatch, dispatch_context, a);
          }
        }
      }
    }
    return new CallExpr(fns.first(), arguments->copy());
  }
  Vec<Type *> *t = 0;
  for (int i = 0; i < fns.n; i++) {
    if (fns.v[i]) {
      if (!t) {
        t = dispatch.v[i]->v[a];
        continue;
      }
      Vec<Type *> diff;
      t->set_disjunction(*dispatch.v[i]->v[a], diff);
      if (diff.n) {
        Type *type = diff.first();
        Vec<FnSymbol *> true_fns, false_fns;
        true_fns.fill(fns.n);
        false_fns.fill(fns.n);
        for (int i = 0; i < fns.n; i++) {
          if (fns.v[i]) {
            if (dispatch.v[i]->v[a]->set_in(type))
              true_fns.v[i] = fns.v[i];
            else
              false_fns.v[i] = fns.v[i];
          }
        }
        return build_dispatch_tree_cond(
          call, arguments, context,
          new CallExpr(PRIMITIVE_TYPE_EQUAL, new SymExpr(type->symbol), arguments->get(a)->copy()),
          true_fns, false_fns, dispatch, dispatch_context, a);
      }
    }
  }
  return build_dispatch_tree(call, arguments, context, fns, dispatch, dispatch_context, a+1);
}

static void
dynamic_dispatch(CallExpr *call, AList<Expr> *arguments, Expr *context = 0) {
  Vec<Vec<Vec<Type *> *> *> dispatch;
  Vec<Vec<int> *> dispatch_context;
  Vec<FnSymbol*> fns;
  call_info(call, fns, &dispatch, &dispatch_context);
  // NOTE: this call used FORTRAN NUMBERING to match the implementation of AList
  Expr *new_expr = build_dispatch_tree(call, arguments, context, fns, dispatch, dispatch_context, 1);  
  call->replace(new_expr);
}

static int insert_closure_arg(AList<Expr>* arguments, Expr *baseExpr, Symbol *field, int nargs, 
                              Expr **context);

static int
insert_closure_type_arg(AList<Expr>* arguments, Expr *baseExpr, Type *t, int nargs, Expr **context) {
  if (nargs < 0)
    return 0;
  AList<Expr> targuments;
  if (t->typeParents.in(dtClosure)) {
    ClassType *ct = dynamic_cast<ClassType*>(t);
    *context = new CallExpr(primitives[PRIMITIVE_GET_MEMBER], baseExpr->copy(),
                            new_StringSymbol(ct->fields.v[ct->fields.n-1]->name));
    CallExpr *ma = new CallExpr(primitives[PRIMITIVE_GET_MEMBER], baseExpr->copy(),
                                new_StringSymbol(ct->fields.v[0]->name));
    resolve_member(ct, 0, &ma->member_offset, &ma->member_type);
    if (!insert_closure_arg(&targuments, ma, ct->fields.v[0], nargs - (ct->fields.n - 2), context))
      return 0;
    arguments->insertAtHead(&targuments);
    for (int i = 1; i < ct->fields.n - 1; i++) {
      CallExpr *ma = new CallExpr(primitives[PRIMITIVE_GET_MEMBER], baseExpr->copy(),
                                  new_StringSymbol(ct->fields.v[i]->name));
      resolve_member(ct, i, &ma->member_offset, &ma->member_type);
      arguments->insertAtTail(ma);
    }
  } else if (t == dtSymbol)
    return !nargs;
  else {
    arguments->insertAtTail(baseExpr->copy());
    return nargs == 1;
  }
  return 1;
}

static int
insert_closure_arg(AList<Expr>* arguments, Expr *baseExpr, Symbol *field, int nargs, Expr **context) {
  Type *tt = field ? field->typeInfo() : baseExpr->typeInfo();
  if (SumType *st = dynamic_cast<SumType*>(tt)) {
    forv_Vec(Type*, x, st->components) {
      if (x->typeParents.in(dtClosure)) {
        ClassType *ct = dynamic_cast<ClassType*>(x);
        if (insert_closure_type_arg(arguments, baseExpr, ct, nargs, context))
          return 1;
      }
    }
    return 0;
  } else
    return insert_closure_type_arg(arguments, baseExpr, tt, nargs, context);
}

static void resolve_symbol(CallExpr* call) {
  if (!call->ainfo)
    return;
  if (call->isPrimitive(PRIMITIVE_GET_MEMBER) ||
      call->isPrimitive(PRIMITIVE_SET_MEMBER))
    resolve_member_access(call, &call->member_offset, &call->member_type);
  if (call->isPrimitive(PRIMITIVE_INIT)) {
    Type* type = call->get(1)->typeInfo();
    if (type->defaultValue) {
      call->replace(new SymExpr(type->defaultValue));
    } else {
      Vec<FnSymbol*> fns;
      call_info(call, fns);
      if (fns.n == 1)
        call->replace(new CallExpr(fns.v[0]));
      else
        INT_FATAL(call, "Cannot resolve default constructor");
    }
    return;
  }
  if (call->primitive)
    return;
  if (call->partialTag == PARTIAL_ALWAYS) {
    ClassType *closureType = dynamic_cast<ClassType*>(type_info(call));
    AList<Expr>* arguments = call->argList->copy(); 
    if (call->baseExpr->typeInfo()->typeParents.in(dtClosure)) {
      arguments->insertAtHead(call->baseExpr->copy());
    } else {
      if (SymExpr *symExpr = dynamic_cast<SymExpr*>(call->baseExpr)) {
        char *fnname = symExpr->var->name;
        arguments->insertAtHead(new SymExpr(new_SymbolSymbol(fnname)));
      } else {
        arguments->insertAtHead(call->baseExpr->copy());
      }
    }
    arguments->insertAtTail(new SymExpr(new_IntSymbol(context_info(call))));
    CallExpr *new_expr = new CallExpr(closureType->defaultConstructor, arguments);
    call->replace(new_expr);
    return;
  }
  if (call->baseExpr && call->baseExpr->typeInfo()->typeParents.in(dtClosure)) {
    Vec<FnSymbol*> fns;
    call_info(call, fns);
    if (fns.n <= 0) {
      INT_FATAL("Unable to resolve function");
    }
    int nargs = fns.v[0]->formals->length() - call->argList->length();
    AList<Expr>* arguments = new AList<Expr>;
    Expr *context = 0;
    if (!insert_closure_arg(arguments, call->baseExpr, 0, nargs, &context)) {
      INT_FATAL("Unable to resolve closure");
    }
    arguments->insertAtTail(call->argList->copy());
    if (fns.n == 1)
      call->replace(new CallExpr(fns.v[0], arguments));
    else {
      if (call->partialTag != PARTIAL_NEVER)
        return;
      dynamic_dispatch(call, arguments, context);
      return;
    }
    return;
  }
  if (!call->isAssign()) {
    Vec<FnSymbol*> fns;
    call_info(call, fns);
    if (fns.n != 1) {
      // HACK: Special case where write(:nilType) requires dynamic
      // dispatch; Take the other one.
      if (fns.n == 2 && !strcmp(fns.v[1]->name, "fwrite") &&
          fns.v[1]->formals->first()->sym->type == dtFile && 
          fns.v[1]->formals->get(2)->sym->type == dtNil) {
        fns.n = 1;
      } else if (fns.n == 2 && !strcmp(fns.v[0]->name, "fwrite") &&
                 fns.v[0]->formals->first()->sym->type == dtFile && 
                 fns.v[0]->formals->get(2)->sym->type == dtNil) {
        fns.v[0] = fns.v[1];
        fns.n = 1;
      }
    }
    AList<Expr>* arguments = call->argList->copy();
    // HACK: to handle special case for a.x(1) translation
    Expr *baseExpr = call->baseExpr;
    if (CallExpr* basecall = dynamic_cast<CallExpr*>(call->baseExpr)) {
      if (basecall->partialTag != PARTIAL_NEVER) {
        Vec<FnSymbol*> fns;
        call_info(basecall, fns);
        if (fns.n == 0) {
          baseExpr = basecall->baseExpr;
          arguments->insertAtHead(basecall->argList->copy());
        }
      }
    }
    if (fns.n != 1) {
      if (call->partialTag != PARTIAL_NEVER)
        return;
      if (fns.n > 1) {
        dynamic_dispatch(call, arguments);
        return;
      }
      INT_FATAL(call, "Unable to resolve function");
      return;
    }
    if (!strcmp("this", fns.v[0]->name)) {
      arguments->insertAtHead(baseExpr->copy());
    }
    CallExpr *new_expr = new CallExpr(fns.v[0], arguments);
    call->replace(new_expr);
    if (fns.v[0]->hasPragma("builtin"))
      new_expr->makeOp();
  } else {
    // Resolve AssignOp to members or setter functions
    Vec<FnSymbol*> fns;
    call_info(call, fns);
    if (fns.n == 1 && !fns.v[0]->hasPragma("builtin")) {
      call->baseExpr->replace(new SymExpr(fns.v[0]));
    } else if (fns.n > 1) {
      int notbuiltin = 0;
      forv_Vec(FnSymbol, f, fns) {
        if (!f->hasPragma("builtin")) {
          notbuiltin = 1;
        }
      }
      if (!notbuiltin) {
        call->makeOp();
      } else {
        INT_FATAL(call, "Unable to resolve setter function");
      }
    } else {
      call->makeOp(); // assume op if can't resolve
    }
  }
}
