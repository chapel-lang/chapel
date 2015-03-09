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

#include "IpeScope.h"

#include "AstDumpToNode.h"
#include "expr.h"
#include "IpeVars.h"
#include "IpeModule.h"
#include "IpeScopeModule.h"
#include "ipeDriver.h"
#include "misc.h"
#include "symbol.h"
#include "type.h"
#include "VisibleSymbols.h"

IpeScope::IpeScope(IpeScope* parent)
{
  mParent = parent;
}

IpeScope::~IpeScope()
{

}

void IpeScope::envPush()
{

}

void IpeScope::envPop()
{

}

void IpeScope::useAdd(IpeModule* use)
{
  mUsedModules.push_back(use);
}

void IpeScope::symbolPush(Symbol* sym)
{
  mSymbols.push_back(sym);
}

VisibleSymbols IpeScope::visibleSymbols(UnresolvedSymExpr* expr) const
{
  return visibleSymbols(expr->unresolved);
}

VisibleSymbols IpeScope::visibleSymbols(Symbol* sym) const
{
  return visibleSymbols(sym->name);
}

VisibleSymbols IpeScope::visibleSymbols(const char* name) const
{
  VisibleSymbols retval;

  findSymbols(name, 0, retval);

  return retval;
}

void IpeScope::findSymbols(const char*     name,
                           int             distance,
                           VisibleSymbols& symbols) const
{
  for (size_t i = 0; i < mSymbols.size(); i++)
  {
    if (strcmp(name, mSymbols[i]->name) == 0)
      symbols.pushBack(mSymbols[i], distance);
  }

  for (size_t i = 0; i < mUsedModules.size(); i++)
  {
    mUsedModules[i]->scope()->findSymbols(name, distance + 1, symbols);
  }

  // NOAKES 2015/02/24  Nested modules.  Need to think about "distance"
  if (mParent != 0 && symbols.count() == 0)
    mParent->findSymbols(name, distance + 1, symbols);
}

void IpeScope::describe(int offset, bool recursiveP) const
{
  char pad[32] = { '\0' };
  int  size    = (int) mSymbols.size();

  if (offset < 32)
  {
    char* tptr = pad;

    for (int i = 0; i < offset; i++)
      *tptr++ = ' ';

    *tptr = '\0';
  }

  printf("%s#<IpeScope 0x%012lX %-8s %s\n", pad, (long) this, type(), name());
  describeHeader(offset);

  if (mParent != 0)
  {
    printf("%s  Parent: %s\n", pad, mParent->name());
  }

  if (mUsedModules.size() > 0)
  {
    for (size_t i = 0; i < mUsedModules.size(); i++)
    {
      if (i == 0)
        printf("%s  uses:   %s\n", pad, mUsedModules[i]->name());
      else
        printf("%s          %s\n", pad, mUsedModules[i]->name());
    }
  }

  for (int i = 0; i < size; i++)
  {
    const char* symName = mSymbols[i]->name;

    if      (isTypeSymbol(mSymbols[i]))
    {
      printf("%s   %3d: type %26s %s\n", pad, i, "", symName);
    }

    else if (isFnSymbol(mSymbols[i]) == true)
    {
      printf("%s   %3d: func %26s %s\n", pad, i, "", symName);
    }

    else if (VarSymbol* sym = toVarSymbol(mSymbols[i]))
    {
      const char* typeName = "??????";

      if (mSymbols[i]->type != 0)
        typeName = mSymbols[i]->type->symbol->name;

      if (sym->immediate != 0)
        printf("%s   %3d: const (%s)", pad, i, typeName);

      else if (sym->hasFlag(FLAG_CONST))
        printf("%s   %3d: const (%s)", pad, i, typeName);

      else if (sym->hasFlag(FLAG_PARAM))
        printf("%s   %3d: param (%s)", pad, i, typeName);

      else
        printf("%s   %3d: var   (%s)", pad, i, typeName);

      if (strlen(typeName) < 10)
      {
        int spaces = 10 - strlen(typeName);

        for (int j = 0; j <= spaces; j++)
          fputc(' ', stdout);
      }

      printf("   %4d %4d %s", sym->depth(), sym->offset(), symName);

      if (sym->depth() == 0 && sym->offset() >= 0)
      {
        IpeValue value = IpeVars::fetch(sym, NULL);

        if (strlen(symName) < 24)
        {
          for (size_t i = strlen(symName); i < 24; i++)
            fputc(' ', stdout);
        }

        if (sym->type == dtBools[BOOL_SIZE_SYS])
          printf("  %s", (value.boolGet() == true) ? " true" : "false");

        else if (sym->type == dtInt[INT_SIZE_64])
          printf("%7ld", value.integerGet());

        else if (sym->type == dtReal[FLOAT_SIZE_64])
          printf("%10.2f", value.realGet());

        else if (sym->type == gIpeTypeModule)
          printf("0x%012lX", (long) value.moduleGet());

        else if (sym->type == gIpeTypeProcedure)
          printf("0x%012lX", (long) value.procedureGet());
      }

      fputc('\n',    stdout);
    }

    else if (ArgSymbol* sym = toArgSymbol(mSymbols[i]))
    {
      const char* typeName = "??????";

      if (mSymbols[i]->type != 0)
        typeName = mSymbols[i]->type->symbol->name;

      if      (sym->hasFlag(FLAG_CONST) == true)
        printf("%s   %3d: const (%s)", pad, i, typeName);

      else if (sym->hasFlag(FLAG_PARAM) == true)
        printf("%s   %3d: param (%s)", pad, i, typeName);

      else
        printf("%s   %3d: arg   (%s)", pad, i, typeName);

      if (strlen(typeName) < 10)
      {
        int spaces = 10 - strlen(typeName);

        for (int j = 0; j <= spaces; j++)
          fputc(' ', stdout);
      }

      fputs("   ",   stdout);

      printf("%4d %4d ", sym->depth(), sym->offset());

      fputs(symName, stdout);
      fputc('\n',    stdout);
    }

    else
    {
      AstDumpToNode logger(stdout, 3);

      printf("Unsupported symbol\n");
      printf("   ");
      mSymbols[i]->accept(&logger);
      printf("\n\n");
    }
  }

  printf("%s>\n", pad);


  if (recursiveP == true && mParent != NULL)
  {
    printf("\n\n");
    mParent->describe(offset, recursiveP);
  }
}
