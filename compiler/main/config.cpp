#include "chpl.h"
#include "expr.h"
#include "stmt.h"
#include "config.h"
#include "../parser/lexyacc.h"

static Map<const char *, Expr*> configMap;
static Vec<const char*> usedConfigParams;

bool mainHasArgs;

void
checkConfigs(void) {
  bool anyBadConfigParams = false;
  Vec<const char*> configParamSetNames;
  // configParamMap.get_keys(configParamSetNames);
  configMap.get_keys(configParamSetNames);
  forv_Vec(const char, name, configParamSetNames) {
    if (!usedConfigParams.in(name)) {
      USR_FATAL_CONT("Trying to set unrecognized config param '%s' via -s flag", name);
      anyBadConfigParams = true;
    }
  }
  if (anyBadConfigParams) {
    USR_STOP();
  }
}

void
parseCmdLineConfig(const char *name, const char *value) {
  const char *fake, *parseFn, *parseMsg;
  if (strcmp(value, "") != 0) {
    fake = astr("dummyConfig=", value, ";");
  } else {
    fake = astr("dummyConfig=true;");
  }
  parseFn = astr("Command-line arg (", name, ")");
  parseMsg = astr("parsing '", value, "'");
  BlockStmt *b = toBlockStmt(parseString(fake, parseFn, parseMsg)->body.head);
  Expr *newExpr=NULL;
  if (CallExpr *c = toCallExpr(b->body.head)) {
    newExpr = c->get(2)->copy();
  } else if (DefExpr *d = toDefExpr(b->body.head)) {
    newExpr = d->init;
  } else {
    INT_ASSERT(false);
  }
  configMap.put(astr(name), newExpr);
  INT_ASSERT(newExpr == configMap.get(astr(name)));
  newExpr->parentSymbol = NULL;
  newExpr->parentExpr = NULL;
  // Remove the call expression from the block (empty blocks removed later)
  b->body.head = b->body.tail = NULL;
  b->body.length = 0;
}

Expr *
getCmdLineConfig(const char *name) {
  return configMap.get(astr(name));
}

void
useCmdLineConfig(const char *name) {
  usedConfigParams.add(name);
}

bool
isUsedCmdLineConfig(const char *name) {
  return usedConfigParams.in(name);
}


