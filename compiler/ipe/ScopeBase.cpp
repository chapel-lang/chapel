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

#include "ScopeBase.h"

#include "AstDumpToNode.h"
#include "ScopeModule.h"
#include "stmt.h"
#include "symbol.h"
#include "VisibleSymbol.h"

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

ScopeBase::ScopeBase(const ScopeBase* parent)
{
  mParent = parent;
}

ScopeBase::~ScopeBase()
{

}

void ScopeBase::extend(BlockStmt* block)
{
  std::vector<ModuleSymbol*> visited;

  extend(block, visited);
}

void ScopeBase::extend(BlockStmt*                  block,
                       std::vector<ModuleSymbol*>& visited)
{
  for_alist(stmt, block->body)
  {
    if (DefExpr* defExpr = toDefExpr(stmt))
    {
      addSym(defExpr->sym);
    }
    else if (CallExpr* callExpr = toCallExpr(stmt))
    {
      if (callExpr->isPrimitive(PRIM_USE) == true)
        addUse(callExpr->get(1));
    }
  }
}

void ScopeBase::addSym(Symbol* symbol)
{
  if (isConflicted(symbol) == false)
    mSymbols.push_back(symbol);
}

void ScopeBase::addUse(Expr* expr)
{
  if (ModuleSymbol* module = moduleByName(expr))
  {
    addUse(module);
  }
  else
  {
    AstDumpToNode logger(stdout, 3);

    printf("ScopeBase::extend: Failed to find module for\n");
    printf("   ");
    expr->accept(&logger);
    printf("\n\n");
  }
}

void ScopeBase::addUse(ModuleSymbol* module)
{
  std::vector<ModuleSymbol*> visited;

  addUse(module, visited);
}

void ScopeBase::addUse(ModuleSymbol*               module,
                       std::vector<ModuleSymbol*>& visited)
{
  if (isConflicted(module)       == false &&
      isVisited(module, visited) == false)
  {
    ScopeModule* usedScope = new ScopeModule(module);

    mUsedScopes.push_back(usedScope);

    visited.push_back(module);
  }
}

ModuleSymbol* ScopeBase::moduleByName(Expr* expr) const
{
  ModuleSymbol* retval = 0;

  if (UnresolvedSymExpr* unres = toUnresolvedSymExpr(expr))
  {
    std::vector<VisibleSymbol> symbols;

    visibleSymbols(unres, symbols);

    if (symbols.size() == 0)
    {
      const char* name = unres->unresolved;

      forv_Vec(ModuleSymbol, mod, gModuleSymbols)
      {
        if (strcmp(mod->name, name) == 0)
        {
          retval = mod;
          break;
        }
      }
    }
    else
      retval = toModuleSymbol(symbols[0].symbol());
  }

  return retval;
}

bool ScopeBase::isVisited(ModuleSymbol*               module,
                          std::vector<ModuleSymbol*>& visited)
{
  bool retval = false;

  for (size_t i = 0; i < visited.size() && retval == false; i++)
    retval = (visited[i] == module) ? true : false;

  return retval;
}


// A scope can have many definitions for a function but
// only one definition for other symbols
bool ScopeBase::isConflicted(Symbol* sym) const
{
  bool retval = false;

  if (isFnSymbol(sym) == false)
  {
    for (size_t i = 0; i < mSymbols.size() && retval == false; i++)
    {
      if (mSymbols[i] == sym)
      {
        AstDumpToNode logger(stdout, 3);

        printf("Error: Symbol is redefined\n");
        printf("   ");
        sym->accept(&logger);
        printf("\n");

        retval = true;
      }
    }
  }

  return retval;
}

// It would be surprising if a module were 'used' more than
// once in a single scope
bool ScopeBase::isConflicted(ModuleSymbol* mod) const
{
  bool retval = false;

  for (size_t i = 0; i < mUsedScopes.size() && retval == false; i++)
  {
    if (mUsedScopes[i]->isScopeForModule(mod) == true)
    {
      AstDumpToNode logger(stdout, 3);

      printf("Warning: Module is 'used' more than once in one scope\n");
      printf("   ");
      mod->accept(&logger);
      printf("\n");

      retval = true;
    }
  }

  return retval;
}


/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

void ScopeBase::visibleSymbols(UnresolvedSymExpr*          unres,
                               std::vector<VisibleSymbol>& symbols) const
{
  visibleSymbols(unres, symbols, 0);
}

void ScopeBase::visibleSymbols(UnresolvedSymExpr*          unres,
                               std::vector<VisibleSymbol>& symbols,
                               int                         distance)  const
{
  const char* needle = unres->unresolved;

  for (size_t i = 0; i < mSymbols.size(); i++)
  {
    if (mSymbols[i]->name == needle)
    {
      VisibleSymbol vis(mSymbols[i], this, distance);

      symbols.push_back(vis);
    }
  }

  for (size_t i = 0; i < mUsedScopes.size(); i++)
  {
    mUsedScopes[i]->visibleSymbols(unres, symbols, distance + 1);
  }

  if (mParent != 0)
  {
    mParent->visibleSymbols(unres, symbols, distance + 1);
  }
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

void ScopeBase::describe(bool recursive) const
{
  printf("#<Scope ");
  describeHeader(stdout);
  printf("\n");

  printf("   symbols:\n");

  for (size_t i = 0; i < mSymbols.size(); i++)
    printf("       %3d: %s\n", (int) i, mSymbols[i]->name);

  if (mUsedScopes.size() > 0)
  {
    printf("\n");
    printf("   modUses:\n");

    for (size_t i = 0; i < mUsedScopes.size(); i++)
      printf("       %3d: %s\n", (int) i, mUsedScopes[i]->name());
  }

  printf(">\n\n");

  if (recursive == true)
  {
    for (size_t i = 0; i < mUsedScopes.size(); i++)
      mUsedScopes[i]->describe(true);

    if (mParent != 0)
      mParent->describe(true);
  }
}
