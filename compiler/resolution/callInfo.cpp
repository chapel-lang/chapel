/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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
#include "driver.h"
#include "expr.h"
#include "iterator.h"
#include "resolution.h"
#include "stringutil.h"

CallInfo::CallInfo() {
  call  = NULL;
  scope = NULL;
  name  = NULL;
}

bool CallInfo::isWellFormed(CallExpr* callExpr) {
  bool retval = true;

  call = callExpr;

  if (SymExpr* se = toSymExpr(call->baseExpr)) {
    if (se->symbol()->hasFlag(FLAG_TYPE_VARIABLE)) {
      name = se->typeInfo()->symbol->name;
    } else {
      name = se->symbol()->name;
    }

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

  for (int i = 1; i <= call->numActuals() && retval == true; i++) {
    Expr* actual = call->get(i);

    if (NamedExpr* named = toNamedExpr(actual)) {
      actualNames.add(named->name);

      actual = named->actual;

    } else {
      actualNames.add(NULL);
    }

    if (isDefExpr(actual)) {
      // This implies a '?t' style query expression, which we don't currently
      // support if we got here
      return false;
    }

    SymExpr* se = toSymExpr(actual);

    INT_ASSERT(se);

    Symbol*  sym = se->symbol();
    Type*    t   = sym->type;

    if (t == dtUnknown && sym->hasFlag(FLAG_TYPE_VARIABLE) == false) {
      retval = false;

    } else if (t->symbol->hasFlag(FLAG_GENERIC) == true) {
      // The _this actual to an initializer may be generic
      bool isInit = strcmp(name, "init") == 0 ||
                    strcmp(name, astrInitEquals) == 0;
      if (isInit && i == 2) {
        actuals.add(sym);

      } else if (sym->hasFlag(FLAG_TYPE_VARIABLE)) {
        // type formals can be generic
        actuals.add(sym);

      } else {
        retval = false;
      }

    } else {
      actuals.add(sym);
    }
  }

  return retval;
}

void CallInfo::haltNotWellFormed() const {
  for (int i = 1; i <= call->numActuals(); i++) {
    Expr* actual = call->get(i);

    if (NamedExpr* named = toNamedExpr(actual)) {
      actual = named->actual;
    }

    if (isDefExpr(actual)) {
      USR_FATAL(actual, "Query expressions are not currently supported in this context");
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

    } else if (t->symbol->hasFlag(FLAG_GENERIC) == true &&
               sym->hasFlag(FLAG_TYPE_VARIABLE) == false) {
      USR_FATAL_CONT(call,
                "the type of the actual argument '%s' is generic",
                sym->name);
      USR_PRINT("generic actual arguments are not currently supported");
      printUndecoratedClassTypeNote(call, t);
      USR_STOP();
    }
  }
}

const char* CallInfo::toString() {
  bool        method = false;
  bool        _this  = false;
  int         start  = 0;
  const char* retval = "";

  if (actuals.n            >  1 &&
      actuals.head()->type == dtMethodToken) {
    method = true;
    start  =    2;
  }

  if (name == astrThis) {
    _this  =  true;
    method = false;
    start  =     2;
  }

  if (method == true) {
    if (actuals.v[1] &&
        actuals.v[1]->hasFlag(FLAG_TYPE_VARIABLE)) {
      retval = astr(retval, "type ",
                    ::toString(actuals.v[1]->type, false), ".");

    } else {
      retval = astr(retval,
                    ::toString(actuals.v[1]->type, false), ".");
    }
  }

  if (_this == false) {
    retval = astr(retval, name);
  }

  if (call->methodTag == false) {
    if (call->square == true) {
      retval = astr(retval, "[");
    } else {
      retval = astr(retval, "(");
    }
  }

  for (int i = start; i < actuals.n; i++) {
    Symbol*        sym  = actuals.v[i];
    VarSymbol*     var  = toVarSymbol(sym);
    Type*          type = sym->type;
    AggregateType* at   = toAggregateType(type);
    IteratorInfo*  ii   = (at != NULL) ? at->iteratorInfo : NULL;

    if (i > start) {
      retval = astr(retval, ", ");
    }

    if (actualNames.v[i] != NULL) {
      retval = astr(retval, actualNames.v[i], "=");
    }

    if (type->symbol->hasFlag(FLAG_ITERATOR_RECORD)   == true &&
        ii->iterator->hasFlag(FLAG_PROMOTION_WRAPPER) == true) {
      retval = astr(retval, "promoted expression");

    } else if (sym->hasFlag(FLAG_TYPE_VARIABLE) == true) {
      retval = astr(retval, "type ", ::toString(type));

    } else if (var != NULL && var->immediate != NULL) {
      if (var->immediate->const_kind == CONST_KIND_STRING) {
        retval = astr(retval, "\"", var->immediate->v_string, "\"");

      } else {
        const size_t bufSize = 512;
        char         buff[bufSize];

        snprint_imm(buff, bufSize, *var->immediate);

        retval = astr(retval, buff);
      }

    } else {
      retval = astr(retval, ::toString(type));
    }
  }

  if (call->methodTag == false) {
    if (call->square == true) {
      retval = astr(retval, "]");
    } else {
      retval = astr(retval, ")");
    }
  }

  return retval;
}
