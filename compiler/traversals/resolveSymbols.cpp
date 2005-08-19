#include <typeinfo>
#include "resolveSymbols.h"
#include "analysis.h"
#include "chplalloc.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "symtab.h"
#include "type.h"
#include "stringutil.h"


static AList<Expr>* copy_argument_list(CallExpr* expr) {
  AList<Expr>* args = new AList<Expr>();
  MemberAccess* member_access = dynamic_cast<MemberAccess*>(expr->baseExpr);
  if (member_access) {
    args->insertAtTail(member_access->base->copy());
  }
  args->insertAtTail(expr->argList->copy());
  return args;
}


static void call_info_noanalysis(CallExpr* expr, Vec<FnSymbol*>& fns) {
  char* name;
  if (SymExpr* variable = dynamic_cast<SymExpr*>(expr->baseExpr)) {
    name = variable->var->name;
  } else if (MemberAccess* member_access = dynamic_cast<MemberAccess*>(expr->baseExpr)) {
    name = member_access->member->name;
  } else {
    INT_FATAL(expr, "Unable to resolve function without analysis");
  }
  SymScope* scope = Symboltable::getCurrentScope();
  Vec<FnSymbol*> candidates;
  for (int i = 0; i < scope->visibleFunctions.n; i++) {
    Vec<FnSymbol*>* fs = scope->visibleFunctions.v[i].value;
    if (fs) {
      forv_Vec(FnSymbol, fn, *fs) {
        if (fn && !strcmp(fn->name, name)) {
          candidates.add(fn);
        }
      }
    }
  }
  if (candidates.n == 1) {
    fns.add(candidates.v[0]);
    return;
  }
  if (candidates.n == 2 && candidates.v[0] == candidates.v[1]) {
    fns.add(candidates.v[0]);
    return;
  }
  FnSymbol* candidate = NULL;
  forv_Vec(FnSymbol*, fn, candidates) {
    DefExpr* formals = fn->formals->first();
    AList<Expr>* actualList = copy_argument_list(expr);
    Expr* actuals = actualList->first();
    bool match = true;
    while (actuals && formals) {
      if (actuals->typeInfo() != formals->sym->type) {
        match = false;
        break;
      }
      actuals = actualList->next();
      formals = fn->formals->next();
    }
    if (actuals || formals) {
      match = false;
    }
    if (match) {
      if (candidate != NULL && candidate != fn) {
        if (strncmp("_construct", fn->name, 10)) {
          INT_FATAL(expr, "Unable to resolve function");
        }
      }
      candidate = fn;
    }
  }
  if (!candidate) {
    INT_FATAL(expr, "Unable to resolve function");
  }
  fns.add(candidate);
}


ResolveSymbols::ResolveSymbols() {
  //  whichModules = MODULES_CODEGEN;
}


OpTag gets_to_op(OpTag i) {
  switch (i) {
    case OP_GETSPLUS: return OP_PLUS;
    case OP_GETSMINUS: return OP_MINUS;
    case OP_GETSMULT: return OP_MULT;
    case OP_GETSDIV: return OP_DIV;
    case OP_GETSBITAND: return OP_BITAND;
    case OP_GETSBITOR: return OP_BITOR;
    case OP_GETSBITXOR: return OP_BITXOR;
    default: 
      INT_FATAL("Unable to convert OpTag");
  }
  return OP_PLUS;
}


static int
is_builtin(FnSymbol *fn) {
  if (fn->defPoint->parentStmt->hasPragma("builtin")) {
    return 1;
  }
  return 0;
}


static void mangle_overloaded_operator_function_names(FnSymbol* fn) {
  static int uid = 0;

  if (is_builtin(fn)) {
    return;
  }

  if (!strcmp(fn->name, "=")) {
    fn->cname = glomstrings(2, "_assign", intstring(uid++));
  }
  else if (!strcmp(fn->name, "+")) {
    fn->cname = glomstrings(2, "_plus", intstring(uid++));
  }
  else if (!strcmp(fn->name, "#")) {
    fn->cname = glomstrings(2, "_pound", intstring(uid++));
  }
  else if (!strcmp(fn->name, "-")) {
    fn->cname = glomstrings(2, "_minus", intstring(uid++));
  }
  else if (!strcmp(fn->name, "*")) {
    fn->cname = glomstrings(2, "_times", intstring(uid++));
  }
  else if (!strcmp(fn->name, "/")) {
    fn->cname = glomstrings(2, "_div", intstring(uid++));
  }
  else if (!strcmp(fn->name, "mod")) {
    fn->cname = glomstrings(2, "_mod", intstring(uid++));
  }
  else if (!strcmp(fn->name, "==")) {
    fn->cname = glomstrings(2, "_eq", intstring(uid++));
  }
  else if (!strcmp(fn->name, "!=")) {
    fn->cname = glomstrings(2, "_ne", intstring(uid++));
  }
  else if (!strcmp(fn->name, "<=")) {
    fn->cname = glomstrings(2, "_le", intstring(uid++));
  }
  else if (!strcmp(fn->name, ">=")) {
    fn->cname = glomstrings(2, "_ge", intstring(uid++));
  }
  else if (!strcmp(fn->name, "<")) {
    fn->cname = glomstrings(2, "_lt", intstring(uid++));
  }
  else if (!strcmp(fn->name, ">")) {
    fn->cname = glomstrings(2, "_gt", intstring(uid++));
  }
  else if (!strcmp(fn->name, "&")) {
    fn->cname = glomstrings(2, "_bitand", intstring(uid++));
  }
  else if (!strcmp(fn->name, "|")) {
    fn->cname = glomstrings(2, "_bitor", intstring(uid++));
  }
  else if (!strcmp(fn->name, "^")) {
    fn->cname = glomstrings(2, "_xor", intstring(uid++));
  }
  else if (!strcmp(fn->name, "and")) {
    fn->cname = glomstrings(2, "_and", intstring(uid++));
  }
  else if (!strcmp(fn->name, "or")) {
    fn->cname = glomstrings(2, "_or", intstring(uid++));
  }
  else if (!strcmp(fn->name, "**")) {
    fn->cname = glomstrings(2, "_exponent", intstring(uid++));
  }
  else if (!strcmp(fn->name, "by")) {
    fn->cname = glomstrings(2, "_by", intstring(uid++));
  } else if (*fn->name == '=') {
    fn->cname = glomstrings(4, "_assign", intstring(uid++), "_", fn->name+1);
  }
}


static void
mangle_overloaded_operator_function_names(Expr *expr) {
  if (DefExpr* def_expr = dynamic_cast<DefExpr*>(expr)) {
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(def_expr->sym)) {
      mangle_overloaded_operator_function_names(fn);
    }
  }
}

static void
resolve_no_analysis(Expr *expr) {

  if (expr->astType == EXPR_CALL) {
    CallExpr* paren = dynamic_cast<CallExpr*>(expr);
    Vec<FnSymbol*> fns;
    call_info_noanalysis(paren, fns);
    if (fns.n != 1) {
      INT_FATAL(expr, "Unable to resolve function without analysis");
    }
    AList<Expr>* arguments = copy_argument_list(paren);
    if (!strcmp("this", fns.e[0]->name)) {
      arguments->insertAtHead(paren->baseExpr->copy());
    }
    CallExpr* new_expr = new CallExpr(fns.e[0], arguments);
    new_expr->opTag = paren->opTag;
    expr->replace(new_expr);
    expr = new_expr;
  }

  if (MemberAccess* member_access = dynamic_cast<MemberAccess*>(expr)) {
    /***
     *** Resolve methods with arguments at CallExpr
     ***/
    if (CallExpr* paren_op = dynamic_cast<CallExpr*>(expr->parentExpr)) {
      if (paren_op->baseExpr == expr) {
        return;
      }
    }
    if (dynamic_cast<UnresolvedSymbol*>(member_access->member)) {
      ClassType* struct_scope =
        dynamic_cast<ClassType*>(member_access->base->typeInfo());
      if (struct_scope) {
        member_access->member = 
          Symboltable::lookupInScope(member_access->member->name,
                                     struct_scope->structScope);

        if (dynamic_cast<FnSymbol*>(member_access->member)) {
          CallExpr *new_expr = new CallExpr(member_access->member,
                                            member_access->base->copy());
          expr->replace(new_expr);
          expr = new_expr;
        }
      } else {
        INT_FATAL(expr, "Cannot resolve MemberAccess");
      }
    }
  }
  mangle_overloaded_operator_function_names(expr);
}


static Expr *
resolve_binary_operator(CallExpr *op, FnSymbol *resolved = 0) {
  Expr *expr = op;
  Vec<FnSymbol*> fns;
  if (resolved)
    fns.add(resolved);
  else
    call_info(expr, fns);
  if (fns.n != 1) {
    if (fns.n == 0) {
      return expr;
    } else {
      INT_FATAL(expr, "Trouble resolving operator");
    }
  } else {
    if (fns.e[0]->defPoint->parentStmt->hasPragma("builtin")) {
      return expr;
    }
    CallExpr *new_expr = new CallExpr(fns.e[0], op->copy());
    expr->replace(new_expr);
    expr = new_expr;
  }
  return expr;
}


void ResolveSymbols::postProcessExpr(Expr* expr) {
  if (!analyzeAST) {
    resolve_no_analysis(expr);
    return;
  }

  // Resolve CallExprs
  if (CallExpr* paren = dynamic_cast<CallExpr*>(expr)) {
    if (paren->opTag < OP_GETSNORM) {
      if (SymExpr* variable = dynamic_cast<SymExpr*>(paren->baseExpr)) {
        if (!strcmp(variable->var->name, "__primitive")) {
          return;
        }
      }
      CallExpr *assign = dynamic_cast<CallExpr*>(paren->parentExpr);
      if (!assign || assign->opTag < OP_GETSNORM ||  assign->get(1) != expr) {
        Vec<FnSymbol*> fns;
        call_info(paren, fns);
        if (fns.n == 0) { // for 0-ary (CallExpr(MemberAccess))
          call_info(paren->baseExpr, fns);
        }
        if (fns.n == 0) { // for set function
          if (assign && assign->opTag >= OP_GETSNORM) {
            Vec<FnSymbol*> fns2;
            call_info(paren->parentExpr, fns2);
            if (fns2.n == 1 && !strcmp(fns2.v[0]->name, "set")) {
              // handle this case below
              return;
            }
          }
        }
        if (fns.n != 1) {
          // HACK: Special case where write(:nilType) requires dynamic
          // dispatch; Take the other one.
          if (fns.n == 2 && !strcmp(fns.e[1]->name, "write") &&
              fns.e[1]->formals->only()->sym->type == dtNil) {
          } else if (fns.n == 2 && !strcmp(fns.e[0]->name, "write") &&
                     fns.e[0]->formals->only()->sym->type == dtNil) {
            fns.v[0] = fns.v[1];
          } else {
            if (OP_ISUNARYOP(paren->opTag)) {
              return;
            }
            if (OP_ISBINARYOP(paren->opTag)) {
              return;
            }
            INT_FATAL(expr, "Unable to resolve function");
            return;
          }
        }

        AList<Expr>* arguments = copy_argument_list(paren);
        if (!strcmp("this", fns.e[0]->name)) {
          arguments->insertAtHead(paren->baseExpr->copy());
        }
        CallExpr *new_expr = new CallExpr(fns.e[0], arguments);
        if (fns.e[0]->defPoint->parentStmt->hasPragma("builtin")) {
          new_expr->opTag = paren->opTag;
        }
        expr->replace(new_expr);
        expr = new_expr;
      }
    }
  }

  // Resolve AssignOp to members or setter functions
  if (CallExpr* aop = dynamic_cast<CallExpr*>(expr)) {
    if (aop->opTag >= OP_GETSNORM) {
      if (typeid(*aop->get(1)) == typeid(CallExpr)) {
        CallExpr* paren = dynamic_cast<CallExpr*>(aop->get(1));
        Vec<FnSymbol*> fns;
        call_info(aop, fns);
        if (fns.n == 1) {
          AList<Expr>* arguments = new AList<Expr>();
          if (!strcmp("=this", fns.e[0]->name))
            arguments->insertAtTail(paren->baseExpr->copy());
          arguments->insertAtTail(copy_argument_list(paren));
          arguments->insertAtTail(aop->get(2)->copy());
          CallExpr *new_expr = new CallExpr(fns.e[0], arguments);
          new_expr->opTag = paren->opTag;
          aop->replace(new_expr);
          expr = new_expr;
        } else {
          INT_FATAL(expr, "Unable to resolve setter function");
        }
      } else if (MemberAccess* member_access = dynamic_cast<MemberAccess*>(aop->get(1))) {
        resolve_member_access(aop, &member_access->member_offset, 
                              &member_access->member_type);
        if (member_access->member_offset < 0) {
          Vec<FnSymbol *> op_fns, assign_fns;
          call_info(aop, op_fns, CALL_INFO_FIND_OPERATOR);
          call_info(aop, assign_fns, CALL_INFO_FIND_FUNCTION);
          if (op_fns.n > 1 || assign_fns.n != 1) 
            INT_FATAL(expr, "Unable to resolve member access");
          FnSymbol *f_op = op_fns.n ? op_fns.v[0] : 0;
          FnSymbol *f_assign = assign_fns.v[0];
          Expr *rhs = aop->get(2)->copy();
          if (f_op) {
            if (!is_builtin(f_op)) {
              rhs = new CallExpr(f_op, member_access->copy(), rhs);
            } else {
              rhs = resolve_binary_operator(new CallExpr(gets_to_op(aop->opTag), aop->get(1)->copy(), aop->get(2)->copy()), f_op);
            }
          }
          AList<Expr>* assign_arguments = new AList<Expr>(member_access->base->copy());
          assign_arguments->insertAtTail(rhs);
          Expr* assign_function = new SymExpr(f_assign);
          CallExpr *new_expr = new CallExpr(assign_function, assign_arguments);
          expr->replace(new_expr);
          if (aop->get(1)->astType == EXPR_MEMBERACCESS) {
            expr = aop->get(1);
          }
        } else {
          if (ClassType* struct_scope =
              dynamic_cast<ClassType*>(member_access->base->typeInfo())) {
            member_access->member = 
              Symboltable::lookupInScope(member_access->member->name,
                                         struct_scope->structScope);
          }
        }
      }
    }
  }

  // Resolve MemberAccesses
  if (MemberAccess* member_access = dynamic_cast<MemberAccess*>(expr)) {
    /***
     *** Resolve methods with arguments at CallExpr
     ***/
    if (CallExpr* paren_op = dynamic_cast<CallExpr*>(expr->parentExpr)) {
      if (paren_op->baseExpr == expr) {
        return;
      }
    }
    if (CallExpr* aop = dynamic_cast<CallExpr*>(expr->parentExpr))
      if (aop->opTag >= OP_GETSNORM && aop->get(1) == expr)
        return;

    resolve_member_access(member_access, &member_access->member_offset,
                          &member_access->member_type);
    if (member_access->member_offset < 0) {
      Vec<FnSymbol *> fns;
      call_info(member_access, fns);
      if (fns.n == 1) {
        Expr *new_expr = new CallExpr(fns.v[0], member_access->base->copy());
        expr->replace(new CallExpr(fns.v[0], member_access->base->copy()));
        expr = new_expr;
      } else
        INT_FATAL(expr, "Unable to resolve member access");
    } else {
      if (ClassType* struct_scope =
          dynamic_cast<ClassType*>(member_access->base->typeInfo())) {
        member_access->member = 
          Symboltable::lookupInScope(member_access->member->name,
                                     struct_scope->structScope);
      }
    }
  }

  // Resolve default constructors
  if (DefExpr* defExpr = dynamic_cast<DefExpr*>(expr)) {
    Vec<FnSymbol*> fns;
    call_info(defExpr, defExpr->initAssign, CALL_INFO_FIND_ASSIGN);
    call_info(defExpr, fns, CALL_INFO_FIND_NON_ASSIGN);
    if (fns.n == 1) {
      defExpr->sym->type->defaultConstructor = fns.v[0];
    } if (fns.n > 1) {
      INT_FATAL(expr, "Unable to resolve default constructor");
    } else if (defExpr->exprType) {
      if (CallExpr *fc = dynamic_cast<CallExpr*>(defExpr->exprType))
        if (SymExpr* v = dynamic_cast<SymExpr*>(fc->baseExpr))
          if (FnSymbol *fn = dynamic_cast<FnSymbol*>(v->var))
            defExpr->sym->type->defaultConstructor = fn;
    }
  }

  mangle_overloaded_operator_function_names(expr);

  // Resolve overloaded binary operators 
//   if (dynamic_cast<BinOp*>(expr)) {
//     if (typeid(BinOp) == typeid(*expr)) { // SJD: WANT TO REMOVE
//       expr = resolve_binary_operator(dynamic_cast<BinOp*>(expr));
//     }
//   }
}
