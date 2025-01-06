/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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
#include "resolution.h"
#include "stmt.h"
#include "stringutil.h"
#include "chpl/uast/Builder.h"
#include <utility>


using namespace chpl;

static Map<const char*, Expr*> configMap; // map from configs to vals
static Map<const char*, VarSymbol*> usedConfigParams; // map from configs to uses

bool                           mainHasArgs;
bool                           mainPreserveDelimiter;

void checkConfigs() {
  if (fMinimalModules == false) {
    if (uast::Builder::checkAllConfigVarsAssigned(gContext)) {
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
  auto pair = std::make_pair(std::string(name), std::string(value));
  gDynoParams.push_back(std::move(pair));
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

bool isSetCmdLineConfig(const char* moduleName, const char* paramName) {
  std::string fullName = std::string(moduleName) + "." + std::string(paramName);
  // TODO: This is O(n) in the number of params, a better way would be nicer.
  // However, there would never be >20-ish params if we're being
  // reasonable. So it's not that bad.
  for (const auto& pair : gDynoParams) {
    if (pair.first == fullName ||
        strcmp(pair.first.c_str(), paramName) == 0){
      return true;
    }
  }
  return false;
}

// Any call to this function MUST cache the result in a local variable
// Ex:
// bool usePointerImplementation(void) {
//   static bool flag = false;
//   static bool flagLegal = false;
//   if(!flagLegal) {
//     flag = getConfigParamBool(baseModule,"flag") == gTrue;
//     flagLegal = true;
//   }
//   return flag;
// }
VarSymbol* getConfigParamBool(ModuleSymbol* modSym, const char* configParamName) {

  // TODO: This is O(n) number of params, we need a better way to look
  // things up after resolve. I think that 'dyno' can always do the
  // elegant thing here. Just preserve the ID for 'ChapelBase', and then
  // use it in conjunction with a lookup for the config name. You fetch
  // the 'ResolvedExpression' and you're done.
  VarSymbol* retVal = nullptr;
  if (!retVal) {
    if (!modSym->initFn || !modSym->initFn->isResolved()) {
      INT_FATAL(modSym, "Called before '%s' is resolved",
                        modSym->name);
    }

    form_Map(SymbolMapElem, e, paramMap) {
      auto sym = e->key;
      if (sym->defPoint && sym->defPoint->getModule() == modSym) {
        if (!strcmp(sym->name, configParamName)) {
          auto vs = toVarSymbol(e->value);
          if (!vs || (vs != gTrue && vs != gFalse)) {
            INT_FATAL("Unexpected config param type or bad AST");
            return nullptr;
          }
          retVal = vs;
          break;
        }
      }
    }

    // Provide a hint, just in case.
    if (!retVal) {
      INT_FATAL("Could not find '%s', is it declared in '%s'?",
                configParamName,
                modSym->name);
    }
  }
  return retVal;
}


