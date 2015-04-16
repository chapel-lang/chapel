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

#include "ipeLocation.h"

#include "AstDumpToNode.h"
#include "expr.h"
#include "IpeBlockStmt.h"
#include "IpeEnv.h"
#include "IpeScope.h"
#include "IpeScopeBlock.h"
#include "WhileDoStmt.h"

// Returns max local frame space to evaluate this expr
int locationExpr(Expr* expr, IpeEnv* env)
{
  int retval = 0;

  if (DefExpr* defExpr = toDefExpr(expr))
  {
    VarSymbol* var   = toVarSymbol(defExpr->sym);
    int        delta = 8;           // NOAKES  Size of every type is currently 8

    INT_ASSERT(var);

    env->locationSet(var);

    retval = delta;
  }

  else if (isCallExpr(expr) == true)
    retval = 0;

  else if (CondStmt*    stmt = toCondStmt(expr))
  {
    if (stmt->elseStmt == NULL)
    {
      retval = locationExpr(stmt->thenStmt, env);
    }

    else
    {
      int thenSize = locationExpr(stmt->thenStmt, env);
      int elseSize = locationExpr(stmt->elseStmt, env);

      retval = (thenSize > elseSize) ? thenSize : elseSize;
    }
  }

  else if (WhileDoStmt* stmt = toWhileDoStmt(expr))
  {
    Expr* body = stmt->body.get(1);

    INT_ASSERT(stmt->body.length == 1);
    INT_ASSERT(isBlockStmt(body));

    retval = locationExpr(body, env);
  }

  else if (BlockStmt* stmt = toBlockStmt(expr))
  {
    IpeBlockStmt* ipeStmt  = (IpeBlockStmt*) stmt;
    int           maxFrame = 0;
    IpeEnv        env(ipeStmt->scopeGet());

    for (int i = 1; i <= ipeStmt->body.length; i++)
    {
      int localSize = locationExpr(ipeStmt->body.get(i), &env);

      if (localSize > maxFrame)
        maxFrame = localSize;
    }

    retval = maxFrame;
  }

  else
  {
    AstDumpToNode logger(stdout, 3);

    printf("   locationExpr(Expr*, IpeEnv* env)  unsupported\n");
    printf("   ");
    expr->accept(&logger);
    printf("\n\n");

    env->describe(3);
    printf("\n\n");

    INT_ASSERT(false);
  }

  return retval;
}
