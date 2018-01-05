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

#include "IpeBlockStmt.h"

#include "AstDumpToNode.h"
#include "IpeEnv.h"
#include "IpeScopeBlock.h"
#include "ipeResolve.h"
#include "misc.h"

IpeBlockStmt::IpeBlockStmt(const std::vector<Expr*>& stmts,
                           IpeEnv*                   env)
  : IpeSequence(stmts, BLOCK_NORMAL)
{
  mEnv = env;
}

IpeBlockStmt::~IpeBlockStmt()
{

}

IpeEnv* IpeBlockStmt::envGet() const
{
  return mEnv;
}

bool IpeBlockStmt::isScopeless() const
{
  return false;
}

void IpeBlockStmt::describe(int offset)
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

  printf("%s#<IpeBlockStmt\n", pad);

  mEnv->describe(offset + 3);
  printf("\n");

  printf("%s   ", pad);
  accept(&logger);
  printf("\n");

  printf("%s>\n", pad);
}

#if 0
IpeScopeBlock* IpeBlockStmt::scopeGet() const
{
  return mScope;
}

void IpeBlockStmt::varAdd(LcnSymbol* variable)
{
  INT_ASSERT(variable);

  mScope->varAdd(variable);
}
#endif

