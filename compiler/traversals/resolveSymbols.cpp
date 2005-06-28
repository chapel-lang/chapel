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


static AList<Expr>* copy_argument_list(ParenOpExpr* expr) {
  AList<Expr>* args = new AList<Expr>();
  MemberAccess* member_access = dynamic_cast<MemberAccess*>(expr->baseExpr);
  if (member_access) {
    args->insertAtTail(member_access->base->copy());
  }
  args->add(expr->argList->copy());
  return args;
}


static void call_info_noanalysis(ParenOpExpr* expr, Vec<FnSymbol*>& fns) {
  char* name;
  if (Variable* variable = dynamic_cast<Variable*>(expr->baseExpr)) {
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


binOpType
gets_to_binop(getsOpType i) {
  switch (i) {
    case GETS_PLUS: return BINOP_PLUS;
    case GETS_MINUS: return BINOP_MINUS;
    case GETS_MULT: return BINOP_MULT;
    case GETS_DIV: return BINOP_DIV;
    case GETS_BITAND: return BINOP_BITAND;
    case GETS_BITOR: return BINOP_BITOR;
    case GETS_BITXOR: return BINOP_BITXOR;
    default: 
      assert(!"bogus case");
  }
  return BINOP_PLUS;
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
      if (fn->parentScope->type != SCOPE_INTERNAL_PRELUDE) {
        mangle_overloaded_operator_function_names(fn);
      }
    }
  }
}

static void
resolve_no_analysis(Expr *expr) {

  if (expr->astType == EXPR_PARENOP) {
    ParenOpExpr* paren = dynamic_cast<ParenOpExpr*>(expr);
    Vec<FnSymbol*> fns;
    call_info_noanalysis(paren, fns);
    if (fns.n != 1) {
      INT_FATAL(expr, "Unable to resolve function without analysis");
    }
    AList<Expr>* arguments = copy_argument_list(paren);
    Expr* function = new Variable(fns.e[0]);
    if (!strcmp("this", fns.e[0]->name)) {
      arguments->insertAtHead(paren->baseExpr->copy());
    }
    Expr* new_expr = new FnCall(function, arguments);
    expr->replace(new_expr);
    expr = new_expr;
  }

  if (MemberAccess* member_access = dynamic_cast<MemberAccess*>(expr)) {
    /***
     *** Resolve methods with arguments at ParenOpExpr
     ***/
    if (ParenOpExpr* paren_op = dynamic_cast<ParenOpExpr*>(expr->parentExpr)) {
      if (paren_op->baseExpr == expr) {
        return;
      }
    }
    if (dynamic_cast<UnresolvedSymbol*>(member_access->member)) {
      StructuralType* struct_scope =
        dynamic_cast<StructuralType*>(member_access->base->typeInfo());
      if (struct_scope) {
        member_access->member = 
          Symboltable::lookupInScope(member_access->member->name,
                                     struct_scope->structScope);

        if (dynamic_cast<FnSymbol*>(member_access->member)) {
          Expr* arguments = member_access->base->copy();
          Expr* function = new Variable(member_access->member);
          Expr *new_expr = new FnCall(function, new AList<Expr>(arguments));
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
resolve_binary_operator(BinOp *op, FnSymbol *resolved = 0) {
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
    AList<Expr>* args = new AList<Expr>(op->left->copy());
    args->insertAtTail(op->right->copy());
    FnCall *new_expr = new FnCall(new Variable(fns.e[0]), args);
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

  // Specialize Array references and Tuple selects
  Expr* paren_replacement = NULL;
  if (ParenOpExpr* paren = dynamic_cast<ParenOpExpr*>(expr)) {
    if (dynamic_cast<ArrayType*>(paren->baseExpr->typeInfo())) {
      paren_replacement = new ArrayRef(paren->baseExpr, paren->argList);
    } else if (dynamic_cast<TupleType*>(paren->baseExpr->typeInfo())) {
      paren_replacement = new TupleSelect(paren->baseExpr, paren->argList);
    }
  }
  if (paren_replacement) {
    expr->replace(paren_replacement);
    expr = paren_replacement;
  }

  // Resolve FnCalls
  if (typeid(*expr) == typeid(ParenOpExpr)) {
    ParenOpExpr* paren = dynamic_cast<ParenOpExpr*>(expr);
    if (Variable* variable = dynamic_cast<Variable*>(paren->baseExpr)) {
      if (!strcmp(variable->var->name, "__primitive")) {
        return;
      }
    }
    AssignOp *assign = dynamic_cast<AssignOp*>(paren->parentExpr);
    if (!assign || assign->left != expr) {
      Vec<FnSymbol*> fns;
      call_info(paren, fns);
      if (fns.n == 0) { // for 0-ary (ParenOpExpr(MemberAccess))
        call_info(paren->baseExpr, fns);
      }
      if (fns.n == 0) { // for set function
        if (paren->parentExpr && paren->parentExpr->astType == EXPR_ASSIGNOP) {
          Vec<FnSymbol*> fns2;
          call_info(paren->parentExpr, fns2);
          if (fns2.n == 1 && !strcmp(fns2.v[0]->name, "set")) {
            // handle this case in AssignOp below
            return;
          }
        }
      }
      if (fns.n != 1) {
        // HACK: Special case where write(:nilType) requires dynamic
        // dispatch; Take the other one.
        if (fns.n == 2 && !strcmp(fns.e[1]->name, "write") &&
            fns.e[1]->formals->only()->sym->type->astType == TYPE_NIL) {
        } else if (fns.n == 2 && !strcmp(fns.e[0]->name, "write") &&
                   fns.e[0]->formals->only()->sym->type->astType == TYPE_NIL) {
          fns.v[0] = fns.v[1];
        } else {
          INT_FATAL(expr, "Unable to resolve function");
          return;
        }
      }

      AList<Expr>* arguments = copy_argument_list(paren);
      Expr* function = new Variable(fns.e[0]);
      if (!strcmp("this", fns.e[0]->name)) {
        arguments->insertAtHead(paren->baseExpr->copy());
      }
      Expr *new_expr = new FnCall(function, arguments);
      expr->replace(new_expr);
      expr = new_expr;
    }
  }

  // Resolve AssignOp to members or setter functions
  if (AssignOp* aop = dynamic_cast<AssignOp*>(expr)) {
    if (typeid(*aop->left) == typeid(ParenOpExpr)) {
      ParenOpExpr* paren = dynamic_cast<ParenOpExpr*>(aop->left);
      Vec<FnSymbol*> fns;
      call_info(aop, fns);
      if (fns.n == 1) {
        Expr* function = new Variable(fns.e[0]);
        AList<Expr>* arguments = new AList<Expr>();
        if (!strcmp("=this", fns.e[0]->name))
          arguments->insertAtTail(paren->baseExpr->copy());
        arguments->add(copy_argument_list(paren));
        arguments->insertAtTail(aop->right->copy());
        Expr *new_expr = new FnCall(function, arguments);
        aop->replace(new_expr);
        expr = new_expr;
      } else {
        INT_FATAL(expr, "Unable to resolve setter function");
      }
    } else if (MemberAccess* member_access = dynamic_cast<MemberAccess*>(aop->left)) {
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
        Expr *rhs = aop->right->copy();
        if (f_op) {
          if (!is_builtin(f_op)) {
            AList<Expr>* op_arguments = new AList<Expr>(member_access->copy());
            op_arguments->insertAtTail(rhs);
            Expr* op_function = new Variable(f_op);
            rhs = new FnCall(op_function, op_arguments);
          } else {
            rhs = resolve_binary_operator(
              new BinOp(gets_to_binop(aop->type), aop->left, aop->right),
              f_op);
          }
        }
        AList<Expr>* assign_arguments = new AList<Expr>(member_access->base->copy());
        assign_arguments->insertAtTail(rhs);
        Expr* assign_function = new Variable(f_assign);
        Expr *new_expr = new FnCall(assign_function, assign_arguments);
        expr->replace(new_expr);
        if (aop->left->astType == EXPR_MEMBERACCESS) {
          expr = aop->left;
        }
      } else {
        if (StructuralType* struct_scope =
            dynamic_cast<StructuralType*>(member_access->base->typeInfo())) {
          member_access->member = 
            Symboltable::lookupInScope(member_access->member->name,
                                       struct_scope->structScope);
        }
      }
    } else if (f_equal_method) {
      Vec<FnSymbol *> op_fns, assign_fns;
      call_info(aop, op_fns, CALL_INFO_FIND_OPERATOR);
      call_info(aop, assign_fns, CALL_INFO_FIND_FUNCTION);
      if (op_fns.n || assign_fns.n) {
        FnSymbol *f_op = op_fns.n ? op_fns.v[0] : 0;
        FnSymbol *f_assign = assign_fns.n ? assign_fns.v[0] : 0;
        Expr *rhs = aop->right->copy();
        if (f_op) {
          if (!is_builtin(f_op)) {
            AList<Expr>* op_arguments = new AList<Expr>(member_access->copy());
            op_arguments->insertAtTail(rhs);
            Expr* op_function = new Variable(f_op);
            Expr *new_rhs = new FnCall(op_function, op_arguments);
            rhs->replace(new_rhs);
          } else {
            rhs = resolve_binary_operator(
              new BinOp(gets_to_binop(aop->type), aop->left, aop->right),
              f_op);
          }
        }
        if (f_assign) {
          AList<Expr>* assign_arguments = new AList<Expr>(aop->left->copy());
          assign_arguments->insertAtTail(rhs);
          Expr* assign_function = new Variable(f_assign);
          Expr *new_expr = new FnCall(assign_function, assign_arguments);
          expr->replace(new_expr);
          expr = new_expr;
        }
      }
    }
  }

  // Resolve MemberAccesses
  if (MemberAccess* member_access = dynamic_cast<MemberAccess*>(expr)) {
    /***
     *** Resolve methods with arguments at ParenOpExpr
     ***/
    if (ParenOpExpr* paren_op = dynamic_cast<ParenOpExpr*>(expr->parentExpr)) {
      if (paren_op->baseExpr == expr) {
        return;
      }
    }
    if (dynamic_cast<VarInitExpr*>(expr->parentExpr))
      return;
    if (AssignOp* aop = dynamic_cast<AssignOp*>(expr->parentExpr))
      if (aop->left == expr)
        return;

    resolve_member_access(member_access, &member_access->member_offset,
                          &member_access->member_type);
    if (member_access->member_offset < 0) {
      Vec<FnSymbol *> fns;
      call_info(member_access, fns);
      if (fns.n == 1) {
        Expr* arguments = member_access->base->copy();
        Expr* function = new Variable(fns.v[0]);
        Expr *new_expr = new FnCall(function, new AList<Expr>(arguments));
        expr->replace(new FnCall(function, new AList<Expr>(arguments->copy())));
        expr = new_expr;
      } else
        INT_FATAL(expr, "Unable to resolve member access");
    } else {
      if (StructuralType* struct_scope =
          dynamic_cast<StructuralType*>(member_access->base->typeInfo())) {
        member_access->member = 
          Symboltable::lookupInScope(member_access->member->name,
                                     struct_scope->structScope);
      }
    }
  }

  // Resolve default constructors
  if (DefExpr* defExpr = dynamic_cast<DefExpr*>(expr)) {
    Vec<FnSymbol*> fns;
    call_info(defExpr, fns);
    if (fns.n == 1) {
      defExpr->sym->type->defaultConstructor = fns.v[0];
    } if (fns.n > 1) {
      INT_FATAL(expr, "Unable to resolve default constructor");
    } else if (defExpr->exprType) {
      if (FnCall *fc = dynamic_cast<FnCall*>(defExpr->exprType))
        if (Variable *v = dynamic_cast<Variable*>(fc->baseExpr))
          if (FnSymbol *fn = dynamic_cast<FnSymbol*>(v->var))
            defExpr->sym->type->defaultConstructor = fn;
    }
  }

  mangle_overloaded_operator_function_names(expr);

  // Resolve overloaded binary operators 
  if (dynamic_cast<BinOp*>(expr)) {
    if (typeid(BinOp) == typeid(*expr)) { // SJD: WANT TO REMOVE
      expr = resolve_binary_operator(dynamic_cast<BinOp*>(expr));
    }
  }
}
