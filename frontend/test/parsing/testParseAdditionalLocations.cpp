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

#include "test-parsing.h"

#include "chpl/parsing/Parser.h"
#include "chpl/framework/Context.h"
#include "chpl/parsing/parsing-queries.h"
#include "chpl/uast/all-uast.h"
#include <array>

static void assertDotFieldAtLoc(Context* ctx, const Dot* dot,
                                int firstLine,
                                int firstColumn) {
  auto loc = parsing::locateDotFieldWithAst(ctx, dot);
  assert(!loc.isEmpty());
  assert(firstLine == loc.firstLine());
  assert(firstColumn == loc.firstColumn());
  int lastLine = firstLine;
  int lastColumn = firstColumn + dot->field().length();
  assert(lastLine == loc.lastLine());
  assert(lastColumn == loc.lastColumn());
}

static void test0() {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  auto path = TEST_NAME_FROM_FN_NAME(ctx);
  std::string contents =
    "x1.field;\n"   // 1:4
    "x2. field;\n"  // 2:5
    "x3.\n"         //
    "   field;\n"   // 4:4
    "x4\n"          //
    ".field;\n";    // 6:2

  setFileText(ctx, path, contents);
  auto& br = parseAndReportErrors(ctx, path);
  assert(!guard.realizeErrors());

  auto mod = br.singleModule();
  assert(mod && mod->numStmts() == 4);
  auto d1 = mod->stmt(0)->toDot();
  auto d2 = mod->stmt(1)->toDot();
  auto d3 = mod->stmt(2)->toDot();
  auto d4 = mod->stmt(3)->toDot();
  assert(d1 && d2 && d3 && d4);
  assertDotFieldAtLoc(ctx, d1, 1, 4);
  assertDotFieldAtLoc(ctx, d2, 2, 5);
  assertDotFieldAtLoc(ctx, d3, 4, 4);
  assertDotFieldAtLoc(ctx, d4, 6, 2);
}

struct pos {
  int line;
  int column;
  pos(int line, int column) : line(line), column(column) {}
 ~pos() = default;
};

static void
assertDeclNameAtPos(Context* ctx, const NamedDecl* nd, const pos& p) {
  auto loc = parsing::locateDeclNameWithAst(ctx, nd);
  assert(!loc.isEmpty());
  assert(p.line == loc.firstLine());
  assert(p.column == loc.firstColumn());
  int lastLine = p.line;
  int lastColumn = p.column + nd->name().length();
  assert(lastLine == loc.lastLine());
  assert(lastColumn == loc.lastColumn());
}

static void test1() {
  Context context;
  Context* ctx = &context;
  ErrorGuard guard(ctx);

  auto path = TEST_NAME_FROM_FN_NAME(ctx);
  std::string contents =
    // proc
    "proc foo() {}\n"           // 1:6
    "proc    foo() {}\n"        // 2:9
    "proc foo () {}\n"          // 3:6
    "proc\n"                    //
    "    foo() {}\n"            // 5:5
    "proc   foo {}\n"           // 6:8
    // iter
    "iter foo() {}\n"           // 7:6
    "iter    bar() {}\n"        // 8:9
    "iter foo () {}\n"          // 9:6
    "iter\n"                    //
    "    foo() {}\n"            // 11:5
    // class
    "class foo : bar {}\n"      // 12:7
    // record
    "record foo {}\n"           // 13:8
    // union
    "union foo {}\n"            // 14:7
    // enum
    "enum foo { bar }\n";       // 15:6

  setFileText(ctx, path, contents);
  auto& br = parseAndReportErrors(ctx, path);
  assert(!guard.realizeErrors());

  static const std::array<pos, 13> positions = {{
    pos(1,6), pos(2,9), pos(3,6), pos(5,5), pos(6,8), pos(7,6), pos(8,9),
    pos(9,6), pos(11,5), pos(12,7), pos(13,8), pos(14,7), pos(15,6)
  }};

  auto mod = br.singleModule();
  assert(mod && mod->numStmts() == 13);
  assert(positions.size() == (size_t) mod->numStmts());

  for (int i = 0; i < mod->numStmts(); i++) {
    auto nd = mod->stmt(i)->toNamedDecl();
    assert(nd);
    assertDeclNameAtPos(ctx, nd, positions[i]);
  }
}

int main() {
  test0();
  test1();
  return 0;
}
