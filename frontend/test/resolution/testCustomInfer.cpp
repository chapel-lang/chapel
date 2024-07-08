/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

#include "test-resolution.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Record.h"
#include "chpl/uast/Variable.h"

static QualifiedType findVarType(const Module* m,
                                 const ResolutionResultByPostorderID& rr,
                                 std::string name) {
  const Variable* var = findOnlyNamed(m, name)->toVariable();
  assert(var != nullptr);
  return rr.byAst(var).type();
}

static void testRecordInt() {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  std::string program =
R"""(
module M {
  pragma "infer custom type"
  record R {
    var val : int;
  }

  proc R.chpl__inferCopyType() type {
    return int;
  }

  operator =(ref lhs: int, const rhs: R) {
    lhs = rhs.val;
  }

  var x : R;
  var y = x;
  var z : R = x;
}
)""";

  auto path = UniqueString::get(context, "input.chpl");
  setFileText(context, path, std::move(program));

  const ModuleVec& vec = parseToplevel(context, path);
  const Module* m = vec[0];

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  QualifiedType x = findVarType(m, rr, "x");
  // Make sure it's actually 'R'
  auto xTypeNode = parsing::idToAst(context, x.type()->getCompositeType()->id());
  assert(xTypeNode == m->stmt(0));

  QualifiedType y = findVarType(m, rr, "y");
  assert(y.type() == IntType::get(context, 0));

  QualifiedType z = findVarType(m, rr, "z");
  assert(z.type() == x.type());

  auto yVar = findOnlyNamed(m, "y")->toVariable();
  const ResolvedExpression* re = rr.byAstOrNull(yVar);
  bool foundAction = false;
  if (re != nullptr) {
    for (auto act : re->associatedActions()) {
      if (act.action() == AssociatedAction::INFER_TYPE) {
        foundAction = true;
      }
    }
  }
  assert(foundAction);
}

int main() {
  testRecordInt();

  return 0;
}
