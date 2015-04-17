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

#include "AstDumpToNode.h"
#include "expr.h"
#include "files.h"
#include "IpeEnv.h"
#include "IpeModule.h"
#include "IpeModuleRoot.h"
#include "IpeModuleInternal.h"
#include "IpeModuleStandard.h"
#include "IpeReaderFile.h"
#include "IpeReaderTerminal.h"
#include "IpeValue.h"
#include "type.h"

PrimitiveType*        gIpeTypeType        = NULL;
PrimitiveType*        gIpeTypeModule      = NULL;
PrimitiveType*        gIpeTypeProcedure   = NULL;
PrimitiveType*        gIpeTypeMethod      = NULL;

int                   gDebugLevelResolve  =    0;
int                   gDebugLevelEvaluate =    0;
int                   gDebugLevelCalls    =    0;
int                   gDebugLevelVars     =    0;

static PrimitiveType* createType(const char* name);
static void           readEvalPrintLoop(IpeModule* module);

void ipeRun()
{
  astlocMarker   markAstLoc(0, "<repl>");
  IpeModuleRoot* rootModule = NULL;
  IpeModule*     replModule = NULL;

  //
  // Initialize IPE faux-types
  //

  gIpeTypeType      = createType("type");

  gIpeTypeModule    = createType("module");

  gIpeTypeProcedure = createType("procedure");
  gIpeTypeMethod    = createType("method");



  //
  // Initialize the Modules
  //

  if (IpeEnv::globalsAllocate(16 * 1024) == false)
  {
    printf("Failed to allocate the Root Environment\n");
    INT_ASSERT(false);
  }

  if ((rootModule = IpeModuleRoot::allocate()) ==  NULL)
  {
    printf("Failed to allocate the Root Module\n");
    INT_ASSERT(false);
  }

  if (IpeModuleInternal::loadAndInitialize(rootModule)  == false)
  {
    printf("Failed to load the internal modules\n");
    INT_ASSERT(false);
  }

  if (IpeModuleStandard::loadAndInitialize(rootModule)  == false)
  {
    printf("Failed to load the standard modules\n");
    INT_ASSERT(false);
  }

#if 0
  printf("\n\n\n\n");
  printf("After module loading\n");
  IpeModule::describeAllModules(0);
  printf("\n\n\n\n");
#endif

  replModule = rootModule->moduleByName("ChapelRepl");
  replModule->moduleResolve(rootModule);
  replModule->initialize();

#if 0
  printf("\n\n\n\n");
  printf("After REPL initialize\n");
  IpeModule::describeAllModules(0);
  printf("\n\n\n\n");
#endif

  readEvalPrintLoop(replModule);

#if 0
  if (replModule != NULL)
    delete replModule;

  if (rootModule != NULL)
    delete rootModule;

  IpeModule::modulesDeallocate();

  // NOAKES 2015/02/23 These will seg-fault for some reason
  cleanAst();
  destroyAst();
#endif
}

static PrimitiveType* createType(const char* name)
{
  PrimitiveType* pt = new PrimitiveType(NULL, true);
  TypeSymbol*    ts = new TypeSymbol(name, pt);

  ts->cname = name;

  // This prevents cleanAST() from sweeping these
  ts->addFlag(FLAG_GLOBAL_TYPE_SYMBOL);

  return pt;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static void readEvalPrintLoop(IpeModule* moduleRepl)
{
  astlocMarker  markAstLoc(0, "<repl>");
  AstDumpToNode logger(stdout, 3);

  printf("\n\n\n");
  printf("IPE 0.1\n");
  printf("\n");

  {
    int         filenum       = 0;
    const char* inputFilename = 0;

    int count = 0;

    while ((inputFilename = nthFilename(filenum++)))
    {
      if (isChplSource(inputFilename))
      {
        IpeReaderFile reader;

        if (reader.open(inputFilename) == true)
        {
          while (Expr* expr = reader.readStmt())
          {
            IpeValue value = moduleRepl->evaluate(expr);

            (void) value;

            if (gDebugLevelResolve > 2 || gDebugLevelEvaluate > 2)
              printf("\n\n\n\n\n\n\n\n");

            count = count + 1;
          }

          reader.close();
        }
      }
    }
  }

#if 0
  printf("\n\nAfter command line script\n");
  printf("\n\n");
  IpeModule::describeAllModules(0);
  printf("\n\n\n\n");
#endif

  {
    int               cmdCount = 1;
    IpeReaderTerminal reader;

    printf("%2d > ", cmdCount);

    while (Expr* expr = reader.readStmt())
    {
      IpeValue value   = moduleRepl->evaluate(expr);

      (void) value;

      cmdCount = cmdCount + 1;
      printf("%2d > ", cmdCount);
    }
  }
}
