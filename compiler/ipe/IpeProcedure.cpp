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

#include "IpeProcedure.h"

#include "AstDumpToNode.h"
#include "expr.h"
#include "IpeScopeProcedure.h"
#include "ipeResolve.h"
#include "symbol.h"

IpeProcedure::IpeProcedure(FnSymbol* fnSym, IpeScope* scope)
{
  mFnDecl    = fnSym;
  mScope     = new IpeScopeProcedure(this, scope);
  mVars      = NULL;
  mState     = kUnresolved;
  mFrameSize = 0;
}

IpeProcedure::~IpeProcedure()
{

}

const char* IpeProcedure::name() const
{
  return mFnDecl->name;
}

IpeScopeProcedure* IpeProcedure::scope() const
{
  return mScope;
}

FnSymbol* IpeProcedure::fnSymbol() const
{
  return mFnDecl;
}

bool IpeProcedure::exactMatch(std::vector<Type*>& actualsTypes)
{
  bool retval = false;

  if (mState == kUnresolved)
  {
    ipeResolveFormalsTypes(this, mScope, mVars);

    mState = kFormalsResolved;
  }

  if (mFnDecl->formals.length == (int) actualsTypes.size())
  {
    bool match = true;

    for (size_t i = 0; i < actualsTypes.size() && match == true; i++)
    {
      DefExpr*   expr = toDefExpr(mFnDecl->formals.get(i + 1));
      INT_ASSERT(expr);

      ArgSymbol* arg  = toArgSymbol(expr->sym);

      INT_ASSERT(arg);

      match = (actualsTypes[i] == arg->type) ? true : false;
    }

    retval = match;
  }

  return retval;
}

bool IpeProcedure::ensureBodyResolved()
{
  bool retval = false;

  if (mState != kResolved)
  {
    INT_ASSERT(mState == kFormalsResolved);

    ipeResolveBody      (this, mScope, mVars);
    ipeResolveReturnType(this, mScope, mVars);

    mState = kResolved;
    retval = true;
  }

  return retval;
}

void IpeProcedure::frameSizeSet(int size)
{
  mFrameSize = size;
}

int IpeProcedure::frameSize() const
{
  return mFrameSize;
}

void IpeProcedure::describe(int offset) const
{
  AstDumpToNode logger(stdout, offset + 2);

  char          pad[32] = { '\0' };

  if (offset < 32)
  {
    char* tptr = pad;

    for (int i = 0; i < offset; i++)
      *tptr++ = ' ';

    *tptr = '\0';
  }

  printf("%s#<IpeProcedure %s\n", pad, name());
  printf("%s  FrameSize: %3d\n",  pad, mFrameSize);

  fputc('\n', stdout);
  fputs(pad,  stdout);
  fputs("  ", stdout);

  mFnDecl->accept(&logger);

  fputc('\n', stdout);

  printf("%s>\n", pad);
}
