/*
 * Copyright 2025 Hewlett Packard Enterprise Development LP
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
#include "chpl/uast/Module.h"
#include "chpl/uast/Variable.h"

static void testRectangularSparse(Context* context,
                                  std::string domainType,
                                  int rank,
                                  std::string idxType,
                                  std::string strides) {
  printf("Testing: sparse subdomain(%s)\n", domainType.c_str());

  context->advanceToNextRevision(false);
  setupModuleSearchPaths(context, false, false, {}, {});
  ErrorGuard guard(context);

  std::string program =
R"""(
module M {
  var parent : )""" + domainType + R"""(;
  var d : sparse subdomain(parent);
  param rg = )""" + std::to_string(rank) + R"""(;
  type ig = )""" + idxType + R"""(;
  type fullIndex = if rg == 1 then ig else rg*ig;

  param r = d.rank;
  type i = d.idxType;
  param s = d.strides;
  param rk = d.isRectangular();
  param ak = d.isAssociative();
  param sk = d.isSparse();

  param rttR = __primitive("get runtime type field", d, "rank");
  type rttI = __primitive("get runtime type field", d, "idxType");
  param rttS = __primitive("get runtime type field", d, "strides");

  var p = d.pid;

  for loopI in d {
    var z = loopI;
  }

  proc generic(arg: domain) {
    type GT = arg.type;
    return 42;
  }

  var g_ret = generic(d);
}
)""";

  auto path = UniqueString::get(context, "input.chpl");
  printf("program: %s\n", program.c_str());
  setFileText(context, path, std::move(program));

  const ModuleVec& vec = parseToplevel(context, path);
  const Module* m = vec[0];

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  const Variable* d = findVariable(m, "d");
  assert(d);
  assert(d->name() == "d");

  QualifiedType dQt = rr.byAst(d).type();
  assert(dQt.type());
  auto dType = dQt.type()->toDomainType();
  assert(dType);

  auto dTypeExpr = d->typeExpression();
  assert(dTypeExpr);
  auto typeRe = rr.byAst(dTypeExpr);

  QualifiedType fullIndexType = findVarType(m, rr, "fullIndex");

  auto rankVarTy = findVarType(m, rr, "r");
  assert(rankVarTy == dType->rank());
  ensureParamInt(rankVarTy, rank);

  assert(findVarType(m, rr, "rttR") == rankVarTy);

  auto idxTypeVarTy = findVarType(m, rr, "i");
  assert(idxTypeVarTy == dType->idxType());
  assert(findVarType(m, rr, "ig") == idxTypeVarTy);

  assert(findVarType(m, rr, "rttI") == idxTypeVarTy);

  auto stridesVarTy = findVarType(m, rr, "s");
  assert(stridesVarTy == dType->strides());
  assert(stridesVarTy.param()->toEnumParam()->value().str == strides);

  assert(findVarType(m, rr, "rttS") == stridesVarTy);

  ensureParamBool(findVarType(m, rr, "rk"), false);

  ensureParamBool(findVarType(m, rr, "ak"), false);

  ensureParamBool(findVarType(m, rr, "sk"), true);

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
    assert(call->byAst(GT).type().type() == dType);
  }

  assert(guard.realizeErrors() == 0);
}

int main() {
  auto context = buildStdContext();

  testRectangularSparse(context, "domain(1)", 1, "int", "one");
  testRectangularSparse(context, "domain(2)", 2, "int", "one");
  testRectangularSparse(context, "domain(1, strides=strideKind.one)", 1, "int", "one");
  testRectangularSparse(context, "domain(2, int(8))", 2, "int(8)", "one");
  testRectangularSparse(context, "domain(3, int(16), strideKind.negOne)", 3, "int(16)", "negOne");
  testRectangularSparse(context, "domain(strides=strideKind.negative, idxType=int, rank=1)", 1, "int", "negative");
}
