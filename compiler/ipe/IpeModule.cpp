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

#include "IpeModule.h"

#include "AstDumpToNode.h"
#include "IpeScopeModule.h"
#include "ipeEvaluate.h"
#include "symbol.h"

IpeModule::IpeModule(ModuleSymbol* sym)
{
  mModuleDecl  = sym;
  mScope       = new IpeScopeModule(this);
  mInitialized = false;
}

IpeModule::~IpeModule()
{

}

const char* IpeModule::name() const
{
  return mModuleDecl->name;
}

IpeScopeModule* IpeModule::scope() const
{
  return mScope;
}

void IpeModule::ensureInitialized(IpeVars* vars)
{
  if (mInitialized == false)
  {
    mInitialized = true;
    ipeEvaluate(mModuleDecl, vars);
  }
}

void IpeModule::describe(int offset) const
{
  char pad[32] = { '\0' };

  if (offset < 32)
  {
    char* tptr = pad;

    for (int i = 0; i < offset; i++)
      *tptr++ = ' ';

    *tptr = '\0';
  }

  printf("%s#<IpeModule %s\n", pad, name());
  printf("%s   Initialized: %s\n", pad, mInitialized ? " true" : "false");

  if (mScope)
    mScope->describe(offset + 3, false);

  printf("%s>\n", pad);
}

