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

#include "IpeCallExpr.h"

#include "AstDumpToNode.h"

IpeCallExpr::IpeCallExpr(Expr* baseExpr,
                         Type* type,
                         int   generation,
                         int   methodIndex) : CallExpr(baseExpr)
{
  mType         = type;
  mProcedureGen = generation;
  mMethodSel    = methodIndex;
}

IpeCallExpr::IpeCallExpr(PrimitiveOp* prim) : CallExpr(prim)
{
  mType         = NULL;
  mProcedureGen =   -1;
  mMethodSel    =   -1;
}

IpeCallExpr::IpeCallExpr(PrimitiveTag prim, Expr* arg1) : CallExpr(prim, arg1)
{
  mType         = NULL;
  mProcedureGen =   -1;
  mMethodSel    =   -1;
}

IpeCallExpr::~IpeCallExpr()
{

}

Type* IpeCallExpr::typeGet() const
{
  return mType;
}

int IpeCallExpr::procedureGeneration() const
{
  return mProcedureGen;
}

int IpeCallExpr::methodId() const
{
  return mMethodSel;
}

void IpeCallExpr::describe(int offset) const
{
  AstDumpToNode logger(stdout, offset + 3);
  char          pad[32] = { '\0' };

  if (offset < 32)
  {
    char* tptr = pad;

    for (int i = 0; i < offset; i++)
      *tptr++ = ' ';

    *tptr = '\0';
  }

  printf("%s#<IpeCallExpr %2d %2d\n", pad, mProcedureGen, mMethodSel);

  printf("%s   ", pad);

  ((IpeCallExpr*) this)->accept(&logger);
  printf("\n");

  if (mType)
  {
    printf("%s   ", pad);
    mType->accept(&logger);
    printf("\n");
  }

  printf("%s>\n", pad);
}
