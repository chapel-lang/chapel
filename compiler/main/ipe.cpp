/*
 * Copyright 2004-2014 Cray Inc.
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

#include "checks.h"
#include "log.h"
#include "passes.h"

#include "AstDumpToNode.h"

struct PassInfo
{
  void        (*function)();
  const char* name;
  char        logTag;
};

// passlist: contains passes in the order that they are called
static PassInfo sPassList[] =
{
  { parse,          "parse",            'p' },
  { scopeResolve,   "scopeResolve",     'S' },
  { resolve,        "resolve",          'R' },
  { resolveIntents, "resolveIntents",   'i' },
  { checkResolved,  "checkResolved",    NUL }
};

void runIPE()
{
  AstDumpToNode logger(stdout);

  size_t passListSize = sizeof(sPassList) / sizeof(sPassList[0]);

  printf("runIPE    00100\n");

  if (rootModule) {
    rootModule->accept(&logger);
    printf("\n\n\n\n\n\n\n\n\n");
  }

  setupLogfiles();

  for (size_t i = 0; i < passListSize; i++)
  {
    printf("RunIPE %s\n", sPassList[i].name);

    sPassList[i].function();

    log_writeLog(sPassList[i].name, i + 1, sPassList[i].logTag);

    cleanAst();

    USR_STOP(); // quit if fatal errors were encountered in pass
  }

  destroyAst();
  teardownLogfiles();
}
