/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#include "config.h"

#include "chpl.h"
#include "driver.h"
#include "expr.h"
#include "parser.h"
#include "stmt.h"
#include "stringutil.h"
#include "chpl/parsing/parsing-queries.h"
#include <utility>


using namespace chpl;

static Map<const char*, Expr*> configMap; // map from configs to vals
static Map<const char*, VarSymbol*> usedConfigParams; // map from configs to uses

bool                           mainHasArgs;
bool                           mainPreserveDelimiter;

void checkConfigs() {
  if (fMinimalModules == false) {
    bool             anyBadConfigParams = false;
    if (fDynoCompilerLibrary) {
      // check that all config vars that were set from the command line were assigned
      auto configs = parsing::configSettings(gContext);
      for (auto config : configs) {
        auto usedId = parsing::nameToConfigSettingId(gContext, config.first);
        if (usedId.isEmpty()) {
          USR_FATAL_CONT("Trying to set unrecognized config '%s' via -s flag",
                         config.first.c_str());
          anyBadConfigParams = true;
        }
      }
    } else {
      Vec<const char *> configParamSetNames;

      configMap.get_keys(configParamSetNames);

      forv_Vec(const char, name, configParamSetNames) {
          if (usedConfigParams.get(name) == NULL) {
            USR_FATAL_CONT("Trying to set unrecognized config '%s' via -s flag",
                           name);
            anyBadConfigParams = true;
          }
        }
    }
    if (anyBadConfigParams) {
      USR_STOP();
    }
  }
}

//
// This function is designed to convert a name/value pair of C strings in to
// corresponding AST that is then inserted into the "configMap".
//
// This is currently achieved by synthesizing a chapel statement as a string,
// invoking the parser, and then extracting the desired portion of the result.
// This means that this function has some knowledge of the structure of the
// generated AST
//
// Historically the generated AST consisted of a BlockStmt which then contained
// a scope-less BlockStmt.  Recent changes to the parser are working to remove
// the internal scope-less BlockStmt.

void parseCmdLineConfig(const char* name, const char* value) {

  // unfortunately this is parsed in the order from the command line, so for
  // it to work, --dyno must come before -sConfigVar=Val or it will not try
  // to use the dyno parser for command line input
  if (fDynoCompilerLibrary) {
    // save the name/value pair for dyno parser
    gDynoParams.push_back(std::make_pair(std::string(name), std::string(value)));
  } else {

    // Generate a C-string for a nominal Chapel assignment statement
    const char* stmtText = (value[0] != '\0') ? astr("dummyConfig=", value, ";") : astr("dummyConfig=true;");
    const char* parseFn  = astr("Command-line arg (", name, ")");
    const char* parseMsg = astr("parsing '", value, "'");

    // Invoke the parser to generate AST
    BlockStmt*  stmt     = parseString(stmtText, parseFn, parseMsg);

    // Determine if the body is also a BlockStmt
    BlockStmt*  b        = toBlockStmt(stmt->body.head);
    Expr*       newExpr  = NULL;

    // If NO then extract the RHS from the stmt
    if (b == 0) {
      if (CallExpr* c = toCallExpr(stmt->body.head)) {
        newExpr = c->get(2)->copy();

      } else {
        INT_ASSERT(false);
      }

    } else {
      if (CallExpr* c = toCallExpr(b->body.head)) {
        newExpr = c->get(2)->copy();

      } else {
        INT_ASSERT(false);
      }

    }

    configMap.put(astr(name), newExpr);

    INT_ASSERT(newExpr == configMap.get(astr(name)));
  }
}

Expr* getCmdLineConfig(const char* name) {
  return configMap.get(astr(name));
}

void useCmdLineConfig(const char* name, VarSymbol* byWhom) {
  usedConfigParams.put(astr(name), byWhom);
}

VarSymbol* isUsedCmdLineConfig(const char* name) {
  return usedConfigParams.get(name);
}


