/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

int main() {
  // testHelloWorld();
  testSerialize();
  testDeserialize();

  return 0;
}

