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

#include "IpeScopeMethod.h"

#include "AstDumpToNode.h"
#include "IpeMethod.h"
#include "IpeModule.h"
#include "symbol.h"

IpeScopeMethod::IpeScopeMethod(IpeMethod* method, IpeScope* parent) : IpeScope(parent)
{
  mMethod = method;
}

IpeScopeMethod::~IpeScopeMethod()
{

}

const char* IpeScopeMethod::type() const
{
  return "Method";
}

const char* IpeScopeMethod::name() const
{
  return (mMethod != 0) ? mMethod->name() : "dummy-name";
}

int IpeScopeMethod::locationSet(ArgSymbol* arg) const
{
  int offset =  0;
  int retval = -1;

  for (size_t i = 0; i < mVariables.size() && retval == -1; i++)
  {
    if (mVariables[i] == arg)
    {
      retval = offset;
      arg->locationSet(1, offset);
    }
    else
    {
      offset = offset + 8;
    }
  }

  INT_ASSERT(retval >= 0);

  return retval;
}

int IpeScopeMethod::locationSet(VarSymbol* var) const
{
  if (var == NULL || var->depth() >= 0 || var->offset() >= 0)
  {
    AstDumpToNode logger(stdout, 3);

    printf("   IpeScopeMethod::locationSet(VarSymbol*)  unsupported\n");
    printf("   ");
    var->accept(&logger);
    printf("\n\n");

    INT_ASSERT(false);
  }

  else
  {
    AstDumpToNode logger(stdout, 3);

    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("   IpeScopeMethod::locationSet(VarSymbol*)  incomplete\n");
    printf("   ");
    var->accept(&logger);
    printf("\n\n");

    INT_ASSERT(false);
  }

  return 0;
}

int IpeScopeMethod::frameSize() const
{
  int delta = 8;           // NOAKES  Size of every type is currently 8

  return mVariables.size() * delta;
}

