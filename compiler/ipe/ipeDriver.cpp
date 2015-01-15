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

#include "ipe.h"

#include "ipeResolve.h"
#include "ipeInlinePrimitives.h"
#include "ipeEvaluate.h"

#include "AstDumpToNode.h"
#include "log.h"
#include "passes.h"
#include "stmt.h"

struct PassInfo
{
  const char* name;
  void        (*function)();
};

static PassInfo sPassList[] =
{
  { "inlinePrimitives", ipeInlinePrimitives },
  { "evaluate",         ipeEvaluate         }
};

static ModuleSymbol* findApplicationModule();

void ipeRun()
{
  size_t        passListSize = sizeof(sPassList) / sizeof(sPassList[0]);
  ModuleSymbol* applModule   = 0;

  setupLogfiles();

  // Run the parser
  parse();
  AstDumpToNode::view("parse", 1);

  // Select the application module
  applModule = findApplicationModule();

  // Resolution is applied to base module and application module
  ipeResolve();
  AstDumpToNode::view("resolve", 2, baseModule);
  AstDumpToNode::view("resolve", 2, applModule);

  // Remaining passes run only on application module
  for (size_t i = 0; i < passListSize; i++)
  {
    sPassList[i].function();

    AstDumpToNode::view(sPassList[i].name, i + 3, applModule);
  }

  cleanAst();
  destroyAst();
  teardownLogfiles();
}

// Scan the Root Module Declaration
//   Every expression should be a DefExpr.
//   Find the single Application Module
static ModuleSymbol* findApplicationModule()
{
  int           count  = 0;
  ModuleSymbol* retval = 0;

  for_alist(stmt, rootModule->block->body)
  {
    if (DefExpr* defExpr = toDefExpr(stmt))
    {
      if (ModuleSymbol* module = toModuleSymbol(defExpr->sym))
      {
        if (module != baseModule)
        {
          retval = module;
          count  = count + 1;
        }
      }
    }
    else
      INT_ASSERT(false);
  }

  // There should be just one application module
  INT_ASSERT(count == 1);

  return retval;
}
