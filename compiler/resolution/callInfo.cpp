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

#include "baseAST.h"
#include "callInfo.h"
#include "expr.h"

CallInfo::CallInfo(CallExpr* icall, bool checkonly, bool initOkay) :
  call(icall),
  scope(NULL),
  name(NULL),
  actuals(),
  actualNames(),
  badcall(false)
{
  if (SymExpr* se = toSymExpr(call->baseExpr))
    name = se->symbol()->name;
  else if (UnresolvedSymExpr* use = toUnresolvedSymExpr(call->baseExpr))
    name = use->unresolved;
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
  for_actuals(actual, call) {
    bool isThis = false;
    if (NamedExpr* named = toNamedExpr(actual)) {
      actualNames.add(named->name);
      if (initOkay && !strcmp(named->name, "this"))
        isThis = true;
      actual = named->actual;
    } else {
      actualNames.add(NULL);
    }
    SymExpr* se = toSymExpr(actual);
    INT_ASSERT(se);
    Type* t = se->symbol()->type;
    if (t == dtUnknown && ! se->symbol()->hasFlag(FLAG_TYPE_VARIABLE) ) {
      if (checkonly) badcall = true;
      else USR_FATAL(call, "use of '%s' before encountering its definition, "
                           "type unknown", se->symbol()->name);
    }
    if (t->symbol->hasFlag(FLAG_GENERIC)) {
      if (initOkay && isThis) {
        // initOkay is only used when resolving an initializer call, so we
        // allow the this argument to be generic, as we will perform the work
        // necessary to instantiate it.
      } else if (checkonly) badcall = true;
      else INT_FATAL(call, "the type of the actual argument '%s' is generic",
                            se->symbol()->name);
    }
    actuals.add(se->symbol());
  }
}
