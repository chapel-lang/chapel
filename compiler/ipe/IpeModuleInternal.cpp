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

#include "IpeModuleInternal.h"

#include "AstDumpToNode.h"
#include "expr.h"
#include "files.h"
#include "IpeModuleRoot.h"
#include "IpeReaderFile.h"
#include "stmt.h"
#include "stringutil.h"

IpeModuleInternal::IpeModuleInternal(ModuleSymbol* sym) : IpeModule(sym)
{

}

IpeModuleInternal::~IpeModuleInternal()
{

}

const char* IpeModuleInternal::moduleTypeAsString() const
{
  return "Internal";
}

bool IpeModuleInternal::loadAndInitialize(IpeModuleRoot* rootModule)
{
  bool retval = false;;

  if      (loadFile(rootModule, "ChapelBase")     == false)
    retval = false;

  else if (loadFile(rootModule, "ChapelStandard") == false)
    retval = false;

  else
    retval =  true;

  return retval;
}

bool IpeModuleInternal::loadFile(IpeModuleRoot* rootModule, const char* baseName)
{
  bool retval = false;

  if (const char* pathName = pathNameForInternalFile(baseName))
  {
    std::vector<DefExpr*> defs = IpeReaderFile::readModules(pathName, MOD_INTERNAL);

    for (size_t i = 0; i < defs.size(); i++)
    {
      DefExpr*      expr   = defs[i];
      ModuleSymbol* modSym = toModuleSymbol(expr->sym);

      INT_ASSERT(modSym);

      rootModule->moduleAdd(new IpeModuleInternal(modSym));
    }

    retval = true;
  }
  else
    printf("IpeModuleInternal::loadFile   failed to find path for %s\n", baseName);

  return retval;
}

