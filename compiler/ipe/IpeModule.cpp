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

#include "IpeBlockStmt.h"
#include "IpeEnv.h"
#include "IpeMethod.h"
#include "IpeProcedure.h"
#include "IpeScopeModule.h"
#include "IpeValue.h"

#include "ipeDriver.h"
#include "ipeResolve.h"
#include "ipeEvaluate.h"
#include "ipeUtils.h"

#include "AstDumpToNode.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"

IpeModule::IpeModule(IpeModule*    parent,
                     ModuleSymbol* modSym)
{
  IpeScopeModule* scope     = new IpeScopeModule(parent, this);
  IpeEnv*         parentEnv = (parent != NULL) ? parent->environment() : NULL;

  mState        = kLoaded;

  mEnv          = new IpeEnv(parentEnv, scope);

  mModSym       = modSym;
  mBodyResolved = NULL;
}

IpeModule::~IpeModule()
{
  if (mEnv          != NULL)
    delete mEnv;

  if (mBodyResolved != NULL)
    delete mBodyResolved;
}

const char* IpeModule::name() const
{
  return mModSym->name;
}

IpeScopeModule* IpeModule::scopeGet() const
{
  IpeScopeModule* retval = NULL;

  if (mEnv != NULL)
    retval = (IpeScopeModule*) mEnv->scopeGet();

  return retval;
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

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

void IpeModule::moduleEvaluate()
{
  moduleResolve();
  initialize();
}

void IpeModule::moduleResolve()
{
  if (mState == kLoaded)
  {
    BlockStmt*   untypedExpr = mModSym->block;
    Expr*        typedExpr   = NULL;
    IpeSequence* seq         = NULL;

    mState        = kResolving;

    typedExpr     = blockResolve(untypedExpr, mEnv);
    seq           = (IpeSequence*) typedExpr;

    INT_ASSERT(isBlockStmt(typedExpr));
    INT_ASSERT(seq->isScopeless() == true);

    mBodyResolved = seq;

    mState        = kResolved;
  }
}

// This is a placeholder
void IpeModule::initialize()
{
  if      (mState == kLoaded       || mState == kResolving)
    INT_ASSERT(false);

  else if (mState == kInitializing || mState == kInitialized)
    ;

  else
  {
    mState = kInitializing;

    mEnv->initializeUsedModules();

    mState = kInitialized;
  }
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

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
  printf("%s  State: %s\n", pad, stateAsString());
  printf("%s  Type:  %s\n", pad, moduleTypeAsString());

  if (mEnv)
  {
    printf("\n");
    mEnv->describe(offset + 2);
  }

  printf("%s>\n", pad);
}

