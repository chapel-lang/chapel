/*
 * Copyright 2024 Hewlett Packard Enterprise Development LP
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

#include "chpl/framework/Context.h"
#include "test-common.h"
#include "test-resolution.h"

using namespace chpl;
using namespace resolution;

static void testStandaloneWithoutSerial(Context* context) {
  // Test that if we fail to resolve a call to an iterator, but there's
  // a tagged overload (e.g., a leader or a follower), that we find the tagged
  // overload instead.
  ADVANCE_PRESERVING_STANDARD_MODULES_(context);
  ErrorGuard guard(context);

  std::string prog =
    R"""(
      iter foo(param tag) where tag == iterKind.standalone {
        yield 1;
        yield 2;
        yield 3;
      }
      var loop = foo();
      forall i in loop {}
      forall j in foo() {}
    )""";

  auto vars = resolveTypesOfVariables(context, prog, {"loop", "i", "j"});

  CHPL_ASSERT(vars.at("loop").type());
  CHPL_ASSERT(vars.at("loop").type()->isIteratorType());
  CHPL_ASSERT(vars.at("i").type());
  CHPL_ASSERT(vars.at("i").type()->isIntType());
  CHPL_ASSERT(vars.at("i").type()->toIntType()->isDefaultWidth());
  CHPL_ASSERT(vars.at("i") == vars.at("j"));
}

static void testLeaderFollowerWithoutSerial(Context* context) {
  // Test that if we fail to resolve a call to a non-iterator, but there's
  // a tagged overload (e.g., a leader or a follower), that we _don't_ find
  // the tagged overload, since procs can't be tagged.
  ADVANCE_PRESERVING_STANDARD_MODULES_(context);

  std::string prog =
    R"""(
      proc foo(param tag) where tag == iterKind.standalone {
        yield 1;
        yield 2;
        yield 3;
      }
      var loop = foo();
      forall i in loop {}
      forall j in foo() {}
    )""";

  auto vars = resolveTypesOfVariables(context, prog, {"loop", "i", "j"});
  for (auto var : vars) {
    CHPL_ASSERT(var.second.isUnknownOrErroneous());
  }
}

static void testIncompatibleYieldTypes(Context* context) {
  // test that various incompatible iterator type cases are caught (or not).
  ErrorGuard guard(context);
  auto assertIncompatibleYieldError = [&]() {
    assert(guard.numErrors() >= 1);
    for (auto& error : guard.errors()) {
      if (error->type() == ErrorType::IncompatibleYieldTypes) {
        guard.realizeErrors();
        return;
      }
    }

    assert(false && "IncompatibleYieldTypes error not found");
  };

  {
    ADVANCE_PRESERVING_STANDARD_MODULES_(context);
    std::string prog =
      R"""(
      iter i1() { yield 0.0; }
      iter i1(param tag: iterKind) where tag == iterKind.standalone { yield 0; }
      var l = i1();
      )""";

    auto vars = resolveTypesOfVariables(context, prog, {"l"});
    assertIncompatibleYieldError();
  }

  {
    ADVANCE_PRESERVING_STANDARD_MODULES_(context);
    std::string prog =
      R"""(
      iter i1() { yield 0.0; }
      iter i1(param tag: iterKind) where tag == iterKind.leader { yield (0,0); }
      iter i1(param tag: iterKind, followThis) where tag == iterKind.follower { yield 0; }
      var l = i1();
      )""";

    auto vars = resolveTypesOfVariables(context, prog, {"l"});
    assertIncompatibleYieldError();
  }

  {
    ADVANCE_PRESERVING_STANDARD_MODULES_(context);
    std::string prog =
      R"""(
      iter i1(param tag: iterKind) where tag == iterKind.standalone { yield 0; }
      iter i1(param tag: iterKind) where tag == iterKind.leader { yield (0,0); }
      iter i1(param tag: iterKind, followThis) where tag == iterKind.follower { yield 0.0; }
      var l = i1();
      )""";

    auto vars = resolveTypesOfVariables(context, prog, {"l"});
    assertIncompatibleYieldError();
  }

  {
    // No leader means we can't invoke a follower, so the follower type is not
    // considered and no error is emitted.

    ADVANCE_PRESERVING_STANDARD_MODULES_(context);
    std::string prog =
      R"""(
      iter i1() { yield 0.0; }
      iter i1(param tag: iterKind, followThis) where tag == iterKind.follower { yield 0; }
      var l = i1();
      )""";

    auto vars = resolveTypesOfVariables(context, prog, {"l"});
    assert(guard.realizeErrors() == 0);
  }

  {
    // No follower means we can't check the follower's type.

    ADVANCE_PRESERVING_STANDARD_MODULES_(context);
    std::string prog =
      R"""(
      iter i1() { yield 0.0; }
      iter i1(param tag: iterKind) where tag == iterKind.leader { yield (0,0); }
      var l = i1();
      )""";

    auto vars = resolveTypesOfVariables(context, prog, {"l"});
    assert(guard.realizeErrors() == 0);
  }
}

static void testIteratorFnPoi(Context* context) {
  // Tests generic iterator producers + PoI

  ADVANCE_PRESERVING_STANDARD_MODULES_(context);
  ErrorGuard guard(context);
  std::string prog =
    R"""(
    module PoiIterator {
      iter myIter(arg) {
        yield computeReturn(arg);
      }
      iter myIter(arg, param tag) where tag == iterKind.standalone {
        yield computeReturn(arg);
      }

      {
        proc computeReturn(arg: int) {
          return arg + 1;
        }

        forall i1 in myIter(13) {}
        for j1 in myIter(13) {}
      }

      {
        proc computeReturn(arg: int) {
          return arg == 1;
        }

        forall i2 in myIter(13) {}
        for j2 in myIter(13) {}
      }

      module Library {
        import super.{myIter};

        proc computeReturn(arg) {
          return "hello";
        }

        proc buildIterator() {
          return myIter(13);
        }
      }


      {
        use Library;

        // This is more specific than library's computeReturn, but was not in scope
        // when buildIterator was called.
        proc computeReturn(arg: int) {
          return arg == 1;
        }

        forall i3 in buildIterator() {}
        for j3 in buildIterator() {}
      }
    }
    )""";

  auto vars = resolveTypesOfVariables(context, prog, {"i1", "j1", "i2", "j2", "i3", "j3"});

  assert(vars.at("i1").type()->isIntType());
  assert(vars.at("j1").type()->isIntType());
  assert(vars.at("i2").type()->isBoolType());
  assert(vars.at("j2").type()->isBoolType());
  assert(vars.at("i3").type()->isStringType());
  assert(vars.at("j3").type()->isStringType());
}

static void testIteratorFnPoiIgnoresIterationScope(Context* context) {
  // Tests generic iterator producers + PoI

  ADVANCE_PRESERVING_STANDARD_MODULES_(context);
  ErrorGuard guard(context);
  std::string prog =
    R"""(
    module PoiIterator {
      iter myIter(arg) {
        yield computeReturn(arg);
      }
      iter myIter(arg, param tag) where tag == iterKind.standalone {
        yield computeReturn(arg);
      }

      module Library {
        import super.{myIter};

        proc buildIterator() {
          return myIter(13);
        }
      }


      {
        use Library;

        // This is available in scope when iterating, but is not available
        // at the time when the iterator is instantiated, so iteration doesn't
        // succeed.
        proc computeReturn(arg: int) {
          return arg == 1;
        }

        forall i in buildIterator() {}
        for j in buildIterator() {}
      }
    }
    )""";

  auto vars = resolveTypesOfVariables(context, prog, {"i", "j"});

  assert(vars.at("i").isUnknownOrErroneous());
  assert(vars.at("j").isUnknownOrErroneous());
  assert(guard.realizeErrors() >= 1);
}

static void testLoopExprIteratorPoi(Context* context) {
  ADVANCE_PRESERVING_STANDARD_MODULES_(context);
  ErrorGuard guard(context);

  // Same as test31, but with loop expressions (which require leader/follower).

  auto prog =
    R"""(
    module PoiLoopExpr {
      record iterableRecord {}

      proc myIter(arg) {
        return [i in arg] (i, i);
      }

      {
        iter iterableRecord.these() {
          yield 1;
        }

        iter iterableRecord.these(param tag) where tag == iterKind.leader {
          yield (1,);
        }

        iter iterableRecord.these(param tag, followThis) where tag == iterKind.follower {
          yield 1;
        }

        forall i1 in myIter(new iterableRecord()) {}
        for j1 in myIter(new iterableRecord()) {}
      }

      {
        iter iterableRecord.these() {
          yield 1.0;
        }

        iter iterableRecord.these(param tag) where tag == iterKind.leader {
          yield (1,);
        }

        iter iterableRecord.these(param tag, followThis) where tag == iterKind.follower {
          yield 1.0;
        }

        forall i2 in myIter(new iterableRecord()) {}
        for j2 in myIter(new iterableRecord()) {}
      }

      module Library {
        import super.{iterableRecord, myIter};

        iter iterableRecord.these() {
          yield true;
        }

        iter iterableRecord.these(param tag) where tag == iterKind.leader {
          yield (1,);
        }

        iter iterableRecord.these(param tag, followThis) where tag == iterKind.follower {
          yield true;
        }

        proc buildIterator() {
          return myIter(new iterableRecord());
        }
      }

      {
        use Library;

        forall i3 in buildIterator() {}
        for j3 in buildIterator() {}
      }
    }
    )""";

  auto vars = resolveTypesOfVariables(context, prog, {"i1", "j1", "i2", "j2", "i3", "j3"});

  assert(vars.at("i1").type()->isTupleType());
  assert(vars.at("i1").type()->toTupleType()->numElements() == 2);
  assert(vars.at("i1").type()->toTupleType()->elementType(0).type()->isIntType());
  assert(vars.at("i1").type()->toTupleType()->elementType(1).type()->isIntType());

  assert(vars.at("j1").type()->isTupleType());
  assert(vars.at("j1").type()->toTupleType()->numElements() == 2);
  assert(vars.at("j1").type()->toTupleType()->elementType(0).type()->isIntType());
  assert(vars.at("j1").type()->toTupleType()->elementType(1).type()->isIntType());

  assert(vars.at("i2").type()->isTupleType());
  assert(vars.at("i2").type()->toTupleType()->numElements() == 2);
  assert(vars.at("i2").type()->toTupleType()->elementType(0).type()->isRealType());
  assert(vars.at("i2").type()->toTupleType()->elementType(1).type()->isRealType());

  assert(vars.at("j2").type()->isTupleType());
  assert(vars.at("j2").type()->toTupleType()->numElements() == 2);
  assert(vars.at("j2").type()->toTupleType()->elementType(0).type()->isRealType());
  assert(vars.at("j2").type()->toTupleType()->elementType(1).type()->isRealType());

  assert(vars.at("i3").type()->isTupleType());
  assert(vars.at("i3").type()->toTupleType()->numElements() == 2);
  assert(vars.at("i3").type()->toTupleType()->elementType(0).type()->isBoolType());
  assert(vars.at("i3").type()->toTupleType()->elementType(1).type()->isBoolType());

  assert(vars.at("j3").type()->isTupleType());
  assert(vars.at("j3").type()->toTupleType()->numElements() == 2);
  assert(vars.at("j3").type()->toTupleType()->elementType(0).type()->isBoolType());
  assert(vars.at("j3").type()->toTupleType()->elementType(1).type()->isBoolType());
}

static void test34(Context* context) {
  // tests that PoI is not used to find new iterator overloads

  ADVANCE_PRESERVING_STANDARD_MODULES_(context);
  ErrorGuard guard(context);
  std::string prog =
    R"""(
    iter myIter(arg) {
      yield 1;
    }

    {
      iter myIter(arg, param tag) where tag == iterKind.standalone {
        yield "hello";
      }

      [i in myIter(true)] {
        // writeln(i); // should print '1', even though new overload is present
      }
    }
    )""";

  auto vars = resolveTypesOfVariables(context, prog, {"i"});

  assert(vars.at("i").type());
  assert(vars.at("i").type()->isIntType());
}

int main() {
  auto ctx = buildStdContext();
  testStandaloneWithoutSerial(ctx.get());
  testLeaderFollowerWithoutSerial(ctx.get());
  testIncompatibleYieldTypes(ctx.get());
  testIteratorFnPoi(ctx.get());
  testIteratorFnPoiIgnoresIterationScope(ctx.get());
  testLoopExprIteratorPoi(ctx.get());
  test34(ctx.get());
}
