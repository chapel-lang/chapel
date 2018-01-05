/*
 * Copyright 2004-2018 Cray Inc.
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

#include "IpeScopeModule.h"

#include "AstDumpToNode.h"
#include "IpeModule.h"
#include "IpeModuleRoot.h"
#include "IpeEnv.h"
#include "ipeDriver.h"

IpeScopeModule::IpeScopeModule(IpeModule* parent, IpeModule* module)
  : IpeScope(parent != NULL ? parent->scopeGet() : NULL)
{
  mModule     = module;
}

IpeScopeModule::~IpeScopeModule()
{

}

IpeModule* IpeScopeModule::module() const
{
  return mModule;
}

const char* IpeScopeModule::type() const
{
  return "Module";
}

bool IpeScopeModule::isScopeModule() const
{
  return true;
}

const char* IpeScopeModule::name() const
{
  return (mModule != 0) ? mModule->name() : "dummy-name";
}

int IpeScopeModule::locationSet(ArgSymbol* arg) const
{
  AstDumpToNode logger(stdout, 3);

  printf("   IpeScopeModule::locationSet(ArgSymbol*)  unsupported\n");
  printf("   ");
  arg->accept(&logger);
  printf("\n\n");

  INT_ASSERT(false);

  return 0;
}

int IpeScopeModule::locationSet(VarSymbol* var) const
{
#if 0
  int   delta  = 8;             // Currently, everything is 8 bytes
  int   offset = 0;

  if (var == NULL)
  {
    AstDumpToNode logger(stdout, 3);

    printf("   IpeScopeModule::locationSet(VarSymbol*)  unsupported\n");
    printf("   ");
    var->accept(&logger);
    printf("\n\n");

    INT_ASSERT(false);
  }

  else if (var->depth() == -1 && var->offset() == -1)
  {
    offset = IpeEnv::reserveSpace(var);
    var->locationSet(0, offset);
  }

  else if (var->type == gIpeTypeModule)
  {
    offset = var->offset();
  }

  else if (var->type == gIpeTypeProcedure)
  {
    offset = var->offset();
  }

  else
  {
    AstDumpToNode logger(stdout, 3);

    printf("   IpeScopeModule::locationSet(VarSymbol*)  unsupported\n");
    printf("   ");
    var->accept(&logger);
    printf("\n\n");

    INT_ASSERT(false);
  }

  return offset + delta;
#else

  INT_ASSERT(false);

  return 0;

#endif
}

int IpeScopeModule::frameSize() const
{
  return 0;
}

