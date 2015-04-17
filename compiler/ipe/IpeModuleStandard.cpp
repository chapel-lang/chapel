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

#include "IpeModuleStandard.h"

#include "AstDumpToNode.h"
#include "expr.h"
#include "files.h"
#include "IpeModuleRoot.h"
#include "IpeReaderFile.h"

IpeModuleStandard::IpeModuleStandard(ModuleSymbol* sym) : IpeModule(sym)
{

}

IpeModuleStandard::~IpeModuleStandard()
{
}

const char* IpeModuleStandard::moduleTypeAsString() const
{
  return "Standard";
}

bool IpeModuleStandard::loadAndInitialize(IpeModuleRoot* rootModule)
{
  bool retval = false;;

  if      (loadFile(rootModule, "ChapelRepl")     == false)
    retval = false;

  else
    retval =  true;

  return retval;
}

bool IpeModuleStandard::loadFile(IpeModuleRoot* rootModule, const char* baseName)
{
  bool retval = false;

  if (const char* pathName = pathNameForStandardFile(baseName))
  {
    std::vector<DefExpr*> defs = IpeReaderFile::readModules(pathName, MOD_STANDARD);

    for (size_t i = 0; i < defs.size(); i++)
    {
      DefExpr*      expr   = defs[i];
      ModuleSymbol* modSym = toModuleSymbol(expr->sym);

      INT_ASSERT(modSym);

      rootModule->moduleAdd(new IpeModuleStandard(modSym));
    }

    retval = true;
  }
  else
    printf("IpeModuleStandard::loadFile   failed to find path for %s\n", baseName);

  return retval;
}

