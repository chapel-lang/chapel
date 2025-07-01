/*
 * Copyright 2024-2025 Hewlett Packard Enterprise Development LP
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

/* An important aspect of data structures in Dyno is that they behave properly
   under the query system. This means that they support re-use and memoization
   of queries. These tests are intended to lock in the behavior of data structures
   under the query framework. In particular, they currently test that equivalent
   but unequal data structures used in queries still allow the queries to be
   re-used across generations. */

#include "chpl/resolution/resolution-types.h"
#include "chpl/framework/query-impl.h"
#include "chpl/types/all-types.h"

using namespace chpl;
using namespace resolution;
using namespace uast;
using namespace types;

static int mscQueryCounter = 0;

static MostSpecificCandidate const& mscQuery(Context* context) {
  QUERY_BEGIN_INPUT(mscQuery, context);

  mscQueryCounter++;

  auto untyped = UntypedFnSignature::get(context,
      /* ID */ ID(),
      /* name */ UniqueString::get(context, "foo"),
      /* isMethod */ false,
      /* isTypeConstructor */ false,
      /* isCompilerGenerated */ true,
      /* throws */ false,
      /* idTag */ uast::asttags::AstTag::Function,
      /* kind */ uast::Function::Kind::PROC,
      /* formals */ { UntypedFnSignature::FormalDetail {
        UniqueString::get(context, "x"),
        UntypedFnSignature::DK_NO_DEFAULT,
        /* decl */ nullptr,
      }},
      /* whereClause */ nullptr
  );

  auto typed = TypedFnSignature::get(context, untyped,
      /* formalTypes */ { QualifiedType(QualifiedType::CONST_IN, IntType::get(context, 32)) },
      TypedFnSignature::WHERE_NONE,
      /* needsInstantiation */ false,
      /* instantiatedFrom */ nullptr,
      /* parentFn */ nullptr,
      /* instantiatedFormals */ Bitmap{},
      {});

  auto ci = CallInfo(
      /* name */ UniqueString::get(context, "foo"),
      /* calledType */ QualifiedType(),
      /* isMethodCall */ false,
      /* hasQuestionArg */ false,
      /* isParenless */ false,
      /* actuals */ {
        CallInfoActual {
          QualifiedType(QualifiedType::VAR, IntType::get(context, 32)),
          UniqueString(),
        }
      });

  auto faMap = FormalActualMap(typed, ci);
  auto promotedFormals = SubstitutionsMap();

  assert(faMap.isValid());

  ResolutionContext rc(context);
  auto msc = MostSpecificCandidate::fromTypedFnSignature(&rc, typed, std::move(faMap), nullptr, nullptr, std::move(promotedFormals));

  return QUERY_END(msc);
}

static int usesMscQueryCounter = 0;

static bool const& usesMscQuery(Context* context) {
  QUERY_BEGIN(usesMscQuery, context);

  usesMscQueryCounter++;
  std::ignore = mscQuery(context);

  return QUERY_END(true);
}

static int mscsQueryCounter = 0;

static MostSpecificCandidates const& mscsQuery(Context* context) {
  QUERY_BEGIN_INPUT(mscsQuery, context);

  mscsQueryCounter++;
  auto& mostSpecific = mscQuery(context);
  auto res = MostSpecificCandidates::getOnly(mostSpecific);

  return QUERY_END(res);
}

static int usesMscsQueryCounter = 0;

static bool const& usesMscsQuery(Context* context) {
  QUERY_BEGIN(usesMscsQuery, context);

  usesMscsQueryCounter++;
  std::ignore = mscsQuery(context);

  return QUERY_END(true);
}

static void test1() {
  Context ctx;
  Context* context = &ctx;

  assert(usesMscQueryCounter == 0);
  assert(mscQueryCounter == 0);

  std::ignore = usesMscQuery(context);
  assert(usesMscQueryCounter == 1);
  assert(mscQueryCounter == 1);

  std::ignore = usesMscQuery(context);
  assert(usesMscQueryCounter == 1);
  assert(mscQueryCounter == 1);

  ctx.advanceToNextRevision(false);

  std::ignore = usesMscQuery(context);
  assert(usesMscQueryCounter == 1);
  assert(mscQueryCounter == 2);
}

static void test2() {
  Context ctx;
  Context* context = &ctx;

  assert(usesMscsQueryCounter == 0);
  assert(mscsQueryCounter == 0);

  std::ignore = usesMscsQuery(context);
  assert(usesMscsQueryCounter == 1);
  assert(mscsQueryCounter == 1);

  std::ignore = usesMscsQuery(context);
  assert(usesMscsQueryCounter == 1);
  assert(mscsQueryCounter == 1);

  ctx.advanceToNextRevision(false);

  std::ignore = usesMscsQuery(context);
  assert(usesMscsQueryCounter == 1);
  assert(mscsQueryCounter == 2);
}

int main() {
  test1();
  test2();
}
