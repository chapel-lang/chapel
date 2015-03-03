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

#include "IpeScopeProcedure.h"

#include "AstDumpToNode.h"
#include "IpeProcedure.h"
#include "symbol.h"

#include "misc.h"

IpeScopeProcedure::IpeScopeProcedure(IpeProcedure* module, IpeScope* parent)
  : IpeScope(parent)
{
  INT_ASSERT(parent);

  mProcedure    = module;
  mDepth        = 1;

  mMaxFrameSize = 0;
  mOffset       = 0;    // Leave a slot for the lexical parent
}

IpeScopeProcedure::~IpeScopeProcedure()
{

}

const char* IpeScopeProcedure::type() const
{
  return "Procedure";
}

const char* IpeScopeProcedure::name() const
{
  return mProcedure->name();
}

void IpeScopeProcedure::describeHeader(int offset) const
{
  char pad[32] = { '\0' };

  if (offset < 32)
  {
    char* tptr = pad;

    for (int i = 0; i < offset; i++)
      *tptr++ = ' ';

    *tptr = '\0';
  }

  printf("%s  depth:        %3d\n", pad, mDepth);
  printf("%s  frameSize:    %3d\n", pad, mOffset);
  printf("%s  maxFrameSize: %3d\n", pad, mMaxFrameSize);
}

int IpeScopeProcedure::maxFrameSize() const
{
  return mMaxFrameSize;
}

void IpeScopeProcedure::extend(Symbol* sym, IpeValue defaultValue, IpeVars* vars)
{
  if (LcnSymbol* lcn = toLcnSymbol(sym))
  {
    int depth = (mDepth >= 1) ? 1 : 0;

    lcn->locationSet(depth, mOffset);

    mOffset = mOffset + 8;

    if (mOffset > mMaxFrameSize)
      mMaxFrameSize = mOffset;
  }

  IpeScope::symbolPush(sym);
}

void IpeScopeProcedure::envPush()
{
  mDepth = mDepth + 1;

  mOffsetStack.push(mOffset);
}

void IpeScopeProcedure::envPop()
{
  mDepth  = mDepth - 1;
  mOffset = mOffsetStack.top();

  mOffsetStack.pop();
}
