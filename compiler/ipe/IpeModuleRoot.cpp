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


#include "IpeModuleRoot.h"

#include "IpeEnv.h"
#include "IpeValue.h"
#include "ipe.h"
#include "ipeDriver.h"

#include "stmt.h"
#include "stringutil.h"

#include "AstDumpToNode.h"

IpeModuleRoot* IpeModuleRoot::sRootModule = NULL;
int            IpeModuleRoot::sHighWater  =    0;

void ipeRootInit()
{
  IpeModuleRoot::setHighWaterMark(rootModule->block->body.length);
}

void IpeModuleRoot::setHighWaterMark(int value)
{
  sHighWater = value;
}

IpeModuleRoot* IpeModuleRoot::allocate()
{
  if (sRootModule == 0)
  {
    ModuleSymbol* modSym = createDeclaration();

    sRootModule = new IpeModuleRoot(modSym);

    sRootModule->init();
  }
  else
  {
    printf("Fatal error:: IpeModuleRoot:allocate has already been executed\n");
    INT_ASSERT(false);
  }

  return sRootModule;
}

ModuleSymbol* IpeModuleRoot::createDeclaration()
{
  ModuleSymbol* retval = new ModuleSymbol("ChapelRoot",
                                          MOD_INTERNAL,
                                          new BlockStmt());

  retval->filename = astr("<ChapelRoot>");

  return retval;
}

IpeModuleRoot::IpeModuleRoot(ModuleSymbol* modSym) : IpeModule(modSym)
{

}

IpeModuleRoot::~IpeModuleRoot()
{

}

const char* IpeModuleRoot::moduleTypeAsString() const
{
  return "Root";
}

void IpeModuleRoot::init()
{
  moduleResolve(this);
  initialize();

  for (int i = 1; i <= sHighWater; i++)
  {
    DefExpr* expr = toDefExpr(rootModule->block->body.get(i));

    INT_ASSERT(expr);

    if      (mEnv->isDefinedLocally(expr->sym->name) == true)
    {
      printf("   Attempt to redefine identifier %s\n", expr->sym->name);
      INT_ASSERT(false);
    }

    else if (TypeSymbol* sym = toTypeSymbol(expr->sym))
    {
      int        offset = mEnv->allocateValue(sym->type);
      VarSymbol* var    = new VarSymbol(sym->name, gIpeTypeType);

      INT_ASSERT(expr->exprType == NULL);
      INT_ASSERT(expr->init     == NULL);

      var->addFlag(FLAG_CONST);
      var->locationSet(0, offset);

      mEnv->varAdd(var);
    }

    else if (expr->sym->isImmediate() == true)
    {
      VarSymbol* var    = toVarSymbol(expr->sym);
      int        offset = mEnv->allocateValue(var->type);

      var->addFlag(FLAG_CONST);
      var->locationSet(0, offset);

      mEnv->varAdd(var);
    }

    else
    {
      AstDumpToNode logger(stdout, 3);

      printf("Unexpected expression while initializing RootModule\n");
      printf("   ");
      expr->accept(&logger);
      printf("\n");

      INT_ASSERT(false);
    }
  }

  moduleAdd(this);
}
