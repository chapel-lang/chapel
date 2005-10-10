#include "normalizeFunctions.h"
#include "symtab.h"
#include "symbol.h"
#include "stmt.h"
#include "expr.h"
#include "stringutil.h"
#include "../traversals/updateSymbols.h"

void NormalizeFunctions::processSymbol(Symbol* sym) {
  if (!formalTemps)
    return;

  FnSymbol* fn = dynamic_cast<FnSymbol*>(sym);

  if (!fn)
    return;

  if (!strcmp("=", fn->name))
    return;

  Vec<DefExpr*> tempDefs;
  ASTMap subs;

  for_alist_backward(DefExpr, formalDef, fn->formals) {
    ArgSymbol* formal = dynamic_cast<ArgSymbol*>(formalDef->sym);
    if (formal->intent == INTENT_REF ||
        formal->intent == INTENT_PARAM ||
        formal->intent == INTENT_TYPE ||
        formal->genericSymbol)
      continue;
    Type *type = formal->type;
    if (type == dtAny || type == dtNumeric)
      type = dtUnknown;
    assert(!type->isGeneric);
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
