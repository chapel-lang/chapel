/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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

#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/types/ComplexType.h"
#include "chpl/types/IntType.h"
#include "chpl/types/Param.h"
#include "chpl/types/RealType.h"
#include "chpl/uast/Module.h"

// always check assertions in this test
#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>

using namespace chpl;
using namespace parsing;
using namespace resolution;
using namespace uast;
using namespace types;

static QualifiedType getTypeForFirstStmt(Context* context,
                                         const std::string& program) {
  auto path = UniqueString::build(context, "input.chpl");
  setFileText(context, path, program);

  const ModuleVec& vec = parse(context, path);
  assert(vec.size() == 1);
  const Module* m = vec[0]->toModule();
  assert(m);
  assert(m->numStmts() == 1);
  auto stmt = m->stmt(0);
  assert(stmt);

  const ResolutionResultByPostorderID& rr = resolveModule(context, m->id());

  const auto& resolvedExpr = rr.byAst(stmt);

  return resolvedExpr.type();
}

static void test1() {
  printf("test1\n");
  Context ctx;

  QualifiedType qt = getTypeForFirstStmt(&ctx,
                                         "__primitive('+', 1, 2);\n");
  assert(qt.hasTypePtr());
  assert(qt.hasParamPtr());

  assert(qt.type() == IntType::get(&ctx, 0));
  assert(qt.param() == IntParam::get(&ctx, 3));
}

static void test2() {
  printf("test2\n");
  Context ctx;

  QualifiedType qt = getTypeForFirstStmt(&ctx,
                                         "__primitive('*', 2.25, 4.25);\n");
  assert(qt.hasTypePtr());
  assert(qt.hasParamPtr());

  assert(qt.type() == RealType::get(&ctx, 0));
  assert(qt.param() == RealParam::get(&ctx, 9.5625));
}

static void test3() {
  printf("test3\n");
  Context ctx;

  QualifiedType qt = getTypeForFirstStmt(&ctx,
                                         "__primitive('+', 3.0, 2.0i);\n");
  assert(qt.hasTypePtr());
  assert(qt.hasParamPtr());

  assert(qt.type() == ComplexType::get(&ctx, 0));
  assert(qt.param() == ComplexParam::get(&ctx, {3.0, 2.0}));
}

int main() {
  test1();
  test2();
  test3();

  return 0;
}
