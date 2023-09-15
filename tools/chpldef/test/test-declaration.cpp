/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#include "./TestClient.h"

static void testDeclaration(const std::string& uri, const std::string& text) {
  TestClient client;

  client.advanceServerToReady();

  /** Send 'DidOpen' to communicate text open in the editor. */
  client.sendDidOpen(uri, text);

  /** Collect locations we are interested in. */
  auto lineLengths = TestClient::collectLineLengthsInSource(text);
  auto mentions = TestClient::collectMentions(uri, text);

  for (auto& m : mentions) {
    auto& range = m.source.range;
    auto& start = range.start;
    auto& end = range.end;

    // TODO: Fuzz characters to the left and the right of the identifier.
    if (m.tag == chpl::uast::asttags::Identifier) {
      CHPL_ASSERT(start.line == end.line);
      const auto line = start.line;
      for (uint64_t c = start.character; c <= end.character; c++) {
        Position cursor(line, c);
        client.dbg() << m.toString();
        client.dbg() << " w/ cursor (" << cursor.line << ":";
        client.dbg() << cursor.character << ")" << std::endl;

        auto targetLoc = client.sendDeclaration(uri, cursor);
        CHPL_ASSERT(targetLoc == m.target);
      }
    } else {
      CHPLDEF_TODO();
    }
  }
}

static void test0(void) {
  const auto uri = "test0.chpl";
  const auto text = R"""(
  class C {
      var f = 0;
  }

  var x1 = 0;
  var x2 = 0.0;
  var x3 = new C();

  x1;
  x2;
  // x3.f; TODO: Dot targets. // BUG: Dot hugs right.

  x1 = 8;
  x2 = x1 * 2; // BUG: Assigned expression not considered.

  proc foo() {}
  proc bar(a: owned C?, b: int=0) { a; b; }

  foo();
  bar(x3, x1);
  )""";

  testDeclaration(uri, text);
}

static void test1(void) {
  const auto uri = "test1.chpl";
  const auto text = R"""(
  class C {}

  proc foo() {}
  proc foo(a: int) {} // BUG: Formal not considered...
  proc foo(b: real, c: owned C?) {}
  proc foo(d: owned C?, e: int=0) {}

  // TODO: How to map calls to procedures with default values?
  // Short of using the resolver itself...
  foo();
  foo(8);
  foo(16.0, new C());
  foo(new C());
  )""";

  testDeclaration(uri, text);
}

int main(int argc, char** argv) {
  test0();
  test1();
  return 0;
}
