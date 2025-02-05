/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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


static void testAssociative(Context* context,
                                  std::string domainType,
                                  std::string idxType,
                                  bool parSafe) {
  printf("Testing: %s\n", domainType.c_str());

  context->advanceToNextRevision(false);
  setupModuleSearchPaths(context, false, false, {}, {});
  ErrorGuard guard(context);

  std::string program =
R"""(
module M {
  var d : )""" + domainType + R"""(;
  type ig = )""" + idxType + R"""(;

  type i = d.idxType;
  param s = d.parSafe;
  param rk = d.isRectangular();
  param ak = d.isAssociative();

  var p = d.pid;

  for loopI in d {
    var z = loopI;
  }

  proc generic(arg: domain) {
    type GT = arg.type;
    return 42;
  }

  proc concrete(arg: )""" + domainType + R"""() {
    type CT = arg.type;
    return 42;
  }

  var g_ret = generic(d);
  var c_ret = concrete(d);
}
)""";

  auto path = UniqueString::get(context, "input.chpl");
  setFileText(context, path, std::move(program));

  const ModuleVec& vec = parseToplevel(context, path);
  const Module* m = vec[0];

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  QualifiedType dType = findVarType(m, rr, "d");
  assert(dType.type()->isDomainType());

  auto fullIndexType = findVarType(m, rr, "i");
  assert(findVarType(m, rr, "ig") == fullIndexType);

  ensureParamBool(findVarType(m, rr, "s"), parSafe);

  ensureParamBool(findVarType(m, rr, "rk"), false);

  ensureParamBool(findVarType(m, rr, "ak"), true);

  assert(findVarType(m, rr, "p").type()->isIntType());

  assert(findVarType(m, rr, "z").type() == fullIndexType.type());

  {
    const Variable* g_ret = findOnlyNamed(m, "g_ret")->toVariable();
    auto res = rr.byAst(g_ret);
    assert(res.type().type()->isIntType());

    auto call = resolveOnlyCandidate(context, rr.byAst(g_ret->initExpression()));
    // Generic function, should have been instantiated
    assert(call->signature()->instantiatedFrom() != nullptr);

    const Variable* GT = findOnlyNamed(m, "GT")->toVariable();
    assert(call->byAst(GT).type().type() == dType.type());
  }

  {
    const Variable* c_ret = findOnlyNamed(m, "c_ret")->toVariable();
    auto res = rr.byAst(c_ret);
    assert(res.type().type()->isIntType());

    auto call = resolveOnlyCandidate(context, rr.byAst(c_ret->initExpression()));
    // Concrete function, should not be instantiated
    assert(call->signature()->instantiatedFrom() == nullptr);

    const Variable* CT = findOnlyNamed(m, "CT")->toVariable();
    assert(call->byAst(CT).type().type() == dType.type());
  }

  assert(guard.errors().size() == 0);
}

int main() {
  // Set up context with standard modules, re-used between tests for
  // performance.
  auto context = buildStdContext();

  testAssociative(context, "domain(int)", "int", false);
  testAssociative(context, "domain(int, true)", "int", true);
  testAssociative(context, "domain(string)", "string", false);

  testDomainLiteral(context, "{1, 2, 3}", DomainType::Kind::Associative);
  testDomainLiteral(context, "{\"apple\", \"banana\"}", DomainType::Kind::Associative);

  testDomainBadPass(context, "domain(int)", "domain(string)");
  testDomainBadPass(context, "domain(1)", "domain(int)");

  testDomainIndex(context, "domain(int)", "int");
  testDomainIndex(context, "domain(string)", "string");

  return 0;
}
