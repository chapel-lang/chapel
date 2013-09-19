#include "baseAST.h"
#include "callInfo.h"
#include "expr.h"


CallInfo::CallInfo(CallExpr* icall) : call(icall), scope(NULL) {
  if (SymExpr* se = toSymExpr(call->baseExpr))
    name = se->var->name;
  else if (UnresolvedSymExpr* use = toUnresolvedSymExpr(call->baseExpr))
    name = use->unresolved;
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
    if (t == dtUnknown)
      USR_FATAL(call, "use of '%s' before encountering its definition, type unknown", se->var->name);
    if (t->symbol->hasFlag(FLAG_GENERIC))
      INT_FATAL(call, "the type of the actual argument '%s' is generic", se->var->name);
    actuals.add(se->var);
  }
}
