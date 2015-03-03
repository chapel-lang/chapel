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

#include "IpeVars.h"

#include "AstDumpToNode.h"
#include "misc.h"
#include "symbol.h"

IpeValue*               IpeVars::sRootData = NULL;
int                     IpeVars::sRootSize =    0;
int                     IpeVars::sRootTail =    0;

std::vector<LcnSymbol*> IpeVars::sSymbols;

IpeVars* IpeVars::rootAllocate(int size)
{
  INT_ASSERT(sRootData == NULL);

  sRootData = (IpeValue*) malloc(size);
  sRootSize = size;
  sRootTail =    0;

  return NULL;
}

// Allocate a "stack frame"
IpeVars* IpeVars::allocate(int dataSize)
{
  IpeVars* retval = (IpeVars*) malloc(sizeof(IpeVars) - sizeof(IpeValue) + dataSize);

  retval->mLexicalParent = NULL;
  retval->mScope         = NULL;

  return retval;
}

// Deallocate a "stack frame"
void IpeVars::deallocate(IpeVars* vars)
{
  if (vars != 0)
    free(vars);
}

void IpeVars::bind(VarSymbol* sym, IpeValue value, IpeVars* env)
{
  INT_ASSERT(sRootData != 0);

  if (env == NULL)
  {
    ensureSpaceToExtend(sizeof(value));

    memcpy(((char*) sRootData) + sRootTail, &value, sizeof(value));

    sym->locationSet(0, sRootTail);

    sSymbols.push_back(sym);
    sRootTail = sRootTail + sizeof(value);
  }
  else
  {
    AstDumpToNode logger(stdout, 3);

    printf("   IpeVars::bind(VarSymbol*, IpeValue, IpeVars*) unsupported\n");
    printf("   ");
    sym->accept(&logger);
    printf("\n");

    INT_ASSERT(false);
  }
}

void IpeVars::ensureSpaceToExtend(int size)
{
  int attempts = 0;

  INT_ASSERT(sRootSize > 0);

  while (attempts < 4 && sRootTail + size > sRootSize)
  {
    int newSize = 2 * sRootSize;

    sRootData = (IpeValue*) realloc(sRootData, newSize);
    sRootSize = newSize;

    attempts = attempts + 1;
  }

  INT_ASSERT(sRootTail + size <= sRootSize);
}

IpeValue IpeVars::addrOf(LcnSymbol* sym, IpeVars* vars)
{
  int      offset = sym->offset();
  IpeValue retval;

  INT_ASSERT(sRootData    != 0);
  INT_ASSERT(offset       >= 0);
  INT_ASSERT((offset % 8) == 0);

  if (sym->depth() == 0)
  {
    retval.valuePtr = (IpeValue*) (((char*) sRootData) + offset);
  }

  else
  {
    int index = offset / sizeof(IpeValue);

    INT_ASSERT(vars         != NULL);
    INT_ASSERT(sym->depth() ==    1);

    retval.valuePtr = &vars->mValues[index];
  }

  return retval;
}

IpeValue IpeVars::fetch(LcnSymbol* sym, IpeVars* vars)
{
  int      depth  = sym->depth();
  int      offset = sym->offset();
  IpeValue retval;

  INT_ASSERT(sRootData    != 0);
  INT_ASSERT(offset       >= 0);
  INT_ASSERT((offset % 8) == 0);

  if (depth == 0)
  {
    memcpy(&retval, ((char*) sRootData) + offset, sizeof(retval));
  }
  else
  {
    int index = offset / sizeof(IpeValue);

    INT_ASSERT(vars  != NULL);
    INT_ASSERT(depth ==    1);

    retval = vars->mValues[index];
  }

  return retval;
}

void IpeVars::store(LcnSymbol* sym, IpeValue value, IpeVars* vars)
{
  int offset = sym->offset();

  INT_ASSERT(sRootData    != 0);
  INT_ASSERT(offset       >= 0);
  INT_ASSERT((offset % 8) == 0);

  if (sym->depth() == 0)
  {
    memcpy(((char*) sRootData) + offset, &value, sizeof(value));
  }

  else
  {
    int index = offset / sizeof(IpeValue);

    INT_ASSERT(vars         != NULL);
    INT_ASSERT(sym->depth() ==    1);

    vars->mValues[index] = value;
  }
}

IpeValue IpeVars::valueGet(int index) const
{
  return mValues[index];
}

void IpeVars::describe(IpeVars* env, int offset)
{
  char pad[32] = { '\0' };

  if (offset < 32)
  {
    char* tptr = pad;

    for (int i = 0; i < offset; i++)
      *tptr++ = ' ';

    *tptr = '\0';
  }

  if (env == 0)
  {
    int size = (int) sSymbols.size();

    printf("%s#<IpeVars   Root %5d symbols. %5d / %5d\n",
           pad,
           (int) sSymbols.size(),
           sRootTail,
           sRootSize);

    for (int i = 0; i < size; i++)
    {
      const char* symName = sSymbols[i]->name;

      if      (isTypeSymbol(sSymbols[i]))
      {
        printf("%s   %3d: type %16s %s\n", pad, i, "", symName);
      }

      else if (VarSymbol* sym = toVarSymbol(sSymbols[i]))
      {
        const char* typeName = "??????";

        if (sSymbols[i]->type != 0)
          typeName = sSymbols[i]->type->symbol->name;

        if (sym->immediate != 0)
          printf("%s   %3d: imm   (%s)", pad, i, typeName);

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

        fputs("   ",   stdout);

        if (sym->immediate == 0)
        {
          printf("%4d ", sym->offset());

          fputs("     ", stdout);
          printf("%-24s ", symName);

          if (sSymbols[i]->type == dtInt[INT_SIZE_64])
          {
            int* ptr = (int*) (((char*) sRootData) + sym->offset());

            printf("%6d", *ptr);
          }

          else if (sSymbols[i]->type == dtReal[FLOAT_SIZE_64])
          {
            double* ptr = (double*) (((char*) sRootData) + sym->offset());

            printf("%9.2f", *ptr);
          }

          else if (sSymbols[i]->type == dtBools[BOOL_SIZE_SYS])
          {
            int* ptr = (int*) (((char*) sRootData) + sym->offset());

            printf(" %s", (*ptr == 1) ? " true" : "false");
          }

          fputc('\n',    stdout);
        }
        else
        {
          fputs(pad, stdout);
          fputs("       ", stdout);
          fputs(symName, stdout);
          fputc('\n',    stdout);
        }
      }

      else if (isFnSymbol(sSymbols[i]) == true)
      {
        printf("%s   %3d: func %16s %s\n", pad, i, "", symName);
      }

      else
      {
        AstDumpToNode logger(stdout, 3);

        printf("Unsupported symbol\n");
        printf("   ");
        sSymbols[i]->accept(&logger);
        printf("\n\n");
      }
    }

    printf("%s>\n", pad);
  }
  else
  {
    printf("%s#<IpeVars   Locals\n", pad);
    printf("%s>\n", pad);
  }
}
