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

#include "DefScope.h"

#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "VisibleSymbol.h"

#include "AstDumpToNode.h"

/************************************ | *************************************
*                                                                           *
* Class State                                                               *
*                                                                           *
************************************* | ************************************/

std::vector<DefScope*> DefScope::mScopes;

DefScope* DefScope::createRoot()
{
  DefScope* retval = new DefScope(0);

  mScopes.push_back(retval);

  return retval;
}

DefScope* DefScope::extend(const DefScope* parent)
{
  DefScope* retval = new DefScope(parent);

  mScopes.push_back(retval);

  return retval;
}

/************************************ | *************************************
*                                                                           *
* Instance State                                                            *
*                                                                           *
************************************* | ************************************/

DefScope::DefScope(const DefScope* parent)
{
  mParent =  parent;
  mDepth  = (parent == 0) ? 0 : parent->mDepth + 1;
}

DefScope::~DefScope()
{

}

int DefScope::depthGet() const
{
  return mDepth;
}

void DefScope::depthSet(int depth)
{
  mDepth = depth;
}

void DefScope::addDefinition(DefExpr* expr)
{
  mDefinitions.push_back(expr);
}

void DefScope::extendByModule(ModuleSymbol* module)
{
  for_alist(stmt, module->block->body)
  {
    if (DefExpr*  expr = toDefExpr(stmt))
    {
      Symbol* sym = expr->sym;

      if (isVarSymbol(sym)    == true ||
          isTypeSymbol(sym)   == true ||
          isFnSymbol(sym)     == true ||
          isModuleSymbol(sym) == true)
        addDefinition(expr);

      else
      {
        AstDumpToNode logger(stdout);

        printf("DefScope::extendByModule failed\n");

        expr->accept(&logger);
        printf("\n");

        sym->accept(&logger);
        printf("\n\n\n\n\n");
      }
    }
  }
}

void DefScope::visibleSymbols(UnresolvedSymExpr*          unres,
                              std::vector<VisibleSymbol>& symbols) const
{
  const char* needle = unres->unresolved;

  // Use iteration to scan the chain of scopes
  for (const DefScope* scope = this; scope != 0; scope = scope->mParent)
  {
    for (size_t i = 0; i < scope->mDefinitions.size(); i++)
    {
      if (scope->mDefinitions[i]->name() == needle)
      {
        VisibleSymbol vis(scope->mDefinitions[i]->sym, scope);

        symbols.push_back(vis);
      }
    }
  }
}
