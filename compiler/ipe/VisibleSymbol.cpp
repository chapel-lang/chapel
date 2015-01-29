#include "VisibleSymbol.h"

#include "DefScope.h"

VisibleSymbol::VisibleSymbol(Symbol* sym, const DefScope* scope)
{
  mSymbol = sym;
  mScope  = scope;
}

VisibleSymbol::~VisibleSymbol()
{

}

Symbol* VisibleSymbol::symbol() const
{
  return mSymbol;
}

const DefScope* VisibleSymbol::scope() const
{
  return mScope;
}

int VisibleSymbol::depth() const
{
  return mScope->depthGet();
}
