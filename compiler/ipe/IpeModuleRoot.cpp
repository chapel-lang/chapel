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
#include "IpeModuleInternal.h"
#include "IpeModuleStandard.h"
#include "IpeReaderFile.h"
#include "IpeScope.h"
#include "IpeScopeModule.h"
#include "IpeValue.h"

#include "ipe.h"
#include "ipeDriver.h"
#include "ipeEvaluate.h"

#include "AstDumpToNode.h"
#include "files.h"
#include "parser.h"
#include "stmt.h"
#include "stringutil.h"

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

IpeModule* IpeModuleRoot::create(ModuleSymbol* sym)
{
  IpeModule* retval = NULL;

  switch (sym->modTag)
  {
    case MOD_INTERNAL:
      retval = new IpeModuleInternal(sRootModule, sym);
      break;

    case MOD_STANDARD:
      retval = new IpeModuleStandard(sRootModule, sym);
      break;

    case MOD_USER:
      retval = NULL;
      INT_ASSERT(false);
      break;
  }

  return retval;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

IpeModuleRoot::IpeModuleRoot(ModuleSymbol* modSym) : IpeModule(NULL, modSym)
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
  mState = kResolved;

  initialize();

  for (int i = 1; i <= sHighWater; i++)
  {
    DefExpr* expr = toDefExpr(rootModule->block->body.get(i));

    INT_ASSERT(expr);

    if      (mEnv->findLocal(expr->sym->name) != NULL)
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

bool IpeModuleRoot::loadSystemModules()
{
  bool retval = false;

  if      (loadFile(MOD_INTERNAL, "ChapelBase")     == false)
    retval = false;

  else if (loadFile(MOD_INTERNAL, "ChapelStandard") == false)
    retval = false;

  else if (loadFile(MOD_STANDARD, "ChapelRepl")     == false)
    retval = false;

  else
    retval =  true;

  return retval;
}

#include "AstDumpToNode.h"

bool IpeModuleRoot::loadFile(ModTag moduleType, const char* fileName)
{
  bool retval = false;

  currentModuleType = moduleType;

  if (const char* pathName = pathNameForFile(moduleType, fileName))
  {
    std::vector<DefExpr*> defs = IpeReaderFile::readModules(pathName, moduleType);

    for (size_t i = 0; i < defs.size(); i++)
      evaluate(defs[i], mEnv);

    retval = true;
  }
  else
    printf("IpeModuleRoot::loadFile   failed to find path for %s\n", fileName);

  return retval;
}

const char* IpeModuleRoot::pathNameForFile(ModTag moduleType, const char* fileName)
{
  const char* retval = NULL;

  switch (moduleType)
  {
    case MOD_INTERNAL:
      retval = pathNameForInternalFile(fileName);
      break;

    case MOD_STANDARD:
      retval = pathNameForStandardFile(fileName);
      break;

    case MOD_USER:
      retval = NULL;
      INT_ASSERT(false);
      break;
  }

  return retval;
}

void IpeModuleRoot::describeAllModules(int offset) const
{
  char      pad[32]    = { '\0' };

  IpeScope* scope      = mEnv->scopeGet();
  int       numModules = 0;

  if (offset < 32)
  {
    char* tptr = pad;

    for (int i = 0; i < offset; i++)
      *tptr++ = ' ';

    *tptr = '\0';
  }

  for (int i = 0; i < scope->varCount(); i++)
  {
    if (scope->varGet(i)->type == gIpeTypeModule)
      numModules = numModules + 1;
  }

  printf("%sThere are %3d modules.\n", pad, numModules);

  for (int i = 0, count = 0; i < scope->varCount(); i++)
  {
    LcnSymbol* var = scope->varGet(i);

    if (var->type == gIpeTypeModule)
    {
      IpeModule* module = (IpeModule*) mEnv->fetchPtr(var);

      if (count > 0)
        printf("\n");

      module->describe(offset + 3);

      count = count + 1;
    }
  }
}
