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

#include "IpeScope.h"

#include "expr.h"
#include "IpeEnv.h"
#include "IpeModule.h"
#include "IpeProcedure.h"
#include "ipeDriver.h"
#include "symbol.h"

IpeScope::IpeScope(IpeScope* parent)
{
  mParent = parent;
}

IpeScope::~IpeScope()
{

}

bool IpeScope::isScopeBlockStmt() const
{
  return false;
}

bool IpeScope::isScopeModule() const
{
  return false;
}

void IpeScope::useAdd(IpeModule* use)
{
  for (size_t i = 0; i < mUsedModules.size(); i++)
    INT_ASSERT(mUsedModules[i] != use);

  mUsedModules.push_back(use);
}

int IpeScope::useCount() const
{
  return (int) mUsedModules.size();
}

IpeModule* IpeScope::useGet(int index) const
{
  IpeModule* retval = NULL;

  if (index >= 0 && index < (int) mUsedModules.size())
    retval = mUsedModules[index];

  return retval;
}

void IpeScope::varAdd(LcnSymbol* variable)
{
  for (size_t i = 0; i < mVariables.size(); i++)
    INT_ASSERT(mVariables[i] != variable);

  mVariables.push_back(variable);
}

int IpeScope::varCount() const
{
  return (int) mVariables.size();
}

LcnSymbol* IpeScope::varGet(int index) const
{
  LcnSymbol* retval = NULL;

  if (index >= 0 && index < (int) mVariables.size())
    retval = mVariables[index];

  return retval;
}

#if 0
bool IpeScope::isDefinedLocally(const char* name) const
{
  return (findVariableFoo(name, true) != NULL) ? true : false;
}

LcnSymbol* IpeScope::findVariableFoo(UnresolvedSymExpr* expr, bool localOnly) const
{
  return findVariableFoo(expr->unresolved, localOnly);
}

LcnSymbol* IpeScope::findVariableFoo(const char* identifier, bool localOnly) const
{
  LcnSymbol* retval = NULL;

  INT_ASSERT(identifier);

  for (size_t i = 0; i < mVariables.size() && retval == NULL; i++)
    retval = (mVariables[i]->name == identifier) ? mVariables[i] : NULL;

  // Possibly search over the used Modules
  if (retval == NULL && localOnly == false)
  {
    std::vector<LcnSymbol*> usedMatches;

    for (size_t i = 0; i < mUsedModules.size(); i++)
    {
      if (LcnSymbol* var = mUsedModules[i]->environment()->findVariable(identifier))
        usedMatches.push_back(var);
    }

    if (usedMatches.size() > 0)
      retval = usedMatches[0];

    for (size_t i = 1; i < usedMatches.size(); i++)
    {
      if (retval != usedMatches[i])
        printf("IpeScope::findVariable %s   multiple definitions\n", identifier);
    }
  }

  // Possibly recurse to the parent
  if (retval == NULL && localOnly == false && mParent != NULL)
    retval = mParent->findVariableFoo(identifier, false);

  return retval;
}
#endif

bool IpeScope::hasParent() const
{
  return (mParent != NULL) ? true : false;
}

void IpeScope::parentSummarize(char* pad) const
{
  printf("%s  Parent: %-12s %s\n", pad, mParent->type(), mParent->name());
}

