#include "ScopeModule.h"

#include "stmt.h"
#include "symbol.h"

static int sRootModuleIndex = 0;

/************************************ | *************************************
*                                                                           *
* IPE needs to extract core definitions from the Root Module but this       *
* module is constantly updated.  We arrange for the core initialization     *
* routine to call this function so that it can establish a high water mark. *
*                                                                           *
************************************* | ************************************/

void ipeRootInit()
{
  sRootModuleIndex = rootModule->block->body.length;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

ScopeModule::ScopeModule(ModuleSymbol* module) : ScopeBase(NULL)
{
  mModule = module;

  if (module == baseModule)
  {
    addUse(rootModule);
  }

  if (module != rootModule)
  {
    extend(module->block);
  }
  else
  {
    BlockStmt* bs = rootModule->block;

    for (int i = 1; i <= sRootModuleIndex; i++)
    {
      Expr* stmt = bs->body.get(i);

      if (DefExpr* defExpr = toDefExpr(stmt))
      {
        addSym(defExpr->sym);
      }
      else
      {
        INT_ASSERT(false);
      }
    }

    forv_Vec(ModuleSymbol, mod, gModuleSymbols)
      addSym(mod);
  }
}

ScopeModule::~ScopeModule()
{

}

void ScopeModule::describeHeader(FILE* fp) const
{
  fprintf(fp, "Module %s", name());
}

bool ScopeModule::isScopeForModule(const ModuleSymbol* mod) const
{
  return (mModule == mod) ? true : false;
}

const char* ScopeModule::name() const
{
  return mModule->name;
}
