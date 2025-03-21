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
#include "chpl/uast/all-uast.h"

#include <functional>

//
// These tests exist to check compilation success of certain library features
//

// TODO: Lock this test in when we feel like the resolver is ready.
/*
static void testHelloWorld() {
  auto config = getConfigWithHome();
  Context ctx(config);
  Context* context = &ctx;
  ErrorGuard guard(context);
  setupModuleSearchPaths(context, false, false, {}, {});

  std::string program = R"""(
    writeln("Hello, world!");
    )""";

  auto m = parseModule(context, std::move(program));
  std::ignore = resolveModule(context, m->id());

  assert(guard.numErrors() == 0);
}
*/

static void testSerialize() {
  auto config = getConfigWithHome();
  Context ctx(config);
  Context* context = &ctx;
  ErrorGuard guard(context);
  setupModuleSearchPaths(context, false, false, {}, {});

  std::string program = R"""(
    use IO;

    record R {
      var x : int;
    }
    class C {
      var x : int;
    }

    proc main() {
      var writer : fileWriter(false);
      var r : R;
      r.serialize(writer, writer.serializer);

      var o = new owned C(5);
      o.serialize(writer, writer.serializer);
      var s = new shared C(5);
      s.serialize(writer, writer.serializer);
      var u = new unmanaged C(5);
      u.serialize(writer, writer.serializer);
    }
    )""";

  auto m = parseModule(context, std::move(program));
  std::ignore = resolveModule(context, m->id());

  assert(guard.numErrors() == 0);
}

static void testDeserialize() {
  auto config = getConfigWithHome();
  Context ctx(config);
  Context* context = &ctx;
  ErrorGuard guard(context);
  setupModuleSearchPaths(context, false, false, {}, {});

  std::string program = R"""(
    use IO;

    record R {
      var x : int;
    }
    class C {
      var x : int;
    }

    proc main() {
      var reader : fileReader(false);
      var r : R;
      r.deserialize(reader, reader.deserializer);

      var o = new owned C(5);
      o.deserialize(reader, reader.deserializer);
      var s = new shared C(5);
      s.deserialize(reader, reader.deserializer);
      var u = new unmanaged C(5);
      u.deserialize(reader, reader.deserializer);
    }
    )""";

  auto m = parseModule(context, std::move(program));
  std::ignore = resolveModule(context, m->id());

  assert(guard.numErrors() == 0);
}

static void testStringBytes() {
  auto config = getConfigWithHome();
  Context ctx(config);
  Context* context = &ctx;
  ErrorGuard guard(context);
  setupModuleSearchPaths(context, false, false, {}, {});

  std::string program = R"""(
    proc test() {
      var x : string;
      var y = x.byteIndices;

      var a : bytes;
      var b = a.byteIndices;
    }

    test();
    )""";

  auto m = parseModule(context, std::move(program));
  auto rr = resolveModule(context, m->id());

  ResolutionContext rcval(context);
  auto testCall = m->stmt(1)->toFnCall();
  auto testSig = rr.byAst(testCall).mostSpecific().only().fn();
  auto testFn = resolveFunction(&rcval, testSig, rr.byAst(testCall).poiScope());

  {
    auto x = findVariable(m, "x");
    auto xRes = testFn->byAst(x);
    assert(xRes.type().type()->isStringType());
    assert(xRes.associatedActions().size() == 1);
    auto action = xRes.associatedActions()[0];
    assert(action.action() == AssociatedAction::DEFAULT_INIT);
    assert(action.fn()->id().str() == "String._string.init");

    auto y = findVariable(m, "y");
    std::stringstream ss;
    testFn->byAst(y).type().type()->stringify(ss, chpl::StringifyKind::CHPL_SYNTAX);
    assert(ss.str() == "range(int(64), both, one)");
  }

  {
    auto a = findVariable(m, "a");
    auto aRes = testFn->byAst(a);
    assert(aRes.type().type()->isBytesType());
    assert(aRes.associatedActions().size() == 1);
    auto action = aRes.associatedActions()[0];
    assert(action.action() == AssociatedAction::DEFAULT_INIT);
    assert(action.fn()->id().str() == "Bytes._bytes.init");

    auto b = findVariable(m, "b");
    std::stringstream ss;
    testFn->byAst(b).type().type()->stringify(ss, chpl::StringifyKind::CHPL_SYNTAX);
    assert(ss.str() == "range(int(64), both, one)");
  }
}

int main() {
  // testHelloWorld();
  testSerialize();
  testDeserialize();
  testStringBytes();

  return 0;
}

