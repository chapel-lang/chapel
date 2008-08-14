#include "baseAST.h"
#include "callInfo.h"
#include "expr.h"


CallInfo::CallInfo(CallExpr* icall) : call(icall), scope(NULL) {
  name = toSymExpr(call->baseExpr)->getName();
  if (call->numActuals() >= 2) {
    if (SymExpr* se = toSymExpr(call->get(1))) {
      if (se->var == gModuleToken) {
        se->remove();
        se = toSymExpr(call->get(1));
        INT_ASSERT(se);
        ModuleSymbol* mod = toModuleSymbol(se->var);
        INT_ASSERT(mod);
        se->remove();
        scope = mod->block;
      }
    }
  }
  for_actuals(actual, call) {
    if (NamedExpr* named = toNamedExpr(actual)) {
      actualNames.add(named->name);
      actual = named->actual;
    } else {
      actualNames.add(NULL);
    }
    SymExpr* se = toSymExpr(actual);
    INT_ASSERT(se);
    Type* t = se->var->type;
    if (t == dtUnknown || t->isGeneric)
      INT_FATAL(call, "actual type is unknown or generic");
    actuals.add(se->var);
  }
}
