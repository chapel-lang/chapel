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

#include "callInfo.h"

#include "baseAST.h"
#include "expr.h"

CallInfo::CallInfo() {
  call    = NULL;
  scope   = NULL;
  name    = NULL;
}

bool CallInfo::isNotWellFormed(CallExpr* callExpr, bool forInit) {
  bool retval = false;

  call = callExpr;

  if (SymExpr* se = toSymExpr(call->baseExpr)) {
    name = se->symbol()->name;

  } else if (UnresolvedSymExpr* use = toUnresolvedSymExpr(call->baseExpr)) {
    name = use->unresolved;
  }

  if (call->numActuals() >= 2) {
    if (SymExpr* se = toSymExpr(call->get(1))) {
      if (se->symbol() == gModuleToken) {
        se->remove();

        se = toSymExpr(call->get(1));
        INT_ASSERT(se);

        ModuleSymbol* mod = toModuleSymbol(se->symbol());
        INT_ASSERT(mod);

        se->remove();

        scope = mod->block;
      }
    }
  }

  for (int i = 1; i <= call->numActuals() && retval == false; i++) {
    Expr* actual = call->get(i);
    bool  isThis = false;

    if (NamedExpr* named = toNamedExpr(actual)) {
      actualNames.add(named->name);

      if (forInit == true && named->name == astrThis) {
        isThis = true;
      }

      actual = named->actual;

    } else {
      actualNames.add(NULL);
    }

    SymExpr* se = toSymExpr(actual);

    INT_ASSERT(se);

    Symbol*  sym = se->symbol();
    Type*    t   = sym->type;

    if (t == dtUnknown && sym->hasFlag(FLAG_TYPE_VARIABLE) == false) {
      retval = true;

    } else if (t->symbol->hasFlag(FLAG_GENERIC) == true && isThis == false) {
      retval = true;

    } else {
      actuals.add(sym);
    }
  }

  return retval;
}

void CallInfo::haltNotWellFormed(bool forInit) const {
  for (int i = 1; i <= call->numActuals(); i++) {
    Expr* actual = call->get(i);
    bool  isThis = false;

    if (NamedExpr* named = toNamedExpr(actual)) {
      if (forInit == true && named->name == astrThis) {
        isThis = true;
      }

      actual = named->actual;
    }

    SymExpr* se = toSymExpr(actual);
    INT_ASSERT(se);

    Symbol*  sym = se->symbol();
    Type*    t   = sym->type;

    if (t == dtUnknown && sym->hasFlag(FLAG_TYPE_VARIABLE) == false) {
      USR_FATAL(call,
                "use of '%s' before encountering its definition, "
                "type unknown",
                sym->name);

    } else if (t->symbol->hasFlag(FLAG_GENERIC) == true) {
      if (forInit == false || isThis == false) {
        INT_FATAL(call,
                  "the type of the actual argument '%s' is generic",
                  sym->name);
      }
    }
  }
}
