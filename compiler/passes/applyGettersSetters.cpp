#include "applyGettersSetters.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"


#define REPLACE(_x) do { \
      BaseAST* replacement = _x; \
      ast->replace(replacement); \
      ast = replacement; \
      goto Ldone; \
} while (0)

static OpTag
gets_to_non(OpTag t) {
  switch (t) {
    default: break;
    case OP_GETSPLUS: return OP_PLUS;
    case OP_GETSMINUS: return OP_MINUS;
    case OP_GETSMULT: return OP_MULT;
    case OP_GETSDIV: return OP_DIV;
    case OP_GETSBITAND: return OP_BITAND;
    case OP_GETSBITOR: return OP_BITOR;
    case OP_GETSBITXOR: return OP_BITXOR;
    case OP_GETSSEQCAT: return OP_SEQCAT;
  }
  assert(!"case");
  return OP_NONE;
}
  
static void 
process(BaseAST* ast) {
  // Most generally:
  //   x.f(1) += y ---> f(x, _mt, 1, _st, +(f(x,_mt, 1), y)
  // or
  //   CallExpr(+=, CallExpr(MemberAccess(x, "f"), 1), y) --->
  //     CallExpr("f", x, _mt, 1, _st, CallExpr("+", CallExpr("f", x, _mt, 1), y))
  // though, it could be a just = (vs +=) 
  //         or a simple MemberAccess without CallExpr
  //         or a CallExpr without a MemberAccess
  CallExpr *call = dynamic_cast<CallExpr*>(ast), *assign = 0;
  if (call && OP_ISASSIGNOP(call->opTag)) {
    assign = call;
    call = dynamic_cast<CallExpr*>(assign->get(1));
  }
  BaseAST *base = call ? call->baseExpr : (assign ? assign->get(1) : ast);
  if (MemberAccess* memberAccess = dynamic_cast<MemberAccess*>(base)) {
    Expr *rhs = assign ? assign->argList->get(2)->copy() : 0;
    // build the RHS operator expression
    if (rhs && assign->opTag != OP_GETSNORM) {
      CallExpr *lhs = new CallExpr(memberAccess->member->name, 
                                   new SymExpr(Symboltable::lookupInternal("_methodToken")),
                                   memberAccess->base->copy());
      if (call && !call->argList->isEmpty()) {
        lhs->partialTag = PARTIAL_OK;
        lhs = new CallExpr(lhs, call->argList->copy());
      }
      rhs = new CallExpr(gets_to_non(assign->opTag), lhs, rhs);
    }
    // build the main accessor/setter
    if ((call && !call->argList->isEmpty())) {
      CallExpr *lhs = new CallExpr(memberAccess->member->name, 
                                   new SymExpr(Symboltable::lookupInternal("_methodToken")),
                                   memberAccess->base->copy());
      lhs->partialTag = PARTIAL_OK;
      AList<Expr>* arguments = call ? call->argList->copy() : new AList<Expr>;
      if (rhs) {
        arguments->insertAtTail(new SymExpr(Symboltable::lookupInternal("_setterToken")));
        arguments->insertAtTail(rhs);
      }
      lhs = new CallExpr(lhs, arguments);
      REPLACE(lhs);
    } else {
      AList<Expr>* arguments = call ? call->argList->copy() : new AList<Expr>;
      arguments->insertAtHead(memberAccess->base->copy());
      arguments->insertAtHead(new SymExpr(Symboltable::lookupInternal("_methodToken")));
      if (rhs) {
        arguments->insertAtTail(new SymExpr(Symboltable::lookupInternal("_setterToken")));
        arguments->insertAtTail(rhs);
      }
      REPLACE(new CallExpr(memberAccess->member->name, arguments));
    }
  } 
  if (assign) {
    Expr *rhs = assign->argList->get(2)->copy();
    if (assign->opTag != OP_GETSNORM)
      rhs = new CallExpr(gets_to_non(assign->opTag), assign->argList->get(1)->copy(), rhs);
    if (call) {
      AList<Expr>* arguments = call->argList->copy();
      arguments->insertAtTail(new SymExpr(Symboltable::lookupInternal("_setterToken")));
      arguments->insertAtTail(rhs);
      REPLACE(new CallExpr(call->baseExpr->copy(), arguments));
    } else
      REPLACE(new CallExpr(OP_GETSNORM, assign->argList->get(1)->copy(), rhs));
  } 
 Ldone:
  // top down, on the modified AST
  Vec<BaseAST *> asts;
  get_ast_children(ast, asts);
  forv_BaseAST(a, asts)
    process(a);
}

void ApplyGettersSetters::run(Vec<ModuleSymbol*>* modules) {
  Vec<Symbol *> symbols;
  forv_Vec(ModuleSymbol, mod, *modules)
    getSymbols(mod->modScope, symbols);
  symbols.set_to_vec();
  qsort(symbols.v, symbols.n, sizeof(symbols.v[0]), compar_baseast);
  forv_Symbol(s, symbols)
    if (FnSymbol *f = dynamic_cast<FnSymbol*>(s)) {
      if (f->_setter || f->_getter || f->fnClass == FN_CONSTRUCTOR)
        continue;
      process(f->body);
    }
}

