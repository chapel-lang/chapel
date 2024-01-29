#include "chpl/resolution/resolution-types.h"
#include "chpl/framework/query-impl.h"
#include "chpl/types/all-types.h"

using namespace chpl;
using namespace resolution;
using namespace uast;
using namespace types;

static int mscQueryCounter = 0;
static const FormalActualMap* oldFaMapPtr = nullptr;

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
        /* hasDefaultValue */ false,
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
      /* instantiatedFormals */ Bitmap{});

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

  assert(faMap.isValid());

  auto msc = MostSpecificCandidate::fromTypedFnSignature(context, typed, std::move(faMap));

  // The test results are only testing what we want (deep == comparison) if
  // the enclosed pointers are not shallowly equal. Make sure a new owned<FAMap>
  // was allocated.
  assert(&msc.formalActualMap() != oldFaMapPtr);
  oldFaMapPtr = &msc.formalActualMap();

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
