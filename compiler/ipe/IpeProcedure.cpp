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
#include "IpeCallExpr.h"
#include "IpeMethod.h"

IpeProcedure::IpeProcedure(const char* identifierName)
{
  mIdentifierName = identifierName;
  mVersion        =              1;
}

IpeProcedure::~IpeProcedure()
{

}

const char* IpeProcedure::name() const
{
  return mIdentifierName;
}

bool IpeProcedure::isValid(int generationId) const
{
  return (mVersion == generationId) ? true : false;
}

int IpeProcedure::methodCount() const
{
  return mMethods.size();
}

void IpeProcedure::methodAdd(IpeMethod* method)
{
  mMethods.push_back(method);
}

IpeMethod* IpeProcedure::methodGet(int index) const
{
  IpeMethod* retval = NULL;

  if (index >= 0 && index < (int) mMethods.size())
    retval = mMethods[index];

  return retval;
}

IpeCallExpr* IpeProcedure::resolve(SymExpr*            procSymExpr,
                                   std::vector<Expr*>& actuals) const
{
  IpeMethod*   matchMethod = NULL;
  int          matchIndex  =    0;
  int          matchCount  =    0;

  IpeCallExpr* retval      =    0;

  for (size_t i = 0; i < mMethods.size(); i++)
  {
    mMethods[i]->resolveFormals();
    mMethods[i]->resolveReturnType();
  }

  for (size_t i = 0; i < mMethods.size(); i++)
  {
    if (mMethods[i]->isExactMatch(actuals) == true)
    {
      matchMethod = mMethods[i];
      matchIndex  = i;
      matchCount  = matchCount + 1;
    }
  }

  if (matchCount == 1)
  {
    SET_LINENO(procSymExpr);

    retval = new IpeCallExpr(procSymExpr->copy(), matchMethod->typeGet(), mVersion, matchIndex);

    for (size_t i = 0; i < actuals.size(); i++)
    {
      ArgSymbol* formal      = matchMethod->formalGet(i);
      bool       formalIsRef = formal->intent & INTENT_REF;

      Expr*      actual      = actuals[i];
      bool       actualIsRef = isActualRef(actual);

      Expr*      actualCopy  = actual;
      Expr*      actualNew   = NULL;

      if      (formalIsRef == false && actualIsRef == false)
        actualNew = actualCopy;

      else if (formalIsRef == false && actualIsRef ==  true)
        INT_ASSERT(false);

      else if (formalIsRef == true  && actualIsRef == false)
        actualNew = new IpeCallExpr(PRIM_ADDR_OF, actualCopy);

      else if (formalIsRef == true  && actualIsRef ==  true)
        INT_ASSERT(false);

      retval->argList.insertAtTail(actualNew);
    }
  }
  else
  {
    AstDumpToNode logger(stdout, 6);

    printf("   IpeProcedure::resolve(SymExpr*, actuals[])\n");
    printf("   matchCount = %d\n", matchCount);
    printf("\n\n");

    printf("      ");
    procSymExpr->accept(&logger);
    printf("\n");

    for (size_t i = 0; i < actuals.size(); i++)
    {
      printf("   %d: ", (int) i);
      actuals[i]->accept(&logger);
      printf("\n");
    }

    INT_ASSERT(false);
  }

  return retval;
}

bool IpeProcedure::isActualRef(Expr* actual) const
{
  bool retval = false;

  if (SymExpr* symExpr = toSymExpr(actual))
  {
    if (ArgSymbol* arg = toArgSymbol(symExpr->var))
    {
      retval = (arg->intent & INTENT_REF) ? true : false;
    }
  }


  return retval;
}

void IpeProcedure::describe(int offset) const
{
  char pad[32] = { '\0' };

  if (offset < 32)
  {
    char* tptr = pad;

    for (int i = 0; i < offset; i++)
      *tptr++ = ' ';

    *tptr = '\0';
  }

  printf("%s#<IpeProcedure %s\n", pad, name());

  if (mMethods.size() == 1)
    printf("%s  %3d method\n",  pad, (int) mMethods.size());
  else
    printf("%s  %3d methods\n", pad, (int) mMethods.size());

  for (size_t i = 0; i < mMethods.size(); i++)
    mMethods[i]->describe(offset + 3);

  printf("%s>\n", pad);
}
