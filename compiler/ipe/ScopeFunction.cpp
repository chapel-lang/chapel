#include "ScopeFunction.h"

#include "IpeSymbol.h"
#include "stmt.h"
#include "symbol.h"

ScopeFunction::ScopeFunction(FnSymbol* fn, const ScopeBase* parent) : ScopeBase(parent)
{
  mFunction = fn;

  for (int i = 1; i <= fn->formals.length; i++)
  {
    DefExpr*   defExpr = 0;
    ArgSymbol* formal  = 0;

    defExpr = toDefExpr(fn->formals.get(i));
    INT_ASSERT(defExpr);

    formal = toArgSymbol(defExpr->sym);

    if (formal == 0)
    {
      IpeSymbol* ipe = toIpeSymbol(defExpr->sym);

      INT_ASSERT(ipe);

      formal = toArgSymbol(ipe->symbol());
    }

    INT_ASSERT(formal);
    INT_ASSERT(formal->type);

    addSym(defExpr->sym);
  }
}

ScopeFunction::~ScopeFunction()
{

}

void ScopeFunction::describeHeader(FILE* fp) const
{
  fprintf(fp, "Function %s", mFunction->name);
}
