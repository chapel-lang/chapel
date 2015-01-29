/*
 * Copyright 2004-2015 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ScopeModule.h"

#include "ipe.h"
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
