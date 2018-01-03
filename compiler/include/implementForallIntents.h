/*
 * Copyright 2004-2018 Cray Inc.
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

#ifndef _implementForallIntents_H_
#define _implementForallIntents_H_

#include "resolution.h"

#include "astutil.h"
#include "DeferStmt.h"
#include "driver.h"
#include "ForallStmt.h"
#include "ForLoop.h"
#include "passes.h"
#include "resolveIntents.h"
#include "stlUtil.h"
#include "stringutil.h"
#include "visibleFunctions.h"
#include <utility>

// functions currently defined/used across implementForallIntents*.cpp

extern Map<FnSymbol*,FnSymbol*> pristineLeaderIterators;

IntentTag argIntentForForallIntent(ForallIntentTag tfi);
bool      callingParallelIterator(CallExpr* call);
void      addArgsToToLeaderCallForPromotionWrapper(FnSymbol* fn,
                                                   int numExtraArgs,
                                                   Symbol* extraFormals[]);
VarSymbol* localizeYieldForExtendLeader(Expr* origRetExpr, Expr* ref);
void       checkAndRemoveOrigRetSym(Symbol* origRet, FnSymbol* parentFn);
FnSymbol*  copyLeaderFn(FnSymbol* origFn, bool ignore_isResolved);

void implementForallIntents2New(ForallStmt* fs, CallExpr* parCall);
void printIFI2cache();

static const char* intentArgName(int ix, const char* base) {
  return astr("_x", istr(ix+1), "_", base);
}

#endif
