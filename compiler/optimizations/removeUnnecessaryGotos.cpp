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

#include "astutil.h"
#include "expr.h"
#include "optimizations.h"
#include "stmt.h"
#include <vector>
#include <set>
#include "stlUtil.h"

//
// Removes gotos where the label immediately follows the goto and
// unused labels
//
void removeUnnecessaryGotos(FnSymbol* fn) {
  std::vector<BaseAST*> asts;
  std::set<BaseAST*> labels;
  collect_asts_STL(fn, asts);
  for_vector(BaseAST, ast, asts) {
    if (GotoStmt* gotoStmt = toGotoStmt(ast)) {
      DefExpr* def = toDefExpr(gotoStmt->next);
      SymExpr* label = toSymExpr(gotoStmt->label);
      INT_ASSERT(label);
      if (def && def->sym == label->var)
        gotoStmt->remove();
      else
        labels.insert(label->var);
    }
  }

  for_vector(BaseAST, ast2, asts) {
    if (DefExpr* def = toDefExpr(ast2))
      if (LabelSymbol* label = toLabelSymbol(def->sym))
        if (labels.find(label) == labels.end())
          def->remove();
  }
}
