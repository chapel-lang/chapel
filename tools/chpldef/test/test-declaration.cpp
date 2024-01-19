/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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

  /** Loop through each mention and convert the location range into a series
      of mouse clicks spanning the entire range. A mouse click is represented
      as a 'Position' in a range. Send that position to the server as the
      argument for a 'Declaration' request. */
  for (auto& m : mentions) {
    if (!m.isValid) continue;

    auto& range = m.source.range;
    auto& start = range.start;
    auto& end = range.end;

    if (m.tag == chpl::uast::asttags::Identifier ||
        m.tag == chpl::uast::asttags::Dot) {

      for (auto line = start.line; line <= end.line; line++) {
        CHPL_ASSERT(line >= 0 && line < lineLengths.size());

        auto lineLength = lineLengths[line];
        auto startChar = line == start.line ? start.character : 0;
        auto endChar = line == end.line ? end.character : lineLength;

        for (auto c = startChar; c <= endChar; c++) {
          Position cursor(line, c);

          client.dbg() << m.toString();
          client.dbg() << " w/ cursor (" << cursor.line << ":";
          client.dbg() << cursor.character << ")" << std::endl;

          auto targetLoc = client.sendDeclaration(uri, cursor);
          assert(targetLoc);
          assert(targetLoc == m.target);
        }
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
  x3.f;
  x3.
     f;

  x1 = 8;
  x2 = x1 * 2;

  proc foo() {}

  // TODO: 'C' does not have an entry in the ResolutionResult for 'bar'.
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
