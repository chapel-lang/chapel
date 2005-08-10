#include "applyGettersSetters.h"
#include "moduleList.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symtab.h"


#define REPLACE(_x) { \
      BaseAST* replacement = _x; \
      ast->replace(replacement); \
      ast = replacement; \
      goto Ldone; \
}

static OpTag
gets_to_non(OpTag t) {
  switch (t) {
    default: assert(!"case"); break;
    case OP_GETSPLUS: return OP_PLUS;
    case OP_GETSMINUS: return OP_MINUS;
    case OP_GETSMULT: return OP_MULT;
    case OP_GETSDIV: return OP_DIV;
    case OP_GETSBITAND: return OP_BITAND;
    case OP_GETSBITOR: return OP_BITOR;
    case OP_GETSBITXOR: return OP_BITXOR;
    case OP_GETSSEQCAT: return OP_SEQCAT;
  }
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
    if (rhs && assign->opTag != OP_GETSNORM) {
      AList<Expr>* arguments = call ? call->argList->copy() : new AList<Expr>;
      arguments->insertAtHead(memberAccess->base->copy());
      arguments->insertAtHead(new Variable(Symboltable::lookupInternal("_methodToken")));
      Expr *lhs = new CallExpr(memberAccess->member->name, arguments);
      rhs = new CallExpr(gets_to_non(assign->opTag), lhs, rhs);
    }
    AList<Expr>* arguments = call ? call->argList->copy() : new AList<Expr>;
    arguments->insertAtHead(memberAccess->base->copy());
    arguments->insertAtHead(new Variable(Symboltable::lookupInternal("_methodToken")));
    if (rhs) {
      arguments->insertAtTail(new Variable(Symboltable::lookupInternal("_setterToken")));
      arguments->insertAtTail(rhs);
    }
    REPLACE(new CallExpr(memberAccess->member->name, arguments));
  } 
  if (assign && call) {
    AList<Expr>* arguments = call->argList->copy();
    arguments->insertAtTail(new Variable(Symboltable::lookupInternal("_setterToken")));
    arguments->insertAtTail(assign->argList->get(2)->copy());
    REPLACE(new CallExpr(call->baseExpr->copy(), arguments));
  }
 Ldone:
  // top down, on the modified AST
  Vec<BaseAST *> asts;
  get_ast_children(ast, asts);
  forv_BaseAST(a, asts)
    process(a);
}

void ApplyGettersSetters::run(ModuleList* moduleList) {
  if (!applyGettersSetters)
    return;
  Vec<Symbol *> symbols;
  for (ModuleSymbol* mod = moduleList->first(); mod; mod = moduleList->next())
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

