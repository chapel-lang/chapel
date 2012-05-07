#include "baseAST.h"
#include "callInfo.h"
#include "expr.h"


CallInfo::CallInfo(CallExpr* icall, bool genericArgumentsFatal) : call(icall), fn(NULL), scope(NULL) {
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
    if (genericArgumentsFatal) {
      Type* t = se->var->type;
      if (t == dtUnknown || t->symbol->hasFlag(FLAG_GENERIC))
        INT_FATAL(call, "the type of the actual argument '%s' is unknown or generic", se->var->name);
    }
    actuals.add(se->var);
  }
}

CallInfo::CallInfo(FnSymbol* iface, CongruenceClosure &cclosure, bool includeNames) :
    call(NULL), fn(iface), scope(NULL) {
  name = iface->name;

  for_alist (formal, iface->formals) {
    if (DefExpr *de = toDefExpr(formal)) {
      if (includeNames)
        actualNames.add(de->sym->cname);
      else
        actualNames.add(NULL);
      actuals.add(de->sym);
    } else {
      INT_FATAL("Unimplemented case in CallInfo(FnSymbol* fn).  Not DefExpr.");
    }
  }
}
