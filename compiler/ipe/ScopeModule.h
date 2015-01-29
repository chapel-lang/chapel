#ifndef _SCOPE_MODULE_H_
#define _SCOPE_MODULE_H_

#include "ScopeBase.h"

class ModuleSymbol;

class ScopeModule : public ScopeBase
{
public:
                        ScopeModule(ModuleSymbol* module);
                       ~ScopeModule();

  const char*           name()                                      const;

  bool                  isScopeForModule(const ModuleSymbol* mod)   const;

private:
                        ScopeModule();

  virtual void          describeHeader(FILE* fp)                    const;

  ModuleSymbol*         mModule;
};

#endif
