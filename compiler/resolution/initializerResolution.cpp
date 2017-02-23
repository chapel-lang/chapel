/*
 * Copyright 2004-2017 Cray Inc.
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

#include "initializerResolution.h"

#include "astutil.h"
#include "caches.h"
#include "callInfo.h"
#include "expr.h"
#include "initializerRules.h"
#include "passes.h"
#include "resolution.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "type.h"
#include "view.h"

void temporaryInitializerFixup(CallExpr* call) {
  if (UnresolvedSymExpr* usym = toUnresolvedSymExpr(call->baseExpr)) {
    // Support super.init() calls (for instance) when the super type does not
    // define either an initializer or a constructor.  Also ignores errors from
    // improperly inserted .init() calls (so be sure to check here if something
    // is behaving oddly - Lydia, 08/19/16)
    if (strcmp(usym->unresolved, "init") == 0 &&
        call->numActuals()               >= 2) {
      SymExpr* _mt = toSymExpr(call->get(1));
      SymExpr* sym = toSymExpr(call->get(2));

      INT_ASSERT(sym != NULL);

      if (AggregateType* ct = toAggregateType(sym->symbol()->type)) {
        if (ct->initializerStyle == DEFINES_NONE_USE_DEFAULT) {

          // This code should be removed when the compiler generates
          // initializers as the default method of construction and
          // initialization for a type (Lydia note, 08/19/16)
          usym->unresolved = astr("_construct_", ct->symbol->name);

          _mt->remove();
        }
      }
    }
  }
}

void removeAggTypeFieldInfo() {
  forv_Vec(AggregateType, at, gAggregateTypes) {
    if (at->symbol->defPoint && at->symbol->defPoint->parentSymbol) {
      // Still in the tree
      if (at->initializerStyle == DEFINES_INITIALIZER) {
        // Defined an initializer (so we left its init and exprType information
        // in the tree)
        for_fields(field, at) {
          if (field->defPoint->exprType) {
            field->defPoint->exprType->remove();
          }
          if (field->defPoint->init) {
            field->defPoint->init->remove();
          }
          // Remove the init and exprType information.
        }
      }
    }
  }
}
