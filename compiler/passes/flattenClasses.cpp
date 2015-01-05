/*
 * Copyright 2004-2015 Cray Inc.
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

#include "alist.h"
#include "astutil.h"
#include "baseAST.h"
#include "expr.h"
#include "passes.h"
#include "stmt.h"


void flattenClasses(void) {
  //
  // collect nested classes
  //
  Vec<AggregateType*> nestedClasses;
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (AggregateType* ct = toAggregateType(ts->type))
      if (toAggregateType(ct->symbol->defPoint->parentSymbol->type))
        nestedClasses.add(ct);
  }

  //
  // move nested classes to module level
  //
  forv_Vec(AggregateType, ct, nestedClasses) {
    ModuleSymbol* mod = ct->getModule();
    DefExpr *def = ct->symbol->defPoint;
    def->remove();
    mod->block->insertAtTail(def);
  }
}
