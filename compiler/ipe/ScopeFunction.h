#ifndef _SCOPE_FUNCTION_H_
#define _SCOPE_FUNCTION_H_

#include "ScopeBase.h"

class FnSymbol;

class ScopeFunction : public ScopeBase
{
public:
                        ScopeFunction(FnSymbol*        fn,
                                      const ScopeBase* parent);
                       ~ScopeFunction();

private:
                        ScopeFunction();

  virtual void          describeHeader(FILE* fp)                    const;

  FnSymbol*             mFunction;
};

#endif
