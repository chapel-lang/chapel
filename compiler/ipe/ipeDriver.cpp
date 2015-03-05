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
#include "astutil.h"
#include "bison-chapel.h"
#include "expr.h"
#include "files.h"
#include "flex-chapel.h"
#include "IpeModule.h"
#include "IpeScopeModule.h"
#include "IpeVars.h"
#include "ipeResolve.h"
#include "ipeEvaluate.h"
#include "log.h"
#include "parser.h"
#include "passes.h"
#include "stmt.h"
#include "stringutil.h"
#include "type.h"
#include "VisibleSymbols.h"

PrimitiveType* gIpeTypeModule      = NULL;
PrimitiveType* gIpeTypeProcedure   = NULL;

IpeScope*      gRootScope          = NULL;
IpeVars*       gRootVars           = NULL;

int            gDebugLevelResolve  =    0;
int            gDebugLevelEvaluate =    0;
int            gDebugLevelCalls    =    0;
int            gDebugLevelVars     =    0;

class ReadStream
{
public:
                 ReadStream();
                ~ReadStream();

  bool           open(const char* fileName);
  Expr*          read();

private:
  FILE*          mFP;
  YYLTYPE        mYYlloc;
  yypstate*      mParser;
  ParserContext  mContext;
};

/************************************ | *************************************
*                                                                           *
* IPE needs to extract core definitions from the Root Module but this       *
* module is constantly updated.  We arrange for the core initialization     *
* routine to call this function so that it can establish a high water mark. *
*                                                                           *
************************************* | ************************************/

static int sRootModuleIndex = 0;

void ipeRootInit()
{
  sRootModuleIndex = rootModule->block->body.length;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static PrimitiveType* createType(const char* name);

static IpeModule*     initializeRoot(IpeVars* rootVars);
static void           initializeChapelStandard(IpeVars* rootVars);
static IpeModule*     initializeRepl(IpeVars* rootVars);

static ModuleSymbol*  createRootModule();
static ModuleSymbol*  createReplModule();
static CallExpr*      createUseChapelStandard();

static void           readEvalPrintLoop(IpeScope* scope, IpeVars* rootVars);

void ipeRun()
{
  astlocMarker    markAstLoc(0, "<repl>");

  IpeModule*      rootModule = NULL;

  IpeModule*      replModule = NULL;
  IpeScopeModule* replScope  = NULL;

  //
  // Parse the internal modules
  //
  setupLogfiles();

  parse();
  AstDumpToNode::view("parse", 1);

  teardownLogfiles();

  //
  // Initialize the REPL
  //

  gIpeTypeModule    = createType("module");
  gIpeTypeProcedure = createType("procedure");

  gRootVars         = IpeVars::rootAllocate(1024 * sizeof(IpeValue));

  rootModule        = initializeRoot(gRootVars);
  gRootScope        = rootModule->scope();

#if 0
  printf("\n\n");
  gRootScope->describe(3);
  printf("\n\n");

  IpeVars::describe(gRootVars, 3);
  printf("\n\n");
#endif

  initializeChapelStandard(gRootVars);

  replModule        = initializeRepl(gRootVars);
  replScope         = replModule->scope();

#if 1
  printf("\n\n");
  gRootScope->describe(3);
  printf("\n\n");

  IpeVars::describe(gRootVars, 3);
  printf("\n\n");
#endif

  readEvalPrintLoop(replScope, gRootVars);

  if (replModule != NULL)
    delete replModule;

  if (rootModule != NULL)
    delete rootModule;

#if 0
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

static IpeModule* initializeRoot(IpeVars* rootVars)
{
  ModuleSymbol*   modSym    = createRootModule();
  IpeModule*      rootMod   = new IpeModule(modSym);
  IpeScopeModule* rootScope = rootMod->scope(); // new IpeScopeModule(rootMod);
  VarSymbol*      var       = NULL;

  IpeValue        modValue;

  for (int i = 1; i <= sRootModuleIndex; i++)
  {
    if (DefExpr* defExpr = toDefExpr(rootModule->block->body.get(i)))
    {
      ipeResolve (defExpr, rootScope, rootVars);
      ipeEvaluate(defExpr,            rootVars);
    }
  }

  var = new VarSymbol(modSym->name, gIpeTypeModule);

  var->addFlag(FLAG_PARAM);

  modValue.modulePtr = rootMod;

  rootScope->extend(var, modValue, rootVars);

  return rootMod;
}

static void initializeChapelStandard(IpeVars* rootVars)
{
  CallExpr* use = createUseChapelStandard();

  ipeResolve (use, gRootScope, rootVars);
  ipeEvaluate(use,             rootVars);
}

static IpeModule* initializeRepl(IpeVars* rootVars)
{
  ModuleSymbol*  mod     = createReplModule();
  DefExpr*       def     = new DefExpr(mod);
  VarSymbol*     varSym  = 0;
  VisibleSymbols symbols;

  ipeResolve (def, gRootScope, rootVars);
  ipeEvaluate(def,             rootVars);

  symbols = gRootScope->visibleSymbols(mod->name);

  INT_ASSERT(symbols.count() == 1);

  varSym  = toVarSymbol(symbols.symbol(0));

  INT_ASSERT(varSym);

  return IpeVars::fetch(varSym, rootVars).modulePtr;
}

static ModuleSymbol* createRootModule()
{
  ModuleSymbol* retval = new ModuleSymbol("ChplRoot",
                                          MOD_INTERNAL,
                                          new BlockStmt());

  retval->filename = astr("<ChapelRoot>");

  return retval;
}

static ModuleSymbol* createReplModule()
{
  CallExpr*     use    = createUseChapelStandard();
  ModuleSymbol* retval = new ModuleSymbol("ChapelRepl",
                                          MOD_INTERNAL,
                                          new BlockStmt());

  retval->filename = astr("<ChapelRepl>");

  retval->block->insertAtTail(use);

  return retval;
}

static CallExpr* createUseChapelStandard()
{
  return new CallExpr(PRIM_USE, new UnresolvedSymExpr("ChapelStandard"));
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

static void readEvalPrintLoop(IpeScope* scope, IpeVars* env)
{
  astlocMarker  markAstLoc(0, "<repl>");
  AstDumpToNode logger(stdout, 3);

  printf("\n\n\n");
  printf("IPE 0.1\n");
  printf("\n");

  {
    int         filenum       = 0;
    const char* inputFilename = 0;

    while ((inputFilename = nthFilename(filenum++)))
    {
      if (isChplSource(inputFilename))
      {
        ReadStream stream;

        if (stream.open(inputFilename))
        {
          while (Expr* expr = stream.read())
          {
            ipeResolve(expr, scope, env);
            ipeEvaluate(expr, env);

            if (gDebugLevelResolve > 2 || gDebugLevelEvaluate > 2)
            {
              printf("\n\n\n\n\n\n\n\n");
            }
          }
        }
      }
    }
  }

#if 0
  printf("\n\nAfter command line script\n");
  scope->describe(3);
  printf("\n\n");

  IpeVars::describe(env, 3);
  printf("\n\n");
#endif

  {
    int        cmdCount = 1;
    ReadStream stream;

    printf("%2d > ", cmdCount);

    while (true)
    {
      Expr* expr = stream.read();

#if 0
      printf("\n\n");
      printf("   Expr\n");
      printf("   ");
      expr->accept(&logger);
      printf("\n\n");
#endif

      // Now evaluate the type
      ipeResolve(expr, scope, env);

#if 0
      printf("\n\n");
      printf("   Resolved\n");
      printf("   ");
      expr->accept(&logger);
      printf("\n\n");
#endif

      // Now evaluate the value
      ipeEvaluate(expr, env);

      // Now Print
#if 0
      IpeVars::describe(env, 3);
      printf("\n\n");
#endif

      cmdCount = cmdCount + 1;
      printf("%2d > ", cmdCount);
    }
  }
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

ReadStream::ReadStream()
{
  yylex_init(&(mContext.scanner));

  mFP                  = NULL;

  mParser              = yypstate_new();

  yyblock              = NULL;
  yyfilename           = NULL;

  mYYlloc.first_line   =   1;
  mYYlloc.first_column =   0;

  mYYlloc.last_line    =   1;
  mYYlloc.last_column  =   0;
}


ReadStream::~ReadStream()
{
  yypstate_delete(mParser);
  yylex_destroy(mContext.scanner);

  if (mFP != 0)
    fclose(mFP);
}

bool ReadStream::open(const char* fileName)
{
  if ((mFP = fopen(fileName, "r")) != 0)
    yyset_in(mFP, mContext.scanner);

  return (mFP != NULL) ? true : false;
}

Expr* ReadStream::read()
{
  int   lexerStatus  = 100;
  int   parserStatus = YYPUSH_MORE;
  Expr* retval       = 0;

  while (lexerStatus != 0 && parserStatus == YYPUSH_MORE && retval == 0)
  {
    YYSTYPE yylval;

    mContext.generatedStmt = NULL;

    lexerStatus           = yylex(&yylval, &mYYlloc, mContext.scanner);

#if 0
    if (lexerStatus > 0)
      printf("lexer: %3d %s\n", lexerStatus, yylval.pch);
    else
      printf("lexer: %3d\n",    lexerStatus);
#endif

    if (lexerStatus >= 0)
      parserStatus = yypush_parse(mParser, lexerStatus, &yylval, &mYYlloc, &mContext);

    else if (lexerStatus == YYLEX_NEWLINE)
    {

    }

    if (mContext.generatedStmt != 0)
    {
      if (BlockStmt* bs = toBlockStmt(mContext.generatedStmt))
      {
        if (Expr* expr = bs->body.last())
        {
          retval = expr;
        }
      }
    }
  }

  if (retval != 0)
    insert_help(retval, NULL, NULL);

  return retval;
}
