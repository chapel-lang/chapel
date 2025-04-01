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

// TODO:
// - Slices

// Test using an array with a type expression consisting of the given domain
// and element type.
// Optionally accepts arguments to test indexing into the array with (in
// addition to doing so with the default value of the domain's index type).
static void testArray(std::string domainType,
                      std::string eltType,
                      std::string testIdxArg = "") {
  std::string arrayText;
  arrayText += "[" + domainType + "] " + eltType;
  printf("Testing array type expression: %s\n", arrayText.c_str());

  Context* context = buildStdContext();
  ErrorGuard guard(context);

  // a different element type from the one we were given
  std::string altElt = "int";
  if (eltType == "int") {
    altElt = "string";
  }

  std::string program = R"""(
module M {
  var d : )""" + domainType + R"""(;
  type eltType = )""" + eltType + R"""(;

  var A : [d] eltType;

  var someElt : eltType;

  type gotEltType = A.eltType;
  param rank = A.rank;
  param strides = A.strides;
  var indices = A.indices;
  var dims = A.dims();
  var dimZero = A.dim(0);
  const AD = A.domain;
  const size = A.size;
  const sizeAsUint = A.sizeAs(uint);
  var reindexedA = A.reindex(d);
  const targetLocales = A.targetLocales();
  var isEmpty = A.isEmpty();
  if (rank == 1) {
    var last = A.last;
    var first = A.first;
  }
  var countElt = A.count(someElt);
  var shape = A.shape;

  var idx : index(A.domain);
  var x1 = A[idx];
  var x2 = A[)""" + (testIdxArg.empty() ? "idx" : testIdxArg) + R"""(];

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
  const Module* m = vec[0];

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  // input types
  QualifiedType dType = findVarType(m, rr, "d");
  QualifiedType eType = findVarType(m, rr, "eltType");
  QualifiedType AType = findVarType(m, rr, "A");

  // array procs
  assert(findVarType(m, rr, "gotEltType").type() == eType.type());
  auto rankQt = findVarType(m, rr, "rank");
  assert(rankQt.type()->isIntType());
  assert(rankQt.param() && rankQt.param()->isIntParam());
  int rank = rankQt.param()->toIntParam()->value();
  assert(findVarType(m, rr, "strides").type()->isEnumType());
  assert(findVarType(m, rr, "indices").type()->isDomainType());
  assert(findVarType(m, rr, "dims").type()->isTupleType());
  assert(findVarType(m, rr, "dimZero").type()->isRecordType());
  assert(findVarType(m, rr, "AD").type() == dType.type());
  assert(findVarType(m, rr, "size").type()->isIntType());
  assert(findVarType(m, rr, "sizeAsUint").type()->isUintType());
  // compare original and reindexed string representation as an approximate
  // equality check
  auto reindexedAQt = findVarType(m, rr, "reindexedA");
  std::stringstream ss1, ss2;
  reindexedAQt.type()->stringify(ss1, chpl::StringifyKind::CHPL_SYNTAX);
  AType.type()->stringify(ss2, chpl::StringifyKind::CHPL_SYNTAX);
  assert(reindexedAQt.type()->isArrayType());
  assert(ss1.str() == ss2.str());
  assert(findVarType(m, rr, "targetLocales").type()->isArrayType());
  assert(findVarType(m, rr, "isEmpty").type()->isBoolType());
  if (rank == 1) {
    assert(findVarType(m, rr, "last").type() == eType.type());
    assert(findVarType(m, rr, "first").type() == eType.type());
  }
  assert(findVarType(m, rr, "countElt").type()->isIntType());
  assert(findVarType(m, rr, "shape").type()->isTupleType());

  assert(findVarType(m, rr, "x1").type() == eType.type());
  assert(findVarType(m, rr, "x2").type() == eType.type());

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

  assert(guard.realizeErrors() == 0);
}

static void testArrayLiteral(std::string arrayLiteral, std::string domainType,
                             std::string eltType) {
  printf("Testing array literal: %s\n", arrayLiteral.c_str());

  Context* context = buildStdContext();
  ErrorGuard guard(context);

  auto vars = resolveTypesOfVariables(context,
    R"""(
    module M {
      var A = )""" + arrayLiteral + R"""(;

      type expectedDomTy = )""" + domainType + R"""(;
      type expectedEltTy = )""" + eltType + R"""(;

      type actualDomTy = A.domain.type;
      type actualEltTy = A.eltType;

      param correctDom = expectedDomTy == actualDomTy;
      param correctElt = expectedEltTy == actualEltTy;
    }
    )""",
    {"A", "correctDom", "correctElt"});
  auto arrType = vars.at("A");
  assert(arrType.type());
  assert(!arrType.type()->isUnknownType());
  assert(arrType.type()->isArrayType());

  ensureParamBool(vars.at("correctDom"), true);
  ensureParamBool(vars.at("correctElt"), true);

  assert(guard.realizeErrors() == 0);
}

int main() {
  // rectangular
  testArray("domain(1)", "int");
  testArray("domain(1)", "string");
  testArray("domain(2)", "int", "0, 1");

  // 1D literals
  testArrayLiteral("[1, 2, 3]", "domain(1)", "int");
  testArrayLiteral("[1, 2, 3,]", "domain(1)", "int");
  testArrayLiteral("[1]", "domain(1)", "int");
  testArrayLiteral("[1.0, 2]", "domain(1)", "real");
  testArrayLiteral("[\"foo\", \"bar\"]", "domain(1)", "string");
  testArrayLiteral("[1..10]", "domain(1)", "range");
  testArrayLiteral("[[1, 2, 3, 4, 5], [6, 7, 8, 9, 10]]", "domain(1)", "[1..5] int");
  // multi-dim literals
  testArrayLiteral("[1, 2; 3, 4]", "domain(2)", "int");
  testArrayLiteral("[1, 2; 3, 4;]", "domain(2)", "int");
  testArrayLiteral("[1;]", "domain(2)", "int");
  testArrayLiteral("[1, 2; 3, 4;; 5, 6; 7, 8]", "domain(3)", "int");

  // associative
  testArray("domain(int)", "int");
  testArray("domain(int, true)", "int");

  return 0;
}
