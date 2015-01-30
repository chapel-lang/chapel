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

#include "ipeEvaluate.h"

#include "AstDumpToNode.h"
#include "stmt.h"

#include <cstdio>
#include <cstdlib>

static int  rootEnvSize();
static void rootEnvInit(void* env);
static void rootEnvDescribe(void* env);

static int  sizeForType(Type* type);

void ipeEvaluate()
{
  int   rootSize = rootEnvSize();
  void* rootEnv  = malloc(rootSize);

  rootEnvInit(rootEnv);
  rootEnvDescribe(rootEnv);

  free(rootEnv);
}

static int rootEnvSize()
{
  int retval = 0;

  for_alist(rootStmt, rootModule->block->body)
  {
    if (DefExpr* rootDefExpr = toDefExpr(rootStmt))
    {
      if (ModuleSymbol* module = toModuleSymbol(rootDefExpr->sym))
      {
        if (module != rootModule)
        {
          for_alist(stmt, module->block->body)
          {
            if (DefExpr* defExpr = toDefExpr(stmt))
            {
              if (Type* type = defExpr->sym->type)
                retval = retval + sizeForType(type);

              else if (isFnSymbol(defExpr->sym) == true)
                retval = retval + 0;

              else
              {
                AstDumpToNode logger(stdout);

                printf("rootEnvSize\n");
                printf("Failed to determine size for\n");

                defExpr->accept(&logger);
                printf("\n\n\n");
              }
            }
          }
        }
      }
    }
  }

  return retval;
}

static void rootEnvInit(void* env)
{
  for_alist(rootStmt, rootModule->block->body)
  {
    if (DefExpr* rootDefExpr = toDefExpr(rootStmt))
    {
      if (ModuleSymbol* module = toModuleSymbol(rootDefExpr->sym))
      {
        if (module != rootModule)
        {
          for_alist(stmt, module->block->body)
          {
            if (DefExpr* defExpr = toDefExpr(stmt))
            {
              VarSymbol* varSym  = 0;

              if (defExpr->init == 0)
              {
                if (Type* type = defExpr->sym->type)
                  varSym = toVarSymbol(type->defaultValue);
              }

              else
              {
                if (SymExpr* symExpr = toSymExpr(defExpr->init))
                  varSym = toVarSymbol(symExpr->var);
              }

              if (varSym != 0)
              {
                bool handled = false;

                if (Immediate* imm = varSym->immediate)
                {
                  Type* type = varSym->type;
                  int   size = sizeForType(type);

                  if (strcmp(type->symbol->name, "bool") == 0)
                  {
                    *((long*) env)   = imm->v_bool;
                    handled          = true;
                  }

                  else if (strcmp(type->symbol->name, "int") == 0)
                  {
                    *((long*) env)   = imm->v_int64;
                    handled          = true;
                  }

                  else if (strcmp(type->symbol->name, "real") == 0)
                  {
                    *((double*) env) = imm->v_float64;
                    handled          = true;
                  }

                  env = ((char*) env) + size;
                }

                if (handled == false)
                  INT_ASSERT(false);
              }

              else if (isFnSymbol(defExpr->sym) == true)
              {

              }

              else
              {
                AstDumpToNode logger(stdout);

                printf("rootEnvInit\n\n");
                printf("Failed to handle\n");

                defExpr->accept(&logger);
                printf("\n\n\n");

                INT_ASSERT(false);
              }
            }
          }
        }
      }
    }
  }
}

static void rootEnvDescribe(void* env)
{
  printf("Final environment\n");

  for_alist(rootStmt, rootModule->block->body)
  {
    if (DefExpr* rootDefExpr = toDefExpr(rootStmt))
    {
      if (ModuleSymbol* module = toModuleSymbol(rootDefExpr->sym))
      {
        if (module != rootModule)
        {
          bool first = true;

          for_alist(stmt, module->block->body)
          {
            if (DefExpr* defExpr = toDefExpr(stmt))
            {
              if (Type* type = defExpr->sym->type)
              {
                int size = sizeForType(type);

                if (first == true)
                {
                  printf("   Module %s\n", module->name);
                  first = false;
                }

                printf("      %-20s ", defExpr->sym->name);

                if (strcmp(type->symbol->name, "bool") == 0)
                {
                  if (*((long*) env) == 0)
                    printf("       false\n");
                  else
                    printf("        true\n");
                }

                else if (strcmp(type->symbol->name, "int") == 0)
                {
                  printf("%12lu\n", *((long*) env));
                }

                else if (strcmp(type->symbol->name, "real") == 0)
                {
                  printf("     %12.4f\n", *((double*) env));
                }

                env = ((char*) env) + size;
              }
            }
          }
        }
      }
    }
  }
}

static int sizeForType(Type* type)
{
  int retval = 0;

  if (false)
    retval = 0;

  else if (strcmp(type->symbol->name, "bool") == 0)
    retval = 8;

  else if (strcmp(type->symbol->name, "int")  == 0)
    retval = 8;

  else if (strcmp(type->symbol->name, "real") == 0)
    retval = 8;

  else
  {
    AstDumpToNode logger(stdout);

    printf("sizeForType unhandled\n");
    type->accept(&logger);
    printf("\n\n\n\n\n");
    INT_ASSERT(false);
  }

  return retval;
}
