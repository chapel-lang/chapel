/*
 * Copyright 2004-2016 Cray Inc.
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

#include "initializerRules.h"

#include "astutil.h"
#include "expr.h"
#include "resolution.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"

// Helper file for verifying the rules placed on initializers, and providing
// the extra functionality associated with them.

void temporaryInitializerFixup(CallExpr* call) {
  if (UnresolvedSymExpr* usym = toUnresolvedSymExpr(call->baseExpr)) {
    // Support super.init() calls (for instance) when the super type does not
    // define either an initializer or a constructor.  Also ignores errors from
    // improperly inserted .init() calls (so be sure to check here if something
    // is behaving oddly - Lydia, 08/19/16)
    if (!strcmp(usym->unresolved, "init")) {
      for_actuals(actual, call) {
        if (NamedExpr* named = toNamedExpr(actual)) {
          if (!strcmp(named->name, "meme")) {
            if (SymExpr* sym = toSymExpr(named->actual)) {
              if (AggregateType* ct = toAggregateType(sym->var->type)) {
                if (ct->initializerStyle == DEFINES_NONE_USE_DEFAULT) {
                  // This code should be removed when the compiler generates
                  // initializers as the default method of construction and
                  // initialization for a type (Lydia note, 08/19/16)
                  usym->unresolved = astr("_construct_", ct->symbol->name);
                } else if (ct->initializerStyle == DEFINES_CONSTRUCTOR) {
                  // This code should be removed when initializers are fully
                  // supported and old style constructors are deprecated
                  // (Lydia note, 08/19/16)
                  USR_FATAL(call, "can't make init call on type with old constructor style");
                }
              }
            }
          }
        }
      }
    }
  }
}
