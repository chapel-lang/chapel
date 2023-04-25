/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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
#include "test-minimal-modules.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Record.h"
#include "chpl/uast/Variable.h"

// TODO:
// - this[] access
// - Slices

static QualifiedType findVarType(const Module* m,
                                 const ResolutionResultByPostorderID& rr,
                                 std::string name) {
  const Variable* var = findOnlyNamed(m, name)->toVariable();
  assert(var != nullptr);
  return rr.byAst(var).type();
}

static void testArray(std::string domainType,
                      std::string eltType) {
  Context ctx;
  Context* context = &ctx;
  ErrorGuard guard(context);

  // a different element type from the one we were given
  std::string altElt = "int";
  if (eltType == "int") {
    altElt = "string";
  }

  std::string program = DomainModule + ArrayModule +
R"""(
module M {
  use ChapelArray;
  
  var d : )""" + domainType + R"""(;
  type eltType = )""" + eltType + R"""(;

  var A : [d] eltType;

  const AD = A.domain;
  const s = A.size;

  for loopI in A {
    var z = loopI;
  }

  proc generic(arg: []) {
    type GT = arg.type;
    return 42;
  }

  proc eltOnly(arg: [] )""" + eltType + R"""() param {
    type ETGood = arg.type;
    return "correct method";
  }

  proc eltOnly(arg: [] )""" + altElt + R"""() param {
    type ETBad = arg.type;
    return "wrong method";
  }

  var g_ret = generic(A);
  param e_ret = eltOnly(A);
}
)""";

  auto path = UniqueString::get(context, "input.chpl");
  setFileText(context, path, std::move(program));

  const ModuleVec& vec = parseToplevel(context, path);
  const Module* m = vec[2]; 

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  QualifiedType dType = findVarType(m, rr, "d");

  QualifiedType eType = findVarType(m, rr, "eltType");

  QualifiedType AType = findVarType(m, rr, "A");

  assert(findVarType(m, rr, "AD").type() == dType.type());

  assert(findVarType(m, rr, "s").type()->isIntType());

  assert(findVarType(m, rr, "z").type() == eType.type());

  {
    const Variable* g_ret = findOnlyNamed(m, "g_ret")->toVariable();
    auto res = rr.byAst(g_ret);
    assert(res.type().type()->isIntType());

    auto call = resolveOnlyCandidate(context, rr.byAst(g_ret->initExpression()));
    // Generic function, should have been instantiated
    assert(call->signature()->instantiatedFrom() != nullptr);

    const Variable* GT = findOnlyNamed(m, "GT")->toVariable();
    assert(call->byAst(GT).type().type() == AType.type());
  }

  {
    const Variable* e_ret = findOnlyNamed(m, "e_ret")->toVariable();
    auto ret = rr.byAst(e_ret).type();
    assert(ret.isParam());
    assert(ret.param()->toStringParam()->value() == "correct method");

    auto call = resolveOnlyCandidate(context, rr.byAst(e_ret->initExpression()));
    // Generic function, should have been instantiated
    assert(call->signature()->instantiatedFrom() != nullptr);

    const Variable* ETGood = findOnlyNamed(m, "ETGood")->toVariable();
    assert(call->byAst(ETGood).type().type() == AType.type());
  }

  assert(guard.errors().size() == 0);

  std::string arrayText;
  arrayText += "[" + domainType + "] " + eltType;
  printf("Success: %s\n", arrayText.c_str());
}

int main() {
  testArray("domain(1)", "int");
  testArray("domain(1)", "string");
  testArray("domain(2)", "int");
  testArray("domain(int)", "int");

  return 0;
}
