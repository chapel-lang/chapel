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

#include "IpeDefExpr.h"

#include "AstDumpToNode.h"




IpeDefExpr::IpeDefExpr(LcnSymbol* sym,
                       Expr*      initExpr,
                       Expr*      typeExpr) : DefExpr(sym, initExpr, typeExpr)
{
  mFnSymbol     = NULL;
  mModuleSymbol = NULL;
}


IpeDefExpr::IpeDefExpr(LcnSymbol* sym,
                       FnSymbol*  fnSymbol) : DefExpr(sym, NULL, NULL)
{
  mFnSymbol     = fnSymbol;
  mModuleSymbol = NULL;
}

IpeDefExpr::IpeDefExpr(LcnSymbol*     sym,
                       ModuleSymbol*  moduleSymbol) : DefExpr(sym, NULL, NULL)
{
  mFnSymbol     = NULL;
  mModuleSymbol = moduleSymbol;
}

IpeDefExpr::~IpeDefExpr()
{

}

FnSymbol* IpeDefExpr::fnSymbolGet() const
{
  return mFnSymbol;
}

ModuleSymbol* IpeDefExpr::moduleSymbolGet() const
{
  return mModuleSymbol;
}

void IpeDefExpr::describe(int offset)
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

  printf("%s#<IpeDefExpr\n", pad);

  printf("%s   ", pad);
  accept(&logger);
  printf("\n");

  if (mFnSymbol)
  {
    printf("\n");
    printf("%s   Function:\n", pad);
    printf("%s   ", pad);
    mFnSymbol->accept(&logger);
    printf("\n");
  }

  if (mModuleSymbol)
  {
    printf("\n");
    printf("%s   Module:\n", pad);
    printf("%s   ", pad);
    mModuleSymbol->accept(&logger);
    printf("\n");
  }

  printf("%s>\n", pad);
}
