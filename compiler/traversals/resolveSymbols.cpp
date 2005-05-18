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

static Expr* copy_argument_list(ParenOpExpr* expr) {
  Expr* args = NULL;
  MemberAccess* member_access = dynamic_cast<MemberAccess*>(expr->baseExpr);
  if (member_access) {
    args = member_access->base->copy();
  }
  args = appendLink(args, expr->argList->copyList());
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
  FnSymbol* candidate = NULL;
  forv_Vec(FnSymbol*, fn, candidates) {
    Symbol* formals = fn->formals;
    Expr* actuals = copy_argument_list(expr);
    bool match = true;
    while (actuals && formals) {
      if (actuals->typeInfo() != formals->type) {
        match = false;
        break;
      }
      actuals = nextLink(Expr, actuals);
      formals = nextLink(Symbol, formals);
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
  whichModules = MODULES_CODEGEN;
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
  Pragma* pr = fn->defPoint->parentStmt->pragmas;
  while (pr) {
    if (!strcmp(pr->str, "builtin")) {
      return 1;
    }
    pr = dynamic_cast<Pragma *>(pr->next);
  }
  return 0;
}


static void mangle_overloaded_operator_function_names(FnSymbol* fn) {
  static int uid = 0;

  Pragma* pr = fn->defPoint->parentStmt->pragmas;
  while (pr) {
    if (!strcmp(pr->str, "builtin")) {
      return;
    }
    pr = dynamic_cast<Pragma *>(pr->next);
  }

  if (!strcmp(fn->name, "=")) {
    fn->cname = glomstrings(2, "_assign", intstring(uid++));
  }
  else if (!strcmp(fn->name, "+")) {
    fn->cname = glomstrings(2, "_plus", intstring(uid++));
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
  }
}


void ResolveSymbols::postProcessExpr(Expr* expr) {

  if (analyzeAST) {
    Expr* paren_replacement = NULL;
    if (ParenOpExpr* paren = dynamic_cast<ParenOpExpr*>(expr)) {
      if (dynamic_cast<ArrayType*>(paren->baseExpr->typeInfo())) {
        paren_replacement = new ArrayRef(paren->baseExpr, paren->argList);
      } else if (dynamic_cast<TupleType*>(paren->baseExpr->typeInfo())) {
        paren_replacement = new TupleSelect(paren->baseExpr, paren->argList);
      } else if (Variable* baseVar = dynamic_cast<Variable*>(paren->baseExpr)) {
        if (!dynamic_cast<TypeSymbol*>(baseVar->var) && 
            dynamic_cast<TupleType*>(baseVar->typeInfo())) {
          paren_replacement = new TupleSelect(baseVar, paren->argList);
        } else if (dynamic_cast<FnSymbol*>(baseVar->var) && !dynamic_cast<FnCall*>(expr)) {
          paren_replacement = new FnCall(baseVar, paren->argList);
        }
      }
    }
    if (paren_replacement) {
      Vec<FnSymbol *> fns;
      call_info(expr, fns);
      if (fns.n > 1)
        INT_FATAL(expr, "unable to resolve call");
      if (fns.n == 1)
        paren_replacement->resolved = fns.v[0];
      expr->replace(paren_replacement);
      expr = paren_replacement;
    }
  }

  if (typeid(*expr) == typeid(ParenOpExpr) ||
      (analyzeAST && typeid(*expr) == typeid(FnCall) && expr->resolved)) 
  {
    ParenOpExpr* paren = dynamic_cast<ParenOpExpr*>(expr);
    Vec<FnSymbol*> fns;

    if (analyzeAST) {
      if (paren->resolved) {
        fns.add(paren->resolved);
      } else {
        call_info(paren, fns);
        if (fns.n == 0) { // for 0-ary (ParenOpExpr(MemberAccess))
          call_info(paren->baseExpr, fns);
        }
        if (fns.n != 1) {

          // HACK: Special case where write(:nilType) requires dynamic
          // dispatch; Take the other one.
          if (fns.n == 2 &&
              !strcmp(fns.e[1]->name, "write") &&
              fns.e[1]->formals->type->astType == TYPE_NIL) {
          } else if (fns.n == 2 &&
              !strcmp(fns.e[0]->name, "write") &&
              fns.e[0]->formals->type->astType == TYPE_NIL) {
            fns.v[0] = fns.v[1];
          } else {
            INT_FATAL(expr, "Unable to resolve function");
          }

        }
      }
    } else {
      call_info_noanalysis(paren, fns);
      if (fns.n != 1) {
        INT_FATAL(expr, "Unable to resolve function without analysis");
      }
    }

    Expr* arguments = copy_argument_list(paren);
    Expr* function = new Variable(fns.e[0]);
    Expr *new_expr = new FnCall(function, arguments);
    expr->replace(new_expr);
    expr = new_expr;
  }

  if (AssignOp* aop = dynamic_cast<AssignOp*>(expr)) {
    if (analyzeAST) {
      if (MemberAccess* member_access = dynamic_cast<MemberAccess*>(aop->left)) {
        resolve_member_access(aop, &member_access->member_offset, 
                              &member_access->member_type);
        if (!member_access->member_type) {
            Vec<FnSymbol *> op_fns, assign_fns;
            call_info(aop, op_fns, CALL_INFO_FIND_OPERATOR);
            call_info(aop, assign_fns, CALL_INFO_FIND_FUNCTION);
            if (op_fns.n > 1 || assign_fns.n != 1) 
              INT_FATAL(expr, "Unable to resolve member access");
            FnSymbol *f_op = op_fns.n ? op_fns.v[0] : 0;
            FnSymbol *f_assign = assign_fns.v[0];
            Expr *rhs = aop->right->copyList();
            if (f_op) {
              if (!is_builtin(f_op)) {
                Expr* op_arguments = member_access->copy();
                op_arguments = appendLink(op_arguments, rhs);
                Expr* op_function = new Variable(f_op);
                rhs = new FnCall(op_function, op_arguments);
              } else {
                BinOp *bop = new BinOp(gets_to_binop(aop->type), aop->left, aop->right);
                bop->resolved = f_op;
                rhs = bop;
              }
            }
            Expr* assign_arguments = member_access->base->copy();
            assign_arguments = appendLink(assign_arguments, rhs);
            Expr* assign_function = new Variable(f_assign);
            expr->replace(new FnCall(assign_function, assign_arguments));
            if (aop->left->astType == EXPR_MEMBERACCESS) {
              expr = aop->left;
            }
        }
      }
    }
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
    if (analyzeAST) {
      if (dynamic_cast<VarInitExpr*>(expr->parentExpr))
        return;
      if (AssignOp* aop = dynamic_cast<AssignOp*>(expr->parentExpr))
        if (aop->left == expr)
          return;
      resolve_member_access(member_access, &member_access->member_offset, 
                            &member_access->member_type);
      if (!member_access->member_type) {
        Vec<FnSymbol *> fns;
        call_info(member_access, fns);
        if (fns.n) {
          assert(fns.n == 1); // we don't handle dispatch yet
          Expr* arguments = member_access->base->copy();
          Expr* function = new Variable(fns.v[0]);
          Expr *new_expr = new FnCall(function, arguments);
          expr->replace(new FnCall(function, arguments));
          expr = new_expr;
        } else
          INT_FATAL(expr, "Unable to resolve member access");
      }
    } else {
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
            Expr *new_expr = new FnCall(function, arguments);
            expr->replace(new_expr);
            expr = new_expr;
          }
        } else {
          INT_FATAL(expr, "Cannot resolve MemberAccess");
        }
      }
    }
  }

  if (!useOldConstructor) {
    if (DefExpr* defExpr = dynamic_cast<DefExpr*>(expr)) {
      if (analyzeAST) {
        Vec<FnSymbol*> fns;
        call_info(defExpr, fns);
        if (fns.n == 1) {
          defExpr->sym->type->defaultConstructor = fns.v[0];
        }
      }
    }
  }
  
  if (DefExpr* def_expr = dynamic_cast<DefExpr*>(expr)) {
    if (FnSymbol* fn = dynamic_cast<FnSymbol*>(def_expr->sym)) {
      if (fn->parentScope->type != SCOPE_INTERNAL_PRELUDE) {
        mangle_overloaded_operator_function_names(fn);
      }
    }
  }
  if (analyzeAST && typeid(*expr) == typeid(BinOp)) {
    BinOp* op = dynamic_cast<BinOp*>(expr);
    Vec<FnSymbol*> fns;
    if (op->resolved)
      fns.add(op->resolved);
    else
      call_info(op, fns);
    if (fns.n != 1) {
      if (fns.n == 0) {
        INT_FATAL(expr, "Operator has no function");
      } else {
        INT_FATAL(expr, "Trouble resolving operator");
      }
    } else {
      Pragma* pr = fns.e[0]->defPoint->parentStmt->pragmas;
      while (pr) {
        if (!strcmp(pr->str, "builtin")) {
          return;
        }
        pr = dynamic_cast<Pragma *>(pr->next);
      }
    
      Expr* args = op->left->copy();
      args->append(op->right->copy());
      FnCall *new_expr = new FnCall(new Variable(fns.e[0]), args);
      expr->replace(new_expr);
      expr = new_expr;
    }
  }
}
