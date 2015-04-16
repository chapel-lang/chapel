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

#include "IpeScopeBlock.h"

#include "AstDumpToNode.h"
#include "symbol.h"

#include "IpeEnv.h"

IpeScopeBlock::IpeScopeBlock(IpeScope* block) : IpeScope(block)
{

}

IpeScopeBlock::~IpeScopeBlock()
{

}

const char* IpeScopeBlock::type() const
{
  return "Block";
}

const char* IpeScopeBlock::name() const
{
  return "Block";
}

int IpeScopeBlock::frameSize() const
{
  int delta = 8;           // NOAKES  Size of every type is currently 8

  return mParent->frameSize() + mVariables.size() * delta;
}

bool IpeScopeBlock::isScopeBlockStmt() const
{
  return true;
}

int IpeScopeBlock::locationSet(ArgSymbol* arg) const
{
  AstDumpToNode logger(stdout, 3);

  printf("   IpeScopeBlock::locationSet(ArgSymbol*)  unsupported\n");
  printf("   ");
  arg->accept(&logger);
  printf("\n\n");

  INT_ASSERT(false);
  return 0;
}

int IpeScopeBlock::locationSet(VarSymbol* var) const
{
  int retval = -1;

  if (var == NULL || var->depth() >= 0 || var->offset() >= 0)
  {
    AstDumpToNode logger(stdout, 3);

    printf("   IpeScopeBlock::locationSet(VarSymbol*)  unsupported\n");
    printf("   ");
    var->accept(&logger);
    printf("\n\n");

    INT_ASSERT(false);
  }

  else
  {
    IpeEnv env((IpeScopeBlock*) this);
    int    offset = mParent->frameSize();

    for (size_t i = 0; i < mVariables.size() && retval == -1; i++)
    {
      if (mVariables[i] == var)
      {
        retval = offset;
        var->locationSet(1, offset);
      }
      else
      {
        offset = offset + 8;
      }
    }

    INT_ASSERT(retval >= 0);
  }

  return retval;
}
