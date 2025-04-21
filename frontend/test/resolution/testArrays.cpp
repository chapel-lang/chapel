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

static void testArray(std::string domainType,
                      std::string eltType) {
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
  // input types
  var d : )""" + domainType + R"""(;
  type eltType = )""" + eltType + R"""(;
  var A : [d] eltType;

  // misc fields and methods
  const AD = A.domain;
  type idxType = A.idxType;
  type fullIdxType = A.fullIdxType;
  type gotEltType = A.eltType;
  param rank = A.rank;
  const size = A.size;
  const sizeAsUint = A.sizeAs(uint);
  const targetLocales = A.targetLocales();
  var isEmpty = A.isEmpty();
  var someElt : eltType;
  var countElt = A.count(someElt);

  // fields and methods specific to array kind
  param isRectangular = A.isRectangular();
  param isAssociative = A.isAssociative();
  if (isRectangular) {
    var strides = A.strides;
    var rectIndices = A.indices;
    var dims = A.dims();
    var dimZero = A.dim(0);
    var reindexedA = A.reindex(d);
    if (rank == 1) {
      var last = A.last;
      var first = A.first;
    }
    var findElt = A.find(someElt);
    var shape = A.shape;
    var reshapedA = reshape(A, d);
  }
  if (isAssociative) {
    var assocIndices = A.indices;
  }

  // indexing
  var idx : index(A.domain);
  var x = A[idx];

  // iteration
  for loopI in A {
    var z = loopI;
  }

  // call resolution
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
  assert(dType.type()->isDomainType());
  QualifiedType eType = findVarType(m, rr, "eltType");
  QualifiedType AType = findVarType(m, rr, "A");
  assert(AType.type()->isArrayType());

  // array procs
  assert(findVarType(m, rr, "AD").type() == dType.type());
  QualifiedType gotIdxType = findVarType(m, rr, "idxType");
  QualifiedType gotFullIdxType = findVarType(m, rr, "fullIdxType");
  assert(gotIdxType == dType.type()->toDomainType()->idxType());
  assert(findVarType(m, rr, "gotEltType").type() == eType.type());
  auto rankQt = findVarType(m, rr, "rank");
  assert(rankQt.type()->isIntType());
  assert(rankQt.param() && rankQt.param()->isIntParam());
  int rank = rankQt.param()->toIntParam()->value();
  assert(findVarType(m, rr, "size").type()->isIntType());
  assert(findVarType(m, rr, "sizeAsUint").type()->isUintType());
  assert(findVarType(m, rr, "targetLocales").type()->isArrayType());
  assert(findVarType(m, rr, "isEmpty").type()->isBoolType());
  assert(findVarType(m, rr, "someElt").type() == eType.type());
  assert(findVarType(m, rr, "countElt").type()->isIntType());

  auto isRectangularQt = findVarType(m, rr, "isRectangular");
  assert(isRectangularQt.type()->isBoolType());
  bool isRectangular = isRectangularQt.isParamTrue();
  auto isAssociativeQt = findVarType(m, rr, "isAssociative");
  assert(isAssociativeQt.type()->isBoolType());
  bool isAssociative = isAssociativeQt.isParamTrue();
  assert(isRectangular != isAssociative); // should be one or the other
  if (isRectangular) {
    assert(findVarType(m, rr, "strides").type()->isEnumType());
    assert(findVarType(m, rr, "rectIndices").type()->isDomainType());
    assert(findVarType(m, rr, "dims").type()->isTupleType());
    assert(findVarType(m, rr, "dimZero").type()->isRecordType());
    {
      // compare original and reindexed string representation as an approximate
      // equality check
      auto reindexedAQt = findVarType(m, rr, "reindexedA");
      std::stringstream ss1, ss2;
      reindexedAQt.type()->stringify(ss1, chpl::StringifyKind::CHPL_SYNTAX);
      AType.type()->stringify(ss2, chpl::StringifyKind::CHPL_SYNTAX);
      assert(reindexedAQt.type()->isArrayType());
      assert(ss1.str() == ss2.str());
    }
    if (rank == 1) {
      assert(findVarType(m, rr, "last").type() == eType.type());
      assert(findVarType(m, rr, "first").type() == eType.type());
    }
    assert(findVarType(m, rr, "findElt").type() == gotFullIdxType.type());
    assert(findVarType(m, rr, "shape").type()->isTupleType());
    assert(findVarType(m, rr, "reshapedA").type()->isArrayType());
  } else if (isAssociative) {
    assert(findVarType(m, rr, "assocIndices").type()->isArrayType());
  }

  assert(findVarType(m, rr, "x").type() == eType.type());

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

static void testArraySlicing(std::string arrayLiteral, std::string sliceArgs,
                             std::string resultType) {
  printf("Testing slicing array %s with %s\n", arrayLiteral.c_str(),
         sliceArgs.c_str());

  Context* context = buildStdContext();
  ErrorGuard guard(context);

  auto vars = resolveTypesOfVariables(context,
    R"""(
    module M {
      var A = )""" + arrayLiteral + R"""(;
      var mySlice = A[)""" + sliceArgs + R"""(];
      type gotSliceTy = mySlice.type;
      type expectedSliceTy = )""" + resultType + R"""(;
    }
    )""",
    {"A", "gotSliceTy", "expectedSliceTy"});
  auto arrType = vars.at("A");
  assert(arrType.type());
  assert(!arrType.type()->isUnknownType());
  assert(arrType.type()->isArrayType());

  auto gotSliceTy = vars.at("gotSliceTy");
  assert(gotSliceTy.type());
  assert(!gotSliceTy.type()->isUnknownType());

  auto expectedSliceTy = vars.at("expectedSliceTy");
  assert(expectedSliceTy.type());
  assert(!expectedSliceTy.type()->isUnknownType());

  // Compare types approximately by stringifying, since they won't have the
  // same underlying instance
  std::stringstream ss1, ss2;
  gotSliceTy.type()->stringify(ss1, chpl::StringifyKind::CHPL_SYNTAX);
  expectedSliceTy.type()->stringify(ss2, chpl::StringifyKind::CHPL_SYNTAX);
  assert(ss1.str() == ss2.str());

  assert(guard.realizeErrors() == 0);
}

int main() {
  // rectangular
  testArray("domain(1)", "int");
  testArray("domain(1)", "string");
  testArray("domain(2)", "int");

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

  // slices
  testArraySlicing("[1, 2, 3]", "0..1", "[0..1] int");
  testArraySlicing("[1, 2; 3, 4]", "0, 1", "int"); // testing multi-dim indexing, not really a slice
  testArraySlicing("[1, 2; 3, 4;; 5, 6; 7, 8]", "0..1, 0..1, 1", "[0..1, 0..1] int");

  // associative
  testArray("domain(int)", "int");
  testArray("domain(int, true)", "int");

  // associative literals
  testArrayLiteral("[1 => \"one\", 10 => \"ten\", 3 => \"three\", 16 => \"sixteen\"]", "domain(int)", "string");
  testArrayLiteral("[1 => 1, 10 => 10, 3 => 3, 16 => 16]", "domain(int)", "int");
  testArrayLiteral("[\"foo\" => false]", "domain(string)", "bool");
  testArrayLiteral("[\"foo\" => false, ]", "domain(string)", "bool");

  return 0;
}
