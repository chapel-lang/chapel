#include "ipe.h"

#include "ipeResolve.h"
#include "ipeInlinePrimitives.h"
#include "ipeInsertVariables.h"
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
  { "parse",            parse               },
  { "resolve",          ipeResolve          },
  { "inlinePrimitives", ipeInlinePrimitives },
  { "insertVariables",  ipeInsertVariables  },
  { "evaluate",         ipeEvaluate         }
};

void ipeRun()
{
  size_t passListSize = sizeof(sPassList) / sizeof(sPassList[0]);

  setupLogfiles();

  // Remaining passes run only on application module
  for (size_t i = 0; i < passListSize; i++)
  {
    sPassList[i].function();

    AstDumpToNode::view(sPassList[i].name, i + 1);
  }

  cleanAst();
  destroyAst();
  teardownLogfiles();
}
