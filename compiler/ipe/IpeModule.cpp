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

#include "IpeModule.h"

#include "AstDumpToNode.h"

#include "expr.h"
#include "IpeEnv.h"
#include "IpeMethod.h"
#include "IpeModuleRoot.h"
#include "IpeProcedure.h"
#include "IpeScopeModule.h"
#include "IpeValue.h"
#include "ipeDriver.h"

#include "ipeResolve.h"
#include "ipeLocation.h"
#include "ipeEvaluate.h"

#include "stmt.h"
#include "stringutil.h"

#include <vector>

static std::vector<IpeModule*> sModules;

IpeModule::IpeModule(ModuleSymbol* sym)
{
  mModuleDecl = sym;
  mEnv        = NULL;

  mState      = kLoaded;

  sModules.push_back(this);
}

IpeModule::~IpeModule()
{
  if (mEnv   != NULL)
    delete mEnv;
}

const char* IpeModule::name() const
{
  return mModuleDecl->name;
}

IpeEnv* IpeModule::environment() const
{
  return mEnv;
}

const char* IpeModule::stateAsString() const
{
  const char* retval = NULL;

  switch (mState)
  {
    case kLoaded:
      retval = "Loaded";
      break;

    case kResolving:
      retval = "Resolving";
      break;

    case kResolved:
      retval = "Resolved";
      break;

    case kInitializing:
      retval = "Initializing";
      break;

    case kInitialized:
      retval = "Initialized";
      break;
  }

  return retval;
}

IpeModule* IpeModule::moduleByName(const char* name) const
{
  const char* identifier = astr(name);
  LcnSymbol*  varSym     = mEnv->findVariable(identifier);

  INT_ASSERT(varSym);
  INT_ASSERT(varSym->type == gIpeTypeModule);

  return (IpeModule*) mEnv->fetchPtr(varSym);
}

void IpeModule::moduleAdd(IpeModule* module)
{
  if (mEnv->findVariable(module->name()) != NULL)
  {
    printf("IpeModule::moduleAdd  Unable to add module %s to module %s; already defined\n",
           module->name(),
           name());
  }
  else
  {
    int        offset = mEnv->allocateValue(module);
    VarSymbol* var    = new VarSymbol(module->name(), gIpeTypeModule);

    var->addFlag(FLAG_CONST);
    var->locationSet(0, offset);

    mEnv->varAdd(var);
  }
}

void IpeModule::ensureInitialized()
{
  if (mState != kInitialized)
  {
    INT_ASSERT(false);
  }
}

bool IpeModule::isUseStmt(Expr* expr) const
{
  bool retval = false;

  if (CallExpr* callExpr = toCallExpr(expr))
    retval = callExpr->isPrimitive(PRIM_USE);

  return retval;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

void IpeModule::moduleResolve(IpeModuleRoot* rootModule)
{
  if (mState == kLoaded)
  {
    IpeScopeModule* scope = new IpeScopeModule(this);

    mState = kResolving;
    mEnv   = new IpeEnv(scope);

    for_alist(expr, mModuleDecl->block->body)
    {
      if (DefExpr* defExpr = toDefExpr(expr))
      {
        const char* identifier = defExpr->sym->name;

        // Add a new variable to the local scope if necessary
        // We do not worry about name re-use at this stage
        if (mEnv->findVariable(identifier, true) == NULL)
          mEnv->varAdd(new VarSymbol(identifier));
      }
    }

    for_alist(expr, mModuleDecl->block->body)
    {
      if (isUseStmt(expr) == true)
      {
        CallExpr*  call   = toCallExpr(expr);
        Expr*      name   = call->get(1);
        LcnSymbol* modSym = NULL;

        if (UnresolvedSymExpr* sel = toUnresolvedSymExpr(name))
        {
          if ((modSym = mEnv->findVariable(sel)) == NULL)
            modSym = rootModule->mEnv->findVariable(sel);
          else
            INT_ASSERT(false);
        }

        else
        {
          INT_ASSERT(false);
        }

        INT_ASSERT(modSym);
        INT_ASSERT(modSym->type == gIpeTypeModule);

        if (modSym->offset() >= 0)
        {
          IpeModule* module = (IpeModule*) mEnv->fetchPtr(modSym);

          module->moduleResolve(rootModule);
          mEnv->useAdd(module);
        }
      }
    }

    for_alist(expr, mModuleDecl->block->body)
    {
      if (DefExpr* defExpr = toDefExpr(expr))
      {
        Symbol*    sym = defExpr->sym;
        LcnSymbol* var = mEnv->findVariable(sym->name, true);

        INT_ASSERT(var);

        if (FnSymbol* fnSymbol = toFnSymbol(sym))
        {
          // If there is no local definition, create an IpeProcedure
          if (var->type == dtUnknown)
          {
            // No support to override a procedure
            INT_ASSERT(mEnv->findVariable(sym->name, false) == var);

            IpeProcedure* procedure = new IpeProcedure(sym->name);
            int           offset    = mEnv->allocateValue(procedure);

            var->type = gIpeTypeProcedure;
            var->locationSet(0, offset);
          }

          if (var->type == gIpeTypeProcedure)
          {
            IpeProcedure* procedure = (IpeProcedure*) mEnv->fetchPtr(var);
            IpeMethod*    method    = new IpeMethod(fnSymbol, mEnv);

            procedure->methodAdd(method);
          }
          else
          {
            printf("Error: The variable %s is already defined as a non-procedure in module %s\n",
                   sym->name,
                   name());
          }
        }

        else
        {
          if (var->type == NULL)
          {

          }
          else
          {
            printf("Error: The variable %s is already defined in module %s\n", sym->name, name());
          }
        }
      }
    }

    mState = kResolved;
  }
}

void IpeModule::initialize()
{
  if       (mState == kLoaded      || mState == kResolving)
    INT_ASSERT(false);

  else if (mState == kInitializing || mState == kInitialized)
    ;

  else
  {
    // This is a gross simplification of the required algorithm
    mState = kInitializing;

    mEnv->initializeUsedModules();

    mState = kInitialized;
  }
}

IpeValue IpeModule::evaluate(Expr* exprSrc) const
{
  static int count = 0;

  int        countStart = count;
  bool       debug      = (countStart >= 1000000) ? true : false;

  AstDumpToNode logger(stdout, 3);

  count = count + 1;

  if (debug)
  {
    printf("\n\n\nIpeModule::evaluate() %5d\n", countStart);

    printf("   exprSrc\n");
    printf("   ");
    exprSrc->accept(&logger);
    printf("\n\n\n");
  }

  Expr*    exprRes = resolveExpr (exprSrc,   mEnv);

  if (debug)
  {
    printf("   exprRes\n");
    printf("   ");
    exprRes->accept(&logger);
    printf("\n\n\n");

    mEnv->describe(3);
    printf("\n\n\n");
  }

  locationExpr(exprRes,   mEnv);

  if (debug)
  {
    printf("   exprLoc\n");
    printf("   ");
    exprRes->accept(&logger);
    printf("\n\n\n");

    mEnv->describe(3);
    printf("\n\n\n");
  }

  IpeValue retval  = evaluateExpr(exprRes, mEnv);

  if (debug)
  {
    printf("   evaluated\n");
    mEnv->describe(3);
    printf("\n\n\n");


    printf("\n\n\n");
    printf("\n\n\n");
    printf("\n\n\n");
    printf("\n\n\n");
    printf("\n\n\n");
  }

  return retval;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

void IpeModule::describeAllModules(int offset)
{
  char pad[32] = { '\0' };

  if (offset < 32)
  {
    char* tptr = pad;

    for (int i = 0; i < offset; i++)
      *tptr++ = ' ';

    *tptr = '\0';
  }

  printf("%sThere are %3lu modules. Global pool %8lu / %8lu\n",
         pad,
         sModules.size(),
         IpeEnv::gMemoryTail,
         IpeEnv::gMemorySize);

  for (size_t i = 0; i < sModules.size(); i++)
  {
    if (i > 0)
      printf("\n");

    sModules[i]->describe(offset + 3);
  }
}

void IpeModule::describe(int offset) const
{
  char pad[32] = { '\0' };

  if (offset < 32)
  {
    char* tptr = pad;

    for (int i = 0; i < offset; i++)
      *tptr++ = ' ';

    *tptr = '\0';
  }

  printf("%s#<IpeModule %s\n", pad, name());
  printf("%s  State:   %s\n", pad, stateAsString());
  printf("%s  Type:    %s\n", pad, moduleTypeAsString());

  if (mEnv)
  {
    printf("\n");
    mEnv->describe(offset + 2);
  }

  printf("%s>\n", pad);
}

